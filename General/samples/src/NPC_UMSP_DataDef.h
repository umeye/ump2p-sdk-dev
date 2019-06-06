/*----------------------------------------------------------------------------
 * Copyright(c)  :  NPC CORPORTAION All Rights Reserved                       
 * FileName      :  NPC_UMSP_DataDef.h
 * Version       :  1.0
 * Author        :  CCH
 * DateTime      :  2013-08-10
 * Description   :  UMSP服务器数据定义
 *----------------------------------------------------------------------------*/

#ifndef __NPC_UMSP_DATADEF_H
#define __NPC_UMSP_DATADEF_H

#include "NPC_TypeDef.h"
#include "NPC_SYS_Thread.h"
#include "NPC_SYS_FuncShare.h"
#include "NPC_MEM_Manager.h"
#include "NPC_TOOLS_Queue.h"
#include "NPC_LOG_Server.h"
#include "NPC_FILECAM_Sdk.h"
#include "NPC_NAV_File.h"
#include "NPC_DPS_DevProNetServer.h"
#include "NPC_DPS_PRO_NI_Custom.h"
#include "NPC_TOOLS_Md5.h"


//-------------------------------------------------------------------------------
//常量定义
//#define NPC_D_UMSP_MC_MAX_CAMERA_NUM						32				//最大摄像机数
#define NPC_D_UMSP_MC_MAX_FILE_WORK_NUM						32				//最大文件工作数
#define NPC_D_UMSP_MC_MAX_MP3_FILE_NUM						32				//最大MP3文件数
//#define NPC_D_UMSP_MC_MAX_CAMERA_STREAM_NUM					2				//最大码流数


//-------------------------------------------------------------------------------
//配置参数
typedef struct _NPC_S_UMSP_CONFIG_DATA
{
	NPC_BOOL									bIfDebug;					//是否调试，在调试状态下，会输出很多日志信息，正式使用时需要关闭
	NPC_BOOL									bIfShowStreamLog;			//是否显示流日志，即媒体流收发日志信息
	NPC_BOOL									bIfWriteLogToFile;			//是否写日志到文件
	NPC_CHAR									sLogDir[256];				//日志目录，日志文件名自动产生，格式为Log日期.txt
	NPC_USHORT									usLogPort;					//日志端口号，如果为0，则不启动日志监听端口，大于0则启动日志监听端口服务
	NPC_BOOL									bIfSwitchVideo;				//是否切换视频
	NPC_BOOL									bPwdIfMd5;					//密码是否为MD5，如果为TRUE表示密码为MD5加密后的，FALSE表示密码为明文
	NPC_BOOL									bIfSendAlarmMsg;			//是否发送报警消息
	NPC_INT										iSendAlarmIntervalTime;		//发送报警消息的间隔时间（秒）
	NPC_INT										iMaxFileConnNum;			//最大文件连接数，最大不能超过NPC_D_UMSP_MC_MAX_FILE_WORK_NUM

} NPC_S_UMSP_CONFIG_DATA, *PNPC_S_UMSP_CONFIG_DATA;

//摄像机数据
typedef struct _NPC_S_UMSP_CAMERA_DATA
{
	NPC_BOOL									bIfUsed;					//是否被占用
	NPC_HANDLE									hWorkLock;					//工作锁

	NPC_INT										iChNo;						//通道号
	NPC_INT										iFrameRate;					//帧率
	NPC_BOOL									bIfSupportAudio;			//是否支持音频
	NPC_CHAR									sChName[64];				//通道名称
	NPC_BOOL									bIfPictureFlip;				//是否图象翻转

	NPC_BOOL									iStreamNo;					//码流号

	NPC_BOOL									bMainStreamOpenFlag;		//主码流打开标志
	NPC_BOOL									bSubStreamOpenFlag;			//子码流打开标志
	NPC_BOOL									bThirdStreamOpenFlag;		//第3码流打开标志

	PNPC_VOID									pMcSrvData;					//主控服务数据

	NPC_HANDLE									hNavFile;					//NAV文件句柄
	NPC_TIME_T									tRecStartTime;				//录像开始时间
	NPC_DWORD									dwRecvFrameNum;				//接收帧数
	NPC_BOOL									bIfWriteIFrame;				//是否写入了I帧

	NPC_BOOL									bTalkIfOpen;				//是否打开对讲

	NPC_INT										iMainVideoLevel;			//主码流视频质量等级
	NPC_INT										iSubVideoLevel;				//子码流视频质量等级

	NPC_TIME_T									tPrecSendAlarmMsgTime;		//前一次发送报警消息的时间
	NPC_INT										iAlarmState;				//报警状态

	NPC_S_PVM_IMD_CFG_DEV_CODEC					tDevCodec;					//通道编码信息
	NPC_S_PVM_IMD_CFG_DEV_ALARM_MOTION_DETECT	tMotionDetect;				//移动侦测
	NPC_S_PVM_IMD_CFG_DEV_ALARM_PROBE			tProbe;						//探头输入报警
	NPC_S_PVM_IMD_CFG_DEV_RECORD_PLAN			tDevRecordPlan;				//录像计划
	NPC_S_PVM_IMD_CFG_ALARM_CLOUD_STORAGE		tAlarmCloudStorage;			//报警云存储配置

	NPC_TIME_T									tPrecShowCacheTime;			//前一次显示缓存的时间

} NPC_S_UMSP_CAMERA_DATA, *PNPC_S_UMSP_CAMERA_DATA;

//文件工作数据
typedef struct _NPC_S_UMSP_FILE_WORK_DATA
{
	NPC_BOOL									bIfUsed;					//是否被占用
	NPC_DWORD									dwFileReadId;				//文件读取ID
	NPC_HANDLE									hNavFile;					//NAV文件句柄
	NPC_DWORD									dwFileTimeLen;				//文件时间长度（秒）
	NPC_INT										iFileVideoFrameRate;		//视频帧率
	time_t										tFileBeginRecTime;			//文件开始录像时间
	NPC_BOOL									bIfOnlySendIFrame;			//是否只发I帧
	NPC_INT										iDiscardIFrame;				//丢弃I帧数，0：不丢I帧，1：丢弃1个I帧（即间隔1个I帧传输），2：丢弃2个I帧，如此类推

} NPC_S_UMSP_FILE_WORK_DATA, *PNPC_S_UMSP_FILE_WORK_DATA;

//MP3文件数据
typedef struct _NPC_S_UMSP_MP3_FILE_DATA
{
	NPC_BOOL									bIfUsed;					//是否占用
	NPC_CHAR									sFileName[128];				//文件名
	NPC_BOOL									bIfPlaying;					//是否在播放
	NPC_INT										iVolumeSize;				//音量
	NPC_DWORD									dwUpFileId;					//上传文件ID

	FILE*										pSaveFile;					//保存文件

} NPC_S_UMSP_MP3_FILE_DATA, *PNPC_S_UMSP_MP3_FILE_DATA;

//主控服务数据
typedef struct _NPC_S_UMSP_MCSRV_DATA
{
	NPC_BOOL									bRunFlag;					//运行标志
	NPC_HANDLE									hThread;					//线程句柄

	NPC_S_UMSP_CONFIG_DATA						tConfig;					//配置参数

	NPC_HANDLE									hUmspProSrv;				//UMSP协议服务句柄
	NPC_HANDLE									hFileCamera;				//文件摄像机句柄
	NPC_INT										iChNum;						//通道数

	NPC_S_DPS_DEV_CAP_INFO						tDevCapInfo;				//设备能力信息
	NPC_S_PVM_IMD_CFG_DEV_IP					tDevIp;						//设备IP
	NPC_S_PVM_IMD_CFG_DEV_WIFI					tDevWifi;					//设备WIFI
	NPC_TIME_T									tDevTime;					//设备时间

	NPC_S_UMSP_CAMERA_DATA						tCameraTable[NPC_D_DPS_MAX_CHANNEL_NUM+1];			//摄像机表
	NPC_S_UMSP_FILE_WORK_DATA					tFileWorkTable[NPC_D_UMSP_MC_MAX_FILE_WORK_NUM];	//文件工作表

	NPC_BOOL									bAlarmIfOpen;				//是否打开布防

	NPC_S_UMSP_MP3_FILE_DATA					tMp3FileTable[NPC_D_UMSP_MC_MAX_MP3_FILE_NUM];	//MP3文件表
	NPC_INT										iMp3FileNum;									//MP3文件数

	NPC_TIME_T									tPrecReadAlarmStateTime;	//前一次读取报警状态的时间
	NPC_TIME_T									tPrecReadConfigTime;		//前一次读取配置的时间

	NPC_UINT									uiAlarmSeqNo;				//报警序号
	NPC_CHAR									sImportCfgBuf[128];			//导入的配置

} NPC_S_UMSP_MCSRV_DATA, *PNPC_S_UMSP_MCSRV_DATA;



#endif

