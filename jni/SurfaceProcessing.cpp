/* DO NOT EDIT THIS FILE - it is machine generated */
#include <SurfaceProcessingActivity.h>
extern "C" {
#include <jpeg/jpeglib.h>
}
#include <setjmp.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>

int read_jpeg_file(unsigned char ** img, int width, int height,
		ANativeWindow_Buffer& nwBuffer);

void circleFuzzy2dir(unsigned char ** img, unsigned char ** imgtmp, int width,
		int height, int component, int x0, int y0, int r);
void GaussianKerneluLong(unsigned long * & kernel, int sigma, int& ksize);
void freeGaussianKerneluLong(unsigned long * & kernel);
void GaussianKernelulong(double ** kernel, int ksize,
		unsigned long ** & kernelulong, unsigned long scale);
void GaussianKernelInt(double ** kernel, int ksize, int ** & kernelint,
		unsigned int scale);
void GaussianSmooth2D(double sigma, double ** & kernel, int& ksize);

void fuzzy2dir(unsigned char ** img, unsigned char ** imgtmp, int width,
		int height, int component, int x0, int y0, int r);
int circleFuzzy(unsigned char ** img, unsigned char ** imgtmp, int width,
		int height, int component, int x0, int y0, int r);
int circleFuzzyDouble(unsigned char ** img, unsigned char ** imgtmp, int width,
		int height, int component, int x0, int y0, int r);
int pow2minus(int x, int x0);



clock_t start, finish;
double duration;

JNIEXPORT void JNICALL Java_com_example_photoprocessing_activity_SurfaceProcessingActivity_grayPhoto(
		JNIEnv * env, jobject activity, jobject surface) {
	ANativeWindow_Buffer nwBuffer;

	LOGI("ANativeWindow_fromSurface ");
	ANativeWindow * mANativeWindow = ANativeWindow_fromSurface(env, surface);

	if (mANativeWindow == NULL) {
		LOGE("ANativeWindow_fromSurface error");
		return;
	}

	LOGI("ANativeWindow_lock ");
	if (0 != ANativeWindow_lock(mANativeWindow, &nwBuffer, 0)) {
		LOGE("ANativeWindow_lock error");
		return;
	}

	LOGI("ANativeWindow_lock nwBuffer->format ");
	if (nwBuffer.format == WINDOW_FORMAT_RGB_565) {
		int y, x;
		LOGI("nwBuffer->format == WINDOW_FORMAT_RGB_565");
		__uint16_t * line = (__uint16_t *) nwBuffer.bits;
		// modify pixels with image processing algorithm
		for (y = 0; y < nwBuffer.height; y++) {

			for (x = 0; x < nwBuffer.width; x++) {
				//RGB565的灰色十进制为1280
				line[x] = 57083;
			}
			line = line + nwBuffer.stride;
		}
	}
	LOGI("ANativeWindow_unlockAndPost ");
	if (0 != ANativeWindow_unlockAndPost(mANativeWindow)) {
		LOGE("ANativeWindow_unlockAndPost error");
		return;
	}

	ANativeWindow_release(mANativeWindow);
	LOGI("ANativeWindow_release ");
}

int read_jpeg_file_show(const char *input_filename,
		ANativeWindow_Buffer& nwBuffer) {
	struct jpeg_decompress_struct cinfo;
	struct jpeg_error_mgr jerr;
	FILE *input_file;
	JSAMPARRAY buffer;
	int row_width;

	unsigned char *buffertmp;
//        unsigned char *tmp = NULL;

	cinfo.err = jpeg_std_error(&jerr);

	if ((input_file = fopen(input_filename, "rb")) == NULL) {
		fprintf(stderr, "can't open %s\n", input_filename);
		LOGI("can't open jpg1");
		return -1;
	}

	//初始化信息
	jpeg_create_decompress(&cinfo);
	LOGI("初始化信息");

	/* Specify data source for decompression */
	//指定图片
	jpeg_stdio_src(&cinfo, input_file);
	LOGI("指定图片");

	/* Read file header, set default decompression parameters */
	(void) jpeg_read_header(&cinfo, TRUE);
	LOGI("读取头信息, set default decompression parameters ");

	/* Start decompressor */
	(void) jpeg_start_decompress(&cinfo);
	LOGI("解压");

	row_width = cinfo.output_width * cinfo.output_components;
	LOGI(
			"图片的宽:%d 图片的高%d 颜色长度:%d", cinfo.output_width, cinfo.output_height, cinfo.output_components);

	buffer = (*cinfo.mem->alloc_sarray)((j_common_ptr) &cinfo, JPOOL_IMAGE,
			row_width, 1);

	//一行
	buffertmp = (unsigned char *) malloc(row_width);
	memset(buffertmp, 0, row_width);
	LOGI("malloc and memset");

	/* Process data */
	int get8h5 = 248, get8h6 = 252;
	__uint16_t * line = (__uint16_t *) nwBuffer.bits;
	int wheight = 0;
	int scalew = 1, scaleh = 1;
//	int height =
//			cinfo.output_height < nwBuffer.height ?
//					cinfo.output_height : nwBuffer.height;
	if (cinfo.output_width > nwBuffer.width) {
		scalew = cinfo.output_width / nwBuffer.width;
	}

	LOGI(" scale of img = %d", scalew);

	for (int i = 0, choosehNum = 0; i < cinfo.output_height; i++) {
		//获得一行
		jpeg_read_scanlines(&cinfo, buffer, 1);
		buffertmp = *buffer;
		//根据缩放选取行
		if (i % scalew == 0 && choosehNum++ < nwBuffer.height) {

			//LOGI("nwBuffer->format == WINDOW_FORMAT_RGB_565");
			for (int j = 0, choosewNum = 0; j < cinfo.output_width; j++) {
				if (j % scalew == 0) {
					if (nwBuffer.format == WINDOW_FORMAT_RGB_565) {
						line[choosewNum] = ((__uint16_t ) buffertmp[3 * j + 0]
								& get8h5) << 8
								| ((__uint16_t ) (buffertmp[3 * j + 1] & get8h6)
										<< 3)
								| ((__uint16_t ) (buffertmp[3 * j + 2] & get8h6)
										>> 3);
						choosewNum++;
					}
				}

			}
			line = line + nwBuffer.stride;
		}
	}

//                memcpy(tmp, *buffer, row_width);
//                tmp += row_width;

	(void) jpeg_finish_decompress(&cinfo);
	LOGI("jpeg_finish_decompress !!");

	jpeg_destroy_decompress(&cinfo);
	LOGI("jpeg_destroy_decompress !!");

	/* Close files, if we opened them */
	fclose(input_file);

	return 0;
}

JNIEXPORT void JNICALL Java_com_example_photoprocessing_activity_SurfaceProcessingActivity_showJPG(
		JNIEnv * env, jobject activity, jobject surface, jstring img) {
	const char * imgChar;
	imgChar = env->GetStringUTFChars(img, 0);
	ANativeWindow_Buffer nwBuffer;

	LOGI("img path : %s  ", imgChar);

	LOGI("ANativeWindow_fromSurface ");
	ANativeWindow * mANativeWindow = ANativeWindow_fromSurface(env, surface);

	ANativeWindow_acquire(mANativeWindow);

	if (mANativeWindow == NULL) {
		LOGE("ANativeWindow_fromSurface error");
		return;
	}

	LOGI("ANativeWindow_lock ");
	if (0 != ANativeWindow_lock(mANativeWindow, &nwBuffer, 0)) {
		LOGE("ANativeWindow_lock error");
		return;
	}

	read_jpeg_file_show(imgChar, nwBuffer);
//	struct jpeg_decompress_struct cinfo;
//	unsigned char ** imgBit;
//	read_jpeg_file("/sdcard/tmp/xuan.jpg", cinfo, imgBit);
//	read_jpeg_file(imgBit, cinfo.output_width, cinfo.output_height, nwBuffer);

	if (nwBuffer.format == WINDOW_FORMAT_RGBA_8888) {
		LOGI("nwBuffer->format == WINDOW_FORMAT_RGBA_8888 ");
	}

	LOGI("ANativeWindow_unlockAndPost ");
	if (0 != ANativeWindow_unlockAndPost(mANativeWindow)) {
		LOGE("ANativeWindow_unlockAndPost error");
		return;
	}

	env->ReleaseStringUTFChars(img, imgChar);
	ANativeWindow_release(mANativeWindow);
	LOGI("ANativeWindow_release ");
	return;
}

JNIEXPORT void JNICALL Java_com_example_photoprocessing_activity_SurfaceProcessingActivity_fuzzyImg(
		JNIEnv * env, jobject activity, jstring originalImg, jstring newImg) {
	struct jpeg_decompress_struct cinfo;
	unsigned char ** img, **imgtmp;
	const char * oImg;
	const char * nImg;

	oImg = env->GetStringUTFChars(originalImg, 0);
	nImg = env->GetStringUTFChars(newImg, 0);

	//oImg原图片的路径
	//读取图片RGB数据到img中,解压信息保存在cinfo中
	read_jpeg_file(oImg, cinfo, img);
	read_jpeg_file(oImg, cinfo, imgtmp);

	LOGI("read_jpeg_file 结束  ");

	//模糊图片
	LOGI(
			"图片的宽:%d 图片的高%d 颜色长度:%d", cinfo.output_width, cinfo.output_height, cinfo.output_components);

	start = clock();
	circleFuzzy2dir(img, imgtmp, cinfo.output_width, cinfo.output_height,
			cinfo.output_components, 50, 150, 200);
	finish = clock();
	duration = (double) (finish - start) / CLOCKS_PER_SEC;
	LOGI("花费时间 :    %f ", duration);

	//根据img数据生成图片nImg
	LOGI(
			"图片的宽:%d 图片的高%d 颜色长度:%d", cinfo.output_width, cinfo.output_height, cinfo.output_components);

	mkjpeg(cinfo.output_width, cinfo.output_height, img, nImg);

	env->ReleaseStringUTFChars(originalImg, oImg);
	env->ReleaseStringUTFChars(newImg, nImg);
	freeImg(img, cinfo.output_height);
	freeImg(imgtmp, cinfo.output_height);
}

//高斯分离模糊
void circleFuzzy2dir(unsigned char ** img, unsigned char ** imgtmp, int width,
		int height, int component, int x0, int y0, int r) {
	LOGI("circleFuzzy ");
	//高斯矩阵
	unsigned long * kernelulong;
	double sigma = 10.0;
	int ksize;
	GaussianKerneluLong(kernelulong, 10, ksize);
	LOGI("获得高斯矩阵  ");

	//3处理函数
	int powr2 = r * r;
	//待检区域
	int checkl, checkt, checkr, checkb;
	checkl = x0 - r > 0 ? x0 - r : 0;
	checkt = y0 - r > 0 ? y0 - r : 0;
	checkr = x0 + r >= width ? width : x0 + r;
	checkb = y0 + r >= height ? height : y0 + r;

	//免检区域
	int notcl, notct, notcr, notcb;
	//sqrt(2)/2
	float sqrt2half = 0.7071;
	int nr = sqrt2half * r;
	notcl = x0 - nr > 0 ? x0 - nr : 0;
	notct = y0 - nr > 0 ? y0 - nr : 0;
	notcr = x0 + nr >= width ? width - 1 : x0 + nr;
	notcb = y0 + nr >= height ? height - 1 : y0 + nr;

	int kcenter = ksize / 2;
	int i = 0, j = 0;
	unsigned long sum;
	//x方向一维高斯模糊
	for (int y = 0; y < height; y++) {
		for (int x = 0; x < width; x++) {
			if (y >= checkt && y <= checkb && x >= checkl && x <= checkr) {
				//判断是否在免检区域内
				if (y >= notct && y <= notcb && x >= notcl && x <= notcr) {

				} else {
					//判断是否在圆内
					if (pow2minus(y, y0) + pow2minus(x, x0) <= powr2) {
						//在圆内
					} else { //不在圆内
						sum = 0;
						unsigned long rmul = 0, gmul = 0, bmul = 0;
						for (i = -kcenter; i <= kcenter; i++) {
							if ((x + i) >= 0 && (x + i) < width) {
								rmul += img[y][3 * (x + i)]
										* kernelulong[kcenter + i];
								gmul += img[y][3 * (x + i) + 1]
										* kernelulong[kcenter + i];
								bmul += img[y][3 * (x + i) + 2]
										* kernelulong[kcenter + i];
								sum += kernelulong[kcenter + i];
							}
						}
						imgtmp[y][3 * x] = rmul / sum;
						imgtmp[y][3 * x + 1] = gmul / sum;
						imgtmp[y][3 * x + 2] = bmul / sum;
					}
				}
			} else {
				sum = 0;
				unsigned long rmul = 0, gmul = 0, bmul = 0;
				for (i = -kcenter; i <= kcenter; i++) {
					if ((x + i) >= 0 && (x + i) < width) {
						rmul += img[y][3 * (x + i)] * kernelulong[kcenter + i];
						gmul += img[y][3 * (x + i) + 1]
								* kernelulong[kcenter + i];
						bmul += img[y][3 * (x + i) + 2]
								* kernelulong[kcenter + i];
						sum += kernelulong[kcenter + i];
					}
				}
				imgtmp[y][3 * x] = rmul / sum;
				imgtmp[y][3 * x + 1] = gmul / sum;
				imgtmp[y][3 * x + 2] = bmul / sum;
			}

		}
	}
	//y方向
	for (int x = 0; x < width; x++) {
		for (int y = 0; y < height; y++) {
			if (y >= checkt && y <= checkb && x >= checkl && x <= checkr) {
				//判断是否在免检区域内
				if (y >= notct && y <= notcb && x >= notcl && x <= notcr) {

				} else {
					//判断是否在圆内
					if (pow2minus(y, y0) + pow2minus(x, x0) <= powr2) {

					} else { //不在圆内
						sum = 0;
						unsigned long rmul = 0, gmul = 0, bmul = 0;
						for (i = -kcenter; i <= kcenter; i++) {
							if ((y + i) >= 0 && (y + i) < height) {
								rmul += imgtmp[y + i][3 * x]
										* kernelulong[kcenter + i];
								gmul += imgtmp[y + i][3 * x + 1]
										* kernelulong[kcenter + i];
								bmul += imgtmp[y + i][3 * x + 2]
										* kernelulong[kcenter + i];
								sum += kernelulong[kcenter + i];
							}
						}
						img[y][3 * x] = rmul / sum;
						img[y][3 * x + 1] = gmul / sum;
						img[y][3 * x + 2] = bmul / sum;
					}

				}
			} else {
				sum = 0;
				unsigned long rmul = 0, gmul = 0, bmul = 0;
				for (i = -kcenter; i <= kcenter; i++) {
					if ((y + i) >= 0 && (y + i) < height) {
						rmul += imgtmp[y + i][3 * x] * kernelulong[kcenter + i];
						gmul += imgtmp[y + i][3 * x + 1]
								* kernelulong[kcenter + i];
						bmul += imgtmp[y + i][3 * x + 2]
								* kernelulong[kcenter + i];
						sum += kernelulong[kcenter + i];
					}
				}
				img[y][3 * x] = rmul / sum;
				img[y][3 * x + 1] = gmul / sum;
				img[y][3 * x + 2] = bmul / sum;
			}

		}
	}

	freeGaussianKerneluLong(kernelulong);
}

//高斯分离模糊
void fuzzy2dir(unsigned char ** img, unsigned char ** imgtmp, int width,
		int height, int component, int x0, int y0, int r) {
	LOGI("circleFuzzy2dir ");
	//高斯矩阵
	unsigned long * kernelulong;
	double sigma = 10.0;
	int ksize;
	GaussianKerneluLong(kernelulong, 10, ksize);
	LOGI("获得高斯矩阵  ");

	int kcenter = ksize / 2;
	int i = 0, j = 0;
	unsigned long sum;
	//x方向一维高斯模糊
	for (int y = 0; y < height; y++) {
		for (int x = 0; x < width; x++) {
			sum = 0;
			unsigned long rmul = 0, gmul = 0, bmul = 0;
			for (i = -kcenter; i <= kcenter; i++) {
				if ((x + i) >= 0 && (x + i) < width) {
					rmul += img[y][3 * (x + i)] * kernelulong[kcenter + i];
					gmul += img[y][3 * (x + i) + 1] * kernelulong[kcenter + i];
					bmul += img[y][3 * (x + i) + 2] * kernelulong[kcenter + i];
					sum += kernelulong[kcenter + i];
				}
			}
			imgtmp[y][3 * x] = rmul / sum;
			imgtmp[y][3 * x + 1] = gmul / sum;
			imgtmp[y][3 * x + 2] = bmul / sum;
		}
	}
	//y方向
	for (int x = 0; x < width; x++) {
		for (int y = 0; y < height; y++) {
			sum = 0;
			unsigned long rmul = 0, gmul = 0, bmul = 0;
			for (i = -kcenter; i <= kcenter; i++) {
				if ((y + i) >= 0 && (y + i) < height) {
					rmul += imgtmp[y + i][3 * x] * kernelulong[kcenter + i];
					gmul += imgtmp[y + i][3 * x + 1] * kernelulong[kcenter + i];
					bmul += imgtmp[y + i][3 * x + 2] * kernelulong[kcenter + i];
					sum += kernelulong[kcenter + i];
				}
			}
			img[y][3 * x] = rmul / sum;
			img[y][3 * x + 1] = gmul / sum;
			img[y][3 * x + 2] = bmul / sum;

		}
	}

}

//圆型模糊图片
int circleFuzzy(unsigned char ** img, unsigned char ** imgtmp, int width,
		int height, int component, int x0, int y0, int r) {
	LOGI("circleFuzzy ");
	//高斯矩阵
	double ** kernel;
	int ** kernelint;
	unsigned long ** kernelulong;
	double sigma = 10.0;
	int ksize;
	unsigned long scale = 0xffffff;
	GaussianSmooth2D(sigma, kernel, ksize);
	//GaussianKernelInt(kernel, ksize, kernelint, scale);
	GaussianKernelulong(kernel, ksize, kernelulong, scale);
	LOGI("获得高斯矩阵  ");

	//3处理函数
	int powr2 = r * r;
	//待检区域
	int checkl, checkt, checkr, checkb;
	checkl = x0 - r > 0 ? x0 - r : 0;
	checkt = y0 - r > 0 ? y0 - r : 0;
	checkr = x0 + r >= width ? width : x0 + r;
	checkb = y0 + r >= height ? height : y0 + r;

	//免检区域
	int notcl, notct, notcr, notcb;
	//sqrt(2)/2
	float sqrt2half = 0.7071;
	int nr = sqrt2half * r;
	notcl = x0 - nr > 0 ? x0 - nr : 0;
	notct = y0 - nr > 0 ? y0 - nr : 0;
	notcr = x0 + nr >= width ? width - 1 : x0 + nr;
	notcb = y0 + nr >= height ? height - 1 : y0 + nr;

	//i控制行 不对边缘进行处理
	for (int i = ksize; i < height - ksize; i++) {
		//j控制列
		for (int j = ksize; j < width - ksize; j++) {
			//判断是否在待检查区域内
			if (i >= checkt && i <= checkb && j >= checkl && j <= checkr) {
				//判断是否在免检区域内
				if (i >= notct && i <= notcb && j >= notcl && j <= notcr) {

				} else {

					//判断是否在圆内
					if (pow2minus(i, y0) + pow2minus(j, x0) <= powr2) {

					} else { //不在圆内
						long middle[3] = { 0, 0, 0 };
						int mkl = j - ksize / 2;
						int mkt = i - ksize / 2;
						for (int k = 0; k < ksize; k++) {
							for (int l = 0; l < ksize; l++) {
								middle[0] += imgtmp[mkt + k][3 * (mkl + l)]
										* kernelulong[k][l];
								middle[1] += imgtmp[mkt + k][3 * (mkl + l) + 1]
										* kernelulong[k][l];
								middle[2] += imgtmp[mkt + k][3 * (mkl + l) + 2]
										* kernelulong[k][l];

							}
						}

						img[i][3 * j] = (unsigned char) (middle[0] / scale);
						img[i][3 * j + 1] = (unsigned char) (middle[1] / scale);
						img[i][3 * j + 2] = (unsigned char) (middle[2] / scale);
					}
				}
			} else { //绝对不在圆内,对其进行模糊处理
				long middle[3] = { 0, 0, 0 };
				int mkl = j - ksize / 2;
				int mkt = i - ksize / 2;
				for (int k = 0; k < ksize; k++) {
					for (int l = 0; l < ksize; l++) {
						middle[0] += imgtmp[mkt + k][3 * (mkl + l)]
								* kernelulong[k][l];
						middle[1] += imgtmp[mkt + k][3 * (mkl + l) + 1]
								* kernelulong[k][l];
						middle[2] += imgtmp[mkt + k][3 * (mkl + l) + 2]
								* kernelulong[k][l];

					}
				}

				img[i][3 * j] = (unsigned char) (middle[0] / scale);
				img[i][3 * j + 1] = (unsigned char) (middle[1] / scale);
				img[i][3 * j + 2] = (unsigned char) (middle[2] / scale);
			}
		}
	}
	LOGI("circleFuzzy end ");
	return 0;
}

//输入两个图片模板的模糊函数
//圆型模糊图片
int circleFuzzyDouble(unsigned char ** img, unsigned char ** imgtmp, int width,
		int height, int component, int x0, int y0, int r) {
	LOGI("circleFuzzy ");
	//高斯矩阵
	double ** kernel;
	double sigma = 10.0;
	int ksize;
	GaussianSmooth2D(sigma, kernel, ksize);
	LOGI("获得高斯矩阵  ");

	//3处理函数
	int powr2 = r * r;
	//待检区域
	int checkl, checkt, checkr, checkb;
	checkl = x0 - r > 0 ? x0 - r : 0;
	checkt = y0 - r > 0 ? y0 - r : 0;
	checkr = x0 + r >= width ? width : x0 + r;
	checkb = y0 + r >= height ? height : y0 + r;

	//免检区域
	int notcl, notct, notcr, notcb;
	//sqrt(2)/2
	float sqrt2half = 0.7071;
	int nr = sqrt2half * r;
	notcl = x0 - nr > 0 ? x0 - nr : 0;
	notct = y0 - nr > 0 ? y0 - nr : 0;
	notcr = x0 + nr >= width ? width - 1 : x0 + nr;
	notcb = y0 + nr >= height ? height - 1 : y0 + nr;

	//i控制行 不对边缘进行处理
	for (int i = ksize; i < height - ksize; i++) {
		//j控制列
		for (int j = ksize; j < width - ksize; j++) {
			//判断是否在待检查区域内
			if (i >= checkt && i <= checkb && j >= checkl && j <= checkr) {
				//判断是否在免检区域内
				if (i >= notct && i <= notcb && j >= notcl && j <= notcr) {

				} else {

					//判断是否在圆内
					if (pow2minus(i, y0) + pow2minus(j, x0) <= powr2) {

					} else { //不在圆内
						double middle[3] = { 0, 0, 0 };
						int mkl = j - ksize / 2;
						int mkt = i - ksize / 2;
						for (int k = 0; k < ksize; k++) {
							for (int l = 0; l < ksize; l++) {
								middle[0] += imgtmp[mkt + k][3 * (mkl + l)]
										* kernel[k][l];
								middle[1] += imgtmp[mkt + k][3 * (mkl + l) + 1]
										* kernel[k][l];
								middle[2] += imgtmp[mkt + k][3 * (mkl + l) + 2]
										* kernel[k][l];

							}
						}

						img[i][3 * j] = (unsigned char) middle[0];
						img[i][3 * j + 1] = (unsigned char) middle[1];
						img[i][3 * j + 2] = (unsigned char) middle[2];
					}
				}
			} else { //绝对不在圆内,对其进行模糊处理
				double middle[3] = { 0, 0, 0 };
				int mkl = j - ksize / 2;
				int mkt = i - ksize / 2;
				for (int k = 0; k < ksize; k++) {
					for (int l = 0; l < ksize; l++) {
						middle[0] += imgtmp[mkt + k][3 * (mkl + l)]
								* kernel[k][l];
						middle[1] += imgtmp[mkt + k][3 * (mkl + l) + 1]
								* kernel[k][l];
						middle[2] += imgtmp[mkt + k][3 * (mkl + l) + 2]
								* kernel[k][l];

					}
				}

				img[i][3 * j] = (unsigned char) middle[0];
				img[i][3 * j + 1] = (unsigned char) middle[1];
				img[i][3 * j + 2] = (unsigned char) middle[2];
			}
		}
	}
	LOGI("circleFuzzy end ");
	return 0;
}

int pow2minus(int x, int x0) {
	return x * x - 2 * x * x0 + x0 * x0;
}

//获得高斯long矩阵
void GaussianKernelulong(double ** kernel, int ksize,
		unsigned long ** & kernelulong, unsigned long scale) {
	kernelulong = (unsigned long **) malloc(sizeof(unsigned long *) * ksize);
	for (int i = 0; i < ksize; i++) {
		kernelulong[i] = (unsigned long *) malloc(
				sizeof(unsigned long) * ksize);
		memset(kernel[i], 0, ksize);
	}

	for (int i = 0; i < ksize; i++) {
		for (int j = 0; j < ksize; j++) {
			kernelulong[i][j] = kernel[i][j] * scale;
		}
	}

}

//获得一维unsigned long高斯 ,放大0xffffff
void GaussianKerneluLong(unsigned long * & kernel, int sigma, int& ksize) {
	sigma = sigma > 0 ? sigma : -sigma;
	//高斯核矩阵的大小为(6*sigma+1)*(6*sigma+1)
	//ksize为奇数
	unsigned long ratio = 0xffffff;
	ksize = ceil(sigma * 3) * 2 + 1;

	//浮点型的一维高斯核矩阵
	double * kdouble = (double *) malloc(sizeof(double) * ksize);

	//计算一维高斯核
	kernel = (unsigned long *) malloc(sizeof(unsigned long) * ksize);

	double scale = -0.5 / (sigma * sigma);
	const double PI = 3.141592653;
	double cons = 1 / sqrt(-scale / PI);

	double sum = 0;
	int kcenter = ksize / 2;
	int i = 0, j = 0;
	for (i = 0; i < ksize; i++) {
		int x = i - kcenter;
		*(kdouble + i) = cons * exp(x * x * scale); //一维高斯函数
		sum += *(kdouble + i);

	}
	//归一化,确保高斯权值在[0,1]之间
	for (i = 0; i < ksize; i++) {
		*(kdouble + i) /= sum;
	}

	//将double型矩阵转化为long型(放大ratio倍)
	for (i = 0; i < ksize; i++) {
		kernel[i] = kdouble[i] * ratio;
	}

	//释放double型核矩阵
	free(kdouble);
}

void freeGaussianKerneluLong(unsigned long * & kernel) {
	free(kernel);
}

//获得2维高斯int矩阵
void GaussianKernelInt(double ** kernel, int ksize, int ** & kernelint,
		unsigned int scale) {
	kernelint = (int **) malloc(sizeof(int *) * ksize);
	for (int i = 0; i < ksize; i++) {
		kernelint[i] = (int *) malloc(sizeof(int) * ksize);
		memset(kernel[i], 0, ksize);
	}

	for (int i = 0; i < ksize; i++) {
		for (int j = 0; j < ksize; j++) {
			kernelint[i][j] = kernel[i][j] * scale;
		}
	}

}

//获得高斯核矩阵 //高斯矩阵待free
void GaussianSmooth2D(double sigma, double ** & kernel, int& ksize) {

	//确保sigma为正数
	sigma = sigma > 0 ? sigma : 0;
	//高斯核矩阵的大小为(6*sigma+1)*(6*sigma+1)
	ksize = ceil(sigma * 3) * 2 + 1;

	//计算高斯核矩阵
	//double *kernel = new double[ksize*ksize];
	kernel = (double **) malloc(sizeof(double*) * ksize);
	for (int i = 0; i < ksize; i++) {
		kernel[i] = (double *) malloc(sizeof(double) * ksize);
		memset(kernel[i], 0, ksize);
	}

	double scale = -0.5 / (sigma * sigma); //-1/2sigma的平方
	const double PI = 3.141592653;
	double cons = -scale / PI;

	double sum = 0.0;

	for (int i = 0; i < ksize; i++) {
		for (int j = 0; j < ksize; j++) {
			int x = i - (ksize - 1) / 2;
			int y = j - (ksize - 1) / 2;
			kernel[i][j] = cons * exp(scale * (x * x + y * y));

			sum += kernel[i][j];
		}
	}
	//归一化
	for (int i = 0; i < ksize; i++) {
		for (int j = 0; j < ksize; j++) {
			kernel[i][j] /= sum;
		}
	}

}

//解析jpeg图片数据到img,图片信息保存在cinfo
int read_jpeg_file(const char *input_filename,
		struct jpeg_decompress_struct& cinfo, unsigned char ** & img) {
	LOGI("read_jpeg_file 开始");
	struct jpeg_error_mgr jerr;
	FILE *input_file;
	JSAMPARRAY buffer;
	int row_width;

	unsigned char *output_buffer;
	unsigned char *tmp = NULL;

	cinfo.err = jpeg_std_error(&jerr);

	if ((input_file = fopen(input_filename, "rb")) == NULL) {
		fprintf(stderr, "can't open %s\n", input_filename);
		LOGI("can't open jpg1");
		return -1;
	}

	//初始化信息
	jpeg_create_decompress(&cinfo);
	LOGI("初始化信息");

	/* Specify data source for decompression */
	//指定图片
	jpeg_stdio_src(&cinfo, input_file);
	LOGI("指定图片");

	/* Read file header, set default decompression parameters */
	(void) jpeg_read_header(&cinfo, TRUE);
	LOGI("读取头信息, set default decompression parameters ");

	/* Start decompressor */
	(void) jpeg_start_decompress(&cinfo);
	LOGI("解压");

	row_width = cinfo.output_width * cinfo.output_components;
	LOGI(
			"图片的宽:%d 图片的高%d 颜色长度:%d", cinfo.output_width, cinfo.output_height, cinfo.output_components);

	buffer = (*cinfo.mem->alloc_sarray)((j_common_ptr) &cinfo, JPOOL_IMAGE,
			row_width, 1);

	img = (unsigned char **) malloc(
			sizeof(unsigned char **) * cinfo.output_height);
	for (int i = 0; i < cinfo.output_height; i++) {
		img[i] = (unsigned char *) malloc(row_width);
		memset(img[i], 0, row_width);
	}

//        output_buffer = (unsigned char *)malloc(row_width * cinfo.output_height);
//        memset(output_buffer, 0, row_width * cinfo.output_height);
//        tmp = output_buffer;

	/* Process data */
//        while (cinfo.output_scanline < cinfo.output_height) {
//                (void) jpeg_read_scanlines(&cinfo, buffer, 1);
//
//                memcpy(tmp, *buffer, row_width);
//                tmp += row_width;
//        }
	for (int i = 0; cinfo.output_scanline < cinfo.output_height; i++) {
		(void) jpeg_read_scanlines(&cinfo, buffer, 1);
		memcpy(img[i], *buffer, row_width);
	}
	LOGI("copy stream end ");

	free(output_buffer);
	LOGI("free output_buffer ");

	(void) jpeg_finish_decompress(&cinfo);

	jpeg_destroy_decompress(&cinfo);

	/* Close files, if we opened them */
	fclose(input_file);

	return 0;
}

int mkjpeg(int width, int height, unsigned char **buffer,
		const char *filename) {
	LOGI("mkjpeg 开始 ");
	struct jpeg_compress_struct jcs;
	struct jpeg_error_mgr jem;
	FILE *fp;
	JSAMPROW row_pointer[1]; // 一行位图

	jcs.err = jpeg_std_error(&jem);
	jpeg_create_compress(&jcs);
	LOGI("初始化: jpeg_create_compress ");

	fp = fopen(filename, "wb");
	if (fp == NULL) {
		LOGI("写文件错误");
		return -1;
	}
	LOGI("打开文件");

	jpeg_stdio_dest(&jcs, fp);

	jcs.image_width = width; // 位图的宽和高，单位为像素
	jcs.image_height = height;
	jcs.input_components = 3; // 在此为1,表示灰度图， 如果是彩色位图，则为3
	jcs.in_color_space = JCS_RGB; //JCS_GRAYSCALE表示灰度图，JCS_RGB表示彩色图像

	jpeg_set_defaults(&jcs);
	jpeg_set_quality(&jcs, 80, TRUE);
	LOGI("设置: end ");

	jpeg_start_compress(&jcs, TRUE);
	LOGI("开始压缩 ");

	int i = 0;
	while (jcs.next_scanline < jcs.image_height) {
		row_pointer[0] = buffer[i];
		jpeg_write_scanlines(&jcs, row_pointer, 1);
		i++;
	}

	jpeg_finish_compress(&jcs);
	LOGI("结束压缩 ");

	jpeg_destroy_compress(&jcs);
	fclose(fp);

	return 0;
}

int freeImg(unsigned char ** img, int height) {
	for (int i = 0; i < height; i++) {
		free(img[i]);
	}
	free(img);
	return 0;
}

int read_jpeg_file(unsigned char ** img, int width, int height,
		ANativeWindow_Buffer& nwBuffer) {

	//实际显示的区域大小,初始化为nativeWindow的大小
	int showWidth = nwBuffer.width, showHeight = nwBuffer.height;
	//实际显示区域的左端点
	int leftX = 0, leftY = 0;
	//右下端点
	int rightX, bottomY;

	//实际缩放比例
	float scale;
	// 原图:NativeWindow区域
	float scaleX = width / nwBuffer.width, scaleY = height / nwBuffer.height;
	if (scaleX > scaleY) {
		scale = scaleX;
		showHeight *= (scaleY / scaleX);
		leftY = (nwBuffer.height - showHeight) / 2;
	} else {
		scale = scaleY;
		showWidth *= (scaleX / scaleY);
		leftX = (nwBuffer.width - showWidth) / 2;
	}
	rightX = leftX + showWidth;
	bottomY = leftY + showHeight;

	/* Process data */
	int get8h5 = 248, get8h6 = 252;
	__uint16_t * line = (__uint16_t *) nwBuffer.bits;

	LOGI(
			" NativeWindow  width: %d height: %d", nwBuffer.width, nwBuffer.height);
	LOGI(" scale of img = %f", scale);
	LOGI(" show width : %d , show height : %d ", showWidth, showHeight);
	LOGI(
			"leftX:%d,rightX:%d,leftY:%d,bottomY:%d", leftX, rightX, leftY, bottomY);

	for(int i=0;i<50;i++){
		line[i] = 57083;
	}

	//选择显示的高区域
	for (int i = leftY; i > 0; i--) {
		line += nwBuffer.stride;
	}
	LOGI(" 选择显示的高区域");

	//插值法
	int *px, *py;
	px = (int *) malloc(sizeof(int) * showWidth);
	py = (int *) malloc(sizeof(int) * showHeight);
	for (int i = 0; i < showWidth; i++) {
		px[i] = i * scale + 0.5;
	}
	for (int i = 0; i < showHeight; i++) {
		py[i] = i * scale + 0.5;
	}
	LOGI(" 插值法 结束");

	for (int i = leftY, y = 0, choosehNum = 0; i < bottomY; i++, y++) {
		//获得一行
		//根据缩放选取行

		//LOGI("nwBuffer->format == WINDOW_FORMAT_RGB_565");
		for (int j = leftX, x = 0; j < rightX; j++, x++) {
			if (nwBuffer.format == WINDOW_FORMAT_RGB_565) {
//				line[j] = ((__uint16_t ) img[py[y]][3 * px[x]] & get8h5) << 8
//						| ((__uint16_t ) (img[py[y]][3 * px[x] + 1] & get8h6)
//								<< 3)
//						| ((__uint16_t ) (img[py[y]][3 * px[x] + 2] & get8h6)
//								>> 3);
				line[j]=57083;
				if (x >= showWidth-1 && y >= showHeight - 1) {
					LOGI("i:%d  j :%d, py[y]:%d, px[x]:%d", i, j, py[y], px[x]);
				}
			}

		}
		line = line + nwBuffer.stride;
	}

	return 0;
}
