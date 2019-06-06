/*----------------------------------------------------------------------------
 * Copyright(c)  :  NPC CORPORTAION All Rights Reserved                       
 * FileName      :  NPC_DEF_CM_DataDef.h
 * Version       :  1.1
 * Author        :  CCH
 * DateTime      :  2011-08-21
 * Description   :  通用常量定义
 *----------------------------------------------------------------------------*/

#ifndef __NPC_DEF_CM_DATADEF_H
#define __NPC_DEF_CM_DATADEF_H


#define NPC_D_DEF_CM_IPADDR_BUFSIZE						40			//IP地址缓冲区大小
#define NPC_D_DEF_CM_DEVID_BUFSIZE						16			//设备ID缓冲区大小
#define NPC_D_DEF_CM_DEVNAME_BUFSIZE					64			//设备名称缓冲区大小
#define NPC_D_DEF_CM_USERNAME_BUFSIZE					64			//用户名缓冲区大小
#define NPC_D_DEF_CM_USERPWD_BUFSIZE					64			//密码缓冲区大小
#define NPC_D_DEF_CM_EXTEND_PARAM_BUFSIZE				512			//扩展参数缓冲区大小
#define NPC_D_DEF_CM_BILLID_BUFSIZE						32			//话单日志ID缓冲区大小
#define NPC_D_DEF_CM_VEDIOFORMAT_BUFSIZE				32			//视频格式缓冲区大小
#define NPC_D_DEF_CM_FILENAME_BUFSIZE					64			//文件名缓冲区大小

#define NPC_D_DEF_CM_H264_SPS_BUFSIZE					32			//H264序例参数集缓冲区大小
#define NPC_D_DEF_CM_H264_PPS_BUFSIZE					16			//H264图像参数集缓冲区大小

#define NPC_D_DEF_CM_VEDIO_PARAMS_BUFSIZE				64			//视频参数缓冲区大小
#define NPC_D_DEF_CM_AUDIO_PARAMS_BUFSIZE				32			//音频参数缓冲区大小


//--------------------------------------------------------------------------------
#define NPC_D_CMGW_MAX_DEV_VENDOR_NUM					2048		//最大设备厂商类型数
#define NPC_D_CMGW_MAX_DEV_NUM							1024		//最大设备数，如DVR、IPC数
#define NPC_D_CMGW_MAX_CH_NUM							64			//设备中最大通道数

#define NPC_D_CMGW_MAX_FILEVOD_NUM						1024		//最大文件点播数


//--------------------------------------------------------------------------------
//设备厂商定义
#define NPC_D_DISP_DEV_VENDOR_HIK						0			//海康
#define NPC_D_DISP_DEV_VENDOR_DH						1			//大华
#define NPC_D_DISP_DEV_VENDOR_FILECAM					2			//文件摄像机
#define NPC_D_DISP_DEV_VENDOR_HKS						3			//华科流媒体服务器

//设备类型定义
#define NPC_D_DISP_DEV_TYPE_DVR							0			//DVR
#define NPC_D_DISP_DEV_TYPE_IPC							1			//IPC
#define NPC_D_DISP_DEV_TYPE_PLATFORM					2			//平台

#define NPC_D_DISP_DEV_TYPE_CAMERA						10			//摄像机

//--------------------------------------------------------------------------------
//日志类型定义
#define NPC_D_LOGTYPE_SEVERITY							0			//严重
#define NPC_D_LOGTYPE_IMPORTANCE						1			//重要
#define NPC_D_LOGTYPE_GENERAL							2			//一般
#define NPC_D_LOGTYPE_PROMPT							3			//提示信息



#endif
