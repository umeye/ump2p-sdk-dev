/*----------------------------------------------------------------------------
 * Copyright(c)  :  NPC CORPORTAION All Rights Reserved                       
 * FileName      :  NPC_MON_ConsDef.h
 * Version       :  1.0
 * Author        :  CCH
 * DateTime      :  2013-08-12
 * Description   :  常量定义
 *----------------------------------------------------------------------------*/

#ifndef __NPC_MON_CONSDEF_H
#define __NPC_MON_CONSDEF_H

#include "NPC_TypeDef.h"


//-------------------------------------------------------------------------------
//常量定义
#define NPC_D_MON_CSD_MAX_FILE_FLAG_LEN								128				//最大文件标识长

//-------------------------------------------------------------------------------
//编码ID定义
#define NPC_D_MON_CSD_CODEC_ID_UNKNOWN								0				//未知类型

#define NPC_D_MON_CSD_CODEC_ID_VEDIO_H264							1				//H264
#define NPC_D_MON_CSD_CODEC_ID_VEDIO_MPEG4							2				//MPEG4
#define NPC_D_MON_CSD_CODEC_ID_VEDIO_MJPG							3				//MJPG
#define NPC_D_MON_CSD_CODEC_ID_VEDIO_H265							4				//H265

#define NPC_D_MON_CSD_CODEC_ID_AUDIO_AAC							1				//AAC
#define NPC_D_MON_CSD_CODEC_ID_AUDIO_PCMA							2				//PCMA（G711A）
#define NPC_D_MON_CSD_CODEC_ID_AUDIO_PCMU							3				//PCMU（G711U）
#define NPC_D_MON_CSD_CODEC_ID_AUDIO_ADPCM							4				//ADPCM
#define NPC_D_MON_CSD_CODEC_ID_AUDIO_G726							5				//G726
#define NPC_D_MON_CSD_CODEC_ID_AUDIO_AMRNB							6				//AMRNB

//视频帧类型
#define NPC_D_MON_CSD_VIDEO_FRAME_TYPE_UNKNOWN						0				//未知类型
#define NPC_D_MON_CSD_VIDEO_FRAME_TYPE_I							1				//I帧
#define NPC_D_MON_CSD_VIDEO_FRAME_TYPE_P							2				//P帧
#define NPC_D_MON_CSD_VIDEO_FRAME_TYPE_B							3				//B帧

//视频分辨率
#define NPC_D_MON_CSD_VEDIO_RESOLUTION_1080P						1				//1080P
#define NPC_D_MON_CSD_VEDIO_RESOLUTION_720P							2				//720P
#define NPC_D_MON_CSD_VEDIO_RESOLUTION_D1							3				//D1
#define NPC_D_MON_CSD_VEDIO_RESOLUTION_HD1							4				//HD1
#define NPC_D_MON_CSD_VEDIO_RESOLUTION_BCIF							5				//BCIF
#define NPC_D_MON_CSD_VEDIO_RESOLUTION_CIF							6				//CIF
#define NPC_D_MON_CSD_VEDIO_RESOLUTION_QCIF							7				//QCIF
#define NPC_D_MON_CSD_VEDIO_RESOLUTION_VGA							8				//VGA
#define NPC_D_MON_CSD_VEDIO_RESOLUTION_QVGA							9				//QVGA
#define NPC_D_MON_CSD_VEDIO_RESOLUTION_SVCD							10				//SVCD
#define NPC_D_MON_CSD_VEDIO_RESOLUTION_QQVGA						11				//QQVGA
#define NPC_D_MON_CSD_VEDIO_RESOLUTION_SXVGA						12				//SXVGA
#define NPC_D_MON_CSD_VEDIO_RESOLUTION_UXGA							13				//UXGA
#define NPC_D_MON_CSD_VEDIO_RESOLUTION_960H							14				//960H
#define NPC_D_MON_CSD_VEDIO_RESOLUTION_960P							15				//960P
#define NPC_D_MON_CSD_VEDIO_RESOLUTION_3M							16				//3M:2304 * 1296
#define NPC_D_MON_CSD_VEDIO_RESOLUTION_4M							17				//4M:2592 * 1520
#define NPC_D_MON_CSD_VEDIO_RESOLUTION_5M							18				//5M:2592 * 1944
#define NPC_D_MON_CSD_VEDIO_RESOLUTION_UHD4K						19				//UHD4K:3840 * 2160
#define NPC_D_MON_CSD_VEDIO_RESOLUTION_12M							20				//12M:4000 * 3000





//图片格式定义
#define NPC_D_MON_CSD_PIC_FORMAT_BMP								1				//BMP文件
#define NPC_D_MON_CSD_PIC_FORMAT_JPEG								2				//JPEG文件

//码流控制模式
#define NPC_D_MON_CSD_BITRATECTRL_CBR								1				//固定码流
#define NPC_D_MON_CSD_BITRATECTRL_VBR								2				//可变码流
#define NPC_D_MON_CSD_BITRATECTRL_MBR								3				//混和码流

//PTZ命令定义
#define NPC_D_MON_CSD_PTZ_CMD_STOP									0				//停止运动，停止前一个启动命令

#define NPC_D_MON_CSD_PTZ_CMD_SET_PRESET							8				//设置预置点，in_dwPtzData为预置点的序号（1至255）
#define NPC_D_MON_CSD_PTZ_CMD_CLE_PRESET							9				//清除预置点，in_dwPtzData为预置点的序号（1至255）

#define NPC_D_MON_CSD_PTZ_CMD_ZOOM_IN								11				//焦距以速度SS变大（倍率变大），in_dwPtzData为速度（1至10），云镜速度初始值为4，in_dwPtzExtParam1为是否自动停止（1或0），in_dwPtzExtParam2为自动停止的间隔时间（1至255秒）
#define NPC_D_MON_CSD_PTZ_CMD_ZOOM_OUT								12				//焦距以速度SS变小（倍率变小），同上
#define NPC_D_MON_CSD_PTZ_CMD_FOCUS_NEAR							13  			//焦点以速度SS前调，同上
#define NPC_D_MON_CSD_PTZ_CMD_FOCUS_FAR								14  			//焦点以速度SS后调，同上
#define NPC_D_MON_CSD_PTZ_CMD_IRIS_OPEN								15  			//光圈以速度SS扩大，同上
#define NPC_D_MON_CSD_PTZ_CMD_IRIS_CLOSE							16  			//光圈以速度SS缩小，同上

#define NPC_D_MON_CSD_PTZ_CMD_TILT_UP								21				//云台以SS的速度上仰，in_dwPtzData为速度（1至10），云台速度初始值为4，in_dwPtzExtParam1为是否自动停止（1或0），in_dwPtzExtParam2为自动停止的间隔时间（1至255秒）
#define NPC_D_MON_CSD_PTZ_CMD_TILT_DOWN								22				//云台以SS的速度下俯，同上
#define NPC_D_MON_CSD_PTZ_CMD_PAN_LEFT								23				//云台以SS的速度左转，同上
#define NPC_D_MON_CSD_PTZ_CMD_PAN_RIGHT								24				//云台以SS的速度右转，同上
#define NPC_D_MON_CSD_PTZ_CMD_UP_LEFT								25				//云台以SS的速度上仰和左转，同上
#define NPC_D_MON_CSD_PTZ_CMD_UP_RIGHT								26				//云台以SS的速度上仰和右转，同上
#define NPC_D_MON_CSD_PTZ_CMD_DOWN_LEFT								27				//云台以SS的速度下俯和左转，同上
#define NPC_D_MON_CSD_PTZ_CMD_DOWN_RIGHT							28				//云台以SS的速度下俯和右转，同上
#define NPC_D_MON_CSD_PTZ_CMD_PAN_AUTO								29				//云台以SS的速度左右自动扫描，同上

#define NPC_D_MON_CSD_PTZ_CMD_FILL_PRE_SEQ							30				//将预置点加入巡航序列，in_dwPtzData为巡航路径号（1至32），in_dwPtzExtParam1为巡航点号（1至32），in_dwPtzExtParam2为预置点号（1至255）
#define NPC_D_MON_CSD_PTZ_CMD_SET_SEQ_DWELL							31				//设置巡航点停顿时间，in_dwPtzData为巡航路径号（1至32），in_dwPtzExtParam1为巡航点号（1至32），in_dwPtzExtParam2为停顿时间（1至255秒）
#define NPC_D_MON_CSD_PTZ_CMD_SET_SEQ_SPEED							32				//设置巡航速度，in_dwPtzData为巡航路径号（1至32），in_dwPtzExtParam1为巡航点号（1至32），in_dwPtzExtParam2为速度（1至10）
#define NPC_D_MON_CSD_PTZ_CMD_CLE_PRE_SEQ							33				//将预置点从巡航序列中删除，in_dwPtzData为巡航路径号（1至32），in_dwPtzExtParam1为巡航点号（1至32），in_dwPtzExtParam2为预置点号（1至255）
#define NPC_D_MON_CSD_PTZ_CMD_STA_MEM_CRUISE						34				//开始记录轨迹，，无参数
#define NPC_D_MON_CSD_PTZ_CMD_STO_MEM_CRUISE						35				//停止记录轨迹，无参数
#define NPC_D_MON_CSD_PTZ_CMD_RUN_CRUISE							36				//开始轨迹，无参数
#define NPC_D_MON_CSD_PTZ_CMD_RUN_SEQ								37				//开始巡航，in_dwPtzData为巡航路径号
#define NPC_D_MON_CSD_PTZ_CMD_STOP_SEQ								38				//停止巡航，in_dwPtzData为巡航路径号
#define NPC_D_MON_CSD_PTZ_CMD_GOTO_PRESET							39				//快球转到预置点，in_dwPtzData为预置点号
#define NPC_D_MON_PTZ_CMD_CLEAR_TOUR                                40				//清除巡航，in_dwPtzData为巡航路径号

#define NPC_D_MON_CSD_PTZ_CMD_UTC_DETECT							101				//UTC 探测是否支持
#define NPC_D_MON_CSD_PTZ_CMD_UTC_SET								102				//UTC 设置
#define NPC_D_MON_CSD_PTZ_CMD_UTC_UP								103				//UTC 上
#define NPC_D_MON_CSD_PTZ_CMD_UTC_DOWN								104				//UTC 下 
#define NPC_D_MON_CSD_PTZ_CMD_UTC_LEFT								105				//UTC 左
#define NPC_D_MON_CSD_PTZ_CMD_UTC_RIGHT								106				//UTC 右
#define NPC_D_MON_CSD_PTZ_CMD_UTC_FOCUS_NEER						107				//UTC 聚焦
#define NPC_D_MON_CSD_PTZ_CMD_UTC_FOCUS_FAR							108				//UTC 聚焦
#define NPC_D_MON_CSD_PTZ_CMD_UTC_ZOOM_WIDE							109				//UTC ZOOM
#define NPC_D_MON_CSD_PTZ_CMD_UTC_ZOOM_TELE							110				//UTC ZOOM
#define NPC_D_MON_CSD_PTZ_CMD_UTC_STOP								111				//UTC 停止




//文件类型定义
#define NPC_D_MON_CSD_FILE_TYPE_ALL									0				//所有文件类型
#define NPC_D_MON_CSD_FILE_TYPE_RECORD								1				//录像文件
#define NPC_D_MON_CSD_FILE_TYPE_PICTRUE								2				//拍照文件

//文件创建方式定义
#define NPC_D_MON_CSD_FILE_CREATE_MODE_ALL							0				//所有方式
#define NPC_D_MON_CSD_FILE_CREATE_MODE_ALARM						1				//报警方式
#define NPC_D_MON_CSD_FILE_CREATE_MODE_MANUAL						2				//手动方式
#define NPC_D_MON_CSD_FILE_CREATE_MODE_TIMER						3				//定时方式

//报警事件定义
#define NPC_D_MON_CSD_ALARM_EVENT_ALL								0				//所有报警事件
#define NPC_D_MON_CSD_ALARM_EVENT_FAULT								1				//设备故障报警
#define NPC_D_MON_CSD_ALARM_EVENT_MOTION							2				//移动侦测报警
#define NPC_D_MON_CSD_ALARM_EVENT_VIDEO_BLIND						3				//视频遮挡报警
#define NPC_D_MON_CSD_ALARM_EVENT_VIDEO_LOSS						4				//视频丢失报警
#define NPC_D_MON_CSD_ALARM_EVENT_PROBE								5				//探头报警（外部输入报警）
#define NPC_D_MON_CSD_ALARM_EVENT_PIR								6				//人体感应报警
#define NPC_D_MON_CSD_ALARM_EVENT_DOORBELL							7				//门铃报警
#define NPC_D_MON_CSD_ALARM_EVENT_HDD_ERROR							8				//硬盘错误
#define NPC_D_MON_CSD_ALARM_EVENT_HDD_FULL							9				//硬盘满
#define NPC_D_MON_CSD_ALARM_EVENT_SMART_IPC_CROSS_LINE				10				//越线侦测报警
#define NPC_D_MON_CSD_ALARM_EVENT_SMART_IPC_AREA_INTRUSION			11				//区域入侵报警
#define NPC_D_MON_CSD_ALARM_EVENT_SMART_IPC_AREA_IN					12				//区域进入报警
#define NPC_D_MON_CSD_ALARM_EVENT_SMART_IPC_AREA_OUT				13				//区域离开报警
#define NPC_D_MON_CSD_ALARM_EVENT_SMART_IPC_OBJECT_FORGET			14				//物品遗留报警
#define NPC_D_MON_CSD_ALARM_EVENT_SMART_IPC_OBJECT_PICKUP			15				//物品拾取报警
#define NPC_D_MON_CSD_ALARM_EVENT_SMART_IPC_FAST					16				//快速移动
#define NPC_D_MON_CSD_ALARM_EVENT_SMART_IPC_FACE_RECOGNITION		17				//人脸识别
#define NPC_D_MON_CSD_ALARM_EVENT_KNOCK								18				//敲门报警
#define NPC_D_MON_CSD_ALARM_EVENT_SMART_IPC_WANDER_DETECT           19              //徘徊侦测报警
#define NPC_D_MON_CSD_ALARM_EVENT_SMART_IPC_AREA_PARK               20              //停车侦测报警
#define NPC_D_MON_CSD_ALARM_EVENT_SMART_IPC_AREA_CROWD              21              //人员聚集报警


//DPS协议错误码定义
#define NPC_D_MON_CSD_DPS_ERROR_CODE_SUCCESS						0				//成功

#define NPC_D_MON_CSD_DPS_ERROR_CODE_NOT_SUPPORT_OP					1				//不支持该操作
#define NPC_D_MON_CSD_DPS_ERROR_CODE_BAN_MODIFY						2				//禁止修改
#define NPC_D_MON_CSD_DPS_ERROR_CODE_MSGFUNC_ERROR					3				//消息功能错误
#define NPC_D_MON_CSD_DPS_ERROR_CODE_PARAM_ERROR					4				//参数错误
#define NPC_D_MON_CSD_DPS_ERROR_CODE_ID_ERROR						5				//ID错误

#define NPC_D_MON_CSD_DPS_ERROR_CODE_OTHER							9				//其它原因

#define NPC_D_MON_CSD_DPS_ERROR_CODE_OPEN_STREAM_FAIL				20				//打开媒体流失败（打开实时预览失败）
#define NPC_D_MON_CSD_DPS_ERROR_CODE_NOT_SUPPORT_SUBSTREAM			21				//不支持该子码流
#define NPC_D_MON_CSD_DPS_ERROR_CODE_NOT_SUPPORT_PTZ				22				//不支持云台控制
#define NPC_D_MON_CSD_DPS_ERROR_CODE_NOT_SUPPORT_FORCE_I_FRAME		23				//不支持强制I帧
#define NPC_D_MON_CSD_DPS_ERROR_CODE_NOT_SUPPORT_TALK				24				//不支持语音对讲
#define NPC_D_MON_CSD_DPS_ERROR_CODE_CHANNEL_NO_ERROR				25				//通道号错误
#define NPC_D_MON_CSD_DPS_ERROR_CODE_SUB_STREAM_NO_ERROR			26				//子码流号错误
#define NPC_D_MON_CSD_DPS_ERROR_CODE_START_TALK_FAIL				27				//启动对讲失败
#define NPC_D_MON_CSD_DPS_ERROR_CODE_USERNAME_ERROR					28				//用户名错误
#define NPC_D_MON_CSD_DPS_ERROR_CODE_PASSWORD_ERROR					29				//密码错误

#define NPC_D_MON_CSD_DPS_ERROR_CODE_WITHOUT_PERMISSION				101				//无此权限
#define NPC_D_MON_CSD_DPS_ERROR_CODE_NO_SUBSCRIBE					102				//没有订阅
#define NPC_D_MON_CSD_DPS_ERROR_CODE_QUERY_FILE_FAIL				103				//查询文件失败
#define NPC_D_MON_CSD_DPS_ERROR_CODE_RESOURCE_USED					104				//资源已被占用
#define NPC_D_MON_CSD_DPS_ERROR_CODE_RESOURCE_NOT_OPEN				105				//资源已未被打开
#define NPC_D_MON_CSD_DPS_ERROR_CODE_RESOURCE_NO_IDLE				106				//无空闲资源可使用
#define NPC_D_MON_CSD_DPS_ERROR_CODE_RESOURCE_NOT_EXIST				107				//资源不存在
#define NPC_D_MON_CSD_DPS_ERROR_CODE_RESOURCE_OPEN_FAIL				108				//打开资源失败
#define NPC_D_MON_CSD_DPS_ERROR_CODE_FILE_OP_FAIL					109				//文件操作失败

#define NPC_D_MON_CSD_DPS_ERROR_CODE_SUCCESS_AND_RESTART			200				//操作成功，但需要重启设备才生效


//自定义错误码范围：150~179


#endif

