import cv

def Load():
	image = cv.LoadImage("./5.jpg")
	faceCascade = cv.Load("./haarcascades/haarcascade_frontalface_alt.xml")
	eyeCascade = cv.Load("./haarcascades/haarcascade_eye.xml")
	return (image, faceCascade, eyeCascade)

def Display(image):
	cv.NamedWindow("Red Eye Test",300)
	cv.ShowImage("Red Eye Test", image)
	cv.WaitKey(0)
	cv.DestroyWindow("Red Eye Test")


# image, faceCascade, eyeCascade = Load()
#  Display(image)

def DetectRedEyes(image, faceCascade, eyeCascade):
	min_size = (20,20)
	image_scale = 2
	haar_scale = 1.2
	min_neighbors = 2
	haar_flags = 0
	# Allocate the temporary images
	gray = cv.CreateImage((image.width, image.height), 8, 1)
	smallImage = cv.CreateImage((cv.Round(image.width / image_scale),
	cv.Round (image.height / image_scale)), 8 ,1)

	# Convert color input image to grayscale
	cv.CvtColor(image, gray, cv.CV_BGR2GRAY)

	# Scale input image for faster processing
	cv.Resize(gray, smallImage, cv.CV_INTER_LINEAR)

	# Equalize the histogram
	cv.EqualizeHist(smallImage, smallImage)

	# Detect the faces
	faces = cv.HaarDetectObjects(smallImage, faceCascade, cv.CreateMemStorage(0),
	haar_scale, min_neighbors, haar_flags, min_size)

	# If faces are found
	if faces:

		for ((x, y, w, h), n) in faces:
	# the input to cv.HaarDetectObjects was resized, so scale the
	# bounding box of each face and convert it to two CvPoints
			pt1 = (int(x * image_scale), int(y * image_scale))
			pt2 = (int((x + w) * image_scale), int((y + h) * image_scale))
			cv.Rectangle(image, pt1, pt2, cv.RGB(255, 0, 0), 3, 8, 0)

		# Estimate the eyes position
		# First, set the image region of interest
		# The last division removes the lower part of the face to lower probability for false recognition
			cv.SetImageROI(image, (pt1[0],
								pt1[1],
								(pt2[0] - pt1[0]),
								int((pt2[1] - pt1[1]) * 0.6)))
		# Detect the eyes
			eyes = cv.HaarDetectObjects(image, eyeCascade,
			cv.CreateMemStorage(0),
			haar_scale, min_neighbors,
			haar_flags, (20,15))

		# If eyes were found
		if eyes:
			# For each eye found
			for eye in eyes:
				# Draw a rectangle around the eye
				cv.Rectangle(image,
				(eye[0][0],
				eye[0][1]),
				(eye[0][0] + eye[0][2],
				eye[0][1] + eye[0][3]),
				cv.RGB(255, 0, 0), 1, 8, 0)
	# Finally, reset the image region of interest (otherwise this won't
	# be drawn correctly
	cv.ResetImageROI(image)
	return image


if __name__ == "__main__":
	image, faceCascade, eyeCascade = Load()
	image = DetectRedEyes(image, faceCascade, eyeCascade)
	Display(image)


