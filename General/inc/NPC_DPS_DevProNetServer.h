/*----------------------------------------------------------------------------
 * Copyright(c)  :  NPC CORPORTAION All Rights Reserved                       
 * FileName      :  NPC_DPS_DevProNetServer.h
 * Version       :  1.0
 * Author        :  CCH
 * DateTime      :  2013-07-16
 * Description   :  UMSP设备协议网络服务接口
 *----------------------------------------------------------------------------*/

#ifndef __NPC_DPS_DEVPRONETSERVER_H
#define __NPC_DPS_DEVPRONETSERVER_H

#include "NPC_TypeDef.h"
#include "NPC_MON_ConsDef.h"
#include "NPC_DPS_DevProtocolDef.h"


#ifdef _WIN32
	#ifdef _NPC_DPS_DEVPRO_EXPORTS
		#define NPC_DPS_DEVPRO_API __declspec(dllexport)
	#else
		#ifdef _NPC_DPS_DEVPRO_NON
			#define NPC_DPS_DEVPRO_API
		#else
			#define NPC_DPS_DEVPRO_API __declspec(dllimport)
		#endif
	#endif
#else
	#define NPC_DPS_DEVPRO_API
#endif


//-------------------------------------------------------------------------------
//常量定义
#define NPC_D_DPS_MAX_CHANNEL_NUM						64						//最大通道数
#define NPC_D_DPS_MAX_CH_STREAM_NUM						3						//最大通道码流数

//-------------------------------------------------------------------------------
//权限位定义
#define NPC_D_DPS_POP_REALPLAY							0x00000001				//实时预览权限（具有全局和通道权限）
#define NPC_D_DPS_POP_PTZ_CTRL							0x00000002				//云台控制权限（具有全局和通道权限）
#define NPC_D_DPS_POP_TALK								0x00000004				//语音对讲权限（具有全局和通道权限）
#define NPC_D_DPS_POP_QUERY_FILE						0x00000008				//录像文件查询权限（具有全局和通道权限）
#define NPC_D_DPS_POP_BACKPLAY							0x00000010				//录像回放权限（只有全局权限）
#define NPC_D_DPS_POP_ALARM								0x00000020				//报警布防权限（只有全局权限）
#define NPC_D_DPS_POP_GET_CONFIG						0x00000040				//查询配置权限（只有全局权限）
#define NPC_D_DPS_POP_SET_CONFIG						0x00000080				//设置配置权限（只有全局权限）


//-------------------------------------------------------------------------------
//用户信息
typedef struct _NPC_S_DPS_USER_INFO
{
	NPC_CHAR										sUserName[32];				//用户名
	NPC_CHAR										sUserPwd[32];				//用户密码（明文）
	NPC_BYTE										ucPopTable[8];				//权限表（暂停使用，兼容以前的程序）

	NPC_DWORD										dwGlobalPop;				//全局权限，位值参考NPC_D_DPS_POP_*定义
	NPC_DWORD										dwChPopTable[NPC_D_DPS_MAX_CHANNEL_NUM];	//通道权限表，位值参考NPC_D_DPS_POP_*定义

} NPC_S_DPS_USER_INFO, *PNPC_S_DPS_USER_INFO;

//设备能力信息
typedef struct _NPC_S_DPS_DEV_CAP_INFO
{
	NPC_CHAR										sDevType[12];				//设备类型，如：DEV，IPC
	NPC_CHAR										sDevModel[32];				//设备型号
	NPC_CHAR										sSerialNumber[32];			//序列号
	NPC_INT											iVideoChanNum;				//设备通道个数
	NPC_INT											iAudioChanNum;				//设备语音通道数
	NPC_INT											iPtzChNum;					//PTZ通道数
	NPC_INT											iAlarmInPortNum;			//报警输入个数
	NPC_INT											iAlarmOutPortNum;			//报警输出个数
	NPC_INT											iDiskNum;					//硬盘个数
	NPC_INT											iSubProtoNum;				//子码流数，0表示不支持子码流
	NPC_BOOL										bIfSupportZeroChannel;		//是否支持零通道
	NPC_BOOL										bIfSupportRecordBytime;		//是否支持按时间回放
	NPC_BOOL										bIfSupportFileRange;		//是否支持获取录像文件时间范围

	NPC_BOOL										bIfOpenDoConnCallback;		//是否开启连接处理回调
	PNPC_VOID										pDoConnCallback;			//连接处理回调函数入口地址	
	PNPC_VOID										pDoConnUserData;			//连接处理回调用户数据	
	
	NPC_CHAR										bEnableSpeedCtl;			//是否支持回放速度控制
	NPC_CHAR										bEnableAlarmPushCallback;	//是否接收报警推送回调，功能ID：
																				//NPC_D_DPS_MSGID_EVENT_E2_GUARD
																				//NPC_D_DPS_MSGID_EVENT_E2_UNGUARD
	NPC_CHAR										sReserve[62];				//预留
} NPC_S_DPS_DEV_CAP_INFO, *PNPC_S_DPS_DEV_CAP_INFO;

//连接处理信息
typedef struct _NPC_S_DPS_CONNECT_INFO
{
	NPC_DWORD										dwLoginID;					//登录识别ID
	NPC_INT											iChNo;						//通道号
	NPC_INT											iStreamNo;					//码流号
	
	NPC_INT											iType;						//连接类型：0=打开实时预览、1=关闭实时预览
	NPC_INT											iRet;						//处理结果：0=成功，其他失败，iType = 0时有效
} NPC_S_DPS_CONNECT_INFO, *PNPC_S_DPS_CONNECT_INFO;
//-------------------------------------------------------------------------------
//协议事件回调函数定义
typedef		NPC_VOID	(*PNPC_F_DPS_ProEventCallback)(
	IN		PNPC_VOID								in_pUserData,				//用户数据
	IN		PNPC_BYTE								in_pMsgDataBuf,				//消息数据缓冲区，包括NPC_S_DPS_MSG_HEAD+包体数据
	IN		NPC_INT									in_iMsgDataLen				//消息数据长度
	);

//查询用户信息回调函数定义
typedef		NPC_BOOL	(*PNPC_F_DPS_QueryUserInfoCallback)(
	IN		PNPC_VOID								in_pUserData,				//用户数据
	IN		PNPC_CHAR								in_pUserName,				//用户名
	OUT		PNPC_S_DPS_USER_INFO					out_pUserInfo				//用户信息
	);

//查询用户信息回调函数定义(扩展，客户端定制标识校验)
//0:校验通过，1:用户名错误，2:密码错误，3:校验错误
typedef		NPC_INT	(*PNPC_F_DPS_UserCallbackByCustom)(
	IN		PNPC_VOID								in_pUserData,				//用户数据
	IN		PNPC_CHAR								in_pUserName,				//用户名
 	IN		NPC_INT									in_iCltCumNum,				//客户端定制标识数量
 	IN		NPC_INT									in_iCltCumStrLen,			//客户端定制标识字符串长度
 	IN		PNPC_CHAR								in_pCltCumStr,				//客户端定制标识字符串
	OUT		PNPC_S_DPS_USER_INFO					out_pUserInfo				//用户信息
	);


//连接处理回调函数定义
typedef		NPC_BOOL	(*PNPC_F_DPS_DoConnectCallback)(
	IN		PNPC_VOID								in_pUserData,				//用户数据
	OUT		PNPC_S_DPS_CONNECT_INFO					out_pDoConnInfo				//连接处理信息
	);

//-------------------------------------------------------------------------------
#ifdef __cplusplus
extern "C"
{
#endif


//启动设备协议网络服务
NPC_DPS_DEVPRO_API		NPC_HANDLE	NPC_F_DPS_StartDevProNetServer(
	IN		NPC_USHORT								in_usMediaTcpPort,			//媒体TCP监听端口号
	IN		PNPC_S_DPS_DEV_CAP_INFO					in_pDevCapInfo,				//设备能力信息
	IN		PNPC_F_DPS_ProEventCallback				in_pProEventCallback,		//协议事件回调函数指针
	IN		PNPC_VOID								in_pProEventUserData,		//协议事件用户数据
	IN		PNPC_F_DPS_QueryUserInfoCallback		in_pQueryUserInfoCallback,	//查询用户信息回调函数指针
	IN		PNPC_VOID								in_pQueryUserInfoUserData,	//查询用户信息用户数据
	IN		NPC_BOOL								in_bIfDebug=FALSE,			//是否调试，在调试状态下，会输出很多日志信息，正式使用时需要关闭
	IN		NPC_BOOL								in_bIfShowStreamLog=FALSE,	//是否显示流日志，即媒体流收发日志信息
	IN		NPC_BOOL								in_bIfWriteLogToFile=FALSE,	//是否写日志到文件
	IN		PNPC_CHAR								in_pLogDir=NULL,			//日志目录，日志文件名自动产生，格式为Log日期.txt
	IN		NPC_USHORT								in_usLogPort=0,				//日志端口号，如果为0，则不启动日志监听端口，大于0则启动日志监听端口服务
	IN		NPC_INT									in_iPwdMode=0				//密码模式（指密码传给SDK的方式），0：明文，1：MD5
	);

//停止设备协议网络服务
NPC_DPS_DEVPRO_API		NPC_VOID	NPC_F_DPS_StopDevProNetServer(
	IN		NPC_HANDLE								in_hDevProNetSrv			//设备协议网络服务句柄
	);

//设置用户信息回调（客户端定制标识校验）
NPC_DPS_DEVPRO_API		NPC_BOOL	NPC_F_DPS_SetUserCallbackByCustom(
	IN		NPC_HANDLE								in_hDevProNetSrv,			//设备协议网络服务句柄
	IN		PNPC_F_DPS_UserCallbackByCustom			in_pUserCallbackByCustom,	//查询用户信息回调函数指针
	IN		PNPC_VOID								in_pUserByCustomUserData	//查询用户信息用户数据
	);

//-------------------------------------------------------------------------------
//发送消息给设备协议网络服务
NPC_DPS_DEVPRO_API		NPC_BOOL	NPC_F_DPS_SendDevProData(
	IN		NPC_HANDLE								in_hDevProNetSrv,			//设备协议网络服务句柄
	IN		PNPC_BYTE								in_pDevProDataBuf,			//设备协议数据缓冲区，包括NPC_S_DPS_MSG_HEAD
	IN		NPC_INT									in_iDevProDataLen			//设备协议数据长度
	);

//设置媒体数据缓存大小
NPC_DPS_DEVPRO_API		NPC_BOOL	NPC_F_DPS_SetMediaDataBufSize(
	IN		NPC_HANDLE								in_hDevProNetSrv,			//设备协议网络服务句柄
	IN		NPC_INT									in_iMediaDataBufSize		//媒体数据缓存大小（字节）
	);

//设置通道媒体数据缓存大小（1~30Mbps），如果in_iMediaDataBufSize = 0，表示不做限制
NPC_DPS_DEVPRO_API		NPC_BOOL	NPC_F_DPS_SetMediaDataBufSizeByCh(
	IN		NPC_HANDLE								in_hDevProNetSrv,			//设备协议网络服务句柄
	IN		NPC_INT									in_iChNo,					//通道号
	IN		NPC_INT									in_iStreamNo,				//码流号，0：主码流，1：子码流
	IN		NPC_INT									in_iMediaDataBufSize		//媒体数据缓存大小（字节）
	);

//设置运行状态信息显示开关
NPC_DPS_DEVPRO_API		NPC_BOOL	NPC_F_DPS_SetShowRunState(
	IN		NPC_HANDLE								in_hDevProNetSrv,			//设备协议网络服务句柄
	IN		NPC_BOOL								in_bIfShowRunState			//是否显示运行状态信息，TRUE表示要显示，FALSE表示不显示，缺省为TRUE
	);

//设置实时预览只发I帧的时长（秒）（暂停）
NPC_DPS_DEVPRO_API		NPC_BOOL	NPC_F_DPS_SetOnlySendIFrameTimeLen(
	IN		NPC_HANDLE								in_hDevProNetSrv,			//设备协议网络服务句柄
	IN		NPC_INT									in_iIFrameTimeLen			//只发I帧的时长（秒），缺省为60秒
	);

//设置最大实时预览路数（客户端路数）
NPC_DPS_DEVPRO_API		NPC_BOOL	NPC_F_DPS_SetMaxRealplayClientNum(
	IN		NPC_HANDLE								in_hDevProNetSrv,			//设备协议网络服务句柄
	IN		NPC_INT									in_iMaxClientNum			//最大实时预览客户端数，缺省为64路，值范围为1~1024
	);

//-------------------------------------------------------------------------------
//输入视频流（相当于发送NPC_D_DPS_MSGID_MEDIA_E8_VIDEO_FRAME消息）
NPC_DPS_DEVPRO_API		NPC_BOOL	NPC_F_DPS_InputVideoStream(
	IN		NPC_HANDLE								in_hDevProNetSrv,			//设备协议网络服务句柄
	IN		NPC_INT									in_iChNo,					//通道号
	IN		NPC_INT									in_iStreamNo,				//码流号，0：主码流，1：子码流
	IN		NPC_INT									in_iFrameType,				//帧类型，参见NPC_D_DPS_VIDEO_FRAME_TYPE_*定义
	IN		PNPC_BYTE								in_pMediaDataBuf,			//媒体数据缓冲区
	IN		NPC_INT									in_iMediaDataLen			//媒体数据长度
	);

//输入音频流（相当于发送NPC_D_DPS_MSGID_MEDIA_E8_AUDIO_FRAME消息）
NPC_DPS_DEVPRO_API		NPC_BOOL	NPC_F_DPS_InputAudioStream(
	IN		NPC_HANDLE								in_hDevProNetSrv,			//设备协议网络服务句柄
	IN		NPC_INT									in_iChNo,					//通道号
	IN		NPC_INT									in_iStreamNo,				//码流号，0：主码流，1：子码流
	IN		PNPC_BYTE								in_pMediaDataBuf,			//媒体数据缓冲区
	IN		NPC_INT									in_iMediaDataLen			//媒体数据长度
	);

//输入视频流（相当于发送NPC_D_DPS_MSGID_MEDIA_E8_VIDEO_FRAME_EX消息）
NPC_DPS_DEVPRO_API		NPC_BOOL	NPC_F_DPS_InputVideoStreamEx(
	IN		NPC_HANDLE								in_hDevProNetSrv,			//设备协议网络服务句柄
	IN		NPC_INT									in_iChNo,					//通道号
	IN		NPC_INT									in_iStreamNo,				//码流号，0：主码流，1：子码流
	IN		NPC_INT									in_iFrameType,				//帧类型，参见NPC_D_DPS_VIDEO_FRAME_TYPE_*定义
	IN		NPC_DWORD								in_dwTimeStamp,				//时间戳，32位
	IN		PNPC_BYTE								in_pMediaDataBuf,			//媒体数据缓冲区
	IN		NPC_INT									in_iMediaDataLen			//媒体数据长度
	);

//输入音频流（相当于发送NPC_D_DPS_MSGID_MEDIA_E8_AUDIO_FRAME_EX消息）
NPC_DPS_DEVPRO_API		NPC_BOOL	NPC_F_DPS_InputAudioStreamEx(
	IN		NPC_HANDLE								in_hDevProNetSrv,			//设备协议网络服务句柄
	IN		NPC_INT									in_iChNo,					//通道号
	IN		NPC_INT									in_iStreamNo,				//码流号，0：主码流，1：子码流
	IN		NPC_DWORD								in_dwTimeStamp,				//时间戳，32位
	IN		PNPC_BYTE								in_pMediaDataBuf,			//媒体数据缓冲区
	IN		NPC_INT									in_iMediaDataLen			//媒体数据长度
	);

//修改编码参数（即发送NPC_D_DPS_MSGID_EVENT_E2_MODIFY_CODEC消息）
NPC_DPS_DEVPRO_API		NPC_BOOL	NPC_F_DPS_ModifyCodec(
	IN		NPC_HANDLE								in_hDevProNetSrv,			//设备协议网络服务句柄
	IN		NPC_INT									in_iChNo,					//通道号
	IN		NPC_INT									in_iStreamNo,				//码流号，0：主码流，1：子码流
	IN		NPC_BOOL								in_bVideoFlag,				//视频标志，TRUE：表示有视频，FALSE：表示没有视频
	IN		NPC_BOOL								in_bAudioFlag,				//音频标志，TRUE：表示有音频，FALSE：表示没有音频
	IN		NPC_INT									in_iVideoCodecId,			//视频编码ID，参见NPC_D_MON_CSD_CODEC_ID_*定义
	IN		NPC_INT									in_iVideoFrameRate,			//视频帧率
	IN		NPC_INT									in_iVideoBitRate,			//视频码率（kbps）
	IN		NPC_USHORT								in_usVideoWidth,			//视频宽度
	IN		NPC_USHORT								in_usVideoHeight,			//视频高度
	IN		NPC_INT									in_iAudioCodecId,			//音频编码ID，参见NPC_D_MON_CSD_CODEC_ID_*定义
	IN		NPC_INT									in_iAudioBitRate,			//音频码率（kbps），如64
	IN		NPC_INT									in_iAudioSampleRate,		//音频采样率，如8000
	IN		NPC_INT									in_iAudioChNum				//音频声道数，如1
	);

//上报报警消息（只有布防的报警消息才发送给服务器）
NPC_DPS_DEVPRO_API		NPC_BOOL	NPC_F_DPS_UpAlarmMsg(
	IN		NPC_HANDLE								in_hDevProNetSrv,			//设备协议网络服务句柄
	IN		NPC_INT									in_iChNo,					//通道号
	IN		NPC_INT									in_iAlarmEvent,				//报警事件，参见NPC_D_MON_CSD_ALARM_EVENT_*定义
	IN		NPC_INT									in_iAlarmState,				//报警状态，0：停止，1：开始
	IN		PNPC_CHAR								in_pAlarmInfo				//报警信息
	);

//发送报警消息（该函数发送的报警消息不经过筛选，直接发送给服务器）
NPC_DPS_DEVPRO_API		NPC_BOOL	NPC_F_DPS_SendAlarmMsg(
	IN		NPC_HANDLE								in_hDevProNetSrv,			//设备协议网络服务句柄
	IN		PNPC_CHAR								in_pAlarmId,				//报警ID，要求设备端唯一，比如采用时间值+序号，如20150301091022001
	IN		NPC_INT									in_iChNo,					//通道号
	IN		NPC_INT									in_iAlarmEvent,				//报警事件，参见NPC_D_MON_CSD_ALARM_EVENT_*定义
	IN		PNPC_CHAR								in_pAlarmInfo,				//报警信息
	IN		PNPC_S_TD_DATE_TIME						in_pAlarmTime=NULL			//报警时间
	);

//查询通道缓存视频帧数
NPC_DPS_DEVPRO_API		NPC_INT		NPC_F_DPS_QueryCacheVideoFrameNum(
	IN		NPC_HANDLE								in_hDevProNetSrv,			//设备协议网络服务句柄
	IN		NPC_INT									in_iChNo,					//通道号
	IN		NPC_INT									in_iStreamNo				//码流号，0：主码流，1：子码流
	);

//查询通道发送媒体数据
NPC_DPS_DEVPRO_API		NPC_BOOL		NPC_F_DPS_QuerySendMediaDataLen(
	IN		NPC_HANDLE								in_hDevProNetSrv,			//设备协议网络服务句柄
	IN		NPC_INT									in_iChNo,					//通道号
	IN		NPC_INT									in_iStreamNo,				//码流号，0：主码流，1：子码流
	OUT		PNPC_DWORD								out_pMediaLen,				//媒体数据长度
	OUT		PNPC_INT								out_pSendTime				//媒体数据长度是发送了多长时间的
	);

//--------------------------------------------------------------------------------
//控制媒体资源阈值
NPC_DPS_DEVPRO_API		NPC_BOOL	NPC_F_DPS_ControlMediaResource(
	IN		NPC_HANDLE								in_hDevProNetSrv,			//设备协议网络服务句柄
	IN		NPC_BOOL								in_bIfOpenMediaResourceCtl,	//是否开启媒体资源阈值控制
	IN		NPC_FLOAT								in_fMaxMediaResource,		//开启有效：最大媒体资源:子码流每个连接损耗1，主码流和回放损耗按设置处理
	IN		NPC_FLOAT								in_fMainMediaResourceLoss,	//开启有效：每路主码流的损耗
	IN		NPC_FLOAT								in_fReplayResourceLoss		//开启有效：每路回放的资源损耗
	);

//获取当前空闲的媒体资源大小
NPC_DPS_DEVPRO_API		NPC_BOOL	NPC_F_DPS_GetFreeMediaResourceNum(
	IN		NPC_HANDLE								in_hDevProNetSrv,			//设备协议网络服务句柄
	IN		PNPC_FLOAT								in_pMaxMediaResource,		//最大媒体资源:子码流每个连接损耗1，主码流和回放损耗按设置处理
	IN		PNPC_FLOAT								in_pFreeMediaResource		//空闲媒体资源
	);


//------------------------------------------------------------------------------
//向所有的媒体通道发送定制消息
NPC_DPS_DEVPRO_API		NPC_BOOL	NPC_F_DPS_SendCusFuncToOrderChnl(
	IN		NPC_HANDLE								in_hDevProNetSrv,			//设备协议网络服务句柄
	IN		NPC_INT									in_iCusFuncId,				//功能ID
	IN		PNPC_BYTE								in_pBodyDataBuf,			//包体数据缓冲区
	IN		NPC_INT									in_iBodyDataLen				//包体数据长度
	);

//查询当前该请求对应通道的用户信息（根据事务ID）
NPC_DPS_DEVPRO_API		NPC_BOOL	NPC_F_DPS_QueryUserInfoByTransId(
	IN		NPC_HANDLE								in_hDevProNetSrv,			//[IN]设备协议网络服务句柄
	IN		NPC_DWORD								in_dwTransId,				//[IN]事务ID
	OUT		PNPC_S_DPS_USER_INFO					out_pUserInfo				//[OUT]用户信息	
	);

#ifdef __cplusplus
}
#endif

#endif

