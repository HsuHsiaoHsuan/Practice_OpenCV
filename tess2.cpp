// Recognize text on an image using Tesseract API and print it to the screen
// Usage: ./tess image.png

#include <tesseract/baseapi.h>
#include <tesseract/strngs.h>

#include <iostream>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>

using namespace cv;
using namespace std;

int main(int argc, char** argv)
{
    if (argc != 2)
    {
        std::cout << "Please specify the input image!" << std::endl;
        return -1;
    }

    Mat img = imread(argv[1]);

    if (img.empty())
    {
        cout << "Cannot open source image!" << endl;
        return -1;
    }

    Mat gray;
    cvtColor(img, gray, CV_BGR2GRAY);
    namedWindow("gray");
    imshow("gray", gray);
    // ...other image pre-processing here...

    // Pass it to Tesseract API
    tesseract::TessBaseAPI tess;
    tess.Init(NULL, "eng", tesseract::OEM_DEFAULT);
    tess.SetPageSegMode(tesseract::PSM_SINGLE_BLOCK);
    tess.SetImage((uchar*)gray.data, gray.cols, gray.rows, 1, gray.cols);

    // Get the text
    char* out = tess.GetUTF8Text();
    cout << out << endl;

    // Exit by pressing 'q'
    while(char(waitKey(1)) != 'q') {}

    return 0;
}
