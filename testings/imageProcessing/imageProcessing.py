from __future__ import division
import cv2
import numpy as np
import serial as Serial


#resize image such that it fits within number of motor steps
im_original = cv2.imread('maybeagoose.jpg')
y, x = im_original.shape[:2]
print(x, y)

if (x > y):
	rescale = 200/x
	print(x, rescale)
else:
	rescale = 200/y
	print(y, rescale)

im_resize = cv2.resize(im_original, (0,0), fx=rescale, fy=rescale, interpolation = cv2.INTER_AREA) #common interpolation for shrinking

cv2.imwrite('goose1resized.png', im_resize)
y, x = im_resize.shape[:2]

#TODO adjust contrast


#loading images, cvtColor converts to grayscale
im_colour = im_resize
im_gray = cv2.cvtColor(im_colour, cv2.COLOR_BGR2GRAY)

(thresh, im_bw) = cv2.threshold(im_gray, 128, 255, cv2.THRESH_BINARY | cv2.THRESH_OTSU)

thresh = 127
im_bw = cv2.threshold(im_gray, thresh, 255, cv2.THRESH_BINARY)[1]

cv2.imwrite('goose1bw.png',im_bw)



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
im_outline = cv2.bitwise_not(im_outline);
cv2.imwrite('goose1outline.png',im_outline)


#TODO detect pixels (000), output in file
#coordinates = np.where(zeros == [255])

xblacks = []
yblacks = []

f = open('coordinates.txt', 'w')
iterations = 0;
for i in range(im_outline.shape[0]):
	for j in range(im_outline.shape[1]):
		if (iterations == 0):
			previousXBlack = i
			previousYBlack = y
			xblacks = np.append(xblacks, i)
			yblacks = np.append(xblacks, j)
			print>>f, i, j
			iterations = iterations + 1
		if (im_outline[i,j] == 0):
			if ((abs(previousXBlack - i) > 5) or (abs(previousYBlack - j) > 5)):
				xblacks = np.append(xblacks, i)
				yblacks = np.append(xblacks, j)
				print>>f, i, j
				previousXBlack = i
				previousYBlack = j
			


print>>f, "!"



ser = Serial('/dev/ttyACM0', 9600, timeout=3)
ser.write(coordinates.txt)



