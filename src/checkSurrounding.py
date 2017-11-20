global x
global y


def generalCheckSurrounding(j,i):
	global coordsX
	global coordsY

	if ((0 <= (j + 1) < x) and (0 <= (j - 1) < x) and (0 <= (i + 1) < y) and (0 <= (i - 1) < y)):
		checkSurroundingX(j,i)
		checkSurroundingY(j,i)
		checkBothSurrounding(j,i)

def checkSurroundingX(current, i):
	global coordsX
	global coordsY
	new = current - 1
	if (0 <= new < x):
		if ((foundX[new] == 0 or foundY[i] == 0) and im_outline[new][i] == 0):
			coordsX = np.append(coordsX, new)
			coordsY = np.append(coordsY, i)
			print >> f, new, i
			print("found %d %d in X +1", new, i)
			foundX[new] = 1
			foundY[i] = 1
			generalCheckSurrounding(new, i)


	new = current + 1
	if (0 <= new < x):
		if ((foundX[new] == 0 or foundY[i] == 0) and im_outline[new][i] == 0):
			coordsX = np.append(coordsX, new)
			coordsY = np.append(coordsY, i)
			print >> f, new, i
			print("found %d %d in X +1", new, i)
			foundX[new] = 1
			foundY[i] = 1
			generalCheckSurrounding(new, i)



def checkSurroundingY(j, current):
	global coordsX
	global coordsY
	new  = current - 1
	if (0 <= new < y):
		if ((foundX[j] == 0 or foundY[new] == 0) and im_outline[j][new] == 0):
			coordsX = np.append(coordsX, j)
			coordsY = np.append(coordsY, new)
			print >> f, j, new
			print("found %d %d in Y -1", j, new)
			foundX[j] = 1
			foundY[new] = 1
			generalCheckSurrounding(j, new)


	new = current + 1
	if (0 <= new < y):
		if ((foundX[j] == 0 or foundY[new] == 0) and im_outline[j][new] == 0):
			coordsX = np.append(coordsX, j)
			coordsY = np.append(coordsY, new)
			print >> f, j, new
			print("found %d %d in Y +1", j, new)
			foundX[j] = 1
			foundY[new] = 1
			generalCheckSurrounding(j, new)


def checkBothSurrounding(j, i):
	global coordsX
	global coordsY
	newI = j - 1
	newJ = i - 1
	if (0 <= newI < x and 0 <= newJ < y):
		if ((foundX[newI] == 0 or foundY[newJ] == 0) and im_outline[newI][newI] == 0):
			coordsX = np.append(coordsX, newI)
			coordsY = np.append(coordsY, newJ)
			print >> f, newI, newJ
			print("found %d %d in dia -1 -1", newI, newJ)
			foundX[newI] = 1
			foundY[newJ] = 1
			generalCheckSurrounding(newI, newJ)


	newI = j - 1
	newJ = i + 1
	if (0 <= newI < x and 0 <= newJ < y):
		if ((foundX[newI] == 0 or foundY[newJ] == 0) and im_outline[newI][newI] == 0):
			coordsX = np.append(coordsX, newI)
			coordsY = np.append(coordsY, newJ)
			print >> f, newI, newJ
			print("found %d %d in dia -1 +1", newI, newJ)
			foundX[newI] = 1
			foundY[newJ] = 1
			generalCheckSurrounding(newI, newJ)


	newI = j + 1
	newJ = i - 1
	if (0 <= newI < x and 0 <= newJ < y):
		if ((foundX[newI] == 0 or foundY[newJ] == 0) and im_outline[newI][newI] == 0):
			coordsX = np.append(coordsX, newI)
			coordsY = np.append(coordsY, newJ)
			print >> f, newI, newJ
			print("found %d %d in dia +1 -1", newI, newJ)
			foundX[newI] = 1
			foundY[newJ] = 1
			generalCheckSurrounding(newI, newJ)

		
	newI = j + 1
	newJ = i + 1

	if (0 <= newI < x and 0 <= newJ < y):
		if ((foundX[newI] == 0 or foundY[newJ] == 0) and im_outline[newI][newI] == 0):
			coordsX = np.append(coordsX, newI)
			coordsY = np.append(coordsY, newJ)
			print >> f, newI, newJ
			print("found %d %d in dia +1 +1", newI, newJ)
			foundX[newI] = 1
			foundY[newJ] = 1
			generalCheckSurrounding(newI, newJ)




for j in range(0, 200):		 	#picture's y, physically x
	for i in range(0, 111):  	#picture's x, physically y
		print ("COORDS", x, y)
		if ( 0 <= j < x and 0<= i < y and im_outline[j][i] == 0):
			print("IS SMALLER")
			if (foundX[j] == 0 or foundY[i] == 0):
				coordsX = np.append(coordsX, j)
				coordsY = np.append(coordsY, i)				
				print >> f, j, i
				print("found %d %d in initial for-loop", j, i)
				foundX[j] = 1
				foundY[i] = 1
				generalCheckSurrounding(j, i)
		