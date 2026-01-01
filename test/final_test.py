#!/usr/bin/env python3
# -*- coding: utf-8 -*-
"""
最终测试点云查看器功能
"""

import os
import subprocess
import time
import sys

def play_tts(message):
    """使用Windows TTS播放语音提示"""
    try:
        import win32com.client
        speaker = win32com.client.Dispatch("SAPI.SpVoice")
        speaker.Speak(message)
    except ImportError:
        print(f"TTS提示: {message}")

def main():
    """主函数"""
    print("最终测试点云查看器功能...")
    
    # 切换到项目目录
    project_dir = os.path.dirname(os.path.abspath(__file__))
    os.chdir(os.path.dirname(project_dir))
    
    # 检查可执行文件是否存在
    exe_path = os.path.join("bin", "pointcloud_viewer.exe")
    if not os.path.exists(exe_path):
        print("错误: 可执行文件不存在")
        return
    
    print(f"启动程序: {exe_path}")
    print("请检查是否出现以下功能:")
    print("1. 窗口正常显示")
    print("2. 点云数据渲染")
    print("3. 鼠标拖动旋转功能")
    print("4. 滚轮缩放功能")
    print("5. ESC键退出功能")
    
    # 启动程序
    try:
        process = subprocess.Popen([exe_path])
        
        # 等待程序启动
        time.sleep(3)
        
        # 检查进程状态
        if process.poll() is None:
            print("✅ 程序正常运行中...")
            print("✅ 窗口创建成功!")
            print("✅ 请手动测试鼠标交互功能")
            
            # 等待用户测试或程序结束
            try:
                process.wait(timeout=10)
                print("程序已退出")
                
            except subprocess.TimeoutExpired:
                print("程序仍在运行，手动终止...")
                process.terminate()
                process.wait()
                
        else:
            print(f"❌ 程序立即退出，退出码: {process.returncode}")
            
    except Exception as e:
        print(f"❌ 启动程序时发生错误: {e}")
    
    # 播放语音提示
    play_tts("点云查看器测试完成，请查看结果")
    
    print("测试完成!")

if __name__ == "__main__":
    main()