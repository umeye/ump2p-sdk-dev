/*----------------------------------------------------------------------------
 * Copyright(c)  :  NPC CORPORTAION All Rights Reserved                       
 * FileName      :  NPC_NAV_File.h
 * Version       :  1.1
 * Author        :  CCH
 * DateTime      :  2012-02-21
 * Description   :  NAV文件读写模块
 *----------------------------------------------------------------------------*/

#ifndef __NPC_NAV_FILE_H
#define __NPC_NAV_FILE_H

#include "NPC_TypeDef.h"


//--------------------------------------------------------------------------------
//常量定义
#define NPC_D_NAV_DEF_CM_VEDIO_PARAMS_BUFSIZE					64								//视频参数缓冲区大小
#define NPC_D_NAV_DEF_CM_AUDIO_PARAMS_BUFSIZE					64								//音频参数缓冲区大小


//--------------------------------------------------------------------------------
//音视频格式数据结构
typedef struct _NPC_S_NAV_AV_FORMAT
{
	NPC_BOOL							bVedioFlag;												//视频标志，TRUE表示有视频，FALSE表示没有视频
	NPC_BOOL							bAudioFlag;												//音频标志，TRUE表示有音频，FALSE表示没有音频

	NPC_INT								iVedioCodecId;											//视频编码参数，0：默认编码，1：H264，2：MPEG4，3：H263。。。
	NPC_USHORT							usVedioWidth;											//视频宽度
	NPC_USHORT							usVedioHeight;											//视频高度
	NPC_INT								iVedioBitRate;											//视频码率参数，0表示为默认码率
	NPC_INT								iVedioFrameRate;										//视频帧率参数，0表示为默认帧率
	NPC_CHAR							sVedioParams[NPC_D_NAV_DEF_CM_VEDIO_PARAMS_BUFSIZE];	//视频参数，BASE64格式，以短号分隔，如果为H264格式，则参数包括SPS和PPS

	NPC_INT								iAudioCodecId;											//音频编码参数，0：默认编码，1：16位线性PCM，2：G711A，3：G711U，4：G721。。。
	NPC_INT								iAudioSampleRate;										//音频采样率
	NPC_INT								iAudioChNum;											//音频通道数，1或2
	NPC_CHAR							sAudioParams[NPC_D_NAV_DEF_CM_AUDIO_PARAMS_BUFSIZE];	//音频参数

} NPC_S_NAV_AV_FORMAT, *PNPC_S_NAV_AV_FORMAT;


//--------------------------------------------------------------------------------
#ifdef __cplusplus
extern "C"
{
#endif


//创建NAV文件
NPC_API_INTER	NPC_HANDLE	NPC_F_NAV_CreateFile(
	IN		PNPC_CHAR						in_pFileName,				//文件名
	IN		NPC_INT							in_iDevVendor,				//设备厂商
	IN		NPC_INT							in_iRecCodecFormat,			//录像编码格式，0：为标准格式，tRecFormat有效，其它为厂商原始格式，需要用厂商的SDK解码
	IN		PNPC_S_NAV_AV_FORMAT			in_pRecFormat,				//录像格式
	IN		NPC_DWORD						in_dwTimestampRef,			//时间戳参考值，默认为90000
	IN		PNPC_CHAR						in_pRecBeginTime			//录制开始时间，格式为：YYYY-MM-DD HH:MM:SS
	);

//打开NAV文件
NPC_API_INTER	NPC_HANDLE	NPC_F_NAV_OpenFile(
	IN		PNPC_CHAR						in_pFileName,				//文件名
	OUT		PNPC_INT						out_pDevVendor,				//设备厂商
	OUT		PNPC_INT						out_pRecCodecFormat,		//录像编码格式，0：为标准格式，tRecFormat有效，其它为厂商原始格式，需要用厂商的SDK解码
	OUT		PNPC_S_NAV_AV_FORMAT			out_pRecFormat,				//录像格式
	OUT		PNPC_DWORD						out_pTimestampRef,			//时间戳参考值，默认为90000
	OUT		PNPC_CHAR						out_pRecBeginTime,			//录制开始时间，格式为：YYYY-MM-DD HH:MM:SS
	OUT		PNPC_CHAR						out_pRecEndTime,			//录制结束时间，格式为：YYYY-MM-DD HH:MM:SS
	OUT		PNPC_DWORD						out_pRecTimeLen				//录制时长（秒）
	);

//关闭NAV文件
NPC_API_INTER	NPC_VOID	NPC_F_NAV_CloseFile(
	IN		NPC_HANDLE						in_hNavFile					//NAV文件句柄
	);

//向文件添加媒体数据（视频或音频）
NPC_API_INTER	NPC_BOOL	NPC_F_NAV_AddMediaData(
	IN		NPC_HANDLE						in_hNavFile,				//NAV文件句柄
	IN		NPC_INT							in_iMediaType,				//媒体类型，0：视频，1：音频
	IN		NPC_CHAR						in_cFrameType,				//帧类型，视频为‘I’，‘P’，‘B’等，音频该参数无效
	IN		NPC_DWORD						in_dwTimestamp,				//时间戳
	IN		NPC_DWORD						in_dwRecOffsetTime,			//录像偏移时间（秒），相对于文件头的时长
	IN		PNPC_BYTE						in_pMediaDataBuf,			//媒体数据缓冲区（原始码流数据）
	IN		NPC_INT							in_iMediaDataLen			//媒体数据长度
	);

//读取媒体数据（视频或音频）
NPC_API_INTER	NPC_BOOL	NPC_F_NAV_ReadMediaData(
	IN		NPC_HANDLE						in_hNavFile,				//NAV文件句柄
	OUT		PNPC_INT						out_pMediaType,				//媒体类型，0：视频，1：音频
	OUT		PNPC_CHAR						out_pFrameType,				//帧类型，视频为‘I’，‘P’，‘B’等，音频该参数无效
	OUT		PNPC_DWORD						out_pTimestamp,				//时间戳
	OUT		PNPC_DWORD						out_pRecOffsetTime,			//录像偏移时间（秒），相对于文件头的时长
	OUT		PNPC_BYTE*						out_ppMediaDataBuf,			//媒体数据缓冲区（原始码流数据）
	OUT		PNPC_INT						out_pMediaDataBufSize,		//媒体数据缓冲区大小
	OUT		PNPC_INT						out_pMediaDataLen			//媒体数据长度
	);

//跳转到指定位置
NPC_API_INTER	NPC_BOOL	NPC_F_NAV_JumpCurPos(
	IN		NPC_HANDLE						in_hNavFile,				//NAV文件句柄
	IN		NPC_DWORD						in_dwRecOffsetTime			//跳转到指定位置，该参数录像偏移时间（秒），相对于文件头的时长
	);

//更新文件头信息
NPC_API_INTER	NPC_BOOL	NPC_F_NAV_UpdateFileHead_RecEndTime(
	IN		NPC_HANDLE						in_hNavFile,				//NAV文件句柄
	IN		PNPC_CHAR						in_pRecEndTime,				//录制结束时间，格式为：YYYY-MM-DD HH:MM:SS
	IN		NPC_DWORD						in_dwRecTimeLen				//录制时长（秒）
	);

//检测文件是否还有数据可读，返回TRUE表示还有数据，FALSE表示没有
NPC_API_INTER	NPC_BOOL	NPC_F_NAV_CheckFileIfEnd(
	IN		NPC_HANDLE						in_hNavFile					//NAV文件句柄
	);

//释放缓冲区
NPC_API_INTER	NPC_VOID	NPC_F_NAV_ReleaseBuf(
	INOUT	PNPC_BYTE*						inout_ppDataBuf				//数据缓冲区
	);


#ifdef __cplusplus
}
#endif


#endif

