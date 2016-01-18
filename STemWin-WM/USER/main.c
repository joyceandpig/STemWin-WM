#include "stm32f10x.h"
#include "led.h"
#include "delay.h"
#include "sys.h"
#include "usart.h"
#include "ILI93xx.h"
#include "key.h"
#include "malloc.h" 
#include "usmart.h" 
#include "GUI.h"
#include "touch.h"
#include "includes.h"
#include "math.h"
#include "bitmap.h"
#include "WM.h"

//ALIENTEK Mini STM32开发板范例代码27
//内存管理实验  
//技术支持：www.openedv.com
//广州市星翼电子科技有限公司 
extern void _MY_GetTouchPos(void);


/////////////////////////UCOSII任务设置///////////////////////////////////
//START 任务
#define START_TASK_PRIO      		20        //开始任务的优先级设置为最低
#define START_STK_SIZE  				64        //设置任务堆栈大小
OS_STK START_TASK_STK[START_STK_SIZE];    //任务堆栈	
void start_task(void *pdata);	            //任务函数

#define LED_TASK_PRIO      			9        //开始任务的优先级设置为最低
#define LED_STK_SIZE  					64        //设置任务堆栈大小
OS_STK LED_TASK_STK[LED_STK_SIZE];    //任务堆栈	
void led_task(void *pdata);	            //任务函数

#define EMWIN_DEMO_TASK_PRIO    8        //开始任务的优先级设置为最低
#define EMWIN_STK_SIZE  				3096        //设置任务堆栈大小
OS_STK EMWIN_TASK_STK[EMWIN_STK_SIZE];    //任务堆栈	
void emwin_demo_task(void *pdata);	            //任务函数

#define TOUCH_TASK_PRIO      		10        //开始任务的优先级设置为最低
#define TOUCH_STK_SIZE  				64        //设置任务堆栈大小
OS_STK TOUCH_TASK_STK[TOUCH_STK_SIZE];    //任务堆栈	
void touch_task(void *pdata);	            //任务函数

void BSP_Init(void)
{
	NVIC_Configuration();	 
	delay_init();	    			 //延时函数初始化	  
	uart_init(115200);	 		//串口初始化为9600
	LED_Init();		  				//初始化与LED连接的硬件接口
	TFTLCD_Init();			   	//初始化LCD		 
	tp_dev.init();
//	tp_dev.adjust();
	mem_init();				//初始化内存池

	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_CRC,ENABLE);
  GUI_Init();
	
}
void main_ui(void)
{
#if 0
	 _MY_GetTouchPos();
#endif
	GUI_SetBkColor(GUI_BLACK);
	GUI_SetColor(GUI_WHITE);
	GUI_DispStringAt("Hello World!", 30, 200);
	GUI_DispStringAt("Hello emWin!", 30, 216);
	GUI_DrawRoundedRect(0,0,200,200,5);
	GUI_DrawRoundedFrame(2,2,180,20,5,2);
}
static void _cbwindow(WM_MESSAGE *pMsg)//窗口回调函数
{
	GUI_RECT Rect;
	static u16 i = 0;

	
	switch(pMsg->MsgId){
		case WM_PAINT:
				i++;
			WM_GetInsideRect(&Rect);
			GUI_SetColor(GUI_YELLOW);
			GUI_SetBkColor(GUI_RED);
		  GUI_ClearRectEx(&Rect);
			GUI_DrawRectEx(&Rect);
			GUI_SetColor(GUI_BLACK);
			GUI_SetFont(&GUI_Font8x16);
//			GUI_DispStringHCenterAt("Foreground window", 120, 0);
		GUI_DispDec(i,3);
		break;
		default:
			WM_DefaultProc(pMsg);
	}
}
static void _cbhkwindow(WM_MESSAGE *pMsg)
{
	switch(pMsg->MsgId){
		case WM_PAINT:
		  GUI_Clear();
		default:
			WM_DefaultProc(pMsg);
	}
}
void _Draw_WM(void)
{
	u8 i;
	WM_HWIN hwin;
	
	WM_SetCreateFlags(WM_CF_MEMDEV);
	WM_EnableMemdev(WM_HBKWIN); 
	
	hwin = WM_CreateWindow(10,10,100,60,WM_CF_SHOW,_cbwindow,0);//创建一个窗口，并指定回调函数
	WM_SetCallback(WM_HBKWIN,_cbhkwindow);//设定背景窗口的回调函数，以便于移动窗口时清除背景
	GUI_Delay(100);
	
	for(i = 0;i<40;i++){
		WM_MoveWindow(hwin,2,4);
		GUI_Delay(2000);
	}
	WM_DeleteWindow(hwin);//删除窗口
	WM_InvalidateWindow(WM_HBKWIN);//无效化指定窗口
	GUI_Exec();//执行重绘
}
int main(void)
{
	BSP_Init();
	


	OSInit();
	OSTaskCreate(start_task,(void *)0,(OS_STK *)&START_TASK_STK[START_STK_SIZE-1],START_TASK_PRIO);//创建起始任务
	OSStart();
}
void start_task(void *pdata)
{
	OS_CPU_SR cpu_sr = 0;

//	GUI_Delay(1000);
	OS_ENTER_CRITICAL();
	OSTaskCreate(emwin_demo_task,(void *)0,&EMWIN_TASK_STK[EMWIN_STK_SIZE-1],EMWIN_DEMO_TASK_PRIO);
	OSTaskCreate(touch_task,(void *)0,&TOUCH_TASK_STK[TOUCH_STK_SIZE-1],TOUCH_TASK_PRIO);
	OSTaskCreate(led_task,(void *)0,(OS_STK*)&LED_TASK_STK[LED_STK_SIZE-1],LED_TASK_PRIO);
	OSTaskSuspend(START_TASK_PRIO);	//挂起起始任务.
	OS_EXIT_CRITICAL();
}
void led_task(void *pdata)
{
	while(1)
	{
		LED0 = !LED0;
		OSTimeDlyHMSM(0,0,0,500);
	}
}
void touch_task(void *pdata)
{
	while(1)
	{
		GUI_TOUCH_Exec();
		OSTimeDlyHMSM(0,0,0,10);
	}
}
void emwin_demo_task(void *pdata)
{
	while(1)
	{
//		GUIDEMO_Main();
		_Draw_WM();
		OSTimeDlyHMSM(0,0,0,1000);
	}
}
