# Copyright (c) 2016 The Khronos Group Inc.
#
# Permission is hereby granted, free of charge, to any person obtaining a
# copy of this software and/or associated documentation files (the
# "Materials"), to deal in the Materials without restriction, including
# without limitation the rights to use, copy, modify, merge, publish,
# distribute, sublicense, and/or sell copies of the Materials, and to
# permit persons to whom the Materials are furnished to do so, subject to
# the following conditions:
#
# The above copyright notice and this permission notice shall be included
# in all copies or substantial portions of the Materials.
#
# THE MATERIALS ARE PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
# EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
# MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
# IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY
# CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT,
# TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE
# MATERIALS OR THE USE OR OTHER DEALINGS IN THE MATERIALS.

# Author: Radhakrishna Giduthuri (radha.giduthuri@ieee.org)
# 
# Supported cmake variables and CACHE entries:
#   OpenVX_SOURCE_DIR   -- OpenVX open source folder with CMakeLists.txt
#   OpenVX_LIBS         -- list of OpenVX libraries
#   OpenVX_LIBS_DIR     -- path to OpenVX libraries
#   OpenVX_INCLUDE_DIRS -- path to non-khronos OpenVX header files (optional)
#   ENABLE_OPENCL       -- flag to enable OpenCL with OpenVX source build (optional)
#   ENABLE_NN_AMD       -- flag to enable Neural Network extension with AMD ROCm (optional)
#   DISABLE_DISPLAY     -- flag to display OpenCV windows
#
# Here are few examples:
# * Build exerciese using an open source implementation with using OpenCL and NN
#     % cmake -DENABLE_NN_AMD=TRUE \
#             .../openvx_tutorial/tutorial_exercises
# * Build exerciese using an open source implementation without using OpenCL
#     % cmake -DOpenVX_SOURCE_DIR=amdovx-modules/deps/amdovx-core/openvx \
#             -DCMAKE_DISABLE_FIND_PACKAGE_OpenCL=TRUE \
#             .../openvx_tutorial/tutorial_exercises
# * Build exerciese using a 3rd-party OpenVX library
#     % cmake -DOpenVX_LIBS_DIR=<path-to-openvx-libraries> \
#             -DOpenVX_LIBS=<list-of-openvx-libraries> \
#             .../openvx_tutorial/tutorial_exercises

# cmake 
#-DOpenVX_SOURCE_DIR=amdovx-modules/deps/amdovx-core/openvx 
#-DOpenVX_LIBS="/opt/intel/openvino/openvx/lib/libiap_clplugin.so /opt/intel/openvino/openvx/lib/libiap_mkldnnplugin.so /opt/intel/openvino/openvx/lib/libiap_openvx_cpu.so /opt/intel/openvino/openvx/lib/libiap_openvx_cpu.so /opt/intel/openvino/openvx/lib/libiap_openvx_gpu.so /opt/intel/openvino/openvx/lib/libiap_openvx_mkldnn.so /opt/intel/openvino/openvx/lib/libiap_openvx.so /opt/intel/openvino/openvx/lib/libiap.so /opt/intel/openvino/openvx/lib/libiomp5.so /opt/intel/openvino/openvx/lib/libopenvx.so /opt/intel/openvino/openvx/lib/libtbbmalloc.so /opt/intel/openvino/openvx/lib/libtbb.so"
#-DOpenVX_LIBS_DIR=/opt/intel/openvino/openvx/lib 
#-DOpenVX_INCLUDE_DIRS=/opt/intel/openvino/openvx/include/ 
#-DOpenCV_INCLUDE_DIRS="/lib/x86_64-linux-gnu/ /usr/lib/x86_64-linux-gnu/ /usr/include/"
#../tutorial_exercises/

set(CMAKE_C_COMPILER /usr/bin/gcc)
set(CMAKE_CXX_COMPILER /usr/bin/g++)
cmake_minimum_required ( VERSION 3.10.2            )
project                ( tutorial_exercises     )

set(OpenCV_INCLUDE_DIRS /home/aswin/Documents/Courses/Udacity/Intel-Edge/Repository/opencv/include/)
# set(OpenVX_INCLUDE_DIRS /opt/intel/openvino/openvx/include)
set(OpenVX_INCLUDE_DIRS /home/aswin/Documents/Courses/Udacity/Intel-Edge/Repository/openvx_tutorial/tutorial_exercises/include)
set(OpenCV_LIBS_DIR /home/aswin/Documents/Courses/Udacity/Intel-Edge/Repository/opencv/build/lib)
set(OpenVX_LIBS_DIR /home/aswin/Documents/Courses/Udacity/Intel-Edge/Repository/OpenVX-sample-impl/out/LINUX/x86_64/release)

set(OpenVX_SOURCE_DIR /home/aswin/Documents/Courses/Udacity/Intel-Edge/Repository/OpenVX-sample-impl/)

set(
    OpenCV_LIBRARIES
    -lopencv_core
    -lopencv_calib3d
    -lopencv_flann
    -lopencv_highgui
    -lopencv_videoio
    -lopencv_video
    -lopencv_photo
    -lopencv_ml
    -lopencv_flann
    -lopencv_highgui
    -lopencv_objdetect
    -lopencv_imgproc
    -lopencv_stitching
    -lopencv_features2d
    -lopencv_dnn
    -lopencv_imgcodecs
    -lopencv_gapi
    )

set(
    OpenVX_LIBRARIES
    -lopenvx
    -lvxu
    -lopenvx-c_model
    -lopenvx-debug
    -lopenvx-extras
    -lxyz
    )

# set(CMAKE_CXX_FLAGS_DEBUG -g -Wall)
SET(CMAKE_CXX_FLAGS  "${CMAKE_CXX_FLAGS} -g -ggdb")

if( POLICY CMP0054 )
  cmake_policy( SET CMP0054 OLD )
endif()
if( "${CMAKE_CXX_COMPILER_ID}" STREQUAL "MSVC" )
  set ( CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/${CMAKE_BUILD_TYPE} )
else()
  set ( CMAKE_MACOSX_RPATH ON                                                  )
endif()

# by default, use open-source OpenVX without OpenCL
if( ENABLE_NN_AMD )
  set  ( OpenVX_LIBS openvx vx_nn CACHE INTERNAL OpenVX_LIBS                   )
  set  ( OpenVX_LIBS_DIR /opt/rocm/lib CACHE INTERNAL OpenVX_LIBS_DIR          )
elseif( NOT OpenVX_LIBS_DIR )
  if( ${CMAKE_SYSTEM_NAME} MATCHES "Linux" )
    EXEC_PROGRAM(cat ARGS "/proc/cpuinfo" OUTPUT_VARIABLE CPUINFO)
    if( ${CPUINFO} MATCHES "sse4_2" )
      set(SSE4_2_FOUND true CACHE BOOL "SSE4.2 available on host")
    else()
      set(SSE4_2_FOUND false CACHE BOOL "SSE4.2 available on host")
    endif()
  elseif( ${CMAKE_SYSTEM_NAME} MATCHES "Darwin" )
    EXEC_PROGRAM("/usr/sbin/sysctl -n machdep.cpu.features" OUTPUT_VARIABLE CPUINFO)
    if( ${CPUINFO} MATCHES "SSE4.2" )
      set(SSE4_2_FOUND true CACHE BOOL "SSE4.2 available on host")
    else()
      set(SSE4_2_FOUND false CACHE BOOL "SSE4.2 available on host")
    endif()
  else()
    set(SSE4_2_FOUND true CACHE BOOL "SSE4.2 available on host")
  endif()
  if( SSE4_2_FOUND )
    set   ( OpenVX_SOURCE_DIR amdovx-modules/deps/amdovx-core/openvx CACHE INTERNAL OpenVX_SOURCE_DIR             )
    if( EXISTS ${CMAKE_SOURCE_DIR}/${OpenVX_SOURCE_DIR} )
      set ( OpenVX_LIBS                       openvx             CACHE INTERNAL OpenVX_LIBS                       )
      if( NOT ENABLE_OPENCL )
        set ( CMAKE_DISABLE_FIND_PACKAGE_OpenCL TRUE             CACHE INTERNAL CMAKE_DISABLE_FIND_PACKAGE_OpenCL )
      else()
        set ( CMAKE_DISABLE_FIND_PACKAGE_OpenCL FALSE            CACHE INTERNAL CMAKE_DISABLE_FIND_PACKAGE_OpenCL )
      endif()
    endif()
  endif( SSE4_2_FOUND )
endif()

if( OpenVX_SOURCE_DIR AND EXISTS ${CMAKE_SOURCE_DIR}/${OpenVX_SOURCE_DIR} )
  if( NOT OpenVX_LIBS )
    set   ( OpenVX_LIBS openvx CACHE INTERNAL OpenVX_LIBS                              )
  endif()
elseif( NOT OpenVX_LIBS_DIR )
  set     ( OpenVX_SAMPLE /home/openvx/openvx_sample/install/Linux/x64                 )
  set     ( OpenVX_LIBS openvx vxu CACHE INTERNAL OpenVX_LIBS                          )
  if( CMAKE_BUILD_TYPE MATCHES ".*Deb.*" )
    set   ( OpenVX_LIBS_DIR ${OpenVX_SAMPLE}/Debug/bin CACHE INTERNAL OpenVX_LIBPATH   )
  else( CMAKE_BUILD_TYPE MATCHES ".*Deb.*" )
    set   ( OpenVX_LIBS_DIR ${OpenVX_SAMPLE}/Release/bin CACHE INTERNAL OpenVX_LIBPATH )
  endif( CMAKE_BUILD_TYPE MATCHES ".*Deb.*" )
endif()

if( ENABLE_OPENCL OR ENABLE_NN_AMD )
  list(APPEND CMAKE_MODULE_PATH ${PROJECT_SOURCE_DIR}/cmake)
  find_package(OpenCL REQUIRED)
endif()

if( OpenVX_SOURCE_DIR AND EXISTS ${CMAKE_SOURCE_DIR}/${OpenVX_SOURCE_DIR} )
  add_subdirectory     ( ${OpenVX_SOURCE_DIR} )
endif()
if( DISABLE_DISPLAY )
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -DENABLE_DISPLAY=0")
endif()

include_directories (include)

add_subdirectory       ( home )
