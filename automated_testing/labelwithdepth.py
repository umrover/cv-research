# import the necessary packages
import cv2
import os, os.path
import math

#image path and valid extensions
fileDir = input("Enter folder for images and depth ") #specify your path here
imageDir = os.path.join(fileDir,"rgb")
depthDir = os.path.join(fileDir,"depth")
newfolder = os.path.join(fileDir,"changed_images")
if os.path.exists(newfolder) == False:
    os.mkdir(newfolder)
image_path_list = []
depth_path_list = []
valid_image_extensions = [".jpg", ".jpeg", ".png", ".tif", ".tiff", ".exr"] #specify your vald extensions here
valid_image_extensions = [item.lower() for item in valid_image_extensions]
# initialize the list of reference points and boolean indicating
# whether cropping is being performed or not
refPt = (0, 0)
counter = 0


def click_and_crop(event, x, y, flags, param):
    # grab references to the global variables
    global refPt, cropping, centers, counter, depthpixel
 
    # if the left mouse button was clicked, record the starting
    # (x, y) coordinates and indicate that cropping is being
    # performed
    if event == cv2.EVENT_LBUTTONDOWN:
        refPt = (x, y)
 
    # check to see if the left mouse button was released
    elif event == cv2.EVENT_LBUTTONUP:
        # record the ending (x, y) coordinates and indicate that
        # the cropping operation is finished
        #cv2.circle(image,refPt, 1, (0,0,0), -1)
        depthpixel = depth[y,x]
        if math.isnan(depthpixel):
           depthpixel = 0
        print(depthpixel)


for file in os.listdir(imageDir):
    extension = os.path.splitext(file)[1]
    if extension.lower() not in valid_image_extensions:
        continue
    image_path_list.append(os.path.join(imageDir, file)) 
    
for file in os.listdir(depthDir):
    extension = os.path.splitext(file)[1]
    if extension.lower() not in valid_image_extensions:
        continue
    depth_path_list.append(os.path.join(depthDir, file)) 

for imagePath in image_path_list:
    image = cv2.imread(imagePath)
    #image = image1.copy()
    depth = cv2.imread(depth_path_list[counter], cv2.IMREAD_ANYCOLOR | cv2.IMREAD_ANYDEPTH)
#TODO
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
                new_file_name = str(counter) + "_" + str(refPt[0]) + "_" + str(refPt[1]) + "_" + str(depthpixel) + "_"
                
                new_file_name_with_ext = new_file_name+extension
                print(new_file_name_with_ext)
                newpath = os.path.join(newfolder,new_file_name_with_ext)
                cv2.imwrite(newpath, image)
                break
            elif key == ord("v"):
                filename_without_ext = os.path.splitext(imagePath)[0]
                extension = os.path.splitext(imagePath)[1]
                counter = counter + 1
                new_file_name = str(counter) + "_" + "none"
                
                new_file_name_with_ext = new_file_name+extension
                print(new_file_name_with_ext)
                newpath = os.path.join(newfolder,new_file_name_with_ext)
                
                cv2.imwrite(newpath, image)
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