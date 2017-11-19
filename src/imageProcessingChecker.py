from __future__ import division
import cv2
import numpy as np
from time import sleep
import struct
'''
import serial.tools.list_ports


ports = list(serial.tools.list_ports.comports())
for p in ports:
    print p
'''

#resize image such that it fits within number of motor steps
im_original = cv2.imread('questionmark.png')
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

#TODO adjust contrast


#loading images, cvtColor converts to grayscale
im_colour = im_resize
im_gray = cv2.cvtColor(im_colour, cv2.COLOR_BGR2GRAY)

(thresh, im_bw) = cv2.threshold(im_gray, 128, 255, cv2.THRESH_BINARY | cv2.THRESH_OTSU)

thresh = 127
im_bw = cv2.threshold(im_gray, thresh, 255, cv2.THRESH_BINARY)[1]

cv2.imwrite('questionmark_bw.png',im_bw)


#edge detection:
im_blurC = cv2.GaussianBlur(im_colour, (7,7),0) #blurs to soften edges, not really sure how effetive this is yet
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
cv2.imshow('questionmark_outline.png',im_outline)
cv2.waitKey(0)



#Parsing coordinates to Arduino
coordsX = []
coordsY = []

maxX = im_outline.shape[0]
maxY = im_outline.shape[1]


f = open('coordinatesquestionSORTED.txt', 'w')

#ser = serial.Serial(str(ports[0])[0:12], 9600, timeout=3)

def writeDebug(s):
	print("printed from DEBUG Function:" + str(s) + "\n")
	sleep(1)
	ser.write(str(s))
	ser.flush()

#not including val iterations in coordinates.txt

iterations = 0

for i in range(im_outline.shape[0]):
	for j in range(im_outline.shape[1]):
		if (im_outline[i,j] == 0):
			coordsX = np.append(coordsX, i)
			coordsY = np.append(coordsY, j)				
			print >> f, i, j
			print("found a coord!!")




'''
iterations = 0
for i in range(im_outline.shape[0]):
	for j in range(im_outline.shape[1]):
		if (iterations == 0 and im_outline[i,j] == 0):
			previousXBlack = i
			previousYBlack = y
			iterations += 1
			coordsX = np.append(coordsX, i)
			coordsY = np.append(coordsY, j)
			print >> f, i, j
		elif (im_outline[i,j] == 0 and not foundX[i] and not foundY[i]):
			coordsX = np.append(coordsX, i)
			coordsY = np.append(coordsY, j)				
			print >> f, i, j
			iterations += 1
		foundX[i] = True
		foundY[j] = True

'''

'''
iterations = 0
for i in range(im_outline.shape[0]):
	for j in range(im_outline.shape[1]):
		if (iterations == 0 and im_outline[i,j] == 0):
			previousXBlack = i
			previousYBlack = y
			iterations += 1
			coordsX = np.append(coordsX, i)
			coordsY = np.append(coordsY, j)
			print >> f, i, j
		elif (im_outline[i,j] == 0): 
			if ((abs(previousXBlack - i) > 7) or (abs(previousYBlack - j) > 7)):
				coordsX = np.append(coordsX, i)
				coordsY = np.append(coordsY, j)
				print >> f, i, j
				previousXBlack = i
				previousYBlack = j
				iterations += 1



def checkSurrounding()



#writeDebug(str(iterations))




message = "not get next"
#loop so that the python program does not quit
for i in range (iterations):
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
	

	message = "message not get >:("



message = "not get next"
#loop so that the python program does not quit
for i in range (iterations):
	while (message != "@GetNext"):
		#read in a non-relevant line i.e. Sleep
		print("from python: waiting")
		while (not ser.in_waiting):
			print(str(i) + "in_waiting from python: waiting to read next arduino line")
			sleep(1)
		
		#next line from arduino exists, read!!
		message = ser.readline()
		print(str(i) + "from arduino, message:" + message)
	print("left @GetNext!!!!!!!\n")
	#is @GetNext, give it next
	#bin = struct.pack('f', coordsX[i])
	ser.write(str(coordsX[i]))
	ser.flush()
	ser.write(" ")
	ser.flush()
	ser.write(str(coordsY[i]))
	ser.flush()
	ser.write("\n")
	ser.flush()
	


	message = "not get next"


'''



