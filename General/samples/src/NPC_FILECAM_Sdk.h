/*----------------------------------------------------------------------------
 * Copyright(c)  :  NPC CORPORTAION All Rights Reserved                       
 * FileName      :  NPC_FILECAM_Sdk.h
 * Version       :  1.1
 * Author        :  CCH
 * DateTime      :  2011-08-24
 * Description   :  NPC系统文件摄像机模块
 *----------------------------------------------------------------------------*/

#ifndef __NPC_FILECAM_SDK_H
#define __NPC_FILECAM_SDK_H

#include "NPC_TypeDef.h"


//回调函数定义
typedef void(CALLBACK *fNPC_F_FILECAM_SDK_RealDataCallBack)(
	IN		NPC_INT			in_iMediaType,				//媒体类型，0：视频，1：音频
	IN		PNPC_BYTE		in_pMediaDataBuf,			//媒体数据
	IN		NPC_INT			in_iMediaDataLen,			//媒体数据长度
	IN		NPC_DWORD		in_dwUserData				//用户数据
	);


//设备参数
typedef struct _NPC_S_FILECAM_SDK_CH_PARAM
{
	NPC_HANDLE				hFileCam;					//文件摄像机句柄，由NPC_F_FILECAM_SDK_ConnectFilecam函数创建
	NPC_INT					iChNo;						//通道号

	NPC_INT					iFrameRate;					//帧率
	NPC_BOOL				bIfSupportAudio;			//是否支持音频

} NPC_S_FILECAM_SDK_CH_PARAM, *PNPC_S_FILECAM_SDK_CH_PARAM;



//--------------------------------------------------------------------------------
#ifdef __cplusplus
extern "C"
{
#endif


//初始化文件摄像机SDK
NPC_API_INTER	NPC_BOOL	NPC_F_FILECAM_SDK_InitSdk();

//反初始化文件摄像机SDK
NPC_API_INTER	NPC_VOID	NPC_F_FILECAM_SDK_UninitSdk();

//连接文件摄像机
NPC_API_INTER	NPC_HANDLE	NPC_F_FILECAM_SDK_ConnectFilecam(
	IN		PNPC_CHAR						in_pDirName,				//目录名
	OUT		PNPC_INT						out_pChNum					//通道数
	);

//断开连接文件摄像机
NPC_API_INTER	NPC_VOID	NPC_F_FILECAM_SDK_DisconnectFilecam(
	IN		NPC_HANDLE						in_hFileCam					//文件摄像机句柄，由NPC_F_FILECAM_SDK_ConnectFilecam函数创建
	);

//开始预览
NPC_API_INTER	NPC_BOOL	NPC_F_FILECAM_SDK_RealPlay(
	IN		NPC_HANDLE						in_hFileCam,				//文件摄像机句柄，由NPC_F_FILECAM_SDK_ConnectFilecam函数创建
	IN		NPC_INT							in_iChNo,					//通道号
	IN		fNPC_F_FILECAM_SDK_RealDataCallBack		in_cbRealDataCallBack,		//媒体数据回调函数
	IN		NPC_DWORD						in_dwUserData,				//用户数据
	OUT		PNPC_S_FILECAM_SDK_CH_PARAM		out_pCamParam				//文件摄像机参数
	);

//结束预览
NPC_API_INTER	NPC_BOOL	NPC_F_FILECAM_SDK_StopRealPlay(
	IN		NPC_HANDLE						in_hFileCam,				//文件摄像机句柄，由NPC_F_FILECAM_SDK_ConnectFilecam函数创建
	IN		NPC_INT							in_iChNo					//通道号
	);

//开始对讲
NPC_API_INTER	NPC_BOOL	NPC_F_FILECAM_SDK_StartTalk(
	IN		NPC_HANDLE						in_hFileCam,				//文件摄像机句柄，由NPC_F_FILECAM_SDK_ConnectFilecam函数创建
	IN		NPC_INT							in_iChNo					//通道号
	);

//停止对讲
NPC_API_INTER	NPC_VOID	NPC_F_FILECAM_SDK_StopTalk(
	IN		NPC_HANDLE						in_hFileCam,				//文件摄像机句柄，由NPC_F_FILECAM_SDK_ConnectFilecam函数创建
	IN		NPC_INT							in_iChNo					//通道号
	);

//输入对讲音频数据
NPC_API_INTER	NPC_BOOL	NPC_F_FILECAM_SDK_InputTalkAudioData(
	IN		NPC_HANDLE						in_hFileCam,				//文件摄像机句柄，由NPC_F_FILECAM_SDK_ConnectFilecam函数创建
	IN		NPC_INT							in_iChNo,					//通道号
	IN		PNPC_BYTE						in_pAudioDataBuf,			//音频数据缓冲区
	IN		NPC_INT							in_iAudioDataLen			//音频数据长度
	);

//请求强制I帧
NPC_API_INTER	NPC_VOID	NPC_F_FILECAM_SDK_RequestForceIFrame(
	IN		NPC_HANDLE						in_hFileCam,				//文件摄像机句柄，由NPC_F_FILECAM_SDK_ConnectFilecam函数创建
	IN		NPC_INT							in_iChNo					//通道号
	);


#ifdef __cplusplus
}
#endif


#endif

