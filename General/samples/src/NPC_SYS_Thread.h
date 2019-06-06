/*----------------------------------------------------------------------------
 * Copyright(c)  :  NPC CORPORTAION All Rights Reserved                       
 * FileName      :  NPC_SYS_Thread.h
 * Version       :  1.1
 * Author        :  CCH
 * DateTime      :  2012-01-20
 * Description   :  NPC系统线程处理模块
 *----------------------------------------------------------------------------*/

#ifndef __NPC_SYS_THREAD_H
#define __NPC_SYS_THREAD_H

#include "NPC_TypeDef.h"


//--------------------------------------------------------------------------------
//导入导出定义
#ifdef _NPC_THREAD_EXPORTS
	#define NPC_THREAD_API __declspec(dllexport)
#else
	#ifdef _NPC_THREAD_IMPORT
		#define NPC_THREAD_API __declspec(dllimport)
	#else
		#define NPC_THREAD_API
	#endif
#endif


//--------------------------------------------------------------------------------
//线程过程定义
typedef NPC_VOID (*fNPC_F_THREAD_StartProc)(PNPC_VOID pInParam);


//---------------------------------------------------------------------
#ifdef _WIN32
// C++ support
#ifdef __cplusplus
extern "C"
{
#endif
#endif


//--------------------------------------------------------------------------------
//创建线程
NPC_THREAD_API		NPC_HANDLE	NPC_F_THREAD_CreateThread(
	IN		fNPC_F_THREAD_StartProc			in_pWorkProc,			//线程工作过程
	IN		PNPC_VOID						in_pInParam				//传入参数
	);

//退出线程
NPC_THREAD_API		NPC_VOID	NPC_F_THREAD_WaitExitThread(
	IN		NPC_HANDLE						in_hThread,				//线程句柄
	IN		NPC_INT							in_iTimeout				//超时时长（毫秒）
	);


//--------------------------------------------------------------------------------
//创建互斥锁
NPC_THREAD_API		NPC_HANDLE	NPC_F_THREAD_Mutex_Create();

//释放互斥锁
NPC_THREAD_API		NPC_VOID	NPC_F_THREAD_Mutex_Destroy(
	IN		NPC_HANDLE						in_hMutex				//互斥锁句柄
	);

//加锁
NPC_THREAD_API		NPC_VOID	NPC_F_THREAD_Mutex_Lock(
	IN		NPC_HANDLE						in_hMutex				//互斥锁句柄
	);

//解锁
NPC_THREAD_API		NPC_VOID	NPC_F_THREAD_Mutex_Unlock(
	IN		NPC_HANDLE						in_hMutex				//互斥锁句柄
	);

//--------------------------------------------------------------------------------
//创建事件
NPC_THREAD_API		NPC_HANDLE	NPC_F_THREAD_Event_Create(
	IN		NPC_BOOL						in_bManualReset, 		//是否手动复位为无信号
	IN		NPC_BOOL						in_bInitSigState		//初始状态是否有信号
	);

//释放事件
NPC_THREAD_API		NPC_VOID	NPC_F_THREAD_Event_Destroy(
	IN		NPC_HANDLE						in_hEvent				//事件句柄
	);

//设置事件有信号
NPC_THREAD_API		NPC_VOID	NPC_F_THREAD_Event_SetEvent(
	IN		NPC_HANDLE						in_hEvent				//事件句柄
	);

//复恢事件为无信号
NPC_THREAD_API		NPC_VOID	NPC_F_THREAD_Event_ResetEvent(
	IN		NPC_HANDLE						in_hEvent				//事件句柄
	);

//等待事件，返回TRUE表示有信号，FALSE表示无信号
NPC_THREAD_API		NPC_BOOL	NPC_F_THREAD_Event_Wait(
	IN		NPC_HANDLE						in_hEvent,				//事件句柄
	IN		NPC_INT							in_iTimeout				//超时时长（毫秒）
	);


#ifdef _WIN32
//获WINDOWS事件句柄
NPC_THREAD_API		HANDLE		NPC_F_THREAD_Event_GetWinEventHandle(
	IN		NPC_HANDLE						in_hEvent				//事件句柄
	);
#else
//查询事件是否有信号，如果没有，则置入等待对象
NPC_THREAD_API		NPC_BOOL	NPC_F_THREAD_Event_QuerySetWaitObj(
	IN		NPC_HANDLE						in_hEvent,				//事件句柄
	IN		NPC_HANDLE						in_hWaitObj				//等待对象句柄
	);

//事件清除等待对象
NPC_THREAD_API		NPC_VOID	NPC_F_THREAD_Event_ClearWaitObj(
	IN		NPC_HANDLE						in_hEvent,				//事件句柄
	IN		NPC_HANDLE						in_hWaitObj				//等待对象句柄
	);

//事件清除所有等待对象
NPC_THREAD_API		NPC_VOID	NPC_F_THREAD_Event_ClearAllWaitObj(
	IN		NPC_HANDLE						in_hEvent				//事件句柄
	);
#endif


//--------------------------------------------------------------------------------
//创建信号量
NPC_THREAD_API		NPC_HANDLE	NPC_F_THREAD_Sem_Create(
	IN		NPC_UINT						in_uiInitSemNum,		//初始信号数
	IN		NPC_UINT						in_uiMaxSemNum			//最大信号数
	);

//释放信号量
NPC_THREAD_API		NPC_VOID	NPC_F_THREAD_Sem_Destroy(
	IN		NPC_HANDLE						in_hSem					//信号量句柄
	);

//释放信号量，即增加N个信号
NPC_THREAD_API		NPC_VOID	NPC_F_THREAD_Sem_Release(
	IN		NPC_HANDLE						in_hSem,				//信号量句柄
	IN		NPC_UINT						in_uiRelSemNum			//释放信号数
	);

//等待信号量，返回TRUE表示有信号，FALSE表示无信号
NPC_THREAD_API		NPC_BOOL	NPC_F_THREAD_Sem_Wait(
	IN		NPC_HANDLE						in_hSem,				//信号量句柄
	IN		NPC_INT							in_iTimeout				//超时时长（毫秒）
	);


//--------------------------------------------------------------------------------
//创建等待对象
NPC_THREAD_API		NPC_HANDLE	NPC_F_THREAD_WaitObj_Create();

//释放等待对象
NPC_THREAD_API		NPC_VOID	NPC_F_THREAD_WaitObj_Destroy(
	IN		NPC_HANDLE						in_hWaitObj				//等待对象
	);

//等待对象添加事件
NPC_THREAD_API		NPC_BOOL	NPC_F_THREAD_WaitObj_AddEvent(
	IN		NPC_HANDLE						in_hWaitObj,			//等待对象
	IN		NPC_HANDLE						in_hEvent				//事件句柄
	);

//等待对象删除事件
NPC_THREAD_API		NPC_BOOL	NPC_F_THREAD_WaitObj_DelEvent(
	IN		NPC_HANDLE						in_hWaitObj,			//等待对象
	IN		NPC_HANDLE						in_hEvent				//事件句柄
	);

//清除所有事件
NPC_THREAD_API		NPC_VOID	NPC_F_THREAD_WaitObj_ClearAllEvent(
	IN		NPC_HANDLE						in_hWaitObj				//等待对象
	);

#ifndef _WIN32
//设置等待对象中的事件有信号
NPC_THREAD_API		NPC_VOID	NPC_F_THREAD_WaitObj_SetEventSig(
	IN		NPC_HANDLE						in_hWaitObj,			//等待对象
	IN		NPC_HANDLE						in_hEvent				//事件句柄
	);
#endif

//等待事件，返回有信号的索引号，返回-1表示超时，返回-2表示有错误，如果等待所有信号，则返回信号数
NPC_THREAD_API		NPC_INT		NPC_F_THREAD_WaitObj_WaitEvent(
	IN		NPC_HANDLE						in_hWaitObj,			//等待对象
	IN		NPC_BOOL						in_bIfWaitAll,			//是否等待所有有信号后才返回
	IN		NPC_INT							in_iTimeout				//超时时长（毫秒）
	);


//-----------------------------------------------------------------------------------------------
#ifdef _WIN32
//获取错误信息
NPC_VOID	NPC_F_THREAD_PR_GetLastErrorInfo(
	IN		NPC_DWORD								in_dwErrorCode,				//错误码
	IN		PNPC_CHAR								in_pErrorBuf,				//错误信息缓冲区
	IN		NPC_INT									in_iErrorBufSize			//错误信息缓冲区大小
	);
#endif



#ifdef _WIN32
// C++ support
#ifdef __cplusplus
}
#endif
#endif



#endif

