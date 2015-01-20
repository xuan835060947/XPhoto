#include "PhotoProcessingActivity.h"

JNIEXPORT void JNICALL Java_com_example_photoprocessing_activity_PhotoProcessingActivity_grayPhoto(
		JNIEnv *env, jobject photoProcessingActivity, jobject bmOriginal,
		jobject bmGray) {
	AndroidBitmapInfo origanalColor;
	void* pixelscolor;
	AndroidBitmapInfo infogray;
	void* pixelsgray;
	int ret;
	int y;
	int x;

	LOGI("In convertToGray");
	if ((ret = AndroidBitmap_getInfo(env, bmOriginal, &origanalColor)) < 0) {
		LOGE("AndroidBitmap_getInfo() failed ! error=%d", ret);
		return;
	}

	if ((ret = AndroidBitmap_getInfo(env, bmGray, &infogray)) < 0) {
		LOGE("AndroidBitmap_getInfo() failed ! error=%d", ret);
		return;
	}
	LOGI(
			"original image :: width is %d; height is %d; stride is %d; format is %d;flags is	%d,stride is %u", origanalColor.width, origanalColor.height, origanalColor.stride, origanalColor.format, origanalColor.flags, origanalColor.stride);

	if (origanalColor.format != ANDROID_BITMAP_FORMAT_RGBA_8888) {
		LOGE("Bitmap format is not RGBA_8888 !");
		//return;
	}

	if (origanalColor.format == ANDROID_BITMAP_FORMAT_RGB_565) {
		LOGI("Original Image is ANDROID_BITMAP_FORMAT_RGB_565");

		if ((ret = AndroidBitmap_lockPixels(env, bmOriginal, &pixelscolor))
				< 0) {
			LOGE("AndroidBitmap_lockPixels() failed ! error=%d", ret);
		}

		if ((ret = AndroidBitmap_lockPixels(env, bmGray, &pixelsgray)) < 0) {
			LOGE("AndroidBitmap_lockPixels() failed ! error=%d", ret);
		}
		// modify pixels with image processing algorithm
		for (y = 0; y < origanalColor.height; y++) {
			__uint16_t * line = (__uint16_t *) pixelscolor;
			uint8_t * grayline = (uint8_t *) pixelsgray;
			for (x = 0; x < origanalColor.width; x++) {
				grayline[x] = (uint8_t)(((line[x] >> 11 <<3) + (line[x] >> 5 & 63 * 16) + (line[x]&31 * 8)) / 3);
				//LOGI("%d %d %d %d",line[x].alpha,line[x].red,line[x].green,line[x].blue);
				/*if(x ==0){
				LOGI("line:%o  grayline %o ",line[x],grayline[x]);
				}*/
			}
			pixelscolor = (char *) pixelscolor + origanalColor.stride;
			pixelsgray = (char *) pixelsgray + infogray.stride;
		}

		LOGI("unlocking pixels");
		AndroidBitmap_unlockPixels(env, bmOriginal);
		AndroidBitmap_unlockPixels(env, bmGray);
		LOGI("Return !! ");
		return ;
	}

	LOGI(
			"gray image :: width is %d; height is %d; stride is %d; format is %d;flags is %d %d,stride is %u", infogray.width, infogray.height, infogray.stride, infogray.format, infogray.flags, infogray.stride);
	if (infogray.format != ANDROID_BITMAP_FORMAT_A_8) {
		LOGE("Bitmap format is not A_8 !");
		return;
	}

	if ((ret = AndroidBitmap_lockPixels(env, bmOriginal, &pixelscolor)) < 0) {
		LOGE("AndroidBitmap_lockPixels() failed ! error=%d", ret);
	}

	if ((ret = AndroidBitmap_lockPixels(env, bmGray, &pixelsgray)) < 0) {
		LOGE("AndroidBitmap_lockPixels() failed ! error=%d", ret);
	}

	// modify pixels with image processing algorithm
	for (y = 0; y < origanalColor.height; y++) {
		argb * line = (argb *) pixelscolor;
		uint8_t * grayline = (uint8_t *) pixelsgray;
		for (x = 0; x < origanalColor.width; x++) {
			grayline[x] = (line[x].red + line[x].green + line[x].blue) / 3;
		}
		pixelscolor = (char *) pixelscolor + origanalColor.stride;
		pixelsgray = (char *) pixelsgray + infogray.stride;
	}

	LOGI("unlocking pixels");
	AndroidBitmap_unlockPixels(env, bmOriginal);
	AndroidBitmap_unlockPixels(env, bmGray);
}

