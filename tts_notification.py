#!/usr/bin/env python3
# -*- coding: utf-8 -*-
import win32com.client

speak = win32com.client.Dispatch("SAPI.SpVoice")
speak.Speak("任务运行完毕，过来看看！")
