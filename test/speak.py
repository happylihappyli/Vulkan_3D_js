import pyttsx3

def speak(text):
    """播放语音提示"""
    engine = pyttsx3.init()
    engine.say(text)
    engine.runAndWait()

if __name__ == "__main__":
    speak("任务运行完毕，过来看看！")
