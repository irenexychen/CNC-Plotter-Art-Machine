from __future__ import division
import cv2
import numpy as np
from time import sleep
import struct

from itertools import repeat

import serial.tools.list_ports


ports = list(serial.tools.list_ports.comports())
for p in ports:
    print p


#resize image such that it fits within number of motor steps
im_original = cv2.imread('questionmark.png')

global x
global y


y, x = im_original.shape[:2]
#print(x, y)

if (x > y):
	rescale = 200/x
	#print(x, rescale)
else:
	rescale = 200/y
	#print(y, rescale)

im_resize = cv2.resize(im_original, (0,0), fx=rescale, fy=rescale, interpolation = cv2.INTER_AREA) #common interpolation for shrinking

cv2.imwrite('questionmark_resize.png', im_resize)

y, x = im_resize.shape[:2]

#loading images, cvtColor converts to grayscale
im_colour = im_resize
im_gray = cv2.cvtColor(im_colour, cv2.COLOR_BGR2GRAY)

(thresh, im_bw) = cv2.threshold(im_gray, 128, 255, cv2.THRESH_BINARY | cv2.THRESH_OTSU)

thresh = 127
im_bw = cv2.threshold(im_gray, thresh, 255, cv2.THRESH_BINARY)[1]

cv2.imwrite('questionmark_bw.png',im_bw)

#edge detection:
im_blurC = cv2.GaussianBlur(im_colour, (7,7),0) #blurs to soften edges, really sure how effetive this is yet
im_blurBW = cv2.GaussianBlur(im_bw, (7,7),0)

im_outlineC = cv2.Canny(im_blurC, 100, 200) #for colour 
#processes and outputs an image, 100 300 is the ratio for acceptable edge gradation 
im_outlineBW = cv2.Canny(im_blurBW, 100, 200) #for black and white
im_outlineG = cv2.Canny(im_gray, 100, 200)#
#so that we can merge all three together to get better acuracy of the image

im_outlineTemp = cv2.addWeighted(im_outlineBW,1,im_outlineC,1,0) #merges two photos together
im_outline = cv2.addWeighted(im_outlineTemp,1,im_outlineG,1,0)#merges a third to it

#invert colours:
im_outline = cv2.bitwise_not(im_outline)
cv2.imwrite('questionmark_outline.png',im_outline)
print("done converting")
#cv2.imshow('questionmark_outline.png',im_outline)
#cv2.waitKey(0)


#Parsing coordinates to Arduino
global coordsX 
coordsX = []
global coordsY 
coordsY = []

f = open('coordinatesquestion.txt', 'w')

foundX = []
foundY = []
foundX = list(repeat(0, x))
foundY = list(repeat(0, y))



ser = serial.Serial(str(ports[0])[0:12], 9600, timeout=3)


def writeDebug(s):
	print("printed from DEBUG Function:" + str(s) + "\n")
	sleep(1)
	ser.write(str(s))
	ser.flush()

def generalCheckSurrounding(i,j):
	global coordsX
	global coordsY

	if ((0 <= (i + 1) < x) and (0 <= (i - 1) < x) and (0 <= (j + 1) < y) and (0 <= (j - 1) < y)):
		checkSurroundingX(i,j)
		checkSurroundingY(i,j)
		checkBothSurrounding(i,j)

def checkSurroundingX(current, j):
	global coordsX
	global coordsY
	new = current - 1
	if (0 <= new < y):
		if (foundX[new] == 0 or foundY[j] == 0):
			if (im_outline[new][j] == 0):
				coordsX = np.append(coordsX, new)
				coordsY = np.append(coordsY, j)
				print >> f, new, j
				print("found %d %d in X +1", new, j)
				foundX[new] = 1
				foundY[j] = 1
				generalCheckSurrounding(new, j)


	new = current + 1
	if (0 <= new < y):
		if (foundX[new] == 0 or foundY[j] == 0):
			if (im_outline[new][j] == 0):
				coordsX = np.append(coordsX, new)
				coordsY = np.append(coordsY, j)
				print >> f, new, j
				print("found %d %d in X +1", new, j)
				foundX[new] = 1
				foundY[j] = 1
				generalCheckSurrounding(new, j)



def checkSurroundingY(i, current):
	global coordsX
	global coordsY
	new  = current - 1
	if (0 <= new < x):
		if (foundX[i] == 0 or foundY[new] == 0):
			if (im_outline[i][new] == 0):
				coordsX = np.append(coordsX, i)
				coordsY = np.append(coordsY, new)
				print >> f, i, new
				print("found %d %d in Y -1", i, new)
				foundX[i] = 1
				foundY[new] = 1
				generalCheckSurrounding(i, new)


	new = current + 1
	if (0 <= new < x):
		if (foundX[i] == 0 or foundY[new] == 0):
			if (im_outline[i][new] == 0):
				coordsX = np.append(coordsX, i)
				coordsY = np.append(coordsY, new)
				print >> f, i, new
				print("found %d %d in Y +1", i, new)
				foundX[i] = 1
				foundY[new] = 1
				generalCheckSurrounding(i, new)


def checkBothSurrounding(i, j):
	global coordsX
	global coordsY
	newI = i - 1
	newJ = j - 1
	if (0 <= newI < y and 0 <= newJ < x):
		if (foundX[newI] == 0 or foundY[newJ] == 0):
			if (im_outline[newI][newI] == 0):
				coordsX = np.append(coordsX, newI)
				coordsY = np.append(coordsY, newJ)
				print >> f, newI, newJ
				print("found %d %d in dia -1 -1", newI, newJ)
				foundX[newI] = 1
				foundY[newJ] = 1
				generalCheckSurrounding(newI, newJ)


	newI = i - 1
	newJ = j + 1
	if (0 <= newI < y and 0 <= newJ < x):
		if (foundX[newI] == 0 or foundY[newJ] == 0): 
			if (im_outline[newI][newI] == 0):
				coordsX = np.append(coordsX, newI)
				coordsY = np.append(coordsY, newJ)
				print >> f, newI, newJ
				print("found %d %d in dia -1 +1", newI, newJ)
				foundX[newI] = 1
				foundY[newJ] = 1
				generalCheckSurrounding(newI, newJ)


	newI = i + 1
	newJ = j - 1
	if (0 <= newI < y and 0 <= newJ < x):
		if (foundX[newI] == 0 or foundY[newJ] == 0):
			if (im_outline[newI][newI] == 0):
				coordsX = np.append(coordsX, newI)
				coordsY = np.append(coordsY, newJ)
				print >> f, newI, newJ
				print("found %d %d in dia +1 -1", newI, newJ)
				foundX[newI] = 1
				foundY[newJ] = 1
				generalCheckSurrounding(newI, newJ)

		
	newI = i + 1
	newJ = j + 1

	if (0 <= newI < y and 0 <= newJ < x):
		if (foundX[newI] == 0 or foundY[newJ] == 0):
			if (im_outline[newI][newI] == 0):
				coordsX = np.append(coordsX, newI)
				coordsY = np.append(coordsY, newJ)
				print >> f, newI, newJ
				print("found %d %d in dia +1 +1", newI, newJ)
				foundX[newI] = 1
				foundY[newJ] = 1
				generalCheckSurrounding(newI, newJ)




for i in range(im_outline.shape[0]):		#200
	for j in range(im_outline.shape[1]):	#111
		if ( 0 <= i < x and 0<= j < y):
			if (im_outline[i][j] == 0):
				if (foundX[i] == 0 or foundY[j] == 0):
					coordsX = np.append(coordsX, i)
					coordsY = np.append(coordsY, j)				
					print >> f, i, j
					print("found %d %d in initial for-loop", i, j)
					foundX[i] = 1
					foundY[j] = 1
					generalCheckSurrounding(i, j)
		
				


#parse in number of eleemets at some point

numElements = len(coordsX)

message = "get next"
#loop so that the python program does quit
for i in range (numElements):
	while (message != "@GetNext"):
		#read in a non-relevant line i.e. Sleep
		print("from python: waiting")
		#check for next line
		message = ser.readline()
		print(str(i) + "from arduino, message:" + message)
	print("left @GetNext!!!!!!!\n")
	#is @GetNext, give it next
	ser.write(str(coordsX[i]))
	ser.flush()
	ser.write(" ")
	ser.flush()
	ser.write(str(coordsY[i]))
	ser.flush()
	ser.write("\n")
	ser.flush()
	

	message = "message get >:("

