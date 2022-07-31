#include <iostream>
#include <stdio.h>
#include <string>
#include <unistd.h>
#include <stdint.h>
#include <inttypes.h>
#include <vector>
#include <map>
#include <chrono>
#include <fstream>
#include <thread>
#include <pthread.h>
#include <dirent.h>
#include <libgen.h>
#include <sys/stat.h>
#include <sys/mman.h>
#include <sys/uio.h>
#include <fcntl.h>
#include <jni.h>
#include <android/log.h>
#include <elf.h>
#include <dlfcn.h>
#include <sys/system_properties.h>
#include <EGL/egl.h>
#include <GLES3/gl3.h>

#include "imgui.h"
#include "backends/imgui_impl_android.h"
#include "backends/imgui_impl_opengl3.h"
#include "android_native_app_glue.h"
#include "fonts/GoogleSans.h"

#include "struct/Vector3.hpp"
#include "struct/Vector2.h"
#include "struct/Color.h"
#include "struct/Rect.h"
#include "struct/Quaternion.h"
#include "struct/MonoString.h"

#include "unity/fake_dlfcn.h"
#include "unity/Il2Cpp.h"
#include "unity/Tools.h"

#include "Setup.h"

EGLBoolean (*orig_eglSwapBuffers)(...);
EGLBoolean _eglSwapBuffers (EGLDisplay dpy, EGLSurface surface) {
	eglQuerySurface(dpy, surface, EGL_WIDTH, &glWidth);
    eglQuerySurface(dpy, surface, EGL_HEIGHT, &glHeight);
    if (!g_Initialized) {
        ImGui::CreateContext();
        ImGuiStyle* style = &ImGui::GetStyle();
        ImGui::StyleColorsDark();
		
        style->WindowPadding = ImVec2(15, 15);
        style->WindowRounding = 5.0f;
        style->WindowBorderSize = 0.0f;
        style->FramePadding = ImVec2(5, 5);
        style->FrameRounding = 4.0f;
        style->ItemInnerSpacing = ImVec2(8, 6);
        style->IndentSpacing = 25.0f;
        style->ScrollbarSize = 15.0f;
        style->ScrollbarRounding = 9.0f;
        style->GrabMinSize = 5.0f;
        style->GrabRounding = 3.0f;
        style->ScaleAllSizes(3.0f);
        style->ScrollbarSize /= 1;

        ImGuiIO* io = &ImGui::GetIO();
        io->IniFilename = nullptr;

        ImGui_ImplAndroid_Init();
        ImGui_ImplOpenGL3_Init("#version 300 es");

		ImFontConfig font_cfg;
		io->Fonts->AddFontFromMemoryCompressedTTF(GoogleSans_compressed_data, GoogleSans_compressed_size, 39.0f/*(float) get_dpi() / 14.0f*/, &font_cfg);
		
        g_Initialized = true;
    }

    ImGuiIO* io = &ImGui::GetIO();
	
	/*get touch from unity game*/
	ImGui_GetTouch(io, get_height());
	
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplAndroid_NewFrame(glWidth, glHeight);
    ImGui::NewFrame();
	
	/*load window here*/
	ImGui::ShowDemoWindow();
	
    ImGui::Render();
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
    if (clearMousePos) {
        io->MousePos = ImVec2(-1, -1);
        clearMousePos = false;
    }
    return orig_eglSwapBuffers(dpy, surface);
}

void *main_thread(void *) {
	while (!m_IL2CPP) {
        m_IL2CPP = Tools::GetBaseAddress(m_IL2CPPLIB);
        sleep(1);
    }

    while (!m_EGL) {
        m_EGL = dlopen_ex("libEGL.so", RTLD_NOW);
        sleep(1);
    }

    Il2CppAttach(m_IL2CPPLIB);
    sleep(5);
	
	Tools::Hook((void *) dlsym_ex(m_EGL, "eglSwapBuffers"), (void *) _eglSwapBuffers, (void **) &orig_eglSwapBuffers);
	
    pthread_t t;
    return 0;
}

__attribute__((constructor))
void lib_main() {
    pthread_t ptid;
    pthread_create(&ptid, NULL, main_thread, NULL);
}

JNIEXPORT jint JNICALL
JNI_OnLoad(JavaVM *vm, void *reserved) {
    JNIEnv *env;
    if (vm->GetEnv(reinterpret_cast<void **>(&env), JNI_VERSION_1_6) != JNI_OK) {
        return JNI_ERR;
    }
    return JNI_VERSION_1_6;
}
