# OpenCVRaspicamFrameBuffer
A simple program demonstrating use of the Raspicam with OpenCV and a Frame Buffer to a SPI TFT LCD
This is a program I put together from other works mainly as a demo. I wanted to get a Raspberry Pi Zero to use the Rasperry Pi Noir Camera working with the Pi Zero but I also wanted to put the image out to a SPI TFT 240x320 LCD that I had on hand. 

Some important things to note about this program: a lot depends on what the camera brings in. I used a Noir and this may differ in the color that comes in, so your setting 
  Camera.set( CV_CAP_PROP_FORMAT, CV_8UC1  );
is apt to be critical here. But I had for a long time some very funky colors on the LCD, looking like an acid scene from a 1960s movie. 
So the LCD itself and it's own color capabilities also wreak havoc here. This line:
 cvtColor(frame, frame2, COLOR_GRAY2BGR565);
 was found to work to finally give out what is essentially a gray image. I think other applications, like a color camera and more capable LCD will differ in the settings. 
 Note also if you are new to frame buffer usage, be mindful of the following:
  - the capture dimensions of the camera. You can almost get a "focus/expand" effect narrowing the FOV of the camera if you mix this up.
  - the detected resolution of your screen versus the virtual resolution. You can make the virtal bigger. 
  - the segfaults that will occur when you are putting "too many bits" into the frame buffer
 
 To compile and link the sample program:
 g++  -o raspistr raspistr.cpp `pkg-config opencv --cflags --libs` -lraspicam -lraspicam_cv -L/path/to/dir_containing/libpng -lpng
 
 yes it's rather redundant to use Raspicam and OpenCV...
 
 Also when you have hooked up your SPI  LCD screen, enter this command:
 
 sudo modprobe fbtft_device name=tm022hdh26 rotate=90
 
 Some other helpful links that helped me in this project:
 
 https://neverbenever.wordpress.com/2015/05/18/how-to-use-pi-camera-with-opencv-for-raspberry-pi/
 http://marcosgildavid.blogspot.com/2014/02/getting-ili9341-spi-screen-working-on.html
 http://raspberrycompote.blogspot.com/2016/
 
