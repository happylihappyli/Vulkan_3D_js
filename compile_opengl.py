#!/usr/bin/env python3
# -*- coding: utf-8 -*-
"""
编译OpenGL点云查看器的脚本
"""

import subprocess
import os
import sys

def compile_opengl(source_file, output_name):
    """编译OpenGL版本的点云查看器"""
    
    # 设置Visual Studio环境
    vs_path = r"D:\Code\VS2022\Community\VC\Auxiliary\Build\vcvars64.bat"
    
    if not os.path.exists(vs_path):
        print(f"错误：找不到Visual Studio环境文件 {vs_path}")
        return False
    
    print("正在设置Visual Studio环境...")
    
    try:
        # 先设置环境变量
        env_result = subprocess.run([vs_path], capture_output=True, text=True, shell=True)
        
        if env_result.returncode != 0:
            print("无法设置Visual Studio环境")
            return False
            
        print("Visual Studio环境设置成功")
        
        # 编译命令
        compile_cmd = [
            "cl",
            "/std:c++20",
            source_file,
            f"/Fe:bin\\{output_name}.exe",
            "/link", "user32", "gdi32", "opengl32", "glu32"
        ]
        
        print("开始编译OpenGL点云查看器...")
        print(f"命令: {' '.join(compile_cmd)}")
        
        # 执行编译
        result = subprocess.run(compile_cmd, capture_output=True, text=True, shell=True)
        
        if result.returncode == 0:
            print("编译成功！")
            print(f"可执行文件位置: bin\\{output_name}.exe")
            return True
        else:
            print("编译失败！")
            print("错误输出:")
            print(result.stderr)
            print("标准输出:")
            print(result.stdout)
            return False
            
    except Exception as e:
        print(f"编译过程中出现异常: {e}")
        return False

if __name__ == "__main__":
    # 获取命令行参数
    if len(sys.argv) < 3:
        print("使用方法: python compile_opengl.py <源文件> <输出名称>")
        sys.exit(1)
    
    source_file = sys.argv[1]
    output_name = sys.argv[2]
    
    os.chdir(r"e:\GitHub3\cpp\Vulkan_Test2")
    success = compile_opengl(source_file, output_name)
    sys.exit(0 if success else 1)