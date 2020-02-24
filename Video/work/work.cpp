#include "opencv_camera_display.h"

#include <VX/vx.h>

////////
// Useful macros for OpenVX error checking:
//   ERROR_CHECK_STATUS     - check status is VX_SUCCESS
//   ERROR_CHECK_OBJECT     - check if the object creation is successful
#define ERROR_CHECK_STATUS( status ) { \
        vx_status status_ = (status); \
        if(status_ != VX_SUCCESS) { \
            printf("ERROR: failed with status = (%d) at " __FILE__ "#%d\n", status_, __LINE__); \
            exit(1); \
        } \
    }

#define ERROR_CHECK_OBJECT( obj ) { \
        vx_status status_ = vxGetStatus((vx_reference)(obj)); \
        if(status_ != VX_SUCCESS) { \
            printf("ERROR: failed with status = (%d) at " __FILE__ "#%d\n", status_, __LINE__); \
            exit(1); \
        } \
    }

////////
// User kernel should have a unique enumerations and name for user kernel:
//   USER_LIBRARY_EXAMPLE      - library ID for user kernels in this example
//   USER_KERNEL_MEDIAN_BLUR   - enumeration for "app.userkernels.median_blur" kernel
//
// TODO:********
//   1. Define USER_LIBRARY_EXAMPLE
//   2. Define USER_KERNEL_MEDIAN_BLUR using VX_KERNEL_BASE() macro
enum user_library_e
{
    USER_LIBRARY_EXAMPLE        = 1,
};
enum user_kernel_e
{
    USER_KERNEL_MEDIAN_BLUR     = VX_KERNEL_BASE( VX_ID_DEFAULT, USER_LIBRARY_EXAMPLE ) + 0x001,
};

int main( int argc, char * argv[] )
{
    // Get default video sequence when nothing is specified on command-line and
    // instantiate OpenCV GUI module for reading input RGB images and displaying
    // the image with OpenVX results
    const char * video_sequence = argv[1];
    CGuiModule gui( video_sequence );

    // Try grab first video frame from the sequence using cv::VideoCapture
    // and check if video frame is available
    if( !gui.Grab() )
    {
        printf( "ERROR: input has no video\n" );
        return 1;
    }

    ////////
    // Set the application configuration parameters. Note that input video
    // sequence is an 8-bit RGB image with dimensions given by gui.GetWidth()
    // and gui.GetHeight(). The parameters for the median filter are:
    //   ksize                   - median filter kernel size
    vx_uint32  width     = gui.GetWidth();
    vx_uint32  height    = gui.GetHeight();
    vx_int32   ksize     = 7;

    ////////
    // Create the OpenVX context and make sure returned context is valid and
    // register the log_callback to receive messages from OpenVX framework.
    vx_context context = vxCreateContext();
    ERROR_CHECK_OBJECT( context );
    vxRegisterLogCallback( context, log_callback, vx_false_e );

    ////////
    // Register user kernels with the context.
    //
    // TODO:********
    //   1. Register user kernel with context by calling your implementation of "registerUserKernel()".
    ERROR_CHECK_STATUS( registerUserKernel( context ) );

    ////////
    // Create OpenVX image object for input RGB image and median filter output image.
    vx_image input_rgb_image       = vxCreateImage( context, width, height, VX_DF_IMAGE_RGB );
    vx_image output_filtered_image = vxCreateImage( context, width, height, VX_DF_IMAGE_U8 );
    ERROR_CHECK_OBJECT( input_rgb_image );
    ERROR_CHECK_OBJECT( output_filtered_image );

    ////////********
    // Create graph object and intermediate image objects.
    // Given that input is an RGB image, it is best to extract a gray image
    // from RGB image, which requires two steps:
    //   - perform RGB to IYUV color conversion
    //   - extract Y channel from IYUV image
    // This requires two intermediate OpenVX image objects. Since you don't
    // need to access these objects from the application, they can be virtual
    // objects that can be created using the vxCreateVirtualImage API.
    vx_graph graph      = vxCreateGraph( context );
    ERROR_CHECK_OBJECT( graph );
    vx_image yuv_image  = vxCreateVirtualImage( graph, width, height, VX_DF_IMAGE_IYUV );
    vx_image luma_image = vxCreateVirtualImage( graph, width, height, VX_DF_IMAGE_U8 );
    ERROR_CHECK_OBJECT( yuv_image );
    ERROR_CHECK_OBJECT( luma_image );

    ////////********
    // Now all the objects have been created for building the graph
    // that converts RGB into luma image and then performs median blur
    // using user kernel USER_KERNEL_MEDIAN_BLUR.
    // Then use vxVerifyGraph API for initialization.
    //
    // TODO:********
    //   1. Use userMedianBlurNode function to add "median_blur" node.
    vx_node nodes[] =
    {
        vxColorConvertNode(   graph, input_rgb_image, yuv_image ),
        vxChannelExtractNode( graph, yuv_image, VX_CHANNEL_Y, luma_image ),
        userMedianBlurNode(   graph, luma_image, output_filtered_image, ksize )
    };
    for( vx_size i = 0; i < sizeof( nodes ) / sizeof( nodes[0] ); i++ )
    {
        ERROR_CHECK_OBJECT( nodes[i] );
        ERROR_CHECK_STATUS( vxReleaseNode( &nodes[i] ) );
    }
    ERROR_CHECK_STATUS( vxReleaseImage( &yuv_image ) );
    ERROR_CHECK_STATUS( vxReleaseImage( &luma_image ) );
    ERROR_CHECK_STATUS( vxVerifyGraph( graph ) );

    ////////
    // Process the video sequence frame by frame until the end of sequence or aborted.
    for( int frame_index = 0; !gui.AbortRequested(); frame_index++ )
    {
        ////////
        // Copy input RGB frame from OpenCV to OpenVX. In order to do this,
        // you need to use vxCopyImagePatch API.
        // See "VX/vx_api.h" for the description of these APIs.
        vx_rectangle_t cv_rgb_image_region;
        cv_rgb_image_region.start_x    = 0;
        cv_rgb_image_region.start_y    = 0;
        cv_rgb_image_region.end_x      = width;
        cv_rgb_image_region.end_y      = height;
        vx_imagepatch_addressing_t cv_rgb_image_layout;
        cv_rgb_image_layout.stride_x   = 3;
        cv_rgb_image_layout.stride_y   = gui.GetStride();
        vx_uint8 * cv_rgb_image_buffer = gui.GetBuffer();
        ERROR_CHECK_STATUS( vxCopyImagePatch( input_rgb_image, &cv_rgb_image_region, 0,
                                              &cv_rgb_image_layout, cv_rgb_image_buffer,
                                              VX_WRITE_ONLY, VX_MEMORY_TYPE_HOST ) );

        ////////
        // Now that input RGB image is ready, just run the graph.
        ERROR_CHECK_STATUS( vxProcessGraph( graph ) );

        ////////
        // Display the output filtered image.
        vx_rectangle_t rect = { 0, 0, width, height };
        vx_map_id map_id;
        vx_imagepatch_addressing_t addr;
        void * ptr;
        ERROR_CHECK_STATUS( vxMapImagePatch( output_filtered_image, &rect, 0, &map_id, &addr, &ptr,
                                             VX_READ_ONLY, VX_MEMORY_TYPE_HOST, VX_NOGAP_X ) );
        cv::Mat mat( height, width, CV_8U, ptr, addr.stride_y );
#if ENABLE_DISPLAY
        cv::imshow( "MedianBlur", mat );
#endif
        ERROR_CHECK_STATUS( vxUnmapImagePatch( output_filtered_image, map_id ) );

        ////////
        // Display the results and grab the next input RGB frame for the next iteration.
        char text[128];
        sprintf( text, "Keyboard ESC/Q-Quit SPACE-Pause [FRAME %d] [ksize %d]", frame_index, ksize );
        gui.DrawText( 0, 16, text );
        gui.Show();
        if( !gui.Grab() )
        {
            // Terminate the processing loop if the end of sequence is detected.
            gui.WaitForKey();
            break;
        }
    }

    ////////********
    // Query graph performance using VX_GRAPH_PERFORMANCE and print timing
    // in milliseconds. Note that time units of vx_perf_t fields are nanoseconds.
    vx_perf_t perf = { 0 };
    ERROR_CHECK_STATUS( vxQueryGraph( graph, VX_GRAPH_PERFORMANCE, &perf, sizeof( perf ) ) );
    printf( "GraphName NumFrames Avg(ms) Min(ms)\n"
            "Median    %9d %7.3f %7.3f\n",
            ( int )perf.num, ( float )perf.avg * 1e-6f, ( float )perf.min * 1e-6f );

    ////////********
    // To release an OpenVX object, you need to call vxRelease<Object> API which takes a pointer to the object.
    // If the release operation is successful, the OpenVX framework will reset the object to NULL.
    ERROR_CHECK_STATUS( vxReleaseGraph( &graph ) );
    ERROR_CHECK_STATUS( vxReleaseImage( &input_rgb_image ) );
    ERROR_CHECK_STATUS( vxReleaseImage( &output_filtered_image ) );
    ERROR_CHECK_STATUS( vxReleaseContext( &context ) );

    return 0;
}
