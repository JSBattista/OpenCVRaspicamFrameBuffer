/*


 To compile and link this program
sudo g++  -o raspistr raspistr.cpp `pkg-config opencv --cflags --libs` -lraspicam -lraspicam_cv -L/path/to/dir_containing/libpng -lpng


*/
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <linux/fb.h>
#include <sys/mman.h>
#include <sys/ioctl.h>
#include <iostream>
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include <raspicam/raspicam_cv.h>
using namespace std;
using namespace cv;
int main(int argc, char* argv[])
{
	  raspicam::RaspiCam_Cv Camera;
	  int fbfd = 0;
	  char *fbp = 0;
  	struct fb_fix_screeninfo finfo;
  	struct fb_var_screeninfo vinfo;
  	memset(&finfo, 0, sizeof(finfo));
  	memset(&vinfo, 0, sizeof(vinfo));
	// Open the file for reading and writing
  	fbfd = open("/dev/fb1", O_RDWR);
  	if (ioctl(fbfd, FBIOGET_FSCREENINFO, &finfo)) {
     		printf("Unable to read fixed screen information");
     		return 0;
 	  } // if
  	if (ioctl(fbfd, FBIOGET_VSCREENINFO, &vinfo)) {
    		printf( "Unable to read variable screen information");
     		return 0;
  	} // if
    //	may conflict with capture dimensions
    Camera.set( CV_CAP_PROP_FORMAT, CV_8UC1  );  // Be mindful of what you can get from the camera.
	  Camera.set(CV_CAP_PROP_FRAME_WIDTH, vinfo.xres);
	  Camera.set(CV_CAP_PROP_FRAME_HEIGHT, vinfo.yres);
    if (!Camera.open()) {cerr<<"Error opening the camera"<<endl;return -1;}
    vinfo.bits_per_pixel = 16;
    vinfo.xres_virtual = vinfo.xres;
    vinfo.yres_virtual = vinfo.yres*2;
	  vinfo.xoffset = 0;
	  vinfo.yoffset = 0;
   	if (ioctl(fbfd, FBIOPUT_VSCREENINFO, &vinfo)) {
      		printf("Error setting variable information.\n");
   	} // if
  	fbp = (char*)mmap(0, 
                    	  finfo.smem_len, 
                    	  PROT_READ | PROT_WRITE, 
                    	  MAP_SHARED, 
                    	  fbfd,
	 		  0);
	uchar *data;
	Mat frame, frame2;
	while(true)	{
        	Camera.grab();
       	 	Camera.retrieve ( frame );
          //process the frame...
		      cvtColor(frame, frame2, COLOR_GRAY2BGR565);// Camera dictates "from what" and screen dictates "to what"
          // Also, if you go to HDMI output you can even skip this step and put the frame .data right to tbe buffer and it
          // gets a little faster
		      //	threshold(frame, frame2, 128, 255, CV_THRESH_BINARY);
		      data = frame2.data;
		      if ((int)fbp == -1) {
			      printf("Failed to mmap.\n");
			      break;
		      } else {
			      memcpy(fbp, data, finfo.smem_len);
		      }  // if
	   }
  	// cleanup
  	Camera.release();
  	munmap(fbp, finfo.smem_len);
  	close(fbfd);
  	return 0;
}
