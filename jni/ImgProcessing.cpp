#include <com_example_photoprocessing_img_ImgProcessing.h>

#include <stdio.h>

#include <stdlib.h>

#include <opencv2/opencv.hpp>

using namespace cv;

JNIEXPORT void JNICALL Java_com_example_photoprocessing_img_ImgProcessing_openCVBlur
  (JNIEnv * env, jobject activity, jstring originalImg, jstring newImg){
	const char * imgName;
	imgName = env->GetStringUTFChars(originalImg, 0);
//	Mat img = imread(originalImg);
}
