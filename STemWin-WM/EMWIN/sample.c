#include "sample.h"

#define MSG_CHANGE_TEXT WM_USER
#define SPEED  1400

static char _acInfoText[40];

static GUI_COLOR _WindowColor1 = GUI_GREEN;
static GUI_COLOR _FrameColor1  = GUI_BLUE;
static GUI_COLOR _WindowColor2 = GUI_RED;
static GUI_COLOR _FrameColor2  = GUI_YELLOW;
static GUI_COLOR _ChildColor   = GUI_YELLOW;
static GUI_COLOR _ChildFrame   = GUI_BLACK;

static WM_CALLBACK *_cbBKWindowOld;
static WM_HWIN _hwin1,_hwin2,_hwinchild;

static void _cbwindow1(WM_MESSAGE *pMsg)//窗口1回调函数
{
	GUI_RECT Rect;
	int x,y;
	switch(pMsg->MsgId)
	{
		case WM_PAINT:
			WM_GetInsideRect(&Rect);
			GUI_SetBkColor(_WindowColor1);
			GUI_SetColor(_FrameColor1);
			GUI_ClearRectEx(&Rect);
			GUI_DrawRectEx(&Rect);
			GUI_SetFont(&GUI_Font16_ASCII);
			x = WM_GetWindowSizeX(pMsg->hWin);
			y = WM_GetWindowSizeY(pMsg->hWin);
			GUI_DispStringHCenterAt("Window1",x / 2, (y / 2) - 12);
			break;
		default:
			WM_DefaultProc(pMsg);
	}
}
static void _cbwindow2(WM_MESSAGE *pMsg)//窗口2回调函数
{
	GUI_RECT Rect;
	int x,y;
	switch(pMsg->MsgId)
	{
		case WM_PAINT:
			WM_GetInsideRect(&Rect);
			GUI_SetBkColor(_WindowColor2);
			GUI_SetColor(_FrameColor2);
			GUI_ClearRectEx(&Rect);
			GUI_DrawRectEx(&Rect);
			GUI_SetFont(&GUI_Font16_ASCII);
			x = WM_GetWindowSizeX(pMsg->hWin);
			y = WM_GetWindowSizeY(pMsg->hWin);
			GUI_DispStringHCenterAt("Window2",x / 2, (y / 2) - 12);
			break;
		default:
			WM_DefaultProc(pMsg);
	}
}
static void _cbBKwindow(WM_MESSAGE *pMsg)
{
	int x,y;
	switch(pMsg->MsgId)
	{
		case MSG_CHANGE_TEXT:
			strcpy(_acInfoText,(const char *)pMsg->Data.p);
		case WM_PAINT:
			GUI_SetBkColor(GUI_BLACK);
			GUI_Clear();
			GUI_SetColor(GUI_WHITE);
			GUI_SetFont(&GUI_Font16_ASCII);
			GUI_DispStringHCenterAt("Window-sample",120,0);
			GUI_DispStringAtCEOL(_acInfoText,10,20);
			break;
		default:
			WM_DefaultProc(pMsg);
	}
}
static void _cbchildwindow(WM_MESSAGE *pMsg)
{
	GUI_RECT Rect;
	int x,y;
	switch(pMsg->MsgId)
	{
		case WM_PAINT:
			WM_GetInsideRect(&Rect);
			GUI_SetBkColor(_ChildColor);
			GUI_SetColor(_ChildFrame);
			GUI_ClearRectEx(&Rect);
			GUI_SetFont(&GUI_Font16_ASCII);
			GUI_DrawRectEx(&Rect);
			GUI_SetColor(GUI_RED);
			x = WM_GetWindowSizeX(pMsg->hWin);
			y = WM_GetWindowSizeY(pMsg->hWin);
			GUI_DispStringHCenterAt("child window",x/2,y/2-12);
			break;
		default:
			WM_DefaultProc(pMsg);
	}
}
static void _ChangeInfoText( const char *pstr)
{
	WM_MESSAGE Text;//特别注意，这里定义时不能使用指针变量，否则将进入硬中断，原因未知?
	Text.MsgId = MSG_CHANGE_TEXT;
	Text.Data.p = pstr;
	WM_SendMessage(WM_HBKWIN,&Text);
	WM_InvalidateWindow(WM_HBKWIN);
}
static void _DrawInfoText( const char *pstr )
{
	GUI_SetColor(GUI_WHITE);
	GUI_SetFont(&GUI_Font16_ASCII);
	GUI_DispStringHCenterAt("window-sample",120,10);
	GUI_DispStringAtCEOL(pstr,10,20);
}	
void _DemoSetDesktopColor(void)
{
	GUI_SetColor(GUI_WHITE);
	GUI_SetBkColor(GUI_BLUE);
	GUI_Clear();
	
	_DrawInfoText("WM_SetDesktopColor");//在桌面窗口背景中写文字
	GUI_Delay(SPEED);
	WM_SetDesktopColor(GUI_BLACK);//设定桌面窗口的颜色，将重新着色
	GUI_Delay(SPEED);
	
	GUI_SetBkColor(GUI_BLACK);
	WM_SetDesktopColor(GUI_INVALID_COLOR);//恢复桌面的默认颜色配置
}
void _DemoCreateWindow(void)
{
	_cbBKWindowOld = WM_SetCallback(WM_HBKWIN,_cbBKwindow);//为桌面窗口设定回调函数，并将原回调返回
	_ChangeInfoText("Create Window");
	GUI_Delay(SPEED);
	
	_hwin1 = WM_CreateWindow(50,70,165,100,WM_CF_SHOW | WM_CF_MEMDEV,_cbwindow1,0);
	_hwin2 = WM_CreateWindow(105,125,165,100,WM_CF_SHOW | WM_CF_MEMDEV,_cbwindow2,0);
	GUI_Delay(SPEED);
}
void _DemoCreateWindowAsChild(void)
{
	_ChangeInfoText("Create WindowAsChild");
	GUI_Delay(SPEED);
	_hwinchild = WM_CreateWindowAsChild(10,50,145,40,_hwin2,WM_CF_SHOW|WM_CF_MEMDEV,_cbchildwindow,0);
	GUI_Delay(SPEED);
}
void _DemoInvalidateWindow(void)
{
	_ChangeInfoText("InvalidateWindow");
	_WindowColor1 = GUI_BLUE;
	_FrameColor1 = GUI_GREEN;
	GUI_Delay(SPEED);
	WM_InvalidateWindow(_hwin1);
	WM_InvalidateWindow(_hwin2);
	GUI_Delay(SPEED);
}
void _DemoBringToTop(void)
{
	_ChangeInfoText("BringWindowToTop");
	GUI_Delay(SPEED);
	WM_BringToTop(_hwin1);
	GUI_Delay(SPEED);
}
void _DemoBringToBottom(void)
{
	_ChangeInfoText("BringWindowToBottom");
	GUI_Delay(SPEED);
	WM_BringToBottom(_hwin1);
	GUI_Delay(SPEED);
}
void _DemoMoveTo(void)
{
	int i,tm,tDiff;
	_ChangeInfoText("MoveWindowToPos");
	GUI_Delay(SPEED);
	
	for(i = 0; i < 56; i++){
		tm = GUI_GetTime();
		WM_MoveTo(_hwin1,50+i,70+i);//移动窗口到指定位置
		WM_MoveTo(_hwin2,105-i,125-i);
		tDiff = 15-(GUI_GetTime()-tm);
		GUI_Delay(tDiff);
	}
		for(i = 0; i < 56; i++){
		tm = GUI_GetTime();
		WM_MoveTo(_hwin2,50+i,70+i);
		WM_MoveTo(_hwin1,105-i,125-i);
		tDiff = 15-(GUI_GetTime()-tm);
		GUI_Delay(tDiff);
	}
}
void _DemoMoveWindow(void)
{
	int i,tm,tDiff;
	_ChangeInfoText("MoveWindowStepByStep");
	GUI_Delay(SPEED);
	
	for(i = 0; i < 56; i++){
		tm = GUI_GetTime();
		WM_MoveWindow(_hwin1,1,1);//移动窗口到指定位置
		WM_MoveWindow(_hwin2,-1,-1);
		tDiff = 15-(GUI_GetTime()-tm);
		GUI_Delay(tDiff);
	}
		for(i = 0; i < 56; i++){
		tm = GUI_GetTime();
		WM_MoveWindow(_hwin1,-1,-1);
		WM_MoveWindow(_hwin2,1,1);
		tDiff = 15-(GUI_GetTime()-tm);
		GUI_Delay(tDiff);
	}
}
void _DemoHideAndShowWindowParent(void)
{
	_ChangeInfoText("HideWindow(Parent)");
	GUI_Delay(SPEED);
	
	WM_HideWindow(_hwin1);
	GUI_Delay(SPEED);
	WM_HideWindow(_hwin2);
	GUI_Delay(SPEED);
	_ChangeInfoText("ShowWindow(Parent)");
	WM_ShowWindow(_hwin1);
	GUI_Delay(SPEED);
	WM_ShowWindow(_hwin2);
	GUI_Delay(SPEED);
}
void _DemoHideAndShowWindowChild(void)
{
	_ChangeInfoText("HideWindow(Child)");
	GUI_Delay(SPEED);
	WM_HideWin(_hwinchild);
	GUI_Delay(SPEED);
	_ChangeInfoText("ShowWindow(Child)");
	GUI_Delay(SPEED);
	WM_ShowWin(_hwinchild);
	GUI_Delay(SPEED);
}
void _DemoClipWindowChild(void)
{
	int i,tm,tDiff;
	_ChangeInfoText("ClipWindow(Child)");
	GUI_Delay(SPEED);
	
	for(i = 0; i < 125; i++){
		tm = GUI_GetTime();
		if(i < 25){
			WM_MoveWindow(_hwinchild,1,0);
		}else if(i < 50){
			WM_MoveWindow(_hwinchild,0,1);
		}else if(i < 75){
			WM_MoveWindow(_hwinchild,-1,0);
		}else if(i < 100){
			WM_MoveWindow(_hwinchild,0,-1);
		}else{
			WM_MoveWindow(_hwinchild,1,0);
		}
		tDiff = 15-(GUI_GetTime()-tm);
		GUI_Delay(tDiff);
	}
	GUI_Delay(SPEED);
}
void _DemoResizeWindow(void)
{
	int i,tm,tDiff;
	_ChangeInfoText("Resize Window");
	GUI_Delay(SPEED);
	
		for(i = 0; i < 125; i++){
		tm = GUI_GetTime();
		if(i < 25){
			WM_ResizeWindow(_hwin1,1,1);
			WM_ResizeWindow(_hwin2,-1,-1);
		}else if(i < 80){
			WM_ResizeWindow(_hwin1,-1,-1);
			WM_ResizeWindow(_hwin2,1,1);
		}else if(i < 100){
			WM_ResizeWindow(_hwin1,1,1);
			WM_ResizeWindow(_hwin2,-1,-1);
		}
		tDiff = 15-(GUI_GetTime()-tm);
		GUI_Delay(tDiff);
	}
	GUI_Delay(SPEED);
}
void _DemoSetDesktopWindowCallback(void)
{
	_ChangeInfoText("Use Callback");
	GUI_Delay(SPEED);
	
	WM_SetCallback(_hwin2,_cbwindow2);
	WM_InvalidateWindow(_hwin2);
	GUI_Delay(SPEED);
	
	WM_SetCallback(_hwin1,_cbwindow1);
	WM_InvalidateWindow(_hwin1);
	GUI_Delay(SPEED);
	
	_ChangeInfoText("Not Use Callback");
	GUI_Delay(SPEED);
	
	WM_SetCallback(_hwin2,_cbwindow2);
	WM_InvalidateWindow(_hwin2);
	GUI_Delay(SPEED);
	
	WM_SetCallback(_hwin1,_cbwindow1);
	WM_InvalidateWindow(_hwin1);
	GUI_Delay(SPEED);
}
void _DemoDeleteWindow(void)
{
	_ChangeInfoText("Delete Window");
	GUI_Delay(SPEED);
	
	WM_DeleteWindow(_hwin1);
	WM_DeleteWindow(_hwin2);
	GUI_Delay(SPEED);
	_ChangeInfoText(" ");
	GUI_Delay(SPEED);
	
	WM_SetCallback(WM_HBKWIN,_cbBKWindowOld);
	_WindowColor1 = GUI_GREEN;
	_WindowColor2 = GUI_RED;
	
}
