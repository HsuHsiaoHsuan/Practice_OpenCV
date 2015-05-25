#include <opencv2/opencv.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>

using namespace std;
using namespace cv;

Mat image;
Mat dst;

const int rotate_slider_max = 3;           // Rotate
int rotate_slider_value;
const int gray_slider_max = 1;             // Gray
int gray_slider_value;
const int BLUR_MAX = 31;
const int blur_slider_max = BLUR_MAX;      // Homogeneous blur
int blur_slider_value;
const int gaussian_slider_max = BLUR_MAX;  // Gaussian blur
int gaussian_slider_value;
const int median_slider_max = BLUR_MAX;    // Median blur
int median_slder_value;
const int bilateral_slider_max = BLUR_MAX; // Bilateral filter
int bilateral_slider_value;

const char* window_before = "Original image";
const char* window_after = "Modified image";

// ROTATE ----
void rotate(int angle) {
    angle = 90 * angle;
    bool const flip_horizontal_or_vertical = angle > 0 ? 1 : 0;
    int const number = abs(angle / 90);
    for (int x = 0; x != number; ++x) {
        transpose(dst, dst);
        flip(dst, dst, flip_horizontal_or_vertical);
    }
    imshow(window_after, dst);
}

void on_trackbar_rotate(int, void*) {
    rotate(rotate_slider_value);
}

// GRAY ----
void gray(int isGray) {
    if (isGray == 1) {
        cvtColor(dst, dst, CV_BGR2GRAY);
    } else {
    } 
    imshow(window_after, dst);
}

void on_trackbar_gray(int, void*) {
    gray(gray_slider_value);
}

// HOMOGENEOUS BLUR ----
void homogeneous(int size) {
    if (size > 0) {
        // Mat tmp;
        // dst.copyTo(tmp);
        blur(dst, dst, Size(size, size));
        imshow(window_after, dst);
    }
}

void on_trackbar_blur(int, void*) {
    homogeneous(blur_slider_value);
}
// MAIN ----
int main() {
    VideoCapture cap(0);

    if (!cap.isOpened()) {
        cout << "Camera can't be opened!" << endl;
        return -1;
    }

    namedWindow(window_before);

    createTrackbar("Rotate",      window_before, &rotate_slider_value, rotate_slider_max, on_trackbar_rotate);
    createTrackbar("Gray",        window_before, &gray_slider_value,   gray_slider_max,   on_trackbar_gray);
    createTrackbar("Homogeneous", window_before, &blur_slider_value,   blur_slider_max,   on_trackbar_blur);

    while (char(waitKey(1)) != 'q' && cap.isOpened()) {
        cap >> image;
        if (image.empty()) {
            cout << "Camera Over!" << endl;
        }
        image.copyTo(dst);
        imshow(window_before, image);
        rotate(rotate_slider_value);
        gray(gray_slider_value);
    }

    return 0;
}
