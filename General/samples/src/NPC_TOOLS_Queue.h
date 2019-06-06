/*----------------------------------------------------------------------------
 * Copyright(c)  :  XXXX CORPORTAION  All Rights Reserved                       
 * FileName      :  NPC_TOOLS_Queue.h
 * Version       :  1.1
 * Author        :  CCH
 * DateTime      :  2010-05-26
 * Description   :  NPC系统队列工具
 *----------------------------------------------------------------------------*/

#ifndef __NPC_TOOLS_QUEUE_H
#define __NPC_TOOLS_QUEUE_H

#include "NPC_TypeDef.h"


//--------------------------------------------------------------------------------
//导入导出定义
#ifdef _WIN32
	#ifdef _NPC_QUEUE_EXPORTS
		#define NPC_QUEUE_API __declspec(dllexport)
	#else
		#ifdef _NPC_QUEUE_IMPORT
			#define NPC_QUEUE_API __declspec(dllimport)
		#else
			#define NPC_QUEUE_API
		#endif
	#endif
#else
	#define NPC_QUEUE_API
#endif


//---------------------------------------------------------------------
#ifdef _WIN32
// C++ support
#ifdef __cplusplus
extern "C"
{
#endif
#endif


//-----------------------------------------------------------------------------
//初始化列队
NPC_QUEUE_API	NPC_BOOL	NPC_F_TOOLS_QUEUE_InitQueue(
	IN		NPC_HANDLE*		in_hQueue,			//队列句柄
	IN		NPC_INT			in_iQueueSize=10000	//队列大小
	);

//释放队列
NPC_QUEUE_API	NPC_VOID	NPC_F_TOOLS_QUEUE_FreeQueue(
	IN		NPC_HANDLE*		in_hQueue		//队列句柄
	);

//-----------------------------------------------------------------------------------
//设置数据到队列
NPC_QUEUE_API	NPC_BOOL	NPC_F_TOOLS_QUEUE_SetData(
	IN		NPC_HANDLE		in_hQueue,		//队列句柄
	IN		PNPC_BYTE		in_pData,		//数据指针
	IN		NPC_INT			in_iDataLen,	//数据长度
	IN		NPC_INT			in_iTimeout		//超时时长（毫秒）
	);

//从队列获取数据
NPC_QUEUE_API	NPC_BOOL	NPC_F_TOOLS_QUEUE_GetData(
	IN		NPC_HANDLE		in_hQueue,		//队列句柄
	OUT		PNPC_BYTE		out_pData,		//输出的数据指针，该缓冲区由调用者分配，缓冲大小必须大于队列中的数据长度，否则调用失败
	OUT		PNPC_INT		out_pDataLen,	//输出的数据长度，传入缓冲区大小，传出数据长度
	IN		NPC_INT			in_iTimeout		//超时时长（毫秒）
	);

//从队列获取数据，由该函数分配内存
NPC_QUEUE_API	NPC_BOOL	NPC_F_TOOLS_QUEUE_GetDataEx(
	IN		NPC_HANDLE		in_hQueue,		//队列句柄
	OUT		PNPC_BYTE*		out_ppData,		//输出的数据指针，由队列分配内存，内存释放由调用者调用NPC_F_TOOLS_QUEUE_FreeMemory释放
	OUT		PNPC_INT		out_pDataLen,	//输出的数据长度
	OUT		PNPC_INT		out_pBufSize,	//缓冲区大小，如果队列中的数据大于该缓冲区大小，则队列重新分配内存，释放之前的内存，并把内存大小通过该参数传出
	IN		NPC_INT			in_iTimeout		//超时时长（毫秒）
	);

//从队列中移除一个数据
NPC_QUEUE_API	NPC_BOOL	NPC_F_TOOLS_QUEUE_MoveData(
	IN		NPC_HANDLE		in_hQueue		//队列句柄
	);

//--------------------------------------------------------------------------------
//获取队列项数
NPC_QUEUE_API	NPC_INT		NPC_F_TOOLS_QUEUE_GetCount(
	IN		NPC_HANDLE		in_hQueue		//队列句柄
	);

//获取队列读事件句柄
NPC_QUEUE_API	HANDLE		NPC_F_TOOLS_QUEUE_GetReadEventHandle(
	IN		NPC_HANDLE		in_hQueue		//队列句柄
	);

//获取队列写事件句柄
NPC_QUEUE_API	HANDLE		NPC_F_TOOLS_QUEUE_GetWriteEventHandle(
	IN		NPC_HANDLE		in_hQueue		//队列句柄
	);

#ifdef _WIN32
//获取队列读事件句柄
NPC_QUEUE_API	HANDLE		NPC_F_TOOLS_QUEUE_GetWinReadEventHandle(
	IN		NPC_HANDLE		in_hQueue		//队列句柄
	);

//获取队列写事件句柄
NPC_QUEUE_API	HANDLE		NPC_F_TOOLS_QUEUE_GetWinWriteEventHandle(
	IN		NPC_HANDLE		in_hQueue		//队列句柄
	);

//设置消息通知的窗口句柄
NPC_QUEUE_API	NPC_VOID	NPC_F_TOOLS_QUEUE_SetMsgWnd(
	IN		NPC_HANDLE		in_hQueue,		//队列句柄
	IN		NPC_HWND		in_hWnd,		//传入窗口句柄
	IN		NPC_UINT		in_uMsg			//传入消息ID
	);
#endif

//设置最大存储数量
NPC_QUEUE_API	NPC_VOID	NPC_F_TOOLS_QUEUE_SetMaxStoreDataNum(
	IN		NPC_HANDLE		in_hQueue,		//队列句柄
	IN		NPC_INT			in_iNum			//最大存储量
	);

//释放内存，该内存由DLL分配的
NPC_QUEUE_API	NPC_VOID	NPC_F_TOOLS_QUEUE_FreeMemory(
	IN		PNPC_BYTE*		in_ppDataPos	//数据指针
	);

//查询队列数据累计大小（字节）
NPC_QUEUE_API	NPC_DWORD	NPC_F_TOOLS_QUEUE_QueryQueueDataTotalLen(
	IN		NPC_HANDLE		in_hQueue		//队列句柄
	);


#ifdef _WIN32
// C++ support
#ifdef __cplusplus
}
#endif
#endif


#endif
