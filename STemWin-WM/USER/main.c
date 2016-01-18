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

//ALIENTEK Mini STM32�����巶������27
//�ڴ����ʵ��  
//����֧�֣�www.openedv.com
//������������ӿƼ����޹�˾ 
extern void _MY_GetTouchPos(void);


/////////////////////////UCOSII��������///////////////////////////////////
//START ����
#define START_TASK_PRIO      		20        //��ʼ��������ȼ�����Ϊ���
#define START_STK_SIZE  				64        //���������ջ��С
OS_STK START_TASK_STK[START_STK_SIZE];    //�����ջ	
void start_task(void *pdata);	            //������

#define LED_TASK_PRIO      			9        //��ʼ��������ȼ�����Ϊ���
#define LED_STK_SIZE  					64        //���������ջ��С
OS_STK LED_TASK_STK[LED_STK_SIZE];    //�����ջ	
void led_task(void *pdata);	            //������

#define EMWIN_DEMO_TASK_PRIO    8        //��ʼ��������ȼ�����Ϊ���
#define EMWIN_STK_SIZE  				3096        //���������ջ��С
OS_STK EMWIN_TASK_STK[EMWIN_STK_SIZE];    //�����ջ	
void emwin_demo_task(void *pdata);	            //������

#define TOUCH_TASK_PRIO      		10        //��ʼ��������ȼ�����Ϊ���
#define TOUCH_STK_SIZE  				64        //���������ջ��С
OS_STK TOUCH_TASK_STK[TOUCH_STK_SIZE];    //�����ջ	
void touch_task(void *pdata);	            //������

void BSP_Init(void)
{
	NVIC_Configuration();	 
	delay_init();	    			 //��ʱ������ʼ��	  
	uart_init(115200);	 		//���ڳ�ʼ��Ϊ9600
	LED_Init();		  				//��ʼ����LED���ӵ�Ӳ���ӿ�
	TFTLCD_Init();			   	//��ʼ��LCD		 
	tp_dev.init();
//	tp_dev.adjust();
	mem_init();				//��ʼ���ڴ��

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
static void _cbwindow(WM_MESSAGE *pMsg)//���ڻص�����
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
	
	hwin = WM_CreateWindow(10,10,100,60,WM_CF_SHOW,_cbwindow,0);//����һ�����ڣ���ָ���ص�����
	WM_SetCallback(WM_HBKWIN,_cbhkwindow);//�趨�������ڵĻص��������Ա����ƶ�����ʱ�������
	GUI_Delay(100);
	
	for(i = 0;i<40;i++){
		WM_MoveWindow(hwin,2,4);
		GUI_Delay(2000);
	}
	WM_DeleteWindow(hwin);//ɾ������
	WM_InvalidateWindow(WM_HBKWIN);//��Ч��ָ������
	GUI_Exec();//ִ���ػ�
}
int main(void)
{
	BSP_Init();
	


	OSInit();
	OSTaskCreate(start_task,(void *)0,(OS_STK *)&START_TASK_STK[START_STK_SIZE-1],START_TASK_PRIO);//������ʼ����
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
	OSTaskSuspend(START_TASK_PRIO);	//������ʼ����.
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
