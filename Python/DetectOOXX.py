import numpy as np
import cv2

def detect_ooxx(img):

    cv2.imshow('before', img)
    return None

    aft = img.copy()

    gray =  cv2.cvtColor(aft, cv2.COLOR_BGR2GRAY)
    gblur1 = cv2.GaussianBlur(gray, (9,9),0)

    whiteblack = cv2.adaptiveThreshold(gblur1, 255, cv2.ADAPTIVE_THRESH_GAUSSIAN_C, cv2.THRESH_BINARY_INV, 11, 2)
    #cv2.imshow('whiteblack', whiteblack)
    kernel = np.ones((8, 8), np.uint8)

    contours, hierarchy = cv2.findContours(whiteblack, cv2.RETR_EXTERNAL, cv2.CHAIN_APPROX_SIMPLE)

    for ct in contours:
        if 20 < cv2.contourArea(ct) < 50000:
            cv2.drawContours(whiteblack, [ct], -1, (255,255,255), 10)

    #cv2.imshow('after contours', whiteblack)

    contours, hierarchy = cv2.findContours(whiteblack, cv2.RETR_EXTERNAL, cv2.CHAIN_APPROX_SIMPLE)

    if(not contours):
        print("Blank")
        return None

    for ct in contours:
        if 20 < cv2.contourArea(ct) < 30000:
            cvhull = cv2.convexHull(ct)

            cv2.drawContours(aft, [ct], -1, (255,0,0), 1)
            cv2.drawContours(aft, [cvhull], -1, (0,255,0), 1)

            ct_area = cv2.contourArea(ct)
            hull_area = cv2.contourArea(cvhull)


            solidity = float(ct_area)/hull_area
            if(solidity > 0.85):
                print("O detected",solidity)
                return 'O'
            else:
                print("X detected",solidity)
                return 'X'

