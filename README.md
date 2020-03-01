## Dashcam Project

The project shows a demo on a screen such that people can analyse its benefits and thereby, customize their experience. The `work`, `holiday` and `home` modes contain exhaustive and mutually inclusive events that takes into consideration the project objectives.

### Automatic License Plate Recognition

License Plate is encoded information required by law for all vehicles to display on the front and back side of the car. US license plates are designed by their states, they also differ by their character length. UK license plates are designed by their counties and model date. Since the license plates form a template, it is efefctive for us to create an index of plates which can be used to identify the origin of the car dealer, manufacturer or ownership. 

Here's a small script that does the work of indexing the plates in the UK. 

When you execute the script provided:

> cd scripts/ && python lpr.py 

you get the result:

('BD', '17', 'ABC')

The first tag is a DVLA memory tag
The second is the age of the car
The third is the random characters assigned to the registered vehicle

An example of the license plate recognition task has been provided below

How to execute the App
----------------------

## Unix / Linux

sudo curl -L "https://github.com/docker/compose/releases/download/1.25.4/docker-compose-$(uname -s)-$(uname -m)" -o /usr/local/bin/docker-compose

sudo chmod +x /usr/local/bin/docker-compose

docker-compose up -d --build

python dashcam.py --video Text-detection-1.mp4

# SOLID Architecture

The dashcam project is organised into **S**ervice **O**n**L**ine **I**ndex **D**ata Architecture for Real-time data. This is with reference to the paper cited at [The Solid architecture for real-time management of big semantic data](https://www.sciencedirect.com/science/article/abs/pii/S0167739X1400209X). All data requests that are sent to the dashcam which uses the Index and Online Layers are grouped into Service Layers. The Merge layer for all the SOLID layers has been exposed as a Python executable. The docker environment provides access to all of the dashcam functionality. 

## Data Layer

`assets/vehicle-measures.csv` file contains min height and max height of lower end and higher end vehicles from which the approximate distance to the vehicle can be detected. 

The data layer contains QGIS (Geographical Information System) data which contains landmark information in a journey presented to the passengers as voice assists.

## Online Layer

- Home, Holiday and Work modes are processed using OpenVX graph API. They produce gaussian filters of the video. 

## Service Layer

`Service/SpatialInformation.py` file gets a Homographic projection of an Image region. If a banner or poster needs to be identified, the dashcam shows them within their journey information.

`Service/LicensePlateIdentifier.py` file extracts the plate information fro m ALPR inference. This is country specific and refers to the Index layer for extracting LicensePlate attributes such as region, age of the license plate. 

`Service/GStreamer.py` and `Service/TextToSpeech.py` converts any text to be displayed to the passengers into voice assists. 

The GIS Service Layer also contains an MQTT service which delivers the usable information about any activity engaging the passengers during the journey.

## Index Layer

ALPR uses character detection region based models and metadata models in the `runtime_data/region` and `runtime_data/config` directories respectively. 

Text detection uses Extremal Regions to extract text regions from the Gaussian Filters. 

The Index layer contains a metadata index about journey information obtained from the GIS service layer.

### Text Detection on Dashcam Video

Using Extremal Region Filter for the 2nd stage classifier of N&M algorithm Neumann L., Matas J., `Text/libmain.cpp` infers bounding boxes from the gaussian filter applied to the video. 

An example of text detection has been provided here:

![Car Text Detected](./scripts/image_text.png)

## Services available

### GStreamer

The gstreamer GSTParse is available as a C++ link below. GSTreamer has also got Python based interface which can be installed using `PyGObject`

GStreamer Parse:
----------------

aacparse:

[https://gstreamer.freedesktop.org/documentation/audioparsers/aacparse.html?gi-language=c](https://gstreamer.freedesktop.org/documentation/audioparsers/aacparse.html?gi-language=c)

flacparse:

[https://gstreamer.freedesktop.org/documentation/audioparsers/flacparse.html?gi-language=c](https://gstreamer.freedesktop.org/documentation/audioparsers/flacparse.html?gi-language=c)

mpegaudioparse:

[https://gstreamer.freedesktop.org/documentation/audioparsers/mpegaudioparse.html?gi-language=c](https://gstreamer.freedesktop.org/documentation/audioparsers/mpegaudioparse.html?gi-language=c)

### Spatial Information 

Based on the `assets/vehicle-measures.csv` file, an incremental update is done to lower end vehicles such as small cars and higher end vehicles and large vehicles. `xmin`, `xmax`, `ymin`, `ymax` are minimum and maximum heights of vehicles for each category respectively. This is named as `reference_intervals` for every service call to obtain Spatial Information.

### Image Segmentation

Image segmentation is done based on:

- `Region Growing`
- `Distance Transform`
- `Connected Components`
- `Markers Dilation`

Any detected text or vehicle or number plate that is in perspective view is warped using transform to show the end user in the demo video. 


### Videos for ALPR

[https://video-assets-dashcam.s3-website.eu-west-2.amazonaws.com/ALPR-detection-1.mp4](https://video-assets-dashcam.s3-website.eu-west-2.amazonaws.com/ALPR-detection-1.mp4)

[https://video-assets-dashcam.s3-website.eu-west-2.amazonaws.com/ALPR-detection-2.mp4](https://video-assets-dashcam.s3-website.eu-west-2.amazonaws.com/ALPR-detection-2.mp4)

### Videos for Text Recognition

[https://video-assets-dashcam.s3-website.eu-west-2.amazonaws.com/Text-detection-1.mp4](https://video-assets-dashcam.s3-website.eu-west-2.amazonaws.com/Text-detection-1.mp4)

[https://video-assets-dashcam.s3-website.eu-west-2.amazonaws.com/Text-detection-2.mp4](https://video-assets-dashcam.s3-website.eu-west-2.amazonaws.com/Text-detection-2.mp4)

### Videos for Vehicle Detection

[https://video-assets-dashcam.s3-website.eu-west-2.amazonaws.com/Vehicle-detection-1.mp4](https://video-assets-dashcam.s3-website.eu-west-2.amazonaws.com/Vehicle-detection-1.mp4)

[https://video-assets-dashcam.s3-website.eu-west-2.amazonaws.com/Vehicle-detection-2.mp4](https://video-assets-dashcam.s3-website.eu-west-2.amazonaws.com/Vehicle-detection-2.mp4)

## Warning System

The warning system implemented in the Dashcam is audio based for which we show a video reference. The audio based warning system uses `gTTS` framework which converts text to voice. 

