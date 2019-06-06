/*----------------------------------------------------------------------------
 * Copyright(c)  :  NPC CORPORTAION All Rights Reserved                       
 * FileName      :  NPC_DPS_DevProtocolDef.h
 * Version       :  1.0
 * Author        :  CCH
 * DateTime      :  2013-07-16
 * Description   :  UMSP设备协议交换数据定义
 *----------------------------------------------------------------------------*/

#ifndef __NPC_DPS_DEVPROTOCOLDEF_H
#define __NPC_DPS_DEVPROTOCOLDEF_H

#include "NPC_TypeDef.h"
#include "NPC_MON_ConsDef.h"
#include "NPC_MON_MpsDef.h"
#include "NPC_PVM_OrderCfgDef.h"


//-------------------------------------------------------------------------------
//消息类型定义
#define NPC_D_DPS_MSG_TYPE_REQUEST								0					//请求消息
#define NPC_D_DPS_MSG_TYPE_RESPONSION							1					//应答消息


//-------------------------------------------------------------------------------
//事件消息ID定义
#define NPC_D_DPS_MSGID_EVENT_E2_OPEN_MEDIA_STREAM				0x0201				//打开媒体流（打开实时预览）
#define NPC_D_DPS_MSGID_EVENT_E2_CLOSE_MEDIA_STREAM				0x0202				//关闭媒体流（关闭实时预览）
#define NPC_D_DPS_MSGID_EVENT_E2_PTZ_CTRL						0x0203				//云台控制
#define NPC_D_DPS_MSGID_EVENT_E2_FORCE_I_FRAME					0x0204				//请求强制发送I帧
#define NPC_D_DPS_MSGID_EVENT_E2_MODIFY_CODEC					0x0205				//修改编码参数（上层请求SDK）
#define NPC_D_DPS_MSGID_EVENT_E2_GET_VIDEO_QUALITY_CAP			0x0206				//获取视频质量等级能力列表
#define NPC_D_DPS_MSGID_EVENT_E2_GET_VIDEO_QUALITY_LEVEL		0x0207				//获取视频质量等级
#define NPC_D_DPS_MSGID_EVENT_E2_SET_VIDEO_QUALITY_LEVEL		0x0208				//设置视频质量等级

#define NPC_D_DPS_MSGID_EVENT_E2_FILE_QUERY						0x0211				//查询文件（录像或图片文件）
#define NPC_D_DPS_MSGID_EVENT_E2_NSP_FILE_QUERY					0x0212				//扩展查询文件（录像或图片文件）
#define NPC_D_DPS_MSGID_EVENT_E2_FILE_RANGE_QUERY				0x0213				//查询录像文件时间范围（与设备能力关联）

#define NPC_D_DPS_MSGID_EVENT_E2_FILE_OPEN						0x0221				//打开文件
#define NPC_D_DPS_MSGID_EVENT_E2_FILE_CLOSE						0x0222				//关闭文件
#define NPC_D_DPS_MSGID_EVENT_E2_FILE_READ						0x0223				//读取文件
#define NPC_D_DPS_MSGID_EVENT_E2_FILE_SEEK						0x0224				//定位文件
#define NPC_D_DPS_MSGID_EVENT_E2_FILE_QVIDEO					0x0225				//查询视频参数
#define NPC_D_DPS_MSGID_EVENT_E2_FILE_ONLY_IFRAME				0x0226				//请求I帧数据
#define NPC_D_DPS_MSGID_EVENT_E2_FILE_RECOVER_ALLFRAME			0x0227				//请求恢复全帧数据
#define NPC_D_DPS_MSGID_EVENT_E2_FILE_OPEN_BY_TIME				0x0228				//打开文件,按时间方式（与设备能力关联）
#define NPC_D_DPS_MSGID_EVENT_E2_FILE_SEEK_BY_TIME				0x0229				//定位文件,按时间方式（与设备能力关联）

#define NPC_D_DPS_MSGID_EVENT_E2_TALK_START						0x0231				//开始语音对讲
#define NPC_D_DPS_MSGID_EVENT_E2_TALK_STOP						0x0232				//停止语音对讲

#define NPC_D_DPS_MSGID_EVENT_E2_GUARD							0x0241				//布防（暂停使用）
#define NPC_D_DPS_MSGID_EVENT_E2_UNGUARD						0x0242				//撤防（暂停使用）
#define NPC_D_DPS_MSGID_EVENT_E2_ALARM_INFO						0x0243				//告警消息
#define NPC_D_DPS_MSGID_EVENT_E2_EX_ALARM_INFO					0x0244				//告警消息（扩展）

#define NPC_D_DPS_MSGID_EVENT_E3_MODIFY_USER_PWD				0x0301				//修改用户密码
#define NPC_D_DPS_MSGID_EVENT_E3_GET_CONFIG						0x0302				//获取配置参数
#define NPC_D_DPS_MSGID_EVENT_E3_SET_CONFIG						0x0303				//设置配置参数
#define NPC_D_DPS_MSGID_EVENT_E3_GET_DEF_CONFIG					0x0304				//获取缺省配置参数
#define NPC_D_DPS_MSGID_EVENT_E3_IEC_IMP_CONFIG					0x0305				//导入配置
#define NPC_D_DPS_MSGID_EVENT_E3_IEC_EXP_CONFIG					0x0306				//导出配置

#define NPC_D_DPS_MSGID_EVENT_E3_QUERY_NET_INFO					0x0311				//查询网络信息
#define NPC_D_DPS_MSGID_EVENT_E3_MODIFY_NET_ADDR				0x0312				//修改网络地址

#define NPC_D_DPS_MSGID_EVENT_E3_COM_OPEN						0x0321				//打开串口
#define NPC_D_DPS_MSGID_EVENT_E3_COM_CLOSE						0x0322				//关闭串口
#define NPC_D_DPS_MSGID_EVENT_E3_COM_SEND_DATA					0x0323				//发送数据(客户端发送给设备端)
#define NPC_D_DPS_MSGID_EVENT_E3_COM_SUBS_DATA					0x0324				//设备串口数据(设备端发送给客户端)(无应答)

#define NPC_D_DPS_MSGID_EVENT_E3_UNS_ADD_USER					0x0331				//增加用户
#define NPC_D_DPS_MSGID_EVENT_E3_UNS_DEL_USER					0x0332				//删除用户
#define NPC_D_DPS_MSGID_EVENT_E3_UNS_GET_USER_POP				0x0333				//获取用户权限列表
#define NPC_D_DPS_MSGID_EVENT_E3_UNS_SET_USER_POP				0x0334				//设置用户权限列表

#define NPC_D_DPS_MSGID_EVENT_E7_CUSTOM							0x0701				//定制功能

#define NPC_D_DPS_MSGID_EVENT_E7_RMY_QUERY_ABILITY				0x0721				//查询能力集
#define NPC_D_DPS_MSGID_EVENT_E7_RMY_REBOOT						0x0722				//重启设备(无包体)(无应答消息)
#define NPC_D_DPS_MSGID_EVENT_E7_RMY_STOP						0x0723				//停止设备(无包体)(无应答消息)

#define NPC_D_DPS_MSGID_EVENT_E7_DUP_QUERY_DEV_VER_INFO			0x0731				//查询设备版本信息
#define NPC_D_DPS_MSGID_EVENT_E7_DUP_REQU_UPGRADE				0x0732				//请求升级
#define NPC_D_DPS_MSGID_EVENT_E7_DUP_QUERY_PROGRESS				0x0733				//查询升级进度
#define NPC_D_DPS_MSGID_EVENT_E7_DUP_QUERY_TRANS_POS			0x0734				//查询传输位置
#define NPC_D_DPS_MSGID_EVENT_E7_DUP_UPGRADE_DATA				0x0735				//升级数据包

#define NPC_D_DPS_MSGID_MEDIA_E8_VIDEO_FRAME					0x0801				//视频帧数据
#define NPC_D_DPS_MSGID_MEDIA_E8_AUDIO_FRAME					0x0802				//音频帧数据
#define NPC_D_DPS_MSGID_MEDIA_E8_VIDEO_FRAME_EX					0x0811				//视频帧数据带时间戳
#define	NPC_D_DPS_MSGID_MEDIA_E8_AUDIO_FRAME_EX					0x0812				//音频时数据带时间戳


//-------------------------------------------------------------------------------
//事件消息头定义
typedef struct _NPC_S_DPS_MSG_HEAD
{
	NPC_INT											iMsgFuncId;						//消息功能ID，参见NPC_D_DPS_MSGID_*定义
	NPC_BYTE										ucMsgType;						//消息类型，参见NPC_D_DPS_MSG_TYPE_*定义
	NPC_BYTE										ucResult;						//结果，0：成功，其它失败，参见NPC_D_MON_CSD_DPS_ERROR_CODE_*定义
	NPC_BYTE										ucReserve[2];					//备用
	NPC_DWORD										dwTransId;						//事务ID，必须应答消息原样返回请求消息中的事务ID
	NPC_INT											iMsgBodyLen;					//消息包体长度

} NPC_S_DPS_MSG_HEAD, *PNPC_S_DPS_MSG_HEAD;


//-------------------------------------------------------------------------------
//事件消息体定义

//打开媒体流
typedef struct _NPC_S_DPS_MSG_BODY_EVENT_E2_OPEN_MEDIA_STREAM
{
	NPC_INT											i_iChNo;						//通道号
	NPC_INT											i_iStreamNo;					//码流号，0：主码流，1：子码流

	NPC_BOOL										o_bVideoFlag;					//视频标志，TRUE：表示有视频，FALSE：表示没有视频
	NPC_BOOL										o_bAudioFlag;					//音频标志，TRUE：表示有音频，FALSE：表示没有音频

	NPC_INT											o_iVideoCodecId;				//视频编码ID，参见NPC_D_MON_CSD_CODEC_ID_*定义
	NPC_INT											o_iVideoFrameRate;				//视频帧率
	NPC_INT											o_iVideoBitRate;				//视频码率（kbps）
	NPC_USHORT										o_usVideoWidth;					//视频宽度
	NPC_USHORT										o_usVideoHeight;				//视频高度

	NPC_INT											o_iAudioCodecId;				//音频编码ID，参见NPC_D_MON_CSD_CODEC_ID_*定义
	NPC_INT											o_iAudioBitRate;				//音频码率（kbps），如64
	NPC_INT											o_iAudioSampleRate;				//音频采样率，如8000
	NPC_INT											o_iAudioChNum;					//音频声道数，如1

} NPC_S_DPS_MSG_BODY_EVENT_E2_OPEN_MEDIA_STREAM, *PNPC_S_DPS_MSG_BODY_EVENT_E2_OPEN_MEDIA_STREAM;

//关闭媒体流
typedef struct _NPC_S_DPS_MSG_BODY_EVENT_E2_CLOSE_MEDIA_STREAM
{
	NPC_INT											i_iChNo;						//通道号
	NPC_INT											i_iStreamNo;					//码流号，0：主码流，1：子码流

} NPC_S_DPS_MSG_BODY_EVENT_E2_CLOSE_MEDIA_STREAM, *PNPC_S_DPS_MSG_BODY_EVENT_E2_CLOSE_MEDIA_STREAM;

//云台控制
typedef struct _NPC_S_DPS_MSG_BODY_EVENT_E2_PTZ_CTRL
{
	NPC_INT											i_iChNo;						//通道号
	NPC_INT											i_iPtzCmd;						//PTZ命令，参见NPC_D_MON_CSD_PTZ_CMD_*定义
	NPC_DWORD										i_dwPtzData;					//PTZ数据
	NPC_DWORD										i_dwPtzExtParam1;				//PTZ扩展参数1
	NPC_DWORD										i_dwPtzExtParam2;				//PTZ扩展参数2

} NPC_S_DPS_MSG_BODY_EVENT_E2_PTZ_CTRL, *PNPC_S_DPS_MSG_BODY_EVENT_E2_PTZ_CTRL;

//请求强制发送I帧
typedef struct _NPC_S_DPS_MSG_BODY_EVENT_E2_FORCE_I_FRAME
{
	NPC_INT											i_iChNo;						//通道号
	NPC_INT											i_iStreamNo;					//码流号，0：主码流，1：子码流

} NPC_S_DPS_MSG_BODY_EVENT_E2_FORCE_I_FRAME, *PNPC_S_DPS_MSG_BODY_EVENT_E2_FORCE_I_FRAME;

//修改编码参数
typedef struct _NPC_S_DPS_MSG_BODY_EVENT_E2_MODIFY_CODEC
{
	NPC_INT											i_iChNo;						//通道号
	NPC_INT											i_iStreamNo;					//码流号，0：主码流，1：子码流

	NPC_BOOL										i_bVideoFlag;					//视频标志，TRUE：表示有视频，FALSE：表示没有视频
	NPC_BOOL										i_bAudioFlag;					//音频标志，TRUE：表示有音频，FALSE：表示没有音频

	NPC_INT											i_iVideoCodecId;				//视频编码ID，参见NPC_D_MON_CSD_CODEC_ID_*定义
	NPC_INT											i_iVideoFrameRate;				//视频帧率
	NPC_INT											i_iVideoBitRate;				//视频码率（kbps）
	NPC_USHORT										i_usVideoWidth;					//视频宽度
	NPC_USHORT										i_usVideoHeight;				//视频高度

	NPC_INT											i_iAudioCodecId;				//音频编码ID，参见NPC_D_MON_CSD_CODEC_ID_*定义
	NPC_INT											i_iAudioBitRate;				//音频码率（kbps），如64
	NPC_INT											i_iAudioSampleRate;				//音频采样率，如8000
	NPC_INT											i_iAudioChNum;					//音频声道数，如1

} NPC_S_DPS_MSG_BODY_EVENT_E2_MODIFY_CODEC, *PNPC_S_DPS_MSG_BODY_EVENT_E2_MODIFY_CODEC;

//获取视频质量等级能力列表
typedef struct _NPC_S_DPS_MSG_BODY_EVENT_E2_GET_VIDEO_QUALITY_CAP
{
	NPC_USHORT										i_usChNo;						//通道号
	NPC_USHORT										i_usStreamNo;					//码流号，0：主码流，1：子码流

	//视频质量等级列表
	NPC_INT											o_iLevelNum;					//等级数
	struct _LevelTable{
		NPC_CHAR									sLevelName[16];					//等级名称
		NPC_INT										iLevelValue;					//等级值，必须大于等于0
		NPC_USHORT									usWidth;						//图像宽
		NPC_USHORT									usHeight;						//图像高
		NPC_USHORT									usFrameRate;					//帧率
		NPC_USHORT									usBitRate;						//码率，单位：kbps
	} o_tLevelTable[8];

} NPC_S_DPS_MSG_BODY_EVENT_E2_GET_VIDEO_QUALITY_CAP, *PNPC_S_DPS_MSG_BODY_EVENT_E2_GET_VIDEO_QUALITY_CAP;

//获取视频质量等级
typedef struct _NPC_S_DPS_MSG_BODY_EVENT_E2_GET_VIDEO_QUALITY_LEVEL
{
	NPC_USHORT										i_usChNo;						//通道号
	NPC_USHORT										i_usStreamNo;					//码流号，0：主码流，1：子码流

	NPC_INT											o_iLevelValue;					//等级值，如果为-1表示缺省设置，即没有设置等级

} NPC_S_DPS_MSG_BODY_EVENT_E2_GET_VIDEO_QUALITY_LEVEL, *PNPC_S_DPS_MSG_BODY_EVENT_E2_GET_VIDEO_QUALITY_LEVEL;

//设置视频质量等级
typedef struct _NPC_S_DPS_MSG_BODY_EVENT_E2_SET_VIDEO_QUALITY_LEVEL
{
	NPC_USHORT										i_usChNo;						//通道号
	NPC_USHORT										i_usStreamNo;					//码流号，0：主码流，1：子码流
	NPC_INT											i_iLevelValue;					//等级值，大于等于0，或者为-1，-1表示缺省设置

} NPC_S_DPS_MSG_BODY_EVENT_E2_SET_VIDEO_QUALITY_LEVEL, *PNPC_S_DPS_MSG_BODY_EVENT_E2_SET_VIDEO_QUALITY_LEVEL;


//-------------------------------------------------------------------------------
//查询文件（录像或图片文件），应答消息中该结构体之后为NPC_S_MON_MPS_FILE_INFO数组
typedef struct _NPC_S_DPS_MSG_BODY_EVENT_E2_FILE_QUERY
{
	NPC_INT											i_iChNo;						//通道号，如果为-1表示查询所有通道的文件
	NPC_INT											i_iFileType;					//文件类型，参见NPC_D_MON_CSD_FILE_TYPE_*定义
	NPC_INT											i_iCreateMode;					//创建方式，参见NPC_D_MON_CSD_FILE_CREATE_MODE_*定义
	NPC_INT											i_iAlarmEvent;					//报警事件，参见NPC_D_MON_CSD_ALARM_EVENT_*定义，只有创建方式为NPC_D_MON_CSD_FILE_CREATE_MODE_ALARM时有效
	NPC_S_TD_DATE_TIME								i_tStartTime;					//开始时间，以文件创建时间或录像的开始时间为查询时间
	NPC_S_TD_DATE_TIME								i_tEndTime;						//结束时间

	NPC_INT											o_iFileNum;						//文件数

} NPC_S_DPS_MSG_BODY_EVENT_E2_FILE_QUERY, *PNPC_S_DPS_MSG_BODY_EVENT_E2_FILE_QUERY;

//扩展查询文件（录像或图片文件），应答消息中该结构体之后为NPC_S_MON_MPS_FILE_HEAD_INFO数组
typedef struct _NPC_S_DPS_MSG_BODY_EVENT_E2_NSP_FILE_QUERY
{
	NPC_INT											i_iChNo;						//通道号，如果为-1表示查询所有通道的文件
	NPC_INT											i_iStreamNo;					//码流号，如果为-1表示查询所有码流的文件
	NPC_INT											i_iFileType;					//文件类型，参见NPC_D_MON_CSD_FILE_TYPE_*定义
	NPC_INT											i_iCreateMode;					//创建方式，参见NPC_D_MON_CSD_FILE_CREATE_MODE_*定义
	NPC_BYTE										i_ucAlarmEventTable[16];		//报警事件表，参见NPC_D_MON_CSD_ALARM_EVENT_*定义，只有创建方式为NPC_D_MON_CSD_FILE_CREATE_MODE_ALARM时有效
	NPC_INT											i_iAlarmEventNum;				//报警事件数
	NPC_S_TD_DATE_TIME								i_tStartTime;					//开始时间，以文件创建时间或录像的开始时间为查询时间
	NPC_S_TD_DATE_TIME								i_tEndTime;						//结束时间

	NPC_INT											o_iFileNum;						//文件数

} NPC_S_DPS_MSG_BODY_EVENT_E2_NSP_FILE_QUERY, *PNPC_S_DPS_MSG_BODY_EVENT_E2_NSP_FILE_QUERY;

//查询录像文件的时间范围，应答消息中该结构体之后为NPC_S_MON_MPS_FILE_RANGE_INFO数组
typedef struct _NPC_S_DPS_MSG_BODY_EVENT_E2_FILE_RANGE_QUERY
{
	NPC_INT											i_iChNo;						//通道号，如果为-1表示查询所有通道的文件
	NPC_INT											i_iStreamNo;					//码流号，如果为-1表示查询所有码流的文件
	NPC_INT											i_iCreateMode;					//创建方式，参见NPC_D_MON_CSD_FILE_CREATE_MODE_*定义
	NPC_BYTE										i_ucAlarmEventTable[16];		//报警事件表，参见NPC_D_MON_CSD_ALARM_EVENT_*定义，只有创建方式为NPC_D_MON_CSD_FILE_CREATE_MODE_ALARM时有效
	NPC_INT											i_iAlarmEventNum;				//报警事件数
	NPC_S_TD_DATE_TIME								i_tStartTime;					//开始时间，以文件创建时间或录像的开始时间为查询时间
	NPC_S_TD_DATE_TIME								i_tEndTime;						//结束时间
	
	NPC_INT											o_iFileNum;						//文件数
	
} NPC_S_DPS_MSG_BODY_EVENT_E2_FILE_RANGE_QUERY, *PNPC_S_DPS_MSG_BODY_EVENT_E2_FILE_RANGE_QUERY;

//-------------------------------------------------------------------------------
//打开文件，可能同时对同一个文件打开多次的可能，即不同的客户端对同一个文件播放
typedef struct _NPC_S_DPS_MSG_BODY_EVENT_E2_FILE_OPEN
{
	NPC_CHAR										i_sFileFlag[NPC_D_MON_CSD_MAX_FILE_FLAG_LEN];		//文件标识，唯一标识该文件，打开文件时必须原样传回
	NPC_DWORD										i_dwStartOffsetDataPos;			//开始读取数据位置（单位字节），指原始文件的读取位置（暂停使用）
	NPC_DWORD										i_dwStartOffsetTimePos;			//开始读取时间位置（单位秒），为相对时间，如果值大于文件总时长，则打开失败
																					//图片文件该参数无效

	NPC_DWORD										o_dwFileReadId;					//返回文件读取ID
	NPC_S_MON_MPS_FILE_INFO							o_tFileInfo;					//文件信息

} NPC_S_DPS_MSG_BODY_EVENT_E2_FILE_OPEN, *PNPC_S_DPS_MSG_BODY_EVENT_E2_FILE_OPEN;

//打开文件，可能同时对同一个文件打开多次的可能，即不同的客户端对同一个文件播放
typedef struct _NPC_S_DPS_MSG_BODY_EVENT_E2_FILE_OPEN_BY_TIME
{
	NPC_INT											i_iChNo;						//通道号，如果为-1表示查询所有通道的文件
	NPC_INT											i_iStreamNo;					//码流号，如果为-1表示查询所有码流的文件
	NPC_INT											i_iFileType;					//文件类型，参见NPC_D_MON_CSD_FILE_TYPE_*定义
	NPC_INT											i_iCreateMode;					//创建方式，参见NPC_D_MON_CSD_FILE_CREATE_MODE_*定义
	NPC_BYTE										i_ucAlarmEventTable[16];		//报警事件表，参见NPC_D_MON_CSD_ALARM_EVENT_*定义，只有创建方式为NPC_D_MON_CSD_FILE_CREATE_MODE_ALARM时有效
	NPC_INT											i_iAlarmEventNum;				//报警事件数
	NPC_S_TD_DATE_TIME								i_tStartTime;					//开始时间，以文件创建时间或录像的开始时间为查询时间
	NPC_S_TD_DATE_TIME								i_tEndTime;						//结束时间,图片文件该参数无效
	
	NPC_DWORD										o_dwFileReadId;					//返回文件读取ID
	NPC_S_MON_MPS_FILE_INFO							o_tFileInfo;					//文件信息
	
} NPC_S_DPS_MSG_BODY_EVENT_E2_FILE_OPEN_BY_TIME, *PNPC_S_DPS_MSG_BODY_EVENT_E2_FILE_OPEN_BY_TIME;

//关闭文件
typedef struct _NPC_S_DPS_MSG_BODY_EVENT_E2_FILE_CLOSE
{
	NPC_DWORD										i_dwFileReadId;					//文件读取ID

} NPC_S_DPS_MSG_BODY_EVENT_E2_FILE_CLOSE, *PNPC_S_DPS_MSG_BODY_EVENT_E2_FILE_CLOSE;

//读取文件（读取下一段数据），应答消息中实际数据在该结构体之后，应答消息中，请求部分应原样返回
typedef struct _NPC_S_DPS_MSG_BODY_EVENT_E2_FILE_READ
{
	NPC_DWORD										i_dwFileReadId;					//文件读取ID
	NPC_DWORD										i_dwRequId;						//请求ID
	NPC_BYTE										i_ucReqDataType;				//请求读取的数据类型，0：混合数据，1：视频，2：音频，3：文件数据（如图片文件），值为100表示停止发送命令
	NPC_BYTE										i_ucReqDataSize;				//请求数据大小，混合数据或视频或音频该参数指秒数，图片数据指多少K字节
																					//如果为0，则不停止，一直读取发送，直到读取文件完必
																					//读取混合数据时，根据文件中的数据顺序读取，如果读取为视频帧，则发送视频帧，如果是音频数据，则发送音频，音频数据单个数据包大小最大10K，一次请求的读取数量，由视频帧为准。

	NPC_BYTE										o_ucRetDataType;				//返回数据类型，1：视频，2：音频，3：文件数据（图片数据）
	NPC_BYTE										o_ucFrameType:4;				//帧类型，视频有效，参见NPC_D_MON_CSD_VIDEO_FRAME_TYPE_*定义
	NPC_BYTE										o_ucIfDataEnd:2;				//是否结束，0：未结束，1：本次请求结束，2：文件结束
	NPC_BYTE										o_ucReserve1:2;					//备用1
	NPC_BYTE										o_ucMediaCodecId;				//媒体编码格式，参见NPC_D_MON_CSD_CODEC_ID_*定义
	NPC_BYTE										o_ucVideoFrameRate;				//视频帧率（为视频帧时有效）
	NPC_BYTE										o_ucReserve2[2];				//备用2
	NPC_DWORD										o_dwOffsetDataPos;				//读取数据位置（单位字节），指原始文件的读取位置
	NPC_DWORD										o_dwOffsetTimePos;				//读取时间位置（单位秒），为相对时间，文件数据该参数无效
	NPC_S_TD_DATE_TIME								o_tDataTime;					//数据时间，为绝对时间，视频为帧的时间，音频为数据的开始时间，文件数据该参数无效
	NPC_DWORD										o_dwDataLen;					//数据长度

} NPC_S_DPS_MSG_BODY_EVENT_E2_FILE_READ, *PNPC_S_DPS_MSG_BODY_EVENT_E2_FILE_READ;


//定位文件
typedef struct _NPC_S_DPS_MSG_BODY_EVENT_E2_FILE_SEEK
{
	NPC_DWORD										i_dwFileReadId;					//文件读取ID
	NPC_DWORD										i_dwOffsetTimePos;				//读取时间位置（单位秒），为相对时间，大于等于0，小于文件总时长

} NPC_S_DPS_MSG_BODY_EVENT_E2_FILE_SEEK, *PNPC_S_DPS_MSG_BODY_EVENT_E2_FILE_SEEK;

//定位文件(按时间方式)
typedef struct _NPC_S_DPS_MSG_BODY_EVENT_E2_FILE_SEEK_BY_TIME
{
	NPC_DWORD										i_dwFileReadId;					//文件读取ID
	NPC_S_TD_DATE_TIME								i_tOffsetTimePos;				//读取时间位置（单位秒）

} NPC_S_DPS_MSG_BODY_EVENT_E2_FILE_SEEK_BY_TIME, *PNPC_S_DPS_MSG_BODY_EVENT_E2_FILE_SEEK_BY_TIME;

//查询视频参数
typedef struct _NPC_S_DPS_MSG_BODY_EVENT_E2_FILE_QVIDEO
{
	NPC_DWORD										i_dwFileReadId;					//文件读取ID

	NPC_INT											o_iVideoFrameRate;				//视频帧率
	NPC_INT											o_iVideoGop;					//视频GOP值

} NPC_S_DPS_MSG_BODY_EVENT_E2_FILE_QVIDEO, *PNPC_S_DPS_MSG_BODY_EVENT_E2_FILE_QVIDEO;

//请求I帧数据（丢弃视频P帧和音频，只传视频I帧）
typedef struct _NPC_S_DPS_MSG_BODY_EVENT_E2_FILE_ONLY_IFRAME
{
	NPC_DWORD										i_dwFileReadId;					//文件读取ID
	NPC_INT											i_iDiscardIFrame;				//丢弃I帧数，0：不丢I帧，1：丢弃1个I帧（即间隔1个I帧传输），2：丢弃2个I帧，如此类推

	NPC_INT											o_iVideoFrameRate;				//视频帧率
	NPC_INT											o_iVideoGop;					//视频GOP值

} NPC_S_DPS_MSG_BODY_EVENT_E2_FILE_ONLY_IFRAME, *PNPC_S_DPS_MSG_BODY_EVENT_E2_FILE_ONLY_IFRAME;

//请求恢复全帧数据
typedef struct _NPC_S_DPS_MSG_BODY_EVENT_E2_FILE_RECOVER_ALLFRAME
{
	NPC_DWORD										i_dwFileReadId;					//文件读取ID

} NPC_S_DPS_MSG_BODY_EVENT_E2_FILE_RECOVER_ALLFRAME, *PNPC_S_DPS_MSG_BODY_EVENT_E2_FILE_RECOVER_ALLFRAME;


//-------------------------------------------------------------------------------
//开始语音对讲
typedef struct _NPC_S_DPS_MSG_BODY_EVENT_E2_TALK_START
{
	NPC_INT											i_iTalkChNo;					//对讲通道号

	NPC_INT											o_iDevAudioCodecId;				//设备端音频编码ID，参见NPC_D_MON_CSD_CODEC_ID_AUDIO_*定义
	NPC_INT											o_iDevAudioBitRate;				//设备端音频码率
	NPC_INT											o_iDevAudioSampleRate;			//设备端音频采样率
	NPC_INT											o_iDevAudioChNum;				//设备端音频声道数

} NPC_S_DPS_MSG_BODY_EVENT_E2_TALK_START, *PNPC_S_DPS_MSG_BODY_EVENT_E2_TALK_START;

//停止语音对讲
typedef struct _NPC_S_DPS_MSG_BODY_EVENT_E2_TALK_STOP
{
	NPC_INT											i_iTalkChNo;					//对讲通道号

} NPC_S_DPS_MSG_BODY_EVENT_E2_TALK_STOP, *PNPC_S_DPS_MSG_BODY_EVENT_E2_TALK_STOP;


//-------------------------------------------------------------------------------
//告警消息
typedef struct _NPC_S_DPS_MSG_BODY_EVENT_E2_ALARM_INFO
{
	NPC_INT											i_iChNo;						//通道号
	NPC_INT											i_iAlarmEvent;					//报警事件，参见NPC_D_MON_CSD_ALARM_EVENT_*定义
	NPC_INT											i_iAlarmState;					//报警状态，0：报警结束，1：报警开始
	NPC_CHAR										i_sAlarmInfo[128];				//报警信息

} NPC_S_DPS_MSG_BODY_EVENT_E2_ALARM_INFO, *PNPC_S_DPS_MSG_BODY_EVENT_E2_ALARM_INFO;

//告警消息（扩展）
typedef struct _NPC_S_DPS_MSG_BODY_EVENT_E2_EX_ALARM_INFO
{
	NPC_CHAR										i_sAlarmId[32];					//报警ID，要求设备端唯一，比如采用时间值+序号，如20150301091022001
	NPC_INT											i_iChNo;						//通道号
	NPC_INT											i_iAlarmEvent;					//报警事件，参见NPC_D_MON_CSD_ALARM_EVENT_*定义
	NPC_CHAR										i_sAlarmInfo[128];				//报警信息

} NPC_S_DPS_MSG_BODY_EVENT_E2_EX_ALARM_INFO, *PNPC_S_DPS_MSG_BODY_EVENT_E2_EX_ALARM_INFO;



//-------------------------------------------------------------------------------
//修改用户密码
typedef struct _NPC_S_DPS_MSG_BODY_EVENT_E3_MODIFY_USER_PWD
{
	NPC_CHAR										i_sUserName[32];				//用户名
	NPC_CHAR										i_sOldPwd[32];					//原密码（明文）
	NPC_CHAR										i_sNewPwd[32];					//新密码（明文）

} NPC_S_DPS_MSG_BODY_EVENT_E3_MODIFY_USER_PWD, *PNPC_S_DPS_MSG_BODY_EVENT_E3_MODIFY_USER_PWD;

//获取配置参数，该结构体之后为配置参数数据，配置参数定义参考NPC_PVM_OrderCfgDef.h文件定义
typedef struct _NPC_S_DPS_MSG_BODY_EVENT_E3_GET_CONFIG
{
	NPC_INT											i_iCfgFuncId;					//配置功能ID，参见NPC_D_PVM_CFG_FUNCID_*定义

	NPC_INT											io_iCfgDataLen;					//配置数据长度

} NPC_S_DPS_MSG_BODY_EVENT_E3_GET_CONFIG, *PNPC_S_DPS_MSG_BODY_EVENT_E3_GET_CONFIG;

//设置配置参数，该结构体之后为配置参数数据，配置参数定义参考NPC_PVM_OrderCfgDef.h文件定义
typedef struct _NPC_S_DPS_MSG_BODY_EVENT_E3_SET_CONFIG
{
	NPC_INT											i_iCfgFuncId;					//配置功能ID，参见NPC_D_PVM_CFG_FUNCID_*定义
	NPC_INT											i_iCfgDataLen;					//配置数据长度

} NPC_S_DPS_MSG_BODY_EVENT_E3_SET_CONFIG, *PNPC_S_DPS_MSG_BODY_EVENT_E3_SET_CONFIG;

//获取缺省配置参数，该结构体之后为配置参数数据，配置参数定义参考NPC_PVM_OrderCfgDef.h文件定义
typedef struct _NPC_S_DPS_MSG_BODY_EVENT_E3_GET_DEF_CONFIG
{
	NPC_INT											i_iCfgFuncId;					//配置功能ID，参见NPC_D_PVM_CFG_FUNCID_*定义
	NPC_INT											i_iChNo;						//通道号

	NPC_INT											io_iCfgDataLen;					//配置数据长度

} NPC_S_DPS_MSG_BODY_EVENT_E3_GET_DEF_CONFIG, *PNPC_S_DPS_MSG_BODY_EVENT_E3_GET_DEF_CONFIG;

//导入配置，该结构体之后为XML格式的配置表数据
typedef struct _NPC_S_DPS_MSG_BODY_EVENT_E3_IEC_IMP_CONFIG
{
	NPC_INT											i_iCfgDataLen;					//配置数据长度，即XML数据长度

} NPC_S_DPS_MSG_BODY_EVENT_E3_IEC_IMP_CONFIG, *PNPC_S_DPS_MSG_BODY_EVENT_E3_IEC_IMP_CONFIG;

//导出配置，该结构体之后为XML格式的配置表数据
typedef struct _NPC_S_DPS_MSG_BODY_EVENT_E3_IEC_EXP_CONFIG
{
	NPC_INT											o_iCfgDataLen;					//配置数据长度，即XML数据长度

} NPC_S_DPS_MSG_BODY_EVENT_E3_IEC_EXP_CONFIG, *PNPC_S_DPS_MSG_BODY_EVENT_E3_IEC_EXP_CONFIG;


//-------------------------------------------------------------------------------
//查询网络信息，应答消息中该结构体之后为网卡信息数组
typedef struct _NPC_S_DPS_MSG_BODY_EVENT_E3_QUERY_NET_INFO
{
	NPC_INT											o_iNetCardNum;					//网卡数

} NPC_S_DPS_MSG_BODY_EVENT_E3_QUERY_NET_INFO, *PNPC_S_DPS_MSG_BODY_EVENT_E3_QUERY_NET_INFO;

//网卡信息
typedef struct _NPC_S_DPS_MSG_BODY_EVENT_E3_NET_CARD_INFO
{
	NPC_CHAR										sNetCardName[32];				//网卡名称
	NPC_BYTE										sMac[6];						//MAC地址
	NPC_BOOL										bIfOpenDhcp;					//是否打开DHCP，如果为TRUE，则后面三个参数无效
	NPC_CHAR										sIpaddr[16];					//IP地址
	NPC_CHAR										sSubmask[16];					//子网掩码
	NPC_CHAR										sGateway[16];					//网关地址

} NPC_S_DPS_MSG_BODY_EVENT_E3_NET_CARD_INFO, *PNPC_S_DPS_MSG_BODY_EVENT_E3_NET_CARD_INFO;

//修改网络地址
typedef struct _NPC_S_DPS_MSG_BODY_EVENT_E3_MODIFY_NET_ADDR
{
	NPC_CHAR										i_sNetCardName[32];				//网卡名称
	NPC_BYTE										i_sMac[6];						//MAC地址
	NPC_BOOL										i_bIfOpenDhcp;					//是否打开DHCP，如果为TRUE，则后面三个参数无效
	NPC_CHAR										i_sIpaddr[16];					//IP地址
	NPC_CHAR										i_sSubmask[16];					//子网掩码
	NPC_CHAR										i_sGateway[16];					//网关地址

} NPC_S_DPS_MSG_BODY_EVENT_E3_MODIFY_NET_ADDR, *PNPC_S_DPS_MSG_BODY_EVENT_E3_MODIFY_NET_ADDR;


//------------------------------------------------------------------------------
//打开串口
typedef struct _NPC_S_DPS_MSG_BODY_EVENT_E3_COM_OPEN
{
	NPC_DWORD										i_dwClientConnId;				//客户端连接ID
	NPC_INT											i_iComType;						//串口类型，0：RS232串口，1：RS485串口
	NPC_INT											i_iPortNo;						//端口号
	NPC_INT											i_iBaudRate;					//波特率，如4800，14400，28800，36600
	NPC_INT											i_iDataBit;						//数据位，如8位
	NPC_INT											i_iStopBit;						//停止位，如1
	NPC_INT											i_iCheckBit;					//奇偶校验位

} NPC_S_DPS_MSG_BODY_EVENT_E3_COM_OPEN, *PNPC_S_DPS_MSG_BODY_EVENT_E3_COM_OPEN;

//关闭串口
typedef struct _NPC_S_DPS_MSG_BODY_EVENT_E3_COM_CLOSE
{
	NPC_DWORD										i_dwClientConnId;				//客户端连接ID
	NPC_BOOL										i_bIfCloseAllCom;				//是否关闭与客户端连接ID相关的所有串口
	NPC_INT											i_iComType;						//串口类型，0：RS232串口，1：RS485串口
	NPC_INT											i_iPortNo;						//端口号

} NPC_S_DPS_MSG_BODY_EVENT_E3_COM_CLOSE, *PNPC_S_DPS_MSG_BODY_EVENT_E3_COM_CLOSE;

//发送串口数据(客户端发送给设备端)，该结构体之后为串口数据
typedef struct _NPC_S_DPS_MSG_BODY_EVENT_E3_COM_SEND_DATA
{
	NPC_DWORD										i_dwClientConnId;				//客户端连接ID
	NPC_INT											i_iComType;						//串口类型，0：RS232串口，1：RS485串口
	NPC_INT											i_iPortNo;						//端口号
	NPC_INT											i_iDataLen;						//数据长度

} NPC_S_DPS_MSG_BODY_EVENT_E3_COM_SEND_DATA, *PNPC_S_DPS_MSG_BODY_EVENT_E3_COM_SEND_DATA;

//设备串口数据(设备端发送给客户端)，该结构体之后为串口数据
typedef struct _NPC_S_DPS_MSG_BODY_EVENT_E3_COM_SUBS_DATA
{
	NPC_DWORD										i_dwClientConnId;				//客户端连接ID
	NPC_INT											i_iComType;						//串口类型，0：RS232串口，1：RS485串口
	NPC_INT											i_iPortNo;						//端口号
	NPC_INT											i_iDataLen;						//数据长度

} NPC_S_DPS_MSG_BODY_EVENT_E3_COM_SUBS_DATA, *PNPC_S_DPS_MSG_BODY_EVENT_E3_COM_SUBS_DATA;


//------------------------------------------------------------------------------
//增加用户
typedef struct _NPC_S_DPS_MSG_BODY_EVENT_E3_UNS_ADD_USER
{
	NPC_CHAR										i_sUserName[32];				//用户名
	NPC_CHAR										i_sUserPwd[32];					//用户密码
	NPC_DWORD										i_dwGlobalPop;					//全局权限，位值参考NPC_D_DPS_POP_*定义
	NPC_INT											i_iChNum;						//通道数，该结构体之后为通道权限表，每4字节为一个通道的权限，4字节的顺序号为通道号，位值参考NPC_D_DPS_POP_*定义
	NPC_DWORD										i_dwChPopTable[64];				//通道权限表，数组索引号为通道号，位值参考NPC_D_DPS_POP_*定义

} NPC_S_DPS_MSG_BODY_EVENT_E3_UNS_ADD_USER, *PNPC_S_DPS_MSG_BODY_EVENT_E3_UNS_ADD_USER;

//删除用户
typedef struct _NPC_S_DPS_MSG_BODY_EVENT_E3_UNS_DEL_USER
{
	NPC_CHAR										i_sUserName[32];				//用户名

} NPC_S_DPS_MSG_BODY_EVENT_E3_UNS_DEL_USER, *PNPC_S_DPS_MSG_BODY_EVENT_E3_UNS_DEL_USER;

//获取用户权限列表
typedef struct _NPC_S_DPS_MSG_BODY_EVENT_E3_UNS_GET_USER_POP
{
	NPC_CHAR										i_sUserName[32];				//用户名

	NPC_DWORD										o_dwGlobalPop;					//全局权限，位值参考NPC_D_DPS_POP_*定义
	NPC_INT											o_iChNum;						//通道数，该结构体之后为通道权限表，每4字节为一个通道的权限，4字节的顺序号为通道号，位值参考NPC_D_DPS_POP_*定义
	NPC_DWORD										o_dwChPopTable[64];				//通道权限表，数组索引号为通道号，位值参考NPC_D_DPS_POP_*定义

} NPC_S_DPS_MSG_BODY_EVENT_E3_UNS_GET_USER_POP, *PNPC_S_DPS_MSG_BODY_EVENT_E3_UNS_GET_USER_POP;

//设置用户权限列表
typedef struct _NPC_S_DPS_MSG_BODY_EVENT_E3_UNS_SET_USER_POP
{
	NPC_CHAR										i_sUserName[32];				//用户名
	NPC_DWORD										i_dwGlobalPop;					//全局权限，位值参考NPC_D_DPS_POP_*定义
	NPC_INT											i_iChNum;						//通道数，该结构体之后为通道权限表，每4字节为一个通道的权限，4字节的顺序号为通道号，位值参考NPC_D_DPS_POP_*定义
	NPC_DWORD										i_dwChPopTable[64];				//通道权限表，数组索引号为通道号，位值参考NPC_D_DPS_POP_*定义

} NPC_S_DPS_MSG_BODY_EVENT_E3_UNS_SET_USER_POP, *PNPC_S_DPS_MSG_BODY_EVENT_E3_UNS_SET_USER_POP;


//-------------------------------------------------------------------------------
//定制功能
typedef struct _NPC_S_DPS_MSG_BODY_EVENT_E7_CUSTOM
{
	NPC_INT											i_iCusFuncId;					//定制功能ID，参见相应文件定义
	NPC_INT											i_iBodyLen;						//包体长度

} NPC_S_DPS_MSG_BODY_EVENT_E7_CUSTOM, *PNPC_S_DPS_MSG_BODY_EVENT_E7_CUSTOM;


//-------------------------------------------------------------------------------
//查询能力集
typedef struct _NPC_S_DPS_MSG_BODY_EVENT_E7_RMY_QUERY_ABILITY
{
	NPC_INT											i_iRequVerNo;					//请求版本号

	NPC_INT											o_iAbilityDataLen;				//能力数据长度,该结构体之后为能力数据,能力数据参见NPC_PVM_OrderCfgDef.h文件的定义

} NPC_S_DPS_MSG_BODY_EVENT_E7_RMY_QUERY_ABILITY, *PNPC_S_DPS_MSG_BODY_EVENT_E7_RMY_QUERY_ABILITY;


//------------------------------------------------------------------------------
//查询设备版本信息
typedef struct _NPC_S_DPS_MSG_BODY_EVENT_E7_DUP_QUERY_DEV_VER_INFO
{
	NPC_USHORT										o_usModuleType[4];				//模块类型,0:未知类型,1:UI,2:SYS,3:UI&SYS
	NPC_UINT										o_uiModuleVerNo[4];				//模块版本号,按顺序对应模块类型,格式为:x.x.x.x,每字节为一个号

} NPC_S_DPS_MSG_BODY_EVENT_E7_DUP_QUERY_DEV_VER_INFO, *PNPC_S_DPS_MSG_BODY_EVENT_E7_DUP_QUERY_DEV_VER_INFO;

//请求升级
typedef struct _NPC_S_DPS_MSG_BODY_EVENT_E7_DUP_REQU_UPGRADE
{
	NPC_USHORT										i_usModuleType;					//模块类型,0:未知类型,1:UI,2:SYS,3:UI&SYS
	NPC_USHORT										i_usCheckSum;					//文件检验和
	NPC_INT											i_iFileSize;					//升级文件大小(字节)

	NPC_UINT										o_uiUpgradeId;					//升级ID

} NPC_S_DPS_MSG_BODY_EVENT_E7_DUP_REQU_UPGRADE, *PNPC_S_DPS_MSG_BODY_EVENT_E7_DUP_REQU_UPGRADE;

//查询升级进度
typedef struct _NPC_S_DPS_MSG_BODY_EVENT_E7_DUP_QUERY_PROGRESS
{
	NPC_UINT										i_uiUpgradeId;					//升级ID

	NPC_INT											o_iUpgProgress;					//升级进度,值为0~100,指写FLASH的进度

} NPC_S_DPS_MSG_BODY_EVENT_E7_DUP_QUERY_PROGRESS, *PNPC_S_DPS_MSG_BODY_EVENT_E7_DUP_QUERY_PROGRESS;

//查询传输位置(命令通道)
typedef struct _NPC_S_DPS_MSG_BODY_EVENT_E7_DUP_QUERY_TRANS_POS
{
	NPC_UINT										i_uiUpgradeId;					//升级ID

	NPC_INT											o_iTransPos;					//传输位置，即相对文件头的偏移位置，从0开始，该值相当于接收到的文件数据长度

} NPC_S_DPS_MSG_BODY_EVENT_E7_DUP_QUERY_TRANS_POS, *PNPC_S_DPS_MSG_BODY_EVENT_E7_DUP_QUERY_TRANS_POS;

//升级数据包
typedef struct _NPC_S_DPS_MSG_BODY_EVENT_E7_DUP_UPGRADE_DATA
{
	NPC_UINT										i_uiUpgradeId;					//升级ID
	NPC_INT											i_iUpgDataLen;					//升级数据长度,该结构体之后为升级数据

} NPC_S_DPS_MSG_BODY_EVENT_E7_DUP_UPGRADE_DATA, *PNPC_S_DPS_MSG_BODY_EVENT_E7_DUP_UPGRADE_DATA;


//-------------------------------------------------------------------------------
//视频帧数据
typedef struct _NPC_S_DPS_MSG_BODY_MEDIA_E8_VIDEO_FRAME
{
	NPC_USHORT										i_usChNo;						//通道号
	NPC_USHORT										i_usStreamNo;					//码流号，0：主码流，1：子码流
	NPC_USHORT										i_usFrameType;					//帧类型，参见NPC_D_MON_CSD_VIDEO_FRAME_TYPE_*定义
	NPC_BYTE										i_ucReserve[2];					//备用

} NPC_S_DPS_MSG_BODY_MEDIA_E8_VIDEO_FRAME, *PNPC_S_DPS_MSG_BODY_MEDIA_E8_VIDEO_FRAME;

//音频帧数据
typedef struct _NPC_S_DPS_MSG_BODY_MEDIA_E8_AUDIO_FRAME
{
	NPC_USHORT										i_usChNo;						//通道号
	NPC_USHORT										i_usStreamNo;					//码流号，0：主码流，1：子码流

} NPC_S_DPS_MSG_BODY_MEDIA_E8_AUDIO_FRAME, *PNPC_S_DPS_MSG_BODY_MEDIA_E8_AUDIO_FRAME;


//视频帧数据
typedef struct _NPC_S_DPS_MSG_BODY_MEDIA_E8_VIDEO_FRAME_EX
{
	NPC_USHORT										i_usChNo;						//通道号
	NPC_USHORT										i_usStreamNo;					//码流号，0：主码流，1：子码流
	NPC_USHORT										i_usFrameType;					//帧类型，参见NPC_D_MON_CSD_VIDEO_FRAME_TYPE_*定义
	NPC_BYTE										i_ucReserve[2];					//备用
	NPC_DWORD										i_dwTimestamp;					//时间戳
	NPC_BYTE										i_ucReserve2[16];				//备用2
	
} NPC_S_DPS_MSG_BODY_MEDIA_E8_VIDEO_FRAME_EX, *PNPC_S_DPS_MSG_BODY_MEDIA_E8_VIDEO_FRAME_EX;

//音频帧数据
typedef struct _NPC_S_DPS_MSG_BODY_MEDIA_E8_AUDIO_FRAME_EX
{
	NPC_USHORT										i_usChNo;						//通道号
	NPC_USHORT										i_usStreamNo;					//码流号，0：主码流，1：子码流
	NPC_DWORD										i_dwTimestamp;					//时间戳
	NPC_BYTE										i_ucReserve[16];				//备用	
} NPC_S_DPS_MSG_BODY_MEDIA_E8_AUDIO_FRAME_EX, *PNPC_S_DPS_MSG_BODY_MEDIA_E8_AUDIO_FRAME_EX;

#endif

