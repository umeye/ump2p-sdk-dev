/*----------------------------------------------------------------------------
 * Copyright(c)  :  NPC CORPORTAION All Rights Reserved                       
 * FileName      :  NPC_SYS_Thread_DataDef.h
 * Version       :  1.1
 * Author        :  CCH
 * DateTime      :  2012-01-20
 * Description   :  NPC系统线程处理模块数据定义
 *----------------------------------------------------------------------------*/

#ifndef __NPC_SYS_THREAD_DATADEF_H
#define __NPC_SYS_THREAD_DATADEF_H

#include "NPC_TypeDef.h"

#ifdef _WIN32
#define NPC_D_THREAD_MAX_WAIT_OBJ_NUM					64			//最大等待对象数
#else
#define NPC_D_THREAD_MAX_WAIT_OBJ_NUM					64			//最大等待对象数
#endif


//线程数据定义
typedef struct _NPC_S_THREAD_DATA
{
	//NPC_BOOL						bRunFlag;						//线程运行标志

#ifdef _WIN32
	NPC_DWORD						dwThreadId;						//线程ID
	HANDLE							hThread;						//线程句柄
#else
	pthread_t						t_thread_data;					//线程数据
	NPC_BOOL						bIfCreated;						//是否被创建
	pthread_mutex_t					t_mutex_lock;					//互斥锁
	NPC_BOOL						bIfInitMutex;					//是否初始化了互斥锁
	pthread_cond_t					cond_exit_notify;				//退出通知条件变量
	NPC_BOOL						bIfInitCond;					//是否初始化了条件变量
#endif

	fNPC_F_THREAD_StartProc			pThreadProc;					//线程处理过程
	PNPC_VOID						pThreadParam;					//线程参数

	NPC_INT							iEndFlag;						//结束标志

} NPC_S_THREAD_DATA, *PNPC_S_THREAD_DATA;

//互斥锁数据定义
typedef struct _NPC_S_THREAD_MUTEX_DATA
{
#ifdef _WIN32
	CRITICAL_SECTION				crsMutex;						//互斥量
#else
	pthread_mutex_t					mutex_lock;						//互斥量
#endif

} NPC_S_THREAD_MUTEX_DATA, *PNPC_S_THREAD_MUTEX_DATA;

//事件数据定义
typedef struct _NPC_S_THREAD_EVENT_DATA
{
#ifdef _WIN32
	NPC_HANDLE						hEvent;							//事件
#else
	pthread_mutex_t					t_mutex_lock;					//互斥锁
	NPC_BOOL						bIfInitMutex;					//是否初始化了互斥锁

	pthread_cond_t					cond_event;						//事件条件变量
	NPC_BOOL						bIfInitCond;					//是否初始化了条件变量

	NPC_BOOL						bSingleFlag;					//信号标志，TRUE表示有信号，FALSE表示无信号
	NPC_BOOL						bManualReset; 					//是否手动复位为无信号

	NPC_HANDLE						hWaitObjTable[NPC_D_THREAD_MAX_WAIT_OBJ_NUM];	//等待对象表
#endif

} NPC_S_THREAD_EVENT_DATA, *PNPC_S_THREAD_EVENT_DATA;

//信号量数据定义
typedef struct _NPC_S_THREAD_SEM_DATA
{
#ifdef _WIN32
	NPC_HANDLE						hSemaphore;						//信号量
#else
	pthread_mutex_t					t_mutex_lock;					//互斥锁
	NPC_BOOL						bIfInitMutex;					//是否初始化了互斥锁

	pthread_cond_t					cond_event;						//事件条件变量
	NPC_BOOL						bIfInitCond;					//是否初始化了条件变量

	NPC_UINT						uiSemNum;						//信号数
	NPC_UINT						uiMaxSemNum;					//最大信号数
#endif

} NPC_S_THREAD_SEM_DATA, *PNPC_S_THREAD_SEM_DATA;

//待待对象数据定义
typedef struct _NPC_S_THREAD_WAITOBJ_DATA
{
	NPC_INT							iObjNum;						//对象数

#ifdef _WIN32
	NPC_HANDLE						hWaitObjTable[NPC_D_THREAD_MAX_WAIT_OBJ_NUM];		//等待对象表
#else
	pthread_mutex_t					t_mutex_lock;					//互斥锁
	NPC_BOOL						bIfInitMutex;					//是否初始化了互斥锁

	pthread_cond_t					cond_event;						//事件条件变量
	NPC_BOOL						bIfInitCond;					//是否初始化了条件变量

	NPC_HANDLE						hWaitObjTable[NPC_D_THREAD_MAX_WAIT_OBJ_NUM];		//等待对象表
	NPC_BOOL						bObjSigFlag[NPC_D_THREAD_MAX_WAIT_OBJ_NUM];			//对象是否有信号
#endif

} NPC_S_THREAD_WAITOBJ_DATA, *PNPC_S_THREAD_WAITOBJ_DATA;



#endif
