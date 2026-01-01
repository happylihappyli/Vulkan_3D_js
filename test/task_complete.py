#!/usr/bin/env python3
# -*- coding: utf-8 -*-
"""
任务完成语音提示脚本
用于在C++项目编译和修改完成后播放语音提示
"""

import win32com.client
import time

def play_tts_message(message):
    """
    使用Windows TTS播放语音消息
    """
    try:
        # 创建TTS语音对象
        speaker = win32com.client.Dispatch("SAPI.SpVoice")
        
        # 设置语音参数
        speaker.Rate = 0  # 语速（-10到10）
        speaker.Volume = 100  # 音量（0到100）
        
        # 播放语音消息
        print(f"播放语音提示: {message}")
        speaker.Speak(message)
        
        print("语音提示播放完成")
        return True
        
    except Exception as e:
        print(f"语音播放失败: {e}")
        return False

def main():
    """
    主函数 - 播放任务完成语音提示
    """
    message = "任务运行完毕，过来看看！"
    
    print("=== C++项目任务完成语音提示 ===")
    print(f"当前时间: {time.strftime('%Y-%m-%d %H:%M:%S')}")
    print("=" * 50)
    
    # 播放语音提示
    if play_tts_message(message):
        print("✅ 语音提示已成功播放")
    else:
        print("❌ 语音提示播放失败")
    
    print("=" * 50)

if __name__ == "__main__":
    main()