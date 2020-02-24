// Produce deprecation warnings (needs to come before arrayobject.h inclusion).
#define NPY_NO_DEPRECATED_API NPY_1_7_API_VERSION

#define TXT_DECODE_VERSION "0.0.1"

#include "Python.h"  // NOLINT(build/include_alpha)
#include "object.h"
#include "Repository/Python-3.6.1/Include/unicodeobject.h"
#include "pycapsule.h"
#include "capsulethunk.h"
// #include "cxxabi.h"

#include <boost/python.hpp>
#include <boost/python/numpy.hpp>
#include <boost/python/object.hpp>
#include <boost/python/module.hpp>
#include <boost/python/def.hpp>
#include <boost/python/detail/defaults_gen.hpp>
#include <boost/python/raw_function.hpp>
#include <boost/python/suite/indexing/vector_indexing_suite.hpp>
#include <numpy/arrayobject.h>

#include "opencv2/opencv.hpp"
#include "opencv2/core.hpp"
#include "opencv2/core/cvstd.hpp"
#include "opencv2/core.hpp"
#include "opencv2/core/types.hpp"
#include "opencv2/core/utility.hpp"
#include "opencv2/text.hpp"

// these need to be included after boost on OS X
#include <string>  // NOLINT(build/include_order)
#include <vector>  // NOLINT(build/include_order)
#include <fstream>  // NOLINT
#include <iostream>
#include <stdio.h>

// #include "text_detection.h"
// #include "text_recognition.h"

#include <iostream>     // std::cout
#include <functional>

#define STRINGIFY(m) #m
#define AS_STRING(m) STRINGIFY(m)

using namespace std;
using namespace cv;

#include  <vector>
#include  <iostream>
#include  <iomanip>

#include <sstream>
#include <string>

namespace bp = boost::python;
namespace np = boost::python::numpy;

#define NM1_CLASSIFIER "./trained_classifierNM1.xml"
#define NM2_CLASSIFIER "./trained_classifierNM2.xml"
#define ERGROUPING_CLASSIFIER "./trained_classifier_erGrouping.xml"
#define OCRHMM_KNN_MODEL "./OCRHMM_knn_model_data.xml.gz"
#define OCRHMM_TRANSITIONS_TABLE "./OCRHMM_transitions_table.xml"

int obtainBeamSearchDecoder(cv::Mat image, const cv::String& filename, std::string& output_text, 
    vector<cv::Rect>* boxes, vector<string>* words, vector<float>* confidences) {

  // character recognition vocabulary
  cv::String voc = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789";
  cv::Mat transition_probabilities;
  cv::Mat emission_probabilities = cv::Mat::eye((int)voc.size(), (int)voc.size(), CV_64FC1);
  cv::FileStorage fs(OCRHMM_TRANSITIONS_TABLE, cv::FileStorage::READ);
  fs["transition_probabilities"] >> transition_probabilities;
  fs.release();

  cv::Ptr<OCRHMMDecoder::ClassifierCallback> hmm =  loadOCRBeamSearchClassifierCNN(filename);
  
  cv::Ptr<OCRBeamSearchDecoder> ocr = OCRBeamSearchDecoder::create(hmm,
      voc, transition_probabilities, emission_p, OCR_DECODER_VITERBI, 50);

  ocr->run(image, output_text, &boxes, &words, &confidences, OCR_LEVEL_WORD);

  return 0;
}


class TextDetection
{

public:
  TextDetection(np::ndarray src) {
    np::ndarray nd = np::array(src);
    int rows = (int) nd.shape(0);
    int cols = (int) nd.shape(1);
    
    boostPythonObject2Mat(nd, rows, cols);

  }
  TextDetection() {}
  ~TextDetection() {}

  void boostPythonObject2Mat(np::ndarray nd, int rows, int cols) {
    image = cv::Mat(rows, cols, CV_8UC3);

    char * data = nd.get_data();

    for(int i = 0; i < rows; i++) {
      for(int j = 0; j < cols; j++) {
        image.at<cv::Vec3b>(i,j)[0] = data[i*cols+j+0];
        image.at<cv::Vec3b>(i,j)[1] = data[i*cols+j+1];
        image.at<cv::Vec3b>(i,j)[2] = data[i*cols+j+2];
      }
    }
  }

  boost::python::tuple Beam_Search()
  {
    const cv::String filename;
    std::string& output_text;
    vector<cv::Rect>* boxes;
    vector<string>* words;
    vector<float>* confidences;
    obtainBeamSearchDecoder(image, filename, output_text, boxes, words, confidences);

    return boost::python::make_tuple(output_text,boxes,confidences);
  }

private:
    cv::Mat image;

};

BOOST_PYTHON_MODULE(libmain) {

  Py_Initialize();
  np::initialize();

  using namespace boost::python;

  bp::scope().attr("__version__") = AS_STRING(TXT_DECODE_VERSION);

  bp::class_<TextDetection>("TextDetection")
      .def(bp::init<np::ndarray>())
      .def("Run_Filters", &TextDetection::Run_Filters)
      .def("BeamSearchDecode", &TextDetection::Beam_Search);

  import_array1();

}
