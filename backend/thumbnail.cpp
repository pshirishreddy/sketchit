#include <iostream>
#include <cv.h>
#include <highgui.h>
#include <math.h>
#include <dirent.h>
#include <string.h>
#include <list>
#include <map>
#include <fstream>
#include <vector>

using namespace std;

struct coord{
    float x;
    float y;
};

IplImage* thumbnail(IplImage* img)
{
    IplImage *dst = cvCreateImage(cvGetSize(img), IPL_DEPTH_16S, 3);
    cvLaplace(img, dst);
    return dst;

/*    IplImage *desat_img = cvCreateImage(cvSize(img->width/10,img->height/10),8,1);
    CvScalar s;
    CvScalar ls;

    for(int i=0; i<img->height; i+=10) {
        for(int j=0; j<img->width; j+=10) {
            s = cvGet2D( img, i, j);
            //cout<<s.val[0]<<" "<<s.val[1]<<" "<<s.val[2]<<endl;
            cvSet2D( desat_img,i,j, s );
        }
    }
    return desat_img;*/
}

int main(int argc, char *argv[]) {
    //cout<<"Starting sepia...."<<endl;
    char saved_file[1024];
    char input_file[1024];
    strcpy(input_file,"/home/sketchit/backend/");
    strcat(input_file, argv[1]);
    //cout<<"input: "<<input_file<<endl;
    strcpy(saved_file,"/home/sketchit/output/");
    strcat(saved_file, argv[1]);
    //cout<<"output: "<<saved_file;
    cvNamedWindow("desat",1);
    IplImage *img =    cvLoadImage(input_file);
    //IplImage *mask =    cvLoadImage("/home/sketchit/backend/oldmask.jpg");

    img = thumbnail(img);

//    IplImage *sketch = Wrapper_sketch(img);
    //IplImage *sketch = old(img);
    //cvSaveImage(saved_file, img);
    cvShowImage("desat",img);
    cvWaitKey(0);
    return 0;
}

