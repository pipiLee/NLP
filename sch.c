/*******************************************************************************************************************
- File name : sch.c
- Description : 实现任务调度功能，包括任务添加，删除，更新，分配
- Version : 1.0
********************************************************************************************************************/


#include "sch.h"

sTask g_SCHTask[SCH_MAX_TASKS] = {0};
eError g_ErrorCode = ERROR_NONE;


/***********************************
*@brief         添加任务到任务数组
*@param pTask   添加的函数指针
*@param Delay   函数延迟执行的时间,单位由时基决定
*@param Period  函数执行的周期,单位由时基决定
*@return        返回添加的任务索引
***********************************/
uint8_t SCH_Add_Task(void (*pTask)(void), uint16_t Delay, uint16_t Period)
{
    uint8_t Index = 0;

    while((g_SCHTask[Index].pTask != 0) && (Index < SCH_MAX_TASKS))
    {
        Index++;
    }

    if(Index == SCH_MAX_TASKS)
    {
        g_ErrorCode = ERROR_SCH_TOO_MANY_TASKS;
        return SCH_MAX_TASKS;
    }

    g_SCHTask[Index].pTask = pTask;
    g_SCHTask[Index].Delay = Delay;
    g_SCHTask[Index].Period = Period;
    g_SCHTask[Index].RunMe = 0;

    return Index;
}


/*******************************************
*@brief         任务调度，根据RunMe标志位
*@param void    空
*@return        空
********************************************/
void SCH_Dispatch_Task(void)
{
    uint8_t Index = 0;

    for(Index = 0; Index < SCH_MAX_TASKS; Index++)
    {
        if(g_SCHTask[Index].RunMe)
        {
            g_SCHTask[Index].pTask();
            g_SCHTask[Index].RunMe = 0;

            if(g_SCHTask[Index].Period == 0)
            {
                SCH_Delete_Task(Index);
            }
        }
    }
}


/****************************************
*@brief         删除任务
*@param Index   任务索引
*@return        删除成功返回1
                删除失败返回0
*****************************************/
_Bool SCH_Delete_Task(uint8_t Index)
{
    _Bool ReturnCode = 0;

    if(g_SCHTask[Index].pTask == 0)
    {
        g_ErrorCode = ERROR_SCH_CANNOT_DELETE_TASK;
        ReturnCode = 0;
    }
    else
    {
        ReturnCode = 1;
    }

    g_SCHTask[Index].pTask = 0x0000;
    g_SCHTask[Index].Delay = 0;
    g_SCHTask[Index].Period = 0;
    g_SCHTask[Index].RunMe = 0;

    return ReturnCode;
}


/********************************************************************
*@brief         任务刷新，调度器的中断服务程序，按时基定时调用
*@param void    空
*@return        空
*********************************************************************/
void SCH_Update(void)
{
    uint8_t Index = 0;

    for(Index = 0; Index < SCH_MAX_TASKS; Index++)
    {
        if(g_SCHTask[Index].pTask)
        {
            if(g_SCHTask[Index].Delay == 0)
            {
                g_SCHTask[Index].RunMe = 1;
                g_SCHTask[Index].Delay = g_SCHTask[Index].Period;
            }
            else
            {
                g_SCHTask[Index].Delay--;
            }
        }
    }
	
	return;
}

