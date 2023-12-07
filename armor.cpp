#include <opencv2/imgcodecs.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include <iostream>
using namespace std;
using namespace cv;


int hmin = 0, hmax = 94, smin = 0, smax = 120, vmin = 250, vmax = 255;
int kernelsize = 10, gaussx = 3, gaussy = 8, gausscoresize = 11, cannyx = 99, cannyy = 206, Corpointmax=7, Corpointmin=4, arealimit=10,hw=3;

void getContours(Mat img, Mat imgout)
{
	vector<vector<Point>> contours;
	vector<Vec4i> hierarchy;
	findContours(img, contours, hierarchy, RETR_EXTERNAL, CHAIN_APPROX_SIMPLE);
	vector<vector<Point>> conPoly(contours.size());
	vector<Rect>boundbox(contours.size());
	for (int i = 0; i < contours.size(); i++)
	{
		int area = contourArea(contours[i]);
		float peri = arcLength(contours[i], true);
		if (area > arealimit*50) { approxPolyDP(contours[i], conPoly[i], 0.02 * peri, true);
		}
		int objCor = (int)conPoly[i].size();
		if (objCor<=Corpointmax && objCor>=Corpointmin){
			boundbox[i] = boundingRect(conPoly[i]);
			if (boundbox[i].height / boundbox[i].width > hw/2) {
				rectangle(imgout, boundbox[i].tl(), boundbox[i].br(), Scalar(255, 0, 255), 2);
			}
		}
	}
}

int main()
{
	string path = "C:\\Users\\north\\Desktop\\learning materials and works\\Data\\1.jpg";
	Mat img = imread(path);
	Mat imgHSV, mask, imgErode, imgGauss, imgDil, imgCanny,imgRec;
	cvtColor(img, imgHSV, COLOR_BGR2HSV);
	namedWindow("trackbar", (640, 400));
	createTrackbar("Hmin", "trackbar", &hmin, 179);
	createTrackbar("Hmax", "trackbar", &hmax, 179);
	createTrackbar("Smin", "trackbar", &smin, 255);
	createTrackbar("Smax", "trackbar", &smax, 255);
	createTrackbar("Vmin", "trackbar", &vmin, 255);
	createTrackbar("Vmax", "trackbar", &vmax, 255);
	createTrackbar("kernelsize", "trackbar", &kernelsize, 100);
	createTrackbar("gaussx", "trackbar", &gaussx, 200);
	createTrackbar("gaussy", "trackbar", &gaussy, 200);
	createTrackbar("gausscoresize", "trackbar", &gausscoresize, 99);
	createTrackbar("cannyx", "trackbar", &cannyx, 500);
	createTrackbar("cannyy", "trackbar", &cannyy, 500);
	createTrackbar("Corpointmin", "trackbar", &Corpointmin, 30);
	createTrackbar("Corpointmax", "trackbar", &Corpointmax, 30);
	createTrackbar("Area", "trackbar", &arealimit,10000 );
	createTrackbar("HW", "trackbar", &hw, 1000);
	while (true)
	{
		imshow("Image", img);
		Mat kernel = getStructuringElement(MORPH_RECT, Size(kernelsize, kernelsize));
		Scalar lower(hmin, smin, vmin);
		Scalar upper(hmax, smax, vmax);
		inRange(imgHSV, lower, upper, mask);
		imshow("Mask", mask);
		GaussianBlur(mask, imgGauss, Size(2 * gausscoresize + 1, 2 * gausscoresize + 1), gaussx, gaussy);
		Canny(imgGauss, imgCanny, cannyx, cannyy);
		dilate(imgCanny, imgDil, kernel);
		erode(imgDil, imgErode, kernel);
		//imshow("ImgDil", imgDil);
		imgRec = imgErode.clone();
		getContours(imgDil, imgRec);
		imshow("ImgRec", imgRec);
		waitKey(1);
	}
	return 0;
}


/*void main()
{
	string path = "Resources/Destiny 2 2023-09-04 14-14-59.mp4";
	VideoCapture cap(path);
	Mat img;
	while (true) {
		cap.read(img);
		Mat imgHSV;
		cvtColor(img, imgHSV, COLOR_BGR2HSV);
		imshow("Video", img);
		waitKey(1);
	}

}*/