/*----------------------------------------------------------------------------
 * Copyright(c)  :  NPC CORPORTAION All Rights Reserved                       
 * FileName      :  NPC_UMSP_McServer.h
 * Version       :  1.0
 * Author        :  CCH
 * DateTime      :  2013-08-10
 * Description   :  UMSP服务器主控模块
 *----------------------------------------------------------------------------*/

#ifndef __NPC_UMSP_MCSERVER_H
#define __NPC_UMSP_MCSERVER_H

#include "NPC_TypeDef.h"
#include "NPC_UMSP_DataDef.h"


//-------------------------------------------------------------------------------
//启动主控服务
NPC_BOOL	NPC_F_UMSP_MC_StartMcServer(
	IN		PNPC_S_UMSP_MCSRV_DATA					in_pMcSrvData				//主控服务数据
	);

//停止主控服务
NPC_VOID	NPC_F_UMSP_MC_StopMcServer(
	IN		PNPC_S_UMSP_MCSRV_DATA					in_pMcSrvData				//主控服务数据
	);

//初始化数据
NPC_BOOL	NPC_F_UMSP_MC_InitData(
	IN		PNPC_S_UMSP_MCSRV_DATA					in_pMcSrvData				//主控服务数据
	);

//反初始化数据
NPC_VOID	NPC_F_UMSP_MC_UninitData(
	IN		PNPC_S_UMSP_MCSRV_DATA					in_pMcSrvData				//主控服务数据
	);

//读取配置
NPC_BOOL	NPC_F_UMSP_MC_ReadConfig(
	IN		PNPC_S_UMSP_MCSRV_DATA					in_pMcSrvData				//主控服务数据
	);

//读取动态配置
NPC_BOOL	NPC_F_UMSP_MC_ReadDynamicConfig(
	IN		PNPC_S_UMSP_MCSRV_DATA					in_pMcSrvData				//主控服务数据
	);


//-------------------------------------------------------------------------------
//启动线程
NPC_BOOL	NPC_F_UMSP_MC_StartThread(
	IN		PNPC_S_UMSP_MCSRV_DATA					in_pMcSrvData				//主控服务数据
	);

//退出线程
NPC_VOID	NPC_F_UMSP_MC_QuitThread(
	IN		PNPC_S_UMSP_MCSRV_DATA					in_pMcSrvData				//主控服务数据
	);

//工作线程
NPC_VOID	NPC_F_UMSP_MC_ThreadWork(
	IN		PNPC_VOID								in_pParameter				//参数指针
	);

//处理定时器
NPC_VOID	NPC_F_UMSP_MC_DoTimer(
	IN		PNPC_S_UMSP_MCSRV_DATA					in_pMcSrvData				//主控服务数据
	);


//-------------------------------------------------------------------------------
//处理协议事件：打开媒体流
NPC_VOID	NPC_F_UMSP_MC_DoProEvent_EVENT_E2_OPEN_MEDIA_STREAM(
	IN		PNPC_S_UMSP_MCSRV_DATA					in_pMcSrvData,				//主控服务数据
	IN		PNPC_S_DPS_MSG_HEAD						in_pMsgHead,				//消息头
	IN		PNPC_BYTE								in_pMsgDataBuf,				//消息数据缓冲区，包括NPC_S_DPS_MSG_HEAD
	IN		NPC_INT									in_iMsgDataLen				//消息数据长度
	);

//处理协议事件：关闭媒体流
NPC_VOID	NPC_F_UMSP_MC_DoProEvent_EVENT_E2_CLOSE_MEDIA_STREAM(
	IN		PNPC_S_UMSP_MCSRV_DATA					in_pMcSrvData,				//主控服务数据
	IN		PNPC_S_DPS_MSG_HEAD						in_pMsgHead,				//消息头
	IN		PNPC_BYTE								in_pMsgDataBuf,				//消息数据缓冲区，包括NPC_S_DPS_MSG_HEAD
	IN		NPC_INT									in_iMsgDataLen				//消息数据长度
	);

//处理协议事件：云台控制
NPC_VOID	NPC_F_UMSP_MC_DoProEvent_EVENT_E2_PTZ_CTRL(
	IN		PNPC_S_UMSP_MCSRV_DATA					in_pMcSrvData,				//主控服务数据
	IN		PNPC_S_DPS_MSG_HEAD						in_pMsgHead,				//消息头
	IN		PNPC_BYTE								in_pMsgDataBuf,				//消息数据缓冲区，包括NPC_S_DPS_MSG_HEAD
	IN		NPC_INT									in_iMsgDataLen				//消息数据长度
	);

//处理协议事件：请求强制发送I帧
NPC_VOID	NPC_F_UMSP_MC_DoProEvent_EVENT_E2_FORCE_I_FRAME(
	IN		PNPC_S_UMSP_MCSRV_DATA					in_pMcSrvData,				//主控服务数据
	IN		PNPC_S_DPS_MSG_HEAD						in_pMsgHead,				//消息头
	IN		PNPC_BYTE								in_pMsgDataBuf,				//消息数据缓冲区，包括NPC_S_DPS_MSG_HEAD
	IN		NPC_INT									in_iMsgDataLen				//消息数据长度
	);


//处理协议事件：获取视频质量等级能力列表
NPC_VOID	NPC_F_UMSP_MC_DoProEvent_EVENT_E2_GET_VIDEO_QUALITY_CAP(
	IN		PNPC_S_UMSP_MCSRV_DATA					in_pMcSrvData,				//主控服务数据
	IN		PNPC_S_DPS_MSG_HEAD						in_pMsgHead,				//消息头
	IN		PNPC_BYTE								in_pMsgDataBuf,				//消息数据缓冲区，包括NPC_S_DPS_MSG_HEAD
	IN		NPC_INT									in_iMsgDataLen				//消息数据长度
	);

//处理协议事件：获取视频质量等级
NPC_VOID	NPC_F_UMSP_MC_DoProEvent_EVENT_E2_GET_VIDEO_QUALITY_LEVEL(
	IN		PNPC_S_UMSP_MCSRV_DATA					in_pMcSrvData,				//主控服务数据
	IN		PNPC_S_DPS_MSG_HEAD						in_pMsgHead,				//消息头
	IN		PNPC_BYTE								in_pMsgDataBuf,				//消息数据缓冲区，包括NPC_S_DPS_MSG_HEAD
	IN		NPC_INT									in_iMsgDataLen				//消息数据长度
	);

//处理协议事件：设置视频质量等级
NPC_VOID	NPC_F_UMSP_MC_DoProEvent_EVENT_E2_SET_VIDEO_QUALITY_LEVEL(
	IN		PNPC_S_UMSP_MCSRV_DATA					in_pMcSrvData,				//主控服务数据
	IN		PNPC_S_DPS_MSG_HEAD						in_pMsgHead,				//消息头
	IN		PNPC_BYTE								in_pMsgDataBuf,				//消息数据缓冲区，包括NPC_S_DPS_MSG_HEAD
	IN		NPC_INT									in_iMsgDataLen				//消息数据长度
	);


//处理协议事件：查询文件（录像或图片文件）
NPC_VOID	NPC_F_UMSP_MC_DoProEvent_EVENT_E2_FILE_QUERY(
	IN		PNPC_S_UMSP_MCSRV_DATA					in_pMcSrvData,				//主控服务数据
	IN		PNPC_S_DPS_MSG_HEAD						in_pMsgHead,				//消息头
	IN		PNPC_BYTE								in_pMsgDataBuf,				//消息数据缓冲区，包括NPC_S_DPS_MSG_HEAD
	IN		NPC_INT									in_iMsgDataLen				//消息数据长度
	);

//处理协议事件：扩展查询文件（录像或图片文件）
NPC_VOID	NPC_F_UMSP_MC_DoProEvent_EVENT_E2_NSP_FILE_QUERY(
	IN		PNPC_S_UMSP_MCSRV_DATA					in_pMcSrvData,				//主控服务数据
	IN		PNPC_S_DPS_MSG_HEAD						in_pMsgHead,				//消息头
	IN		PNPC_BYTE								in_pMsgDataBuf,				//消息数据缓冲区，包括NPC_S_DPS_MSG_HEAD
	IN		NPC_INT									in_iMsgDataLen				//消息数据长度
	);

//处理协议事件：查询录像文件时间范围
NPC_VOID	NPC_F_UMSP_MC_DoProEvent_EVENT_E2_FILE_RANGE_QUERY(
	IN		PNPC_S_UMSP_MCSRV_DATA					in_pMcSrvData,				//主控服务数据
	IN		PNPC_S_DPS_MSG_HEAD						in_pMsgHead,				//消息头
	IN		PNPC_BYTE								in_pMsgDataBuf,				//消息数据缓冲区，包括NPC_S_DPS_MSG_HEAD
	IN		NPC_INT									in_iMsgDataLen				//消息数据长度
	);

//处理协议事件：打开文件
NPC_VOID	NPC_F_UMSP_MC_DoProEvent_EVENT_E2_FILE_OPEN(
	IN		PNPC_S_UMSP_MCSRV_DATA					in_pMcSrvData,				//主控服务数据
	IN		PNPC_S_DPS_MSG_HEAD						in_pMsgHead,				//消息头
	IN		PNPC_BYTE								in_pMsgDataBuf,				//消息数据缓冲区，包括NPC_S_DPS_MSG_HEAD
	IN		NPC_INT									in_iMsgDataLen				//消息数据长度
	);

//处理协议事件：打开文件,按时间方式
NPC_VOID	NPC_F_UMSP_MC_DoProEvent_EVENT_E2_FILE_OPEN_BY_TIME(
	IN		PNPC_S_UMSP_MCSRV_DATA					in_pMcSrvData,				//主控服务数据
	IN		PNPC_S_DPS_MSG_HEAD						in_pMsgHead,				//消息头
	IN		PNPC_BYTE								in_pMsgDataBuf,				//消息数据缓冲区，包括NPC_S_DPS_MSG_HEAD
	IN		NPC_INT									in_iMsgDataLen				//消息数据长度
	);

//处理协议事件：关闭文件
NPC_VOID	NPC_F_UMSP_MC_DoProEvent_EVENT_E2_FILE_CLOSE(
	IN		PNPC_S_UMSP_MCSRV_DATA					in_pMcSrvData,				//主控服务数据
	IN		PNPC_S_DPS_MSG_HEAD						in_pMsgHead,				//消息头
	IN		PNPC_BYTE								in_pMsgDataBuf,				//消息数据缓冲区，包括NPC_S_DPS_MSG_HEAD
	IN		NPC_INT									in_iMsgDataLen				//消息数据长度
	);

//处理协议事件：读取文件
NPC_VOID	NPC_F_UMSP_MC_DoProEvent_EVENT_E2_FILE_READ(
	IN		PNPC_S_UMSP_MCSRV_DATA					in_pMcSrvData,				//主控服务数据
	IN		PNPC_S_DPS_MSG_HEAD						in_pMsgHead,				//消息头
	IN		PNPC_BYTE								in_pMsgDataBuf,				//消息数据缓冲区，包括NPC_S_DPS_MSG_HEAD
	IN		NPC_INT									in_iMsgDataLen				//消息数据长度
	);

//处理协议事件：定位文件
NPC_VOID	NPC_F_UMSP_MC_DoProEvent_EVENT_E2_FILE_SEEK(
	IN		PNPC_S_UMSP_MCSRV_DATA					in_pMcSrvData,				//主控服务数据
	IN		PNPC_S_DPS_MSG_HEAD						in_pMsgHead,				//消息头
	IN		PNPC_BYTE								in_pMsgDataBuf,				//消息数据缓冲区，包括NPC_S_DPS_MSG_HEAD
	IN		NPC_INT									in_iMsgDataLen				//消息数据长度
	);

//处理协议事件：定位文件
NPC_VOID	NPC_F_UMSP_MC_DoProEvent_EVENT_E2_FILE_SEEK_BY_TIME(
	IN		PNPC_S_UMSP_MCSRV_DATA					in_pMcSrvData,				//主控服务数据
	IN		PNPC_S_DPS_MSG_HEAD						in_pMsgHead,				//消息头
	IN		PNPC_BYTE								in_pMsgDataBuf,				//消息数据缓冲区，包括NPC_S_DPS_MSG_HEAD
	IN		NPC_INT									in_iMsgDataLen				//消息数据长度
	);

//处理协议事件：查询视频参数
NPC_VOID	NPC_F_UMSP_MC_DoProEvent_EVENT_E2_FILE_QVIDEO(
	IN		PNPC_S_UMSP_MCSRV_DATA					in_pMcSrvData,				//主控服务数据
	IN		PNPC_S_DPS_MSG_HEAD						in_pMsgHead,				//消息头
	IN		PNPC_BYTE								in_pMsgDataBuf,				//消息数据缓冲区，包括NPC_S_DPS_MSG_HEAD
	IN		NPC_INT									in_iMsgDataLen				//消息数据长度
	);

//处理协议事件：请求I帧数据
NPC_VOID	NPC_F_UMSP_MC_DoProEvent_EVENT_E2_FILE_ONLY_IFRAME(
	IN		PNPC_S_UMSP_MCSRV_DATA					in_pMcSrvData,				//主控服务数据
	IN		PNPC_S_DPS_MSG_HEAD						in_pMsgHead,				//消息头
	IN		PNPC_BYTE								in_pMsgDataBuf,				//消息数据缓冲区，包括NPC_S_DPS_MSG_HEAD
	IN		NPC_INT									in_iMsgDataLen				//消息数据长度
	);

//处理协议事件：请求恢复全帧数据
NPC_VOID	NPC_F_UMSP_MC_DoProEvent_EVENT_E2_FILE_RECOVER_ALLFRAME(
	IN		PNPC_S_UMSP_MCSRV_DATA					in_pMcSrvData,				//主控服务数据
	IN		PNPC_S_DPS_MSG_HEAD						in_pMsgHead,				//消息头
	IN		PNPC_BYTE								in_pMsgDataBuf,				//消息数据缓冲区，包括NPC_S_DPS_MSG_HEAD
	IN		NPC_INT									in_iMsgDataLen				//消息数据长度
	);


//处理协议事件：开始语音对讲
NPC_VOID	NPC_F_UMSP_MC_DoProEvent_EVENT_E2_TALK_START(
	IN		PNPC_S_UMSP_MCSRV_DATA					in_pMcSrvData,				//主控服务数据
	IN		PNPC_S_DPS_MSG_HEAD						in_pMsgHead,				//消息头
	IN		PNPC_BYTE								in_pMsgDataBuf,				//消息数据缓冲区，包括NPC_S_DPS_MSG_HEAD
	IN		NPC_INT									in_iMsgDataLen				//消息数据长度
	);

//处理协议事件：停止语音对讲
NPC_VOID	NPC_F_UMSP_MC_DoProEvent_EVENT_E2_TALK_STOP(
	IN		PNPC_S_UMSP_MCSRV_DATA					in_pMcSrvData,				//主控服务数据
	IN		PNPC_S_DPS_MSG_HEAD						in_pMsgHead,				//消息头
	IN		PNPC_BYTE								in_pMsgDataBuf,				//消息数据缓冲区，包括NPC_S_DPS_MSG_HEAD
	IN		NPC_INT									in_iMsgDataLen				//消息数据长度
	);

//处理协议事件：布防
NPC_VOID	NPC_F_UMSP_MC_DoProEvent_EVENT_E2_GUARD(
	IN		PNPC_S_UMSP_MCSRV_DATA					in_pMcSrvData,				//主控服务数据
	IN		PNPC_S_DPS_MSG_HEAD						in_pMsgHead,				//消息头
	IN		PNPC_BYTE								in_pMsgDataBuf,				//消息数据缓冲区，包括NPC_S_DPS_MSG_HEAD
	IN		NPC_INT									in_iMsgDataLen				//消息数据长度
	);

//处理协议事件：撤防
NPC_VOID	NPC_F_UMSP_MC_DoProEvent_EVENT_E2_UNGUARD(
	IN		PNPC_S_UMSP_MCSRV_DATA					in_pMcSrvData,				//主控服务数据
	IN		PNPC_S_DPS_MSG_HEAD						in_pMsgHead,				//消息头
	IN		PNPC_BYTE								in_pMsgDataBuf,				//消息数据缓冲区，包括NPC_S_DPS_MSG_HEAD
	IN		NPC_INT									in_iMsgDataLen				//消息数据长度
	);

//处理协议事件：修改用户密码
NPC_VOID	NPC_F_UMSP_MC_DoProEvent_EVENT_E3_MODIFY_USER_PWD(
	IN		PNPC_S_UMSP_MCSRV_DATA					in_pMcSrvData,				//主控服务数据
	IN		PNPC_S_DPS_MSG_HEAD						in_pMsgHead,				//消息头
	IN		PNPC_BYTE								in_pMsgDataBuf,				//消息数据缓冲区，包括NPC_S_DPS_MSG_HEAD
	IN		NPC_INT									in_iMsgDataLen				//消息数据长度
	);

//处理协议事件：获取配置参数
NPC_VOID	NPC_F_UMSP_MC_DoProEvent_EVENT_E3_GET_CONFIG(
	IN		PNPC_S_UMSP_MCSRV_DATA					in_pMcSrvData,				//主控服务数据
	IN		PNPC_S_DPS_MSG_HEAD						in_pMsgHead,				//消息头
	IN		PNPC_BYTE								in_pMsgDataBuf,				//消息数据缓冲区，包括NPC_S_DPS_MSG_HEAD
	IN		NPC_INT									in_iMsgDataLen				//消息数据长度
	);

//处理协议事件：设置配置参数
NPC_VOID	NPC_F_UMSP_MC_DoProEvent_EVENT_E3_SET_CONFIG(
	IN		PNPC_S_UMSP_MCSRV_DATA					in_pMcSrvData,				//主控服务数据
	IN		PNPC_S_DPS_MSG_HEAD						in_pMsgHead,				//消息头
	IN		PNPC_BYTE								in_pMsgDataBuf,				//消息数据缓冲区，包括NPC_S_DPS_MSG_HEAD
	IN		NPC_INT									in_iMsgDataLen				//消息数据长度
	);

//处理协议事件：获取缺省配置参数
NPC_VOID	NPC_F_UMSP_MC_DoProEvent_EVENT_E3_GET_DEF_CONFIG(
	IN		PNPC_S_UMSP_MCSRV_DATA					in_pMcSrvData,				//主控服务数据
	IN		PNPC_S_DPS_MSG_HEAD						in_pMsgHead,				//消息头
	IN		PNPC_BYTE								in_pMsgDataBuf,				//消息数据缓冲区，包括NPC_S_DPS_MSG_HEAD
	IN		NPC_INT									in_iMsgDataLen				//消息数据长度
	);

//处理协议事件：导入配置
NPC_VOID	NPC_F_UMSP_MC_DoProEvent_EVENT_E3_IEC_IMP_CONFIG(
	IN		PNPC_S_UMSP_MCSRV_DATA					in_pMcSrvData,				//主控服务数据
	IN		PNPC_S_DPS_MSG_HEAD						in_pMsgHead,				//消息头
	IN		PNPC_BYTE								in_pMsgDataBuf,				//消息数据缓冲区，包括NPC_S_DPS_MSG_HEAD
	IN		NPC_INT									in_iMsgDataLen				//消息数据长度
	);

//处理协议事件：导出配置
NPC_VOID	NPC_F_UMSP_MC_DoProEvent_EVENT_E3_IEC_EXP_CONFIG(
	IN		PNPC_S_UMSP_MCSRV_DATA					in_pMcSrvData,				//主控服务数据
	IN		PNPC_S_DPS_MSG_HEAD						in_pMsgHead,				//消息头
	IN		PNPC_BYTE								in_pMsgDataBuf,				//消息数据缓冲区，包括NPC_S_DPS_MSG_HEAD
	IN		NPC_INT									in_iMsgDataLen				//消息数据长度
	);



//处理协议事件：查询网络信息
NPC_VOID	NPC_F_UMSP_MC_DoProEvent_EVENT_E3_QUERY_NET_INFO(
	IN		PNPC_S_UMSP_MCSRV_DATA					in_pMcSrvData,				//主控服务数据
	IN		PNPC_S_DPS_MSG_HEAD						in_pMsgHead,				//消息头
	IN		PNPC_BYTE								in_pMsgDataBuf,				//消息数据缓冲区，包括NPC_S_DPS_MSG_HEAD
	IN		NPC_INT									in_iMsgDataLen				//消息数据长度
	);

//处理协议事件：修改网络地址
NPC_VOID	NPC_F_UMSP_MC_DoProEvent_EVENT_E3_MODIFY_NET_ADDR(
	IN		PNPC_S_UMSP_MCSRV_DATA					in_pMcSrvData,				//主控服务数据
	IN		PNPC_S_DPS_MSG_HEAD						in_pMsgHead,				//消息头
	IN		PNPC_BYTE								in_pMsgDataBuf,				//消息数据缓冲区，包括NPC_S_DPS_MSG_HEAD
	IN		NPC_INT									in_iMsgDataLen				//消息数据长度
	);


//-----------------------------------------------------------------------------------
//打开串口
NPC_VOID	NPC_F_UMSP_MC_DoProEvent_EVENT_E3_COM_OPEN(
	IN		PNPC_S_UMSP_MCSRV_DATA					in_pMcSrvData,				//主控服务数据
	IN		PNPC_S_DPS_MSG_HEAD						in_pMsgHead,				//消息头
	IN		PNPC_BYTE								in_pMsgDataBuf,				//消息数据缓冲区，包括NPC_S_DPS_MSG_HEAD
	IN		NPC_INT									in_iMsgDataLen				//消息数据长度
	);

//关闭串口
NPC_VOID	NPC_F_UMSP_MC_DoProEvent_EVENT_E3_COM_CLOSE(
	IN		PNPC_S_UMSP_MCSRV_DATA					in_pMcSrvData,				//主控服务数据
	IN		PNPC_S_DPS_MSG_HEAD						in_pMsgHead,				//消息头
	IN		PNPC_BYTE								in_pMsgDataBuf,				//消息数据缓冲区，包括NPC_S_DPS_MSG_HEAD
	IN		NPC_INT									in_iMsgDataLen				//消息数据长度
	);

//发送数据(客户端发送给设备端)
NPC_VOID	NPC_F_UMSP_MC_DoProEvent_EVENT_E3_COM_SEND_DATA(
	IN		PNPC_S_UMSP_MCSRV_DATA					in_pMcSrvData,				//主控服务数据
	IN		PNPC_S_DPS_MSG_HEAD						in_pMsgHead,				//消息头
	IN		PNPC_BYTE								in_pMsgDataBuf,				//消息数据缓冲区，包括NPC_S_DPS_MSG_HEAD
	IN		NPC_INT									in_iMsgDataLen				//消息数据长度
	);

//设备串口数据(设备端发送给客户端)(无应答)
NPC_VOID	NPC_F_UMSP_MC_DoProEvent_EVENT_E3_COM_SUBS_DATA(
	IN		PNPC_S_UMSP_MCSRV_DATA					in_pMcSrvData,				//主控服务数据
	IN		PNPC_S_DPS_MSG_HEAD						in_pMsgHead,				//消息头
	IN		PNPC_BYTE								in_pMsgDataBuf,				//消息数据缓冲区，包括NPC_S_DPS_MSG_HEAD
	IN		NPC_INT									in_iMsgDataLen				//消息数据长度
	);


//-----------------------------------------------------------------------------------
//增加用户
NPC_VOID	NPC_F_UMSP_MC_DoProEvent_EVENT_E3_UNS_ADD_USER(
	IN		PNPC_S_UMSP_MCSRV_DATA					in_pMcSrvData,				//主控服务数据
	IN		PNPC_S_DPS_MSG_HEAD						in_pMsgHead,				//消息头
	IN		PNPC_BYTE								in_pMsgDataBuf,				//消息数据缓冲区，包括NPC_S_DPS_MSG_HEAD
	IN		NPC_INT									in_iMsgDataLen				//消息数据长度
	);

//删除用户
NPC_VOID	NPC_F_UMSP_MC_DoProEvent_EVENT_E3_UNS_DEL_USER(
	IN		PNPC_S_UMSP_MCSRV_DATA					in_pMcSrvData,				//主控服务数据
	IN		PNPC_S_DPS_MSG_HEAD						in_pMsgHead,				//消息头
	IN		PNPC_BYTE								in_pMsgDataBuf,				//消息数据缓冲区，包括NPC_S_DPS_MSG_HEAD
	IN		NPC_INT									in_iMsgDataLen				//消息数据长度
	);

//获取用户权限列表
NPC_VOID	NPC_F_UMSP_MC_DoProEvent_EVENT_E3_UNS_GET_USER_POP(
	IN		PNPC_S_UMSP_MCSRV_DATA					in_pMcSrvData,				//主控服务数据
	IN		PNPC_S_DPS_MSG_HEAD						in_pMsgHead,				//消息头
	IN		PNPC_BYTE								in_pMsgDataBuf,				//消息数据缓冲区，包括NPC_S_DPS_MSG_HEAD
	IN		NPC_INT									in_iMsgDataLen				//消息数据长度
	);

//设置用户权限列表
NPC_VOID	NPC_F_UMSP_MC_DoProEvent_EVENT_E3_UNS_SET_USER_POP(
	IN		PNPC_S_UMSP_MCSRV_DATA					in_pMcSrvData,				//主控服务数据
	IN		PNPC_S_DPS_MSG_HEAD						in_pMsgHead,				//消息头
	IN		PNPC_BYTE								in_pMsgDataBuf,				//消息数据缓冲区，包括NPC_S_DPS_MSG_HEAD
	IN		NPC_INT									in_iMsgDataLen				//消息数据长度
	);


//-----------------------------------------------------------------------------------
//定制功能
NPC_VOID	NPC_F_UMSP_MC_DoProEvent_EVENT_E7_CUSTOM(
	IN		PNPC_S_UMSP_MCSRV_DATA					in_pMcSrvData,				//主控服务数据
	IN		PNPC_S_DPS_MSG_HEAD						in_pMsgHead,				//消息头
	IN		PNPC_BYTE								in_pMsgDataBuf,				//消息数据缓冲区，包括NPC_S_DPS_MSG_HEAD
	IN		NPC_INT									in_iMsgDataLen				//消息数据长度
	);


//-----------------------------------------------------------------------------------
//查询能力集
NPC_VOID	NPC_F_UMSP_MC_DoProEvent_EVENT_E7_RMY_QUERY_ABILITY(
	IN		PNPC_S_UMSP_MCSRV_DATA					in_pMcSrvData,				//主控服务数据
	IN		PNPC_S_DPS_MSG_HEAD						in_pMsgHead,				//消息头
	IN		PNPC_BYTE								in_pMsgDataBuf,				//消息数据缓冲区，包括NPC_S_DPS_MSG_HEAD
	IN		NPC_INT									in_iMsgDataLen				//消息数据长度
	);

//重启设备(无包体)(无应答消息)
NPC_VOID	NPC_F_UMSP_MC_DoProEvent_EVENT_E7_RMY_REBOOT(
	IN		PNPC_S_UMSP_MCSRV_DATA					in_pMcSrvData,				//主控服务数据
	IN		PNPC_S_DPS_MSG_HEAD						in_pMsgHead,				//消息头
	IN		PNPC_BYTE								in_pMsgDataBuf,				//消息数据缓冲区，包括NPC_S_DPS_MSG_HEAD
	IN		NPC_INT									in_iMsgDataLen				//消息数据长度
	);

//停止设备(无包体)(无应答消息)
NPC_VOID	NPC_F_UMSP_MC_DoProEvent_EVENT_E7_RMY_STOP(
	IN		PNPC_S_UMSP_MCSRV_DATA					in_pMcSrvData,				//主控服务数据
	IN		PNPC_S_DPS_MSG_HEAD						in_pMsgHead,				//消息头
	IN		PNPC_BYTE								in_pMsgDataBuf,				//消息数据缓冲区，包括NPC_S_DPS_MSG_HEAD
	IN		NPC_INT									in_iMsgDataLen				//消息数据长度
	);



//-----------------------------------------------------------------------------------
//查询设备版本信息
NPC_VOID	NPC_F_UMSP_MC_DoProEvent_EVENT_E7_DUP_QUERY_DEV_VER_INFO(
	IN		PNPC_S_UMSP_MCSRV_DATA					in_pMcSrvData,				//主控服务数据
	IN		PNPC_S_DPS_MSG_HEAD						in_pMsgHead,				//消息头
	IN		PNPC_BYTE								in_pMsgDataBuf,				//消息数据缓冲区，包括NPC_S_DPS_MSG_HEAD
	IN		NPC_INT									in_iMsgDataLen				//消息数据长度
	);

//请求升级
NPC_VOID	NPC_F_UMSP_MC_DoProEvent_EVENT_E7_DUP_REQU_UPGRADE(
	IN		PNPC_S_UMSP_MCSRV_DATA					in_pMcSrvData,				//主控服务数据
	IN		PNPC_S_DPS_MSG_HEAD						in_pMsgHead,				//消息头
	IN		PNPC_BYTE								in_pMsgDataBuf,				//消息数据缓冲区，包括NPC_S_DPS_MSG_HEAD
	IN		NPC_INT									in_iMsgDataLen				//消息数据长度
	);

//查询升级进度
NPC_VOID	NPC_F_UMSP_MC_DoProEvent_EVENT_E7_DUP_QUERY_PROGRESS(
	IN		PNPC_S_UMSP_MCSRV_DATA					in_pMcSrvData,				//主控服务数据
	IN		PNPC_S_DPS_MSG_HEAD						in_pMsgHead,				//消息头
	IN		PNPC_BYTE								in_pMsgDataBuf,				//消息数据缓冲区，包括NPC_S_DPS_MSG_HEAD
	IN		NPC_INT									in_iMsgDataLen				//消息数据长度
	);

//查询传输位置
NPC_VOID	NPC_F_UMSP_MC_DoProEvent_EVENT_E7_DUP_QUERY_TRANS_POS(
	IN		PNPC_S_UMSP_MCSRV_DATA					in_pMcSrvData,				//主控服务数据
	IN		PNPC_S_DPS_MSG_HEAD						in_pMsgHead,				//消息头
	IN		PNPC_BYTE								in_pMsgDataBuf,				//消息数据缓冲区，包括NPC_S_DPS_MSG_HEAD
	IN		NPC_INT									in_iMsgDataLen				//消息数据长度
	);

//升级数据包
NPC_VOID	NPC_F_UMSP_MC_DoProEvent_EVENT_E7_DUP_UPGRADE_DATA(
	IN		PNPC_S_UMSP_MCSRV_DATA					in_pMcSrvData,				//主控服务数据
	IN		PNPC_S_DPS_MSG_HEAD						in_pMsgHead,				//消息头
	IN		PNPC_BYTE								in_pMsgDataBuf,				//消息数据缓冲区，包括NPC_S_DPS_MSG_HEAD
	IN		NPC_INT									in_iMsgDataLen				//消息数据长度
	);



//-----------------------------------------------------------------------------------
//处理协议事件：音频帧数据
NPC_VOID	NPC_F_UMSP_MC_DoProEvent_MEDIA_E8_AUDIO_FRAME(
	IN		PNPC_S_UMSP_MCSRV_DATA					in_pMcSrvData,				//主控服务数据
	IN		PNPC_S_DPS_MSG_HEAD						in_pMsgHead,				//消息头
	IN		PNPC_BYTE								in_pMsgDataBuf,				//消息数据缓冲区，包括NPC_S_DPS_MSG_HEAD
	IN		NPC_INT									in_iMsgDataLen				//消息数据长度
	);


//-----------------------------------------------------------------------------------
//检测帧类型，返回帧类型，0：未检测到，1：I帧，2：P帧
NPC_INT		NPC_F_UMSP_MC_CheckFrameType(
	IN		PNPC_BYTE								in_pFrameDataBuf,			//帧数据缓冲区
	IN		NPC_INT									in_iFrameDataLen			//帧数据长度
	);


//-----------------------------------------------------------------------------------
//开始录像
NPC_BOOL	NPC_F_UMSP_MC_StartRecord(
	IN		PNPC_S_UMSP_MCSRV_DATA					in_pMcSrvData,				//主控服务数据
	IN		PNPC_S_UMSP_CAMERA_DATA					in_pCameraData,				//摄像机数据
	IN		NPC_INT									in_iChNo,					//通道号
	IN		NPC_INT									in_iFrameRate				//帧率
	);

//停止录像
NPC_VOID	NPC_F_UMSP_MC_StopRecord(
	IN		PNPC_S_UMSP_MCSRV_DATA					in_pMcSrvData,				//主控服务数据
	IN		PNPC_S_UMSP_CAMERA_DATA					in_pCameraData				//摄像机数据
	);


//-----------------------------------------------------------------------------------
//查询文件
NPC_BOOL	NPC_F_UMSP_MC_FindFile(
	IN		PNPC_S_UMSP_MCSRV_DATA					in_pMcSrvData,				//主控服务数据
	IN		NPC_INT									in_iChNo,					//通道号
	IN		PNPC_S_TD_DATE_TIME						in_pStartTime,				//开始时间
	IN		PNPC_S_TD_DATE_TIME						in_pEndTime,				//结束时间
	OUT		PNPC_INT								out_pFileNum,				//文件数
	OUT		PNPC_BYTE*								out_ppFileInfoDataBuf,		//文件信息数据缓冲区
	OUT		PNPC_INT								out_pFileInfoDataBufSize,	//文件信息数据缓冲区大小
	OUT		PNPC_INT								out_pFileInfoDataLen		//文件信息数据长度
	);

//扩展查询文件
NPC_BOOL	NPC_F_UMSP_MC_NspFindFile(
	IN		PNPC_S_UMSP_MCSRV_DATA					in_pMcSrvData,				//主控服务数据
	IN		NPC_INT									in_iChNo,					//通道号
	IN		NPC_INT									in_iStreamNo,				//码流号
	IN		PNPC_S_TD_DATE_TIME						in_pStartTime,				//开始时间
	IN		PNPC_S_TD_DATE_TIME						in_pEndTime,				//结束时间
	OUT		PNPC_INT								out_pFileNum,				//文件数
	OUT		PNPC_BYTE*								out_ppFileInfoDataBuf,		//文件信息数据缓冲区，为NPC_S_MON_MPS_FILE_HEAD_INFO数组
	OUT		PNPC_INT								out_pFileInfoDataBufSize,	//文件信息数据缓冲区大小
	OUT		PNPC_INT								out_pFileInfoDataLen		//文件信息数据长度
	);


//获取录像文件时间范围
NPC_BOOL	NPC_F_UMSP_MC_GetFileRange(
	IN		PNPC_S_UMSP_MCSRV_DATA					in_pMcSrvData,				//主控服务数据
	IN		NPC_INT									in_iChNo,					//通道号
	IN		NPC_INT									in_iStreamNo,				//码流号
	IN		PNPC_S_TD_DATE_TIME						in_pStartTime,				//开始时间
	IN		PNPC_S_TD_DATE_TIME						in_pEndTime,				//结束时间
	OUT		PNPC_INT								out_pFileNum,				//文件数
	OUT		PNPC_BYTE*								out_ppFileInfoDataBuf,		//文件信息数据缓冲区，为NPC_S_MON_MPS_FILE_HEAD_INFO数组
	OUT		PNPC_INT								out_pFileInfoDataBufSize,	//文件信息数据缓冲区大小
	OUT		PNPC_INT								out_pFileInfoDataLen		//文件信息数据长度
	);

//查询文件,获取第一个文件信息
NPC_BOOL	NPC_F_UMSP_MC_NspFindFirstFile(
	IN		PNPC_S_UMSP_MCSRV_DATA					in_pMcSrvData,				//主控服务数据
	IN		NPC_INT									in_iChNo,					//通道号
	IN		NPC_INT									in_iStreamNo,				//码流号
	IN		PNPC_S_TD_DATE_TIME						in_pStartTime,				//开始时间
	IN		PNPC_S_TD_DATE_TIME						in_pEndTime,				//结束时间
	INOUT	PNPC_S_MON_MPS_FILE_HEAD_INFO			io_pFileInfo				//文件信息
	);

//读取录像文件信息
NPC_BOOL	NPC_F_UMSP_MC_ReadRecFileInfo(
	IN		PNPC_CHAR								in_pFileName,				//文件名，包括路径
	OUT		PNPC_S_NAV_AV_FORMAT					out_pAvFormat,				//文件格式
	OUT		PNPC_CHAR								out_pRecBeginTime,			//录制开始时间，格式为：YYYY-MM-DD HH:MM:SS
	OUT		PNPC_CHAR								out_pRecEndTime,			//录制结束时间，格式为：YYYY-MM-DD HH:MM:SS
	OUT		PNPC_DWORD								out_pRecTimeLen				//录制时长（秒）
	);


//-----------------------------------------------------------------------------------
//打开文件，返回码为NPC_D_MON_CSD_DPS_ERROR_CODE_*
NPC_INT		NPC_F_UMSP_MC_File_OpenFile(
	IN		PNPC_S_UMSP_MCSRV_DATA					in_pMcSrvData,				//主控服务数据
	IN		PNPC_CHAR								in_pFileFlag,				//文件标识，唯一标识该文件，打开文件时必须原样传回
	IN		NPC_DWORD								in_dwStartOffsetDataPos,	//开始读取数据位置（单位字节），指原始文件的读取位置
	IN		NPC_DWORD								in_dwStartOffsetTimePos,	//开始读取时间位置（单位秒），为相对时间，如果值大于文件总时长，则打开失败
	OUT		PNPC_DWORD								out_pFileReadId,			//返回文件读取ID
	OUT		PNPC_S_MON_MPS_FILE_INFO				out_pFileInfo				//文件信息
	);

//打开文件（按时间方式），返回码为NPC_D_MON_CSD_DPS_ERROR_CODE_*
NPC_INT		NPC_F_UMSP_MC_File_OpenFile_BY_TIME(
	IN		PNPC_S_UMSP_MCSRV_DATA					in_pMcSrvData,				//主控服务数据
	IN		PNPC_CHAR								in_pFileFlag,				//文件标识，唯一标识该文件，打开文件时必须原样传回
	IN		NPC_DWORD								in_dwStartOffsetDataPos,	//开始读取数据位置（单位字节），指原始文件的读取位置
	IN		NPC_DWORD								in_dwStartOffsetTimePos,	//开始读取时间位置（单位秒），为相对时间，如果值大于文件总时长，则打开失败
	OUT		PNPC_DWORD								out_pFileReadId,			//返回文件读取ID
	OUT		PNPC_S_MON_MPS_FILE_INFO				out_pFileInfo				//文件信息
	);

//关闭文件
NPC_VOID	NPC_F_UMSP_MC_File_CloseFile(
	IN		PNPC_S_UMSP_MCSRV_DATA					in_pMcSrvData,				//主控服务数据
	IN		NPC_DWORD								in_dwFileReadId				//文件读取ID
	);

//定位文件，返回码为NPC_D_MON_CSD_DPS_ERROR_CODE_*
NPC_INT		NPC_F_UMSP_MC_File_SeekFile(
	IN		PNPC_S_UMSP_MCSRV_DATA					in_pMcSrvData,				//主控服务数据
	IN		NPC_DWORD								in_dwFileReadId,			//文件读取ID
	IN		NPC_DWORD								in_dwOffsetTimePos			//读取时间位置（单位秒），为相对时间
	);

//获取文件的视频参数
NPC_BOOL	NPC_F_UMSP_MC_File_GetFileVideoParam(
	IN		PNPC_S_UMSP_MCSRV_DATA					in_pMcSrvData,				//主控服务数据
	IN		NPC_DWORD								in_dwFileReadId,			//文件读取ID
	OUT		PNPC_INT								out_pFrameRate,				//帧率
	OUT		PNPC_INT								out_pGOP					//GOP
	);

//发送一媒体帧数据（视频或音频）
NPC_BOOL	NPC_F_UMSP_MC_File_SendMediaFrameData(
	IN		PNPC_S_UMSP_MCSRV_DATA					in_pMcSrvData,				//主控服务数据
	IN		PNPC_S_UMSP_FILE_WORK_DATA				in_pFileWorkData,			//文件工作数据
	IN		PNPC_BYTE								in_pMsgDataBuf,				//消息数据缓冲区，包括NPC_S_DPS_MSG_HEAD
	IN		NPC_INT									in_iMsgDataLen,				//消息数据长度
	IN		NPC_INT									in_iEndFlag,				//结束标志，0：未结束，1：本次请求结束，2：文件结束
	IN		NPC_INT									in_iMediaType,				//媒体类型，0：视频，1：音频
	IN		NPC_CHAR								in_cFrameType,				//帧类型，视频为‘I’，‘P’，‘B’等，音频该参数无效
	IN		NPC_DWORD								in_dwTimestamp,				//时间戳
	IN		NPC_DWORD								in_dwRecOffsetTime,			//录像偏移时间（秒），相对于文件头的时长
	IN		PNPC_BYTE								in_pMediaDataBuf,			//媒体数据缓冲区（原始码流数据）
	IN		NPC_INT									in_iMediaDataLen			//媒体数据长度
	);

//查询文件工作数据（根据文件读ID）
PNPC_S_UMSP_FILE_WORK_DATA		NPC_F_UMSP_MC_File_QueryFileWorkDataByFileReadId(
	IN		PNPC_S_UMSP_MCSRV_DATA					in_pMcSrvData,				//主控服务数据
	IN		NPC_DWORD								in_dwFileReadId				//文件读取ID
	);


//-----------------------------------------------------------------------------------
//计算文件大小
NPC_LONG	NPC_F_UMSP_MC_ComputeFileSize(
	IN		PNPC_CHAR								in_pFileName				//文件名（包括路径）
	);


//-----------------------------------------------------------------------------------
//处理报警状态
NPC_VOID	NPC_F_UMSP_MC_DoAlarmState(
	IN		PNPC_S_UMSP_MCSRV_DATA					in_pMcSrvData				//主控服务数据
	);

//发送报警消息
NPC_BOOL	NPC_F_UMSP_MC_SendAlarmMsg(
	IN		PNPC_S_UMSP_MCSRV_DATA					in_pMcSrvData,				//主控服务数据
	IN		PNPC_S_UMSP_CAMERA_DATA					in_pCameraData,				//摄像机数据
	IN		NPC_INT									in_iChNo,					//通道号
	IN		NPC_INT									in_iAlarmEvent,				//报警事件
	IN		NPC_INT									in_iAlarmState				//报警状态
	);

//发送关闭媒体流消息
NPC_BOOL	NPC_F_UMSP_MC_SendCloseMediaStreamMsg(
	IN		PNPC_S_UMSP_MCSRV_DATA					in_pMcSrvData,				//主控服务数据
	IN		NPC_INT									in_iChNo,					//通道号
	IN		NPC_INT									in_iStreamNo				//流号
	);

//发送关闭对讲消息
NPC_BOOL	NPC_F_UMSP_MC_SendCloseTalkMsg(
	IN		PNPC_S_UMSP_MCSRV_DATA					in_pMcSrvData,				//主控服务数据
	IN		NPC_INT									in_iChNo					//通道号
	);


//-----------------------------------------------------------------------------------
//修改用户密码，返回0表示成功，1：用户名错误，2：原密码错误，9：其它失败
NPC_INT		NPC_F_UMSP_MC_ModifyUserPwd(
	IN		PNPC_S_UMSP_MCSRV_DATA					in_pMcSrvData,				//主控服务数据
	IN		PNPC_CHAR								in_pUserName,				//用户名
	IN		PNPC_CHAR								in_pOldPwd,					//原密码
	IN		PNPC_CHAR								in_pNewPwd					//新密码
	);

//查询网络信息
NPC_INT		NPC_F_UMSP_MC_QueryNetInfo(
	IN		PNPC_S_UMSP_MCSRV_DATA					in_pMcSrvData,				//主控服务数据
	OUT		PNPC_INT								out_pNetCardNum,			//网卡数
	OUT		PNPC_BYTE								out_ppNetCardDataBuf,		//网卡数据缓冲区，为NPC_S_DPS_MSG_BODY_EVENT_E3_NET_CARD_INFO数组
	OUT		PNPC_INT								out_pNetCardDataLen			//网卡数据长度
	);


//------------------------------------------------------------------------------------------
//查询MP3文件列表
NPC_BOOL	NPC_F_UMSP_MC_DoCustomEvent_QUERY_MP3_FILE_LIST(
	IN		PNPC_S_UMSP_MCSRV_DATA					in_pMcSrvData,				//主控服务数据
	IN		PNPC_S_DPS_MSG_HEAD						in_pMsgHead,				//消息头
	IN		PNPC_BYTE								in_pMsgDataBuf,				//消息数据缓冲区，包括NPC_S_DPS_MSG_HEAD
	IN		NPC_INT									in_iMsgDataLen				//消息数据长度
	);

//删除MP3文件
NPC_BOOL	NPC_F_UMSP_MC_DoCustomEvent_DELETE_MP3_FILE(
	IN		PNPC_S_UMSP_MCSRV_DATA					in_pMcSrvData,				//主控服务数据
	IN		PNPC_S_DPS_MSG_HEAD						in_pMsgHead,				//消息头
	IN		PNPC_BYTE								in_pMsgDataBuf,				//消息数据缓冲区，包括NPC_S_DPS_MSG_HEAD
	IN		NPC_INT									in_iMsgDataLen				//消息数据长度
	);

//开始播放MP3文件
NPC_BOOL	NPC_F_UMSP_MC_DoCustomEvent_START_PLAY_MP3_FILE(
	IN		PNPC_S_UMSP_MCSRV_DATA					in_pMcSrvData,				//主控服务数据
	IN		PNPC_S_DPS_MSG_HEAD						in_pMsgHead,				//消息头
	IN		PNPC_BYTE								in_pMsgDataBuf,				//消息数据缓冲区，包括NPC_S_DPS_MSG_HEAD
	IN		NPC_INT									in_iMsgDataLen				//消息数据长度
	);

//停止播放MP3文件
NPC_BOOL	NPC_F_UMSP_MC_DoCustomEvent_STOP_PLAY_MP3_FILE(
	IN		PNPC_S_UMSP_MCSRV_DATA					in_pMcSrvData,				//主控服务数据
	IN		PNPC_S_DPS_MSG_HEAD						in_pMsgHead,				//消息头
	IN		PNPC_BYTE								in_pMsgDataBuf,				//消息数据缓冲区，包括NPC_S_DPS_MSG_HEAD
	IN		NPC_INT									in_iMsgDataLen				//消息数据长度
	);

//控制音量
NPC_BOOL	NPC_F_UMSP_MC_DoCustomEvent_CTRL_VOLUME(
	IN		PNPC_S_UMSP_MCSRV_DATA					in_pMcSrvData,				//主控服务数据
	IN		PNPC_S_DPS_MSG_HEAD						in_pMsgHead,				//消息头
	IN		PNPC_BYTE								in_pMsgDataBuf,				//消息数据缓冲区，包括NPC_S_DPS_MSG_HEAD
	IN		NPC_INT									in_iMsgDataLen				//消息数据长度
	);

//开始上传文件
NPC_BOOL	NPC_F_UMSP_MC_DoCustomEvent_START_UPLOAD_MP3_FILE(
	IN		PNPC_S_UMSP_MCSRV_DATA					in_pMcSrvData,				//主控服务数据
	IN		PNPC_S_DPS_MSG_HEAD						in_pMsgHead,				//消息头
	IN		PNPC_BYTE								in_pMsgDataBuf,				//消息数据缓冲区，包括NPC_S_DPS_MSG_HEAD
	IN		NPC_INT									in_iMsgDataLen				//消息数据长度
	);

//停止上传文件
NPC_BOOL	NPC_F_UMSP_MC_DoCustomEvent_STOP_UPLOAD_MP3_FILE(
	IN		PNPC_S_UMSP_MCSRV_DATA					in_pMcSrvData,				//主控服务数据
	IN		PNPC_S_DPS_MSG_HEAD						in_pMsgHead,				//消息头
	IN		PNPC_BYTE								in_pMsgDataBuf,				//消息数据缓冲区，包括NPC_S_DPS_MSG_HEAD
	IN		NPC_INT									in_iMsgDataLen				//消息数据长度
	);

//上传文件数据
NPC_BOOL	NPC_F_UMSP_MC_DoCustomEvent_UPLOAD_FILE_DATA(
	IN		PNPC_S_UMSP_MCSRV_DATA					in_pMcSrvData,				//主控服务数据
	IN		PNPC_S_DPS_MSG_HEAD						in_pMsgHead,				//消息头
	IN		PNPC_BYTE								in_pMsgDataBuf,				//消息数据缓冲区，包括NPC_S_DPS_MSG_HEAD
	IN		NPC_INT									in_iMsgDataLen				//消息数据长度
	);


//------------------------------------------------------------------------------------------
//关闭所有视频流
NPC_BOOL	NPC_F_UMSP_MC_CloseVideoStream(
	IN		PNPC_S_UMSP_MCSRV_DATA					in_pMcSrvData				//主控服务数据
	);


//------------------------------------------------------------------------------------------
//创建报警ID
NPC_VOID	NPC_F_UMSP_MC_CreateAlarmId(
	IN		PNPC_S_UMSP_MCSRV_DATA					in_pMcSrvData,				//主控服务数据
	OUT		PNPC_CHAR								out_pAlarmId				//报警ID
	);

//发送报警消息
NPC_VOID	NPC_F_UMSP_MC_SendAlarmInfo(
	IN		PNPC_S_UMSP_MCSRV_DATA					in_pMcSrvData,				//主控服务数据
	IN		NPC_INT									in_iChNo,					//通道号
	IN		NPC_INT									in_iAlarmEvent,				//报警事件
	IN		PNPC_CHAR								in_pAlarmInfo				//报警信息
	);


#endif


