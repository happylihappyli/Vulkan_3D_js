#!/usr/bin/env python3
# -*- coding: utf-8 -*-
"""
详细诊断OpenGL点云查看器的问题
"""

import subprocess
import time
import os
import threading
import sys

def detailed_test():
    """详细测试并捕获错误信息"""
    print("🔍 详细诊断OpenGL点云查看器")
    print("=" * 50)
    
    exe_path = r"e:\GitHub3\cpp\Vulkan_Test2\bin\opengl_viewer.exe"
    
    if not os.path.exists(exe_path):
        print(f"❌ 可执行文件不存在: {exe_path}")
        return
    
    print(f"📁 可执行文件: {exe_path}")
    print(f"📊 文件大小: {os.path.getsize(exe_path):,} 字节")
    
    print("\n🚀 启动程序并捕获详细输出...")
    
    try:
        # 使用Popen启动程序并实时获取输出
        process = subprocess.Popen(
            [exe_path],
            stdout=subprocess.PIPE,
            stderr=subprocess.PIPE,
            text=True,
            cwd=os.path.dirname(exe_path),
            bufsize=1,
            universal_newlines=True
        )
        
        print(f"📋 进程ID: {process.pid}")
        
        # 实时读取输出
        def read_output(pipe, label):
            try:
                for line in iter(pipe.readline, ''):
                    print(f"[{label}] {line.strip()}")
            except:
                pass
        
        # 创建线程读取输出
        stdout_thread = threading.Thread(target=read_output, args=(process.stdout, "STDOUT"))
        stderr_thread = threading.Thread(target=read_output, args=(process.stderr, "STDERR"))
        
        stdout_thread.daemon = True
        stderr_thread.daemon = True
        
        stdout_thread.start()
        stderr_thread.start()
        
        print("⏳ 等待程序启动...")
        time.sleep(3)
        
        # 检查进程状态
        if process.poll() is None:
            print("✅ 程序正在运行")
            
            # 运行一段时间
            print("⏱️  运行5秒...")
            time.sleep(5)
            
            print("🔚 尝试正常结束程序...")
            try:
                process.terminate()
                
                # 等待进程结束
                for i in range(10):  # 最多等待10秒
                    if process.poll() is not None:
                        break
                    time.sleep(1)
                    print(f"⏳ 等待结束... ({i+1}/10)")
                
                if process.poll() is None:
                    print("⚠️  强制终止进程")
                    process.kill()
                
            except Exception as e:
                print(f"❌ 终止进程时出错: {e}")
                process.kill()
        else:
            print("⚠️  程序已经结束")
        
        # 获取剩余输出
        try:
            stdout, stderr = process.communicate(timeout=2)
            if stdout:
                print("\n📤 剩余标准输出:")
                print(stdout)
            if stderr:
                print("\n📤 剩余错误输出:")
                print(stderr)
        except:
            pass
        
        exit_code = process.returncode
        print(f"\n🔚 最终退出码: {exit_code}")
        
        if exit_code == 0:
            print("✅ 程序正常结束")
        else:
            print(f"❌ 程序异常结束 (退出码: {exit_code})")
            
    except Exception as e:
        print(f"❌ 启动程序时出错: {e}")
    
    print("\n" + "=" * 50)

def check_opengl_capability():
    """检查OpenGL能力"""
    print("\n🔧 检查OpenGL能力...")
    
    # 创建一个简单的OpenGL测试程序
    test_cpp = r'''
#include <windows.h>
#include <GL/gl.h>
#include <GL/glu.h>

int main() {
    // 尝试获取DC和创建OpenGL上下文
    HDC hdc = GetDC(NULL);
    if (!hdc) {
        printf("ERROR: 无法获取DC\n");
        return 1;
    }
    
    // 检查OpenGL支持
    PIXELFORMATDESCRIPTOR pfd = {};
    pfd.nSize = sizeof(PIXELFORMATDESCRIPTOR);
    pfd.nVersion = 1;
    pfd.dwFlags = PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL | PFD_DOUBLEBUFFER;
    pfd.iPixelType = PFD_TYPE_RGBA;
    pfd.cColorBits = 32;
    
    int pixelFormat = ChoosePixelFormat(hdc, &pfd);
    if (pixelFormat == 0) {
        printf("ERROR: 无法选择像素格式\n");
        ReleaseDC(NULL, hdc);
        return 1;
    }
    
    if (!SetPixelFormat(hdc, pixelFormat, &pfd)) {
        printf("ERROR: 无法设置像素格式\n");
        ReleaseDC(NULL, hdc);
        return 1;
    }
    
    HGLRC hrc = wglCreateContext(hdc);
    if (!hrc) {
        printf("ERROR: 无法创建OpenGL上下文\n");
        ReleaseDC(NULL, hdc);
        return 1;
    }
    
    if (!wglMakeCurrent(hdc, hrc)) {
        printf("ERROR: 无法设置当前OpenGL上下文\n");
        wglDeleteContext(hrc);
        ReleaseDC(NULL, hdc);
        return 1;
    }
    
    // 获取OpenGL版本信息
    const GLubyte* version = glGetString(GL_VERSION);
    const GLubyte* vendor = glGetString(GL_VENDOR);
    const GLubyte* renderer = glGetString(GL_RENDERER);
    
    printf("SUCCESS: OpenGL版本: %s\\n", version ? (char*)version : "未知");
    printf("SUCCESS: 厂商: %s\\n", vendor ? (char*)vendor : "未知");
    printf("SUCCESS: 渲染器: %s\\n", renderer ? (char*)renderer : "未知");
    
    // 清理
    wglMakeCurrent(NULL, NULL);
    wglDeleteContext(hrc);
    ReleaseDC(NULL, hdc);
    
    return 0;
}
'''
    
    # 写入临时测试文件
    test_file = r"e:\GitHub3\cpp\Vulkan_Test2\test\opengl_test.cpp"
    try:
        with open(test_file, 'w', encoding='utf-8-sig') as f:
            f.write(test_cpp)
        
        print("📝 编译OpenGL测试程序...")
        
        # 编译测试程序
        compile_cmd = [
            'cl', '/std:c++20', '/W0', test_file, 
            '/Fe:test\\opengl_test.exe',
            'opengl32.lib', 'glu32.lib', 'user32.lib', 'gdi32.lib'
        ]
        
        result = subprocess.run(
            compile_cmd,
            capture_output=True,
            text=True,
            cwd=r"e:\GitHub3\cpp\Vulkan_Test2"
        )
        
        if result.returncode == 0:
            print("✅ OpenGL测试程序编译成功")
            
            print("🚀 运行OpenGL测试...")
            test_result = subprocess.run(
                [r"e:\GitHub3\cpp\Vulkan_Test2\test\opengl_test.exe"],
                capture_output=True,
                text=True,
                timeout=10
            )
            
            print("📤 OpenGL测试输出:")
            print(test_result.stdout)
            if test_result.stderr:
                print("❌ OpenGL测试错误:")
                print(test_result.stderr)
            
            if test_result.returncode == 0:
                print("✅ OpenGL支持正常")
            else:
                print(f"❌ OpenGL测试失败 (退出码: {test_result.returncode})")
        else:
            print("❌ OpenGL测试程序编译失败")
            print("编译错误:")
            print(result.stderr)
            
    except Exception as e:
        print(f"❌ OpenGL测试时出错: {e}")
    finally:
        # 清理临时文件
        try:
            if os.path.exists(test_file):
                os.remove(test_file)
            exe_file = r"e:\GitHub3\cpp\Vulkan_Test2\test\opengl_test.exe"
            if os.path.exists(exe_file):
                os.remove(exe_file)
        except:
            pass

def main():
    print("🧪 OpenGL点云查看器详细诊断")
    print("=" * 50)
    
    detailed_test()
    check_opengl_capability()
    
    print("\n🎯 诊断总结:")
    print("1. 如果OpenGL测试失败，可能是显卡驱动问题")
    print("2. 如果程序启动后立即退出，可能是OpenGL上下文创建失败")
    print("3. 如果程序运行时崩溃，可能是显存或内存不足")
    print("=" * 50)

if __name__ == "__main__":
    main()