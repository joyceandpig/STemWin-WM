#ifndef _SAMPLE_H_
#define _SAMPLE_H_
#include "GUI.h"
#include "WM.h"
#include "stm32f10x.h"
#include "string.h"


extern void _DemoSetDesktopColor(void);
extern void _DemoCreateWindow(void);
extern void _DemoCreateWindowAsChild(void);
extern void _DemoInvalidateWindow(void);
extern void _DemoBringToTop(void);
extern void _DemoBringToBottom(void);
extern void _DemoMoveTo(void);
extern void _DemoMoveWindow(void);
extern void _DemoHideAndShowWindowParent(void);
extern void _DemoHideAndShowWindowChild(void);
extern void _DemoClipWindowChild(void);
extern void _DemoResizeWindow(void);
extern void _DemoSetDesktopWindowCallback(void);
extern void _DemoDeleteWindow(void);
#endif