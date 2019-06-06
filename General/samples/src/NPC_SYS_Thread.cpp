/*----------------------------------------------------------------------------
 * Copyright(c)  :  NPC CORPORTAION All Rights Reserved                       
 * FileName      :  NPC_SYS_Thread.cpp
 * Version       :  1.1
 * Author        :  CCH
 * DateTime      :  2012-01-20
 * Description   :  NPC系统线程处理模块
 *----------------------------------------------------------------------------*/

#include "NPC_SYS_Thread.h"
#include "NPC_SYS_Thread_DataDef.h"
#include "NPC_LOG_Server.h"
#include "NPC_SYS_FuncShare.h"

#ifdef WINAPI_FAMILY
	using namespace std;
	using namespace Windows::System::Threading;
	#include <winbase.h>
	#include <windows.h>
	#include <synchapi.h>
	#include "WindowsPhoneRuntimeComponent1.h"
	#include "ThreadEmulation.h"
#endif

#ifdef _WIN32
	#include <process.h>
#endif


#ifndef _WIN32
//条件变量等待
NPC_INT		NPC_F_THREAD_PR_CondTimeWait(
	IN		pthread_cond_t*						in_pCond,					//条件变量指针
	IN		pthread_mutex_t*					in_pMutex,					//互斥锁指针
	IN		NPC_INT								in_iTimeout					//超时时长（毫秒）
	)
{
	struct timeval now;
	struct timespec outtime;

	gettimeofday(&now, NULL);

	outtime.tv_sec = now.tv_sec + ((now.tv_usec + in_iTimeout * 1000) / (1000 * 1000));
	outtime.tv_nsec = ((now.tv_usec + in_iTimeout * 1000) % (1000 * 1000)) * 1000;

	return pthread_cond_timedwait(
		in_pCond, 
		in_pMutex, 
		&outtime);
}
#endif


#ifdef _WIN32
void ThreadWriteLog(char *pInfo)
{
	FILE *f = fopen(".\\thread_log.txt", "a+");
	if ( f )
	{
		fprintf(f, "%s\n", pInfo);

		fclose(f);
		f = NULL;
	}

#ifndef WINAPI_FAMILY
	printf("%s\n", pInfo);
#endif
}
#endif

//------------------------------------------------------------------------------------------------
//工作线程
#ifdef _WIN32
NPC_DWORD	NPC_F_THREAD_ThreadWork(
	IN		PNPC_VOID							in_pParameter				//参数指针
	)
#else
NPC_VOID	NPC_F_THREAD_ThreadWork(
	IN		PNPC_VOID							in_pParameter				//参数指针
	)
#endif
{
	PNPC_S_THREAD_DATA							pThreadData;

	if ( in_pParameter )
	{
		pThreadData = (PNPC_S_THREAD_DATA)in_pParameter;

		if ( pThreadData->pThreadProc )
		{
			pThreadData->pThreadProc(pThreadData->pThreadParam);
		}

		pThreadData->iEndFlag = 200;
	}

#ifdef _WIN32
	#ifndef WINAPI_FAMILY
	_endthreadex(0);
	#endif
	return 0;
#endif
}

//--------------------------------------------------------------------------------
//创建线程
NPC_THREAD_API		NPC_HANDLE	NPC_F_THREAD_CreateThread(
	IN		fNPC_F_THREAD_StartProc			in_pWorkProc,			//线程工作过程
	IN		PNPC_VOID						in_pInParam				//传入参数
	)
{
	NPC_BOOL								bRet;
	PNPC_S_THREAD_DATA						pThreadData;

	//INIT AREA
	{
		bRet = TRUE;
		pThreadData = NULL;
	}

	//CODE AREA
	{
		//分配内存
		pThreadData = (PNPC_S_THREAD_DATA)malloc(sizeof(NPC_S_THREAD_DATA));
		if ( pThreadData == NULL )
		{
#ifndef WINAPI_FAMILY
			printf("NPC_F_THREAD_CreateThread malloc fail.\n");
#endif

			bRet = FALSE;
			goto _NPC_CLEAR;
		}
		memset(pThreadData, 0, sizeof(NPC_S_THREAD_DATA));

		pThreadData->pThreadProc = in_pWorkProc;
		pThreadData->pThreadParam = in_pInParam;

#ifdef _WIN32
	#ifdef WINAPI_FAMILY
		WindowsPhoneRuntimeComponent1::Oval_thread::Thread_start((US_THREAD_START_ROUTIN)NPC_F_THREAD_ThreadWork, pThreadData);
		pThreadData->hThread = (HANDLE)(rand());
	#else
		//创建线程
		pThreadData->hThread = (HANDLE)_beginthreadex(
			NULL, 
			1024*256, 
			(unsigned int (__stdcall *)(void *))NPC_F_THREAD_ThreadWork, 
			pThreadData, 
			0, 
			(unsigned int *)&(pThreadData->dwThreadId));
		if ( pThreadData->hThread == NULL || (NPC_INT)(pThreadData->hThread) == -1 )
		{
			NPC_CHAR sTempErrorBuf[512];
			NPC_CHAR sTempLogBuf[700];
			NPC_DWORD dwErrCode;

			dwErrCode = GetLastError();

			//获取错误信息
			NPC_F_THREAD_PR_GetLastErrorInfo(
				dwErrCode,				//错误码
				sTempErrorBuf,			//错误信息缓冲区
				sizeof(sTempErrorBuf)	//错误信息缓冲区大小
				);

			sprintf(sTempLogBuf, "NPC_F_THREAD_CreateThread _beginthreadex fail, ret=%d, GetLastError=%ld, ErrInfo: %s.", 
				(NPC_INT)(pThreadData->hThread), dwErrCode, sTempErrorBuf);

			ThreadWriteLog(sTempLogBuf);

			bRet = FALSE;
			goto _NPC_CLEAR;
		}
	#endif
#else
		NPC_INT iRet;

#if NPC_D_DPS_IF_CONTROL_PTHREAD_ATTR_SETSTACKSIZE
		pthread_attr_t attr; 
		pthread_attr_init(&attr);
		pthread_attr_setstacksize(&attr, 4*1024*1024);
#endif

		//创建线程
		if ( (iRet=pthread_create(
			&pThreadData->t_thread_data, 
			0, 
			(void*(*)(void*))NPC_F_THREAD_ThreadWork, 
			(void*)pThreadData)) != 0 )
		{
			printf("NPC_F_THREAD_CreateThread pthread_create fail, ret=%d.\n", iRet);

			bRet = FALSE;
			goto _NPC_CLEAR;
		}
		pThreadData->bIfCreated = TRUE;
#endif
	}

	//CLEAR AREA
	_NPC_CLEAR:
	{
		if ( !bRet && pThreadData )
		{
			//等待退出线程
			NPC_F_THREAD_WaitExitThread(
				(NPC_HANDLE)pThreadData,		//线程数据
				0								//超时时长（毫秒）
				);
			pThreadData = NULL;
		}
	}

	return (NPC_HANDLE)pThreadData;
}

//等待退出线程
NPC_THREAD_API		NPC_VOID	NPC_F_THREAD_WaitExitThread(
	IN		NPC_HANDLE						in_hThread,				//线程句柄
	IN		NPC_INT							in_iTimeout				//超时时长（毫秒）
	)
{
	PNPC_S_THREAD_DATA						pThreadData;

	pThreadData = (PNPC_S_THREAD_DATA)in_hThread;
	if ( pThreadData == NULL )
		return;

#ifdef _WIN32
	#ifdef WINAPI_FAMILY
		while ( in_iTimeout > 0 )
		{
			if ( pThreadData->iEndFlag == 200 )
				break;

			NPC_F_SYS_Sleep(10);
			in_iTimeout -= 10;
		}
	#else
		if ( pThreadData->hThread ) 
		{
			if ( WaitForSingleObject(pThreadData->hThread, in_iTimeout) != WAIT_OBJECT_0 )
			{
				TerminateThread(pThreadData->hThread, 0);
			}

			CloseHandle(pThreadData->hThread);
			pThreadData->hThread = NULL;
		}
	#endif
#else
	if ( pThreadData->bIfCreated )
	{
		while ( in_iTimeout > 0 )
		{
			if ( pThreadData->iEndFlag == 200 )
				break;

			NPC_F_SYS_Sleep(10);
			in_iTimeout -= 10;
		}

		pthread_join(pThreadData->t_thread_data, NULL);
	}
#endif

	free(pThreadData);
	pThreadData = NULL;
}


//--------------------------------------------------------------------------------
//创建互斥锁
NPC_THREAD_API		NPC_HANDLE	NPC_F_THREAD_Mutex_Create()
{
	NPC_BOOL								bRet;
	PNPC_S_THREAD_MUTEX_DATA				pMutexData;

	//INIT AREA
	{
		bRet = TRUE;
		pMutexData = NULL;
	}

	//CODE AREA
	{
		//分配内存
		pMutexData = (PNPC_S_THREAD_MUTEX_DATA)malloc(sizeof(NPC_S_THREAD_MUTEX_DATA));
		if ( pMutexData == NULL )
		{
#ifndef WINAPI_FAMILY
			printf("NPC_F_THREAD_Mutex_Create malloc fail.\n");
#endif
			bRet = FALSE;
			goto _NPC_CLEAR;
		}
		memset(pMutexData, 0, sizeof(NPC_S_THREAD_MUTEX_DATA));

#ifdef _WIN32
	#ifdef WINAPI_FAMILY
        InitializeCriticalSectionEx(&pMutexData->crsMutex, 0, 0);
	#else
		InitializeCriticalSection(&pMutexData->crsMutex);
	#endif
#else
		if ( pthread_mutex_init(&pMutexData->mutex_lock, NULL) != 0 )
		{
			printf("NPC_F_THREAD_Mutex_Create pthread_mutex_init fail.\n");

			bRet = FALSE;
			goto _NPC_CLEAR;
		}
#endif
	}

	//CLEAR AREA
	_NPC_CLEAR:
	{
		if ( !bRet && pMutexData )
		{
			//释放互斥锁
			NPC_F_THREAD_Mutex_Destroy(
				(NPC_HANDLE)pMutexData			//互斥锁句柄
				);
			pMutexData = NULL;
		}
	}

	return (NPC_HANDLE)pMutexData;
}

//释放互斥锁
NPC_THREAD_API		NPC_VOID	NPC_F_THREAD_Mutex_Destroy(
	IN		NPC_HANDLE						in_hMutex				//互斥锁句柄
	)
{
	PNPC_S_THREAD_MUTEX_DATA				pMutexData;

	pMutexData = (PNPC_S_THREAD_MUTEX_DATA)in_hMutex;
	if ( pMutexData == NULL )
		return;

#ifdef _WIN32
	DeleteCriticalSection(&pMutexData->crsMutex);
#else
	pthread_mutex_destroy(&pMutexData->mutex_lock);
#endif

	free(pMutexData);
	pMutexData = NULL;
}

//加锁
NPC_THREAD_API		NPC_VOID	NPC_F_THREAD_Mutex_Lock(
	IN		NPC_HANDLE						in_hMutex				//互斥锁句柄
	)
{
	PNPC_S_THREAD_MUTEX_DATA				pMutexData;

	pMutexData = (PNPC_S_THREAD_MUTEX_DATA)in_hMutex;
	if ( pMutexData == NULL )
		return;

#ifdef _WIN32
	EnterCriticalSection(&pMutexData->crsMutex);
#else
	pthread_mutex_lock(&pMutexData->mutex_lock);
#endif
}

//解锁
NPC_THREAD_API		NPC_VOID	NPC_F_THREAD_Mutex_Unlock(
	IN		NPC_HANDLE						in_hMutex				//互斥锁句柄
	)
{
	PNPC_S_THREAD_MUTEX_DATA				pMutexData;

	pMutexData = (PNPC_S_THREAD_MUTEX_DATA)in_hMutex;
	if ( pMutexData == NULL )
		return;

#ifdef _WIN32
	LeaveCriticalSection(&pMutexData->crsMutex);
#else
	pthread_mutex_unlock(&pMutexData->mutex_lock);
#endif
}


//--------------------------------------------------------------------------------
//创建事件
NPC_THREAD_API		NPC_HANDLE	NPC_F_THREAD_Event_Create(
	IN		NPC_BOOL						in_bManualReset, 		//是否手动复位为无信号
	IN		NPC_BOOL						in_bInitSigState		//初始状态是否有信号
	)
{
	NPC_BOOL								bRet;
	PNPC_S_THREAD_EVENT_DATA				pEventData;

	//INIT AREA
	{
		bRet = TRUE;
		pEventData = NULL;
	}

	//CODE AREA
	{
		pEventData = (PNPC_S_THREAD_EVENT_DATA)malloc(sizeof(NPC_S_THREAD_EVENT_DATA));
		if ( pEventData == NULL )
		{
#ifndef WINAPI_FAMILY
			printf("NPC_F_THREAD_Event_Create malloc fail.\n");
#endif

			bRet = FALSE;
			goto _NPC_CLEAR;
		}
		memset(pEventData, 0, sizeof(NPC_S_THREAD_EVENT_DATA));

#ifdef _WIN32
	#ifdef WINAPI_FAMILY
		DWORD dwFlag = 0;

		if ( in_bManualReset )
			dwFlag += CREATE_EVENT_MANUAL_RESET;

		if ( in_bInitSigState )
			dwFlag += CREATE_EVENT_INITIAL_SET;

		pEventData->hEvent = CreateEventEx(NULL, NULL, dwFlag, EVENT_ALL_ACCESS);
		if ( pEventData->hEvent == NULL )
		{
			return FALSE;
		}
	#else
		pEventData->hEvent = CreateEvent(NULL, in_bManualReset, in_bInitSigState, NULL);
		if ( pEventData->hEvent == NULL )
		{
			printf("NPC_F_THREAD_Event_Create CreateEvent fail.\n");
			return FALSE;
		}
	#endif
#else
		//初始化互斥锁
		if ( pthread_mutex_init(&pEventData->t_mutex_lock, NULL ) != 0 )
		{
			printf("NPC_F_THREAD_Event_Create pthread_mutex_init fail.\n");

			return FALSE;
		}
		pEventData->bIfInitMutex = TRUE;

		//初始化条件变量
		if ( pthread_cond_init(&pEventData->cond_event, NULL) != 0 )
		{
			printf("NPC_F_THREAD_Event_Create pthread_cond_init fail.\n");

			return FALSE;
		}
		pEventData->bIfInitCond = TRUE;

		pEventData->bSingleFlag = in_bInitSigState;
		pEventData->bManualReset = in_bManualReset;
#endif
	}

	//CLEAR AREA
	_NPC_CLEAR:
	{
		if ( !bRet && pEventData )
		{
			//反初始化事件
			NPC_F_THREAD_Event_Destroy(
				(NPC_HANDLE)pEventData				//事件句柄
				);
			pEventData = NULL;
		}
	}

	return (NPC_HANDLE)pEventData;
}

//释放事件
NPC_THREAD_API		NPC_VOID	NPC_F_THREAD_Event_Destroy(
	IN		NPC_HANDLE						in_hEvent				//事件句柄
	)
{
	PNPC_S_THREAD_EVENT_DATA				pEventData;

	pEventData = (PNPC_S_THREAD_EVENT_DATA)in_hEvent;
	if ( pEventData == NULL )
		return;

#ifdef _WIN32
	if ( pEventData->hEvent )
	{
		CloseHandle(pEventData->hEvent);
		pEventData->hEvent = NULL;
	}
#else
	if ( pEventData->bIfInitMutex )
	{
		pthread_mutex_destroy(&pEventData->t_mutex_lock);
		pEventData->bIfInitMutex = FALSE;
	}

	if ( pEventData->bIfInitCond )
	{
		pthread_cond_destroy(&pEventData->cond_event);
		pEventData->bIfInitCond = FALSE;
	}
#endif

	free(pEventData);
	pEventData = NULL;
}

//设置事件有信号
NPC_THREAD_API		NPC_VOID	NPC_F_THREAD_Event_SetEvent(
	IN		NPC_HANDLE						in_hEvent				//事件句柄
	)
{
	PNPC_S_THREAD_EVENT_DATA				pEventData;

	pEventData = (PNPC_S_THREAD_EVENT_DATA)in_hEvent;
	if ( pEventData == NULL )
		return;

#ifdef _WIN32
	if ( pEventData->hEvent )
	{
		SetEvent(pEventData->hEvent);
	}
#else
	{
		NPC_INT i;

		pthread_mutex_lock(&pEventData->t_mutex_lock);

		pEventData->bSingleFlag = TRUE;

		pthread_cond_signal(&pEventData->cond_event);

		pthread_mutex_unlock(&pEventData->t_mutex_lock);

		for ( i=0; i<NPC_D_THREAD_MAX_WAIT_OBJ_NUM; i++ )
		{
			if ( pEventData->hWaitObjTable[i] )
			{
				//设置等待对象中的事件有信号
				NPC_F_THREAD_WaitObj_SetEventSig(
					pEventData->hWaitObjTable[i],		//等待对象
					in_hEvent							//事件句柄
					);
			}
		}
	}
#endif
}

//复恢事件为无信号
NPC_THREAD_API		NPC_VOID	NPC_F_THREAD_Event_ResetEvent(
	IN		NPC_HANDLE						in_hEvent				//事件句柄
	)
{
	PNPC_S_THREAD_EVENT_DATA				pEventData;

	pEventData = (PNPC_S_THREAD_EVENT_DATA)in_hEvent;
	if ( pEventData == NULL )
		return;

#ifdef _WIN32
	if ( pEventData->hEvent )
	{
		ResetEvent(pEventData->hEvent);
	}
#else
	pthread_mutex_lock(&pEventData->t_mutex_lock);

	pEventData->bSingleFlag = FALSE;

	pthread_mutex_unlock(&pEventData->t_mutex_lock);
#endif
}

//等待事件，返回TRUE表示有信号，FALSE表示无信号
NPC_THREAD_API		NPC_BOOL	NPC_F_THREAD_Event_Wait(
	IN		NPC_HANDLE						in_hEvent,				//事件句柄
	IN		NPC_INT							in_iTimeout				//超时时长（毫秒）
	)
{
	PNPC_S_THREAD_EVENT_DATA				pEventData;
	NPC_BOOL								bIfHasSig;

	//INIT AREA
	{
		pEventData = (PNPC_S_THREAD_EVENT_DATA)in_hEvent;
		if ( pEventData == NULL )
			return FALSE;

		bIfHasSig = FALSE;
	}

	//CODE AREA
	{
#ifdef _WIN32
	#ifdef WINAPI_FAMILY
		if ( pEventData->hEvent )
		{
			if ( WaitForSingleObjectEx(pEventData->hEvent, in_iTimeout, TRUE) == WAIT_OBJECT_0 )
			{
				bIfHasSig = TRUE;
			}
		}
	#else
		if ( pEventData->hEvent )
		{
			if ( WaitForSingleObject(pEventData->hEvent, in_iTimeout) == WAIT_OBJECT_0 )
			{
				bIfHasSig = TRUE;
			}
		}
	#endif
#else
		pthread_mutex_lock(&pEventData->t_mutex_lock);

		if ( !pEventData->bSingleFlag )
		{
#ifdef __APPLE__
			//条件变量等待
			NPC_F_THREAD_PR_CondTimeWait(
				&pEventData->cond_event,				//条件变量指针
				&pEventData->t_mutex_lock,				//互斥锁指针
				in_iTimeout								//超时时长（毫秒）
				);
#else
			while ( in_iTimeout > 0 )
			{
				if ( pEventData->bSingleFlag )
					break;

				pthread_mutex_unlock(&pEventData->t_mutex_lock);

				NPC_F_SYS_Sleep(10);
				in_iTimeout -= 10;

				pthread_mutex_lock(&pEventData->t_mutex_lock);
			}
#endif
		}

		if ( pEventData->bSingleFlag )
		{
			bIfHasSig = TRUE;

			if ( !pEventData->bManualReset )
			{
				pEventData->bSingleFlag = FALSE;
			}
		}

		pthread_mutex_unlock(&pEventData->t_mutex_lock);
#endif
	}

	return bIfHasSig;
}

#ifdef _WIN32
//获WINDOWS事件句柄
NPC_THREAD_API		HANDLE		NPC_F_THREAD_Event_GetWinEventHandle(
	IN		NPC_HANDLE						in_hEvent				//事件句柄
	)
{
	PNPC_S_THREAD_EVENT_DATA				pEventData;

	pEventData = (PNPC_S_THREAD_EVENT_DATA)in_hEvent;
	if ( pEventData == NULL )
		return NULL;

	return pEventData->hEvent;
}
#else

//查询事件是否有信号，如果没有，则置入等待对象
NPC_THREAD_API		NPC_BOOL	NPC_F_THREAD_Event_QuerySetWaitObj(
	IN		NPC_HANDLE						in_hEvent,				//事件句柄
	IN		NPC_HANDLE						in_hWaitObj				//等待对象句柄
	)
{
	NPC_BOOL								bRet;
	PNPC_S_THREAD_EVENT_DATA				pEventData;
	NPC_INT									iWaitObjIndex;
	NPC_INT									i;

	//INIT AREA
	{
		bRet = FALSE;

		pEventData = (PNPC_S_THREAD_EVENT_DATA)in_hEvent;
		if ( pEventData == NULL )
			return FALSE;

		pthread_mutex_lock(&pEventData->t_mutex_lock);
	}

	//CODE AREA
	{
		//判断事件是否有信号
		if ( pEventData->bSingleFlag )
		{
			bRet = TRUE;
			goto _NPC_CLEAR;
		}

		//查询是否存在
		iWaitObjIndex = -1;
		for ( i=0; i<NPC_D_THREAD_MAX_WAIT_OBJ_NUM; i++ )
		{
			if ( pEventData->hWaitObjTable[i] && 
				pEventData->hWaitObjTable[i] == in_hWaitObj )
			{
				iWaitObjIndex = i;
				break;
			}
		}
		if ( iWaitObjIndex == -1 )
		{
			for ( i=0; i<NPC_D_THREAD_MAX_WAIT_OBJ_NUM; i++ )
			{
				if ( pEventData->hWaitObjTable[i] == NULL )
				{
					iWaitObjIndex = i;
					break;
				}
			}
			if ( iWaitObjIndex != -1 )
			{
				pEventData->hWaitObjTable[iWaitObjIndex] = in_hWaitObj;
			}
		}
	}

	//CLEAR AREA
	_NPC_CLEAR:
	{
		pthread_mutex_unlock(&pEventData->t_mutex_lock);
	}

	return bRet;
}

//事件清除等待对象
NPC_THREAD_API		NPC_VOID	NPC_F_THREAD_Event_ClearWaitObj(
	IN		NPC_HANDLE						in_hEvent,				//事件句柄
	IN		NPC_HANDLE						in_hWaitObj				//等待对象句柄
	)
{
	PNPC_S_THREAD_EVENT_DATA				pEventData;
	NPC_INT									i;

	pEventData = (PNPC_S_THREAD_EVENT_DATA)in_hEvent;
	if ( pEventData == NULL )
		return;

	for ( i=0; i<NPC_D_THREAD_MAX_WAIT_OBJ_NUM; i++ )
	{
		if ( pEventData->hWaitObjTable[i] == in_hWaitObj )
		{
			pEventData->hWaitObjTable[i] = NULL;
		}
	}
}

//事件清除所有等待对象
NPC_THREAD_API		NPC_VOID	NPC_F_THREAD_Event_ClearAllWaitObj(
	IN		NPC_HANDLE						in_hEvent				//事件句柄
	)
{
	PNPC_S_THREAD_EVENT_DATA				pEventData;
	NPC_INT									i;

	pEventData = (PNPC_S_THREAD_EVENT_DATA)in_hEvent;
	if ( pEventData == NULL )
		return;

	for ( i=0; i<NPC_D_THREAD_MAX_WAIT_OBJ_NUM; i++ )
	{
		pEventData->hWaitObjTable[i] = NULL;
	}
}
#endif


//--------------------------------------------------------------------------------
//创建信号量
NPC_THREAD_API		NPC_HANDLE	NPC_F_THREAD_Sem_Create(
	IN		NPC_UINT						in_uiInitSemNum,		//初始信号数
	IN		NPC_UINT						in_uiMaxSemNum			//最大信号数
	)
{
	NPC_BOOL								bRet;
	PNPC_S_THREAD_SEM_DATA					pSemData;

	//INIT AREA
	{
		bRet = TRUE;
		pSemData = NULL;
	}

	//CODE AREA
	{
		pSemData = (PNPC_S_THREAD_SEM_DATA)malloc(sizeof(NPC_S_THREAD_SEM_DATA));
		if ( pSemData == NULL )
		{
			printf("NPC_F_THREAD_Sem_Create malloc fail.\n");

			bRet = FALSE;
			goto _NPC_CLEAR;
		}
		memset(pSemData, 0, sizeof(NPC_S_THREAD_SEM_DATA));

#ifdef _WIN32
		//if ( in_uiInitSemNum < 1 )
		//	in_uiInitSemNum = 1;

	#ifdef WINAPI_FAMILY
		pSemData->hSemaphore = CreateSemaphoreEx( 
			NULL,							// no security attributes
			in_uiInitSemNum,				// initial count
			in_uiMaxSemNum,					// maximum count
			NULL, 							// unnamed semaphore
			0, 
			EVENT_ALL_ACCESS
			);
		if ( pSemData->hSemaphore == NULL )
		{
			return FALSE;
		}
	#else
		pSemData->hSemaphore = CreateSemaphore( 
			NULL,							// no security attributes
			in_uiInitSemNum,				// initial count
			in_uiMaxSemNum,					// maximum count
			NULL							// unnamed semaphore
			);
		if ( pSemData->hSemaphore == NULL )
		{
			printf("NPC_F_THREAD_Sem_Create CreateSemaphore fail.\n");

			return FALSE;
		}
	#endif
#else
		//初始化互斥锁
		if ( pthread_mutex_init(&pSemData->t_mutex_lock, NULL ) != 0 )
		{
			printf("NPC_F_THREAD_Sem_Create pthread_mutex_init fail.\n");

			return FALSE;
		}
		pSemData->bIfInitMutex = TRUE;

		//初始化条件变量
		if ( pthread_cond_init(&pSemData->cond_event, NULL) != 0 )
		{
			printf("NPC_F_THREAD_Sem_Create pthread_cond_init fail.\n");

			pthread_mutex_destroy(&pSemData->t_mutex_lock);
			pSemData->bIfInitMutex = FALSE;
			return FALSE;
		}
		pSemData->bIfInitCond = TRUE;

		pSemData->uiSemNum = in_uiInitSemNum;
		pSemData->uiMaxSemNum = in_uiMaxSemNum;
#endif
	}

	//CLEAR AREA
	_NPC_CLEAR:
	{
		if ( !bRet && pSemData )
		{
			//释放信号量
			NPC_F_THREAD_Sem_Destroy(
				(NPC_HANDLE)pSemData					//信号量句柄
				);
			pSemData = NULL;
		}
	}

	return (NPC_HANDLE)pSemData;
}

//释放信号量
NPC_THREAD_API		NPC_VOID	NPC_F_THREAD_Sem_Destroy(
	IN		NPC_HANDLE						in_hSem					//信号量句柄
	)
{
	PNPC_S_THREAD_SEM_DATA					pSemData;

	pSemData = (PNPC_S_THREAD_SEM_DATA)in_hSem;
	if ( pSemData == NULL )
		return;

#ifdef _WIN32
	if ( pSemData->hSemaphore )
	{
		CloseHandle(pSemData->hSemaphore);
		pSemData->hSemaphore = NULL;
	}
#else
	if ( pSemData->bIfInitMutex )
	{
		pthread_mutex_destroy(&pSemData->t_mutex_lock);
		pSemData->bIfInitMutex = FALSE;
	}

	if ( pSemData->bIfInitCond )
	{
		pthread_cond_destroy(&pSemData->cond_event);
		pSemData->bIfInitCond = FALSE;
	}

	pSemData->uiSemNum = 0;
	pSemData->uiMaxSemNum = 0;
#endif

	free(pSemData);
	pSemData = NULL;
}

//释放信号量，即增加N个信号
NPC_THREAD_API		NPC_VOID	NPC_F_THREAD_Sem_Release(
	IN		NPC_HANDLE						in_hSem,				//信号量句柄
	IN		NPC_UINT						in_uiRelSemNum			//释放信号数
	)
{
	PNPC_S_THREAD_SEM_DATA					pSemData;

	pSemData = (PNPC_S_THREAD_SEM_DATA)in_hSem;
	if ( pSemData == NULL )
		return;

#ifdef _WIN32
	if ( pSemData->hSemaphore )
	{
		ReleaseSemaphore(pSemData->hSemaphore, in_uiRelSemNum, NULL);
	}
#else
	pthread_mutex_lock(&pSemData->t_mutex_lock);

	if ( pSemData->uiSemNum + in_uiRelSemNum > pSemData->uiMaxSemNum )
	{
		pSemData->uiSemNum = pSemData->uiMaxSemNum;
	}
	else
	{
		pSemData->uiSemNum += in_uiRelSemNum;
	}

	pthread_cond_signal(&pSemData->cond_event);

	pthread_mutex_unlock(&pSemData->t_mutex_lock);
#endif
}

//等待信号量，返回TRUE表示有信号，FALSE表示无信号
NPC_THREAD_API		NPC_BOOL	NPC_F_THREAD_Sem_Wait(
	IN		NPC_HANDLE						in_hSem,				//信号量句柄
	IN		NPC_INT							in_iTimeout				//超时时长（毫秒）
	)
{
	PNPC_S_THREAD_SEM_DATA					pSemData;
	NPC_BOOL								bIfHasSig;

	//INIT AREA
	{
		pSemData = (PNPC_S_THREAD_SEM_DATA)in_hSem;
		if ( pSemData == NULL )
			return FALSE;

		bIfHasSig = FALSE;
	}

	//CODE AREA
	{
#ifdef _WIN32
	#ifdef WINAPI_FAMILY
		if ( pSemData->hSemaphore )
		{
			if ( WaitForSingleObjectEx(pSemData->hSemaphore, in_iTimeout, TRUE) == WAIT_OBJECT_0 )
			{
				bIfHasSig = TRUE;
			}
		}
	#else
		if ( pSemData->hSemaphore )
		{
			if ( WaitForSingleObject(pSemData->hSemaphore, in_iTimeout) == WAIT_OBJECT_0 )
			{
				bIfHasSig = TRUE;
			}
		}
	#endif
#else
		pthread_mutex_lock(&pSemData->t_mutex_lock);

		if ( pSemData->uiSemNum == 0 )
		{
			//条件变量等待
			NPC_F_THREAD_PR_CondTimeWait(
				&pSemData->cond_event,					//条件变量指针
				&pSemData->t_mutex_lock,				//互斥锁指针
				in_iTimeout								//超时时长（毫秒）
				);
		}

		if ( pSemData->uiSemNum > 0 )
		{
			bIfHasSig = TRUE;
			pSemData->uiSemNum--;
		}

		pthread_mutex_unlock(&pSemData->t_mutex_lock);
#endif
	}

	return bIfHasSig;
}


//--------------------------------------------------------------------------------
//创建等待对象
NPC_THREAD_API		NPC_HANDLE	NPC_F_THREAD_WaitObj_Create()
{
	NPC_BOOL								bRet;
	PNPC_S_THREAD_WAITOBJ_DATA				pWaitObjData;

	//INIT AREA
	{
		bRet = TRUE;
		pWaitObjData = NULL;
	}

	//CODE AREA
	{
		pWaitObjData = (PNPC_S_THREAD_WAITOBJ_DATA)malloc(sizeof(NPC_S_THREAD_WAITOBJ_DATA));
		if ( pWaitObjData == NULL )
		{
			bRet = FALSE;
			goto _NPC_CLEAR;
		}
		memset(pWaitObjData, 0, sizeof(NPC_S_THREAD_WAITOBJ_DATA));

#ifndef _WIN32
		//初始化互斥锁
		if ( pthread_mutex_init(&pWaitObjData->t_mutex_lock, NULL ) != 0 )
		{
			printf("NPC_F_THREAD_WaitObj_Create pthread_mutex_init fail.\n");
			return FALSE;
		}
		pWaitObjData->bIfInitMutex = TRUE;

		//初始化条件变量
		if ( pthread_cond_init(&pWaitObjData->cond_event, NULL) != 0 )
		{
			printf("NPC_F_THREAD_WaitObj_Create pthread_cond_init fail.\n");

			pthread_mutex_destroy(&pWaitObjData->t_mutex_lock);
			pWaitObjData->bIfInitMutex = FALSE;
			return FALSE;
		}
		pWaitObjData->bIfInitCond = TRUE;
#endif
	}

	//CLEAR AREA
	_NPC_CLEAR:
	{
		if ( !bRet && pWaitObjData )
		{
			//释放等待对象
			NPC_F_THREAD_WaitObj_Destroy(
				(NPC_HANDLE)pWaitObjData			//等待对象
				);
			pWaitObjData = NULL;
		}
	}

	return (NPC_HANDLE)pWaitObjData;
}

//释放等待对象
NPC_THREAD_API		NPC_VOID	NPC_F_THREAD_WaitObj_Destroy(
	IN		NPC_HANDLE						in_hWaitObj				//等待对象
	)
{
	PNPC_S_THREAD_WAITOBJ_DATA				pWaitObjData;

	pWaitObjData = (PNPC_S_THREAD_WAITOBJ_DATA)in_hWaitObj;
	if ( pWaitObjData == NULL )
		return;

#ifndef _WIN32
	if ( pWaitObjData->bIfInitMutex )
	{
		pthread_mutex_destroy(&pWaitObjData->t_mutex_lock);
		pWaitObjData->bIfInitMutex = FALSE;
	}

	if ( pWaitObjData->bIfInitCond )
	{
		pthread_cond_destroy(&pWaitObjData->cond_event);
		pWaitObjData->bIfInitCond = FALSE;
	}
#endif

	free(pWaitObjData);
	pWaitObjData = NULL;
}

//等待对象添加事件
NPC_THREAD_API		NPC_BOOL	NPC_F_THREAD_WaitObj_AddEvent(
	IN		NPC_HANDLE						in_hWaitObj,			//等待对象
	IN		NPC_HANDLE						in_hEvent				//事件句柄
	)
{
	PNPC_S_THREAD_WAITOBJ_DATA				pWaitObjData;

	pWaitObjData = (PNPC_S_THREAD_WAITOBJ_DATA)in_hWaitObj;
	if ( pWaitObjData == NULL )
		return FALSE;

	if ( pWaitObjData->iObjNum >= NPC_D_THREAD_MAX_WAIT_OBJ_NUM )
		return FALSE;

#ifdef _WIN32
	pWaitObjData->hWaitObjTable[pWaitObjData->iObjNum] = in_hEvent;
#else
	pWaitObjData->hWaitObjTable[pWaitObjData->iObjNum] = in_hEvent;
	pWaitObjData->bObjSigFlag[pWaitObjData->iObjNum] = FALSE;
#endif

	pWaitObjData->iObjNum++;

	return TRUE;
}

//等待对象删除事件
NPC_THREAD_API		NPC_BOOL	NPC_F_THREAD_WaitObj_DelEvent(
	IN		NPC_HANDLE						in_hWaitObj,			//等待对象
	IN		NPC_HANDLE						in_hEvent				//事件句柄
	)
{
	PNPC_S_THREAD_WAITOBJ_DATA				pWaitObjData;
	NPC_INT									i;

	pWaitObjData = (PNPC_S_THREAD_WAITOBJ_DATA)in_hWaitObj;
	if ( pWaitObjData == NULL )
		return FALSE;

#ifdef _WIN32
	for ( i=0; i<pWaitObjData->iObjNum; i++ )
	{
		if ( pWaitObjData->hWaitObjTable[i] == in_hEvent )
		{
			pWaitObjData->hWaitObjTable[i] = NULL;
		}
	}
#else
	for ( i=0; i<pWaitObjData->iObjNum; i++ )
	{
		if ( pWaitObjData->hWaitObjTable[i] == in_hEvent )
		{
			pWaitObjData->hWaitObjTable[i] = NULL;
		}
	}
#endif

	return TRUE;
}

//清除所有事件
NPC_THREAD_API		NPC_VOID	NPC_F_THREAD_WaitObj_ClearAllEvent(
	IN		NPC_HANDLE						in_hWaitObj				//等待对象
	)
{
	PNPC_S_THREAD_WAITOBJ_DATA				pWaitObjData;
	NPC_INT									i;

	pWaitObjData = (PNPC_S_THREAD_WAITOBJ_DATA)in_hWaitObj;
	if ( pWaitObjData == NULL )
		return ;

#ifdef _WIN32
	for ( i=0; i<pWaitObjData->iObjNum; i++ )
	{
		pWaitObjData->hWaitObjTable[i] = NULL;
	}
#else
	for ( i=0; i<pWaitObjData->iObjNum; i++ )
	{
		pWaitObjData->hWaitObjTable[i] = NULL;
	}
#endif
	pWaitObjData->iObjNum = 0;
}

#ifndef _WIN32
//设置等待对象中的事件有信号
NPC_THREAD_API		NPC_VOID	NPC_F_THREAD_WaitObj_SetEventSig(
	IN		NPC_HANDLE						in_hWaitObj,			//等待对象
	IN		NPC_HANDLE						in_hEvent				//事件句柄
	)
{
	PNPC_S_THREAD_WAITOBJ_DATA				pWaitObjData;
	NPC_BOOL								bIfFind;
	NPC_INT									i;

	pWaitObjData = (PNPC_S_THREAD_WAITOBJ_DATA)in_hWaitObj;
	if ( pWaitObjData == NULL )
		return ;

	pthread_mutex_lock(&pWaitObjData->t_mutex_lock);

	bIfFind = FALSE;
	for ( i=0; i<pWaitObjData->iObjNum; i++ )
	{
		if ( pWaitObjData->hWaitObjTable[i] && 
			pWaitObjData->hWaitObjTable[i] == in_hEvent )
		{
			pWaitObjData->bObjSigFlag[i] = TRUE;
			bIfFind = TRUE;
		}
	}

	if ( bIfFind )
	{
		pthread_cond_signal(&pWaitObjData->cond_event);
	}

	pthread_mutex_unlock(&pWaitObjData->t_mutex_lock);
}
#endif

//等待事件，返回有信号的索引号，返回-1表示超时，返回-2表示有错误，如果等待所有信号，则返回信号数
NPC_THREAD_API		NPC_INT		NPC_F_THREAD_WaitObj_WaitEvent(
	IN		NPC_HANDLE						in_hWaitObj,			//等待对象
	IN		NPC_BOOL						in_bIfWaitAll,			//是否等待所有有信号后才返回
	IN		NPC_INT							in_iTimeout				//超时时长（毫秒）
	)
{
	PNPC_S_THREAD_WAITOBJ_DATA				pWaitObjData;
	NPC_INT									iSigIndex;
	NPC_INT									i;

	//INIT AREA
	{
		pWaitObjData = (PNPC_S_THREAD_WAITOBJ_DATA)in_hWaitObj;
		if ( pWaitObjData == NULL )
			return -2;

		iSigIndex = -1;
	}

	//CODE AREA
	{
#ifdef _WIN32
		HANDLE hEventTable[NPC_D_THREAD_MAX_WAIT_OBJ_NUM];
		NPC_INT iEventIndex[NPC_D_THREAD_MAX_WAIT_OBJ_NUM];
		NPC_INT iEventNum;
		NPC_INT iWaitResult;

		iEventNum = 0;
		for ( i=0; i<NPC_D_THREAD_MAX_WAIT_OBJ_NUM; i++ )
		{
			if ( pWaitObjData->hWaitObjTable[i] )
			{
				//获WINDOWS事件句柄
				hEventTable[iEventNum] = NPC_F_THREAD_Event_GetWinEventHandle(
					pWaitObjData->hWaitObjTable[i]				//事件句柄
					);
				iEventIndex[iEventNum] = i;

				iEventNum++;
			}
		}

		if ( iEventNum > 0 )
		{
			iWaitResult = WaitForMultipleObjectsEx(
				iEventNum, 
				hEventTable, 
				in_bIfWaitAll, 
				in_iTimeout, 
				FALSE);

			if ( iWaitResult == WAIT_FAILED )
			{
				iSigIndex = -2;
				goto _NPC_CLEAR;
			}
			if ( iWaitResult == WAIT_TIMEOUT )
			{
				iSigIndex = -1;
				goto _NPC_CLEAR;
			}

			iWaitResult -= WAIT_OBJECT_0;

			if ( iWaitResult < 0 || iWaitResult >= iEventNum )
			{
				iSigIndex = -2;
				goto _NPC_CLEAR;
			}

			iSigIndex = iEventIndex[iWaitResult];
		}
#else
		NPC_BOOL							bIfHasNoSig;

		//查询事件是否有信号
		bIfHasNoSig = FALSE;
		for ( i=0; i<pWaitObjData->iObjNum; i++ )
		{
			if ( pWaitObjData->hWaitObjTable[i] )
			{
				pWaitObjData->bObjSigFlag[i] = FALSE;

				//查询事件是否有信号，如果没有，则置入等待对象
				if ( NPC_F_THREAD_Event_QuerySetWaitObj(
					pWaitObjData->hWaitObjTable[i],		//事件句柄
					in_hWaitObj							//等待对象句柄
					) )
				{
					if ( !in_bIfWaitAll )
					{
						iSigIndex = i;
						goto _NPC_CLEAR;
					}
				}
				else
				{
					bIfHasNoSig = TRUE;
				}
			}
		}
		if ( in_bIfWaitAll && !bIfHasNoSig )
		{
			iSigIndex = pWaitObjData->iObjNum;
			goto _NPC_CLEAR;
		}

		//-------------------------------------------------------------------------
		//等待信号
		pthread_mutex_lock(&pWaitObjData->t_mutex_lock);

		bIfHasNoSig = FALSE;
		iSigIndex = -1;
		for ( i=0; i<pWaitObjData->iObjNum; i++ )
		{
			if ( pWaitObjData->hWaitObjTable[i] )
			{
				if ( pWaitObjData->bObjSigFlag[i] )
				{
					if ( !in_bIfWaitAll )
					{
						iSigIndex = i;
						break;
					}
				}
				else
				{
					bIfHasNoSig = TRUE;
				}
			}
		}
		if ( in_bIfWaitAll && !bIfHasNoSig )
		{
			iSigIndex = pWaitObjData->iObjNum;
		}

		if ( iSigIndex < 0 )
		{
			//条件变量等待
			if ( NPC_F_THREAD_PR_CondTimeWait(
				&pWaitObjData->cond_event,				//条件变量指针
				&pWaitObjData->t_mutex_lock,			//互斥锁指针
				in_iTimeout								//超时时长（毫秒）
				) == 0 )
			{
				bIfHasNoSig = FALSE;
				iSigIndex = -1;
				for ( i=0; i<pWaitObjData->iObjNum; i++ )
				{
					if ( pWaitObjData->hWaitObjTable[i] )
					{
						if ( pWaitObjData->bObjSigFlag[i] )
						{
							if ( !in_bIfWaitAll )
							{
								iSigIndex = i;
								break;
							}
						}
						else
						{
							bIfHasNoSig = TRUE;
						}
					}
				}
				if ( in_bIfWaitAll && !bIfHasNoSig )
				{
					iSigIndex = pWaitObjData->iObjNum;
				}
			}
		}

		pthread_mutex_unlock(&pWaitObjData->t_mutex_lock);

		//-------------------------------------------------------------------------
		//删除事件的等待对象
		for ( i=0; i<pWaitObjData->iObjNum; i++ )
		{
			if ( pWaitObjData->hWaitObjTable[i] )
			{
				//事件清除等待对象
				NPC_F_THREAD_Event_ClearWaitObj(
					pWaitObjData->hWaitObjTable[i],		//事件句柄
					in_hWaitObj							//等待对象句柄
					);
			}
		}
#endif
	}

	//CLEAR AREA
	_NPC_CLEAR:
	{
	}

	return iSigIndex;
}


//-------------------------------------------------------------------------------------
#ifdef _WIN32
//获取错误信息
NPC_VOID	NPC_F_THREAD_PR_GetLastErrorInfo(
	IN		NPC_DWORD								in_dwErrorCode,				//错误码
	IN		PNPC_CHAR								in_pErrorBuf,				//错误信息缓冲区
	IN		NPC_INT									in_iErrorBufSize			//错误信息缓冲区大小
	)
{
#ifdef WINAPI_FAMILY
	sprintf(in_pErrorBuf, "error: %d", in_dwErrorCode);
#else
    LPVOID lpMsgBuf;

    FormatMessage(
        FORMAT_MESSAGE_ALLOCATE_BUFFER | 
        FORMAT_MESSAGE_FROM_SYSTEM,
        NULL,
        in_dwErrorCode, 
        MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
        (LPTSTR)&lpMsgBuf,
        0, 
		NULL);

	strncpy(in_pErrorBuf, (char*)lpMsgBuf, in_iErrorBufSize-1);
	in_pErrorBuf[in_iErrorBufSize-1] = 0;

    LocalFree(lpMsgBuf);
#endif
}
#endif

