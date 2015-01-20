#include <CircleBluzActivity.h>

extern "C" {
#include <jpeg/jpeglib.h>
}
#include <setjmp.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>

#include <prof.h>

//声明
int read_jpeg_file(const char *input_filename,
		struct jpeg_decompress_struct& cinfo, unsigned char ** & img);
int showInWindow(unsigned char ** img, int width, int height,
		ANativeWindow_Buffer& nwBuffer, int saveTmp);
void freeImg(unsigned char ** img, int height);
void GaussianKerneluLong(unsigned long * & kernel, float sigma, int& ksize);
int pow2minus(int x, int x0);
void freeGaussianKerneluLong(unsigned long * & kernel);
void circleFuzzy2dir(unsigned char ** img, unsigned char ** imgtmp, int width,
		int height, int component, int x0, int y0, int r, float sigma);
void copyTo(struct Img in, struct Img &out);
void changToFact(struct Img originalImg, struct Img window, int &x, int &y,
		int &radius);
void changToShowImg(struct Img img, ANativeWindow_Buffer nwBuffer, int &x,
		int &y, int &radius);
int mkjpeg(unsigned char **buffer, int width, int height, const char *filename);
;

struct Img {
	unsigned char ** img;
	int width;
	int height;
	int component;
};

//全局变量
struct Img originalImg, showImg, window;

JNIEXPORT void JNICALL Java_com_example_photoprocessing_activity_CircleBluzActivity_showImg(
		JNIEnv * env, jobject activity, jobject surface, jstring imgPath) {
//	monstartup("libCircleBluz.so");
//	LOGI("monstartup   ");

	ANativeWindow_Buffer nwBuffer;
	const char * imgChar;
	struct jpeg_decompress_struct cinfo;

	imgChar = env->GetStringUTFChars(imgPath, 0);

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

	window.width = nwBuffer.width;
	window.height = nwBuffer.height;

	int result = read_jpeg_file(imgChar, cinfo, originalImg.img);
	if (result < 0) {
		LOGE("read jpeg bit error");
		return;
	}
	originalImg.width = cinfo.output_width;
	originalImg.height = cinfo.output_height;
	originalImg.component = 3;

	showInWindow(originalImg.img, originalImg.width, originalImg.height,
			nwBuffer, 1);

	if (nwBuffer.format == WINDOW_FORMAT_RGBA_8888) {
		LOGI("nwBuffer->format == WINDOW_FORMAT_RGBA_8888 ");
	}

	LOGI("ANativeWindow_unlockAndPost ");
	if (0 != ANativeWindow_unlockAndPost(mANativeWindow)) {
		LOGE("ANativeWindow_unlockAndPost error");
		return;
	}

	env->ReleaseStringUTFChars(imgPath, imgChar);
	ANativeWindow_release(mANativeWindow);
	LOGI("ANativeWindow_release ");
}

JNIEXPORT void JNICALL Java_com_example_photoprocessing_activity_CircleBluzActivity_circleBluz(
		JNIEnv *env, jobject activity, jstring savePath, jint x, jint y, jint r,
		jfloat sigma) {
	clock_t start, finish;
	double duration;
	start = clock();
	const char * saveChar;
	saveChar = env->GetStringUTFChars(savePath, 0);

	//生成原图的复制
	LOGI("width :%d, height:%d", originalImg.width, originalImg.height);
	struct Img out, imgTmp;
	copyTo(originalImg, out);
	copyTo(originalImg, imgTmp);
	LOGI("copy stream end ");

	//求圆心 半径的在真实图片中的大小
	changToFact(originalImg, window, x, y, r);

	//将原图复制输出版模糊
	circleFuzzy2dir(out.img, imgTmp.img, out.width, out.height, out.component,
			x, y, r, sigma);

	//生成图片
	(void) mkjpeg(out.img, out.width, out.height, saveChar);

	finish = clock();
	duration = (double) (finish - start) / CLOCKS_PER_SEC;
	LOGI("花费时间 :    %f ", duration);
}

//显示
JNIEXPORT void JNICALL Java_com_example_photoprocessing_activity_CircleBluzActivity_circleBluzShow(
		JNIEnv * env, jobject activity, jobject surface, jint x, jint y, jint r,
		jfloat sigma) {
	ANativeWindow_Buffer nwBuffer;
	clock_t start, finish;
	double duration;
	start = clock();

	LOGI("start sigma : %f ", sigma);

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

	Img tmp1, tmp2;
	copyTo(showImg, tmp1);
	copyTo(showImg, tmp2);
	LOGI("copy stream end!! ");

	changToShowImg(tmp1, nwBuffer, x, y, r);
	LOGI("changToShowImg end!! ");

	//降低缩放效果
	sigma *= ((float) showImg.width / originalImg.width);
	LOGI("end sigma : %f ", sigma);

	circleFuzzy2dir(tmp1.img, tmp2.img, tmp1.width, tmp1.height, tmp1.component,
			x, y, r, sigma);
	LOGI(" circleFuzzy2dir end!! ");

	showInWindow(tmp1.img, tmp1.width, tmp1.height, nwBuffer, 0);

	if (nwBuffer.format == WINDOW_FORMAT_RGBA_8888) {
		LOGI("nwBuffer->format == WINDOW_FORMAT_RGBA_8888 ");
	}

	LOGI("ANativeWindow_unlockAndPost ");
	if (0 != ANativeWindow_unlockAndPost(mANativeWindow)) {
		LOGE("ANativeWindow_unlockAndPost error");
		return;
	}

	freeImg(tmp1.img, tmp1.height);
	freeImg(tmp2.img, tmp2.height);

	ANativeWindow_release(mANativeWindow);
	LOGI("ANativeWindow_release ");

	finish = clock();
	duration = (double) (finish - start) / CLOCKS_PER_SEC;
	LOGI("花费时间 :    %f ", duration);
}

//直接显示,不缩放
JNIEXPORT void JNICALL Java_com_example_photoprocessing_activity_CircleBluzActivity_showCurImg(
		JNIEnv * env, jobject activity, jobject surface) {
	ANativeWindow_Buffer nwBuffer;
	clock_t start, finish;
	double duration;
	start = clock();

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

	showInWindow(showImg.img, showImg.width, showImg.height, nwBuffer, 0);

	LOGI("ANativeWindow_unlockAndPost ");
	if (0 != ANativeWindow_unlockAndPost(mANativeWindow)) {
		LOGE("ANativeWindow_unlockAndPost error");
		return;
	}

	ANativeWindow_release(mANativeWindow);
	LOGI("ANativeWindow_release ");

	finish = clock();
	duration = (double) (finish - start) / CLOCKS_PER_SEC;
	LOGI("花费时间 :    %f ", duration);

}

//读取jpeg图片并缩放到nwBuffer中(效果:读取图片并居中显示)
//并获得imgTmpGlobal
//saveTmp == 1表示更新缩约图showImg
int showInWindow(unsigned char ** img, int width, int height,
		ANativeWindow_Buffer& nwBuffer, int saveTmp) {

	//实际显示的区域大小,初始化为nativeWindow的大小
	int showWidth = nwBuffer.width, showHeight = nwBuffer.height;
	//实际显示区域的左端点
	int leftX = 0, leftY = 0;
	//右下端点
	int rightX, bottomY;

	//实际缩放比例
	float scale;
	// 原图:NativeWindow区域
	float scaleX = (float) width / nwBuffer.width, scaleY = (float) height
			/ nwBuffer.height;
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
	LOGI(" scale of img = %.2f", scale);
	LOGI(" show width : %d , show height : %d ", showWidth, showHeight);
	LOGI(
			"leftX:%d,rightX:%d,leftY:%d,bottomY:%d", leftX, rightX, leftY, bottomY);

	//选择显示的高区域
	for (int i = leftY; i > 0; i--) {
		line += nwBuffer.stride;
	}
	LOGI(" 选择显示的高区域  ");

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
		//LOGI("nwBuffer->format == WINDOW_FORMAT_RGB_565");
		for (int j = leftX, x = 0; j < rightX; j++, x++) {
			if (nwBuffer.format == WINDOW_FORMAT_RGB_565) {
				line[j] = ((__uint16_t ) img[py[y]][3 * px[x]] & get8h5) << 8
						| ((__uint16_t ) (img[py[y]][3 * px[x] + 1] & get8h6)
								<< 3)
						| ((__uint16_t ) (img[py[y]][3 * px[x] + 2] & get8h6)
								>> 3);
			}

		}
		line += nwBuffer.stride;
	}

	if (saveTmp == 1) {
		//填充展示缓存
		showImg.img = (unsigned char **) malloc(
				sizeof(unsigned char *) * showHeight);
		for (int i = 0; i < showHeight; i++) {
			showImg.img[i] = (unsigned char *) malloc(
					sizeof(unsigned char) * showWidth * 3);
		}
		for (int i = 0; i < showHeight; i++) {
			for (int j = 0; j < showWidth; j++) {
				showImg.img[i][3 * j] = img[py[i]][3 * px[j]];
				showImg.img[i][3 * j + 1] = img[py[i]][3 * px[j] + 1];
				showImg.img[i][3 * j + 2] = img[py[i]][3 * px[j] + 2];
			}
		}
		showImg.width = showWidth;
		showImg.height = showHeight;
		showImg.component = 3;
	}

	free(px);
	free(py);

	return 0;
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

//根据圆心 半径在TouchCircle中的值确定其在显示图片中的值
void changToShowImg(struct Img img, ANativeWindow_Buffer nwBuffer, int &x,
		int &y, int &radius) {
	//图片显示的左端点
	int leftX = 0, leftY = 0;

	int showWidth = nwBuffer.width, showHeight = nwBuffer.height;
	//实际缩放比例
	float scale;
	// 原图:NativeWindow区域
	float scaleX = (float) img.width / nwBuffer.width, scaleY =
			(float) img.height / nwBuffer.height;
	if (scaleX > scaleY) {
		scale = scaleX;
		showHeight *= (scaleY / scaleX);
		leftY = (nwBuffer.height - showHeight) / 2;
	} else {
		scale = scaleY;
		showWidth *= (scaleX / scaleY);
		leftX = (nwBuffer.width - showWidth) / 2;
	}

	x -= leftX;
	y -= leftY;
//	x *= scale;
//	y *= scale;
//	radius *= scale;

}

//根据圆心 半径在Native Window中的值确定其在原高清图片中的值
void changToFact(struct Img originalImg, struct Img window, int &x, int &y,
		int &radius) {
	//图片显示的左端点
	int leftX = 0, leftY = 0;
	LOGI("x:%d, y:%d, radius:%d", x, y, radius);

	int showWidth = window.width, showHeight = window.height;
	//实际缩放比例
	float scale;
	// 原图:NativeWindow区域
	float scaleX = (float) originalImg.width / window.width, scaleY =
			(float) originalImg.height / window.height;
	if (scaleX > scaleY) {
		scale = scaleX;
		showHeight *= (scaleY / scaleX);
		leftY = (window.height - showHeight) / 2 + 0.5;
	} else {
		scale = scaleY;
		showWidth *= (scaleX / scaleY);
		leftX = (window.width - showWidth) / 2 + 0.5;
	}

	x -= leftX;
	y -= leftY;
	x *= scale;
	y *= scale;
	radius *= scale;

	LOGI("x:%d, y:%d, radius:%d scale : %f", x, y, radius, scale);

}

//高斯分离模糊
void circleFuzzy2dir(unsigned char ** img, unsigned char ** imgtmp, int width,
		int height, int component, int x0, int y0, int r, float sigma) {
	LOGI("circleFuzzy ");
	//高斯矩阵
	unsigned long * kernelulong;
	int ksize;

	if (sigma <= 0) {
		return;
	}

	GaussianKerneluLong(kernelulong, sigma, ksize);
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
	LOGI(" freeGaussianKerneluLong ");
}

//copy unsigned char ** img to out
void copyTo(struct Img in, struct Img &out) {
	out.img = (unsigned char **) malloc(sizeof(unsigned char *) * in.height);
	for (int i = 0; i < in.height; i++) {
		out.img[i] = (unsigned char *) malloc(
				sizeof(unsigned char) * in.width * in.component);
	}
	for (int i = 0; i < in.height; i++) {
		memcpy(out.img[i], in.img[i], in.width * in.component);
	}
	out.component = in.component;
	out.width = in.width;
	out.height = in.height;
//	LOGI("%c")

}

JNIEXPORT void JNICALL Java_com_example_photoprocessing_activity_CircleBluzActivity_destroyNativeAll(
		JNIEnv *, jobject) {
	if (originalImg.img != NULL) {
		freeImg(originalImg.img, originalImg.height);
	}
	originalImg.img = NULL;

//	moncleanup();
}

//生成图片
int mkjpeg(unsigned char **buffer, int width, int height,
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
	jpeg_set_quality(&jcs, 60, TRUE);
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

void freeImg(unsigned char ** img, int height) {
	for (int i = 0; i < height; i++) {
		free(img[i]);
	}
	free(img);
}

void freeGaussianKerneluLong(unsigned long * & kernel) {
	free(kernel);
}

//获得一维unsigned long高斯 ,放大0xffffff
void GaussianKerneluLong(unsigned long * & kernel, float sigma, int& ksize) {
	sigma = sigma > 0 ? sigma : -sigma;
	//高斯核矩阵的大小为(6*sigma+1)*(6*sigma+1)
	//ksize为奇数
	unsigned long ratio = 0xffffff;
	ksize = ceil(sigma * 3) * 2 + 1;
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

		//		cout << " " << *(kernel+i);
	}
	//	cout << endl;
	//归一化,确保高斯权值在[0,1]之间
	for (i = 0; i < ksize; i++) {
		*(kdouble + i) /= sum;
		//		cout << " " << *(kernel+i);
	}

	for (i = 0; i < ksize; i++) {
		kernel[i] = kdouble[i] * ratio;
	}

	free(kdouble);
}

int pow2minus(int x, int x0) {
	return x * x - 2 * x * x0 + x0 * x0;
}
