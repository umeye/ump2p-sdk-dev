/*----------------------------------------------------------------------------
 * Copyright(c)  :  NPC CORPORTAION All Rights Reserved                       
 * FileName      :  NPC_MON_MpsDef.h
 * Version       :  1.0
 * Author        :  CCH
 * DateTime      :  2013-08-12
 * Description   :  常用数据结构定义
 *----------------------------------------------------------------------------*/

#ifndef __NPC_MON_MPSDEF_H
#define __NPC_MON_MPSDEF_H

#include "NPC_TypeDef.h"
#include "NPC_MON_ConsDef.h"


#pragma pack(1)

//监控文件信息
typedef struct _NPC_S_MON_MPS_FILE_INFO
{
	NPC_CHAR										sFileFlag[NPC_D_MON_CSD_MAX_FILE_FLAG_LEN];		//文件标识，唯一标识该文件，打开文件时必须原样传回
	NPC_INT											iFileType;					//文件类型，参见NPC_D_MON_CSD_FILE_TYPE_*定义
	NPC_INT											iCreateMode;				//创建方式，参见NPC_D_MON_CSD_FILE_CREATE_MODE_*定义
	NPC_INT											iAlarmEvent;				//报警事件，参见NPC_D_MON_CSD_ALARM_EVENT_*定义，只有创建方式为NPC_D_MON_CSD_FILE_CREATE_MODE_ALARM时有效
	NPC_S_TD_DATE_TIME								tStartTime;					//文件开始时间，拍照文件为拍照的时间
	NPC_S_TD_DATE_TIME								tEndTime;					//文件结束时间，拍照文件该参数无效
	NPC_DWORD										dwFileTimeLen;				//文件总时长（秒），录像文件有效
	NPC_DWORD										dwFileSize;					//文件大小（单位：KB）

	NPC_BOOL										bVideoFlag;					//视频标志，TRUE：表示有视频，FALSE：表示没有视频
	NPC_BOOL										bAudioFlag;					//音频标志，TRUE：表示有音频，FALSE：表示没有音频
	NPC_BOOL										bPictureFlag;				//图片标志，TRUE：表示为图片，FALSE：表示不是图片，如果该参数TRUE，则音视频参数无效

	NPC_INT											iVideoCodecId;				//视频编码ID，参见NPC_D_MON_CSD_CODEC_ID_VEDIO_*定义
	NPC_INT											iVideoFrameRate;			//视频帧率
	NPC_INT											iVideoBitRate;				//视频码率（kbps）
	NPC_USHORT										usVideoWidth;				//视频宽度
	NPC_USHORT										usVideoHeight;				//视频高度

	NPC_INT											iAudioCodecId;				//音频编码ID，参见NPC_D_MON_CSD_CODEC_ID_AUDIO_*定义
	NPC_INT											iAudioBitRate;				//音频码率（kbps）
	NPC_INT											iAudioSampleRate;			//音频采样率
	NPC_INT											iAudioChNum;				//音频声道数

	NPC_INT											iPictureFormat;				//图片格式，参见NPC_D_MON_CSD_PIC_FORMAT_*定义

} NPC_S_MON_MPS_FILE_INFO, *PNPC_S_MON_MPS_FILE_INFO;

//文件头信息
typedef struct _NPC_S_MON_MPS_FILE_HEAD_INFO
{
	NPC_CHAR										sFileFlag[128];				//文件标识，唯一标识该文件，打开文件时必须原样传回，如文件名
	NPC_USHORT										usChNo;						//通道号
	NPC_BYTE										ucStreamNo;					//码流号
	NPC_BYTE										ucFileType;					//文件类型，参见NPC_D_MON_CSD_FILE_TYPE_*定义
	NPC_BYTE										ucCreateMode;				//创建方式，参见NPC_D_MON_CSD_FILE_CREATE_MODE_*定义
	NPC_BYTE										ucAlarmEvent;				//报警事件，参见NPC_D_MON_CSD_ALARM_EVENT_*定义，只有创建方式为NPC_D_MON_CSD_FILE_CREATE_MODE_ALARM时有效
	NPC_BYTE										ucReserve1[2];				//备用1
	NPC_S_TD_DATE_TIME								tStartTime;					//文件开始时间，拍照文件为拍照的时间
	NPC_S_TD_DATE_TIME								tEndTime;					//文件结束时间，拍照文件该参数无效
	NPC_UINT										uiFileTimeLen;				//文件总时长（秒），录像文件有效
	NPC_UINT										uiFileSize;					//文件大小（单位：KB）

	//以下参数在ucFileType值为NPC_D_MON_CSD_FILE_TYPE_RECORD时有效
	NPC_BYTE										ucVideoFlag;				//视频标志，1：表示有视频，0：表示没有视频
	NPC_BYTE										ucAudioFlag;				//音频标志，1：表示有音频，0：表示没有音频

	NPC_BYTE										ucVideoCodecId;				//视频编码ID，参见NPC_D_MON_CSD_CODEC_ID_VEDIO_*定义
	NPC_BYTE										ucVideoFrameRate;			//视频帧率
	NPC_UINT										uiVideoBitRate;				//视频码率（kbps）
	NPC_USHORT										usVideoWidth;				//视频宽度
	NPC_USHORT										usVideoHeight;				//视频高度

	NPC_USHORT										usAudioCodecId;				//音频编码ID，参见NPC_D_MON_CSD_CODEC_ID_AUDIO_*定义
	NPC_USHORT										usAudioBitRate;				//音频码率（kbps）
	NPC_UINT										uiAudioSampleRate;			//音频采样率
	NPC_USHORT										usAudioChNum;				//音频声道数

	//以下参数在ucFileType值为NPC_D_MON_CSD_FILE_TYPE_PICTRUE时有效
	NPC_BYTE										ucPictureFormat;			//图片格式，参见NPC_D_MON_CSD_PIC_FORMAT_*定义
	NPC_BYTE										ucReserve2;					//备用2

} NPC_S_MON_MPS_FILE_HEAD_INFO, *PNPC_S_MON_MPS_FILE_HEAD_INFO;

//录像文件时间范围
typedef struct _NPC_S_MON_MPS_FILE_RANGE_INFO
{
	NPC_DWORD										dwFileFlag;					//标识	
	NPC_INT											iCreateMode;				//创建方式，参见NPC_D_MON_CSD_FILE_CREATE_MODE_*定义
	NPC_INT											iAlarmEvent;				//报警事件，参见NPC_D_MON_CSD_ALARM_EVENT_*定义，只有创建方式为NPC_D_MON_CSD_FILE_CREATE_MODE_ALARM时有效

	NPC_S_TD_DATE_TIME								tStartTime;					//开始时间，以文件创建时间或录像的开始时间为查询时间
	NPC_S_TD_DATE_TIME								tEndTime;					//结束时间
}NPC_S_MON_MPS_FILE_RANGE_INFO,*PNPC_S_MON_MPS_FILE_RANGE_INFO;


#pragma pack()

#endif

