# import the necessary packages
import cv2
import os, os.path

########################
#Usage

# python label.python
# enter folder location ex."C:\users\ynjason\desktop\mrover"
# click on center of tennis ball if there is one 
# a small black dot will appear
# able to click as many times as needed to get as close to center as possible
# (comment out line 69 to be able to drag resize image)
# press c to go on to the next picture
# if there is no tennis ball in picture press v
# to quit labeling pictures in the middle of the images press q

#labeling in the format "number of picture_x_y" if tennis ball in the picture and "number of picture_no_tennis_ball" if no tennis ball
######################

#image path and valid extensions
imageDir = input("Enter folder for images ") #specify your path here
image_path_list = []
valid_image_extensions = [".jpg", ".jpeg", ".png", ".tif", ".tiff"] #specify your vald extensions here
valid_image_extensions = [item.lower() for item in valid_image_extensions]
# initialize the list of reference points and boolean indicating
# whether cropping is being performed or not
refPt = (0, 0)
centers = []
cropping = False
counter = 0
def click_and_crop(event, x, y, flags, param):
    # grab references to the global variables
    global refPt, cropping, centers, counter
 
    # if the left mouse button was clicked, record the starting
    # (x, y) coordinates and indicate that cropping is being
    # performed
    if event == cv2.EVENT_LBUTTONDOWN:
        refPt = (x, y)
        centers.append(refPt)
        cropping = True
 
    # check to see if the left mouse button was released
    elif event == cv2.EVENT_LBUTTONUP:
        # record the ending (x, y) coordinates and indicate that
        # the cropping operation is finished
        cv2.circle(image,refPt, 5, (0,0,0), -1)
        cropping = False

for file in os.listdir(imageDir):
    extension = os.path.splitext(file)[1]
    if extension.lower() not in valid_image_extensions:
        continue
    image_path_list.append(os.path.join(imageDir, file)) 
# load the image, clone it, and setup the mouse callback function

for imagePath in image_path_list:
    image = cv2.imread(imagePath)
    
    # display the image on screen with imshow()
    # after checking that it loaded
    if image is not None:

        while True:
        # display the image and wait for a keypress
            #image = cv2.resize(image, (1920, 1080))
            cv2.namedWindow(imagePath,cv2.WINDOW_NORMAL)
            cv2.resizeWindow(imagePath, 756, 1008)
            cv2.imshow(imagePath, image)
            cv2.setMouseCallback(imagePath, click_and_crop)
            key = cv2.waitKey(1) & 0xFF
    
        # if the 'c' key is pressed, break from the loop
            if key == ord("c"):
                filename_without_ext = os.path.splitext(imagePath)[0]
                extension = os.path.splitext(imagePath)[1]
                counter = counter + 1
                new_file_name = str(counter) + "_" + str(refPt[0]) + "_" + str(refPt[1])
                
                new_file_name_with_ext = new_file_name+extension
                print(new_file_name_with_ext)
                os.rename(os.path.join(imageDir,imagePath),os.path.join(imageDir,new_file_name_with_ext))
                break
            elif key == ord("v"):
                filename_without_ext = os.path.splitext(imagePath)[0]
                extension = os.path.splitext(imagePath)[1]
                counter = counter + 1
                new_file_name = str(counter) + "_" + "no_tennis_ball"
                
                new_file_name_with_ext = new_file_name+extension
                print(new_file_name_with_ext)
                os.rename(os.path.join(imageDir,imagePath),os.path.join(imageDir,new_file_name_with_ext))
                break
            elif key == ord("q"):
                break
        if key == ord("q"):
            break
        
    elif image is None:
        #end this loop iteration and move on to next image
        continue  

 
# keep looping until the 'q' key is pressed

#print(refPt[0], refPt[1]) 

 
# close all open windows
cv2.destroyAllWindows()