#include <iostream>
#include <cv.h>
#include <highgui.h>
#include <math.h>
#include <dirent.h>
#include <string.h>
#include <list>
#include <map>
#include <fstream>

using namespace std;

struct coord{
    float x;
    float y;
};

/** Start sketching
  * 1) desaturate by luminousity
  * 2) Create New layer, Invert, GaussBlurr 5
  * 3) DODGE layers
  */
IplImage* desat_lum(IplImage* img)
{
    IplImage *desat_img = cvCreateImage(cvSize(256,256),8,1);
    IplImage *res_img = cvCreateImage(cvSize(256,256),8,3);
    cvResize(img, res_img, CV_INTER_LINEAR);

    //cvFillImage( desat_img,255);
    CvScalar s;
    CvScalar ls;

    for(int i=0; i<256; i++) {
        for(int j=0; j<256; j++) {

            s = cvGet2D( res_img, i, j);
            ls.val[0] = 0.07*s.val[0] + 0.72*s.val[1] + 0.21*s.val[2];
            cvSet2D( desat_img,i,j, ls );
        }
    }
    return desat_img;
}

IplImage* layer_invert_blur(IplImage* img)
{
    IplImage *dup_lay = cvCreateImage(cvSize(256,256),8,1);
    cvCopyImage(img, dup_lay);
    CvScalar s;

    for(int i=0; i<256; i++) {
        for(int j=0; j<256; j++) {
            s = cvGet2D( dup_lay, i, j);
            s.val[0] = 255-s.val[0];
            cvSet2D( dup_lay, i, j, s);
        }
    }
    cvSmooth( dup_lay, dup_lay, CV_GAUSSIAN,17,17);
    return dup_lay;

}

IplImage* layer_dodge(IplImage* topLayer, IplImage* bottomLayer)
{
    CvScalar st, sb, sr;
    IplImage* res_img = cvCreateImage(cvSize(256,256),8,1);
    for(int i=0; i<256; i++) {
        for(int j=0; j<256; j++) {
            st = cvGet2D( topLayer, i, j); sb = cvGet2D( bottomLayer, i, j);
            sr.val[0] = (sb.val[0]*256)/((255-st.val[0])+1);
            cvSet2D( res_img, i, j, sr);
        }
    }
    return res_img;
}

IplImage* Wrapper_sketch(IplImage* img)
{
    IplImage *desat_img, *layer_inv_blur;
    desat_img = desat_lum(img);
    layer_inv_blur = layer_invert_blur( desat_img);

    IplImage *sketch = layer_dodge( layer_inv_blur, desat_img );
    cvReleaseImage( &desat_img );
    cvReleaseImage( &layer_inv_blur);
    return sketch;
}

int main() {
    cvNamedWindow("desat",1);
    IplImage *img =    cvLoadImage("image11.jpg");
    IplImage *sketch = Wrapper_sketch(img);
    cvShowImage("desat",sketch);
    cvWaitKey(0);
    return 0;
}

