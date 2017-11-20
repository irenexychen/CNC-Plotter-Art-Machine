from __future__ import division
import cv2
import numpy as np
from time import sleep
import struct
import sys

from itertools import repeat

import serial.tools.list_ports
sys.setrecursionlimit(3000)

ports = list(serial.tools.list_ports.comports())
for p in ports:
    print p


#resize image such that it fits within number of motor steps
im_original = cv2.imread('img.png')

global x
global y


y, x = im_original.shape[:2]


if (x > y):
	rescale = 200/x
	#print(x, rescale)
else:
	rescale = 200/y
	#print(y, rescale)

im_resize = cv2.resize(im_original, (0,0), fx=rescale, fy=rescale, interpolation = cv2.INTER_AREA) #common interpolation for shrinking

cv2.imwrite('img_resize.png', im_resize)

y, x = im_resize.shape[:2]
print ("dimensions:", x, y)

#loading images, cvtColor converts to grayscale
im_colour = im_resize
im_gray = cv2.cvtColor(im_colour, cv2.COLOR_BGR2GRAY)

(thresh, im_bw) = cv2.threshold(im_gray, 128, 255, cv2.THRESH_BINARY | cv2.THRESH_OTSU)

thresh = 127
im_bw = cv2.threshold(im_gray, thresh, 255, cv2.THRESH_BINARY)[1]

cv2.imwrite('img_bw.png',im_bw)

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
cv2.imwrite('img_outline.png',im_outline)
print("done converting")
#cv2.imshow('questionmark_outline.png',im_outline)
#cv2.waitKey(0)


#Parsing coordinates to Arduino
global coordsX 
coordsX = []
global coordsY 
coordsY = []

f = open('coordinatesIMG.txt', 'w')

tester = [[0 for i in range (3)] for k in range (5)]
print(np.matrix(tester))

global wentTo
wentTo = [[0 for i in range (x)] for k in range (y)] #repeat 110 zeros(y coord), inside an array of 200 elements (x coord)
print(np.matrix(wentTo))

#for i in range(len(wentTo)):
#	for k in range(len(wentTo[i])):
#		print wentTo[i][k]
#foundX = list(repeat(0, y))
#foundY = list(repeat(0, x))

print("max X is", y)
print("max Y is", x)

ser = serial.Serial(str(ports[0])[0:12], 9600, timeout=3)


def writeDebug(s):
	#print("printed from DEBUG Function:" + str(s) + "\n")
	sleep(1)
	ser.write(str(s))
	ser.flush()		

def checkAround(i, j):
	global coordsX
	global coordsY
	global foundX
	global foundY
	global wentTo

	print("checking around", i, j)

	newI = i - 1
	newJ = j
	checkIfBlack(newI, newJ);	

	newI = i - 1
	newJ = j + 1
	checkIfBlack(newI, newJ);

	newI = i 
	newJ = j + 1
	checkIfBlack(newI, newJ);

	newI = i + 1
	newJ = j + 1
	checkIfBlack(newI, newJ);

	newI = i + 1
	newJ = j
	checkIfBlack(newI, newJ);

	newI = i + 1
	newJ = j - 1
	checkIfBlack(newI, newJ);

	newI = i
	newJ = j - 1
	checkIfBlack(newI, newJ);

	newI = i - 1
	newJ = j - 1
	checkIfBlack(newI, newJ);

def checkIfBlack(newI, newJ):
	global coordsX
	global coordsY
	global foundX
	global foundY
	global wentTo

	print ("looking at", newI, newJ)

	if (0 <= newI < im_outline.shape[0]) and (0 <= newJ < im_outline.shape[1]):
		#print("is a valid point within range")
		if (wentTo[newI][newJ] == 0):
			#print ("valid point to looking at", newI, newJ)
			if (im_outline[newI][newJ] == 0):
				#print("found in checkIfBlack: ", newI, newJ , "is black")
				coordsX = np.append(coordsX, newI)
				coordsY = np.append(coordsY, newJ)
				print >> f, newI, newJ
				wentTo[newI][newJ] = 1
				checkAround(newI, newJ)
			wentTo[newI][newJ] = 1



for i in range(im_outline.shape[0]):		#200
	for j in range(im_outline.shape[1]):	#111
		#print ("inside initial for-loop", i, j)
		if ( 0 <= i < x and 0<= j < y):
			if (im_outline[i][j] == 0):
				#print("pixel is black inside for-loop")
				if (wentTo[i][j] == 0):
					coordsX = np.append(coordsX, i)
					coordsY = np.append(coordsY, j)				
					print >> f, i, j
					#print("found and marked black pixel in initial for-loop", i, j)
					wentTo[i][j] = 1
					checkAround(i,j)
				wentTo[i][j] = 1


'''
def checkIfBlack(newI, newJ):
	global coordsX
	global coordsY
	global foundX
	global foundY

	print ("looking at", newI, newJ)

	if (0 <= newI < im_outline.shape[0]) and (0 <= newJ < im_outline.shape[1]):
		print("is a valid point within range")
		print(foundX[newI], foundY[newJ])
		if (foundX[newI] == 0 or foundY[newJ] == 0):
			print ("valid point to looking at", newI, newJ)
			if (im_outline[newI][newJ] == 0):
				print("found in checkIfBlack: ", newI, newJ , "is black")
				coordsX = np.append(coordsX, newI)
				coordsY = np.append(coordsY, newJ)
				print >> f, newI, newJ
				foundX[newI] = 1
				foundY[newJ] = 1
				checkAround(newI, newJ)
			foundX[newI] = 1
			foundY[newJ] = 1



for i in range(im_outline.shape[0]):		#200
	for j in range(im_outline.shape[1]):	#111
		print ("inside initial for-loop", i, j)
		if ( 0 <= i < x and 0<= j < y):
			if (im_outline[i][j] == 0):
				print("pixel is black inside for-loop")
				if (foundX[i] == 0 or foundY[j] == 0):
					coordsX = np.append(coordsX, i)
					coordsY = np.append(coordsY, j)				
					print >> f, i, j
					print("found and marked black pixel in initial for-loop", i, j)
					foundX[i] = 1
					foundY[j] = 1
					checkAround(i,j)
				foundX[i] = 1
				foundY[j] = 1

#parse in number of eleemets at some point
'''
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