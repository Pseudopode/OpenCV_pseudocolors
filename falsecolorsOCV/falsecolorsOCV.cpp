//Copyright (c) 2012 Jean-Philippe Deblonde.
//Do what you want with the code. Cite me if you do anything useful
//with it.
//I'm not reponsible for any problem/damaged done by using that code...

//change the following include lines to match your installation
//don't forget the linked libraries in the project's properties !
#include "C:\OpenCV2.1\include\opencv\cv.h"
#include "C:\OpenCV2.1\include\opencv\cxcore.h"
#include "C:\OpenCV2.1\include\opencv\highgui.h"

void evaluatePixel(IplImage *src, IplImage *heatmap, IplImage *dst){

	IplImage *R, *G,*B;

	//we split the heatmap along the 3 channels
	R = cvCreateImage(cvGetSize(heatmap),heatmap->depth,1);
	G = cvCloneImage(R);
	B = cvCloneImage(R);
	cvSplit(heatmap,B,G,R,NULL);

	for(int x=0; x<src->width; x++){
		for(int y=0;y<src->height; y++){
			//memory access to the destination color image (faster than splitting the 3 channels...)
			unsigned char *data=&((unsigned char*)(dst->imageData + dst->widthStep*y ))[x*3];
			
			//read the intensity value in the grayscale image
			unsigned char gray=src->imageData[src->widthStep*y + x];

			//remember, OpenCV store images as BGR internally ! 
			//So access [2] for Red, [1] for Green et [3] for Blue
			data[2] = cvGet2D(R,gray/255.0*500,1).val[0]; //Red channel
			data[1] = cvGet2D(G,gray/255.0*500,1).val[0]; //Green channel
			data[0] = cvGet2D(B,gray/255.0*500,1).val[0]; //Blue channel
		}
	}

}

int main(int argc, char* argv[])
{
	//sanity check
	if(argc <= 1 ) {
		printf("Usage : falsecolorsOCV.exe input_img_gray input_colormap\n");
		return 1;
	}

	IplImage *img = cvLoadImage(argv[1],CV_LOAD_IMAGE_GRAYSCALE); //the input image, grayscale
	IplImage *heatmap = cvLoadImage(argv[2],CV_LOAD_IMAGE_COLOR); //the heatmap image, color
	IplImage *dst = cvCreateImage(cvGetSize(img),8,3); //the result image, color

	evaluatePixel(img, heatmap, dst);

	printf("done !\n");
	printf("look at false_colors.bmp...\n");

	cvSaveImage("false_colors.bmp",dst);
	
	return 0;
}

