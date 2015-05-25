#include <iostream>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>

using namespace std;
using namespace cv;

Mat image;

void rgb2cmyk(cv::Mat& src, std::vector<cv::Mat>& cmyk)
{
    CV_Assert(src.type() == CV_8UC3);

    cmyk.clear();
    for (int x = 0; x < 4; ++x)
    {
        cmyk.push_back(Mat(src.size(), CV_32F));
    }

    for (int x = 0; x < src.rows; ++x)
    {
        for (int y = 0; y < src.cols; ++y)
        {
            Vec3b p = src.at<Vec3b>(x,y);

            float r = p[2] / 255.;
            float g = p[1] / 255.;
            float b = p[0] / 255.;
            float k = (1 - max(max(r,g),b));

            cmyk[0].at<float>(x,y) = (1 - r - k) / (1 - k); 
            cmyk[1].at<float>(x,y) = (1 - g - k) / (1 - k);
            cmyk[2].at<float>(x,y) = (1 - b - k) / (1 - k);
            cmyk[3].at<float>(x,y) = k;
        }
    }
}

int main(int argc, char **argv)
{
    if (argc != 2)
    {
        cout << "Please specify the input image." << endl;
        return -1;
    }
    image = imread(argv[1], CV_LOAD_IMAGE_COLOR);
    namedWindow("original");
    imshow("original", image);
    
    vector<Mat> cmyk;
    rgb2cmyk(image, cmyk);
    namedWindow("C");
    imshow("C", cmyk[0]);
    namedWindow("M");
    imshow("M", cmyk[1]);
    namedWindow("Y");
    imshow("Y", cmyk[2]);
    namedWindow("K");
    imshow("K", cmyk[3]);

    // -----------------------------------------------------------------
    Mat im1;
    im1 = cmyk[3].mul(1 - cmyk[1]) > 0.25;
    imshow("im1", im1);

    Mat im2;
    im1.convertTo(im2, CV_8U);

    vector< vector<Point> > contours;
    findContours(im2, contours, CV_RETR_EXTERNAL, CV_CHAIN_APPROX_NONE);

    double max_area = 0;
    int max_idx = 0;
    for (int x = 0; x < contours.size(); x++)
    {
        double area = contourArea(contours[1]);
        max_idx = area > max_area ? x : max_idx;
        max_area = area > max_area ? area : max_area;
    }

    im2.setTo(Scalar(0));
    drawContours(im2, contours, max_idx, Scalar(255), -1);
    //imshow("im2", im2);

    Mat dst = im1.clone();
    findContours(dst.clone(), contours, CV_RETR_EXTERNAL, CV_CHAIN_APPROX_NONE);
    for (int x = 0; x < contours.size(); x++)
    {
        if (contourArea(contours[x]) < 100)
        {
            drawContours(dst, contours, x, Scalar(0), -1);
        }
    }
    imshow("dst", dst);
    // -----------------------------------------------------------------

    cout << "Press 'q' to quit..." << endl;
    while(1)
    {
        if(char(waitKey(1)) == 'q') break;
    }
    destroyAllWindows();

    return 0;
}
