#!/usr/bin/env python

# Copyright (c) 2007-2008, FutureToday. All rights reserved.
#  author: abak

import os
import wx
import threading

class GuiThread(threading.Thread):
	def __init__(self):
		threading.Thread.__init__(self)        

	def run(self):
		app = wx.PySimpleApp()
		frame = wx.Frame(None, wx.ID_ANY, "Arena GUI")
		frame.Show(True)
		app.MainLoop()	
		print "hello from trhead"



gui = GuiThread()



def start():
	gui.start()

def stop():
	gui.stop()