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

/** Start sketching
  * 1) desaturate by luminousity
  * 2) Create New layer, Invert, GaussBlurr 5
  * 3) DODGE layers
  */
IplImage* gray(IplImage* img)
{
    IplImage *desat_img = cvCreateImage(cvSize(img->width,img->height),8,1);
    CvScalar s;
    CvScalar ls;

    for(int i=0; i<img->height; i++) {
        for(int j=0; j<img->width; j++) {

            s = cvGet2D( img, i, j);
            //cout<<s.val[0]<<" "<<s.val[1]<<" "<<s.val[2]<<endl;
            ls.val[0] = (s.val[0] + s.val[1] + s.val[2]);
            ls.val[1] = (s.val[0] + s.val[1] + s.val[2])/3;
            ls.val[2] = (s.val[0] + s.val[1] + s.val[2])/3;
            if(ls.val[0]>255) ls.val[0]=255;
            if(ls.val[1]>255) ls.val[1]=255;
            if(ls.val[2]>255) ls.val[2]=255;
            cvSet2D( img,i,j, ls );
        }
    }
    return img;
}

IplImage* sepia(IplImage* img)
{
    img = gray(img);
    
    CvScalar s,ls;
    for(int i=0; i<img->height; i++) {
        for(int j=0; j<img->width; j++) {

            s = cvGet2D( img, i, j);
            //cout<<s.val[0]<<" "<<s.val[1]<<" "<<s.val[2]<<endl;
            ls.val[2] = (s.val[2]*0.393 + s.val[1]*0.769 + s.val[0]*0.189);
            ls.val[1] = (s.val[2]*0.349 + s.val[1]*0.686 + s.val[0]*0.168);
            ls.val[0] = (s.val[2]*0.272 + s.val[1]*0.534 + s.val[0]*0.131);
            cvSet2D( img,i,j, ls );
        }
    }
    return img;
}


IplImage* old(IplImage* img)
{
    img = sepia(img);
    
    int w = img->width;
    w = w/10;
    int lines = rand()%w;
    vector<int> p(lines);
    map<int,bool> already;

    for(int i=0;i<lines;) {
      p[i] = rand()%(img->width);
      if(already[p[i]]) continue;
      else already[p[i]] = 1,i++;
    }

    sort(p.begin(),p.end());
    int pc = 0;

    CvScalar s,ls;
    for(int j=0; j<img->width; j++) {
      if(pc < p.size() && p[pc] == j) {
        pc += 1;
        for(int i=0; i<img->height; i++) {
           s = cvGet2D( img, i, j);
           ls.val[2] = 130;//(s.val[2]+s.val[1]+s.val[0])/4;
           ls.val[1] = 130;//(s.val[2]+s.val[1]+s.val[0])/4;
           ls.val[0] = 130;//(s.val[2]+s.val[1]+s.val[0])/4;
           cvSet2D( img,i,j, ls );
        }
      }
    }
    return img;
}

int main(int argc, char *argv[]) {
    //cout<<"Starting sepia...."<<endl;
    char saved_file[1024];
    char input_file[1024];
    strcpy(input_file,"/home/sketchit/sepia_input/");
    strcat(input_file, argv[1]);
    //cout<<"input: "<<input_file<<endl;
    strcpy(saved_file,"/home/sketchit/sepia_output/");
    strcat(saved_file, argv[1]);
    //cout<<"output: "<<saved_file;
    //cvNamedWindow("desat",1);
    IplImage *img =    cvLoadImage(input_file);
    //IplImage *mask =    cvLoadImage("/home/sketchit/backend/oldmask.jpg");

    img = sepia(img);

//    IplImage *sketch = Wrapper_sketch(img);
    //IplImage *sketch = old(img);
    cvSaveImage(saved_file, img);
    //cvShowImage("desat",img);
    //cvWaitKey(0);
    return 0;
}

