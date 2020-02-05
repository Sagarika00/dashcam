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
