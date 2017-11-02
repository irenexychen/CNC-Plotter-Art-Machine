import cv2

#TODO resize image such that it fits within number of motor steps

#TODO adjust contrast


im_colour = cv2.imread('image7.png')
im_gray = cv2.cvtColor(im_colour, cv2.COLOR_BGR2GRAY)

(thresh, im_bw) = cv2.threshold(im_gray, 128, 255, cv2.THRESH_BINARY | cv2.THRESH_OTSU)

thresh = 127
im_bw = cv2.threshold(im_gray, thresh, 255, cv2.THRESH_BINARY)[1]

cv2.imwrite('image_bw7.png',im_bw)

#edge detection:
im_blurC = cv2.GaussianBlur(im_colour, (7,7),0) #blurs to soften edges, not really sure how effetive this is yet
im_blurBW = cv2.GaussianBlur(im_bw, (7,7),0)

im_outlineC = cv2.Canny(im_blurC, 100, 200)#for colour #function that processes and outputs an image, 100 300 is the ratio for acceptable edge gradation 
im_outlineBW = cv2.Canny(im_blurBW, 100, 200) #for black and white
im_outlineG = cv2.Canny(im_gray, 100, 200)#
#so that we can merge all three together to get better acuracy of the image

im_outlineTemp = cv2.addWeighted(im_outlineBW,1,im_outlineC,1,0) #merges two photos together
im_outline = cv2.addWeighted(im_outlineTemp,1,im_outlineG,1,0)#merges a third to it

#cv2.imwrite('image_outline10.png',im_outlineC)
#cv2.imwrite('image_outline11.png',im_outlineBW)
cv2.imwrite('image_outlineYAY2.png',im_outline)
#cv2.imwrite('image_outline12.png',im_outlineG)

#TODO detect pixels (000), output in file
