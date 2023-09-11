/**
 * @brief Crops images and overwrites(saves) the original image with
 * the crop.
 *
 * @note Captures mouse drag to create a box around the image and
 * once the mouse is let up then crops to the box. It will automatically
 * save the image once the crop is produced and overwrite the original, so make sure the crop is
 * good before letting off the mouse button.
 *
 * @author Craig R Harrell
 *
 */

#include "opencv2/core/core.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include <iostream>
#include <filesystem>

using namespace cv;
using namespace std;
namespace fs = std::filesystem;

// vars for creating cropped images

// mouse vars
bool leftDown = false, leftup = false;

// image var
Mat img;

// coordinate and box vars
Point cor1, cor2;
Rect box;

// current name of file var
string currentName;

/**
 * @brief grabs the mouse actions, paints a box, crops
 * image to inside box, and saves the cropped image
 * @param event
 * @param x
 * @param y
 */
void mouse_call(int event, int x, int y, int, void *) {
    // first corner of box
    if (event == EVENT_LBUTTONDOWN) {
        leftDown = true;
        cor1.x = x;
        cor1.y = y;
    }

    // second corner
    if (event == EVENT_LBUTTONUP) {
        // if box is bigger than 20 pixels
        if (abs(x - cor1.x) > 20 && abs(y - cor1.y) > 20) {
            leftup = true;
            cor2.x = x;
            cor2.y = y;
        } else {
            cout << "Select a region more than 20 pixels" << endl;
        }
    }

    //when the left button is down
    // paint box
    if (leftDown && !leftup) {
        Point pt;
        pt.x = x;
        pt.y = y;
        Mat temp_img = img.clone();
        rectangle(temp_img, cor1, pt, Scalar(0, 0, 255)); //drawing a rectangle continuously
        imshow("Original", temp_img);
    }

    // when selection is complete
    if (leftDown && leftup) {
        // create our bounds
        box.width = abs(cor1.x - cor2.x);
        box.height = abs(cor1.y - cor2.y);
        box.x = min(cor1.x, cor2.x);
        box.y = min(cor1.y, cor2.y);
        Mat crop(img, box); //Selecting a ROI(region of interest) from the original pic

        // display our cropped image
        namedWindow("Cropped Image");
        imshow("Cropped Image", crop); //showing the cropped image

        // write to disk
        imwrite(currentName, crop);

        // reset our variables
        leftDown = false;
        leftup = false;
    }

}

/**
 * @brief Main function. takes a single argument of the path name
 * to working directory from command line, if not set then one must
 * be provided.
 * @param argc
 * @param argv
 * @return
 */
int main(int argc, char **argv) {
    // define our path placeholder
    std::string path;
    if (argc > 1) { // check to see if path was passed
        path = argv[1];
        cout << "Use the 'n' key to move to the next image after cropping or to skip current image." << endl;
        cout << "To exit use the 'q' key." << endl;
    } else { // it wasn't so show error message and exit
        cout << "Must enter a command line argument." << endl;
        cout << "example: " << endl;
        cout << "./cropImage /PATH/TO/IMAGE/DIRECTORY/" << endl;
        return 0;
    }


        // Grab every file in the directory and loop over it
        for (const auto &entry: fs::directory_iterator(path)) {
            currentName = entry.path(); // get current file name and path

            // read in our image
            img = imread(currentName);

            // display our image
            namedWindow("Original");
            imshow("Original", img);

            //setting the mouse callback for selecting the region with mouse
            setMouseCallback("Original", mouse_call);

            while (char(waitKey(1) != 'n')) {
                // listen for n key to move to next image in directory
            }


            // remove current windows to make way for new windows
            destroyAllWindows();
        }

    return 0;
}