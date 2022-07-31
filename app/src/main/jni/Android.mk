LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)
LOCAL_MODULE := libdobby
LOCAL_SRC_FILES := dobby/libraries/$(TARGET_ARCH_ABI)/libdobby.a
LOCAL_EXPORT_C_INCLUDES := $(LOCAL_PATH)/dobby/
include $(PREBUILT_STATIC_LIBRARY)

include $(CLEAR_VARS)
LOCAL_MODULE := lie
LOCAL_C_INCLUDES += $(LOCAL_PATH)
LOCAL_C_INCLUDES += $(LOCAL_PATH)/imgui
LOCAL_CPPFLAGS := -Wno-error=format-security -fvisibility=hidden -fexceptions -std=c++17
LOCAL_LDLIBS := -llog -landroid -lEGL -lGLESv3
LOCAL_ARM_MODE := arm
LOCAL_STATIC_LIBRARIES := libdobby
LOCAL_SRC_FILES := imgui/imgui.cpp \
imgui/imgui_demo.cpp \
imgui/imgui_draw.cpp \
imgui/imgui_tables.cpp \
imgui/imgui_widgets.cpp \
imgui/backends/imgui_impl_android.cpp \
imgui/backends/imgui_impl_opengl3.cpp \
struct/MonoString.cpp \
unity/fake_dlfcn.cpp \
unity/Il2Cpp.cpp \
unity/Tools.cpp \
Main.cpp
                     
include $(BUILD_SHARED_LIBRARY)
