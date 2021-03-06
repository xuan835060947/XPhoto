/* DO NOT EDIT THIS FILE - it is machine generated */
#include <jni.h>
/* Header for class com_example_photoprocessing_activity_PhotoProcessingActivity */
#include <android/bitmap.h>
#include <android/log.h>
#include <math.h>
#include <string.h>


#ifndef _Included_com_example_photoprocessing_activity_PhotoProcessingActivity
#define _Included_com_example_photoprocessing_activity_PhotoProcessingActivity
#ifdef __cplusplus
extern "C" {
#endif
#undef com_example_photoprocessing_activity_PhotoProcessingActivity_MODE_PRIVATE
#define com_example_photoprocessing_activity_PhotoProcessingActivity_MODE_PRIVATE 0L
#undef com_example_photoprocessing_activity_PhotoProcessingActivity_MODE_WORLD_READABLE
#define com_example_photoprocessing_activity_PhotoProcessingActivity_MODE_WORLD_READABLE 1L
#undef com_example_photoprocessing_activity_PhotoProcessingActivity_MODE_WORLD_WRITEABLE
#define com_example_photoprocessing_activity_PhotoProcessingActivity_MODE_WORLD_WRITEABLE 2L
#undef com_example_photoprocessing_activity_PhotoProcessingActivity_MODE_APPEND
#define com_example_photoprocessing_activity_PhotoProcessingActivity_MODE_APPEND 32768L
#undef com_example_photoprocessing_activity_PhotoProcessingActivity_MODE_MULTI_PROCESS
#define com_example_photoprocessing_activity_PhotoProcessingActivity_MODE_MULTI_PROCESS 4L
#undef com_example_photoprocessing_activity_PhotoProcessingActivity_MODE_ENABLE_WRITE_AHEAD_LOGGING
#define com_example_photoprocessing_activity_PhotoProcessingActivity_MODE_ENABLE_WRITE_AHEAD_LOGGING 8L
#undef com_example_photoprocessing_activity_PhotoProcessingActivity_BIND_AUTO_CREATE
#define com_example_photoprocessing_activity_PhotoProcessingActivity_BIND_AUTO_CREATE 1L
#undef com_example_photoprocessing_activity_PhotoProcessingActivity_BIND_DEBUG_UNBIND
#define com_example_photoprocessing_activity_PhotoProcessingActivity_BIND_DEBUG_UNBIND 2L
#undef com_example_photoprocessing_activity_PhotoProcessingActivity_BIND_NOT_FOREGROUND
#define com_example_photoprocessing_activity_PhotoProcessingActivity_BIND_NOT_FOREGROUND 4L
#undef com_example_photoprocessing_activity_PhotoProcessingActivity_BIND_ABOVE_CLIENT
#define com_example_photoprocessing_activity_PhotoProcessingActivity_BIND_ABOVE_CLIENT 8L
#undef com_example_photoprocessing_activity_PhotoProcessingActivity_BIND_ALLOW_OOM_MANAGEMENT
#define com_example_photoprocessing_activity_PhotoProcessingActivity_BIND_ALLOW_OOM_MANAGEMENT 16L
#undef com_example_photoprocessing_activity_PhotoProcessingActivity_BIND_WAIVE_PRIORITY
#define com_example_photoprocessing_activity_PhotoProcessingActivity_BIND_WAIVE_PRIORITY 32L
#undef com_example_photoprocessing_activity_PhotoProcessingActivity_BIND_IMPORTANT
#define com_example_photoprocessing_activity_PhotoProcessingActivity_BIND_IMPORTANT 64L
#undef com_example_photoprocessing_activity_PhotoProcessingActivity_BIND_ADJUST_WITH_ACTIVITY
#define com_example_photoprocessing_activity_PhotoProcessingActivity_BIND_ADJUST_WITH_ACTIVITY 128L
#undef com_example_photoprocessing_activity_PhotoProcessingActivity_CONTEXT_INCLUDE_CODE
#define com_example_photoprocessing_activity_PhotoProcessingActivity_CONTEXT_INCLUDE_CODE 1L
#undef com_example_photoprocessing_activity_PhotoProcessingActivity_CONTEXT_IGNORE_SECURITY
#define com_example_photoprocessing_activity_PhotoProcessingActivity_CONTEXT_IGNORE_SECURITY 2L
#undef com_example_photoprocessing_activity_PhotoProcessingActivity_CONTEXT_RESTRICTED
#define com_example_photoprocessing_activity_PhotoProcessingActivity_CONTEXT_RESTRICTED 4L
#undef com_example_photoprocessing_activity_PhotoProcessingActivity_RESULT_CANCELED
#define com_example_photoprocessing_activity_PhotoProcessingActivity_RESULT_CANCELED 0L
#undef com_example_photoprocessing_activity_PhotoProcessingActivity_RESULT_OK
#define com_example_photoprocessing_activity_PhotoProcessingActivity_RESULT_OK -1L
#undef com_example_photoprocessing_activity_PhotoProcessingActivity_RESULT_FIRST_USER
#define com_example_photoprocessing_activity_PhotoProcessingActivity_RESULT_FIRST_USER 1L
#undef com_example_photoprocessing_activity_PhotoProcessingActivity_DEFAULT_KEYS_DISABLE
#define com_example_photoprocessing_activity_PhotoProcessingActivity_DEFAULT_KEYS_DISABLE 0L
#undef com_example_photoprocessing_activity_PhotoProcessingActivity_DEFAULT_KEYS_DIALER
#define com_example_photoprocessing_activity_PhotoProcessingActivity_DEFAULT_KEYS_DIALER 1L
#undef com_example_photoprocessing_activity_PhotoProcessingActivity_DEFAULT_KEYS_SHORTCUT
#define com_example_photoprocessing_activity_PhotoProcessingActivity_DEFAULT_KEYS_SHORTCUT 2L
#undef com_example_photoprocessing_activity_PhotoProcessingActivity_DEFAULT_KEYS_SEARCH_LOCAL
#define com_example_photoprocessing_activity_PhotoProcessingActivity_DEFAULT_KEYS_SEARCH_LOCAL 3L
#undef com_example_photoprocessing_activity_PhotoProcessingActivity_DEFAULT_KEYS_SEARCH_GLOBAL
#define com_example_photoprocessing_activity_PhotoProcessingActivity_DEFAULT_KEYS_SEARCH_GLOBAL 4L
/*
 * Class:     com_example_photoprocessing_activity_PhotoProcessingActivity
 * Method:    grayPhoto
 * Signature: (Landroid/graphics/Bitmap;Landroid/graphics/Bitmap;)V
 */
/**
 * xuan
 */
#define  LOG_TAG    "PhotoProcessing"
#define  LOGI(...)  __android_log_print(ANDROID_LOG_INFO,LOG_TAG,__VA_ARGS__)
#define  LOGE(...)  __android_log_print(ANDROID_LOG_ERROR,LOG_TAG,__VA_ARGS__)

typedef struct {
	uint8_t alpha;
	uint8_t red;
	uint8_t green;
	uint8_t blue;
} argb;


JNIEXPORT void JNICALL Java_com_example_photoprocessing_activity_PhotoProcessingActivity_grayPhoto
  (JNIEnv *, jobject, jobject, jobject);

#ifdef __cplusplus
}
#endif
#endif
