/*----------------------------------------------------------------------------
 * Copyright(c)  :  NPC CORPORTAION All Rights Reserved                       
 * FileName      :  NPC_DISP_Sdk_DataDef.h
 * Version       :  1.1
 * Author        :  CCH
 * DateTime      :  2011-08-29
 * Description   :  调度SDK数据定义
 *----------------------------------------------------------------------------*/

#ifndef __NPC_DISP_SDK_DATADEF_H
#define __NPC_DISP_SDK_DATADEF_H

#include "NPC_TypeDef.h"
#include "NPC_DEF_CM_DataDef.h"


//--------------------------------------------------------------------------------
//设备ID的主类型定义
#define NPC_D_DISP_SDK_DEVMAINTYPE_SERVER					1								//服务器类型（调度服务器/网关/存储服务器/流媒体服务器）
#define NPC_D_DISP_SDK_DEVMAINTYPE_DEV						3								//设备类型（DVR/IPC/平台）
#define NPC_D_DISP_SDK_DEVMAINTYPE_CAMERA					5								//摄像机类型
#define NPC_D_DISP_SDK_DEVMAINTYPE_CLIENT					6								//调度客户端类型（如录像控制服务器、业务接口服务器、用户客户端）


//设备ID的子类型定义
//服务器子类型
#define NPC_D_DISP_SDK_DEVSUBTYPE_SR_DISP					1								//调度服务器
#define NPC_D_DISP_SDK_DEVSUBTYPE_SR_GATEWAY				2								//网关
#define NPC_D_DISP_SDK_DEVSUBTYPE_SR_STORE					3								//存储服务器
#define NPC_D_DISP_SDK_DEVSUBTYPE_SR_MEDIA					4								//流媒体服务器

//设备子类型
#define NPC_D_DISP_SDK_DEVSUBTYPE_DV_DVR					1								//DVR
#define NPC_D_DISP_SDK_DEVSUBTYPE_DV_IPC					2								//IPC
#define NPC_D_DISP_SDK_DEVSUBTYPE_DV_FLAT					3								//平台

//客户端子类型
#define NPC_D_DISP_SDK_DEVSUBTYPE_CL_REC					1								//录像控制服务器
#define NPC_D_DISP_SDK_DEVSUBTYPE_CL_SIG					2								//信令服务器
#define NPC_D_DISP_SDK_DEVSUBTYPE_CL_INTER					6								//业务接口服务器
#define NPC_D_DISP_SDK_DEVSUBTYPE_CL_USER					8								//用户客户端


//--------------------------------------------------------------------------------
//录像文件常量定义
#define NPC_D_DISP_SDK_RECFILE_FILE_FLAG					0x4143504E						//录像文件头标志，即为NPCA
#define NPC_D_DISP_SDK_RECFILE_FIELD_FLAG					0x01000000						//录像文件段标志
#define NPC_D_DISP_SDK_RECFILE_FRAME_HEAD_TYPE				0xEC							//录像帧头类型
#define NPC_D_DISP_SDK_RECFILE_INDEX_HEAD_TYPE				0xED							//录像索引表头类型
#define NPC_D_DISP_SDK_RECFILE_INDEX_TABLE_SIZE				8192							//缺省录像索引表大小
#define NPC_D_DISP_SDK_RECFILE_CACHE_MEDIA_SIZE				1024*1024*4						//缺省缓存媒体数据大小

//--------------------------------------------------------------------------------
//RTSP参数常量定义
#define NPC_D_DISP_SDK_RTSP_VEDIO_SDP_ID					96								//视频SDPID
#define NPC_D_DISP_SDK_RTSP_AUDIO_SDP_ID					97								//音频SDPID

#define NPC_D_DISP_SDK_RTSP_NAV_VEDIO_SDP_ID				180								//厂商格式视频SDPID
#define NPC_D_DISP_SDK_RTSP_NAV_AUDIO_SDP_ID				181								//厂商格式音频SDPID

#define NPC_D_DISP_SDK_RTSP_DEF_VEDIO_TIMESTAMP				90000							//缺省视频时间戳参考值

#define NPC_D_DISP_SDK_RTSP_VEDIO_TRACK_ID					0								//视频TRACKID
#define NPC_D_DISP_SDK_RTSP_AUDIO_TRACK_ID					1								//音频TRACKID

//--------------------------------------------------------------------------------
//视频编码常量定义
#define NPC_D_DISP_SDK_VEDIO_CODEC_H264						0x0101							//H264编码
#define NPC_D_DISP_SDK_VEDIO_CODEC_MPEG4					0x0102							//MPEG4编码
#define NPC_D_DISP_SDK_VEDIO_CODEC_H263						0x0103							//H263编码

//音频编码常量定义
#define NPC_D_DISP_SDK_AUDIO_ENCODE_PCM_S16					0x0201							//16位线性PCM
#define NPC_D_DISP_SDK_AUDIO_ENCODE_PCM_ALAW				0x0202							//8位A率PCM（G711A）
#define NPC_D_DISP_SDK_AUDIO_ENCODE_PCM_ULAW				0x0203							//8位U率PCM（G711U）
#define NPC_D_DISP_SDK_AUDIO_ENCODE_G721					0x0204							//G721
#define NPC_D_DISP_SDK_AUDIO_ENCODE_G723_1					0x0205							//G723.1
#define NPC_D_DISP_SDK_AUDIO_ENCODE_G726					0x0206							//G726
#define NPC_D_DISP_SDK_AUDIO_ENCODE_G729					0x0207							//G729
#define NPC_D_DISP_SDK_AUDIO_ENCODE_AAC						0x0208							//AAC
#define NPC_D_DISP_SDK_AUDIO_ENCODE_AMR						0x0209							//AMR

//录像文件的媒体头的媒体类型常量定义
#define NPC_D_DISP_SDK_RECFILE_MEDIATYPE_VEDIO				0								//视频
#define NPC_D_DISP_SDK_RECFILE_MEDIATYPE_AUDIO				1								//音频


//--------------------------------------------------------------------------------
//实时预览控制代码定义
#define NPC_D_DISP_SDK_REALPLAY_CTRLCODE_STOP				1								//停止
#define NPC_D_DISP_SDK_REALPLAY_CTRLCODE_PAUSE				2								//暂停
#define NPC_D_DISP_SDK_REALPLAY_CTRLCODE_CONTINUE			3								//继续
#define NPC_D_DISP_SDK_REALPLAY_CTRLCODE_START_AUDIO		4								//启动音频
#define NPC_D_DISP_SDK_REALPLAY_CTRLCODE_STOP_AUDIO			5								//停止音频

//--------------------------------------------------------------------------------
//录像回放控制代码定义
#define NPC_D_DISP_SDK_PLAYBACK_CTRLCODE_STOP				1								//停止
#define NPC_D_DISP_SDK_PLAYBACK_CTRLCODE_PAUSE				2								//暂停
#define NPC_D_DISP_SDK_PLAYBACK_CTRLCODE_CONTINUE			3								//继续
#define NPC_D_DISP_SDK_PLAYBACK_CTRLCODE_FAST_FORWARD		4								//快进
#define NPC_D_DISP_SDK_PLAYBACK_CTRLCODE_SLOW_FORWARD		5								//慢进
#define NPC_D_DISP_SDK_PLAYBACK_CTRLCODE_FAST_BACK			6								//快退
#define NPC_D_DISP_SDK_PLAYBACK_CTRLCODE_SLOW_BACK			7								//慢退
#define NPC_D_DISP_SDK_PLAYBACK_CTRLCODE_RESUME				8								//恢复正常速率
#define NPC_D_DISP_SDK_PLAYBACK_CTRLCODE_FRAME_FORWARD		9								//单帧进
#define NPC_D_DISP_SDK_PLAYBACK_CTRLCODE_FRAME_BACK			10								//单帧退
#define NPC_D_DISP_SDK_PLAYBACK_CTRLCODE_SEEK				11								//跳转播放位置
#define NPC_D_DISP_SDK_PLAYBACK_CTRLCODE_SET_FRAMERATE		12								//设置帧率

//--------------------------------------------------------------------------------
//录像下载控制代码定义
#define NPC_D_DISP_SDK_RECDOWN_CTRLCODE_STOP				1								//停止
#define NPC_D_DISP_SDK_RECDOWN_CTRLCODE_PAUSE				2								//暂停
#define NPC_D_DISP_SDK_RECDOWN_CTRLCODE_CONTINUE			3								//继续

//--------------------------------------------------------------------------------
//云台预置点命令定义
#define NPC_D_DISP_SDK_PTZPRESET_SET_PRESET					0								//设置预置点 
#define NPC_D_DISP_SDK_PTZPRESET_CLE_PRESET					1								//清除预置点 
#define NPC_D_DISP_SDK_PTZPRESET_GOTO_PRESET				2								//转到预置点 

//--------------------------------------------------------------------------------
//音视频格式数据结构
typedef struct _NPC_S_DISP_SDK_MN_AV_FORMAT
{
	NPC_BOOL							bVedioFlag;											//视频标志，TRUE表示有视频，FALSE表示没有视频
	NPC_BOOL							bAudioFlag;											//音频标志，TRUE表示有音频，FALSE表示没有音频

	NPC_INT								iVedioCodecId;										//视频编码参数，0：默认编码，1：H264，2：MPEG4，3：H263。。。
	NPC_USHORT							usVedioWidth;										//视频宽度
	NPC_USHORT							usVedioHeight;										//视频高度
	NPC_INT								iVedioBitRate;										//视频码率参数，0表示为默认码率
	NPC_INT								iVedioFrameRate;									//视频帧率参数，0表示为默认帧率
	NPC_CHAR							sVedioParams[NPC_D_DEF_CM_VEDIO_PARAMS_BUFSIZE];	//视频参数，BASE64格式，以短号分隔，如果为H264格式，则参数包括SPS和PPS

	NPC_INT								iAudioCodecId;										//音频编码参数，0：默认编码，1：16位线性PCM，2：G711A，3：G711U，4：G721。。。
	NPC_INT								iAudioSampleRate;									//音频采样率
	NPC_INT								iAudioChNum;										//音频通道数，1或2
	NPC_CHAR							sAudioParams[NPC_D_DEF_CM_AUDIO_PARAMS_BUFSIZE];	//音频参数

} NPC_S_DISP_SDK_MN_AV_FORMAT, *PNPC_S_DISP_SDK_MN_AV_FORMAT;

//网关信息数据结构
typedef struct _NPC_S_DISP_SDK_GW_INFO
{
	NPC_CHAR							sGwId[NPC_D_DEF_CM_DEVID_BUFSIZE];					//网关ID
	NPC_CHAR							sGwName[NPC_D_DEF_CM_DEVNAME_BUFSIZE];				//网关名称
	NPC_CHAR							sGwIpAddr[NPC_D_DEF_CM_IPADDR_BUFSIZE];				//网关IP地址
	NPC_USHORT							usGwRtspPort;										//网关RTSP端口号
	NPC_INT								iDevNum;											//设备数

} NPC_S_DISP_SDK_GW_INFO, *PNPC_S_DISP_SDK_GW_INFO;

//设备信息数据结构
typedef struct _NPC_S_DISP_SDK_DEV_INFO
{
	NPC_CHAR							sDevId[NPC_D_DEF_CM_DEVID_BUFSIZE];					//设备ID
	NPC_INT								iDevVendor;											//设备厂商
	NPC_INT								iDevType;											//设备类型
	NPC_CHAR							sDevName[NPC_D_DEF_CM_DEVNAME_BUFSIZE];				//设备名称
	NPC_CHAR							sDevIpAddr[NPC_D_DEF_CM_IPADDR_BUFSIZE];			//设备IP地址
	NPC_USHORT							usDevPort;											//设备端口号
	NPC_CHAR							sDevUserName[NPC_D_DEF_CM_USERNAME_BUFSIZE];		//用户名
	NPC_CHAR							sDevPwd[NPC_D_DEF_CM_USERPWD_BUFSIZE];				//密码
	NPC_CHAR							sExtendParam[NPC_D_DEF_CM_EXTEND_PARAM_BUFSIZE];	//扩展参数
	NPC_INT								iCameraNum;											//摄像机数
	NPC_CHAR							sGwId[NPC_D_DEF_CM_DEVID_BUFSIZE];					//所属网关ID

} NPC_S_DISP_SDK_DEV_INFO, *PNPC_S_DISP_SDK_DEV_INFO;

//摄像机信息数据结构
typedef struct _NPC_S_DISP_SDK_CAMERA_INFO
{
	NPC_CHAR							sCameraId[NPC_D_DEF_CM_DEVID_BUFSIZE];				//摄像机ID
	NPC_INT								iChNo;												//通道号
	NPC_INT								iState;												//摄像机状态，0：停用状态，1：可用状态
	NPC_BOOL							bIfRecing;											//是否在录像
	NPC_CHAR							sExtId[NPC_D_DEF_CM_DEVID_BUFSIZE];					//扩展ID
	NPC_CHAR							sAtDevId[NPC_D_DEF_CM_DEVID_BUFSIZE];				//所在设备ID

} NPC_S_DISP_SDK_CAMERA_INFO, *PNPC_S_DISP_SDK_CAMERA_INFO;

//单元头结构
typedef struct _NPC_S_DISP_SDK_UNIT_HEAD
{
	NPC_INT								iUnitType;											//单元类型，0：网关，1：设备，2：摄像机

} NPC_S_DISP_SDK_UNIT_HEAD, *PNPC_S_DISP_SDK_UNIT_HEAD;

//录像文件信息数据结构
typedef struct _NPC_S_DISP_SDK_RECFILE_INFO
{
	NPC_CHAR							sCameraId[NPC_D_DEF_CM_DEVID_BUFSIZE];				//摄像机ID
	NPC_CHAR							sRecFileName[NPC_D_DEF_CM_FILENAME_BUFSIZE];		//录像文件名
	NPC_CHAR							sRecBeginTime[20];									//录像开始时间，格式为：YYYY-MM-DD HH:MM:SS
	NPC_CHAR							sRecEndTime[20];									//录像结束时间，格式为：YYYY-MM-DD HH:MM:SS
	NPC_INT								iDevVendor;											//设备厂商
	NPC_INT								iRecCodecFormat;									//录像编码格式，0：为标准格式，参见tRecFormat，其它为厂商原始格式，需要用厂商的SDK解码
	NPC_S_DISP_SDK_MN_AV_FORMAT			tRecFormat;											//录像格式
	NPC_INT								iRecPosType;										//录像所在位置类型，0：在网关，1：在设备，2：在存储服务器
	NPC_CHAR							sRecPosDevId[NPC_D_DEF_CM_DEVID_BUFSIZE];			//录像所在位置的设备ID

} NPC_S_DISP_SDK_RECFILE_INFO, *PNPC_S_DISP_SDK_RECFILE_INFO;


//--------------------------------------------------------------------------------
//录像文件头数据结构
typedef struct _NPC_S_DISP_SDK_REC_FILE_HEAD
{
	NPC_DWORD							dwFileFlag;											//文件头标志，值为"NPCA"，参见NPC_D_DISP_SDK_RECFILE_FILE_FLAG定义

	NPC_INT								iDevVendor;											//设备厂商
	NPC_INT								iRecCodecFormat;									//录像编码格式，0：为标准格式，tRecFormat有效，其它为厂商原始格式，需要用厂商的SDK解码
	NPC_S_DISP_SDK_MN_AV_FORMAT			tRecFormat;											//录像格式

	NPC_DWORD							dwTimestampRef;										//时间戳参考值，默认为90000

	NPC_CHAR							sRecBeginTime[20];									//录像开始时间，格式为：YYYY-MM-DD HH:MM:SS
	NPC_CHAR							sRecEndTime[20];									//录像结束时间，格式为：YYYY-MM-DD HH:MM:SS
	NPC_DWORD							dwRecTimeLen;										//录像时长（秒）

	NPC_DWORD							dwFirstIndexOffset;									//第一个索引表字节偏移量，相对于文件头

} NPC_S_DISP_SDK_REC_FILE_HEAD, *PNPC_S_DISP_SDK_REC_FILE_HEAD;

//录像段头数据结构
typedef struct _NPC_S_DISP_SDK_REC_FIELD_HEAD
{
	NPC_DWORD							dwFieldFlag;										//段标识，值为NPC_D_DISP_SDK_RECFILE_FIELD_FLAG
	NPC_BYTE							ucFieldType;										//段类型，值为NPC_D_DISP_SDK_RECFILE_FRAME_HEAD_TYPE或NPC_D_DISP_SDK_RECFILE_INDEX_HEAD_TYPE
	NPC_BYTE							ucReserve;											//备用
	NPC_USHORT							usReserve;											//备用
	NPC_DWORD							dwFieldDataLen;										//段数据长度，不包括该头

} NPC_S_DISP_SDK_REC_FIELD_HEAD, *PNPC_S_DISP_SDK_REC_FIELD_HEAD;

//录像帧头数据结构
typedef struct _NPC_S_DISP_SDK_REC_FRAME_HEAD
{
	NPC_BYTE							ucFrameType;										//帧类型，视频为‘I’，‘P’，‘B’等，音频该参数无效
	NPC_BYTE							ucReserve;											//备用
	NPC_USHORT							usMediaType;										//媒体类型，0：视频，1：音频，参见NPC_D_DISP_SDK_RECFILE_MEDIATYPE_*定义
	NPC_DWORD							dwTimestamp;										//时间戳
	NPC_DWORD							dwRecOffsetTime;									//录像偏移时间（秒），相对于文件头的时长

} NPC_S_DISP_SDK_REC_FRAME_HEAD, *PNPC_S_DISP_SDK_REC_FRAME_HEAD;

//录像索引表头数据结构
typedef struct _NPC_S_DISP_SDK_REC_INDEX_TABLE_HEAD
{
	NPC_DWORD							dwIndexTableSize;									//索引表大小，默认为8192项，即64K字节的索引表
	NPC_DWORD							dwValidityIndexItemNum;								//有效索引表项数
	NPC_DWORD							dwBeginOffsetTime;									//该索引表的录像偏移开始时间（秒），索引表的序号相对该时间的偏移时间
	NPC_DWORD							dwNextIndexOffset;									//下一个索引表偏移位置，相对于文件开始位置，如果为0表示是最后一个索引表

} NPC_S_DISP_SDK_REC_INDEX_TABLE_HEAD, *PNPC_S_DISP_SDK_REC_INDEX_TABLE_HEAD;

//录像索引项数据结构
typedef struct _NPC_S_DISP_SDK_REC_INDEX_ITEM
{
	NPC_DWORD							dwRecDataOffset;									//录像数据偏移量（字节），相对于文件开始位置，索引表的序号为相对时间（秒）

} NPC_S_DISP_SDK_REC_INDEX_ITEM, *PNPC_S_DISP_SDK_REC_INDEX_ITEM;


//--------------------------------------------------------------------------------
//透传数据
typedef struct _NPC_S_DISP_COM_BODY_EXPAND_DATA
{
	NPC_CHAR							sDestDevId[NPC_D_DEF_CM_DEVID_BUFSIZE];				//目标设备ID，该ID为单元ID，如网关ID、DVRID，摄像机ID等
	NPC_CHAR							sSrcDevId[NPC_D_DEF_CM_DEVID_BUFSIZE];				//源设备ID，该ID为单元ID，如网关ID、DVRID，摄像机ID等
	NPC_BOOL							bIfReturnResp;										//是否要求返回应答消息
	NPC_INT								iDataLen;											//发送数据长度

	NPC_INT								iRespResult;										//应答结果，bIfWaitResp参数为TRUE，该参数才有效

} NPC_S_DISP_COM_BODY_EXPAND_DATA, *PNPC_S_DISP_COM_BODY_EXPAND_DATA;


//--------------------------------------------------------------------------------
//话单信息
typedef struct _NPC_S_DISP_COM_BODY_BILL_DATA
{
	NPC_CHAR							sBillId[NPC_D_DEF_CM_BILLID_BUFSIZE];				//话单ID
	NPC_CHAR							sClientIpAddr[NPC_D_DEF_CM_IPADDR_BUFSIZE];			//客户端IP地址
	NPC_CHAR							sMediaName[NPC_D_DEF_CM_FILENAME_BUFSIZE];			//被访问的媒体名称，如摄像机ID或文件名 
	NPC_CHAR							sBeginTime[20];										//访问开始时间，格式为：YYYY-MM-DD HH:MM:SS
	NPC_CHAR							sEndTime[20];										//访问结束时间，格式为：YYYY-MM-DD HH:MM:SS
	NPC_INT								iTimeLen;											//访问时长，单位为秒

} NPC_S_DISP_COM_BODY_BILL_DATA, *PNPC_S_DISP_COM_BODY_BILL_DATA;

//日志消息数据结构
typedef struct _NPC_S_DISP_COM_BODY_LOG_DATA
{
	NPC_CHAR							sLogId[NPC_D_DEF_CM_BILLID_BUFSIZE];				//日志ID
	NPC_CHAR							sDevId[NPC_D_DEF_CM_DEVID_BUFSIZE];					//产生日志的设备ID
	NPC_CHAR							sLogTime[20];										//日志产生的时间 ，格式为：YYYY-MM-DD HH:MM:SS
	NPC_INT								iLogType;											//日志类型，0：严重，1：重要，2：一般，3：提示信息
	NPC_CHAR							sLogInfo[256];										//日志信息，以'\0'结束

} NPC_S_DISP_COM_BODY_LOG_DATA, *PNPC_S_DISP_COM_BODY_LOG_DATA;

//告警信息
typedef struct _NPC_S_DISP_COM_BODY_ALARM_DATA
{
	NPC_CHAR							sAlarmId[NPC_D_DEF_CM_BILLID_BUFSIZE];				//告警ID
	NPC_CHAR							sAlarmTime[20];										//报警产生的时间 ，格式为：YYYY-MM-DD HH:MM:SS
	NPC_INT								iAlarmType;											//报警类型
	NPC_INT								iAlarmLevel;										//重要程度
	NPC_CHAR							sAlarmInfo[256];									//报警信息，以'\0'结束

} NPC_S_DISP_COM_BODY_ALARM_DATA, *PNPC_S_DISP_COM_BODY_ALARM_DATA;

//录像回放进程消息
typedef struct _NPC_S_DISP_COM_BODY_PLAYBACK_DATA
{
	NPC_DWORD							dwMnId;												//管理ID
	NPC_DWORD							dwPlaybackId;										//回放ID
	NPC_CHAR							sRecFileName[NPC_D_DEF_CM_FILENAME_BUFSIZE];		//录像文件名
	NPC_DWORD							dwPlayPos;											//当前播放位置（秒）
	NPC_INT								iPlayFrameRate;										//播放帧率

} NPC_S_DISP_COM_BODY_PLAYBACK_DATA, *PNPC_S_DISP_COM_BODY_PLAYBACK_DATA;

//录像完成通知消息
typedef struct _NPC_S_DISP_COM_BODY_RECCOMPLETE_DATA
{
	NPC_DWORD							dwMnId;												//管理ID
	NPC_S_DISP_SDK_RECFILE_INFO			tRecFileInfo;										//录像文件信息

} NPC_S_DISP_COM_BODY_RECCOMPLETE_DATA, *PNPC_S_DISP_COM_BODY_RECCOMPLETE_DATA;


//--------------------------------------------------------------------------------
//错误码常量定义
#define NPC_D_DISP_SDK_ERROR_CODE_SUCCESS							0						//成功

#define NPC_D_DISP_SDK_ERROR_CODE_GENERAL							1						//一般错误
#define NPC_D_DISP_SDK_ERROR_CODE_SYS_ERROR							2						//系统错误，如分配内存失败，调用系统函数失败等
#define NPC_D_DISP_SDK_ERROR_CODE_PARAM_ERROR						3						//参数错误
#define NPC_D_DISP_SDK_ERROR_CODE_DB_ACC_FAIL						4						//数据库访问失败
#define NPC_D_DISP_SDK_ERROR_CODE_USERORPWD_ERROR					5						//客户端登录，用户名或密码错误
#define NPC_D_DISP_SDK_ERROR_CODE_SEND_MSG_FAIL						6						//发送消息失败
#define NPC_D_DISP_SDK_ERROR_CODE_CLIENT_NO_LOGIN					1000					//客户端未登录
#define NPC_D_DISP_SDK_ERROR_CODE_DEV_ID_EXIST						1001					//设备ID已存在
#define NPC_D_DISP_SDK_ERROR_CODE_GW_ID_EXIST						1002					//网关ID已存在
#define NPC_D_DISP_SDK_ERROR_CODE_GW_ID_NOT_FIND					1003					//网关ID不存在
#define NPC_D_DISP_SDK_ERROR_CODE_DEV_ID_NOT_FIND					1004					//设备ID不存在
#define NPC_D_DISP_SDK_ERROR_CODE_NOT_IDLE_GW						1005					//无空闲网关可用
#define NPC_D_DISP_SDK_ERROR_CODE_ADD_DEV_FAIL						1006					//添加设备失败
#define NPC_D_DISP_SDK_ERROR_CODE_DEL_DEV_FAIL						1007					//删除设备失败
#define NPC_D_DISP_SDK_ERROR_CODE_ADD_CAMERA_FAIL					1008					//添加摄像机失败
#define NPC_D_DISP_SDK_ERROR_CODE_DEL_CAMERA_FAIL					1009					//删除摄像机失败
#define NPC_D_DISP_SDK_ERROR_CODE_CAMERA_ID_EXIST					1010					//摄像机ID已存在
#define NPC_D_DISP_SDK_ERROR_CODE_CAMERA_ID_NOT_FIND				1011					//摄像机ID不存在
#define NPC_D_DISP_SDK_ERROR_CODE_CH_DEV_PARAM_FAIL					1012					//修改设备参数失败
#define NPC_D_DISP_SDK_ERROR_CODE_PTZ_LOCKED						1013					//云台被锁定
#define NPC_D_DISP_SDK_ERROR_CODE_NO_FIND_DEST_ID					1014					//未找到目的ID
#define NPC_D_DISP_SDK_ERROR_CODE_GW_NOT_ONLINE						1015					//网关ID不在线




#endif

