#!/usr/bin/python

import sys
import os

import cv2 as cv
import numpy as np
import matplotlib.pyplot as plt

print('       A demo script of the Extremal Region Filter algorithm described in:')
print('       Neumann L., Matas J.: Real-Time Scene Text Localization and Recognition, CVPR 2012\n')

if __name__ == "_main__":

    pathname = "/home/aswin/Documents/Courses/Udacity/Intel-Edge/new-notebooks"

    img = cv.imread('car_image.jpg')
    # gray = cv.cvtColor(img, cv.COLOR_BGR2GRAY)
    # ret, img = cv.threshold(gray,100,200,cv.THRESH_BINARY+cv.THRESH_OTSU)
    # for visualization
    # img = np.stack([img]*3).transpose(1,2,0)
    vis      = img.copy()

    # Extract channels to be processed individually
    channels = cv.text.computeNMChannels(img)
    # Append negative channels to detect ER- (bright regions over dark background)
    cn = len(channels)-1
    for c in range(0,cn):
        channels.append((255-channels[c]))

    # Apply the default cascade classifier to each independent channel (could be done in parallel)
    print("Extracting Class Specific Extremal Regions from "+str(len(channels))+" channels ...")
    print("    (...) this may take a while (...)")
    for channel in channels:

        erc1 = cv.text.loadClassifierNM1(pathname+'/trained_classifierNM1.xml')
        er1 = cv.text.createERFilterNM1(erc1,64,0.00015,0.13,0.9,True,0.1)

        erc2 = cv.text.loadClassifierNM2(pathname+'/trained_classifierNM2.xml')
        er2 = cv.text.createERFilterNM2(erc2,0.9)

        regions = cv.text.detectRegions(channel,er1,er2)

        rects = cv.text.erGrouping(img,channel,[r.tolist() for r in regions])
        #rects = cv.text.erGrouping(img,channel,[x.tolist() for x in regions], cv.text.ERGROUPING_ORIENTATION_ANY,'../../GSoC2014/opencv_contrib/modules/text/samples/trained_classifier_erGrouping.xml',0.5)

        #Visualization
        for r in range(0,np.shape(rects)[0]):
            rect = rects[r]
            cv.rectangle(vis, (rect[0]-10,rect[1]-10), (rect[0]+rect[2]+10,rect[1]+rect[3]+10), (0, 255, 0), 8)
            cv.rectangle(vis, (rect[0]-10,rect[1]-10), (rect[0]+rect[2]+10,rect[1]+rect[3]+10), (0, 0, 255), 2)


    #Visualization
    plt.imshow(vis)
    plt.show()
    # cv.waitKey(0)
    cv.imwrite('image_text.png', vis)