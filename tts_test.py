import win32com.client

speaker = win32com.client.Dispatch("SAPI.SpVoice")
speaker.Speak("QuickJS集成测试完成，过来看看！")
