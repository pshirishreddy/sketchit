#include <iostream>
#include <highgui.h>
#include <cv.h>

using namespace std;

template<class T> class Image
{
  private:
  IplImage* imgp;
  public:
  Image(IplImage* img=0) {imgp=img;}
  ~Image(){imgp=0;}
  void operator=(IplImage* img) {imgp=img;}
  inline T* operator[](const int rowIndx) {
    return ((T *)(imgp->imageData + rowIndx*imgp->widthStep));}
};

typedef struct{
  unsigned char b,g,r;
} RgbPixel;

typedef struct{
  float b,g,r;
} RgbPixelFloat;

typedef Image<RgbPixel>       RgbImage;
typedef Image<RgbPixelFloat>  RgbImageFloat;
typedef Image<unsigned char>  BwImage;
typedef Image<float>          BwImageFloat;


void invert_image(IplImage *img) {

  RgbImage imgR(img);
  int height, width;

  for(int row=0; row<img->height; row++) {
    for(int col=0; col<img->width; col++) {
      imgR[row][col].b = 255-(float)imgR[row][col].b;
      imgR[row][col].g = 255-(float)imgR[row][col].g;
      imgR[row][col].r = 255-(float)imgR[row][col].r;
    }
  }
}

int main(int argc, char* argv[]) {
  IplImage *first = cvLoadImage("test.jpg");
  IplImage *second = cvCreateImage(cvSize(first->width, first->height),8,3);
  cvSmooth(first, second, CV_GAUSSIAN, 15,15);
  cvNamedWindow("test",1);
  cvShowImage("test",first);
  cvWaitKey(0);
  cvShowImage("test",second);
  cvWaitKey(0);
  invert_image(second);
  cvShowImage("test",second);
  cvWaitKey(0);

  return 0;
}

