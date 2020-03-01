import sys
sys.path.append("../")
sys.path.append("/home/openalpr/openalpr/src/bindings/python")
from Service.LicensePlateIdentifier import LicensePlateIdentifier
from openalpr import Alpr
from Service.VideoCapture import VideoCapture
import argparse
import cv2

def parse_args():
    parser = argparse.ArgumentParser()
    parser.add_argument("--video", default="/home/aswin/Documents/Courses/Udacity/Intel-Edge/Work/EdgeApp/License_Plate_Recognition/Dashcam-project/Text-detection-1.mp4", type=str)
    parser.add_argument("--save_video", type=bool, default=False)
    
    return parser.parse_args()

def get_video(args):
    video_capture = VideoCapture(args.video)
    video_capture.create_capture()
    if args.save_video:
        video_capture.create_output()
    return video_capture.cap, video_capture.out

if __name__ == "__main__":
    cap, out = get_video(parse_args())
    alpr = Alpr("us", "../ALPR/alpr_config/runtime_data/us.conf", "../ALPR/alpr_config/runtime_data")
    print(alpr)
    if not alpr.is_loaded():
        print("Error loading OpenALPR")
        sys.exit(1)
    # while True:
    #     ret, frame = cap.read()
    #     if ret == True:
    #         key_pressed = cv2.waitKey(60)
    #         cv2.imshow("alpr_video_test", frame)
    #         # lpr = LicensePlateIdentifier(frame, alpr)
    #         # lpr.apply_alpr()
    #         # if len(lpr.results['results']) > 0:
    #         #     plates, confidences = lpr.extract_plates()
    #         #     print(plates, confidences)
    #         if key_pressed == 100:
    #             cv2.imwrite("image.png", frame)
    #             break
    #     else:
    #         break