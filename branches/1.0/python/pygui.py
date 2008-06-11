#!/usr/bin/env python

# Copyright (c) 2007-2008, FutureToday. All rights reserved.
#  author: abak

import os
import wx
import threading

import guiwindow

class GuiThread(threading.Thread):
	def __init__(self):
		threading.Thread.__init__(self)        

	def run(self):
		guiwindow.Open()


gui = GuiThread()



def start():
	gui.start()

def stop():
	gui.stop()
