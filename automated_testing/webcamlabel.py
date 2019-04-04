# -*- coding: utf-8 -*-
"""
Created on Wed Jan 16 19:11:54 2019

@author: ynjason
"""

# import the necessary packages
import cv2
import numpy as np

refPt = (0, 0)

cap = cv2.VideoCapture(0)

def click_and_crop(event, x, y, flags, param):
    # grab references to the global variables
    global refPt
 
    # if the left mouse button was clicked, record the starting
    # (x, y) coordinates and indicate that cropping is being
    # performed
    if event == cv2.EVENT_LBUTTONDOWN:
        refPt = (x, y)
        
 
    # check to see if the left mouse button was released
    elif event == cv2.EVENT_LBUTTONUP:
        
        print("x:", str(refPt[0]),", y:", str(refPt[1]))
        pixel = frame[refPt[1]][refPt[0]]
        print("r:", pixel[2], "g:", pixel[1], "b:", pixel[0])
        #color = np.uint8([[[pixel[0],pixel[1],pixel[2] ]]])
        #hsv = cv2.cvtColor(color, cv2.COLOR_BGR2HSV)
        #print("hsv:", hsv)
# Check if the webcam is opened correctly
if not cap.isOpened():
    raise IOError("Cannot open webcam")


while True:
    ret, frame = cap.read()
    #frame = cv2.resize(frame, None, fx=0.5, fy=0.5, interpolation=cv2.INTER_AREA)
    cv2.imshow('Input', frame)
    cv2.setMouseCallback('Input', click_and_crop)
    key = cv2.waitKey(1) & 0xFF
    if key == ord("q"):
        break
cap.release()
# close all open windows
cv2.destroyAllWindows()