import wx
import libpygen

class MainPanel(wx.Panel):
	def __init__(self, parent, id):
		wx.Panel.__init__(self, parent, id)

		self.radioList = ['x1.0' , 'x0.5', 'x2.0']
		rb = wx.RadioBox(self, 50, " Select simulation speed: ", wx.Point(20, 30), wx.DefaultSize,  self.radioList, 3, wx.RA_SPECIFY_ROWS)
		wx.EVT_RADIOBOX(self, 50, self.EvtSimulationRadioBox)

		#self.button =wx.Button(self, 10, "Apply", wx.Point(50, 150))
		#wx.EVT_BUTTON(self, 10, self.OnClick_Apply)

		self.button =wx.Button(self, 12, "Dump properties", wx.Point(50, 200))
		wx.EVT_BUTTON(self, 12, self.OnClick_Props)

#	def OnClick_Apply(self,event):
#		libpygen.setPropInt("update.speed",2)
	
	def OnClick_Props(self,event):
		libpygen.dumpProps()
	
	def EvtSimulationRadioBox(self, event):
		n = event.GetInt()
		if n == 0:
			print "Update speed to 1.0"
			libpygen.setPropFloat("update.speed",1.0)
		elif n==1:
			print "Update speed to 0.5"
			libpygen.setPropFloat("update.speed",0.5)
		elif n==2:
			print "Update speed to 2.0"
			libpygen.setPropFloat("update.speed",2.0)


def Open():
	app = wx.PySimpleApp()
	frame = wx.Frame(None, -1, " Arena GUI", size = (250,300))
	MainPanel(frame,-1)
	frame.Show(True)
	app.MainLoop()	

