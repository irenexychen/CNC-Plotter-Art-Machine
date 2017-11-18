from __future__ import division
import cv2
import numpy as np

#TODO Use Otsu's binarization method to reduce noise before outlining

#read image as greyscale image
im_bw = cv2.imread('3portrait.jpg', 0)

#creating outlined image
blur = cv2.GaussianBlur(im_bw,(5,5),0)
edge = cv2.Canny(blur,100,200)#outline produced after just guassian blur
retval,thresh = cv2.threshold(blur,0,255,cv2.THRESH_BINARY+cv2.THRESH_OTSU)#finds optimal threshold value (may use Otu's thresholding or just im_bw)
edge1 = cv2.Canny(thresh,100,200)#outline produced after gussian blur and Otu's binarization

im_outline = cv2.addWeighted(edge1,1,edge,1,0)#combining the two images

cv2.imwrite('3im_bw.png', im_bw)
cv2.imwrite('3im_binaryT.png',thresh)#note: sometimes thresh is not used and im_bw will be used for outlining
cv2.imwrite('3im_outline.png', im_outline)

#display image on screen
cv2.startWindowThread()

cv2.namedWindow("im_bw")
cv2.imshow('im_bw', im_bw)


cv2.namedWindow("im_binaryT")
cv2.imshow('im_binaryT', thresh) 

cv2.namedWindow("im_outline")
cv2.imshow('im_outline', im_outline)

cv2.waitKey()
cv2.destroyAllWindows()