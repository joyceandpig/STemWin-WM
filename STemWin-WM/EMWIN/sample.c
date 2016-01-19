#include "sample.h"

static WM_HWIN _hWin1;
static WM_HWIN _hWin2;
static WM_HWIN _hBut1;
static WM_HWIN _hBut2;
static int     _PaintCount1;
static int     _PaintCount2;

static GUI_COLOR _aColors[] = {
  GUI_BLUE, GUI_YELLOW, GUI_RED, GUI_LIGHTCYAN, GUI_DARKRED, GUI_DARKRED
};

/*********************************************************************
*
*       Static code
*
**********************************************************************
*/
/*********************************************************************
*
*       _cbBkWin
*/
static void _cbBkWin(WM_MESSAGE* pMsg) {
	switch(pMsg->MsgId){
		case WM_PAINT:
			GUI_SetBkColor(GUI_BLACK);
			GUI_Clear();
			GUI_SetColor(0x0060FF);
			GUI_DispStringAt("PaintCount (Early):", 0, 0);
			GUI_DispDecAt(_PaintCount1, 120, 0, 5);
			GUI_SetColor(0x00FFC0);
			GUI_DispStringAt("PaintCount (Late):", 0, 12);
			GUI_DispDecAt(_PaintCount2, 120, 12, 5);
			break;
		case WM_NOTIFY_PARENT:
			if(pMsg->Data.v == WM_NOTIFICATION_RELEASED){//按钮被释放
				if(pMsg->hWinSrc == _hBut1){//button1被释放
					WM_InvalidateWindow(_hWin1);//窗口1失效
					WM_InvalidateWindow(_hWin2);
				}else if (pMsg->hWinSrc == _hBut2){//button2被释放
					_PaintCount1 = 0;
					_PaintCount2 = 0;
					WM_InvalidateWindow(pMsg->hWin);//主窗口失效
				}
			}
			break;
		default:
			WM_DefaultProc(pMsg);
	}
}

/*********************************************************************
*
*       _cbTop
*/
static void _cbTop(WM_MESSAGE* pMsg) {
  switch(pMsg->MsgId) {
  case WM_PAINT:
    GUI_SetBkColor(GUI_MAGENTA);
    GUI_Clear();
    break;
  default:
    WM_DefaultProc(pMsg);
  }
}

/*********************************************************************
*
*       _cbFrameWin1
*/
static void _cbFrameWin1(WM_MESSAGE* pMsg) {
	switch(pMsg->MsgId){
		case WM_PAINT:
			GUI_SetBkColor(_aColors[_PaintCount1 % 6]);
			GUI_Clear();
			GUI_SetColor(0x0060FF);
			GUI_FillCircle(25,25,15);
			GUI_SetColor(GUI_BLACK);
			GUI_DrawCircle(25, 25, 15);
			_PaintCount1++;
			WM_InvalidateWindow(WM_HBKWIN);
			break;
		default:
			WM_DefaultProc(pMsg);
	}
}

/*********************************************************************
*
*       _cbFrameWin2
*/
static void _cbFrameWin2(WM_MESSAGE* pMsg) {
	switch(pMsg->MsgId) {
		case WM_PAINT:
			GUI_SetBkColor(_aColors[_PaintCount2 % 6]);
			GUI_Clear();
			GUI_SetColor(0x00FFC0);
			GUI_FillCircle(25, 25, 15);
			GUI_SetColor(GUI_BLACK);
			GUI_DrawCircle(25, 25, 15);
			_PaintCount2++;
			WM_InvalidateWindow(WM_HBKWIN);
			break;
		default:
			WM_DefaultProc(pMsg);
  }
}
void _Draw_Clip_Window(void)
{
	WM_HWIN hwin0,hwin1,hwin2;
	WM_HWIN hFrame1,hFrame2;
	WM_HWIN hClient1,hClient2;
		
	WM_SetCallback(WM_HBKWIN,_cbBkWin);
	hFrame1 = FRAMEWIN_CreateEx(10,30,140,140,0,WM_CF_SHOW,FRAMEWIN_CF_MOVEABLE,0,"Early Cliping",_cbFrameWin1);
	hFrame2 = FRAMEWIN_CreateEx(170,30,140,140,0,WM_CF_SHOW,FRAMEWIN_CF_MOVEABLE,0,"Late Cliping",_cbFrameWin2);
	
	hClient1 = WM_GetClientWindow(hFrame1);//获取客户端窗口的句柄
	hClient2 = WM_GetClientWindow(hFrame2);
	
	_hWin1 = WM_CreateWindowAsChild(0,0,WM_GetWindowSizeX(hClient1),WM_GetWindowSizeY(hClient1),hClient1,WM_CF_SHOW,_cbFrameWin1,0);
	_hWin2 = WM_CreateWindowAsChild(0,0,WM_GetWindowSizeX(hClient2),WM_GetWindowSizeY(hClient2),hClient2,WM_CF_SHOW,_cbFrameWin2,0);
	
	_hBut1   = BUTTON_CreateEx(10,210,140,20,0,WM_CF_SHOW,0,1);//id:按钮按下时返回的值
	_hBut2   = BUTTON_CreateEx(170, 210, 140, 20, 0, WM_CF_SHOW, 0, 2);
	
	hwin0    = FRAMEWIN_CreateEx( 60,  80, 40, 40, 0, WM_CF_SHOW | WM_CF_STAYONTOP, FRAMEWIN_CF_MOVEABLE, 0, "Top 0", _cbTop);
  hwin1    = FRAMEWIN_CreateEx(220,  80, 40, 40, 0, WM_CF_SHOW | WM_CF_STAYONTOP, FRAMEWIN_CF_MOVEABLE, 0, "Top 1", _cbTop);
  hwin2    = FRAMEWIN_CreateEx(140, 170, 40, 40, 0, WM_CF_SHOW | WM_CF_STAYONTOP, FRAMEWIN_CF_MOVEABLE, 0, "Top 2", _cbTop);
	
  FRAMEWIN_SetResizeable(hwin0, 1);//将窗口设置为可缩放状态，即可以通过拉伸窗口边沿放大窗口,1为可缩放，0为不可缩放
  FRAMEWIN_SetResizeable(hwin1, 1);
  FRAMEWIN_SetResizeable(hwin2, 1);
  BUTTON_SetText(_hBut1, "Invalidate");//在按钮上显示文本
  BUTTON_SetText(_hBut2, "Reset counters");
  while(1) {
    GUI_Delay(50);
  }
}

