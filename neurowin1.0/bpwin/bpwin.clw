; CLW file contains information for the MFC ClassWizard

[General Info]
Version=1
LastClass=CMainViewPane
LastTemplate=CTreeView
NewFileInclude1=#include "stdafx.h"
NewFileInclude2=#include "bpwin.h"
LastPage=0

ClassCount=21
Class1=CBpwinApp
Class2=CBpwinDoc
Class3=CBpwinView
Class4=CMainFrame

ResourceCount=17
Resource1=IDD_SET_LAYER3
Resource2=IDD_RUN_SETUP
Class5=CAboutDlg
Class6=CTopologyDlg
Resource3=IDD_TRAINING_WARNING
Class7=CInfoDlg
Resource4=IDD_SET_LAYER2
Resource5=IDD_LMS_PAGE
Resource6=IDD_RN_PROGRESS
Class8=CLayer3Page
Class9=CLayer2Page
Class10=CLayer1Page
Resource7=IDD_MD_ZEROINPUT_WARNING
Class11=CLayerWarningDlg
Resource8=IDD_MD_PROGRESS
Resource9=IDD_SET_LAYER1
Class12=CBkpPage
Class13=CLmsPage
Resource10=IDD_ABOUTBOX
Class14=CTrainingWarning
Resource11=IDD_BKP_PAGE
Class15=CBkpProgressDlg
Resource12=IDD_INFODISP
Class16=CMdPage
Resource13=IDD_SET_TOPOLOGY
Resource14=IDD_LAYER_WARNING
Class17=CMdProgressDlg
Class18=CMdZeroInputwarningDlg
Resource15=IDD_BKP_PROGRESS
Class19=CRunDlg
Resource16=IDR_MAINFRAME
Class20=CMainViewPane
Class21=CTreeViewPane
Resource17=IDD_MD_PAGE

[CLS:CBpwinApp]
Type=0
HeaderFile=bpwin.h
ImplementationFile=bpwin.cpp
Filter=N

[CLS:CBpwinDoc]
Type=0
HeaderFile=bpwinDoc.h
ImplementationFile=bpwinDoc.cpp
Filter=N

[CLS:CBpwinView]
Type=0
HeaderFile=bpwinView.h
ImplementationFile=bpwinView.cpp
Filter=C
LastObject=CBpwinView

[CLS:CMainFrame]
Type=0
HeaderFile=MainFrm.h
ImplementationFile=MainFrm.cpp
Filter=T
LastObject=CMainFrame
BaseClass=CFrameWnd
VirtualFilter=fWC



[CLS:CAboutDlg]
Type=0
HeaderFile=bpwin.cpp
ImplementationFile=bpwin.cpp
Filter=D
LastObject=CAboutDlg

[DLG:IDD_ABOUTBOX]
Type=1
Class=CAboutDlg
ControlCount=9
Control1=IDC_STATIC,static,1342177283
Control2=IDC_STATIC,static,1342308480
Control3=IDC_STATIC,static,1342308352
Control4=IDOK,button,1342373889
Control5=IDC_STATIC,static,1342308352
Control6=IDC_PHYSICAL_MEM,static,1342308352
Control7=IDC_DISK_SPACE,static,1342308352
Control8=IDC_STATIC,static,1342308352
Control9=IDC_STATIC,static,1342308352

[MNU:IDR_MAINFRAME]
Type=1
Class=CMainFrame
Command1=ID_APP_EXIT
Command2=ID_NETWORK_TOPOLOGY
Command3=ID_NETWORK_LAYER
Command4=ID_NETWORK_TRAINING
Command5=ID_NETWORK_TRAIN
Command6=ID_NETWORK_RUNSETUP
Command7=ID_NETWORK_RUN
Command8=ID_NETWORK_INFO
Command9=ID_VIEW_TOOLBAR
Command10=ID_VIEW_STATUS_BAR
Command11=ID_HELP_FINDER
Command12=ID_APP_ABOUT
CommandCount=12

[ACL:IDR_MAINFRAME]
Type=1
Class=CMainFrame
Command1=ID_EDIT_COPY
Command2=ID_FILE_NEW
Command3=ID_FILE_OPEN
Command4=ID_FILE_PRINT
Command5=ID_FILE_SAVE
Command6=ID_EDIT_PASTE
Command7=ID_EDIT_UNDO
Command8=ID_EDIT_CUT
Command9=ID_HELP
Command10=ID_CONTEXT_HELP
Command11=ID_NEXT_PANE
Command12=ID_PREV_PANE
Command13=ID_EDIT_COPY
Command14=ID_EDIT_PASTE
Command15=ID_EDIT_CUT
Command16=ID_EDIT_UNDO
CommandCount=16

[TB:IDR_MAINFRAME]
Type=1
Class=CMainFrame
Command1=ID_NETWORK_TOPOLOGY
Command2=ID_NETWORK_LAYER
Command3=ID_NETWORK_TRAINING
Command4=ID_NETWORK_TRAIN
Command5=ID_NETWORK_RUNSETUP
Command6=ID_NETWORK_RUN
Command7=ID_NETWORK_INFO
Command8=ID_APP_ABOUT
Command9=ID_CONTEXT_HELP
CommandCount=9

[DLG:IDD_SET_TOPOLOGY]
Type=1
Class=CTopologyDlg
ControlCount=7
Control1=IDOK,button,1342242817
Control2=IDCANCEL,button,1342242816
Control3=IDC_INPUTDIM,edit,1350639744
Control4=IDC_STATIC,static,1342308352
Control5=IDC_3LAYERS,button,1342177289
Control6=IDC_2LAYERS,button,1342177289
Control7=IDC_STATIC,button,1342177287

[CLS:CTopologyDlg]
Type=0
HeaderFile=TopologyDlg.h
ImplementationFile=TopologyDlg.cpp
BaseClass=CDialog
Filter=D
LastObject=CTopologyDlg
VirtualFilter=dWC

[DLG:IDD_INFODISP]
Type=1
Class=CInfoDlg
ControlCount=10
Control1=IDOK,button,1342242817
Control2=IDCANCEL,button,1342242816
Control3=IDC_VARIABLE_LIST,SysListView32,1350631425
Control4=IDC_STATIC,static,1342177283
Control5=IDC_LIST_LAYER1,SysListView32,1342242817
Control6=IDC_LIST_LAYER2,SysListView32,1342242817
Control7=IDC_LIST_LAYER3,SysListView32,1342242817
Control8=IDC_STATIC,static,1342308352
Control9=IDC_STATIC,static,1342308352
Control10=IDC_STATIC,static,1342308352

[CLS:CInfoDlg]
Type=0
HeaderFile=InfoDlg.h
ImplementationFile=InfoDlg.cpp
BaseClass=CDialog
Filter=D
LastObject=CInfoDlg
VirtualFilter=dWC

[DLG:IDD_SET_LAYER1]
Type=1
Class=CLayer1Page
ControlCount=10
Control1=IDC_LAYER1_NODES,edit,1350639744
Control2=IDC_SPIN_LAYER1_NODE,msctls_updown32,1342177334
Control3=IDC_LAYER1_BIAS,edit,1350631552
Control4=IDC_LAYER1_LINEAR,button,1342177289
Control5=IDC_LAYER1_LOGISTIC,button,1342177289
Control6=IDC_LAYER1_TANH,button,1342177289
Control7=IDC_LAYER1_THRESHOLD,button,1342177289
Control8=IDC_STATIC,button,1342177287
Control9=IDC_STATIC,static,1342308352
Control10=IDC_STATIC,static,1342308352

[DLG:IDD_SET_LAYER3]
Type=1
Class=CLayer3Page
ControlCount=10
Control1=IDC_LAYER3_NODES,edit,1350639744
Control2=IDC_SPIN_LAYER3_NODE,msctls_updown32,1342177334
Control3=IDC_LAYER3_BIAS,edit,1350631552
Control4=IDC_STATIC,button,1342177287
Control5=IDC_LAYER3_LINEAR,button,1342177289
Control6=IDC_LAYER3_LOGISTIC,button,1342177289
Control7=IDC_LAYER3_TANH,button,1342177289
Control8=IDC_STATIC,static,1342308352
Control9=IDC_STATIC,static,1342308352
Control10=IDC_LAYER3_THRESHOLD,button,1342177289

[DLG:IDD_SET_LAYER2]
Type=1
Class=CLayer2Page
ControlCount=10
Control1=IDC_LAYER2_NODES,edit,1350639744
Control2=IDC_SPIN_LAYER2_NODE,msctls_updown32,1342177334
Control3=IDC_LAYER2_BIAS,edit,1350631552
Control4=IDC_STATIC,button,1342177287
Control5=IDC_LAYER2_LINEAR,button,1342177289
Control6=IDC_LAYER2_LOGISTIC,button,1342177289
Control7=IDC_LAYER2_TANH,button,1342177289
Control8=IDC_STATIC,static,1342308352
Control9=IDC_STATIC,static,1342308352
Control10=IDC_LAYER2_THRESHOLD,button,1342177289

[CLS:CLayer3Page]
Type=0
HeaderFile=LayerPages.h
ImplementationFile=LayerPages.cpp
BaseClass=CPropertyPage
Filter=D
LastObject=IDC_LAYER3_THRESHOLD
VirtualFilter=idWC

[CLS:CLayer2Page]
Type=0
HeaderFile=LayerPages.h
ImplementationFile=LayerPages.cpp
BaseClass=CPropertyPage
Filter=D
VirtualFilter=idWC
LastObject=IDC_LAYER2_THRESHOLD

[CLS:CLayer1Page]
Type=0
HeaderFile=LayerPages.h
ImplementationFile=LayerPages.cpp
BaseClass=CPropertyPage
Filter=D
VirtualFilter=idWC
LastObject=CLayer1Page

[DLG:IDD_LAYER_WARNING]
Type=1
Class=CLayerWarningDlg
ControlCount=9
Control1=IDOK,button,1342242817
Control2=IDCANCEL,button,1342242816
Control3=IDC_STATIC,static,1342308352
Control4=IDC_STATIC,static,1342308352
Control5=IDC_STATIC,static,1342308352
Control6=IDC_STATIC,static,1342308352
Control7=IDC_STATIC,static,1342308352
Control8=IDC_STATIC,static,1342308352
Control9=IDC_STATIC,static,1342177283

[CLS:CLayerWarningDlg]
Type=0
HeaderFile=WarningDlg.h
ImplementationFile=WarningDlg.cpp
BaseClass=CDialog
Filter=D
LastObject=IDCANCEL

[CLS:CBkpPage]
Type=0
HeaderFile=TrainPages.h
ImplementationFile=TrainPages.cpp
BaseClass=CPropertyPage
Filter=D
LastObject=IDC_UPPER_WEIGHT
VirtualFilter=idWC

[DLG:IDD_BKP_PAGE]
Type=1
Class=CBkpPage
ControlCount=14
Control1=IDC_DATA_FILE_EDIT,edit,1350631552
Control2=IDC_INPUT_FILE_BTN,button,1342242816
Control3=IDC_OUTPUT_FILE_EDIT,edit,1350631552
Control4=IDC_OUTPUT_FILE_BTN,button,1342242816
Control5=IDC_NUM_VECTORS,edit,1350639744
Control6=IDC_TRAIN_PARAM,edit,1350631552
Control7=IDC_TRAIN_PASSES,edit,1350639744
Control8=IDC_UPPER_WEIGHT,edit,1350631552
Control9=IDC_LOWER_WEIGHT,edit,1350631552
Control10=IDC_STATIC,static,1342308352
Control11=IDC_STATIC,static,1342308352
Control12=IDC_STATIC,static,1342308352
Control13=IDC_STATIC,static,1342308352
Control14=IDC_STATIC,static,1342308352

[DLG:IDD_LMS_PAGE]
Type=1
Class=CLmsPage
ControlCount=2
Control1=IDC_STATIC,static,1342308352
Control2=IDC_LMS_TRAIN_PARAM,edit,1350631552

[CLS:CLmsPage]
Type=0
HeaderFile=trainpages.h
ImplementationFile=trainpages.cpp
BaseClass=CPropertyPage
Filter=D
LastObject=CLmsPage
VirtualFilter=idWC

[DLG:IDD_TRAINING_WARNING]
Type=1
Class=CTrainingWarning
ControlCount=4
Control1=IDOK,button,1342242817
Control2=IDCANCEL,button,1342242816
Control3=IDC_STATIC,static,1342308352
Control4=IDC_STATIC,static,1342177283

[CLS:CTrainingWarning]
Type=0
HeaderFile=warningdlg.h
ImplementationFile=warningdlg.cpp
BaseClass=CDialog
Filter=D
LastObject=CTrainingWarning

[DLG:IDD_BKP_PROGRESS]
Type=1
Class=CBkpProgressDlg
ControlCount=4
Control1=IDCANCEL,button,1342242816
Control2=IDC_TRAINING_PROGRESS,msctls_progress32,1350565888
Control3=IDC_STATIC,static,1342177283
Control4=IDC_START_TRAINING,button,1342242816

[CLS:CBkpProgressDlg]
Type=0
HeaderFile=ProgressDlg.h
ImplementationFile=ProgressDlg.cpp
BaseClass=CDialog
Filter=D
VirtualFilter=dWC
LastObject=IDCANCEL

[DLG:IDD_MD_PAGE]
Type=1
Class=CMdPage
ControlCount=14
Control1=IDC_DATA_FILE_EDIT,edit,1350631552
Control2=IDC_INPUT_FILE_BTN,button,1342242816
Control3=IDC_OUTPUT_FILE_EDIT,edit,1350631552
Control4=IDC_OUTPUT_FILE_BTN,button,1342242816
Control5=IDC_NUM_VECTORS,edit,1350639744
Control6=IDC_TRAIN_PARAM,edit,1350631552
Control7=IDC_TRAIN_PASSES,edit,1350639744
Control8=IDC_UPPER_WEIGHT,edit,1350631552
Control9=IDC_LOWER_WEIGHT,edit,1350631552
Control10=IDC_STATIC,static,1342308352
Control11=IDC_STATIC,static,1342308352
Control12=IDC_STATIC,static,1342308352
Control13=IDC_STATIC,static,1342308352
Control14=IDC_STATIC,static,1342308352

[CLS:CMdPage]
Type=0
HeaderFile=trainpages.h
ImplementationFile=trainpages.cpp
BaseClass=CPropertyPage
Filter=D
LastObject=CMdPage
VirtualFilter=idWC

[DLG:IDD_MD_PROGRESS]
Type=1
Class=CMdProgressDlg
ControlCount=4
Control1=IDCANCEL,button,1342242816
Control2=IDC_TRAINING_PROGRESS,msctls_progress32,1350565888
Control3=IDC_STATIC,static,1342177283
Control4=IDC_START_TRAINING,button,1342242816

[CLS:CMdProgressDlg]
Type=0
HeaderFile=progressdlg.h
ImplementationFile=progressdlg.cpp
BaseClass=CDialog
Filter=D
VirtualFilter=dWC
LastObject=CMdProgressDlg

[DLG:IDD_MD_ZEROINPUT_WARNING]
Type=1
Class=CMdZeroInputwarningDlg
ControlCount=5
Control1=IDOK,button,1342242817
Control2=IDCANCEL,button,1342242816
Control3=IDC_STATIC,static,1342177283
Control4=IDC_STATIC,static,1342308352
Control5=IDC_STATIC,static,1342308352

[CLS:CMdZeroInputwarningDlg]
Type=0
HeaderFile=warningdlg.h
ImplementationFile=warningdlg.cpp
BaseClass=CDialog
Filter=D
VirtualFilter=dWC
LastObject=CMdZeroInputwarningDlg

[DLG:IDD_RUN_SETUP]
Type=1
Class=CRunDlg
ControlCount=10
Control1=IDOK,button,1342242817
Control2=IDCANCEL,button,1342242816
Control3=IDC_OUTPUT_FILE_BTN,button,1342242816
Control4=IDC_DATA_FILE_EDIT,edit,1350631552
Control5=IDC_INPUT_FILE_BTN,button,1342242816
Control6=IDC_OUTPUT_FILE_EDIT,edit,1350631552
Control7=IDC_WEIGHTS_FILE_BTN,button,1342242816
Control8=IDC_WEIGHTS_FILE_EDIT,edit,1350631552
Control9=IDC_NUM_VECTORS,edit,1350639744
Control10=IDC_STATIC,static,1342308352

[CLS:CRunDlg]
Type=0
HeaderFile=trainpages.h
ImplementationFile=trainpages.cpp
BaseClass=CDialog
Filter=D
LastObject=CRunDlg
VirtualFilter=dWC

[DLG:IDD_RN_PROGRESS]
Type=1
Class=?
ControlCount=4
Control1=IDCANCEL,button,1342242816
Control2=IDC_RUNNING_PROGRESS,msctls_progress32,1350565888
Control3=IDC_STATIC,static,1342177283
Control4=IDC_START_TRAINING,button,1342242816

[CLS:CMainViewPane]
Type=0
HeaderFile=MainViewPane.h
ImplementationFile=MainViewPane.cpp
BaseClass=CEditView
Filter=C
LastObject=CMainViewPane
VirtualFilter=VWC

[CLS:CTreeViewPane]
Type=0
HeaderFile=TreeViewPane.h
ImplementationFile=TreeViewPane.cpp
BaseClass=CTreeView
Filter=C
LastObject=CTreeViewPane

