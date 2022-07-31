#pragma once

#define m_IL2CPPLIB "libil2cpp.so"
uintptr_t m_IL2CPP;
bool g_Initialized, clearMousePos = true;
void *m_EGL;

enum TouchPhase : int {
	Began = 0,
	Moved = 1,
	Stationary = 2,
	Ended = 3,
	Canceled = 4,
};

struct UnityEngine_Touch_Fields {
    int32_t m_FingerId;
    Vector2 m_Position;
    Vector2 m_RawPosition;
    Vector2 m_PositionDelta;
    float m_TimeDelta;
    int32_t m_TapCount;
    int32_t m_Phase;
    int32_t m_Type;
    float m_Pressure;
    float m_maximumPossiblePressure;
    float m_Radius;
    float m_RadiusVariance;
    float m_AltitudeAngle;
    float m_AzimuthAngle;
};

/*Class Input*/
#define Input_GetTouch (uintptr_t) Il2CppGetMethodOffset("UnityEngine.dll", "UnityEngine", "Input", "GetTouch", 1)
#define Input_get_touchSupported (uintptr_t) Il2CppGetMethodOffset("UnityEngine.dll", "UnityEngine", "Input", "get_touchSupported")
#define Input_get_touchCount (uintptr_t) Il2CppGetMethodOffset("UnityEngine.dll", "UnityEngine", "Input", "get_touchCount")
#define Input_get_mousePosition (uintptr_t) Il2CppGetMethodOffset("UnityEngine.dll", "UnityEngine", "Input", "get_mousePosition")

bool get_touchSupported(void *instance) {
	return reinterpret_cast<bool(__fastcall *)(void *)>(Input_get_touchSupported)(instance);
}

int get_touchCount() {
	return reinterpret_cast<int(__fastcall *)()>(Input_get_touchCount)();
}

Vector3 get_mousePosition(void *instance) {
	return reinterpret_cast<Vector3(__fastcall *)(void *)>(Input_get_mousePosition)(instance);
}

/*Class Screen*/
#define Screen_get_width (uintptr_t) Il2CppGetMethodOffset("UnityEngine.dll", "UnityEngine", "Screen", "get_width")
#define Screen_get_height (uintptr_t) Il2CppGetMethodOffset("UnityEngine.dll", "UnityEngine", "Screen", "get_height")

int get_width() {
	return reinterpret_cast<int(__fastcall *)()>(Screen_get_width)();
}

int get_height() {
	return reinterpret_cast<int(__fastcall *)()>(Screen_get_height)();
}

void ImGui_GetTouch(ImGuiIO* io, int screenHeight) {
	void *instance;
    if (get_touchCount() > 0) {
        if (get_touchSupported(instance)) {
            UnityEngine_Touch_Fields touch = ((UnityEngine_Touch_Fields (*)(void *, int))(Input_GetTouch))(instance, 0);
            switch (touch.m_Phase) {
            case TouchPhase::Began:
            case TouchPhase::Stationary:
                io->MouseDown[0] = true;
                ImGui::GetIO().MousePos = ImVec2(get_mousePosition(instance).x, screenHeight - get_mousePosition(instance).y);
                break;
            case TouchPhase::Ended:
            case TouchPhase::Canceled:
                io->MouseDown[0] = false;
                clearMousePos = true;
                break;
            case TouchPhase::Moved:
                ImGui::GetIO().MousePos = ImVec2(get_mousePosition(instance).x, screenHeight - get_mousePosition(instance).y);
                break;
            default:
                break;
            }
        }
    }
}
