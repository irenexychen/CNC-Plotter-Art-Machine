import cv2

im_colour = cv2.imread('image.png')
im_gray = cv2.cvtColor(im_colour, cv2.COLOR_BGR2GRAY)

(thresh, im_bw) = cv2.threshold(im_gray, 128, 255, cv2.THRESH_BINARY | cv2.THRESH_OTSU)

thresh = 127
im_bw = cv2.threshold(im_gray, thresh, 255, cv2.THRESH_BINARY)[1]

cv2.imwrite('image_bw.png',im_bw)