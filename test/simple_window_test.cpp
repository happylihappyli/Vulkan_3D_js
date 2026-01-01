// Simple Windows Window Test
#define WIN32_LEAN_AND_MEAN
#include <windows.h>

LRESULT CALLBACK SimpleWindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
    switch (uMsg) {
        case WM_DESTROY:
            PostQuitMessage(0);
            return 0;
            
        case WM_PAINT:
            {
                PAINTSTRUCT ps;
                HDC hdc = BeginPaint(hwnd, &ps);
                
                // Draw simple text
                TextOutA(hdc, 10, 10, "Simple Window Test - Working!", 28);
                TextOutA(hdc, 10, 30, "Close window to exit", 19);
                
                EndPaint(hwnd, &ps);
            }
            return 0;
            
        default:
            return DefWindowProc(hwnd, uMsg, wParam, lParam);
    }
}

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow) {
    // Register window class
    WNDCLASSA wc = {};
    wc.lpfnWndProc = SimpleWindowProc;
    wc.hInstance = hInstance;
    wc.lpszClassName = "SimpleTestWindow";
    wc.hCursor = LoadCursor(nullptr, IDC_ARROW);
    wc.hbrBackground = reinterpret_cast<HBRUSH>(COLOR_WINDOW + 1);
    
    if (!RegisterClassA(&wc)) {
        MessageBoxA(nullptr, "Window class registration failed", "Error", MB_OK | MB_ICONERROR);
        return -1;
    }
    
    // Create window
    HWND hwnd = CreateWindowExA(
        0,
        "SimpleTestWindow",
        "Simple Window Test",
        WS_OVERLAPPEDWINDOW,
        CW_USEDEFAULT, CW_USEDEFAULT, 400, 300,
        nullptr,
        nullptr,
        hInstance,
        nullptr
    );
    
    if (!hwnd) {
        MessageBoxA(nullptr, "Window creation failed", "Error", MB_OK | MB_ICONERROR);
        return -1;
    }
    
    // Show window
    ShowWindow(hwnd, nCmdShow);
    UpdateWindow(hwnd);
    
    // Message loop
    MSG msg = {};
    while (GetMessage(&msg, nullptr, 0, 0)) {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }
    
    return static_cast<int>(msg.wParam);
}