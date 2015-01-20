LOCAL_PATH := $(call my-dir)

#include $(CLEAR_VARS)
#
#LOCAL_MODULE    := PhotoProcessing
#LOCAL_SRC_FILES := PhotoProcessing.cpp
##系统库
#LOCAL_LDLIBS    := -llog -ljnigraphics
#
#include $(BUILD_SHARED_LIBRARY)

  
## prebuilt the static libs  
include $(CLEAR_VARS)  

$(info $(LOCAL_PATH))
LOCAL_MODULE := jpeg  
LOCAL_SRC_FILES := ../libs/$(TARGET_ARCH_ABI)/libjpeg.a  
  
include $(PREBUILT_STATIC_LIBRARY)

#include $(CLEAR_VARS)
#
#LOCAL_MODULE    := SurfaceProcessing
#LOCAL_SRC_FILES := SurfaceProcessing.cpp
#LOCAL_LDLIBS    := -llog -landroid
#LOCAL_STATIC_LIBRARIES += jpeg  
#
#include $(BUILD_SHARED_LIBRARY)

include $(CLEAR_VARS)

LOCAL_MODULE    := CircleBluz
LOCAL_SRC_FILES := CircleBluz.cpp
LOCAL_LDLIBS    := -llog -landroid
LOCAL_STATIC_LIBRARIES += jpeg  

#启用分析工具
MY_ANDROID_NDK_PROFILER_ENABLED := true

#如果已经被启用
#ifeq ($(MY_ANDROID_NDK_PROFILER_ENABLED),true)

#显示信息
#$(info GNU Profiler is enabled)


#使用Enable the monitor function静态库
#LOCAL_CFLAGS := -pg
#LOCAL_STATIC_LIBRARIES += android-ndk-profiler
#endif

include $(BUILD_SHARED_LIBRARY)
   
#$(info import-module is enabled)
#$(call import-module,android-ndk-profiler/jni)
