/*******************************************************************************************************************
- File name : sch.c
- Description : ʵ��������ȹ��ܣ�����������ӣ�ɾ�������£�����
- Version : 1.0
********************************************************************************************************************/


#include "sch.h"

sTask g_SCHTask[SCH_MAX_TASKS] = {0};
eError g_ErrorCode = ERROR_NONE;


/***********************************
*@brief         ���������������
*@param pTask   ��ӵĺ���ָ��
*@param Delay   �����ӳ�ִ�е�ʱ��,��λ��ʱ������
*@param Period  ����ִ�е�����,��λ��ʱ������
*@return        ������ӵ���������
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
*@brief         ������ȣ�����RunMe��־λ
*@param void    ��
*@return        ��
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
*@brief         ɾ������
*@param Index   ��������
*@return        ɾ���ɹ�����1
                ɾ��ʧ�ܷ���0
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
*@brief         ����ˢ�£����������жϷ�����򣬰�ʱ����ʱ����
*@param void    ��
*@return        ��
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

