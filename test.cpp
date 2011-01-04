#include <iostream>
#include <highgui.h>
#include <cv.h>

using namespace std;

int main(int argc, char* argv[]) {
  IplImage *first = cvLoadImage("test.jpg");
  IplImage *second = cvCreateImage(cvSize(first->width, first->height),8,3);
  cvSmooth(first, second, CV_GAUSSIAN, 15,15);
  cvNamedWindow("test",1);
  cvShowImage("test",first);
  cvWaitKey(0);
  cvShowImage("test",second);
  cvWaitKey(0);

  return 0;
}

