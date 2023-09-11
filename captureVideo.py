# Captures images from attached webcam and saves them
# Use for custom classifiers


import cv2
import os
import time

# 
#####################################################

myPath = 'data/images'  
cameraNo = 1 # Change to zero (0) if this setting doesn't work
cameraBrightness = 180
moduleVal = 10  # SAVE EVERY NTH FRAME TO AVOID REPETITION
minBlur = 100  # SMALLER VALUE MEANS MORE BLURRINESS PRESENT
grayImage = False  # IMAGES SAVED COLORED OR GRAY
saveData = True  # SAVE DATA FLAG
showImage = True  # IMAGE DISPLAY FLAG
imgWidth = 180
imgHeight = 120

#####################################################

global countFolder

# capture camera input
cap = cv2.VideoCapture(cameraNo)
cap.set(3, 640)
cap.set(4, 480)
cap.set(10, cameraBrightness)

count = 0
countSave = 0

#
# Creates the sequenced folders  
#
def saveDataFunc():
    global countFolder
    countFolder = 0
    while os.path.exists(myPath + str(countFolder)):
        countFolder += 1
    os.makedirs(myPath + str(countFolder))


if saveData: saveDataFunc()


while True:

    # read data from camera
    success, img = cap.read()
    # resize the image to fit our view
    img = cv2.resize(img, (imgWidth, imgHeight))

    # if gray turn to color
    if grayImage: img = cv2.cvtColor(img, cv2.COLOR_BGR2GRAY)

    
    if saveData:

        # check the blur factor don't want to blurry
        blur = cv2.Laplacian(img, cv2.CV_64F).var()
        if count % moduleVal == 0 and blur > minBlur:

            # get the time
            nowTime = time.time()

            # write to current folder with unique name
            # fix this if images are not going to where you expect
            cv2.imwrite(myPath + str(countFolder) + "\\"
                        + str(countSave) + "_" + str(int(blur)) + "_" + str(nowTime) + ".png", img)
            countSave += 1
        count += 1

    if showImage:
        cv2.imshow("Image", img)

    if cv2.waitKey(1) & 0xFF == ord('q'):
        break

# release the camera back to the system
cap.release()
# close all open windows
cv2.destroyAllWindows()
