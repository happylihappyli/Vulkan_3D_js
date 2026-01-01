import win32com.client as wincl
import sys

def speak(text):
    """使用Windows TTS播放语音"""
    try:
        speaker = wincl.Dispatch("SAPI.SpVoice")
        speaker.Speak(text)
    except Exception as e:
        print(f"TTS播放失败: {e}")

if __name__ == "__main__":
    if len(sys.argv) > 1:
        speak(sys.argv[1])
    else:
        speak("任务运行完毕，过来看看！")
