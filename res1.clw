; CLW file contains information for the MFC ClassWizard

[General Info]
Version=1
LastClass=AboutBox
LastTemplate=CDialog
NewFileInclude1=#include "stdafx.h"
NewFileInclude2=#include "res1.h"
LastPage=0

ClassCount=7
Class1=AboutBox
Class2=GameWindow

ResourceCount=6
Resource1=IDD_TUTORIAL
Resource2=IDD_THANKS
Class3=CTutorial
Resource3=ABOUTDIALOG
Class4=CSettings
Resource4=IDD_HIGHSCORE
Class5=CHighscore
Resource5=IDD_SETTINGS
Class6=CPassword
Class7=CThanks
Resource6="MAINMENU"

[CLS:AboutBox]
Type=0
BaseClass=CDialog
HeaderFile=AboutBox.h
ImplementationFile=AboutBox.cpp
Filter=D
VirtualFilter=dWC
LastObject=IDC_TEXT1

[CLS:GameWindow]
Type=0
BaseClass=CFrameWnd
HeaderFile=GameWindow.h
ImplementationFile=GameWindow.cpp
LastObject=GameWindow

[DLG:ABOUTDIALOG]
Type=1
Class=AboutBox
ControlCount=4
Control1=IDOK,button,1342242817
Control2=IDC_STATIC,static,1342177294
Control3=IDC_STATIC,static,1342308353
Control4=IDC_URL,static,1342308865

[MNU:"MAINMENU"]
Type=1
Class=?
Command1=ID_GAME_NEW
Command2=ID_GAME_PAUSE
Command3=ID_GAME_EXIT
Command4=ID_OPTIONS_SETTINGS
Command5=ID_HELP_HOWTOPLAY
Command6=ID_HELP_ABOUT
CommandCount=6

[CLS:CTutorial]
Type=0
HeaderFile=Tutorial.h
ImplementationFile=Tutorial.cpp
BaseClass=CDialog
Filter=D
VirtualFilter=dWC

[DLG:IDD_TUTORIAL]
Type=1
Class=CTutorial
ControlCount=2
Control1=IDOK,button,1342242817
Control2=IDC_EDIT1,edit,1352730692

[CLS:CSettings]
Type=0
HeaderFile=Settings.h
ImplementationFile=Settings.cpp
BaseClass=CDialog
Filter=D
LastObject=CSettings
VirtualFilter=dWC

[DLG:IDD_SETTINGS]
Type=1
Class=CSettings
ControlCount=4
Control1=IDOK,button,1342242817
Control2=IDC_RADIO1,button,1342177289
Control3=IDC_RADIO2,button,1342177289
Control4=IDC_RADIO3,button,1342177289

[DLG:IDD_HIGHSCORE]
Type=1
Class=CHighscore
ControlCount=3
Control1=IDOK,button,1342242817
Control2=IDC_HIGHSCORE,static,1342308865
Control3=IDC_SCORE,static,1342308865

[CLS:CHighscore]
Type=0
HeaderFile=Highscore.h
ImplementationFile=Highscore.cpp
BaseClass=CDialog
Filter=D
VirtualFilter=dWC
LastObject=IDC_HIGHSCORE

[CLS:CPassword]
Type=0
HeaderFile=Password.h
ImplementationFile=Password.cpp
BaseClass=CDialog
Filter=D
VirtualFilter=dWC
LastObject=IDC_EDIT1

[DLG:IDD_THANKS]
Type=1
Class=CThanks
ControlCount=3
Control1=IDOK,button,1342242817
Control2=IDC_TEXT,static,1342308865
Control3=IDC_TEXT2,static,1342308865

[CLS:CThanks]
Type=0
HeaderFile=Thanks.h
ImplementationFile=Thanks.cpp
BaseClass=CDialog
Filter=D
VirtualFilter=dWC
LastObject=IDC_TEXT2

