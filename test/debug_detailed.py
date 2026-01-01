#!/usr/bin/env python3
# -*- coding: utf-8 -*-
"""
详细调试Win32 API版本的点云查看器
"""

import os
import subprocess
import time
import sys

def main():
    """主函数"""
    print("详细调试Win32 API点云查看器...")
    
    # 切换到项目目录
    project_dir = os.path.dirname(os.path.abspath(__file__))
    os.chdir(os.path.dirname(project_dir))
    
    # 检查可执行文件是否存在
    exe_path = os.path.join("bin", "pointcloud_viewer.exe")
    if not os.path.exists(exe_path):
        print("错误: 可执行文件不存在")
        return
    
    print(f"启动程序: {exe_path}")
    
    # 启动程序并捕获所有输出
    try:
        process = subprocess.Popen([exe_path], 
                                 stdout=subprocess.PIPE, 
                                 stderr=subprocess.PIPE,
                                 text=True,
                                 encoding='utf-8',
                                 bufsize=1,
                                 universal_newlines=True)
        
        # 实时读取输出
        print("等待程序输出...")
        
        # 读取标准输出和错误输出
        stdout_lines = []
        stderr_lines = []
        
        # 等待一段时间
        time.sleep(3)
        
        # 检查进程状态
        if process.poll() is not None:
            print(f"程序已退出，退出码: {process.returncode}")
            
            # 读取剩余输出
            stdout, stderr = process.communicate(timeout=1)
            if stdout:
                print("标准输出:")
                print(stdout)
            if stderr:
                print("错误输出:")
                print(stderr)
        else:
            print("程序仍在运行，尝试终止...")
            process.terminate()
            stdout, stderr = process.communicate(timeout=2)
            if stdout:
                print("标准输出:")
                print(stdout)
            if stderr:
                print("错误输出:")
                print(stderr)
                
    except subprocess.TimeoutExpired:
        print("读取输出超时")
        process.kill()
        stdout, stderr = process.communicate()
        if stdout:
            print("标准输出:")
            print(stdout)
        if stderr:
            print("错误输出:")
            print(stderr)
    except Exception as e:
        print(f"调试过程中发生错误: {e}")

if __name__ == "__main__":
    main()