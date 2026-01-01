// Debug window creation issue
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <iostream>

// Simple window procedure
LRESULT CALLBACK SimpleWindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
    switch (uMsg) {
        case WM_DESTROY:
            std::cout << "Received WM_DESTROY message" << std::endl;
            PostQuitMessage(0);
            return 0;
        case WM_PAINT:
            {
                PAINTSTRUCT ps;
                HDC hdc = BeginPaint(hwnd, &ps);
                
                // Draw simple text
                RECT rect;
                GetClientRect(hwnd, &rect);
                SetBkMode(hdc, TRANSPARENT);
                SetTextColor(hdc, RGB(255, 255, 255));
                
                std::string text = "Simple Window Test - Press ESC to exit";
                DrawTextA(hdc, text.c_str(), -1, &rect, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
                
                EndPaint(hwnd, &ps);
            }
            return 0;
        case WM_KEYDOWN:
            if (wParam == VK_ESCAPE) {
                std::cout << "Received ESC key, exiting program" << std::endl;
                PostQuitMessage(0);
            }
            return 0;
    }
    
    return DefWindowProc(hwnd, uMsg, wParam, lParam);
}

int main() {
    std::cout << "Starting window creation..." << std::endl;
    
    // Get instance handle
    HINSTANCE hInstance = GetModuleHandle(nullptr);
    std::cout << "Instance handle: " << hInstance << std::endl;
    
    // Register window class
    WNDCLASSA wc = {};
    wc.style = CS_HREDRAW | CS_VREDRAW;
    wc.lpfnWndProc = SimpleWindowProc;
    wc.cbClsExtra = 0;
    wc.cbWndExtra = 0;
    wc.hInstance = hInstance;
    wc.hIcon = LoadIcon(nullptr, IDI_APPLICATION);
    wc.hCursor = LoadCursor(nullptr, IDC_ARROW);
    wc.hbrBackground = reinterpret_cast<HBRUSH>(COLOR_WINDOW + 1);
    wc.lpszMenuName = nullptr;
    wc.lpszClassName = "SimpleTestWindow";
    
    ATOM classAtom = RegisterClassA(&wc);
    if (classAtom == 0) {
        DWORD error = GetLastError();
        std::cout << "Window class registration failed, error code: " << error << std::endl;
        return -1;
    }
    std::cout << "Window class registration successful" << std::endl;
    
    // Create window
    HWND hwnd = CreateWindowExA(
        0,
        "SimpleTestWindow",
        "Simple Window Test",
        WS_OVERLAPPEDWINDOW,
        CW_USEDEFAULT, CW_USEDEFAULT, 800, 600,
        nullptr,
        nullptr,
        hInstance,
        nullptr
    );
    
    if (hwnd == nullptr) {
        DWORD error = GetLastError();
        std::cout << "Window creation failed, error code: " << error << std::endl;
        return -1;
    }
    std::cout << "Window creation successful, handle: " << hwnd << std::endl;
    
    // Show window
    ShowWindow(hwnd, SW_SHOW);
    UpdateWindow(hwnd);
    std::cout << "Window displayed" << std::endl;
    
    // Message loop
    MSG msg = {};
    std::cout << "Entering message loop..." << std::endl;
    while (GetMessage(&msg, nullptr, 0, 0)) {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }
    
    std::cout << "Program exited normally" << std::endl;
    return 0;
}