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

python dashcam.py --video Text-detection-1.mp4



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

`pip install gTTS`

On Windows:
-----------

`pacman -S mingw-w64-x86_64-gtk3 mingw-w64-x86_64-python3 mingw-w64-x86_64-python3-gobject`

On Ubuntu:
----------

`sudo apt install python3-gi python3-gi-cairo gir1.2-gtk-3.0`

`sudo apt install libgirepository1.0-dev gcc libcairo2-dev pkg-config python3-dev gir1.2-gtk-3.0`

`pip3 install pycairo`
`pip3 install PyGObject`

