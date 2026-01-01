#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <GL/gl.h>
#include <GL/glu.h>
#include <cstdio>
#include "utils.h"
#include "viewer.h"

static PointCloudViewer* g_viewer = nullptr;

LRESULT CALLBACK WndProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
    switch (uMsg) {
        case WM_CREATE:
            g_viewer = new PointCloudViewer(hwnd);
            if (!g_viewer->initialize()) {
                MessageBoxA(hwnd, "OpenGL initialization failed!", "Error", MB_OK | MB_ICONERROR);
                PostQuitMessage(1);
                return 0;
            }
            return 0;
            
        case WM_PAINT:
            if (g_viewer) {
                g_viewer->render();
            }
            ValidateRect(hwnd, nullptr);
            return 0;
            
        case WM_LBUTTONDOWN:
            if (g_viewer) {
                int x = LOWORD(lParam);
                int y = HIWORD(lParam);
                g_viewer->onMouseDown(x, y);
            }
            return 0;
            
        case WM_LBUTTONUP:
            if (g_viewer) {
                g_viewer->onMouseUp();
            }
            return 0;
            
        case WM_MOUSEMOVE:
            if (g_viewer) {
                int x = LOWORD(lParam);
                int y = HIWORD(lParam);
                g_viewer->onMouseMove(x, y);
                InvalidateRect(hwnd, nullptr, FALSE);
            }
            return 0;
            
        case WM_MOUSEWHEEL:
            if (g_viewer) {
                int delta = HIWORD(wParam);
                g_viewer->onMouseWheel(delta);
                InvalidateRect(hwnd, nullptr, FALSE);
            }
            return 0;
            
        case WM_SIZE:
            if (g_viewer) {
                int width = LOWORD(lParam);
                int height = HIWORD(lParam);
                g_viewer->updateViewport(width, height);
            }
            return 0;
            
        case WM_KEYDOWN:
            if (wParam == VK_ESCAPE) {
                PostQuitMessage(0);
            } else if (wParam == 'J' || wParam == 'j') {
                if (g_viewer) {
                    wprintf(L"按J键：尝试加载OBJ模型\n");
                    g_viewer->loadJSModel("test/sample.obj");
                    InvalidateRect(hwnd, nullptr, FALSE);
                }
            } else if (wParam == 'T' || wParam == 't') {
                if (g_viewer) {
                    wprintf(L"按T键：执行测试 JavaScript 代码\n");
                    std::string testCode = R"(
                        // 测试 JavaScript 代码
                        console.log("Hello from QuickJS!");
                        var x = 10;
                        var y = 20;
                        var result = x + y;
                        console.log("10 + 20 = " + result);
                    )";
                    g_viewer->executeJSCode(testCode);
                }
            } else if (wParam == 'L' || wParam == 'l') {
                if (g_viewer) {
                    wprintf(L"按L键：加载 JavaScript 脚本文件\n");
                    g_viewer->loadJSScript("test/test_script.js");
                    InvalidateRect(hwnd, nullptr, FALSE);
                }
            } else if (wParam == 'M' || wParam == 'm') {
                if (g_viewer) {
                    wprintf(L"按M键：切换模型渲染模式\n");
                    g_viewer->toggleModelWireframeMode();
                    InvalidateRect(hwnd, nullptr, FALSE);
                }
            } else if (wParam == VK_UP) {
                if (g_viewer) {
                    wprintf(L"按上方向键：放大模型\n");
                    g_viewer->adjustModelScale(0.1f);
                    InvalidateRect(hwnd, nullptr, FALSE);
                }
            } else if (wParam == VK_DOWN) {
                if (g_viewer) {
                    wprintf(L"按下方向键：缩小模型\n");
                    g_viewer->adjustModelScale(-0.1f);
                    InvalidateRect(hwnd, nullptr, FALSE);
                }
            } else if (wParam == VK_LEFT) {
                if (g_viewer) {
                    wprintf(L"按左方向键：模型向左旋转\n");
                    g_viewer->adjustModelRotation(0.0f, -5.0f);
                    InvalidateRect(hwnd, nullptr, FALSE);
                }
            } else if (wParam == VK_RIGHT) {
                if (g_viewer) {
                    wprintf(L"按右方向键：模型向右旋转\n");
                    g_viewer->adjustModelRotation(0.0f, 5.0f);
                    InvalidateRect(hwnd, nullptr, FALSE);
                }
            }
            return 0;
            
        case WM_CLOSE:
            if (g_viewer) {
                g_viewer->cleanup();
                delete g_viewer;
                g_viewer = nullptr;
            }
            PostQuitMessage(0);
            return 0;
    }
    
    return DefWindowProc(hwnd, uMsg, wParam, lParam);
}

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow) {
    setConsoleUTF8();
    
    AllocConsole();
    FILE* console_out;
    freopen_s(&console_out, "CONOUT$", "w", stdout);
    freopen_s(&console_out, "CONIN$", "r", stdin);
    
    wprintf(L"=== OpenGL点云查看器 启动信息 ===\n");
    wprintf(L"构建时间: %hs %hs\n", __DATE__, __TIME__);
    wprintf(L"窗口标题: OpenGL Point Cloud Viewer - 飞机形状\n");
    wprintf(L"点云大小: 10万个点（飞机形状）\n");
    wprintf(L"飞机尺寸: 长度=25.0, 翼展=28.0, 高度=9.0\n");
    wprintf(L"相机距离: 38.0\n");
    wprintf(L"模型加载器: OBJ模型支持\n");
    wprintf(L"JavaScript引擎: QuickJS 集成\n");
    wprintf(L"操作说明:\n");
    wprintf(L"  鼠标左键拖动: 旋转视角\n");
    wprintf(L"  鼠标滚轮: 缩放视图\n");
    wprintf(L"  ESC键: 退出程序\n");
    wprintf(L"  J键: 加载OBJ模型\n");
    wprintf(L"  M键: 切换模型渲染模式（线框/实体）\n");
    wprintf(L"  上方向键: 放大模型\n");
    wprintf(L"  下方向键: 缩小模型\n");
    wprintf(L"  左方向键: 模型向左旋转\n");
    wprintf(L"  右方向键: 模型向右旋转\n");
    wprintf(L"  T键: 执行测试 JavaScript 代码\n");
    wprintf(L"  L键: 加载并执行 JavaScript 脚本文件\n");
    wprintf(L"===========================================\n");
    
    const char* className = "PointCloudViewer";
    
    WNDCLASS wc = {0};
    wc.style = CS_HREDRAW | CS_VREDRAW | CS_OWNDC;
    wc.lpfnWndProc = WndProc;
    wc.hInstance = hInstance;
    wc.hIcon = LoadIconA(nullptr, IDI_APPLICATION);
    wc.hCursor = LoadCursorA(nullptr, IDC_ARROW);
    wc.hbrBackground = static_cast<HBRUSH>(GetStockObject(BLACK_BRUSH));
    wc.lpszClassName = className;
    
    if (!RegisterClassA(&wc)) {
        MessageBoxA(nullptr, "Failed to register window class!", "Error", MB_OK | MB_ICONERROR);
        return -1;
    }
    
    wprintf(L"正在创建窗口...\n");
    HWND hwnd = CreateWindowExA(0, className, "OpenGL Point Cloud Viewer - 飞机形状",
        WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, CW_USEDEFAULT, 800, 600,
        nullptr, nullptr, hInstance, nullptr);
    
    if (!hwnd) {
        MessageBoxA(nullptr, "Failed to create window!", "Error", MB_OK | MB_ICONERROR);
        return -1;
    }
    
    ShowWindow(hwnd, nCmdShow);
    
    MSG msg = {0};
    int frameCount = 0;
    DWORD lastTime = GetTickCount();
    
    wprintf(L"窗口已创建，消息循环开始...\n");
    
    while (GetMessage(&msg, nullptr, 0, 0)) {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
        
        frameCount++;
        if (frameCount % 100 == 0) {
            DWORD currentTime = GetTickCount();
            float fps = 100.0f * 1000.0f / (currentTime - lastTime);
            wprintf(L"FPS: %f | 帧数: %d\n", fps, frameCount);
            lastTime = currentTime;
        }
    }
    
    wprintf(L"程序退出，共渲染 %d 帧\n", frameCount);
    
    FreeConsole();
    
    return static_cast<int>(msg.wParam);
}
