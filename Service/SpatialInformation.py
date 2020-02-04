import cv2
import numpy as np
from scipy.spatial.distance import euclidean
import ImageSegmentationService

class SpatialInformationService():

    classes = "../../voc_classes/object_detection_classes_pascal_voc.txt"
    object_classes = ['car', 'bus', 'motorbike', 'bicycle']
    reference_intervals = [(1216, 1650), (1675, 1935)]
    dilationKernel = np.ones((7,7), np.uint8)
    closingKernel = cv2.getStructuringElement(cv2.MORPH_ELLIPSE, (15,15))

    def __init__(self, camera_props, label_ids, image_width):
        self.camera_props = camera_props
        self.label_ids = label_ids
        self.image_width = image_width

    def parse_labels(self):
        self.labels = dict(zip(self.label_ids, list(filter(None, SpatialInformationService.classes.split("\n")))))

    def get_focal_length(self):
        return self.camera_props['focal_length']
    
    def get_fov_pixels(self):
        return float(self.camera_props['fov']) * self.image_width / float(self.camera_props['sensor_width'])
    
    def get_ref_interval(self, object_height):
        return self.get_fov_pixels() * np.array(SpatialInformationService.reference_intervals) / object_height

    def calculate_angle(self, thresh, frame):
        dilation, closed, opening = ImageSegmentation.region_growing(thresh, SpatialInformationService.dilationKernel, SpatialInformationService.closingKernel)
        unknown, fg = ImageSegmentation.distance_transform(opening, dilation, factor=1.0)
        markers, contours = ImageSegmentation.connected_components(fg, unknown)
        dilation = ImageSegmentation.markers_dilation(markers)
        dilation, frame, angle = ImageSegmentation.fit_line(contours, dilation,(0,255,0))

        return angle, frame

    def get_distances(self, ref_interval, angle):
        nearest_dist = ref_interval * np.cos(angle * np.pi/180)
        lane_dist = ref_interval * np.sin(angle * np.pi/180)

        return nearest_dist, lane_dist

    def extract_points(self, dilation, contours):
        coordinates = np.where(dilation == 0)
        xmin, ymin, xmax, ymax = np.min(coordinates[0]), np.min(coordinates[1]), np.max(coordinates[0]), np.max(coordinates[1])
        distance = np.array([])
        distances = []
        points = np.array([])
        for cnt in contours[0:len(contours)-1]:
            points = np.append(points, cnt)
            for p in ((xmin, ymin),(xmin,ymax),(xmax,ymin),(xmax, ymax)):
                distance = np.append(distance, np.sum(np.square(points - np.array(p)), axis=0))
            distances.append(np.min(distance))
        
        return distances, points

    def drawHull(self, points, dilation):
        hull = cv2.convexHull(np.array(points))
        d = np.zeros(dilation.shape)
        img = cv2.polylines(d,[hull],True,(255,255,255))
        
        return img
    
    def drawContours(self, img):
        contours, hierarchy = cv2.findContours(img, cv2.RETR_LIST, cv2.CHAIN_APPROX_SIMPLE)
        img = cv2.drawContours(img,contours,0,(0,255,0),-1)

        return img

    def getMinAreaRect(self, contours, color=(0,0,255), thickness=5):
        for cnt in contours:
            rect = cv2.minAreaRect(cnt)
            box = cv2.boxPoints(rect)
            box = np.int0(box)
            img = cv2.drawContours(img,[box],0,color,thickness)

        return img, box

    def calculate_homography(self, thresh, frame, bounding_box):
        gray = cv2.cvtColor(frame, cv2.COLOR_BGR2GRAY)
        dilation, closed, opening = ImageSegmentation.region_growing(thresh, SpatialInformationService.dilationKernel, SpatialInformationService.closingKernel)
        unknown, fg = ImageSegmentation.distance_transform(opening, dilation, factor=1.0)
        markers, contours = ImageSegmentation.connected_components(fg, unknown)
        dilation = ImageSegmentation.markers_dilation(markers)
        distance, points = self.extract_points(dilation, contours)
        img = self.drawHull(points, dilation)
        img = self.drawContours(img)
        img, angle = ImageSegmentation.fit_line(contours, img)
        img, box = self.getMinAreaRect(contours)

        H, mask = cv2.findHomography(box, bounding_box)

        return H, mask  

    def warp_image(self, T, frame, box, bounding_box):
        box = box.flatten()
        original_frame = frame[box[1]:box[3],box[0]:box[2]]
        dsize = (bounding_box[3] - bounding_box[1], bounding_box[2] - bounding_box[0])
        output_image = cv2.warpPerspective(original_frame, T, dsize)
        
        return output_image

