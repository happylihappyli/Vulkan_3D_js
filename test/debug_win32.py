#!/usr/bin/env python3
# -*- coding: utf-8 -*-
"""
调试Win32 API版本的点云查看器
"""

import os
import subprocess
import time
import psutil

def check_process_running(process_name):
    """检查进程是否在运行"""
    for proc in psutil.process_iter(['name']):
        try:
            if process_name.lower() in proc.info['name'].lower():
                return True
        except (psutil.NoSuchProcess, psutil.AccessDenied):
            pass
    return False

def main():
    """主函数"""
    print("启动Win32 API点云查看器进行调试...")
    
    # 切换到项目目录
    project_dir = os.path.dirname(os.path.abspath(__file__))
    os.chdir(os.path.dirname(project_dir))
    
    # 检查可执行文件是否存在
    exe_path = os.path.join("bin", "pointcloud_viewer.exe")
    if not os.path.exists(exe_path):
        print("错误: 可执行文件不存在")
        return
    
    print(f"启动程序: {exe_path}")
    
    # 启动程序
    try:
        process = subprocess.Popen([exe_path], 
                                 stdout=subprocess.PIPE, 
                                 stderr=subprocess.PIPE,
                                 text=True,
                                 encoding='utf-8')
        
        # 等待程序启动
        time.sleep(2)
        
        # 检查进程是否在运行
        if process.poll() is None:
            print("程序正在运行...")
            print("请检查是否出现窗口")
            
            # 等待用户输入或程序结束
            try:
                process.wait(timeout=10)
                print("程序已退出")
                
                # 获取输出
                stdout, stderr = process.communicate()
                if stdout:
                    print("标准输出:")
                    print(stdout)
                if stderr:
                    print("错误输出:")
                    print(stderr)
                    
            except subprocess.TimeoutExpired:
                print("程序仍在运行，手动终止...")
                process.terminate()
                process.wait()
                
        else:
            print("程序立即退出，退出码:", process.returncode)
            stdout, stderr = process.communicate()
            if stdout:
                print("标准输出:")
                print(stdout)
            if stderr:
                print("错误输出:")
                print(stderr)
                
    except Exception as e:
        print(f"启动程序时发生错误: {e}")

if __name__ == "__main__":
    main()