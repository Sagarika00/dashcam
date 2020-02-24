#include <vector>
#include <opencv2/opencv.hpp>
#include <VX/vx.h>
#include <VX/vx_api.h>

using namespace std;

void DrawBoundingBoxes(
        cv::Mat                 &inImg,         // Input image, can be input color images, or output binary image
        vector<vx_rectangle_t>  &componentBoxes,
        int                     scaleFactor)
{
    // Draw bounding box for detected moving objects
    for (int i = 0; i < componentBoxes.size(); i++)
    {
        cv::rectangle(inImg, cv::Point(componentBoxes[i].start_x * scaleFactor,
		    componentBoxes[i].start_y * scaleFactor),
		    cv::Point(componentBoxes[i].end_x * scaleFactor,
		    componentBoxes[i].end_y * scaleFactor), cv::Scalar(255, 255, 255));
    }
}