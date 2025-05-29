#include <SFML/Graphics.hpp>
#include <windows.h>
#include <dwmapi.h>
#include <GL/gl.h>
#include <GL/glu.h>
#include <vector>
#include <random>
#include <fstream>
#include <string>
#include <Windowsx.h> 
#pragma comment(lib, "dwmapi.lib")

struct Vec3 {
    float x, y, z;
};

Vec3 lerp(const Vec3& a, const Vec3& b, float t) {
    return { a.x + (b.x - a.x) * t,
             a.y + (b.y - a.y) * t,
             a.z + (b.z - a.z) * t };
}

float randomOffset(float range = 0.4f) {
    static std::default_random_engine engine{ std::random_device{}() };
    static std::uniform_real_distribution<float> dist(-range, range);
    return dist(engine);
}

std::vector<Vec3> generateDistortedCubeVertices() {
    float base[8][3] = {
        {-1, -1, -1}, {-1, -1,  1},
        {-1,  1, -1}, {-1,  1,  1},
         {1, -1, -1}, { 1, -1,  1},
         {1,  1, -1}, { 1,  1,  1},
    };
    std::vector<Vec3> vertices;
    for (auto& v : base) {
        vertices.push_back({ (v[0] + randomOffset()) * 2.0f,
                             (v[1] + randomOffset()) * 2.0f,
                             (v[2] + randomOffset()) * 2.0f });
    }
    return vertices;
}

void drawCube(const std::vector<Vec3>& v) {
    glBegin(GL_QUADS);
    const int faces[6][4] = {
        {0,1,3,2}, {4,6,7,5},
        {0,4,5,1}, {2,3,7,6},
        {0,2,6,4}, {1,5,7,3},
    };
    const float colors[6][3] = {
        {1,0,0}, {0,1,0}, {0,0,1},
        {1,1,0}, {1,0,1}, {0,1,1}
    };
    for (int i = 0; i < 6; ++i) {
        glColor4f(colors[i][0], colors[i][1], colors[i][2], 0.77f);
        for (int j = 0; j < 4; ++j) {
            Vec3 vert = v[faces[i][j]];
            glVertex3f(vert.x, vert.y, vert.z);
        }
    }
    glEnd();
}

POINT selectedPos = { 100, 100 }; // default
bool launchWidget = false;

LRESULT CALLBACK OverlayProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam) {
    switch (msg) {
    case WM_LBUTTONDOWN:
        selectedPos.x = GET_X_LPARAM(lParam);
        selectedPos.y = GET_Y_LPARAM(lParam);
        {
            std::ofstream file("widget_pos.txt");
            file << selectedPos.x << " " << selectedPos.y;
        }
        launchWidget = true;
        PostMessage(hwnd, WM_CLOSE, 0, 0);
        return 0;

    case WM_KEYDOWN:
        if (wParam == 'S') {
            std::ifstream file("widget_pos.txt");
            if (!file) {
                MessageBox(hwnd, L"Position not set yet.", L"Info", MB_OK);
                break;
            }
            file >> selectedPos.x >> selectedPos.y;
            launchWidget = true;
            PostMessage(hwnd, WM_CLOSE, 0, 0);
        }
        return 0;

    case WM_PAINT:
    {
        PAINTSTRUCT ps;
        HDC hdc = BeginPaint(hwnd, &ps);
        RECT rect;
        GetClientRect(hwnd, &rect);
        HBRUSH brush = CreateSolidBrush(RGB(0, 0, 0));
        FillRect(hdc, &rect, brush);
        SetBkMode(hdc, TRANSPARENT);
        SetTextColor(hdc, RGB(255, 255, 255));
        DrawText(hdc, L"Choose the position", -1, &rect, DT_CENTER | DT_TOP | DT_SINGLELINE);
        RECT rect2 = rect;
        rect2.top += 40;
        DrawText(hdc, L"Set previous: S", -1, &rect2, DT_CENTER | DT_TOP | DT_SINGLELINE);
        DeleteObject(brush);
        EndPaint(hwnd, &ps);
    }
    return 0;

    case WM_DESTROY:
        PostQuitMessage(0);
        return 0;
    }
    return DefWindowProc(hwnd, msg, wParam, lParam);
}

void RunOverlay(HINSTANCE hInst) {
    WNDCLASSEX wc = { sizeof(WNDCLASSEX), CS_HREDRAW | CS_VREDRAW, OverlayProc,
        0, 0, hInst, nullptr, LoadCursor(nullptr, IDC_ARROW),
        (HBRUSH)(COLOR_WINDOW + 1), nullptr, L"OverlayClass", nullptr };
    RegisterClassEx(&wc);

    HWND hwnd = CreateWindowEx(WS_EX_TOPMOST | WS_EX_LAYERED,
        L"OverlayClass", L"", WS_POPUP,
        0, 0, GetSystemMetrics(SM_CXSCREEN), GetSystemMetrics(SM_CYSCREEN),
        nullptr, nullptr, hInst, nullptr);

    SetLayeredWindowAttributes(hwnd, 0, 200, LWA_ALPHA);
    ShowWindow(hwnd, SW_SHOW);
    UpdateWindow(hwnd);
    ShowWindow(hwnd, SW_SHOW);

    MSG msg;
    while (GetMessage(&msg, nullptr, 0, 0)) {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }
}
LRESULT CALLBACK CubeProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam) {
    switch (msg) {
    case WM_CLOSE:
        PostQuitMessage(0);
        return 0;
    }
    return DefWindowProc(hwnd, msg, wParam, lParam);
}

void RunCube(HWND parent = nullptr) {
    auto CubeProc = [](HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam) -> LRESULT {
        switch (msg) {
        case WM_CLOSE:
            DestroyWindow(hwnd);
            return 0;
        case WM_DESTROY:
            PostQuitMessage(0);
            return 0;
        }
        return DefWindowProc(hwnd, msg, wParam, lParam);
        };

    WNDCLASS wc = { 0 };
    wc.lpfnWndProc = CubeProc;
    wc.hInstance = GetModuleHandle(nullptr);
    wc.lpszClassName = L"CubeWidget";
    RegisterClass(&wc);

    HWND hwnd = CreateWindowEx(
        WS_EX_LAYERED | WS_EX_TOPMOST | WS_EX_TRANSPARENT | WS_EX_APPWINDOW,
        L"CubeWidget", L"My Cube", WS_POPUP,
        selectedPos.x, selectedPos.y, 300, 300,
        nullptr, nullptr, GetModuleHandle(nullptr), nullptr);

    SetLayeredWindowAttributes(hwnd, RGB(0, 0, 0), 0, LWA_COLORKEY);
    ShowWindow(hwnd, SW_SHOW);

    HDC hdc = GetDC(hwnd);
    PIXELFORMATDESCRIPTOR pfd = {
        sizeof(PIXELFORMATDESCRIPTOR), 1,
        PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL | PFD_DOUBLEBUFFER,
        PFD_TYPE_RGBA, 32,
        0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0,
        16, 0, 0,
        PFD_MAIN_PLANE, 0, 0, 0, 0
    };
    int pf = ChoosePixelFormat(hdc, &pfd);
    SetPixelFormat(hdc, pf, &pfd);
    HGLRC rc = wglCreateContext(hdc);
    wglMakeCurrent(hdc, rc);

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glEnable(GL_DEPTH_TEST);
    glClearColor(0, 0, 0, 0);

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(90.f, 1.0f, 1.f, 500.f);

    auto fromVertices = generateDistortedCubeVertices();
    auto toVertices = generateDistortedCubeVertices();
    auto currentVertices = fromVertices;

    float elapsed = 0.0f, angle = 0.0f;
    const float transitionTime = 1.0f;
    DWORD lastTime = GetTickCount();

    MSG msg = {};
    bool running = true;
    while (running) {
        while (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE)) {
            if (msg.message == WM_QUIT) {
                running = false;
                break;
            }
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }

        if (!running)
            break;

        DWORD now = GetTickCount();
        float deltaTime = (now - lastTime) / 1000.0f;
        lastTime = now;
        elapsed += deltaTime;

        float t = std::min(elapsed / transitionTime, 1.0f);
        currentVertices.clear();
        for (size_t i = 0; i < fromVertices.size(); ++i)
            currentVertices.push_back(lerp(fromVertices[i], toVertices[i], t));

        if (t >= 1.0f) {
            elapsed = 0.0f;
            fromVertices = toVertices;
            toVertices = generateDistortedCubeVertices();
        }

        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glMatrixMode(GL_MODELVIEW);
        glLoadIdentity();
        glTranslatef(0.f, 0.f, -6.f);
        glRotatef(angle, 1.f, 1.f, 0.f);
        drawCube(currentVertices);
        SwapBuffers(hdc);

        angle += 50.0f * deltaTime;
        Sleep(10);
    }

    wglMakeCurrent(nullptr, nullptr);
    wglDeleteContext(rc);
    ReleaseDC(hwnd, hdc);
    DestroyWindow(hwnd);
}


int WINAPI WinMain(HINSTANCE hInst, HINSTANCE, LPSTR, int) 
{

    RunOverlay(hInst);
    if (launchWidget) {
        RunCube();
    }
    return 0;
}