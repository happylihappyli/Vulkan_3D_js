#!/usr/bin/env python3
# -*- coding: utf-8 -*-
"""
TTS语音提示功能 - 任务完成后播放语音提示
"""

import os
import sys
import subprocess

def speak_chinese(text):
    """播放中文语音提示"""
    try:
        # 使用Windows TTS播放中文语音
        if sys.platform == "win32":
            # 使用PowerShell调用SAPI进行TTS
            ps_script = f'''
Add-Type -TypeDefinition "
using System;
using System.Speech.Synthesis;
public class TTSHelper {{
    public static void Speak(string text) {{
        var synth = new SpeechSynthesizer();
        synth.Speak(text);
    }}
}}"
[TTSHelper]::Speak("{text}")
'''
            subprocess.run(["powershell", "-Command", ps_script], 
                         capture_output=True, text=True, encoding='utf-8')
            print(f"语音提示: {text}")
        else:
            # 其他平台的备用方案
            print(f"语音提示: {text}")
            
    except Exception as e:
        print(f"语音提示播放失败: {e}")
        print(f"提示内容: {text}")

def main():
    """主函数 - 处理命令行参数"""
    if len(sys.argv) < 2:
        text = "任务运行完毕，过来看看！"
    else:
        text = sys.argv[1]
    
    speak_chinese(text)
    return True

if __name__ == '__main__':
    success = main()
    sys.exit(0 if success else 1)