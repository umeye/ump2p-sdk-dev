/*----------------------------------------------------------------------------
 * Copyright(c)  :  NPC CORPORTAION All Rights Reserved                       
 * FileName      :  NPC_UMSP_McServer.cpp
 * Version       :  1.0
 * Author        :  CCH
 * DateTime      :  2013-08-10
 * Description   :  UMSP服务器主控模块
 *----------------------------------------------------------------------------*/

#include "NPC_UMSP_McServer.h"

#ifdef _WIN32
#pragma comment(lib, "ws2_32.lib")
#endif

#ifndef NPC_D_DPS_ZERO_CHANNEL_VALUE
#define NPC_D_DPS_ZERO_CHANNEL_VALUE	10000
#endif

//回调函数定义
NPC_VOID	CALLBACK  NPC_F_UMSP_FILECAM_SDK_RealDataCallBack(
	IN		NPC_INT									in_iMediaType,				//媒体类型
	IN		PNPC_BYTE								in_pMediaDataBuf,			//媒体数据
	IN		NPC_INT									in_iMediaDataLen,			//媒体数据长度
	IN		NPC_DWORD								in_dwUserData				//用户数据
	)
{
	PNPC_S_UMSP_CAMERA_DATA							pCameraData;
	PNPC_S_UMSP_MCSRV_DATA							pMcSrvData;
	NPC_INT											iFrameType;

	//INIT AREA
	{
		pCameraData = (PNPC_S_UMSP_CAMERA_DATA)in_dwUserData;
		if ( pCameraData == NULL )
			return;

		if ( !pCameraData->bIfUsed )
			return;

		pMcSrvData = (PNPC_S_UMSP_MCSRV_DATA)(pCameraData->pMcSrvData);
		if ( pMcSrvData == NULL )
			return;

		//加锁
		NPC_F_THREAD_Mutex_Lock(
			pCameraData->hWorkLock			//互斥锁句柄
			);
	}

	//CODE AREA
	{
		if ( in_iMediaType == 0 )
		{
			//检测帧类型，返回帧类型，0：未检测到，1：I帧，2：P帧
			switch ( NPC_F_UMSP_MC_CheckFrameType(
				in_pMediaDataBuf,			//帧数据缓冲区
				in_iMediaDataLen			//帧数据长度
				) )
			{
			case 1:
				iFrameType = NPC_D_MON_CSD_VIDEO_FRAME_TYPE_I;
				break;
			case 2:
				iFrameType = NPC_D_MON_CSD_VIDEO_FRAME_TYPE_P;
				break;
			case 0:
			default:
				iFrameType = NPC_D_MON_CSD_VIDEO_FRAME_TYPE_P;
				break;
			}
			
			//主码流
			if ( pCameraData->bMainStreamOpenFlag )
			{
				//输入视频流
				NPC_F_DPS_InputVideoStream(
					pMcSrvData->hUmspProSrv,			//设备协议网络服务句柄
					pCameraData->iChNo,					//通道号
					0,									//码流号，0：主码流，1：子码流
					iFrameType,							//帧类型，参见NPC_D_MON_CSD_VIDEO_FRAME_TYPE_*定义
					in_pMediaDataBuf,					//媒体数据缓冲区
					in_iMediaDataLen					//媒体数据长度
					);
			}

			//子码流
			if ( pCameraData->bSubStreamOpenFlag )
			{
				//输入视频流
				NPC_F_DPS_InputVideoStream(
					pMcSrvData->hUmspProSrv,			//设备协议网络服务句柄
					pCameraData->iChNo,					//通道号
					1,									//码流号，0：主码流，1：子码流
					iFrameType,							//帧类型，参见NPC_D_MON_CSD_VIDEO_FRAME_TYPE_*定义
					in_pMediaDataBuf,					//媒体数据缓冲区
					in_iMediaDataLen					//媒体数据长度
					);
			}

			//第三码流
			if ( pCameraData->bThirdStreamOpenFlag)
			{
				//输入视频流
				NPC_F_DPS_InputVideoStream(
					pMcSrvData->hUmspProSrv,			//设备协议网络服务句柄
					pCameraData->iChNo,					//通道号
					2,									//码流号，0：主码流，1：子码流
					iFrameType,							//帧类型，参见NPC_D_MON_CSD_VIDEO_FRAME_TYPE_*定义
					in_pMediaDataBuf,					//媒体数据缓冲区
					in_iMediaDataLen					//媒体数据长度
					);
			}

			pCameraData->dwRecvFrameNum++;

			if ( time(NULL) - pCameraData->tPrecShowCacheTime >= 1 )
			{
				NPC_INT iMainCacheVideoFrameNum, iSubCacheVideoFrameNum;

				pCameraData->tPrecShowCacheTime = time(NULL);
				iMainCacheVideoFrameNum = iSubCacheVideoFrameNum = 0;

				if ( pCameraData->bMainStreamOpenFlag )
				{
					//查询通道缓存视频帧数
					iMainCacheVideoFrameNum = NPC_F_DPS_QueryCacheVideoFrameNum(
						pMcSrvData->hUmspProSrv,		//设备协议网络服务句柄
						pCameraData->iChNo,				//通道号
						0								//码流号，0：主码流，1：子码流
						);
				}

				if ( pCameraData->bSubStreamOpenFlag )
				{
					//查询通道缓存视频帧数
					iSubCacheVideoFrameNum = NPC_F_DPS_QueryCacheVideoFrameNum(
						pMcSrvData->hUmspProSrv,		//设备协议网络服务句柄
						pCameraData->iChNo,				//通道号
						1								//码流号，0：主码流，1：子码流
						);
				}

				//写日志
				NPC_CHAR sTempLogBuf[256];
				sprintf(sTempLogBuf, "[UMSP]  Cache video frame num: main: %d, sub: %d.", 
					iMainCacheVideoFrameNum, iSubCacheVideoFrameNum);
				NPC_F_LOG_SR_WriteLog(sTempLogBuf);
			}
		}
		else
		{
			//音频
			if ( pCameraData->bMainStreamOpenFlag )
			{
				//输入音频流
				NPC_F_DPS_InputAudioStream(
					pMcSrvData->hUmspProSrv,			//设备协议网络服务句柄
					pCameraData->iChNo,					//通道号
					0,									//码流号，0：主码流，1：子码流
					in_pMediaDataBuf,					//媒体数据缓冲区
					in_iMediaDataLen					//媒体数据长度
					);
			}

			if ( pCameraData->bSubStreamOpenFlag )
			{
				//输入音频流
				NPC_F_DPS_InputAudioStream(
					pMcSrvData->hUmspProSrv,			//设备协议网络服务句柄
					pCameraData->iChNo,					//通道号
					1,									//码流号，0：主码流，1：子码流
					in_pMediaDataBuf,					//媒体数据缓冲区
					in_iMediaDataLen					//媒体数据长度
					);
			}
		}

		//写录像文件数据
		if ( pCameraData->hNavFile )
		{
			NPC_CHAR cFrameType;
			NPC_DWORD dwTimestamp;
			NPC_DWORD dwRecOffsetTime;
			NPC_BOOL bIfNeedWriteData;

			bIfNeedWriteData = FALSE;
			cFrameType = 0;

			if ( in_iMediaType == 0 )
			{
				if ( !pCameraData->bIfWriteIFrame )
				{
					if ( iFrameType == NPC_D_MON_CSD_VIDEO_FRAME_TYPE_I )
					{
						bIfNeedWriteData = TRUE;
						pCameraData->bIfWriteIFrame = TRUE;
					}
				}
				else
				{
					bIfNeedWriteData = TRUE;
				}

				switch (iFrameType)
				{
				case NPC_D_MON_CSD_VIDEO_FRAME_TYPE_I:
					cFrameType = 'I';
					break;
				case NPC_D_MON_CSD_VIDEO_FRAME_TYPE_P:
				default:
					cFrameType = 'P';
					break;
				}
			}
			else
			{
				bIfNeedWriteData = pCameraData->bIfWriteIFrame;
			}

			if ( bIfNeedWriteData )
			{
				dwRecOffsetTime = pCameraData->dwRecvFrameNum / pCameraData->iFrameRate;
				dwTimestamp = dwRecOffsetTime * 1000 + 
					(pCameraData->dwRecvFrameNum % pCameraData->iFrameRate) * 
					(1000 / pCameraData->iFrameRate);

				//向文件添加媒体数据（视频或音频）
				if ( !NPC_F_NAV_AddMediaData(
					pCameraData->hNavFile,		//NAV文件句柄
					in_iMediaType,				//媒体类型，0：视频，1：音频
					cFrameType,					//帧类型，视频为‘I’，‘P’，‘B’等，音频该参数无效
					dwTimestamp,				//时间戳
					dwRecOffsetTime,			//录像偏移时间（秒），相对于文件头的时长
					in_pMediaDataBuf,			//媒体数据缓冲区（原始码流数据）
					in_iMediaDataLen			//媒体数据长度
					) )
				{
					//写日志
					NPC_F_LOG_SR_WriteLog("[UMSP]  NPC_F_UMSP_FILECAM_SDK_RealDataCallBack NPC_F_NAV_AddMediaData fail.");
				}
			}
		}
	}

	//CLEAR AREA
	{
		//解锁
		NPC_F_THREAD_Mutex_Unlock(
			pCameraData->hWorkLock				//互斥锁句柄
			);
	}
}

//协议事件回调函数定义
NPC_VOID	NPC_F_UMSP_DPS_ProEventCallback(
	IN		PNPC_VOID								in_pUserData,				//用户数据
	IN		PNPC_BYTE								in_pMsgDataBuf,				//消息数据缓冲区，包括NPC_S_DPS_MSG_HEAD+包体数据
	IN		NPC_INT									in_iMsgDataLen				//消息数据长度
	)
{
	PNPC_S_UMSP_MCSRV_DATA							pMcSrvData;
	PNPC_S_DPS_MSG_HEAD								pMsgHead;

	pMcSrvData = (PNPC_S_UMSP_MCSRV_DATA)in_pUserData;
	if ( pMcSrvData == NULL )
		return ;

	pMsgHead = (PNPC_S_DPS_MSG_HEAD)in_pMsgDataBuf;

	switch ( pMsgHead->iMsgFuncId )
	{
	case NPC_D_DPS_MSGID_EVENT_E2_OPEN_MEDIA_STREAM:		//打开媒体流
		NPC_F_UMSP_MC_DoProEvent_EVENT_E2_OPEN_MEDIA_STREAM(pMcSrvData, pMsgHead, in_pMsgDataBuf, in_iMsgDataLen);
		break;
	case NPC_D_DPS_MSGID_EVENT_E2_CLOSE_MEDIA_STREAM:		//关闭媒体流
		NPC_F_UMSP_MC_DoProEvent_EVENT_E2_CLOSE_MEDIA_STREAM(pMcSrvData, pMsgHead, in_pMsgDataBuf, in_iMsgDataLen);
		break;
	case NPC_D_DPS_MSGID_EVENT_E2_PTZ_CTRL:					//云台控制
		NPC_F_UMSP_MC_DoProEvent_EVENT_E2_PTZ_CTRL(pMcSrvData, pMsgHead, in_pMsgDataBuf, in_iMsgDataLen);
		break;
	case NPC_D_DPS_MSGID_EVENT_E2_FORCE_I_FRAME:			//请求强制发送I帧
		NPC_F_UMSP_MC_DoProEvent_EVENT_E2_FORCE_I_FRAME(pMcSrvData, pMsgHead, in_pMsgDataBuf, in_iMsgDataLen);
		break;
	case NPC_D_DPS_MSGID_EVENT_E2_GET_VIDEO_QUALITY_CAP:	//获取视频质量等级能力列表
		NPC_F_UMSP_MC_DoProEvent_EVENT_E2_GET_VIDEO_QUALITY_CAP(pMcSrvData, pMsgHead, in_pMsgDataBuf, in_iMsgDataLen);
		break;
	case NPC_D_DPS_MSGID_EVENT_E2_GET_VIDEO_QUALITY_LEVEL:	//获取视频质量等级
		NPC_F_UMSP_MC_DoProEvent_EVENT_E2_GET_VIDEO_QUALITY_LEVEL(pMcSrvData, pMsgHead, in_pMsgDataBuf, in_iMsgDataLen);
		break;
	case NPC_D_DPS_MSGID_EVENT_E2_SET_VIDEO_QUALITY_LEVEL:	//设置视频质量等级
		NPC_F_UMSP_MC_DoProEvent_EVENT_E2_SET_VIDEO_QUALITY_LEVEL(pMcSrvData, pMsgHead, in_pMsgDataBuf, in_iMsgDataLen);
		break;

	case NPC_D_DPS_MSGID_EVENT_E2_FILE_QUERY:				//查询文件（录像或图片文件）
		NPC_F_UMSP_MC_DoProEvent_EVENT_E2_FILE_QUERY(pMcSrvData, pMsgHead, in_pMsgDataBuf, in_iMsgDataLen);
		break;
	case NPC_D_DPS_MSGID_EVENT_E2_NSP_FILE_QUERY:			//扩展查询文件（录像或图片文件）
		NPC_F_UMSP_MC_DoProEvent_EVENT_E2_NSP_FILE_QUERY(pMcSrvData, pMsgHead, in_pMsgDataBuf, in_iMsgDataLen);
		break;

	case NPC_D_DPS_MSGID_EVENT_E2_FILE_RANGE_QUERY:
		NPC_F_UMSP_MC_DoProEvent_EVENT_E2_FILE_RANGE_QUERY(pMcSrvData, pMsgHead, in_pMsgDataBuf, in_iMsgDataLen);
		break;

	case NPC_D_DPS_MSGID_EVENT_E2_FILE_OPEN:				//打开文件
		NPC_F_UMSP_MC_DoProEvent_EVENT_E2_FILE_OPEN(pMcSrvData, pMsgHead, in_pMsgDataBuf, in_iMsgDataLen);
		break;

	case NPC_D_DPS_MSGID_EVENT_E2_FILE_OPEN_BY_TIME:
		NPC_F_UMSP_MC_DoProEvent_EVENT_E2_FILE_OPEN_BY_TIME(pMcSrvData, pMsgHead, in_pMsgDataBuf, in_iMsgDataLen);
		break;

	case NPC_D_DPS_MSGID_EVENT_E2_FILE_CLOSE:				//关闭文件
		NPC_F_UMSP_MC_DoProEvent_EVENT_E2_FILE_CLOSE(pMcSrvData, pMsgHead, in_pMsgDataBuf, in_iMsgDataLen);
		break;
	case NPC_D_DPS_MSGID_EVENT_E2_FILE_READ:				//读取文件
		NPC_F_UMSP_MC_DoProEvent_EVENT_E2_FILE_READ(pMcSrvData, pMsgHead, in_pMsgDataBuf, in_iMsgDataLen);
		break;
	case NPC_D_DPS_MSGID_EVENT_E2_FILE_SEEK:				//定位文件
		NPC_F_UMSP_MC_DoProEvent_EVENT_E2_FILE_SEEK(pMcSrvData, pMsgHead, in_pMsgDataBuf, in_iMsgDataLen);
		break;

	case NPC_D_DPS_MSGID_EVENT_E2_FILE_SEEK_BY_TIME:		//定位文件,按时间方式
		NPC_F_UMSP_MC_DoProEvent_EVENT_E2_FILE_SEEK_BY_TIME(pMcSrvData, pMsgHead, in_pMsgDataBuf, in_iMsgDataLen);
		break;
	case NPC_D_DPS_MSGID_EVENT_E2_FILE_QVIDEO:				//查询视频参数
		NPC_F_UMSP_MC_DoProEvent_EVENT_E2_FILE_QVIDEO(pMcSrvData, pMsgHead, in_pMsgDataBuf, in_iMsgDataLen);
		break;
	case NPC_D_DPS_MSGID_EVENT_E2_FILE_ONLY_IFRAME:			//请求I帧数据
		NPC_F_UMSP_MC_DoProEvent_EVENT_E2_FILE_ONLY_IFRAME(pMcSrvData, pMsgHead, in_pMsgDataBuf, in_iMsgDataLen);
		break;
	case NPC_D_DPS_MSGID_EVENT_E2_FILE_RECOVER_ALLFRAME:	//请求恢复全帧数据
		NPC_F_UMSP_MC_DoProEvent_EVENT_E2_FILE_RECOVER_ALLFRAME(pMcSrvData, pMsgHead, in_pMsgDataBuf, in_iMsgDataLen);
		break;

	case NPC_D_DPS_MSGID_EVENT_E2_TALK_START:				//开始语音对讲
		NPC_F_UMSP_MC_DoProEvent_EVENT_E2_TALK_START(pMcSrvData, pMsgHead, in_pMsgDataBuf, in_iMsgDataLen);
		break;
	case NPC_D_DPS_MSGID_EVENT_E2_TALK_STOP:				//停止语音对讲
		NPC_F_UMSP_MC_DoProEvent_EVENT_E2_TALK_STOP(pMcSrvData, pMsgHead, in_pMsgDataBuf, in_iMsgDataLen);
		break;

	case NPC_D_DPS_MSGID_EVENT_E3_MODIFY_USER_PWD:			//修改用户密码
		NPC_F_UMSP_MC_DoProEvent_EVENT_E3_MODIFY_USER_PWD(pMcSrvData, pMsgHead, in_pMsgDataBuf, in_iMsgDataLen);
		break;
	case NPC_D_DPS_MSGID_EVENT_E3_GET_CONFIG:				//获取配置参数
		NPC_F_UMSP_MC_DoProEvent_EVENT_E3_GET_CONFIG(pMcSrvData, pMsgHead, in_pMsgDataBuf, in_iMsgDataLen);
		break;
	case NPC_D_DPS_MSGID_EVENT_E3_SET_CONFIG:				//设置配置参数
		NPC_F_UMSP_MC_DoProEvent_EVENT_E3_SET_CONFIG(pMcSrvData, pMsgHead, in_pMsgDataBuf, in_iMsgDataLen);
		break;
	case NPC_D_DPS_MSGID_EVENT_E3_GET_DEF_CONFIG:			//获取缺省配置参数
		NPC_F_UMSP_MC_DoProEvent_EVENT_E3_GET_DEF_CONFIG(pMcSrvData, pMsgHead, in_pMsgDataBuf, in_iMsgDataLen);
		break;
	case NPC_D_DPS_MSGID_EVENT_E3_IEC_IMP_CONFIG:			//导入配置
		NPC_F_UMSP_MC_DoProEvent_EVENT_E3_IEC_IMP_CONFIG(pMcSrvData, pMsgHead, in_pMsgDataBuf, in_iMsgDataLen);
		break;
	case NPC_D_DPS_MSGID_EVENT_E3_IEC_EXP_CONFIG:			//导出配置
		NPC_F_UMSP_MC_DoProEvent_EVENT_E3_IEC_EXP_CONFIG(pMcSrvData, pMsgHead, in_pMsgDataBuf, in_iMsgDataLen);
		break;

	case NPC_D_DPS_MSGID_EVENT_E3_QUERY_NET_INFO:			//查询网络信息
		NPC_F_UMSP_MC_DoProEvent_EVENT_E3_QUERY_NET_INFO(pMcSrvData, pMsgHead, in_pMsgDataBuf, in_iMsgDataLen);
		break;
	case NPC_D_DPS_MSGID_EVENT_E3_MODIFY_NET_ADDR:			//修改网络地址
		NPC_F_UMSP_MC_DoProEvent_EVENT_E3_MODIFY_NET_ADDR(pMcSrvData, pMsgHead, in_pMsgDataBuf, in_iMsgDataLen);
		break;

	case NPC_D_DPS_MSGID_EVENT_E3_COM_OPEN:					//打开串口
		NPC_F_UMSP_MC_DoProEvent_EVENT_E3_COM_OPEN(pMcSrvData, pMsgHead, in_pMsgDataBuf, in_iMsgDataLen);
		break;
	case NPC_D_DPS_MSGID_EVENT_E3_COM_CLOSE:				//关闭串口
		NPC_F_UMSP_MC_DoProEvent_EVENT_E3_COM_CLOSE(pMcSrvData, pMsgHead, in_pMsgDataBuf, in_iMsgDataLen);
		break;
	case NPC_D_DPS_MSGID_EVENT_E3_COM_SEND_DATA:			//发送数据(客户端发送给设备端)
		NPC_F_UMSP_MC_DoProEvent_EVENT_E3_COM_SEND_DATA(pMcSrvData, pMsgHead, in_pMsgDataBuf, in_iMsgDataLen);
		break;
	case NPC_D_DPS_MSGID_EVENT_E3_COM_SUBS_DATA:			//设备串口数据(设备端发送给客户端)(无应答)
		NPC_F_UMSP_MC_DoProEvent_EVENT_E3_COM_SUBS_DATA(pMcSrvData, pMsgHead, in_pMsgDataBuf, in_iMsgDataLen);
		break;

	case NPC_D_DPS_MSGID_EVENT_E3_UNS_ADD_USER:				//增加用户
		NPC_F_UMSP_MC_DoProEvent_EVENT_E3_UNS_ADD_USER(pMcSrvData, pMsgHead, in_pMsgDataBuf, in_iMsgDataLen);
		break;
	case NPC_D_DPS_MSGID_EVENT_E3_UNS_DEL_USER:				//删除用户
		NPC_F_UMSP_MC_DoProEvent_EVENT_E3_UNS_DEL_USER(pMcSrvData, pMsgHead, in_pMsgDataBuf, in_iMsgDataLen);
		break;
	case NPC_D_DPS_MSGID_EVENT_E3_UNS_GET_USER_POP:			//获取用户权限列表
		NPC_F_UMSP_MC_DoProEvent_EVENT_E3_UNS_GET_USER_POP(pMcSrvData, pMsgHead, in_pMsgDataBuf, in_iMsgDataLen);
		break;
	case NPC_D_DPS_MSGID_EVENT_E3_UNS_SET_USER_POP:			//设置用户权限列表
		NPC_F_UMSP_MC_DoProEvent_EVENT_E3_UNS_SET_USER_POP(pMcSrvData, pMsgHead, in_pMsgDataBuf, in_iMsgDataLen);
		break;

	case NPC_D_DPS_MSGID_EVENT_E7_CUSTOM:					//定制功能
		NPC_F_UMSP_MC_DoProEvent_EVENT_E7_CUSTOM(pMcSrvData, pMsgHead, in_pMsgDataBuf, in_iMsgDataLen);
		break;

	case NPC_D_DPS_MSGID_EVENT_E7_RMY_QUERY_ABILITY:		//查询能力集
		NPC_F_UMSP_MC_DoProEvent_EVENT_E7_RMY_QUERY_ABILITY(pMcSrvData, pMsgHead, in_pMsgDataBuf, in_iMsgDataLen);
		break;
	case NPC_D_DPS_MSGID_EVENT_E7_RMY_REBOOT:				//重启设备(无包体)(无应答消息)
		NPC_F_UMSP_MC_DoProEvent_EVENT_E7_RMY_REBOOT(pMcSrvData, pMsgHead, in_pMsgDataBuf, in_iMsgDataLen);
		break;
	case NPC_D_DPS_MSGID_EVENT_E7_RMY_STOP:					//停止设备(无包体)(无应答消息)
		NPC_F_UMSP_MC_DoProEvent_EVENT_E7_RMY_STOP(pMcSrvData, pMsgHead, in_pMsgDataBuf, in_iMsgDataLen);
		break;

	case NPC_D_DPS_MSGID_EVENT_E7_DUP_QUERY_DEV_VER_INFO:	//查询设备版本信息
		NPC_F_UMSP_MC_DoProEvent_EVENT_E7_DUP_QUERY_DEV_VER_INFO(pMcSrvData, pMsgHead, in_pMsgDataBuf, in_iMsgDataLen);
		break;
	case NPC_D_DPS_MSGID_EVENT_E7_DUP_REQU_UPGRADE:			//请求升级
		NPC_F_UMSP_MC_DoProEvent_EVENT_E7_DUP_REQU_UPGRADE(pMcSrvData, pMsgHead, in_pMsgDataBuf, in_iMsgDataLen);
		break;
	case NPC_D_DPS_MSGID_EVENT_E7_DUP_QUERY_PROGRESS:		//查询升级进度
		NPC_F_UMSP_MC_DoProEvent_EVENT_E7_DUP_QUERY_PROGRESS(pMcSrvData, pMsgHead, in_pMsgDataBuf, in_iMsgDataLen);
		break;
	case NPC_D_DPS_MSGID_EVENT_E7_DUP_QUERY_TRANS_POS:		//查询传输位置
		NPC_F_UMSP_MC_DoProEvent_EVENT_E7_DUP_QUERY_TRANS_POS(pMcSrvData, pMsgHead, in_pMsgDataBuf, in_iMsgDataLen);
		break;
	case NPC_D_DPS_MSGID_EVENT_E7_DUP_UPGRADE_DATA:			//升级数据包
		NPC_F_UMSP_MC_DoProEvent_EVENT_E7_DUP_UPGRADE_DATA(pMcSrvData, pMsgHead, in_pMsgDataBuf, in_iMsgDataLen);
		break;

	case NPC_D_DPS_MSGID_MEDIA_E8_AUDIO_FRAME:				//音频帧数据
		NPC_F_UMSP_MC_DoProEvent_MEDIA_E8_AUDIO_FRAME(pMcSrvData, pMsgHead, in_pMsgDataBuf, in_iMsgDataLen);
		break;

// 	case NPC_D_DPS_MSGID_EVENT_E2_GUARD:					//布防（停用）
// 		NPC_F_UMSP_MC_DoProEvent_EVENT_E2_GUARD(pMcSrvData, pMsgHead, in_pMsgDataBuf, in_iMsgDataLen);
// 		break;
// 	case NPC_D_DPS_MSGID_EVENT_E2_UNGUARD:					//撤防（停用）
// 		NPC_F_UMSP_MC_DoProEvent_EVENT_E2_UNGUARD(pMcSrvData, pMsgHead, in_pMsgDataBuf, in_iMsgDataLen);
// 		break;

	default:
		{
			//写日志
			NPC_CHAR sTempLogBuf[256];
			sprintf(sTempLogBuf, "[UMSP]  Massage function id error: %8.8X.", pMsgHead->iMsgFuncId);
			NPC_F_LOG_SR_WriteLog(sTempLogBuf);

			pMsgHead->ucResult = NPC_D_MON_CSD_DPS_ERROR_CODE_MSGFUNC_ERROR;

			//发送消息给设备协议网络服务
			NPC_F_DPS_SendDevProData(
				pMcSrvData->hUmspProSrv,			//设备协议网络服务句柄
				in_pMsgDataBuf,						//设备协议数据缓冲区，包括NPC_S_DPS_MSG_HEAD
				in_iMsgDataLen						//设备协议数据长度
				);
		}
		break;
	}
}

//查询用户信息回调函数定义
NPC_BOOL	NPC_F_UMSP_DPS_QueryUserInfoCallback(
	IN		PNPC_VOID								in_pUserData,				//用户数据
	IN		PNPC_CHAR								in_pUserName,				//用户名
	OUT		PNPC_S_DPS_USER_INFO					out_pUserInfo				//用户信息
	)
{
	NPC_CHAR										sCfgFileName[256];
	NPC_BOOL										bPwdIfMd5;
	NPC_CHAR										sKeyName[128];
	NPC_CHAR										sKeyValue[128];
	NPC_CHAR										sUserName[32];
	NPC_CHAR										sUserPwd[32];
	NPC_INT											i, k, iUserNum;

	strcpy(sCfgFileName, ".\\npc_umsp_server_config.ini");

	//用户数
	iUserNum = GetPrivateProfileInt("USER", "UserNum", -1, sCfgFileName);
	if ( iUserNum < 0 )
	{
		WritePrivateProfileString("USER", "UserNum", "1", sCfgFileName);
		WritePrivateProfileString("USER", "UserName[0]", "admin", sCfgFileName);
		WritePrivateProfileString("USER", "UserPwd[0]", "admin", sCfgFileName);

		iUserNum = 1;
	}

	//NPC_BOOL									bPwdIfMd5;					//密码是否为MD5，如果为TRUE表示密码为MD5加密后的，FALSE表示密码为明文
	bPwdIfMd5 = GetPrivateProfileInt("USER", "PwdIfMd5", 0, sCfgFileName);

	for ( i=0; i<iUserNum; i++ )
	{
		sprintf(sKeyName, "UserName[%d]", i);
		GetPrivateProfileString("USER", sKeyName, "", sUserName, sizeof(sUserName), sCfgFileName);

		if ( strcmp(in_pUserName, sUserName) == 0 )
		{
			sprintf(sKeyName, "UserPwd[%d]", i);
			GetPrivateProfileString("USER", sKeyName, "", sUserPwd, sizeof(sUserPwd), sCfgFileName);

			strcpy(out_pUserInfo->sUserName, in_pUserName);
			if ( bPwdIfMd5 )
			{
				//MD5加密
				NPC_TOOLS_MD5_MD5Encrypt(out_pUserInfo->sUserPwd, sUserPwd);
			}
			else
			{
				strcpy(out_pUserInfo->sUserPwd, sUserPwd);
			}
			memset(out_pUserInfo->ucPopTable, 1, sizeof(out_pUserInfo->ucPopTable));

			//全局权限
			sprintf(sKeyName, "GlobalPop[%d]", i);
			GetPrivateProfileString("USER", sKeyName, "", sKeyValue, sizeof(sKeyValue), sCfgFileName);

			if ( sKeyValue[0] == 0 )
				out_pUserInfo->dwGlobalPop = 0;
			else
				out_pUserInfo->dwGlobalPop = NPC_F_SYS_HexStrToDWORD(sKeyValue);

			//通道权限
			for ( k=0; k<NPC_D_DPS_MAX_CHANNEL_NUM; k++ )
			{
				sprintf(sKeyName, "ChPop[%d][%d]", i, k);
				GetPrivateProfileString("USER", sKeyName, "", sKeyValue, sizeof(sKeyValue), sCfgFileName);

				if ( sKeyValue[0] == 0 )
					out_pUserInfo->dwChPopTable[k] = 0;
				else
					out_pUserInfo->dwChPopTable[k] = NPC_F_SYS_HexStrToDWORD(sKeyValue);
			}

/*
#define NPC_D_DPS_POP_REALPLAY							0x00000001				//实时预览权限（具有全局和通道权限）
#define NPC_D_DPS_POP_PTZ_CTRL							0x00000002				//云台控制权限（具有全局和通道权限）
#define NPC_D_DPS_POP_TALK								0x00000004				//语音对讲权限（具有全局和通道权限）
#define NPC_D_DPS_POP_QUERY_FILE						0x00000008				//录像文件查询权限（只有全局权限）
#define NPC_D_DPS_POP_BACKPLAY							0x00000010				//录像回放权限（只有全局权限）
#define NPC_D_DPS_POP_ALARM								0x00000020				//报警布防权限（只有全局权限）
#define NPC_D_DPS_POP_GET_CONFIG						0x00000040				//查询配置权限（只有全局权限）
#define NPC_D_DPS_POP_SET_CONFIG						0x00000080				//设置配置权限（只有全局权限）
*/
			return TRUE;
		}
	}

	return FALSE;
}

NPC_INT			G_iSum = 0;

//查询用户信息回调函数定义
NPC_BOOL	NPC_F_UMSP_DPS_DoConnectCallback(
	IN		PNPC_VOID								in_pUserData,				//用户数据
	OUT		PNPC_S_DPS_CONNECT_INFO					out_pDoConnInfo				//连接处理信息	
	)
{
	if (G_iSum >= 10)
	{
		out_pDoConnInfo->iRet = 1;	
	}
	else if (out_pDoConnInfo->iType == 0)
	{
		G_iSum++;
	}
	
	if (out_pDoConnInfo->iType == 0)
	{
		
		printf("[DEBUG_UMSP]:获取到连接处理回调事件,ID = %0.8x, 事件类型：连接\r\n", out_pDoConnInfo->dwLoginID);
	}
	else
	{
		printf("[DEBUG_UMSP]:获取到连接处理回调事件,ID = %0.8x, 事件类型：断开连接\r\n", out_pDoConnInfo->dwLoginID);
	}
	return TRUE;
}
//-------------------------------------------------------------------------------
//启动主控服务
NPC_BOOL	NPC_F_UMSP_MC_StartMcServer(
	IN		PNPC_S_UMSP_MCSRV_DATA					in_pMcSrvData				//主控服务数据
	)
{
	NPC_BOOL										bRet;

	//INIT AREA
	{
		bRet = TRUE;
	}

	//CODE AREA
	{
		//初始化数据
		if ( !NPC_F_UMSP_MC_InitData(
			in_pMcSrvData			//主控服务数据
			) )
		{
			//写日志
			NPC_F_LOG_SR_WriteLog("[UMSP]  NPC_F_UMSP_MC_StartMcServer NPC_F_UMSP_MC_InitData fail.");

			bRet = FALSE;
			goto _NPC_CLEAR;
		}

		//读取配置
		if ( !NPC_F_UMSP_MC_ReadConfig(
			in_pMcSrvData				//主控服务数据
			) )
		{
			//写日志
			NPC_F_LOG_SR_WriteLog("[UMSP]  NPC_F_UMSP_MC_StartMcServer NPC_F_UMSP_MC_ReadConfig fail.");

			bRet = FALSE;
			goto _NPC_CLEAR;
		}

		//初始化文件摄像机SDK
		if ( !NPC_F_FILECAM_SDK_InitSdk() )
		{
			//写日志
			NPC_F_LOG_SR_WriteLog("[UMSP]  NPC_F_UMSP_MC_StartMcServer NPC_F_FILECAM_SDK_InitSdk fail.");

			bRet = FALSE;
			goto _NPC_CLEAR;
		}

		//连接文件摄像机
		in_pMcSrvData->hFileCamera = NPC_F_FILECAM_SDK_ConnectFilecam(
			"FileCamera",				//目录名
			&in_pMcSrvData->iChNum		//通道数
			);
		if ( in_pMcSrvData->hFileCamera == NULL )
		{
			//写日志
			NPC_F_LOG_SR_WriteLog("[UMSP]  NPC_F_UMSP_MC_StartMcServer NPC_F_FILECAM_SDK_ConnectFilecam fail.");

			bRet = FALSE;
			goto _NPC_CLEAR;
		}

		//写日志
		NPC_F_LOG_SR_WriteLog("[UMSP]  Connection file camera success.");

		strcpy(in_pMcSrvData->tDevCapInfo.sDevType, "FileCamera");			//设备类型
		strcpy(in_pMcSrvData->tDevCapInfo.sDevModel, "FileCamera");			//设备型号
		strcpy(in_pMcSrvData->tDevCapInfo.sSerialNumber, "123456");			//序列号
		in_pMcSrvData->tDevCapInfo.iVideoChanNum = in_pMcSrvData->iChNum;	//设备通道个数
		in_pMcSrvData->tDevCapInfo.iAudioChanNum = in_pMcSrvData->iChNum;	//设备语音通道数
		in_pMcSrvData->tDevCapInfo.iAlarmInPortNum = 0;						//报警输入个数
		in_pMcSrvData->tDevCapInfo.iAlarmOutPortNum = 0;					//报警输出个数
		in_pMcSrvData->tDevCapInfo.iDiskNum = 0;							//硬盘个数
		in_pMcSrvData->tDevCapInfo.iSubProtoNum = 2;						//子码流数
		in_pMcSrvData->tDevCapInfo.bIfSupportZeroChannel = TRUE;			//零通道
		in_pMcSrvData->tDevCapInfo.bIfSupportRecordBytime = TRUE;			//按时间回放
		in_pMcSrvData->tDevCapInfo.bIfSupportFileRange = FALSE;				//录像时间范围

#if 0
		in_pMcSrvData->tDevCapInfo.bIfOpenDoConnCallback = TRUE;
		in_pMcSrvData->tDevCapInfo.pDoConnCallback = (PNPC_VOID)NPC_F_UMSP_DPS_DoConnectCallback;
		in_pMcSrvData->tDevCapInfo.pDoConnUserData = (PNPC_VOID)in_pMcSrvData;
#endif

		//启动设备协议网络服务
		in_pMcSrvData->hUmspProSrv = NPC_F_DPS_StartDevProNetServer(
			5800,										//媒体TCP监听端口号
			&in_pMcSrvData->tDevCapInfo,				//设备能力信息
			NPC_F_UMSP_DPS_ProEventCallback,			//协议事件回调函数指针
			(PNPC_VOID)in_pMcSrvData,					//协议事件用户数据
			NPC_F_UMSP_DPS_QueryUserInfoCallback,		//查询用户信息回调函数指针
			(PNPC_VOID)in_pMcSrvData,					//查询用户信息用户数据
			in_pMcSrvData->tConfig.bIfDebug,			//是否调试，在调试状态下，会输出很多日志信息，正式使用时需要关闭
			in_pMcSrvData->tConfig.bIfShowStreamLog,	//是否显示流日志，即媒体流收发日志信息
			in_pMcSrvData->tConfig.bIfWriteLogToFile,	//是否写日志到文件
			in_pMcSrvData->tConfig.sLogDir,				//日志目录，日志文件名自动产生，格式为Log日期.txt
			in_pMcSrvData->tConfig.usLogPort,			//日志端口号，如果为0，则不启动日志监听端口，大于0则启动日志监听端口服务
			(in_pMcSrvData->tConfig.bPwdIfMd5?1:0)		//密码模式（指密码传给SDK的方式），0：明文，1：MD5
			);
		if ( in_pMcSrvData->hUmspProSrv == NULL )
		{
			//写日志
			NPC_F_LOG_SR_WriteLog("[UMSP]  NPC_F_UMSP_MC_StartMcServer NPC_F_DPS_StartDevProNetServer fail.");

			bRet = FALSE;
			goto _NPC_CLEAR;
		}

		//设置运行状态信息显示开关
		NPC_F_DPS_SetShowRunState(
			in_pMcSrvData->hUmspProSrv,				//设备协议网络服务句柄
			TRUE									//是否显示运行状态信息，TRUE表示要显示，FALSE表示不显示，缺省为TRUE
			);

		

		//设置实时预览只发I帧的时长（秒）
		//NPC_F_DPS_SetOnlySendIFrameTimeLen(
		//	in_pMcSrvData->hUmspProSrv,			//设备协议网络服务句柄
		//	30									//只发I帧的时长（秒），缺省为60秒
		//	);

		//启动线程
		if ( !NPC_F_UMSP_MC_StartThread(
			in_pMcSrvData							//主控服务数据
			) )
		{
			//写日志
			NPC_F_LOG_SR_WriteLog("[UMSP]  NPC_F_UMSP_MC_StartMcServer NPC_F_UMSP_MC_StartThread fail.");

			bRet = FALSE;
			goto _NPC_CLEAR;
		}

		//写日志
		NPC_F_LOG_SR_WriteLog("[UMSP]  The success of network service protocol for starting the equipment.");
	}

	//CLEAR AREA
	_NPC_CLEAR:
	{
		if ( !bRet )
		{
			//停止主控服务
			NPC_F_UMSP_MC_StopMcServer(
				in_pMcSrvData				//主控服务数据
				);
		}
	}

	return bRet;
}

//停止主控服务
NPC_VOID	NPC_F_UMSP_MC_StopMcServer(
	IN		PNPC_S_UMSP_MCSRV_DATA					in_pMcSrvData				//主控服务数据
	)
{
	//退出线程
	NPC_F_UMSP_MC_QuitThread(
		in_pMcSrvData						//主控服务数据
		);

	//断开连接文件摄像机
	if ( in_pMcSrvData->hFileCamera )
	{
		NPC_F_FILECAM_SDK_DisconnectFilecam(
			in_pMcSrvData->hFileCamera		//文件摄像机句柄
			);
		in_pMcSrvData->hFileCamera = NULL;
	}

	//反初始化文件摄像机SDK
	NPC_F_FILECAM_SDK_UninitSdk();

	//停止设备协议网络服务
	if ( in_pMcSrvData->hUmspProSrv )
	{
		NPC_F_DPS_StopDevProNetServer(
			in_pMcSrvData->hUmspProSrv		//设备协议网络服务句柄
			);
		in_pMcSrvData->hUmspProSrv = NULL;
	}

	//反初始化数据
	NPC_F_UMSP_MC_UninitData(
		in_pMcSrvData						//主控服务数据
		);
}

//初始化数据
NPC_BOOL	NPC_F_UMSP_MC_InitData(
	IN		PNPC_S_UMSP_MCSRV_DATA					in_pMcSrvData				//主控服务数据
	)
{
	NPC_INT											i;
	PNPC_S_UMSP_CAMERA_DATA							pCameraData;

	//初始化摄像机数据
	for ( i=0; i<NPC_D_DPS_MAX_CHANNEL_NUM; i++ )
	{
		pCameraData = &in_pMcSrvData->tCameraTable[i];

		//创建互斥锁
		pCameraData->hWorkLock = NPC_F_THREAD_Mutex_Create();
		if ( pCameraData->hWorkLock == NULL )
		{
			return FALSE;
		}

		sprintf(pCameraData->sChName, "Camera%2.2d", i);
	}

	in_pMcSrvData->iMp3FileNum = NPC_D_UMSP_MC_MAX_MP3_FILE_NUM / 4;
	for ( i=0; i<in_pMcSrvData->iMp3FileNum; i++ )
	{
		in_pMcSrvData->tMp3FileTable[i].bIfUsed = TRUE;
		sprintf(in_pMcSrvData->tMp3FileTable[i].sFileName, "TestFile-%d.mp3", i);
		in_pMcSrvData->tMp3FileTable[i].bIfPlaying = FALSE;
	}

	return TRUE;
}

//反初始化数据
NPC_VOID	NPC_F_UMSP_MC_UninitData(
	IN		PNPC_S_UMSP_MCSRV_DATA					in_pMcSrvData				//主控服务数据
	)
{
	NPC_INT											i;

	for ( i=0; i<NPC_D_DPS_MAX_CHANNEL_NUM; i++ )
	{
		if ( in_pMcSrvData->tCameraTable[i].hWorkLock )
		{
			//释放互斥锁
			NPC_F_THREAD_Mutex_Destroy(
				in_pMcSrvData->tCameraTable[i].hWorkLock	//互斥锁句柄
				);
			in_pMcSrvData->tCameraTable[i].hWorkLock = NULL;
		}
	}
}

//读取配置
NPC_BOOL	NPC_F_UMSP_MC_ReadConfig(
	IN		PNPC_S_UMSP_MCSRV_DATA					in_pMcSrvData				//主控服务数据
	)
{
	NPC_CHAR										sCfgFileName[256];

	strcpy(sCfgFileName, ".\\npc_umsp_server_config.ini");

	//NPC_BOOL									bIfDebug;					//是否调试，在调试状态下，会输出很多日志信息，正式使用时需要关闭
	in_pMcSrvData->tConfig.bIfDebug = GetPrivateProfileInt("DEBUG", "IfDebug", 0, sCfgFileName);

	//NPC_BOOL									bIfShowStreamLog;			//是否显示流日志，即媒体流收发日志信息
	in_pMcSrvData->tConfig.bIfShowStreamLog = GetPrivateProfileInt("DEBUG", "IfShowStreamLog", 0, sCfgFileName);

	//NPC_BOOL									bIfWriteLogToFile;			//是否写日志到文件
	in_pMcSrvData->tConfig.bIfWriteLogToFile = GetPrivateProfileInt("DEBUG", "IfWriteLogToFile", 0, sCfgFileName);

	if ( in_pMcSrvData->tConfig.bIfWriteLogToFile )
	{
		//NPC_CHAR								sLogDir[256];				//日志目录，日志文件名自动产生，格式为Log日期.txt
		GetPrivateProfileString("DEBUG", "LogDir", "", in_pMcSrvData->tConfig.sLogDir, sizeof(in_pMcSrvData->tConfig.sLogDir), sCfgFileName);
	}

	//NPC_USHORT								usLogPort;					//日志端口号，如果为0，则不启动日志监听端口，大于0则启动日志监听端口服务
	in_pMcSrvData->tConfig.usLogPort = GetPrivateProfileInt("DEBUG", "LogPort", 0, sCfgFileName);

	//NPC_BOOL									bPwdIfMd5;					//密码是否为MD5，如果为TRUE表示密码为MD5加密后的，FALSE表示密码为明文
	in_pMcSrvData->tConfig.bPwdIfMd5 = GetPrivateProfileInt("USER", "PwdIfMd5", 0, sCfgFileName);

	//NPC_BOOL									bIfSendAlarmMsg;			//是否发送报警消息
	in_pMcSrvData->tConfig.bIfSendAlarmMsg = GetPrivateProfileInt("DEBUG", "IfSendAlarmMsg", 1, sCfgFileName);

	//NPC_INT									iSendAlarmIntervalTime;		//发送报警消息的间隔时间（秒）
	in_pMcSrvData->tConfig.iSendAlarmIntervalTime = GetPrivateProfileInt("DEBUG", "SendAlarmIntervalTime", 10, sCfgFileName);

	//NPC_INT									iMaxFileConnNum;			//最大文件连接数，最大不能超过NPC_D_UMSP_MC_MAX_FILE_WORK_NUM
	in_pMcSrvData->tConfig.iMaxFileConnNum = GetPrivateProfileInt("DEBUG", "MaxFileConnNum", NPC_D_UMSP_MC_MAX_FILE_WORK_NUM, sCfgFileName);
	if ( in_pMcSrvData->tConfig.iMaxFileConnNum <= 0 )
		in_pMcSrvData->tConfig.iMaxFileConnNum = 1;
	else if ( in_pMcSrvData->tConfig.iMaxFileConnNum > NPC_D_UMSP_MC_MAX_FILE_WORK_NUM )
		in_pMcSrvData->tConfig.iMaxFileConnNum = NPC_D_UMSP_MC_MAX_FILE_WORK_NUM;

	return TRUE;
}

//读取动态配置
NPC_BOOL	NPC_F_UMSP_MC_ReadDynamicConfig(
	IN		PNPC_S_UMSP_MCSRV_DATA					in_pMcSrvData				//主控服务数据
	)
{
	NPC_CHAR										sCfgFileName[256];

	strcpy(sCfgFileName, ".\\npc_umsp_server_config.ini");

	//NPC_BOOL									bIfSwitchVideo;				//是否切换视频
	in_pMcSrvData->tConfig.bIfSwitchVideo = GetPrivateProfileInt("DEBUG", "IfSwitchVideo", 0, sCfgFileName);

	if ( in_pMcSrvData->tConfig.bIfSwitchVideo )
	{
		WritePrivateProfileString("DEBUG", "IfSwitchVideo", "0", sCfgFileName);
	}

	return TRUE;
}


//-------------------------------------------------------------------------------
//启动线程
NPC_BOOL	NPC_F_UMSP_MC_StartThread(
	IN		PNPC_S_UMSP_MCSRV_DATA					in_pMcSrvData				//主控服务数据
	)
{
	in_pMcSrvData->bRunFlag = TRUE;

	//创建线程
	in_pMcSrvData->hThread = NPC_F_THREAD_CreateThread(
		NPC_F_UMSP_MC_ThreadWork,				//线程工作过程
		(PNPC_VOID)in_pMcSrvData				//传入参数
		);
	if ( in_pMcSrvData->hThread == NULL )
	{
		return FALSE;
	}

	return TRUE;
}

//退出线程
NPC_VOID	NPC_F_UMSP_MC_QuitThread(
	IN		PNPC_S_UMSP_MCSRV_DATA					in_pMcSrvData				//主控服务数据
	)
{
	in_pMcSrvData->bRunFlag = FALSE;

	if ( in_pMcSrvData->hThread )
	{
		//等待退出线程
		NPC_F_THREAD_WaitExitThread(
			in_pMcSrvData->hThread,			//线程句柄
			5000							//超时时长（毫秒）
			);
	}
}

//工作线程
NPC_VOID	NPC_F_UMSP_MC_ThreadWork(
	IN		PNPC_VOID								in_pParameter				//参数指针
	)
{
	PNPC_S_UMSP_MCSRV_DATA							pMcSrvData;

	pMcSrvData = (PNPC_S_UMSP_MCSRV_DATA)in_pParameter;
	if ( pMcSrvData == NULL )
		return;

	while ( pMcSrvData->bRunFlag )
	{
		//处理定时器
		NPC_F_UMSP_MC_DoTimer(
			pMcSrvData			//DPS主控服务数据
			);

		NPC_F_SYS_Sleep(100);
	}
}

//处理定时器
NPC_VOID	NPC_F_UMSP_MC_DoTimer(
	IN		PNPC_S_UMSP_MCSRV_DATA					in_pMcSrvData				//主控服务数据
	)
{
	time_t											tCurTime;

	tCurTime = time(NULL);

	if ( tCurTime - in_pMcSrvData->tPrecReadAlarmStateTime >= 1 )
	{
		in_pMcSrvData->tPrecReadAlarmStateTime = tCurTime;

		//处理报警状态
		NPC_F_UMSP_MC_DoAlarmState(
			in_pMcSrvData				//主控服务数据
			);
	}

	if ( tCurTime - in_pMcSrvData->tPrecReadConfigTime >= 5 )
	{
		in_pMcSrvData->tPrecReadConfigTime = tCurTime;

		//读取动态配置
		NPC_F_UMSP_MC_ReadDynamicConfig(
			in_pMcSrvData				//主控服务数据
			);

		if ( in_pMcSrvData->tConfig.bIfSwitchVideo )
		{
			in_pMcSrvData->tConfig.bIfSwitchVideo = FALSE;

			//关闭所有视频流
			NPC_F_UMSP_MC_CloseVideoStream(
				in_pMcSrvData			//主控服务数据
				);
		}
	}
}


//-------------------------------------------------------------------------------
//处理协议事件：打开媒体流
NPC_VOID	NPC_F_UMSP_MC_DoProEvent_EVENT_E2_OPEN_MEDIA_STREAM(
	IN		PNPC_S_UMSP_MCSRV_DATA					in_pMcSrvData,				//主控服务数据
	IN		PNPC_S_DPS_MSG_HEAD						in_pMsgHead,				//消息头
	IN		PNPC_BYTE								in_pMsgDataBuf,				//消息数据缓冲区，包括NPC_S_DPS_MSG_HEAD
	IN		NPC_INT									in_iMsgDataLen				//消息数据长度
	)
{
	PNPC_S_DPS_MSG_BODY_EVENT_E2_OPEN_MEDIA_STREAM	pMsgBody;
	PNPC_S_UMSP_CAMERA_DATA							pCameraData;
	NPC_BOOL										bIfLock;
	NPC_S_FILECAM_SDK_CH_PARAM						tChParam = {0};
	NPC_INT											iChNo = 0;

	//INIT AREA
	{
		pMsgBody = (PNPC_S_DPS_MSG_BODY_EVENT_E2_OPEN_MEDIA_STREAM)((PNPC_BYTE)in_pMsgHead + sizeof(NPC_S_DPS_MSG_HEAD));

		in_pMsgHead->ucResult = 0;
		in_pMsgHead->ucMsgType = NPC_D_DPS_MSG_TYPE_RESPONSION;

		pCameraData = NULL;
		bIfLock = FALSE;
	}

	//CODE AREA
	{
		//显示信息
		NPC_CHAR sTempLogBuf[256];
		sprintf(sTempLogBuf, "[UMSP]  Open media stream: channel no: %d, stream no: %d.", 
			pMsgBody->i_iChNo, 
			pMsgBody->i_iStreamNo);
		NPC_F_LOG_SR_ShowInfo(sTempLogBuf);

			
		if ( pMsgBody->i_iChNo < 0 || pMsgBody->i_iChNo >= NPC_D_DPS_MAX_CHANNEL_NUM && pMsgBody->i_iChNo != NPC_D_DPS_ZERO_CHANNEL_VALUE)
		{
			//写日志
			NPC_F_LOG_SR_WriteLog("[UMSP]  NPC_F_UMSP_MC_DoProEvent_EVENT_E2_OPEN_MEDIA_STREAM i_iChNo error.");

			in_pMsgHead->ucResult = NPC_D_MON_CSD_DPS_ERROR_CODE_CHANNEL_NO_ERROR;
			goto _NPC_CLEAR;
		}

		if ( pMsgBody->i_iStreamNo < 0 || pMsgBody->i_iStreamNo > NPC_D_DPS_MAX_CH_STREAM_NUM )
		{
			//写日志
			NPC_F_LOG_SR_WriteLog("[UMSP]  NPC_F_UMSP_MC_DoProEvent_EVENT_E2_OPEN_MEDIA_STREAM i_iStreamNo error.");

			in_pMsgHead->ucResult = NPC_D_MON_CSD_DPS_ERROR_CODE_SUB_STREAM_NO_ERROR;
			goto _NPC_CLEAR;
		}

		if (pMsgBody->i_iChNo == NPC_D_DPS_ZERO_CHANNEL_VALUE)
		{
			iChNo = NPC_D_DPS_MAX_CHANNEL_NUM;
		}
		else
		{
			iChNo = pMsgBody->i_iChNo;
		}
		pCameraData = &(in_pMcSrvData->tCameraTable[iChNo]);

		//加锁
		NPC_F_THREAD_Mutex_Lock(
			pCameraData->hWorkLock		//互斥锁句柄
			);
		bIfLock = TRUE;

		switch ( pMsgBody->i_iStreamNo)
		{
		case 0:
			{
				pCameraData->bMainStreamOpenFlag = TRUE;
				pCameraData->iMainVideoLevel = -1;
			}
			break;

		case 1:
			{
				pCameraData->bSubStreamOpenFlag = TRUE;
				pCameraData->iSubVideoLevel = -1;
			}
			break;
		case 2:
			{
				pCameraData->bThirdStreamOpenFlag = TRUE;
				pCameraData->iSubVideoLevel = -1;
			}
			break;
		}
		

		pCameraData->iStreamNo = pMsgBody->i_iStreamNo;

		if ( !pCameraData->bIfUsed )
		{
			if (pMsgBody->i_iChNo != NPC_D_DPS_ZERO_CHANNEL_VALUE)
			{
				//开始预览
				if ( !NPC_F_FILECAM_SDK_RealPlay(
					in_pMcSrvData->hFileCamera,					//文件摄像机句柄
					pMsgBody->i_iChNo,							//通道号
					NPC_F_UMSP_FILECAM_SDK_RealDataCallBack,	//媒体数据回调函数
					(NPC_DWORD)pCameraData,						//用户数据
					&tChParam									//文件摄像机参数
					) )
				{
					//写日志
					NPC_F_LOG_SR_WriteLog("[UMSP]  Open file camera realplay fail!");
					
					in_pMsgHead->ucResult = NPC_D_MON_CSD_DPS_ERROR_CODE_OPEN_STREAM_FAIL;
					goto _NPC_CLEAR;
				}
			}
			else
			{
				//开始预览
				if ( !NPC_F_FILECAM_SDK_RealPlay(
					in_pMcSrvData->hFileCamera,					//文件摄像机句柄
					0,							//通道号
					NPC_F_UMSP_FILECAM_SDK_RealDataCallBack,	//媒体数据回调函数
					(NPC_DWORD)pCameraData,						//用户数据
					&tChParam									//文件摄像机参数
					) )
				{
					//写日志
					NPC_F_LOG_SR_WriteLog("[UMSP]  Open file camera realplay fail!");
					
					in_pMsgHead->ucResult = NPC_D_MON_CSD_DPS_ERROR_CODE_OPEN_STREAM_FAIL;
					goto _NPC_CLEAR;
				}
			}
			

			pCameraData->bIfUsed = TRUE;
			pCameraData->iChNo = pMsgBody->i_iChNo;
			pCameraData->pMcSrvData = (PNPC_VOID)in_pMcSrvData;
			pCameraData->iFrameRate = tChParam.iFrameRate;
			pCameraData->bIfSupportAudio = tChParam.bIfSupportAudio;

			pCameraData->tRecStartTime = 0;
			pCameraData->dwRecvFrameNum = 0;
			pCameraData->bIfWriteIFrame = FALSE;

			//开始录像
			if ( !NPC_F_UMSP_MC_StartRecord(
				in_pMcSrvData,				//主控服务数据
				pCameraData,				//摄像机数据
				pMsgBody->i_iChNo,			//通道号
				tChParam.iFrameRate			//帧率
				) )
			{
				//写日志
				NPC_F_LOG_SR_WriteLog("[UMSP]  Start record fail!");
			}
		}

		pMsgBody->o_bVideoFlag = TRUE;									//视频标志，TRUE：表示有视频，FALSE：表示没有视频
		pMsgBody->o_bAudioFlag = pCameraData->bIfSupportAudio;			//音频标志，TRUE：表示有音频，FALSE：表示没有音频

		pMsgBody->o_iVideoCodecId = NPC_D_MON_CSD_CODEC_ID_VEDIO_H264;	//视频编码ID，参见NPC_D_MON_CSD_CODEC_ID_VEDIO_*定义
		pMsgBody->o_iVideoFrameRate = tChParam.iFrameRate;				//视频帧率
		pMsgBody->o_iVideoBitRate = 128;								//视频码率（kbps）
		pMsgBody->o_usVideoWidth = 352;									//视频宽度
		pMsgBody->o_usVideoHeight = 288;								//视频高度

		if ( pCameraData->bIfSupportAudio )
		{
			pMsgBody->o_iAudioCodecId = NPC_D_MON_CSD_CODEC_ID_AUDIO_PCMA;	//音频编码ID，参见NPC_D_MON_CSD_CODEC_ID_*定义
			pMsgBody->o_iAudioBitRate = 64;									//音频码率（kbps），如64
			pMsgBody->o_iAudioSampleRate = 8000;							//音频采样率，如8000
			pMsgBody->o_iAudioChNum = 1;									//音频声道数，如1
		}
	}

	//CLEAR AREA
	_NPC_CLEAR:
	{
		if ( pCameraData && bIfLock )
		{
			//解锁
			NPC_F_THREAD_Mutex_Unlock(
				pCameraData->hWorkLock			//互斥锁句柄
				);
		}

		//发送消息给设备协议网络服务
		NPC_F_DPS_SendDevProData(
			in_pMcSrvData->hUmspProSrv,			//设备协议网络服务句柄
			in_pMsgDataBuf,						//设备协议数据缓冲区，包括NPC_S_DPS_MSG_HEAD
			in_iMsgDataLen						//设备协议数据长度
			);
	}
}

//处理协议事件：关闭媒体流
NPC_VOID	NPC_F_UMSP_MC_DoProEvent_EVENT_E2_CLOSE_MEDIA_STREAM(
	IN		PNPC_S_UMSP_MCSRV_DATA					in_pMcSrvData,				//主控服务数据
	IN		PNPC_S_DPS_MSG_HEAD						in_pMsgHead,				//消息头
	IN		PNPC_BYTE								in_pMsgDataBuf,				//消息数据缓冲区，包括NPC_S_DPS_MSG_HEAD
	IN		NPC_INT									in_iMsgDataLen				//消息数据长度
	)
{
	PNPC_S_DPS_MSG_BODY_EVENT_E2_CLOSE_MEDIA_STREAM	pMsgBody;
	PNPC_S_UMSP_CAMERA_DATA							pCameraData;
	NPC_BOOL										bIfLock;
	NPC_INT											iChNo;

	//INIT AREA
	{
		pMsgBody = (PNPC_S_DPS_MSG_BODY_EVENT_E2_CLOSE_MEDIA_STREAM)((PNPC_BYTE)in_pMsgHead + sizeof(NPC_S_DPS_MSG_HEAD));

		if ( in_pMsgHead->ucMsgType != NPC_D_DPS_MSG_TYPE_REQUEST )
			return;

		in_pMsgHead->ucResult = 0;
		in_pMsgHead->ucMsgType = NPC_D_DPS_MSG_TYPE_RESPONSION;

		pCameraData = NULL;
		bIfLock = FALSE;
	}

	//CODE AREA
	{
		//显示信息
		NPC_CHAR sTempLogBuf[256];
		sprintf(sTempLogBuf, "[UMSP]  Close media stream: channel no: %d, stream no: %d.", 
			pMsgBody->i_iChNo, 
			pMsgBody->i_iStreamNo);
		NPC_F_LOG_SR_ShowInfo(sTempLogBuf);

		if ( pMsgBody->i_iChNo < 0 || pMsgBody->i_iChNo >= NPC_D_DPS_MAX_CHANNEL_NUM && pMsgBody->i_iChNo != NPC_D_DPS_ZERO_CHANNEL_VALUE)
		{
			//写日志
			NPC_F_LOG_SR_WriteLog("[UMSP]  NPC_F_UMSP_MC_DoProEvent_EVENT_E2_CLOSE_MEDIA_STREAM i_iChNo error.");

			in_pMsgHead->ucResult = NPC_D_MON_CSD_DPS_ERROR_CODE_CHANNEL_NO_ERROR;
			goto _NPC_CLEAR;
		}

		if ( pMsgBody->i_iStreamNo < 0 || pMsgBody->i_iStreamNo >= NPC_D_DPS_MAX_CH_STREAM_NUM )
		{
			//写日志
			NPC_F_LOG_SR_WriteLog("[UMSP]  NPC_F_UMSP_MC_DoProEvent_EVENT_E2_CLOSE_MEDIA_STREAM i_iStreamNo error.");

			in_pMsgHead->ucResult = NPC_D_MON_CSD_DPS_ERROR_CODE_SUB_STREAM_NO_ERROR;
			goto _NPC_CLEAR;
		}

		if (pMsgBody->i_iChNo == NPC_D_DPS_ZERO_CHANNEL_VALUE)
		{
			iChNo = NPC_D_DPS_MAX_CHANNEL_NUM;
		}
		else
		{
			iChNo = pMsgBody->i_iChNo;
		}

		pCameraData = &(in_pMcSrvData->tCameraTable[iChNo]);

		//加锁
		NPC_F_THREAD_Mutex_Lock(
			pCameraData->hWorkLock		//互斥锁句柄
			);
		bIfLock = TRUE;

		if ( pMsgBody->i_iStreamNo == 0 )
			pCameraData->bMainStreamOpenFlag = FALSE;
		else
			pCameraData->bSubStreamOpenFlag = FALSE;

		if ( pCameraData->bIfUsed && 
			!pCameraData->bMainStreamOpenFlag && 
			!pCameraData->bSubStreamOpenFlag )
		{
			//结束预览
			NPC_F_FILECAM_SDK_StopRealPlay(
				in_pMcSrvData->hFileCamera,			//文件摄像机句柄
				pMsgBody->i_iChNo					//通道号
				);
			pCameraData->bIfUsed = FALSE;

			//停止录像
			NPC_F_UMSP_MC_StopRecord(
				in_pMcSrvData,			//主控服务数据
				pCameraData				//摄像机数据
				);
		}
	}

	//CLEAR AREA
	_NPC_CLEAR:
	{
		if ( pCameraData && bIfLock )
		{
			//解锁
			NPC_F_THREAD_Mutex_Unlock(
				pCameraData->hWorkLock			//互斥锁句柄
				);
		}

		//发送消息给设备协议网络服务
		NPC_F_DPS_SendDevProData(
			in_pMcSrvData->hUmspProSrv,			//设备协议网络服务句柄
			in_pMsgDataBuf,						//设备协议数据缓冲区，包括NPC_S_DPS_MSG_HEAD
			in_iMsgDataLen						//设备协议数据长度
			);
	}
}

//处理协议事件：云台控制
NPC_VOID	NPC_F_UMSP_MC_DoProEvent_EVENT_E2_PTZ_CTRL(
	IN		PNPC_S_UMSP_MCSRV_DATA					in_pMcSrvData,				//主控服务数据
	IN		PNPC_S_DPS_MSG_HEAD						in_pMsgHead,				//消息头
	IN		PNPC_BYTE								in_pMsgDataBuf,				//消息数据缓冲区，包括NPC_S_DPS_MSG_HEAD
	IN		NPC_INT									in_iMsgDataLen				//消息数据长度
	)
{
	PNPC_S_DPS_MSG_BODY_EVENT_E2_PTZ_CTRL			pMsgBody;

	//INIT AREA
	{
		pMsgBody = (PNPC_S_DPS_MSG_BODY_EVENT_E2_PTZ_CTRL)((PNPC_BYTE)in_pMsgHead + sizeof(NPC_S_DPS_MSG_HEAD));

		in_pMsgHead->ucResult = 0;
		in_pMsgHead->ucMsgType = NPC_D_DPS_MSG_TYPE_RESPONSION;
	}

	//CODE AREA
	{
		//显示信息
		NPC_CHAR sTempLogBuf[256];
		sprintf(sTempLogBuf, "[UMSP]  PTZ ctrl: channel no: %d, order: %d, param: %d, ext_param1: %d, ext_param2: %d.", 
			pMsgBody->i_iChNo, 
			pMsgBody->i_iPtzCmd, 
			pMsgBody->i_dwPtzData, 
			pMsgBody->i_dwPtzExtParam1, 
			pMsgBody->i_dwPtzExtParam2 
			);
		NPC_F_LOG_SR_ShowInfo(sTempLogBuf);

		if ( pMsgBody->i_iChNo < 0 || pMsgBody->i_iChNo >= NPC_D_DPS_MAX_CHANNEL_NUM )
		{
			//写日志
			NPC_F_LOG_SR_WriteLog("[UMSP]  NPC_F_UMSP_MC_DoProEvent_EVENT_E2_PTZ_CTRL i_iChNo error.");

			in_pMsgHead->ucResult = NPC_D_MON_CSD_DPS_ERROR_CODE_CHANNEL_NO_ERROR;
			goto _NPC_CLEAR;
		}

#if 0
		if (pMsgBody->i_iPtzCmd == 101)
		{
			//写日志
			NPC_F_LOG_SR_WriteLog("[UMSP]  NPC_F_UMSP_MC_DoProEvent_EVENT_E2_PTZ_CTRL PtzCmd = 101.");
			
			in_pMsgHead->ucResult = NPC_D_MON_CSD_DPS_ERROR_CODE_WITHOUT_PERMISSION;
			goto _NPC_CLEAR;
		}
#endif
	}

	//CLEAR AREA
	_NPC_CLEAR:
	{
		//发送消息给设备协议网络服务
		NPC_F_DPS_SendDevProData(
			in_pMcSrvData->hUmspProSrv,			//设备协议网络服务句柄
			in_pMsgDataBuf,						//设备协议数据缓冲区，包括NPC_S_DPS_MSG_HEAD
			in_iMsgDataLen						//设备协议数据长度
			);
	}
}

//处理协议事件：请求强制发送I帧
NPC_VOID	NPC_F_UMSP_MC_DoProEvent_EVENT_E2_FORCE_I_FRAME(
	IN		PNPC_S_UMSP_MCSRV_DATA					in_pMcSrvData,				//主控服务数据
	IN		PNPC_S_DPS_MSG_HEAD						in_pMsgHead,				//消息头
	IN		PNPC_BYTE								in_pMsgDataBuf,				//消息数据缓冲区，包括NPC_S_DPS_MSG_HEAD
	IN		NPC_INT									in_iMsgDataLen				//消息数据长度
	)
{
	PNPC_S_DPS_MSG_BODY_EVENT_E2_FORCE_I_FRAME		pMsgBody;

	//INIT AREA
	{
		pMsgBody = (PNPC_S_DPS_MSG_BODY_EVENT_E2_FORCE_I_FRAME)((PNPC_BYTE)in_pMsgHead + sizeof(NPC_S_DPS_MSG_HEAD));

		in_pMsgHead->ucResult = 0;
		in_pMsgHead->ucMsgType = NPC_D_DPS_MSG_TYPE_RESPONSION;
	}

	//CODE AREA
	{
		//这个是示例程序，未处理强制I帧，设备程序需要处理该消息，强制从I帧发送

		//显示信息
		NPC_CHAR sTempLogBuf[256];
		sprintf(sTempLogBuf, "[UMSP]  Force i frame: channel no: %d, stream no: %d.", 
			pMsgBody->i_iChNo, 
			pMsgBody->i_iStreamNo
			);
		NPC_F_LOG_SR_ShowInfo(sTempLogBuf);

		if ( pMsgBody->i_iChNo < 0 || pMsgBody->i_iChNo >= NPC_D_DPS_MAX_CHANNEL_NUM )
		{
			//写日志
			NPC_F_LOG_SR_WriteLog("[UMSP]  NPC_F_UMSP_MC_DoProEvent_EVENT_E2_FORCE_I_FRAME i_iChNo error.");

			in_pMsgHead->ucResult = NPC_D_MON_CSD_DPS_ERROR_CODE_CHANNEL_NO_ERROR;
			goto _NPC_CLEAR;
		}

		if ( pMsgBody->i_iStreamNo < 0 || pMsgBody->i_iStreamNo >= NPC_D_DPS_MAX_CH_STREAM_NUM )
		{
			//写日志
			NPC_F_LOG_SR_WriteLog("[UMSP]  NPC_F_UMSP_MC_DoProEvent_EVENT_E2_FORCE_I_FRAME i_iStreamNo error.");

			in_pMsgHead->ucResult = NPC_D_MON_CSD_DPS_ERROR_CODE_SUB_STREAM_NO_ERROR;
			goto _NPC_CLEAR;
		}

		//请求强制I帧
		NPC_F_FILECAM_SDK_RequestForceIFrame(
			in_pMcSrvData->hFileCamera,			//文件摄像机句柄
			pMsgBody->i_iChNo					//通道号
			);
	}

	//CLEAR AREA
	_NPC_CLEAR:
	{
		//发送消息给设备协议网络服务
		NPC_F_DPS_SendDevProData(
			in_pMcSrvData->hUmspProSrv,			//设备协议网络服务句柄
			in_pMsgDataBuf,						//设备协议数据缓冲区，包括NPC_S_DPS_MSG_HEAD
			in_iMsgDataLen						//设备协议数据长度
			);
	}
}


//处理协议事件：获取视频质量等级能力列表
NPC_VOID	NPC_F_UMSP_MC_DoProEvent_EVENT_E2_GET_VIDEO_QUALITY_CAP(
	IN		PNPC_S_UMSP_MCSRV_DATA					in_pMcSrvData,				//主控服务数据
	IN		PNPC_S_DPS_MSG_HEAD						in_pMsgHead,				//消息头
	IN		PNPC_BYTE								in_pMsgDataBuf,				//消息数据缓冲区，包括NPC_S_DPS_MSG_HEAD
	IN		NPC_INT									in_iMsgDataLen				//消息数据长度
	)
{
	PNPC_S_DPS_MSG_BODY_EVENT_E2_GET_VIDEO_QUALITY_CAP		pMsgBody;

	//INIT AREA
	{
		pMsgBody = (PNPC_S_DPS_MSG_BODY_EVENT_E2_GET_VIDEO_QUALITY_CAP)((PNPC_BYTE)in_pMsgHead + sizeof(NPC_S_DPS_MSG_HEAD));

		in_pMsgHead->ucResult = 0;
		in_pMsgHead->ucMsgType = NPC_D_DPS_MSG_TYPE_RESPONSION;
	}

	//CODE AREA
	{
		//显示信息
		NPC_CHAR sTempLogBuf[256];
		sprintf(sTempLogBuf, "[UMSP]  Get video quality cap: channel no: %d, stream no: %d.", 
			pMsgBody->i_usChNo, 
			pMsgBody->i_usStreamNo
			);
		NPC_F_LOG_SR_ShowInfo(sTempLogBuf);

		if ( pMsgBody->i_usChNo < 0 || pMsgBody->i_usChNo >= NPC_D_DPS_MAX_CHANNEL_NUM )
		{
			//写日志
			NPC_F_LOG_SR_WriteLog("[UMSP]  NPC_F_UMSP_MC_DoProEvent_EVENT_E2_GET_VIDEO_QUALITY_CAP i_usChNo error.");

			in_pMsgHead->ucResult = NPC_D_MON_CSD_DPS_ERROR_CODE_CHANNEL_NO_ERROR;
			goto _NPC_CLEAR;
		}

		if ( pMsgBody->i_usStreamNo < 0 || pMsgBody->i_usStreamNo >= NPC_D_DPS_MAX_CH_STREAM_NUM )
		{
			//写日志
			NPC_F_LOG_SR_WriteLog("[UMSP]  NPC_F_UMSP_MC_DoProEvent_EVENT_E2_GET_VIDEO_QUALITY_CAP i_usStreamNo error.");

			in_pMsgHead->ucResult = NPC_D_MON_CSD_DPS_ERROR_CODE_SUB_STREAM_NO_ERROR;
			goto _NPC_CLEAR;
		}

		pMsgBody->o_iLevelNum = 3;

		strcpy(pMsgBody->o_tLevelTable[0].sLevelName, "高");		//等级名称
		pMsgBody->o_tLevelTable[0].iLevelValue = 0;					//等级值，必须大于等于0
		pMsgBody->o_tLevelTable[0].usWidth = 1280;					//图像宽
		pMsgBody->o_tLevelTable[0].usHeight = 720;					//图像高
		pMsgBody->o_tLevelTable[0].usFrameRate = 10;				//帧率
		pMsgBody->o_tLevelTable[0].usBitRate = 1280;				//码率，单位：kbps

		strcpy(pMsgBody->o_tLevelTable[1].sLevelName, "中");		//等级名称
		pMsgBody->o_tLevelTable[1].iLevelValue = 1;					//等级值，必须大于等于0
		pMsgBody->o_tLevelTable[1].usWidth = 720;					//图像宽
		pMsgBody->o_tLevelTable[1].usHeight = 576;					//图像高
		pMsgBody->o_tLevelTable[1].usFrameRate = 8;					//帧率
		pMsgBody->o_tLevelTable[1].usBitRate = 512;					//码率，单位：kbps

		strcpy(pMsgBody->o_tLevelTable[2].sLevelName, "低");		//等级名称
		pMsgBody->o_tLevelTable[2].iLevelValue = 2;					//等级值，必须大于等于0
		pMsgBody->o_tLevelTable[2].usWidth = 320;					//图像宽
		pMsgBody->o_tLevelTable[2].usHeight = 288;					//图像高
		pMsgBody->o_tLevelTable[2].usFrameRate = 5;					//帧率
		pMsgBody->o_tLevelTable[2].usBitRate = 128;					//码率，单位：kbps
	}

	//CLEAR AREA
	_NPC_CLEAR:
	{
		//发送消息给设备协议网络服务
		NPC_F_DPS_SendDevProData(
			in_pMcSrvData->hUmspProSrv,			//设备协议网络服务句柄
			in_pMsgDataBuf,						//设备协议数据缓冲区，包括NPC_S_DPS_MSG_HEAD
			in_iMsgDataLen						//设备协议数据长度
			);
	}
}

//处理协议事件：获取视频质量等级
NPC_VOID	NPC_F_UMSP_MC_DoProEvent_EVENT_E2_GET_VIDEO_QUALITY_LEVEL(
	IN		PNPC_S_UMSP_MCSRV_DATA					in_pMcSrvData,				//主控服务数据
	IN		PNPC_S_DPS_MSG_HEAD						in_pMsgHead,				//消息头
	IN		PNPC_BYTE								in_pMsgDataBuf,				//消息数据缓冲区，包括NPC_S_DPS_MSG_HEAD
	IN		NPC_INT									in_iMsgDataLen				//消息数据长度
	)
{
	PNPC_S_DPS_MSG_BODY_EVENT_E2_GET_VIDEO_QUALITY_LEVEL	pMsgBody;
	PNPC_S_UMSP_CAMERA_DATA									pCameraData;

	//INIT AREA
	{
		pMsgBody = (PNPC_S_DPS_MSG_BODY_EVENT_E2_GET_VIDEO_QUALITY_LEVEL)((PNPC_BYTE)in_pMsgHead + sizeof(NPC_S_DPS_MSG_HEAD));

		in_pMsgHead->ucResult = 0;
		in_pMsgHead->ucMsgType = NPC_D_DPS_MSG_TYPE_RESPONSION;
	}

	//CODE AREA
	{
		//显示信息
		NPC_CHAR sTempLogBuf[256];
		sprintf(sTempLogBuf, "[UMSP]  Get video quality level: channel no: %d, stream no: %d.", 
			pMsgBody->i_usChNo, 
			pMsgBody->i_usStreamNo
			);
		NPC_F_LOG_SR_ShowInfo(sTempLogBuf);

		if ( pMsgBody->i_usChNo < 0 || pMsgBody->i_usChNo >= NPC_D_DPS_MAX_CHANNEL_NUM )
		{
			//写日志
			NPC_F_LOG_SR_WriteLog("[UMSP]  NPC_F_UMSP_MC_DoProEvent_EVENT_E2_GET_VIDEO_QUALITY_LEVEL i_usChNo error.");

			in_pMsgHead->ucResult = NPC_D_MON_CSD_DPS_ERROR_CODE_CHANNEL_NO_ERROR;
			goto _NPC_CLEAR;
		}

		if ( pMsgBody->i_usStreamNo < 0 || pMsgBody->i_usStreamNo >= NPC_D_DPS_MAX_CH_STREAM_NUM )
		{
			//写日志
			NPC_F_LOG_SR_WriteLog("[UMSP]  NPC_F_UMSP_MC_DoProEvent_EVENT_E2_GET_VIDEO_QUALITY_LEVEL i_iStreamNo error.");

			in_pMsgHead->ucResult = NPC_D_MON_CSD_DPS_ERROR_CODE_SUB_STREAM_NO_ERROR;
			goto _NPC_CLEAR;
		}

		pCameraData = &(in_pMcSrvData->tCameraTable[pMsgBody->i_usChNo]);

		if ( pMsgBody->i_usStreamNo == 0 )
			pMsgBody->o_iLevelValue = pCameraData->iMainVideoLevel;
		else
			pMsgBody->o_iLevelValue = pCameraData->iSubVideoLevel;
	}

	//CLEAR AREA
	_NPC_CLEAR:
	{
		//发送消息给设备协议网络服务
		NPC_F_DPS_SendDevProData(
			in_pMcSrvData->hUmspProSrv,			//设备协议网络服务句柄
			in_pMsgDataBuf,						//设备协议数据缓冲区，包括NPC_S_DPS_MSG_HEAD
			in_iMsgDataLen						//设备协议数据长度
			);
	}
}

//处理协议事件：设置视频质量等级
NPC_VOID	NPC_F_UMSP_MC_DoProEvent_EVENT_E2_SET_VIDEO_QUALITY_LEVEL(
	IN		PNPC_S_UMSP_MCSRV_DATA					in_pMcSrvData,				//主控服务数据
	IN		PNPC_S_DPS_MSG_HEAD						in_pMsgHead,				//消息头
	IN		PNPC_BYTE								in_pMsgDataBuf,				//消息数据缓冲区，包括NPC_S_DPS_MSG_HEAD
	IN		NPC_INT									in_iMsgDataLen				//消息数据长度
	)
{
	PNPC_S_DPS_MSG_BODY_EVENT_E2_SET_VIDEO_QUALITY_LEVEL	pMsgBody;
	PNPC_S_UMSP_CAMERA_DATA									pCameraData;

	//INIT AREA
	{
		pMsgBody = (PNPC_S_DPS_MSG_BODY_EVENT_E2_SET_VIDEO_QUALITY_LEVEL)((PNPC_BYTE)in_pMsgHead + sizeof(NPC_S_DPS_MSG_HEAD));

		in_pMsgHead->ucResult = 0;
		in_pMsgHead->ucMsgType = NPC_D_DPS_MSG_TYPE_RESPONSION;
	}

	//CODE AREA
	{
		//显示信息
		NPC_CHAR sTempLogBuf[256];
		sprintf(sTempLogBuf, "[UMSP]  Set video quality level: channel no: %d, stream no: %d, level: %d.", 
			pMsgBody->i_usChNo, 
			pMsgBody->i_usStreamNo, 
			pMsgBody->i_iLevelValue
			);
		NPC_F_LOG_SR_ShowInfo(sTempLogBuf);

		if ( pMsgBody->i_usChNo < 0 || pMsgBody->i_usChNo >= NPC_D_DPS_MAX_CHANNEL_NUM )
		{
			//写日志
			NPC_F_LOG_SR_WriteLog("[UMSP]  NPC_F_UMSP_MC_DoProEvent_EVENT_E2_SET_VIDEO_QUALITY_LEVEL i_iChNo error.");

			in_pMsgHead->ucResult = NPC_D_MON_CSD_DPS_ERROR_CODE_CHANNEL_NO_ERROR;
			goto _NPC_CLEAR;
		}

		if ( pMsgBody->i_usStreamNo < 0 || pMsgBody->i_usStreamNo >= NPC_D_DPS_MAX_CH_STREAM_NUM )
		{
			//写日志
			NPC_F_LOG_SR_WriteLog("[UMSP]  NPC_F_UMSP_MC_DoProEvent_EVENT_E2_SET_VIDEO_QUALITY_LEVEL i_iStreamNo error.");

			in_pMsgHead->ucResult = NPC_D_MON_CSD_DPS_ERROR_CODE_SUB_STREAM_NO_ERROR;
			goto _NPC_CLEAR;
		}

		pCameraData = &(in_pMcSrvData->tCameraTable[pMsgBody->i_usChNo]);

		if ( pMsgBody->i_usStreamNo == 0 )
			pCameraData->iMainVideoLevel = pMsgBody->i_iLevelValue;
		else
			pCameraData->iSubVideoLevel = pMsgBody->i_iLevelValue;
	}

	//CLEAR AREA
	_NPC_CLEAR:
	{
		//发送消息给设备协议网络服务
		NPC_F_DPS_SendDevProData(
			in_pMcSrvData->hUmspProSrv,			//设备协议网络服务句柄
			in_pMsgDataBuf,						//设备协议数据缓冲区，包括NPC_S_DPS_MSG_HEAD
			in_iMsgDataLen						//设备协议数据长度
			);
	}
}


//处理协议事件：查询文件（录像或图片文件）
NPC_VOID	NPC_F_UMSP_MC_DoProEvent_EVENT_E2_FILE_QUERY(
	IN		PNPC_S_UMSP_MCSRV_DATA					in_pMcSrvData,				//主控服务数据
	IN		PNPC_S_DPS_MSG_HEAD						in_pMsgHead,				//消息头
	IN		PNPC_BYTE								in_pMsgDataBuf,				//消息数据缓冲区，包括NPC_S_DPS_MSG_HEAD
	IN		NPC_INT									in_iMsgDataLen				//消息数据长度
	)
{
	NPC_BOOL										bRet;
	PNPC_S_DPS_MSG_BODY_EVENT_E2_FILE_QUERY			pMsgBody;
	NPC_CHAR										sStartTimeStr[20];
	NPC_CHAR										sEndTimeStr[20];

	PNPC_BYTE										pFileInfoDataBuf;
	NPC_INT											iFileInfoDataBufSize;
	NPC_INT											iFileInfoDataLen;

	PNPC_BYTE										pSendDataBuf;
	NPC_INT											iSendDataLen;

	PNPC_S_DPS_MSG_HEAD								pSendMsgHead;

	//INIT AREA
	{
		bRet = TRUE;
		pMsgBody = (PNPC_S_DPS_MSG_BODY_EVENT_E2_FILE_QUERY)((PNPC_BYTE)in_pMsgHead + sizeof(NPC_S_DPS_MSG_HEAD));

		in_pMsgHead->ucResult = 0;
		in_pMsgHead->ucMsgType = NPC_D_DPS_MSG_TYPE_RESPONSION;

		pFileInfoDataBuf = NULL;
		iFileInfoDataBufSize = 0;
		iFileInfoDataLen = 0;

		pSendDataBuf = NULL;
		iSendDataLen = 0;
	}

	//CODE AREA
	{
		//将NPC_S_TD_DATE_TIME转换为时间字符串
		NPC_F_SYS_TdTime_To_TimeStr(
			&pMsgBody->i_tStartTime,		//TD时间
			sStartTimeStr					//时间字符串
			);

		//将NPC_S_TD_DATE_TIME转换为时间字符串
		NPC_F_SYS_TdTime_To_TimeStr(
			&pMsgBody->i_tEndTime,			//TD时间
			sEndTimeStr						//时间字符串
			);

		//显示信息
		NPC_CHAR sTempLogBuf[256];
		sprintf(sTempLogBuf, "[UMSP]  Query file: channel no: %d, file type: %d, create mode: %d, alerm event: %d, start time: %s, end time: %s.", 
			pMsgBody->i_iChNo, 
			pMsgBody->i_iFileType, 
			pMsgBody->i_iCreateMode, 
			pMsgBody->i_iAlarmEvent, 
			sStartTimeStr,  
			sEndTimeStr
			);
		NPC_F_LOG_SR_ShowInfo(sTempLogBuf);

		//--------------------------------------------------------------------
		//查询文件
		if ( !NPC_F_UMSP_MC_FindFile(
			in_pMcSrvData,				//主控服务数据
			pMsgBody->i_iChNo,			//通道号
			&pMsgBody->i_tStartTime,	//开始时间
			&pMsgBody->i_tEndTime,		//结束时间
			&pMsgBody->o_iFileNum,		//文件数
			&pFileInfoDataBuf,			//文件信息数据缓冲区
			&iFileInfoDataBufSize,		//文件信息数据缓冲区大小
			&iFileInfoDataLen			//文件信息数据长度
			) )
		{
			//写日志
			NPC_F_LOG_SR_WriteLog("[UMSP]  NPC_F_UMSP_MC_DoProEvent_EVENT_E2_FILE_QUERY NPC_F_UMSP_MC_FindFile fail.");

			in_pMsgHead->ucResult = NPC_D_MON_CSD_DPS_ERROR_CODE_QUERY_FILE_FAIL;

			bRet = FALSE;
			goto _NPC_CLEAR;
		}

		iSendDataLen = in_iMsgDataLen + iFileInfoDataLen;
		pSendDataBuf = (PNPC_BYTE)malloc(iSendDataLen);
		if ( pSendDataBuf == NULL )
		{
			//写日志
			NPC_F_LOG_SR_WriteLog("[UMSP]  NPC_F_UMSP_MC_DoProEvent_EVENT_E2_FILE_QUERY malloc fail.");

			in_pMsgHead->ucResult = NPC_D_MON_CSD_DPS_ERROR_CODE_OTHER;

			bRet = FALSE;
			goto _NPC_CLEAR;
		}

		memcpy(pSendDataBuf, in_pMsgDataBuf, in_iMsgDataLen);
		if ( pFileInfoDataBuf && iFileInfoDataLen > 0 )
			memcpy(pSendDataBuf+in_iMsgDataLen, pFileInfoDataBuf, iFileInfoDataLen);

		pSendMsgHead = (PNPC_S_DPS_MSG_HEAD)pSendDataBuf;

		pSendMsgHead->iMsgBodyLen = iSendDataLen - sizeof(NPC_S_DPS_MSG_HEAD);

		if ( pMsgBody->o_iFileNum == 0 )
			pSendMsgHead->ucResult = NPC_D_MON_CSD_DPS_ERROR_CODE_RESOURCE_NOT_EXIST;

		//发送消息给设备协议网络服务
		NPC_F_DPS_SendDevProData(
			in_pMcSrvData->hUmspProSrv,			//设备协议网络服务句柄
			pSendDataBuf,						//设备协议数据缓冲区，包括NPC_S_DPS_MSG_HEAD
			iSendDataLen						//设备协议数据长度
			);
	}

	//CLEAR AREA
	_NPC_CLEAR:
	{
		if ( !bRet )
		{
			//发送消息给设备协议网络服务
			NPC_F_DPS_SendDevProData(
				in_pMcSrvData->hUmspProSrv,			//设备协议网络服务句柄
				in_pMsgDataBuf,						//设备协议数据缓冲区，包括NPC_S_DPS_MSG_HEAD
				in_iMsgDataLen						//设备协议数据长度
				);
		}

		if ( pFileInfoDataBuf )
		{
			free(pFileInfoDataBuf);
			pFileInfoDataBuf = NULL;
		}

		if ( pSendDataBuf )
		{
			free(pSendDataBuf);
			pSendDataBuf = NULL;
		}
	}
}

//处理协议事件：扩展查询文件（录像或图片文件）
NPC_VOID	NPC_F_UMSP_MC_DoProEvent_EVENT_E2_NSP_FILE_QUERY(
	IN		PNPC_S_UMSP_MCSRV_DATA					in_pMcSrvData,				//主控服务数据
	IN		PNPC_S_DPS_MSG_HEAD						in_pMsgHead,				//消息头
	IN		PNPC_BYTE								in_pMsgDataBuf,				//消息数据缓冲区，包括NPC_S_DPS_MSG_HEAD
	IN		NPC_INT									in_iMsgDataLen				//消息数据长度
	)
{
	NPC_BOOL										bRet;
	PNPC_S_DPS_MSG_BODY_EVENT_E2_NSP_FILE_QUERY		pMsgBody;
	NPC_CHAR										sStartTimeStr[20];
	NPC_CHAR										sEndTimeStr[20];

	PNPC_BYTE										pFileInfoDataBuf;
	NPC_INT											iFileInfoDataBufSize;
	NPC_INT											iFileInfoDataLen;

	PNPC_BYTE										pSendDataBuf;
	NPC_INT											iSendDataLen;

	PNPC_S_DPS_MSG_HEAD								pSendMsgHead;
	NPC_CHAR										sTempAlarmEventTable[128];
	PNPC_CHAR										pPos;
	NPC_INT											i;

	//INIT AREA
	{
		bRet = TRUE;
		pMsgBody = (PNPC_S_DPS_MSG_BODY_EVENT_E2_NSP_FILE_QUERY)((PNPC_BYTE)in_pMsgHead + sizeof(NPC_S_DPS_MSG_HEAD));

		in_pMsgHead->ucResult = 0;
		in_pMsgHead->ucMsgType = NPC_D_DPS_MSG_TYPE_RESPONSION;

		pFileInfoDataBuf = NULL;
		iFileInfoDataBufSize = 0;
		iFileInfoDataLen = 0;

		pSendDataBuf = NULL;
		iSendDataLen = 0;
	}

	//CODE AREA
	{
		//将NPC_S_TD_DATE_TIME转换为时间字符串
		NPC_F_SYS_TdTime_To_TimeStr(
			&pMsgBody->i_tStartTime,		//TD时间
			sStartTimeStr					//时间字符串
			);

		//将NPC_S_TD_DATE_TIME转换为时间字符串
		NPC_F_SYS_TdTime_To_TimeStr(
			&pMsgBody->i_tEndTime,			//TD时间
			sEndTimeStr						//时间字符串
			);

		sTempAlarmEventTable[0] = 0;
		pPos = sTempAlarmEventTable;
		for ( i=0; i<pMsgBody->i_iAlarmEventNum; i++ )
		{
			if ( i == 0 )
				sprintf(pPos, "%d", pMsgBody->i_ucAlarmEventTable[i]);
			else
				sprintf(pPos, ",%d", pMsgBody->i_ucAlarmEventTable[i]);
			pPos += strlen(pPos);
		}

		//显示信息
		NPC_CHAR sTempLogBuf[512];
		sprintf(sTempLogBuf, "[UMSP]  Query nsp file: channel no: %d, file type: %d, create mode: %d, alerm event: %s, start time: %s, end time: %s.", 
			pMsgBody->i_iChNo, 
			pMsgBody->i_iFileType, 
			pMsgBody->i_iCreateMode, 
			sTempAlarmEventTable, 
			sStartTimeStr,  
			sEndTimeStr
			);
		NPC_F_LOG_SR_ShowInfo(sTempLogBuf);

		//--------------------------------------------------------------------
		//扩展查询文件
		if ( !NPC_F_UMSP_MC_NspFindFile(
			in_pMcSrvData,				//主控服务数据
			pMsgBody->i_iChNo,			//通道号
			pMsgBody->i_iStreamNo,		//码流号
			&pMsgBody->i_tStartTime,	//开始时间
			&pMsgBody->i_tEndTime,		//结束时间
			&pMsgBody->o_iFileNum,		//文件数
			&pFileInfoDataBuf,			//文件信息数据缓冲区，为NPC_S_MON_MPS_FILE_HEAD_INFO数组
			&iFileInfoDataBufSize,		//文件信息数据缓冲区大小
			&iFileInfoDataLen			//文件信息数据长度
			) )
		{
			//写日志
			NPC_F_LOG_SR_WriteLog("[UMSP]  NPC_F_UMSP_MC_DoProEvent_EVENT_E2_NSP_FILE_QUERY NPC_F_UMSP_MC_FindFile fail.");

			in_pMsgHead->ucResult = NPC_D_MON_CSD_DPS_ERROR_CODE_QUERY_FILE_FAIL;

			bRet = FALSE;
			goto _NPC_CLEAR;
		}

		iSendDataLen = in_iMsgDataLen + iFileInfoDataLen;
		pSendDataBuf = (PNPC_BYTE)malloc(iSendDataLen);
		if ( pSendDataBuf == NULL )
		{
			//写日志
			NPC_F_LOG_SR_WriteLog("[UMSP]  NPC_F_UMSP_MC_DoProEvent_EVENT_E2_NSP_FILE_QUERY malloc fail.");

			in_pMsgHead->ucResult = NPC_D_MON_CSD_DPS_ERROR_CODE_OTHER;

			bRet = FALSE;
			goto _NPC_CLEAR;
		}

		memcpy(pSendDataBuf, in_pMsgDataBuf, in_iMsgDataLen);
		if ( pFileInfoDataBuf && iFileInfoDataLen > 0 )
			memcpy(pSendDataBuf+in_iMsgDataLen, pFileInfoDataBuf, iFileInfoDataLen);

		pSendMsgHead = (PNPC_S_DPS_MSG_HEAD)pSendDataBuf;

		pSendMsgHead->iMsgBodyLen = iSendDataLen - sizeof(NPC_S_DPS_MSG_HEAD);

		if ( pMsgBody->o_iFileNum == 0 )
			pSendMsgHead->ucResult = NPC_D_MON_CSD_DPS_ERROR_CODE_RESOURCE_NOT_EXIST;

		//发送消息给设备协议网络服务
		NPC_F_DPS_SendDevProData(
			in_pMcSrvData->hUmspProSrv,			//设备协议网络服务句柄
			pSendDataBuf,						//设备协议数据缓冲区，包括NPC_S_DPS_MSG_HEAD
			iSendDataLen						//设备协议数据长度
			);
	}

	//CLEAR AREA
	_NPC_CLEAR:
	{
		if ( !bRet )
		{
			//发送消息给设备协议网络服务
			NPC_F_DPS_SendDevProData(
				in_pMcSrvData->hUmspProSrv,			//设备协议网络服务句柄
				in_pMsgDataBuf,						//设备协议数据缓冲区，包括NPC_S_DPS_MSG_HEAD
				in_iMsgDataLen						//设备协议数据长度
				);
		}

		if ( pFileInfoDataBuf )
		{
			free(pFileInfoDataBuf);
			pFileInfoDataBuf = NULL;
		}

		if ( pSendDataBuf )
		{
			free(pSendDataBuf);
			pSendDataBuf = NULL;
		}
	}
}


//处理协议事件：查询录像文件时间范围
NPC_VOID	NPC_F_UMSP_MC_DoProEvent_EVENT_E2_FILE_RANGE_QUERY(
	IN		PNPC_S_UMSP_MCSRV_DATA					in_pMcSrvData,				//主控服务数据
	IN		PNPC_S_DPS_MSG_HEAD						in_pMsgHead,				//消息头
	IN		PNPC_BYTE								in_pMsgDataBuf,				//消息数据缓冲区，包括NPC_S_DPS_MSG_HEAD
	IN		NPC_INT									in_iMsgDataLen				//消息数据长度
	)
{
	NPC_BOOL										bRet;
	PNPC_S_DPS_MSG_BODY_EVENT_E2_FILE_RANGE_QUERY	pMsgBody;
	NPC_CHAR										sStartTimeStr[20];
	NPC_CHAR										sEndTimeStr[20];

	PNPC_BYTE										pFileInfoDataBuf;
	NPC_INT											iFileInfoDataBufSize;
	NPC_INT											iFileInfoDataLen;

	PNPC_BYTE										pSendDataBuf;
	NPC_INT											iSendDataLen;

	PNPC_S_DPS_MSG_HEAD								pSendMsgHead;
	NPC_CHAR										sTempAlarmEventTable[128];
	PNPC_CHAR										pPos;
	NPC_INT											i;

	//INIT AREA
	{
		bRet = TRUE;
		pMsgBody = (PNPC_S_DPS_MSG_BODY_EVENT_E2_FILE_RANGE_QUERY)((PNPC_BYTE)in_pMsgHead + sizeof(NPC_S_DPS_MSG_HEAD));

		in_pMsgHead->ucResult = 0;
		in_pMsgHead->ucMsgType = NPC_D_DPS_MSG_TYPE_RESPONSION;

		pFileInfoDataBuf = NULL;
		iFileInfoDataBufSize = 0;
		iFileInfoDataLen = 0;

		pSendDataBuf = NULL;
		iSendDataLen = 0;
	}

	//CODE AREA
	{
		//将NPC_S_TD_DATE_TIME转换为时间字符串
		NPC_F_SYS_TdTime_To_TimeStr(
			&pMsgBody->i_tStartTime,		//TD时间
			sStartTimeStr					//时间字符串
			);

		//将NPC_S_TD_DATE_TIME转换为时间字符串
		NPC_F_SYS_TdTime_To_TimeStr(
			&pMsgBody->i_tEndTime,			//TD时间
			sEndTimeStr						//时间字符串
			);

		sTempAlarmEventTable[0] = 0;
		pPos = sTempAlarmEventTable;
		for ( i=0; i<pMsgBody->i_iAlarmEventNum; i++ )
		{
			if ( i == 0 )
				sprintf(pPos, "%d", pMsgBody->i_ucAlarmEventTable[i]);
			else
				sprintf(pPos, ",%d", pMsgBody->i_ucAlarmEventTable[i]);
			pPos += strlen(pPos);
		}

		//显示信息
		NPC_CHAR sTempLogBuf[512];
		sprintf(sTempLogBuf, "[UMSP]  Query file Range: channel no: %d,  create mode: %d, alerm event: %s, start time: %s, end time: %s.", 
			pMsgBody->i_iChNo, 
			pMsgBody->i_iCreateMode, 
			sTempAlarmEventTable, 
			sStartTimeStr,  
			sEndTimeStr
			);
		NPC_F_LOG_SR_ShowInfo(sTempLogBuf);

		//--------------------------------------------------------------------
		//获取录像文件时间范围
		if ( !NPC_F_UMSP_MC_GetFileRange(
			in_pMcSrvData,				//主控服务数据
			pMsgBody->i_iChNo,			//通道号
			pMsgBody->i_iStreamNo,		//码流号
			&pMsgBody->i_tStartTime,	//开始时间
			&pMsgBody->i_tEndTime,		//结束时间
			&pMsgBody->o_iFileNum,		//文件数
			&pFileInfoDataBuf,			//文件信息数据缓冲区，为NPC_S_MON_MPS_FILE_HEAD_INFO数组
			&iFileInfoDataBufSize,		//文件信息数据缓冲区大小
			&iFileInfoDataLen			//文件信息数据长度
			) )
		{
			//写日志
			NPC_F_LOG_SR_WriteLog("[UMSP]  NPC_F_UMSP_MC_DoProEvent_EVENT_E2_NSP_FILE_QUERY NPC_F_UMSP_MC_FindFile fail.");

			in_pMsgHead->ucResult = NPC_D_MON_CSD_DPS_ERROR_CODE_QUERY_FILE_FAIL;

			bRet = FALSE;
			goto _NPC_CLEAR;
		}

		iSendDataLen = in_iMsgDataLen + iFileInfoDataLen;
		pSendDataBuf = (PNPC_BYTE)malloc(iSendDataLen);
		if ( pSendDataBuf == NULL )
		{
			//写日志
			NPC_F_LOG_SR_WriteLog("[UMSP]  NPC_F_UMSP_MC_DoProEvent_EVENT_E2_NSP_FILE_QUERY malloc fail.");

			in_pMsgHead->ucResult = NPC_D_MON_CSD_DPS_ERROR_CODE_OTHER;

			bRet = FALSE;
			goto _NPC_CLEAR;
		}

		memcpy(pSendDataBuf, in_pMsgDataBuf, in_iMsgDataLen);
		if ( pFileInfoDataBuf && iFileInfoDataLen > 0 )
			memcpy(pSendDataBuf+in_iMsgDataLen, pFileInfoDataBuf, iFileInfoDataLen);

		pSendMsgHead = (PNPC_S_DPS_MSG_HEAD)pSendDataBuf;

		pSendMsgHead->iMsgBodyLen = iSendDataLen - sizeof(NPC_S_DPS_MSG_HEAD);

		if ( pMsgBody->o_iFileNum == 0 )
			pSendMsgHead->ucResult = NPC_D_MON_CSD_DPS_ERROR_CODE_RESOURCE_NOT_EXIST;

		//发送消息给设备协议网络服务
		NPC_F_DPS_SendDevProData(
			in_pMcSrvData->hUmspProSrv,			//设备协议网络服务句柄
			pSendDataBuf,						//设备协议数据缓冲区，包括NPC_S_DPS_MSG_HEAD
			iSendDataLen						//设备协议数据长度
			);
	}

	//CLEAR AREA
	_NPC_CLEAR:
	{
		if ( !bRet )
		{
			//发送消息给设备协议网络服务
			NPC_F_DPS_SendDevProData(
				in_pMcSrvData->hUmspProSrv,			//设备协议网络服务句柄
				in_pMsgDataBuf,						//设备协议数据缓冲区，包括NPC_S_DPS_MSG_HEAD
				in_iMsgDataLen						//设备协议数据长度
				);
		}

		if ( pFileInfoDataBuf )
		{
			free(pFileInfoDataBuf);
			pFileInfoDataBuf = NULL;
		}

		if ( pSendDataBuf )
		{
			free(pSendDataBuf);
			pSendDataBuf = NULL;
		}
	}
}


//处理协议事件：打开文件
NPC_VOID	NPC_F_UMSP_MC_DoProEvent_EVENT_E2_FILE_OPEN(
	IN		PNPC_S_UMSP_MCSRV_DATA					in_pMcSrvData,				//主控服务数据
	IN		PNPC_S_DPS_MSG_HEAD						in_pMsgHead,				//消息头
	IN		PNPC_BYTE								in_pMsgDataBuf,				//消息数据缓冲区，包括NPC_S_DPS_MSG_HEAD
	IN		NPC_INT									in_iMsgDataLen				//消息数据长度
	)
{
	NPC_BOOL										bRet;
	PNPC_S_DPS_MSG_BODY_EVENT_E2_FILE_OPEN			pMsgBody;

	//INIT AREA
	{
		bRet = TRUE;
		pMsgBody = (PNPC_S_DPS_MSG_BODY_EVENT_E2_FILE_OPEN)((PNPC_BYTE)in_pMsgHead + sizeof(NPC_S_DPS_MSG_HEAD));

		in_pMsgHead->ucResult = 0;
		in_pMsgHead->ucMsgType = NPC_D_DPS_MSG_TYPE_RESPONSION;
	}

	//CODE AREA
	{
		//显示信息
		NPC_CHAR sTempLogBuf[256];
		sprintf(sTempLogBuf, "[UMSP]  Open file: file flag: %s, start data pos: %d, start time pos: %d.", 
			pMsgBody->i_sFileFlag, 
			pMsgBody->i_dwStartOffsetDataPos, 
			pMsgBody->i_dwStartOffsetTimePos
			);
		NPC_F_LOG_SR_ShowInfo(sTempLogBuf);

		//--------------------------------------------------------------------
		//打开文件
		in_pMsgHead->ucResult = NPC_F_UMSP_MC_File_OpenFile(
			in_pMcSrvData,						//主控服务数据
			pMsgBody->i_sFileFlag,				//文件标识，唯一标识该文件，打开文件时必须原样传回
			pMsgBody->i_dwStartOffsetDataPos,	//开始读取数据位置（单位字节），指原始文件的读取位置
			pMsgBody->i_dwStartOffsetTimePos,	//开始读取时间位置（单位秒），为相对时间，如果值大于文件总时长，则打开失败
			&pMsgBody->o_dwFileReadId,			//返回文件读取ID
			&pMsgBody->o_tFileInfo				//文件信息
			);
	}

	//CLEAR AREA
	{
		//发送消息给设备协议网络服务
		NPC_F_DPS_SendDevProData(
			in_pMcSrvData->hUmspProSrv,			//设备协议网络服务句柄
			in_pMsgDataBuf,						//设备协议数据缓冲区，包括NPC_S_DPS_MSG_HEAD
			in_iMsgDataLen						//设备协议数据长度
			);
	}
}

//处理协议事件：打开文件,按时间方式
NPC_VOID	NPC_F_UMSP_MC_DoProEvent_EVENT_E2_FILE_OPEN_BY_TIME(
	IN		PNPC_S_UMSP_MCSRV_DATA					in_pMcSrvData,				//主控服务数据
	IN		PNPC_S_DPS_MSG_HEAD						in_pMsgHead,				//消息头
	IN		PNPC_BYTE								in_pMsgDataBuf,				//消息数据缓冲区，包括NPC_S_DPS_MSG_HEAD
	IN		NPC_INT									in_iMsgDataLen				//消息数据长度
	)
{
	NPC_BOOL										bRet;
	PNPC_S_DPS_MSG_BODY_EVENT_E2_FILE_OPEN_BY_TIME	pMsgBody;

	NPC_CHAR										sStartTimeStr[20];
	NPC_CHAR										sEndTimeStr[20];

	NPC_CHAR										sTempAlarmEventTable[128];
	PNPC_CHAR										pPos;
	NPC_INT											i;

	NPC_S_MON_MPS_FILE_HEAD_INFO					tFileInfo = {0};

	//INIT AREA
	{
		bRet = TRUE;
		pMsgBody = (PNPC_S_DPS_MSG_BODY_EVENT_E2_FILE_OPEN_BY_TIME)((PNPC_BYTE)in_pMsgHead + sizeof(NPC_S_DPS_MSG_HEAD));

		in_pMsgHead->ucResult = 0;
		in_pMsgHead->ucMsgType = NPC_D_DPS_MSG_TYPE_RESPONSION;
	}

	//CODE AREA
	{
		//显示信息

		//将NPC_S_TD_DATE_TIME转换为时间字符串
		NPC_F_SYS_TdTime_To_TimeStr(
			&pMsgBody->i_tStartTime,		//TD时间
			sStartTimeStr					//时间字符串
			);
		
		//将NPC_S_TD_DATE_TIME转换为时间字符串
		NPC_F_SYS_TdTime_To_TimeStr(
			&pMsgBody->i_tEndTime,			//TD时间
			sEndTimeStr						//时间字符串
			);
		
		sTempAlarmEventTable[0] = 0;
		pPos = sTempAlarmEventTable;
		for ( i=0; i<pMsgBody->i_iAlarmEventNum; i++ )
		{
			if ( i == 0 )
				sprintf(pPos, "%d", pMsgBody->i_ucAlarmEventTable[i]);
			else
				sprintf(pPos, ",%d", pMsgBody->i_ucAlarmEventTable[i]);
			pPos += strlen(pPos);
		}
		
		//显示信息
		NPC_CHAR sTempLogBuf[512];
		sprintf(sTempLogBuf, "[UMSP]  open file by time: channel no: %d, file type: %d, create mode: %d, alerm event: %s, start time: %s, end time: %s.", 
			pMsgBody->i_iChNo, 
			pMsgBody->i_iFileType, 
			pMsgBody->i_iCreateMode, 
			sTempAlarmEventTable, 
			sStartTimeStr,  
			sEndTimeStr
			);
		NPC_F_LOG_SR_ShowInfo(sTempLogBuf);

		//--------------------------------------------------------------------
		//查询文件,获取第一个文件信息
		if ( !NPC_F_UMSP_MC_NspFindFirstFile(
			in_pMcSrvData,				//主控服务数据
			pMsgBody->i_iChNo,			//通道号
			pMsgBody->i_iStreamNo,		//码流号
			&pMsgBody->i_tStartTime,	//开始时间
			&pMsgBody->i_tEndTime,		//结束时间
			&tFileInfo		
			) )
		{
			//写日志
			NPC_F_LOG_SR_WriteLog("[UMSP]  NPC_F_UMSP_MC_DoProEvent_EVENT_E2_FILE_OPEN_BY_TIME NPC_F_UMSP_MC_NspFindFirstFile fail.");

			in_pMsgHead->ucResult = NPC_D_MON_CSD_DPS_ERROR_CODE_QUERY_FILE_FAIL;

			bRet = FALSE;
			goto _NPC_CLEAR;
		}
		
		//打开文件
		in_pMsgHead->ucResult = NPC_F_UMSP_MC_File_OpenFile(
			in_pMcSrvData,						//主控服务数据
			tFileInfo.sFileFlag,				//文件标识，唯一标识该文件，打开文件时必须原样传回			
			0,	//开始读取数据位置（单位字节），指原始文件的读取位置
			0,	//开始读取时间位置（单位秒），为相对时间，如果值大于文件总时长，则打开失败
			&pMsgBody->o_dwFileReadId,			//返回文件读取ID
			&pMsgBody->o_tFileInfo				//文件信息
			);
	}

	//CLEAR AREA
_NPC_CLEAR:
	{
		//发送消息给设备协议网络服务
		NPC_F_DPS_SendDevProData(
			in_pMcSrvData->hUmspProSrv,			//设备协议网络服务句柄
			in_pMsgDataBuf,						//设备协议数据缓冲区，包括NPC_S_DPS_MSG_HEAD
			in_iMsgDataLen						//设备协议数据长度
			);
	}
}

//处理协议事件：关闭文件
NPC_VOID	NPC_F_UMSP_MC_DoProEvent_EVENT_E2_FILE_CLOSE(
	IN		PNPC_S_UMSP_MCSRV_DATA					in_pMcSrvData,				//主控服务数据
	IN		PNPC_S_DPS_MSG_HEAD						in_pMsgHead,				//消息头
	IN		PNPC_BYTE								in_pMsgDataBuf,				//消息数据缓冲区，包括NPC_S_DPS_MSG_HEAD
	IN		NPC_INT									in_iMsgDataLen				//消息数据长度
	)
{
	NPC_BOOL										bRet;
	PNPC_S_DPS_MSG_BODY_EVENT_E2_FILE_CLOSE			pMsgBody;

	//INIT AREA
	{
		bRet = TRUE;
		pMsgBody = (PNPC_S_DPS_MSG_BODY_EVENT_E2_FILE_CLOSE)((PNPC_BYTE)in_pMsgHead + sizeof(NPC_S_DPS_MSG_HEAD));

		in_pMsgHead->ucResult = 0;
		in_pMsgHead->ucMsgType = NPC_D_DPS_MSG_TYPE_RESPONSION;
	}

	//CODE AREA
	{
		//显示信息
		NPC_CHAR sTempLogBuf[256];
		sprintf(sTempLogBuf, "[UMSP]  Close file: file read ID: 0x%8.8x.", 
			pMsgBody->i_dwFileReadId
			);
		NPC_F_LOG_SR_ShowInfo(sTempLogBuf);

		//--------------------------------------------------------------------
		if ( pMsgBody->i_dwFileReadId != 0 )
		{
			//关闭文件
			NPC_F_UMSP_MC_File_CloseFile(
				in_pMcSrvData,				//主控服务数据
				pMsgBody->i_dwFileReadId	//文件读取ID
				);
		}
	}

	//CLEAR AREA
	//_NPC_CLEAR:
	{
		//发送消息给设备协议网络服务
		NPC_F_DPS_SendDevProData(
			in_pMcSrvData->hUmspProSrv,			//设备协议网络服务句柄
			in_pMsgDataBuf,						//设备协议数据缓冲区，包括NPC_S_DPS_MSG_HEAD
			in_iMsgDataLen						//设备协议数据长度
			);
	}
}

//处理协议事件：读取文件
NPC_VOID	NPC_F_UMSP_MC_DoProEvent_EVENT_E2_FILE_READ(
	IN		PNPC_S_UMSP_MCSRV_DATA					in_pMcSrvData,				//主控服务数据
	IN		PNPC_S_DPS_MSG_HEAD						in_pMsgHead,				//消息头
	IN		PNPC_BYTE								in_pMsgDataBuf,				//消息数据缓冲区，包括NPC_S_DPS_MSG_HEAD
	IN		NPC_INT									in_iMsgDataLen				//消息数据长度
	)
{
	NPC_BOOL										bRet;
	PNPC_S_DPS_MSG_BODY_EVENT_E2_FILE_READ			pMsgBody;
	PNPC_S_UMSP_FILE_WORK_DATA						pFileWorkData;

	NPC_INT											iMediaType;					//媒体类型，0：视频，1：音频
	NPC_CHAR										cFrameType;					//帧类型，视频为‘I’，‘P’，‘B’等，音频该参数无效
	NPC_DWORD										dwTimestamp;				//时间戳
	NPC_DWORD										dwRecOffsetTime;			//录像偏移时间（秒），相对于文件头的时长
	PNPC_BYTE										pMediaDataBuf;				//媒体数据缓冲区（原始码流数据）
	NPC_INT											iMediaDataBufSize;			//媒体数据缓冲区大小
	NPC_INT											iMediaDataLen;				//媒体数据长度

	NPC_DWORD										dwBeginReadTime;
	NPC_INT											iVideoFrameNum;
	NPC_BOOL										bIfSendFrameData;
	NPC_INT											iEndFlag;
	NPC_INT											iIFrameNum;

	//INIT AREA
	{
		bRet = TRUE;
		pMsgBody = (PNPC_S_DPS_MSG_BODY_EVENT_E2_FILE_READ)((PNPC_BYTE)in_pMsgHead + sizeof(NPC_S_DPS_MSG_HEAD));

		in_pMsgHead->ucResult = 0;
		in_pMsgHead->ucMsgType = NPC_D_DPS_MSG_TYPE_RESPONSION;

		pMediaDataBuf = NULL;
		iMediaDataBufSize = 0;
		iMediaDataLen = 0;
	}

	//CODE AREA
	{
		//显示信息
		NPC_CHAR sTempLogBuf[256];
		sprintf(sTempLogBuf, "[UMSP]  Read file: file read ID: 0x%8.8x.", 
			pMsgBody->i_dwFileReadId
			);
		NPC_F_LOG_SR_ShowInfo(sTempLogBuf);

		//--------------------------------------------------------------------
		//查询文件工作数据（根据文件读ID）
		pFileWorkData = NPC_F_UMSP_MC_File_QueryFileWorkDataByFileReadId(
			in_pMcSrvData,				//主控服务数据
			pMsgBody->i_dwFileReadId	//文件读取ID
			);
		if ( pFileWorkData == NULL )
		{
			//显示信息
			NPC_F_LOG_SR_ShowInfo("[UMSP]  NPC_F_UMSP_MC_DoProEvent_EVENT_E2_FILE_READ i_dwFileReadId not exist.");

			in_pMsgHead->ucResult = NPC_D_MON_CSD_DPS_ERROR_CODE_ID_ERROR;

			bRet = FALSE;
			goto _NPC_CLEAR;
		}

		if ( pFileWorkData->hNavFile == NULL )
		{
			//显示信息
			NPC_F_LOG_SR_ShowInfo("[UMSP]  NPC_F_UMSP_MC_DoProEvent_EVENT_E2_FILE_READ file not open.");

			in_pMsgHead->ucResult = NPC_D_MON_CSD_DPS_ERROR_CODE_RESOURCE_NOT_OPEN;

			bRet = FALSE;
			goto _NPC_CLEAR;
		}

		//--------------------------------------------------------------------
		//读取文件
		/*
		NPC_BYTE		i_ucReqDataSize;	
		//请求数据大小，混合数据指音视频数据，该参数指帧数，视频指帧数，音频指秒数，图片数据指多少K字节
		//如果为0，则不停止，一直读取发送，直到读取文件完必
		//读取混合数据时，根据文件中的数据顺序读取，如果读取为视频帧，则发送视频帧，如果是音频数据，则发送音频，音频数据单个数据包大小最大10K，一次请求的读取数量，由视频帧为准。
		*/

		//请求读取的数据类型，0：混合数据，1：视频，2：音频，（如果是图片文件，该参数无效），值为100表示停止发送命令
		dwBeginReadTime = 0;
		iVideoFrameNum = 0;
		iIFrameNum = 0;

		while ( 1 )
		{
			//读取媒体数据（视频或音频）
			if ( !NPC_F_NAV_ReadMediaData(
				pFileWorkData->hNavFile,		//NAV文件句柄
				&iMediaType,					//媒体类型，0：视频，1：音频
				&cFrameType,					//帧类型，视频为‘I’，‘P’，‘B’等，音频该参数无效
				&dwTimestamp,					//时间戳
				&dwRecOffsetTime,				//录像偏移时间（秒），相对于文件头的时长
				&pMediaDataBuf,					//媒体数据缓冲区（原始码流数据）
				&iMediaDataBufSize,				//媒体数据缓冲区大小
				&iMediaDataLen					//媒体数据长度
				) )
			{
				//显示信息
				NPC_F_LOG_SR_ShowInfo("[UMSP]  Read file end.");

				//发送一媒体帧数据（视频或音频）
				if ( !NPC_F_UMSP_MC_File_SendMediaFrameData(
					in_pMcSrvData,				//主控服务数据
					pFileWorkData,				//文件工作数据
					in_pMsgDataBuf,				//消息数据缓冲区，包括NPC_S_DPS_MSG_HEAD
					in_iMsgDataLen,				//消息数据长度
					2,							//结束标志，0：未结束，1：本次请求结束，2：文件结束
					0,							//媒体类型，0：视频，1：音频
					'P',						//帧类型，视频为‘I’，‘P’，‘B’等，音频该参数无效
					0,							//时间戳
					0,							//录像偏移时间（秒），相对于文件头的时长
					NULL,						//媒体数据缓冲区（原始码流数据）
					0							//媒体数据长度
					) )
				{
					//显示信息
					NPC_F_LOG_SR_ShowInfo("[UMSP]  NPC_F_UMSP_MC_DoProEvent_EVENT_E2_FILE_READ NPC_F_UMSP_MC_File_SendMediaFrameData fail.");
				}

				if ( pMsgBody->i_dwFileReadId != 0 )
				{
					//关闭文件
					NPC_F_UMSP_MC_File_CloseFile(
						in_pMcSrvData,					//主控服务数据
						pFileWorkData->dwFileReadId		//文件读取ID
						);
				}
				goto _NPC_CLEAR;
			}

			if ( iMediaType == 0 )
				iVideoFrameNum++;

			if ( dwBeginReadTime == 0 )
				dwBeginReadTime = dwRecOffsetTime;

			bIfSendFrameData = FALSE;
			iEndFlag = 0;

			if ( pFileWorkData->bIfOnlySendIFrame )
			{
				//iMediaType,								//媒体类型，0：视频，1：音频
				//cFrameType,								//帧类型，视频为‘I’，‘P’，‘B’等，音频该参数无效
				//pFileWorkData->bIfOnlySendIFrame;			//是否只发I帧
				//pFileWorkData->iDiscardIFrame;			//丢弃I帧数，0：不丢I帧，1：丢弃1个I帧（即间隔1个I帧传输），2：丢弃2个I帧，如此类推

				if ( iMediaType == 0 && cFrameType == 'I' )
				{
					if ( ++iIFrameNum >= pFileWorkData->iDiscardIFrame )
					{
						bIfSendFrameData = TRUE;
						iEndFlag = 1;
					}
				}
			}
			else
			{
				switch ( pMsgBody->i_ucReqDataType )
				{
				case 0: //0：混合数据
					bIfSendFrameData = TRUE;
					break;
				case 1: //1：视频
					if ( iMediaType == 0 )
						bIfSendFrameData = TRUE;
					break;
				case 2: //2：音频
					if ( iMediaType == 1 )
						bIfSendFrameData = TRUE;
					break;
				}

				if ( pMsgBody->i_ucReqDataSize != 0 && dwRecOffsetTime - dwBeginReadTime >= pMsgBody->i_ucReqDataSize )
				{
					iEndFlag = 1;
				}
			}

			if ( bIfSendFrameData )
			{
				//发送一媒体帧数据（视频或音频）
				if ( !NPC_F_UMSP_MC_File_SendMediaFrameData(
					in_pMcSrvData,				//主控服务数据
					pFileWorkData,				//文件工作数据
					in_pMsgDataBuf,				//消息数据缓冲区，包括NPC_S_DPS_MSG_HEAD
					in_iMsgDataLen,				//消息数据长度
					iEndFlag,					//结束标志，0：未结束，1：本次请求结束，2：文件结束
					iMediaType,					//媒体类型，0：视频，1：音频
					cFrameType,					//帧类型，视频为‘I’，‘P’，‘B’等，音频该参数无效
					dwTimestamp,				//时间戳
					dwRecOffsetTime,			//录像偏移时间（秒），相对于文件头的时长
					pMediaDataBuf,				//媒体数据缓冲区（原始码流数据）
					iMediaDataLen				//媒体数据长度
					) )
				{
					break;
				}
			}

			if ( iEndFlag == 1 )
			{
				break;
			}
		}
	}

	//CLEAR AREA
	_NPC_CLEAR:
	{
		if ( !bRet )
		{
			//发送消息给设备协议网络服务
			NPC_F_DPS_SendDevProData(
				in_pMcSrvData->hUmspProSrv,			//设备协议网络服务句柄
				in_pMsgDataBuf,						//设备协议数据缓冲区，包括NPC_S_DPS_MSG_HEAD
				in_iMsgDataLen						//设备协议数据长度
				);
		}

		//释放缓冲区
		NPC_F_NAV_ReleaseBuf(
			&pMediaDataBuf		//数据缓冲区
			);
	}
}

//处理协议事件：定位文件
NPC_VOID	NPC_F_UMSP_MC_DoProEvent_EVENT_E2_FILE_SEEK(
	IN		PNPC_S_UMSP_MCSRV_DATA					in_pMcSrvData,				//主控服务数据
	IN		PNPC_S_DPS_MSG_HEAD						in_pMsgHead,				//消息头
	IN		PNPC_BYTE								in_pMsgDataBuf,				//消息数据缓冲区，包括NPC_S_DPS_MSG_HEAD
	IN		NPC_INT									in_iMsgDataLen				//消息数据长度
	)
{
	NPC_BOOL										bRet;
	PNPC_S_DPS_MSG_BODY_EVENT_E2_FILE_SEEK			pMsgBody;

	//INIT AREA
	{
		bRet = TRUE;
		pMsgBody = (PNPC_S_DPS_MSG_BODY_EVENT_E2_FILE_SEEK)((PNPC_BYTE)in_pMsgHead + sizeof(NPC_S_DPS_MSG_HEAD));

		in_pMsgHead->ucResult = 0;
		in_pMsgHead->ucMsgType = NPC_D_DPS_MSG_TYPE_RESPONSION;
	}

	//CODE AREA
	{
		//显示信息
		NPC_CHAR sTempLogBuf[256];
		sprintf(sTempLogBuf, "[UMSP]  Seek file: file read ID: 0x%8.8x.", 
			pMsgBody->i_dwFileReadId
			);
		NPC_F_LOG_SR_ShowInfo(sTempLogBuf);

		//--------------------------------------------------------------------
		//定位文件
		in_pMsgHead->ucResult = NPC_F_UMSP_MC_File_SeekFile(
			in_pMcSrvData,					//主控服务数据
			pMsgBody->i_dwFileReadId,		//文件读取ID
			pMsgBody->i_dwOffsetTimePos		//读取时间位置（单位秒），为相对时间
			);
		if ( in_pMsgHead->ucResult != 0 )
		{
			//显示信息
			NPC_F_LOG_SR_ShowInfo("[UMSP]  Seek file fail.");
		}
	}

	//CLEAR AREA
	{
		//发送消息给设备协议网络服务
		NPC_F_DPS_SendDevProData(
			in_pMcSrvData->hUmspProSrv,			//设备协议网络服务句柄
			in_pMsgDataBuf,						//设备协议数据缓冲区，包括NPC_S_DPS_MSG_HEAD
			in_iMsgDataLen						//设备协议数据长度
			);
	}
}


//处理协议事件：定位文件
NPC_VOID	NPC_F_UMSP_MC_DoProEvent_EVENT_E2_FILE_SEEK_BY_TIME(
	IN		PNPC_S_UMSP_MCSRV_DATA					in_pMcSrvData,				//主控服务数据
	IN		PNPC_S_DPS_MSG_HEAD						in_pMsgHead,				//消息头
	IN		PNPC_BYTE								in_pMsgDataBuf,				//消息数据缓冲区，包括NPC_S_DPS_MSG_HEAD
	IN		NPC_INT									in_iMsgDataLen				//消息数据长度
	)
{
	NPC_BOOL												bRet;
	PNPC_S_DPS_MSG_BODY_EVENT_E2_FILE_SEEK_BY_TIME			pMsgBody;

	//INIT AREA
	{
		bRet = TRUE;
		pMsgBody = (PNPC_S_DPS_MSG_BODY_EVENT_E2_FILE_SEEK_BY_TIME)((PNPC_BYTE)in_pMsgHead + sizeof(NPC_S_DPS_MSG_HEAD));

		in_pMsgHead->ucResult = 0;
		in_pMsgHead->ucMsgType = NPC_D_DPS_MSG_TYPE_RESPONSION;
	}

	//CODE AREA
	{
		//显示信息
		NPC_CHAR sTempLogBuf[256];
		sprintf(sTempLogBuf, "[UMSP]  Seek file by time: file read ID: 0x%8.8x.", 
			pMsgBody->i_dwFileReadId
			);
		NPC_F_LOG_SR_ShowInfo(sTempLogBuf);

		//--------------------------------------------------------------------
		//定位文件
		in_pMsgHead->ucResult = NPC_F_UMSP_MC_File_SeekFile(
			in_pMcSrvData,					//主控服务数据
			pMsgBody->i_dwFileReadId,		//文件读取ID
			0								//读取时间位置（单位秒），为相对时间
			);
		if ( in_pMsgHead->ucResult != 0 )
		{
			//显示信息
			NPC_F_LOG_SR_ShowInfo("[UMSP]  Seek file fail.");
		}
	}

	//CLEAR AREA
	{
		//发送消息给设备协议网络服务
		NPC_F_DPS_SendDevProData(
			in_pMcSrvData->hUmspProSrv,			//设备协议网络服务句柄
			in_pMsgDataBuf,						//设备协议数据缓冲区，包括NPC_S_DPS_MSG_HEAD
			in_iMsgDataLen						//设备协议数据长度
			);
	}
}

//处理协议事件：查询视频参数
NPC_VOID	NPC_F_UMSP_MC_DoProEvent_EVENT_E2_FILE_QVIDEO(
	IN		PNPC_S_UMSP_MCSRV_DATA					in_pMcSrvData,				//主控服务数据
	IN		PNPC_S_DPS_MSG_HEAD						in_pMsgHead,				//消息头
	IN		PNPC_BYTE								in_pMsgDataBuf,				//消息数据缓冲区，包括NPC_S_DPS_MSG_HEAD
	IN		NPC_INT									in_iMsgDataLen				//消息数据长度
	)
{
	NPC_BOOL										bRet;
	PNPC_S_DPS_MSG_BODY_EVENT_E2_FILE_QVIDEO		pMsgBody;

	//INIT AREA
	{
		bRet = TRUE;
		pMsgBody = (PNPC_S_DPS_MSG_BODY_EVENT_E2_FILE_QVIDEO)((PNPC_BYTE)in_pMsgHead + sizeof(NPC_S_DPS_MSG_HEAD));

		in_pMsgHead->ucResult = 0;
		in_pMsgHead->ucMsgType = NPC_D_DPS_MSG_TYPE_RESPONSION;
	}

	//CODE AREA
	{
		//显示信息
		NPC_CHAR sTempLogBuf[256];
		sprintf(sTempLogBuf, "[UMSP]  Query video file param: file read ID: 0x%8.8x.", 
			pMsgBody->i_dwFileReadId
			);
		NPC_F_LOG_SR_ShowInfo(sTempLogBuf);

		//获取文件的视频参数
		if ( !NPC_F_UMSP_MC_File_GetFileVideoParam(
			in_pMcSrvData,					//主控服务数据
			pMsgBody->i_dwFileReadId,		//文件读取ID
			&pMsgBody->o_iVideoFrameRate,	//帧率
			&pMsgBody->o_iVideoGop			//GOP
			) )
		{
			in_pMsgHead->ucResult = NPC_D_MON_CSD_DPS_ERROR_CODE_ID_ERROR;

			//显示信息
			NPC_F_LOG_SR_ShowInfo("[UMSP]  Query video file param fail.");
		}
	}

	//CLEAR AREA
	{
		//发送消息给设备协议网络服务
		NPC_F_DPS_SendDevProData(
			in_pMcSrvData->hUmspProSrv,			//设备协议网络服务句柄
			in_pMsgDataBuf,						//设备协议数据缓冲区，包括NPC_S_DPS_MSG_HEAD
			in_iMsgDataLen						//设备协议数据长度
			);
	}
}

//处理协议事件：请求I帧数据
NPC_VOID	NPC_F_UMSP_MC_DoProEvent_EVENT_E2_FILE_ONLY_IFRAME(
	IN		PNPC_S_UMSP_MCSRV_DATA					in_pMcSrvData,				//主控服务数据
	IN		PNPC_S_DPS_MSG_HEAD						in_pMsgHead,				//消息头
	IN		PNPC_BYTE								in_pMsgDataBuf,				//消息数据缓冲区，包括NPC_S_DPS_MSG_HEAD
	IN		NPC_INT									in_iMsgDataLen				//消息数据长度
	)
{
	NPC_BOOL										bRet;
	PNPC_S_DPS_MSG_BODY_EVENT_E2_FILE_ONLY_IFRAME	pMsgBody;
	PNPC_S_UMSP_FILE_WORK_DATA						pFileWorkData;

	//INIT AREA
	{
		bRet = TRUE;
		pMsgBody = (PNPC_S_DPS_MSG_BODY_EVENT_E2_FILE_ONLY_IFRAME)((PNPC_BYTE)in_pMsgHead + sizeof(NPC_S_DPS_MSG_HEAD));

		in_pMsgHead->ucResult = 0;
		in_pMsgHead->ucMsgType = NPC_D_DPS_MSG_TYPE_RESPONSION;
	}

	//CODE AREA
	{
		//显示信息
		NPC_CHAR sTempLogBuf[256];
		sprintf(sTempLogBuf, "[UMSP]  Request file only i frame: file read ID: 0x%8.8x.", 
			pMsgBody->i_dwFileReadId
			);
		NPC_F_LOG_SR_ShowInfo(sTempLogBuf);

		//查询文件工作数据（根据文件读ID）
		pFileWorkData = NPC_F_UMSP_MC_File_QueryFileWorkDataByFileReadId(
			in_pMcSrvData,				//主控服务数据
			pMsgBody->i_dwFileReadId	//文件读取ID
			);
		if ( pFileWorkData == NULL )
		{
			in_pMsgHead->ucResult = NPC_D_MON_CSD_DPS_ERROR_CODE_ID_ERROR;

			//显示信息
			NPC_F_LOG_SR_ShowInfo("[UMSP]  Query file work data fail.");
			goto _NPC_CLEAR;
		}

		pFileWorkData->bIfOnlySendIFrame = TRUE;							//是否只发I帧
		pFileWorkData->iDiscardIFrame = pMsgBody->i_iDiscardIFrame;			//丢弃I帧数，0：不丢I帧，1：丢弃1个I帧（即间隔1个I帧传输），2：丢弃2个I帧，如此类推

		//------------------------------------------------------------------------------
		//获取文件的视频参数
		if ( !NPC_F_UMSP_MC_File_GetFileVideoParam(
			in_pMcSrvData,						//主控服务数据
			pMsgBody->i_dwFileReadId,			//文件读取ID
			&pMsgBody->o_iVideoFrameRate,		//帧率
			&pMsgBody->o_iVideoGop				//GOP
			) )
		{
			in_pMsgHead->ucResult = NPC_D_MON_CSD_DPS_ERROR_CODE_ID_ERROR;

			//显示信息
			NPC_F_LOG_SR_ShowInfo("[UMSP]  Get file video param fail.");
			goto _NPC_CLEAR;
		}
	}

	//CLEAR AREA
	_NPC_CLEAR:
	{
		//发送消息给设备协议网络服务
		NPC_F_DPS_SendDevProData(
			in_pMcSrvData->hUmspProSrv,			//设备协议网络服务句柄
			in_pMsgDataBuf,						//设备协议数据缓冲区，包括NPC_S_DPS_MSG_HEAD
			in_iMsgDataLen						//设备协议数据长度
			);
	}
}

//处理协议事件：请求恢复全帧数据
NPC_VOID	NPC_F_UMSP_MC_DoProEvent_EVENT_E2_FILE_RECOVER_ALLFRAME(
	IN		PNPC_S_UMSP_MCSRV_DATA					in_pMcSrvData,				//主控服务数据
	IN		PNPC_S_DPS_MSG_HEAD						in_pMsgHead,				//消息头
	IN		PNPC_BYTE								in_pMsgDataBuf,				//消息数据缓冲区，包括NPC_S_DPS_MSG_HEAD
	IN		NPC_INT									in_iMsgDataLen				//消息数据长度
	)
{
	NPC_BOOL											bRet;
	PNPC_S_DPS_MSG_BODY_EVENT_E2_FILE_RECOVER_ALLFRAME	pMsgBody;
	PNPC_S_UMSP_FILE_WORK_DATA							pFileWorkData;

	//INIT AREA
	{
		bRet = TRUE;
		pMsgBody = (PNPC_S_DPS_MSG_BODY_EVENT_E2_FILE_RECOVER_ALLFRAME)((PNPC_BYTE)in_pMsgHead + sizeof(NPC_S_DPS_MSG_HEAD));

		in_pMsgHead->ucResult = 0;
		in_pMsgHead->ucMsgType = NPC_D_DPS_MSG_TYPE_RESPONSION;
	}

	//CODE AREA
	{
		//显示信息
		NPC_CHAR sTempLogBuf[256];
		sprintf(sTempLogBuf, "[UMSP]  Request recover all frame: file read ID: 0x%8.8x.", 
			pMsgBody->i_dwFileReadId
			);
		NPC_F_LOG_SR_ShowInfo(sTempLogBuf);

		//查询文件工作数据（根据文件读ID）
		pFileWorkData = NPC_F_UMSP_MC_File_QueryFileWorkDataByFileReadId(
			in_pMcSrvData,				//主控服务数据
			pMsgBody->i_dwFileReadId	//文件读取ID
			);
		if ( pFileWorkData == NULL )
		{
			in_pMsgHead->ucResult = NPC_D_MON_CSD_DPS_ERROR_CODE_ID_ERROR;

			//显示信息
			NPC_F_LOG_SR_ShowInfo("[UMSP]  Query file work data fail.");
			goto _NPC_CLEAR;
		}

		pFileWorkData->bIfOnlySendIFrame = FALSE;		//是否只发I帧
		pFileWorkData->iDiscardIFrame = 0;				//丢弃I帧数，0：不丢I帧，1：丢弃1个I帧（即间隔1个I帧传输），2：丢弃2个I帧，如此类推
	}

	//CLEAR AREA
	_NPC_CLEAR:
	{
		//发送消息给设备协议网络服务
		NPC_F_DPS_SendDevProData(
			in_pMcSrvData->hUmspProSrv,			//设备协议网络服务句柄
			in_pMsgDataBuf,						//设备协议数据缓冲区，包括NPC_S_DPS_MSG_HEAD
			in_iMsgDataLen						//设备协议数据长度
			);
	}
}


//处理协议事件：开始语音对讲
NPC_VOID	NPC_F_UMSP_MC_DoProEvent_EVENT_E2_TALK_START(
	IN		PNPC_S_UMSP_MCSRV_DATA					in_pMcSrvData,				//主控服务数据
	IN		PNPC_S_DPS_MSG_HEAD						in_pMsgHead,				//消息头
	IN		PNPC_BYTE								in_pMsgDataBuf,				//消息数据缓冲区，包括NPC_S_DPS_MSG_HEAD
	IN		NPC_INT									in_iMsgDataLen				//消息数据长度
	)
{
	NPC_BOOL										bRet;
	PNPC_S_DPS_MSG_BODY_EVENT_E2_TALK_START			pMsgBody;
	PNPC_S_UMSP_CAMERA_DATA							pCameraData;
	NPC_BOOL										bIfLock;
	NPC_INT											iChNo = 0;

	//INIT AREA
	{
		bRet = TRUE;
		pMsgBody = (PNPC_S_DPS_MSG_BODY_EVENT_E2_TALK_START)((PNPC_BYTE)in_pMsgHead + sizeof(NPC_S_DPS_MSG_HEAD));

		in_pMsgHead->ucResult = 0;
		in_pMsgHead->ucMsgType = NPC_D_DPS_MSG_TYPE_RESPONSION;

		pCameraData = NULL;
		bIfLock = FALSE;
	}

	//CODE AREA
	{
		//显示信息
		NPC_CHAR sTempLogBuf[256];
		sprintf(sTempLogBuf, "[UMSP]  Start talk: channel no: %d.", 
			pMsgBody->i_iTalkChNo
			);
		NPC_F_LOG_SR_ShowInfo(sTempLogBuf);

		//--------------------------------------------------------------------
		if ( pMsgBody->i_iTalkChNo < 0 || pMsgBody->i_iTalkChNo >= NPC_D_DPS_MAX_CHANNEL_NUM && pMsgBody->i_iTalkChNo != NPC_D_DPS_ZERO_CHANNEL_VALUE)
		{
			//写日志
			NPC_F_LOG_SR_WriteLog("[UMSP]  NPC_F_UMSP_MC_DoProEvent_EVENT_E2_TALK_START i_iChNo error.");

			in_pMsgHead->ucResult = NPC_D_MON_CSD_DPS_ERROR_CODE_CHANNEL_NO_ERROR;
			goto _NPC_CLEAR;
		}


		if (pMsgBody->i_iTalkChNo == NPC_D_DPS_ZERO_CHANNEL_VALUE)
		{
			iChNo = NPC_D_DPS_MAX_CHANNEL_NUM;
		}
		else
		{
			iChNo = pMsgBody->i_iTalkChNo;
		}

		pCameraData = &(in_pMcSrvData->tCameraTable[iChNo]);

		//加锁
		NPC_F_THREAD_Mutex_Lock(
			pCameraData->hWorkLock		//互斥锁句柄
			);
		bIfLock = TRUE;

		pMsgBody->o_iDevAudioCodecId = NPC_D_MON_CSD_CODEC_ID_AUDIO_PCMA;	//设备端音频编码ID，参见NPC_D_PVM_DP_UMSP_CODEC_ID_AUDIO_*定义
		pMsgBody->o_iDevAudioBitRate = 64;									//设备端音频码率
		pMsgBody->o_iDevAudioSampleRate = 8000;								//设备端音频采样率
		pMsgBody->o_iDevAudioChNum = 1;										//设备端音频声道数

		if ( pCameraData->bTalkIfOpen )
		{
			goto _NPC_CLEAR;
		}
		if (pMsgBody->i_iTalkChNo != NPC_D_DPS_ZERO_CHANNEL_VALUE)
		{
			//开始对讲
			if ( !NPC_F_FILECAM_SDK_StartTalk(
				in_pMcSrvData->hFileCamera,			//文件摄像机句柄，由NPC_F_FILECAM_SDK_ConnectFilecam函数创建
				pMsgBody->i_iTalkChNo				//通道号
				) )
			{
				//写日志
				NPC_F_LOG_SR_WriteLog("[UMSP]  NPC_F_UMSP_MC_DoProEvent_EVENT_E2_TALK_START NPC_F_FILECAM_SDK_StartTalk fail.");
				
				in_pMsgHead->ucResult = NPC_D_MON_CSD_DPS_ERROR_CODE_START_TALK_FAIL;
				goto _NPC_CLEAR;
			}
		}
		else
		{
			//开始对讲
			if ( !NPC_F_FILECAM_SDK_StartTalk(
				in_pMcSrvData->hFileCamera,			//文件摄像机句柄，由NPC_F_FILECAM_SDK_ConnectFilecam函数创建
				0				//通道号
				) )
			{
				//写日志
				NPC_F_LOG_SR_WriteLog("[UMSP]  NPC_F_UMSP_MC_DoProEvent_EVENT_E2_TALK_START NPC_F_FILECAM_SDK_StartTalk fail.");
				
				in_pMsgHead->ucResult = NPC_D_MON_CSD_DPS_ERROR_CODE_START_TALK_FAIL;
				goto _NPC_CLEAR;
			}
		}
		
		pCameraData->bTalkIfOpen = TRUE;
	}

	//CLEAR AREA
	_NPC_CLEAR:
	{
		if ( pCameraData && bIfLock )
		{
			//解锁
			NPC_F_THREAD_Mutex_Unlock(
				pCameraData->hWorkLock			//互斥锁句柄
				);
		}

		//发送消息给设备协议网络服务
		NPC_F_DPS_SendDevProData(
			in_pMcSrvData->hUmspProSrv,			//设备协议网络服务句柄
			in_pMsgDataBuf,						//设备协议数据缓冲区，包括NPC_S_DPS_MSG_HEAD
			in_iMsgDataLen						//设备协议数据长度
			);
	}
}

//处理协议事件：停止语音对讲
NPC_VOID	NPC_F_UMSP_MC_DoProEvent_EVENT_E2_TALK_STOP(
	IN		PNPC_S_UMSP_MCSRV_DATA					in_pMcSrvData,				//主控服务数据
	IN		PNPC_S_DPS_MSG_HEAD						in_pMsgHead,				//消息头
	IN		PNPC_BYTE								in_pMsgDataBuf,				//消息数据缓冲区，包括NPC_S_DPS_MSG_HEAD
	IN		NPC_INT									in_iMsgDataLen				//消息数据长度
	)
{
	NPC_BOOL										bRet;
	PNPC_S_DPS_MSG_BODY_EVENT_E2_TALK_STOP			pMsgBody;
	PNPC_S_UMSP_CAMERA_DATA							pCameraData;
	NPC_BOOL										bIfLock;

	//INIT AREA
	{
		bRet = TRUE;
		pMsgBody = (PNPC_S_DPS_MSG_BODY_EVENT_E2_TALK_STOP)((PNPC_BYTE)in_pMsgHead + sizeof(NPC_S_DPS_MSG_HEAD));

		in_pMsgHead->ucResult = 0;
		in_pMsgHead->ucMsgType = NPC_D_DPS_MSG_TYPE_RESPONSION;

		pCameraData = NULL;
		bIfLock = FALSE;
	}

	//CODE AREA
	{
		//显示信息
		NPC_CHAR sTempLogBuf[256];
		sprintf(sTempLogBuf, "[UMSP]  Stop talk: channel no: %d.", 
			pMsgBody->i_iTalkChNo
			);
		NPC_F_LOG_SR_ShowInfo(sTempLogBuf);

		//--------------------------------------------------------------------
		if ( pMsgBody->i_iTalkChNo < 0 || pMsgBody->i_iTalkChNo >= NPC_D_DPS_MAX_CHANNEL_NUM )
		{
			//写日志
			NPC_F_LOG_SR_WriteLog("[UMSP]  NPC_F_UMSP_MC_DoProEvent_EVENT_E2_TALK_STOP i_iChNo error.");

			in_pMsgHead->ucResult = NPC_D_MON_CSD_DPS_ERROR_CODE_CHANNEL_NO_ERROR;
			goto _NPC_CLEAR;
		}

		pCameraData = &(in_pMcSrvData->tCameraTable[pMsgBody->i_iTalkChNo]);

		//加锁
		NPC_F_THREAD_Mutex_Lock(
			pCameraData->hWorkLock		//互斥锁句柄
			);
		bIfLock = TRUE;

		if ( !pCameraData->bTalkIfOpen )
		{
			goto _NPC_CLEAR;
		}

		//停止对讲
		NPC_F_FILECAM_SDK_StopTalk(
			in_pMcSrvData->hFileCamera,			//文件摄像机句柄，由NPC_F_FILECAM_SDK_ConnectFilecam函数创建
			pMsgBody->i_iTalkChNo				//通道号
			);
		pCameraData->bTalkIfOpen = FALSE;
	}

	//CLEAR AREA
	_NPC_CLEAR:
	{
		if ( pCameraData && bIfLock )
		{
			//解锁
			NPC_F_THREAD_Mutex_Unlock(
				pCameraData->hWorkLock			//互斥锁句柄
				);
		}

		//发送消息给设备协议网络服务
		NPC_F_DPS_SendDevProData(
			in_pMcSrvData->hUmspProSrv,			//设备协议网络服务句柄
			in_pMsgDataBuf,						//设备协议数据缓冲区，包括NPC_S_DPS_MSG_HEAD
			in_iMsgDataLen						//设备协议数据长度
			);
	}
}

//处理协议事件：布防
NPC_VOID	NPC_F_UMSP_MC_DoProEvent_EVENT_E2_GUARD(
	IN		PNPC_S_UMSP_MCSRV_DATA					in_pMcSrvData,				//主控服务数据
	IN		PNPC_S_DPS_MSG_HEAD						in_pMsgHead,				//消息头
	IN		PNPC_BYTE								in_pMsgDataBuf,				//消息数据缓冲区，包括NPC_S_DPS_MSG_HEAD
	IN		NPC_INT									in_iMsgDataLen				//消息数据长度
	)
{
	//INIT AREA
	{
		in_pMsgHead->ucResult = 0;
		in_pMsgHead->ucMsgType = NPC_D_DPS_MSG_TYPE_RESPONSION;
	}

	//CODE AREA
	{
		//显示信息
		//NPC_F_LOG_SR_ShowInfo((PNPC_CHAR)"布防");

		//in_pMcSrvData->bAlarmIfOpen = TRUE;

		//不处理，直接返回应答
	}

	//CLEAR AREA
	{
		//发送消息给设备协议网络服务
		NPC_F_DPS_SendDevProData(
			in_pMcSrvData->hUmspProSrv,			//设备协议网络服务句柄
			in_pMsgDataBuf,						//设备协议数据缓冲区，包括NPC_S_DPS_MSG_HEAD
			in_iMsgDataLen						//设备协议数据长度
			);
	}
}

//处理协议事件：撤防
NPC_VOID	NPC_F_UMSP_MC_DoProEvent_EVENT_E2_UNGUARD(
	IN		PNPC_S_UMSP_MCSRV_DATA					in_pMcSrvData,				//主控服务数据
	IN		PNPC_S_DPS_MSG_HEAD						in_pMsgHead,				//消息头
	IN		PNPC_BYTE								in_pMsgDataBuf,				//消息数据缓冲区，包括NPC_S_DPS_MSG_HEAD
	IN		NPC_INT									in_iMsgDataLen				//消息数据长度
	)
{
	//INIT AREA
	{
		in_pMsgHead->ucResult = 0;
		in_pMsgHead->ucMsgType = NPC_D_DPS_MSG_TYPE_RESPONSION;
	}

	//CODE AREA
	{
		//显示信息
		//NPC_F_LOG_SR_ShowInfo((PNPC_CHAR)"撤防");

		//in_pMcSrvData->bAlarmIfOpen = FALSE;

		//不处理，直接返回应答
	}

	//CLEAR AREA
	{
		//发送消息给设备协议网络服务
		NPC_F_DPS_SendDevProData(
			in_pMcSrvData->hUmspProSrv,			//设备协议网络服务句柄
			in_pMsgDataBuf,						//设备协议数据缓冲区，包括NPC_S_DPS_MSG_HEAD
			in_iMsgDataLen						//设备协议数据长度
			);
	}
}


//处理协议事件：修改用户密码
NPC_VOID	NPC_F_UMSP_MC_DoProEvent_EVENT_E3_MODIFY_USER_PWD(
	IN		PNPC_S_UMSP_MCSRV_DATA					in_pMcSrvData,				//主控服务数据
	IN		PNPC_S_DPS_MSG_HEAD						in_pMsgHead,				//消息头
	IN		PNPC_BYTE								in_pMsgDataBuf,				//消息数据缓冲区，包括NPC_S_DPS_MSG_HEAD
	IN		NPC_INT									in_iMsgDataLen				//消息数据长度
	)
{
	NPC_BOOL										bRet;
	PNPC_S_DPS_MSG_BODY_EVENT_E3_MODIFY_USER_PWD	pMsgBody;

	//INIT AREA
	{
		bRet = TRUE;
		pMsgBody = (PNPC_S_DPS_MSG_BODY_EVENT_E3_MODIFY_USER_PWD)((PNPC_BYTE)in_pMsgHead + sizeof(NPC_S_DPS_MSG_HEAD));

		in_pMsgHead->ucResult = 0;
		in_pMsgHead->ucMsgType = NPC_D_DPS_MSG_TYPE_RESPONSION;
	}

	//CODE AREA
	{
		//显示信息
		NPC_CHAR sTempLogBuf[256];
		sprintf(sTempLogBuf, "[UMSP]  Modify user password: user name: %s.", 
			pMsgBody->i_sUserName
			);
		NPC_F_LOG_SR_ShowInfo(sTempLogBuf);

		//--------------------------------------------------------------------
		//修改用户密码，返回0表示成功，1：用户名错误，2：原密码错误，9：其它失败
		switch ( NPC_F_UMSP_MC_ModifyUserPwd(
			in_pMcSrvData,				//主控服务数据
			pMsgBody->i_sUserName,		//用户名
			pMsgBody->i_sOldPwd,		//原密码
			pMsgBody->i_sNewPwd			//新密码
			) )
		{
		case 0: //0：成功
			break;
		case 1: //1：用户名错误
			{
				//显示信息
				NPC_F_LOG_SR_ShowInfo("[UMSP]  Modify user password fail, cause: user name error.");

				in_pMsgHead->ucResult = NPC_D_MON_CSD_DPS_ERROR_CODE_USERNAME_ERROR;
			}
			break;
		case 2: //2：原密码错误
			{
				//显示信息
				NPC_F_LOG_SR_ShowInfo("[UMSP]  Modify user password fail, cause: old password error.");

				in_pMsgHead->ucResult = NPC_D_MON_CSD_DPS_ERROR_CODE_PASSWORD_ERROR;
			}
			break;
		case 9:
		default:
			{
				//显示信息
				NPC_F_LOG_SR_ShowInfo("[UMSP]  Modify user password fail, cause: other error.");

				in_pMsgHead->ucResult = NPC_D_MON_CSD_DPS_ERROR_CODE_OTHER;
			}
			break;
		}
	}

	//CLEAR AREA
	{
		//发送消息给设备协议网络服务
		NPC_F_DPS_SendDevProData(
			in_pMcSrvData->hUmspProSrv,			//设备协议网络服务句柄
			in_pMsgDataBuf,						//设备协议数据缓冲区，包括NPC_S_DPS_MSG_HEAD
			in_iMsgDataLen						//设备协议数据长度
			);
	}
}

//处理协议事件：获取配置参数
NPC_VOID	NPC_F_UMSP_MC_DoProEvent_EVENT_E3_GET_CONFIG(
	IN		PNPC_S_UMSP_MCSRV_DATA					in_pMcSrvData,				//主控服务数据
	IN		PNPC_S_DPS_MSG_HEAD						in_pMsgHead,				//消息头
	IN		PNPC_BYTE								in_pMsgDataBuf,				//消息数据缓冲区，包括NPC_S_DPS_MSG_HEAD
	IN		NPC_INT									in_iMsgDataLen				//消息数据长度
	)
{
	NPC_BOOL										bRet;
	PNPC_S_DPS_MSG_BODY_EVENT_E3_GET_CONFIG			pMsgBody;

	//INIT AREA
	{
		bRet = TRUE;
		pMsgBody = (PNPC_S_DPS_MSG_BODY_EVENT_E3_GET_CONFIG)((PNPC_BYTE)in_pMsgHead + sizeof(NPC_S_DPS_MSG_HEAD));

		in_pMsgHead->ucResult = 0;
		in_pMsgHead->ucMsgType = NPC_D_DPS_MSG_TYPE_RESPONSION;
	}

	//CODE AREA
	{
		switch ( pMsgBody->i_iCfgFuncId )
		{
		case NPC_D_PVM_CFG_FUNCID_DEV_NAME:					//设备名称
			//显示信息
			NPC_F_LOG_SR_ShowInfo("[UMSP]  Get config: dev name.");

			in_pMsgHead->ucResult = NPC_D_MON_CSD_DPS_ERROR_CODE_NOT_SUPPORT_OP;
			bRet = FALSE;
			break;
		case NPC_D_PVM_CFG_FUNCID_DEV_IP:					//设备有线IP地址
			{
				struct _PacketData{
					NPC_S_DPS_MSG_HEAD						tMsgHead;
					NPC_S_DPS_MSG_BODY_EVENT_E3_GET_CONFIG	tMsgBody;
					NPC_S_PVM_IMD_CFG_DEV_IP				tCfgBody;
				} tPacketData = {0};

				//显示信息
				NPC_F_LOG_SR_ShowInfo("[UMSP]  Get config: dev ip.");

				memcpy(&tPacketData, in_pMsgDataBuf, 
					sizeof(NPC_S_DPS_MSG_HEAD) + 
					sizeof(NPC_S_DPS_MSG_BODY_EVENT_E3_GET_CONFIG));

				tPacketData.tMsgHead.iMsgBodyLen = 
					sizeof(NPC_S_DPS_MSG_BODY_EVENT_E3_GET_CONFIG) + 
					sizeof(NPC_S_PVM_IMD_CFG_DEV_IP);

				tPacketData.tMsgBody.io_iCfgDataLen = sizeof(NPC_S_PVM_IMD_CFG_DEV_IP);

				if ( in_pMcSrvData->tDevIp.sNetcardName[0] == 0 )
				{
					strcpy(in_pMcSrvData->tDevIp.sNetcardName, "eth0");			//网卡名称
					strcpy(in_pMcSrvData->tDevIp.sMac, "28-D2-44-02-D3-17");	//网卡MAC地址，格式为XX-XX-

					in_pMcSrvData->tDevIp.bDhcpEnable = FALSE;					//DHCP使能
					strcpy(in_pMcSrvData->tDevIp.sIpaddr, "192.168.1.160");		//IP地址
					strcpy(in_pMcSrvData->tDevIp.sNetmask, "255.255.255.0");	//子网掩码
					strcpy(in_pMcSrvData->tDevIp.sGateway, "192.168.1.1");		//网关地址
				}
				tPacketData.tCfgBody = in_pMcSrvData->tDevIp;

				//发送消息给设备协议网络服务
				NPC_F_DPS_SendDevProData(
					in_pMcSrvData->hUmspProSrv,			//设备协议网络服务句柄
					(PNPC_BYTE)&tPacketData,			//设备协议数据缓冲区，包括NPC_S_DPS_MSG_HEAD
					sizeof(tPacketData)					//设备协议数据长度
					);
			}
			break;
		case NPC_D_PVM_CFG_FUNCID_DEV_WIFI:					//设备WIFI信息
			{
				struct _PacketData{
					NPC_S_DPS_MSG_HEAD						tMsgHead;
					NPC_S_DPS_MSG_BODY_EVENT_E3_GET_CONFIG	tMsgBody;
					NPC_S_PVM_IMD_CFG_DEV_WIFI				tCfgBody;
				} tPacketData = {0};

				//显示信息
				NPC_F_LOG_SR_ShowInfo("[UMSP]  Get config: dev wifi.");

				memcpy(&tPacketData, in_pMsgDataBuf, 
					sizeof(NPC_S_DPS_MSG_HEAD) + 
					sizeof(NPC_S_DPS_MSG_BODY_EVENT_E3_GET_CONFIG));

				tPacketData.tMsgHead.iMsgBodyLen = 
					sizeof(NPC_S_DPS_MSG_BODY_EVENT_E3_GET_CONFIG) + 
					sizeof(NPC_S_PVM_IMD_CFG_DEV_WIFI);

				tPacketData.tMsgBody.io_iCfgDataLen = sizeof(NPC_S_PVM_IMD_CFG_DEV_WIFI);

				if ( in_pMcSrvData->tDevWifi.sNetcardName[0] == 0 )
				{
					strcpy(in_pMcSrvData->tDevWifi.sNetcardName, "eth2");			//网卡名称（可选）
					strcpy(in_pMcSrvData->tDevWifi.sMac, "28-D2-44-02-D3-17");		//网卡MAC地址（可选，为空，则为默认WIFI网卡），格式为XX-XX-

					in_pMcSrvData->tDevWifi.bEnable = TRUE;							//是否启用（必填）

					strcpy(in_pMcSrvData->tDevWifi.sWifiSSID, "cchdlink");			//WIFI SSID（必填）
					strcpy(in_pMcSrvData->tDevWifi.sWifiPwd, "123445676789");		//WIFI密码（必填）

					in_pMcSrvData->tDevWifi.bIfSetNetParam = TRUE;					//是否设置网络参数（必填），TRUE：设置，以下参数有效，FALSE：不设置，保持原网络参数
					in_pMcSrvData->tDevWifi.bDhcpEnable = FALSE;					//DHCP使能（可选）
					strcpy(in_pMcSrvData->tDevWifi.sIpaddr, "192.168.1.22");		//IP地址（可选）
					strcpy(in_pMcSrvData->tDevWifi.sNetmask, "255.255.255.0");		//子网掩码（可选）
					strcpy(in_pMcSrvData->tDevWifi.sGateway, "192.168.1.1");		//网关地址（可选）
				}
				tPacketData.tCfgBody = in_pMcSrvData->tDevWifi;

				//发送消息给设备协议网络服务
				NPC_F_DPS_SendDevProData(
					in_pMcSrvData->hUmspProSrv,			//设备协议网络服务句柄
					(PNPC_BYTE)&tPacketData,			//设备协议数据缓冲区，包括NPC_S_DPS_MSG_HEAD
					sizeof(tPacketData)					//设备协议数据长度
					);
			}
			break;
		case NPC_D_PVM_CFG_FUNCID_GET_WIFIAP_LIST:			//获取WIFIAP列表
			{
				struct _PacketData{
					NPC_S_DPS_MSG_HEAD						tMsgHead;
					NPC_S_DPS_MSG_BODY_EVENT_E3_GET_CONFIG	tMsgBody;
					NPC_S_PVM_IMD_CFG_GET_WIFIAP_LIST		tCfgBody;
					NPC_S_PVM_IMD_CFG_WIFIAP_INFO			tWifiApTable[5];
				} tPacketData = {0};

				//显示信息
				NPC_F_LOG_SR_ShowInfo("[UMSP]  Get config: get wifiap list.");

				memcpy(&tPacketData, in_pMsgDataBuf, 
					sizeof(NPC_S_DPS_MSG_HEAD) + 
					sizeof(NPC_S_DPS_MSG_BODY_EVENT_E3_GET_CONFIG));

				tPacketData.tMsgHead.iMsgBodyLen = 
					sizeof(NPC_S_DPS_MSG_BODY_EVENT_E3_GET_CONFIG) + 
					sizeof(NPC_S_PVM_IMD_CFG_GET_WIFIAP_LIST) + 
					sizeof(NPC_S_PVM_IMD_CFG_WIFIAP_INFO) * 5;

				tPacketData.tMsgBody.io_iCfgDataLen = 
					sizeof(NPC_S_PVM_IMD_CFG_GET_WIFIAP_LIST) + 
					sizeof(NPC_S_PVM_IMD_CFG_WIFIAP_INFO) * 5;

				tPacketData.tCfgBody.iWifiApNum = 5;

				for ( NPC_INT i=0; i<5; i++ )
				{
					sprintf(tPacketData.tWifiApTable[i].sSSID, "wifissid-%d", i);		//SSID（必填）
					strcpy(tPacketData.tWifiApTable[i].sAuthType, "WPA2PSK");			//安全类型（可选）
					strcpy(tPacketData.tWifiApTable[i].sEncrypType, "AES");				//加密类型（可选）
					tPacketData.tWifiApTable[i].iChannel = 10 + i;						//信道号（可选）
					tPacketData.tWifiApTable[i].iRSSI = 50 + 10 * i;					//信号强度，值范围为0~100，0为信号最低，100为信号最强
				}

				//发送消息给设备协议网络服务
				NPC_F_DPS_SendDevProData(
					in_pMcSrvData->hUmspProSrv,			//设备协议网络服务句柄
					(PNPC_BYTE)&tPacketData,			//设备协议数据缓冲区，包括NPC_S_DPS_MSG_HEAD
					sizeof(tPacketData)					//设备协议数据长度
					);
			}
			break;
		case NPC_D_PVM_CFG_FUNCID_DEV_CODEC:				//编码配置
			{
				PNPC_S_UMSP_CAMERA_DATA pCameraData;
				PNPC_S_PVM_IMD_CFG_DEV_CODEC pCfgBody;

				//显示信息
				NPC_F_LOG_SR_ShowInfo("[UMSP]  Get config: dev codec.");

				if ( in_iMsgDataLen != (NPC_INT)(
					sizeof(NPC_S_DPS_MSG_HEAD) + 
					sizeof(NPC_S_DPS_MSG_BODY_EVENT_E3_GET_CONFIG) + 
					sizeof(NPC_S_PVM_IMD_CFG_DEV_CODEC) ) )
				{
					//显示信息
					NPC_F_LOG_SR_ShowInfo("[UMSP]  Get config: dev codec fail, cause: in_iMsgDataLen error.");

					in_pMsgHead->ucResult = NPC_D_MON_CSD_DPS_ERROR_CODE_PARAM_ERROR;

					bRet = FALSE;
					goto _NPC_CLEAR;
				}

				pCfgBody = (PNPC_S_PVM_IMD_CFG_DEV_CODEC)((PNPC_BYTE)pMsgBody + sizeof(NPC_S_DPS_MSG_BODY_EVENT_E3_GET_CONFIG));

				if ( pCfgBody->iChNo < 0 || 
					pCfgBody->iChNo >= NPC_D_DPS_MAX_CHANNEL_NUM || 
					pCfgBody->iChNo >= in_pMcSrvData->iChNum )
				{
					//显示信息
					NPC_F_LOG_SR_ShowInfo("[UMSP]  Get config: dev codec fail, cause: iChNo error.");

					in_pMsgHead->ucResult = NPC_D_MON_CSD_DPS_ERROR_CODE_CHANNEL_NO_ERROR;

					bRet = FALSE;
					goto _NPC_CLEAR;
				}

				pCameraData = &in_pMcSrvData->tCameraTable[pCfgBody->iChNo];

				*pCfgBody = pCameraData->tDevCodec;

				//发送消息给设备协议网络服务
				NPC_F_DPS_SendDevProData(
					in_pMcSrvData->hUmspProSrv,		//设备协议网络服务句柄
					in_pMsgDataBuf,					//设备协议数据缓冲区，包括NPC_S_DPS_MSG_HEAD
					in_iMsgDataLen					//设备协议数据长度
					);
			}
			break;

		case NPC_D_PVM_CFG_FUNCID_DEV_ALARM_MOTION_DETECT:	//移动侦测报警参数
			{
				PNPC_S_UMSP_CAMERA_DATA pCameraData;
				PNPC_S_PVM_IMD_CFG_DEV_ALARM_MOTION_DETECT pCfgBody;

				//显示信息
				NPC_F_LOG_SR_ShowInfo("[UMSP]  Get config: dev alarm motion detect.");

				if ( in_iMsgDataLen != (NPC_INT)(
					sizeof(NPC_S_DPS_MSG_HEAD) + 
					sizeof(NPC_S_DPS_MSG_BODY_EVENT_E3_GET_CONFIG) + 
					sizeof(NPC_S_PVM_IMD_CFG_DEV_ALARM_MOTION_DETECT) ) )
				{
					//显示信息
					NPC_F_LOG_SR_ShowInfo("[UMSP]  Get config: dev alarm motion detect fail, cause: in_iMsgDataLen error.");

					in_pMsgHead->ucResult = NPC_D_MON_CSD_DPS_ERROR_CODE_PARAM_ERROR;

					bRet = FALSE;
					goto _NPC_CLEAR;
				}

				pCfgBody = (PNPC_S_PVM_IMD_CFG_DEV_ALARM_MOTION_DETECT)((PNPC_BYTE)pMsgBody + sizeof(NPC_S_DPS_MSG_BODY_EVENT_E3_GET_CONFIG));

				if ( pCfgBody->iChNo < 0 || 
					pCfgBody->iChNo >= NPC_D_DPS_MAX_CHANNEL_NUM || 
					pCfgBody->iChNo >= in_pMcSrvData->iChNum )
				{
					//显示信息
					NPC_F_LOG_SR_ShowInfo("[UMSP]  Get config: dev alarm motion detect fail, cause: iChNo error.");

					in_pMsgHead->ucResult = NPC_D_MON_CSD_DPS_ERROR_CODE_CHANNEL_NO_ERROR;

					bRet = FALSE;
					goto _NPC_CLEAR;
				}

				pCameraData = &in_pMcSrvData->tCameraTable[pCfgBody->iChNo];

				*pCfgBody = pCameraData->tMotionDetect;

				//发送消息给设备协议网络服务
				NPC_F_DPS_SendDevProData(
					in_pMcSrvData->hUmspProSrv,		//设备协议网络服务句柄
					in_pMsgDataBuf,					//设备协议数据缓冲区，包括NPC_S_DPS_MSG_HEAD
					in_iMsgDataLen					//设备协议数据长度
					);
			}
			break;
		case NPC_D_PVM_CFG_FUNCID_DEV_ALARM_PROBE:			//探头输入报警参数
			{
				PNPC_S_UMSP_CAMERA_DATA pCameraData;
				PNPC_S_PVM_IMD_CFG_DEV_ALARM_PROBE pCfgBody;

				//显示信息
				NPC_F_LOG_SR_ShowInfo("[UMSP]  Get config: dev alarm probe.");

				if ( in_iMsgDataLen != (NPC_INT)(
					sizeof(NPC_S_DPS_MSG_HEAD) + 
					sizeof(NPC_S_DPS_MSG_BODY_EVENT_E3_GET_CONFIG) + 
					sizeof(NPC_S_PVM_IMD_CFG_DEV_ALARM_PROBE) ) )
				{
					//显示信息
					NPC_F_LOG_SR_ShowInfo("[UMSP]  Get config: dev alarm probe fail, cause: in_iMsgDataLen error.");

					in_pMsgHead->ucResult = NPC_D_MON_CSD_DPS_ERROR_CODE_PARAM_ERROR;

					bRet = FALSE;
					goto _NPC_CLEAR;
				}

				pCfgBody = (PNPC_S_PVM_IMD_CFG_DEV_ALARM_PROBE)((PNPC_BYTE)pMsgBody + sizeof(NPC_S_DPS_MSG_BODY_EVENT_E3_GET_CONFIG));

				if ( pCfgBody->iChNo < 0 || 
					pCfgBody->iChNo >= NPC_D_DPS_MAX_CHANNEL_NUM || 
					pCfgBody->iChNo >= in_pMcSrvData->iChNum )
				{
					//显示信息
					NPC_F_LOG_SR_ShowInfo("[UMSP]  Get config: dev alarm probe fail, cause: iChNo error.");

					in_pMsgHead->ucResult = NPC_D_MON_CSD_DPS_ERROR_CODE_CHANNEL_NO_ERROR;

					bRet = FALSE;
					goto _NPC_CLEAR;
				}

				pCameraData = &in_pMcSrvData->tCameraTable[pCfgBody->iChNo];

				*pCfgBody = pCameraData->tProbe;

				//发送消息给设备协议网络服务
				NPC_F_DPS_SendDevProData(
					in_pMcSrvData->hUmspProSrv,		//设备协议网络服务句柄
					in_pMsgDataBuf,					//设备协议数据缓冲区，包括NPC_S_DPS_MSG_HEAD
					in_iMsgDataLen					//设备协议数据长度
					);
			}
			break;

		case NPC_D_PVM_CFG_FUNCID_DEV_SYS_TIME:				//设备系统时间
			{
				struct _PacketData{
					NPC_S_DPS_MSG_HEAD								tMsgHead;
					NPC_S_DPS_MSG_BODY_EVENT_E3_GET_CONFIG			tMsgBody;
					NPC_S_PVM_IMD_CFG_DEV_SYS_TIME					tCfgBody;
				} tPacketData = {0};

				//显示信息
				NPC_F_LOG_SR_ShowInfo("[UMSP]  Get config: dev sys time.");

				memcpy(&tPacketData, in_pMsgDataBuf, 
					sizeof(NPC_S_DPS_MSG_HEAD) + 
					sizeof(NPC_S_DPS_MSG_BODY_EVENT_E3_GET_CONFIG));

				tPacketData.tMsgHead.iMsgBodyLen = 
					sizeof(NPC_S_DPS_MSG_BODY_EVENT_E3_GET_CONFIG) + 
					sizeof(NPC_S_PVM_IMD_CFG_DEV_SYS_TIME);

				tPacketData.tMsgBody.io_iCfgDataLen = 
					sizeof(NPC_S_PVM_IMD_CFG_DEV_SYS_TIME);

				if ( in_pMcSrvData->tDevTime == 0 )
					in_pMcSrvData->tDevTime = time(NULL);

				//将time_t转换为NPC_S_TD_DATE_TIME时间
				NPC_F_SYS_Timet_To_TdTime(
					in_pMcSrvData->tDevTime,			//时间值
					&tPacketData.tCfgBody.tDevTime		//TD时间
					);

				//发送消息给设备协议网络服务
				NPC_F_DPS_SendDevProData(
					in_pMcSrvData->hUmspProSrv,			//设备协议网络服务句柄
					(PNPC_BYTE)&tPacketData,			//设备协议数据缓冲区，包括NPC_S_DPS_MSG_HEAD
					sizeof(tPacketData)					//设备协议数据长度
					);
			}
			break;
		case NPC_D_PVM_CFG_FUNCID_DEV_INFO:					//设备信息
			//不处理该消息，客户端通过登录时获取设备信息
			break;
		case NPC_D_PVM_CFG_FUNCID_CH_NAME:					//通道名称
			{
				PNPC_S_UMSP_CAMERA_DATA pCameraData;
				PNPC_S_PVM_IMD_CFG_CH_NAME pCfgBody;

				//显示信息
				NPC_F_LOG_SR_ShowInfo("[UMSP]  Get config: ch name.");

				if ( in_iMsgDataLen != (NPC_INT)(
					sizeof(NPC_S_DPS_MSG_HEAD) + 
					sizeof(NPC_S_DPS_MSG_BODY_EVENT_E3_GET_CONFIG) + 
					sizeof(NPC_S_PVM_IMD_CFG_CH_NAME) ) )
				{
					//显示信息
					NPC_F_LOG_SR_ShowInfo("[UMSP]  Get config: ch name fail, cause: in_iMsgDataLen error.");

					in_pMsgHead->ucResult = NPC_D_MON_CSD_DPS_ERROR_CODE_PARAM_ERROR;

					bRet = FALSE;
					goto _NPC_CLEAR;
				}

				pCfgBody = (PNPC_S_PVM_IMD_CFG_CH_NAME)((PNPC_BYTE)pMsgBody + sizeof(NPC_S_DPS_MSG_BODY_EVENT_E3_GET_CONFIG));

				if ( pCfgBody->iChNo < 0 || 
					pCfgBody->iChNo >= NPC_D_DPS_MAX_CHANNEL_NUM || 
					pCfgBody->iChNo >= in_pMcSrvData->iChNum )
				{
					//显示信息
					NPC_F_LOG_SR_ShowInfo("[UMSP]  Get config: ch name fail, cause: iChNo error.");

					in_pMsgHead->ucResult = NPC_D_MON_CSD_DPS_ERROR_CODE_CHANNEL_NO_ERROR;

					bRet = FALSE;
					goto _NPC_CLEAR;
				}

				pCameraData = &in_pMcSrvData->tCameraTable[pCfgBody->iChNo];

				strcpy(pCfgBody->sChName, pCameraData->sChName);

				//发送消息给设备协议网络服务
				NPC_F_DPS_SendDevProData(
					in_pMcSrvData->hUmspProSrv,		//设备协议网络服务句柄
					in_pMsgDataBuf,					//设备协议数据缓冲区，包括NPC_S_DPS_MSG_HEAD
					in_iMsgDataLen					//设备协议数据长度
					);
			}
			break;

		case NPC_D_PVM_CFG_FUNCID_CAMERA_PARAM:				//摄像机参数
			{
				PNPC_S_UMSP_CAMERA_DATA pCameraData;
				PNPC_S_PVM_IMD_CFG_CAMERA_PARAM pCfgBody;

				//显示信息
				NPC_F_LOG_SR_ShowInfo("[UMSP]  Get config: camera param.");

				if ( in_iMsgDataLen != (NPC_INT)(
					sizeof(NPC_S_DPS_MSG_HEAD) + 
					sizeof(NPC_S_DPS_MSG_BODY_EVENT_E3_GET_CONFIG) + 
					sizeof(NPC_S_PVM_IMD_CFG_CAMERA_PARAM) ) )
				{
					//显示信息
					NPC_F_LOG_SR_ShowInfo("[UMSP]  Get config: camera param fail, cause: in_iMsgDataLen error.");

					in_pMsgHead->ucResult = NPC_D_MON_CSD_DPS_ERROR_CODE_PARAM_ERROR;

					bRet = FALSE;
					goto _NPC_CLEAR;
				}

				pCfgBody = (PNPC_S_PVM_IMD_CFG_CAMERA_PARAM)((PNPC_BYTE)pMsgBody + sizeof(NPC_S_DPS_MSG_BODY_EVENT_E3_GET_CONFIG));

				if ( pCfgBody->iChNo < 0 || 
					pCfgBody->iChNo >= NPC_D_DPS_MAX_CHANNEL_NUM || 
					pCfgBody->iChNo >= in_pMcSrvData->iChNum )
				{
					//显示信息
					NPC_F_LOG_SR_ShowInfo("[UMSP]  Get config: camera param fail, cause: iChNo error.");

					in_pMsgHead->ucResult = NPC_D_MON_CSD_DPS_ERROR_CODE_CHANNEL_NO_ERROR;

					bRet = FALSE;
					goto _NPC_CLEAR;
				}

				pCameraData = &in_pMcSrvData->tCameraTable[pCfgBody->iChNo];

				pCfgBody->bIfPictureFlip = pCameraData->bIfPictureFlip;

				//发送消息给设备协议网络服务
				NPC_F_DPS_SendDevProData(
					in_pMcSrvData->hUmspProSrv,		//设备协议网络服务句柄
					in_pMsgDataBuf,					//设备协议数据缓冲区，包括NPC_S_DPS_MSG_HEAD
					in_iMsgDataLen					//设备协议数据长度
					);
			}
			break;

		case NPC_D_PVM_CFG_FUNCID_DEV_RECORD_PLAN:			//录像计划配置
			{
				PNPC_S_UMSP_CAMERA_DATA pCameraData;
				PNPC_S_PVM_IMD_CFG_DEV_RECORD_PLAN pCfgBody;

				//显示信息
				NPC_F_LOG_SR_ShowInfo("[UMSP]  Get config: dev record plan.");

				if ( in_iMsgDataLen != (NPC_INT)(
					sizeof(NPC_S_DPS_MSG_HEAD) + 
					sizeof(NPC_S_DPS_MSG_BODY_EVENT_E3_GET_CONFIG) + 
					sizeof(NPC_S_PVM_IMD_CFG_DEV_RECORD_PLAN) ) )
				{
					//显示信息
					NPC_F_LOG_SR_ShowInfo("[UMSP]  Get config: dev record plan fail, cause: in_iMsgDataLen error.");

					in_pMsgHead->ucResult = NPC_D_MON_CSD_DPS_ERROR_CODE_PARAM_ERROR;

					bRet = FALSE;
					goto _NPC_CLEAR;
				}

				pCfgBody = (PNPC_S_PVM_IMD_CFG_DEV_RECORD_PLAN)((PNPC_BYTE)pMsgBody + sizeof(NPC_S_DPS_MSG_BODY_EVENT_E3_GET_CONFIG));

				if ( pCfgBody->iChNo < 0 || 
					pCfgBody->iChNo >= NPC_D_DPS_MAX_CHANNEL_NUM || 
					pCfgBody->iChNo >= in_pMcSrvData->iChNum )
				{
					//显示信息
					NPC_F_LOG_SR_ShowInfo("[UMSP]  Get config: dev record plan fail, cause: iChNo error.");

					in_pMsgHead->ucResult = NPC_D_MON_CSD_DPS_ERROR_CODE_CHANNEL_NO_ERROR;

					bRet = FALSE;
					goto _NPC_CLEAR;
				}

				pCameraData = &in_pMcSrvData->tCameraTable[pCfgBody->iChNo];

				*pCfgBody = pCameraData->tDevRecordPlan;

				//发送消息给设备协议网络服务
				NPC_F_DPS_SendDevProData(
					in_pMcSrvData->hUmspProSrv,		//设备协议网络服务句柄
					in_pMsgDataBuf,					//设备协议数据缓冲区，包括NPC_S_DPS_MSG_HEAD
					in_iMsgDataLen					//设备协议数据长度
					);
			}
			break;
		case NPC_D_PVM_CFG_FUNCID_ALARM_CLOUD_STORAGE:		//报警云存储配置
			{
				PNPC_S_UMSP_CAMERA_DATA pCameraData;
				PNPC_S_PVM_IMD_CFG_ALARM_CLOUD_STORAGE pCfgBody;

				//显示信息
				NPC_F_LOG_SR_ShowInfo("[UMSP]  Get config: alarm cloud storate.");

				if ( in_iMsgDataLen != (NPC_INT)(
					sizeof(NPC_S_DPS_MSG_HEAD) + 
					sizeof(NPC_S_DPS_MSG_BODY_EVENT_E3_GET_CONFIG) + 
					sizeof(NPC_S_PVM_IMD_CFG_ALARM_CLOUD_STORAGE) ) )
				{
					//显示信息
					NPC_F_LOG_SR_ShowInfo("[UMSP]  Get config: alarm cloud storate fail, cause: in_iMsgDataLen error.");

					in_pMsgHead->ucResult = NPC_D_MON_CSD_DPS_ERROR_CODE_PARAM_ERROR;

					bRet = FALSE;
					goto _NPC_CLEAR;
				}

				pCfgBody = (PNPC_S_PVM_IMD_CFG_ALARM_CLOUD_STORAGE)((PNPC_BYTE)pMsgBody + sizeof(NPC_S_DPS_MSG_BODY_EVENT_E3_GET_CONFIG));

				if ( pCfgBody->iChNo < 0 || 
					pCfgBody->iChNo >= NPC_D_DPS_MAX_CHANNEL_NUM || 
					pCfgBody->iChNo >= in_pMcSrvData->iChNum )
				{
					//显示信息
					NPC_F_LOG_SR_ShowInfo("[UMSP]  Get config: alarm cloud storate fail, cause: iChNo error.");

					in_pMsgHead->ucResult = NPC_D_MON_CSD_DPS_ERROR_CODE_CHANNEL_NO_ERROR;

					bRet = FALSE;
					goto _NPC_CLEAR;
				}

				pCameraData = &in_pMcSrvData->tCameraTable[pCfgBody->iChNo];

				*pCfgBody = pCameraData->tAlarmCloudStorage;

				//发送消息给设备协议网络服务
				NPC_F_DPS_SendDevProData(
					in_pMcSrvData->hUmspProSrv,		//设备协议网络服务句柄
					in_pMsgDataBuf,					//设备协议数据缓冲区，包括NPC_S_DPS_MSG_HEAD
					in_iMsgDataLen					//设备协议数据长度
					);
			}
			break;

		case NPC_D_PVM_CFG_FUNCID_STORAGE_QUERY_SIZE:		//查询存储容量
			{
				struct _PacketData{
					NPC_S_DPS_MSG_HEAD								tMsgHead;
					NPC_S_DPS_MSG_BODY_EVENT_E3_GET_CONFIG			tMsgBody;
					NPC_S_PVM_IMD_CFG_STORAGE_QUERY_SIZE			tCfgBody;
				} tPacketData = {0};

				//显示信息
				NPC_F_LOG_SR_ShowInfo("[UMSP]  Get config: storage query size.");

				memcpy(&tPacketData, in_pMsgDataBuf, 
					sizeof(NPC_S_DPS_MSG_HEAD) + 
					sizeof(NPC_S_DPS_MSG_BODY_EVENT_E3_GET_CONFIG));

				tPacketData.tMsgHead.iMsgBodyLen = 
					sizeof(NPC_S_DPS_MSG_BODY_EVENT_E3_GET_CONFIG) + 
					sizeof(NPC_S_PVM_IMD_CFG_STORAGE_QUERY_SIZE);

				tPacketData.tMsgBody.io_iCfgDataLen = 
					sizeof(NPC_S_PVM_IMD_CFG_STORAGE_QUERY_SIZE);

				tPacketData.tCfgBody.iStorageNum = 1;

				tPacketData.tCfgBody.tStorage[0].iSerialNo = 0;
				tPacketData.tCfgBody.tStorage[0].dwStorageSize = 200;
				tPacketData.tCfgBody.tStorage[0].dwRemainSize = 100;

				//发送消息给设备协议网络服务
				NPC_F_DPS_SendDevProData(
					in_pMcSrvData->hUmspProSrv,			//设备协议网络服务句柄
					(PNPC_BYTE)&tPacketData,			//设备协议数据缓冲区，包括NPC_S_DPS_MSG_HEAD
					sizeof(tPacketData)					//设备协议数据长度
					);
			}
			break;
		case NPC_D_PVM_CFG_FUNCID_STORAGE_FORMAT:			//格式化存储器
			//不处理该消息，该消息用于设置参数使用
			break;

		default:
			{
				//显示信息
				NPC_CHAR sTempLogBuf[128];
				sprintf(sTempLogBuf, "[UMSP]  Get config: error fund id: %d.", pMsgBody->i_iCfgFuncId);
				NPC_F_LOG_SR_ShowInfo(sTempLogBuf);

				in_pMsgHead->ucResult = NPC_D_MON_CSD_DPS_ERROR_CODE_ID_ERROR;
				bRet = FALSE;
			}
			break;
		}
	}

	//CLEAR AREA
	_NPC_CLEAR:
	{
		if ( !bRet )
		{
			//发送消息给设备协议网络服务
			NPC_F_DPS_SendDevProData(
				in_pMcSrvData->hUmspProSrv,			//设备协议网络服务句柄
				in_pMsgDataBuf,						//设备协议数据缓冲区，包括NPC_S_DPS_MSG_HEAD
				in_iMsgDataLen						//设备协议数据长度
				);
		}
	}
}

//处理协议事件：设置配置参数
NPC_VOID	NPC_F_UMSP_MC_DoProEvent_EVENT_E3_SET_CONFIG(
	IN		PNPC_S_UMSP_MCSRV_DATA					in_pMcSrvData,				//主控服务数据
	IN		PNPC_S_DPS_MSG_HEAD						in_pMsgHead,				//消息头
	IN		PNPC_BYTE								in_pMsgDataBuf,				//消息数据缓冲区，包括NPC_S_DPS_MSG_HEAD
	IN		NPC_INT									in_iMsgDataLen				//消息数据长度
	)
{
	NPC_BOOL										bRet;
	PNPC_S_DPS_MSG_BODY_EVENT_E3_SET_CONFIG			pMsgBody;

	//INIT AREA
	{
		bRet = TRUE;
		pMsgBody = (PNPC_S_DPS_MSG_BODY_EVENT_E3_SET_CONFIG)((PNPC_BYTE)in_pMsgHead + sizeof(NPC_S_DPS_MSG_HEAD));

		in_pMsgHead->ucResult = 0;
		in_pMsgHead->ucMsgType = NPC_D_DPS_MSG_TYPE_RESPONSION;
	}

	//CODE AREA
	{
		switch ( pMsgBody->i_iCfgFuncId )
		{
		case NPC_D_PVM_CFG_FUNCID_DEV_NAME:					//设备名称
			//暂不处理该消息
			break;
		case NPC_D_PVM_CFG_FUNCID_DEV_IP:					//设备有线IP地址
			{
				PNPC_S_PVM_IMD_CFG_DEV_IP pCfgBody;

				//显示信息
				NPC_F_LOG_SR_ShowInfo("[UMSP]  Set config: dev ip.");

				if ( in_iMsgDataLen != (NPC_INT)(
					sizeof(NPC_S_DPS_MSG_HEAD) + 
					sizeof(NPC_S_DPS_MSG_BODY_EVENT_E3_SET_CONFIG) + 
					sizeof(NPC_S_PVM_IMD_CFG_DEV_IP) ) )
				{
					//显示信息
					NPC_F_LOG_SR_ShowInfo("[UMSP]  Set config: dev ip fail, cause: in_iMsgDataLen error.");
					
					in_pMsgHead->ucResult = NPC_D_MON_CSD_DPS_ERROR_CODE_PARAM_ERROR;
					bRet = FALSE;
					break;
				}

				pCfgBody = (PNPC_S_PVM_IMD_CFG_DEV_IP)((PNPC_BYTE)pMsgBody + sizeof(NPC_S_DPS_MSG_BODY_EVENT_E3_SET_CONFIG));

				in_pMcSrvData->tDevIp = *pCfgBody;

				//---------------------------------------------------------------------------
				NPC_CHAR sTempLogBuf[256];

				sprintf(sTempLogBuf, "[UMSP]  Set dev ip: sNetcardName: %s.", pCfgBody->sNetcardName);
				NPC_F_LOG_SR_ShowInfo(sTempLogBuf);

				sprintf(sTempLogBuf, "[UMSP]  Set dev ip: sMac: %s.", pCfgBody->sMac);
				NPC_F_LOG_SR_ShowInfo(sTempLogBuf);

				sprintf(sTempLogBuf, "[UMSP]  Set dev ip: bDhcpEnable: %d.", pCfgBody->bDhcpEnable);
				NPC_F_LOG_SR_ShowInfo(sTempLogBuf);

				sprintf(sTempLogBuf, "[UMSP]  Set dev ip: sIpaddr: %s.", pCfgBody->sIpaddr);
				NPC_F_LOG_SR_ShowInfo(sTempLogBuf);

				sprintf(sTempLogBuf, "[UMSP]  Set dev ip: sNetmask: %s.", pCfgBody->sNetmask);
				NPC_F_LOG_SR_ShowInfo(sTempLogBuf);

				sprintf(sTempLogBuf, "[UMSP]  Set dev ip: sGateway: %s.", pCfgBody->sGateway);
				NPC_F_LOG_SR_ShowInfo(sTempLogBuf);
			}
			break;

		case NPC_D_PVM_CFG_FUNCID_DEV_WIFI:					//设备WIFI信息
			{
				PNPC_S_PVM_IMD_CFG_DEV_WIFI pDevWifi;
				NPC_CHAR sTempLogBuf[256];

				//显示信息
				NPC_F_LOG_SR_ShowInfo("[UMSP]  Set config: dev wifi.");

				if ( in_iMsgDataLen != (NPC_INT)(
					sizeof(NPC_S_DPS_MSG_HEAD) + 
					sizeof(NPC_S_DPS_MSG_BODY_EVENT_E3_SET_CONFIG) + 
					sizeof(NPC_S_PVM_IMD_CFG_DEV_WIFI) ) )
				{
					//显示信息
					NPC_F_LOG_SR_ShowInfo("[UMSP]  Set config: dev wifi fail, cause: in_iMsgDataLen error.");

					in_pMsgHead->ucResult = NPC_D_MON_CSD_DPS_ERROR_CODE_PARAM_ERROR;
					bRet = FALSE;
					break;
				}

				pDevWifi = (PNPC_S_PVM_IMD_CFG_DEV_WIFI)((PNPC_BYTE)pMsgBody + sizeof(NPC_S_DPS_MSG_BODY_EVENT_E3_SET_CONFIG));

				in_pMcSrvData->tDevWifi = *pDevWifi;

				//---------------------------------------------------------------------------
				//显示信息
				sprintf(sTempLogBuf, "[UMSP]  Set wifi: bEnable: %d.", pDevWifi->bEnable);
				NPC_F_LOG_SR_ShowInfo(sTempLogBuf);

				sprintf(sTempLogBuf, "[UMSP]  Set wifi: sWifiSSID: %s.", pDevWifi->sWifiSSID);
				NPC_F_LOG_SR_ShowInfo(sTempLogBuf);

				sprintf(sTempLogBuf, "[UMSP]  Set wifi: sWifiPwd: %s.", pDevWifi->sWifiPwd);
				NPC_F_LOG_SR_ShowInfo(sTempLogBuf);

				sprintf(sTempLogBuf, "[UMSP]  Set wifi: bFieldEnable_AuthType: %d, sAuthType: %s.", pDevWifi->bFieldEnable_AuthType, pDevWifi->sAuthType);
				NPC_F_LOG_SR_ShowInfo(sTempLogBuf);

				sprintf(sTempLogBuf, "[UMSP]  Set wifi: bFieldEnable_EncrypType: %d, sEncrypType: %s.", pDevWifi->bFieldEnable_EncrypType, pDevWifi->sEncrypType);
				NPC_F_LOG_SR_ShowInfo(sTempLogBuf);

				sprintf(sTempLogBuf, "[UMSP]  Set wifi: bFieldEnable_Channel: %d, iChannel: %d.", pDevWifi->bFieldEnable_Channel, pDevWifi->iChannel);
				NPC_F_LOG_SR_ShowInfo(sTempLogBuf);

				sprintf(sTempLogBuf, "[UMSP]  Set wifi: iRSSI: %d, iRSSI: %d.", pDevWifi->iRSSI, pDevWifi->iRSSI);
				NPC_F_LOG_SR_ShowInfo(sTempLogBuf);
			}
			break;
		case NPC_D_PVM_CFG_FUNCID_DEV_CODEC:				//编码配置
			{
				PNPC_S_PVM_IMD_CFG_DEV_CODEC pCfgBody;
				PNPC_S_UMSP_CAMERA_DATA pCameraData;

				//显示信息
				NPC_F_LOG_SR_ShowInfo("[UMSP]  Set config: dev codec.");

				if ( in_iMsgDataLen != (NPC_INT)(
					sizeof(NPC_S_DPS_MSG_HEAD) + 
					sizeof(NPC_S_DPS_MSG_BODY_EVENT_E3_SET_CONFIG) + 
					sizeof(NPC_S_PVM_IMD_CFG_DEV_CODEC) ) )
				{
					//显示信息
					NPC_F_LOG_SR_ShowInfo("[UMSP]  Set config: dev codec fail, cause: in_iMsgDataLen error.");

					in_pMsgHead->ucResult = NPC_D_MON_CSD_DPS_ERROR_CODE_PARAM_ERROR;
					bRet = FALSE;
					break;
				}

				pCfgBody = (PNPC_S_PVM_IMD_CFG_DEV_CODEC)((PNPC_BYTE)pMsgBody + sizeof(NPC_S_DPS_MSG_BODY_EVENT_E3_SET_CONFIG));

				if ( pCfgBody->iChNo < 0 || pCfgBody->iChNo >= NPC_D_DPS_MAX_CHANNEL_NUM )
				{
					//显示信息
					NPC_F_LOG_SR_ShowInfo("[UMSP]  Set config: dev codec fail, cause: iChNo error.");

					in_pMsgHead->ucResult = NPC_D_MON_CSD_DPS_ERROR_CODE_CHANNEL_NO_ERROR;

					bRet = FALSE;
					break;
				}

				pCameraData = &in_pMcSrvData->tCameraTable[pCfgBody->iChNo];

				pCameraData->tDevCodec = *pCfgBody;

				//---------------------------------------------------------------------------
				NPC_CHAR sTempLogBuf[256];

				sprintf(sTempLogBuf, "[UMSP]  Set dev codec: iChNo: %d.", pCfgBody->iChNo);
				NPC_F_LOG_SR_ShowInfo(sTempLogBuf);

				for ( NPC_INT y=0; y<2; y++ )
				{
					sprintf(sTempLogBuf, "[UMSP]  Set dev codec: bVideoEnable[%d]: %d.", y, pCfgBody->tCodecInfo[y].bVideoEnable);
					NPC_F_LOG_SR_ShowInfo(sTempLogBuf);

					sprintf(sTempLogBuf, "[UMSP]  Set dev codec: bAudioEnable[%d]: %d.", y, pCfgBody->tCodecInfo[y].bAudioEnable);
					NPC_F_LOG_SR_ShowInfo(sTempLogBuf);

					sprintf(sTempLogBuf, "[UMSP]  Set dev codec: tVideoFormat.iCodecId[%d]: %d.", y, pCfgBody->tCodecInfo[y].tVideoFormat.iCodecId);
					NPC_F_LOG_SR_ShowInfo(sTempLogBuf);

					sprintf(sTempLogBuf, "[UMSP]  Set dev codec: tVideoFormat.iResolution[%d]: %d.", y, pCfgBody->tCodecInfo[y].tVideoFormat.iResolution);
					NPC_F_LOG_SR_ShowInfo(sTempLogBuf);

					sprintf(sTempLogBuf, "[UMSP]  Set dev codec: tVideoFormat.iBitRateControl[%d]: %d.", y, pCfgBody->tCodecInfo[y].tVideoFormat.iBitRateControl);
					NPC_F_LOG_SR_ShowInfo(sTempLogBuf);

					sprintf(sTempLogBuf, "[UMSP]  Set dev codec: tVideoFormat.iBitRate[%d]: %d.", y, pCfgBody->tCodecInfo[y].tVideoFormat.iBitRate);
					NPC_F_LOG_SR_ShowInfo(sTempLogBuf);

					sprintf(sTempLogBuf, "[UMSP]  Set dev codec: tVideoFormat.iFrameRate[%d]: %d.", y, pCfgBody->tCodecInfo[y].tVideoFormat.iFrameRate);
					NPC_F_LOG_SR_ShowInfo(sTempLogBuf);

					sprintf(sTempLogBuf, "[UMSP]  Set dev codec: tVideoFormat.iGOP[%d]: %d.", y, pCfgBody->tCodecInfo[y].tVideoFormat.iGOP);
					NPC_F_LOG_SR_ShowInfo(sTempLogBuf);

					sprintf(sTempLogBuf, "[UMSP]  Set dev codec: tVideoFormat.iQuality[%d]: %d.", y, pCfgBody->tCodecInfo[y].tVideoFormat.iQuality);
					NPC_F_LOG_SR_ShowInfo(sTempLogBuf);

					sprintf(sTempLogBuf, "[UMSP]  Set dev codec: tAudioFormat.iCodecId[%d]: %d.", y, pCfgBody->tCodecInfo[y].tAudioFormat.iCodecId);
					NPC_F_LOG_SR_ShowInfo(sTempLogBuf);

					sprintf(sTempLogBuf, "[UMSP]  Set dev codec: tAudioFormat.iBitRate[%d]: %d.", y, pCfgBody->tCodecInfo[y].tAudioFormat.iBitRate);
					NPC_F_LOG_SR_ShowInfo(sTempLogBuf);

					sprintf(sTempLogBuf, "[UMSP]  Set dev codec: tAudioFormat.iSampleRate[%d]: %d.", y, pCfgBody->tCodecInfo[y].tAudioFormat.iSampleRate);
					NPC_F_LOG_SR_ShowInfo(sTempLogBuf);

					sprintf(sTempLogBuf, "[UMSP]  Set dev codec: tAudioFormat.iMaxVolume[%d]: %d.", y, pCfgBody->tCodecInfo[y].tAudioFormat.iMaxVolume);
					NPC_F_LOG_SR_ShowInfo(sTempLogBuf);
				}
			}
			break;

		case NPC_D_PVM_CFG_FUNCID_DEV_ALARM_MOTION_DETECT:	//移动侦测报警参数
			{
				PNPC_S_PVM_IMD_CFG_DEV_ALARM_MOTION_DETECT pCfgBody;
				PNPC_S_UMSP_CAMERA_DATA pCameraData;

				//显示信息
				NPC_F_LOG_SR_ShowInfo("[UMSP]  Set config: dev alarm motion detect.");

				if ( in_iMsgDataLen != (NPC_INT)(
					sizeof(NPC_S_DPS_MSG_HEAD) + 
					sizeof(NPC_S_DPS_MSG_BODY_EVENT_E3_SET_CONFIG) + 
					sizeof(NPC_S_PVM_IMD_CFG_DEV_ALARM_MOTION_DETECT) ) )
				{
					//显示信息
					NPC_F_LOG_SR_ShowInfo("[UMSP]  Set config: dev alarm motion detect fail, cause: in_iMsgDataLen error.");

					in_pMsgHead->ucResult = NPC_D_MON_CSD_DPS_ERROR_CODE_PARAM_ERROR;
					bRet = FALSE;
					break;
				}

				pCfgBody = (PNPC_S_PVM_IMD_CFG_DEV_ALARM_MOTION_DETECT)((PNPC_BYTE)pMsgBody + sizeof(NPC_S_DPS_MSG_BODY_EVENT_E3_SET_CONFIG));

				if ( pCfgBody->iChNo < 0 || pCfgBody->iChNo >= NPC_D_DPS_MAX_CHANNEL_NUM )
				{
					//显示信息
					NPC_F_LOG_SR_ShowInfo("[UMSP]  Set config: dev alarm motion detect fail, cause: iChNo error.");

					in_pMsgHead->ucResult = NPC_D_MON_CSD_DPS_ERROR_CODE_CHANNEL_NO_ERROR;

					bRet = FALSE;
					break;
				}

				pCameraData = &in_pMcSrvData->tCameraTable[pCfgBody->iChNo];

				pCameraData->tMotionDetect = *pCfgBody;

				//---------------------------------------------------------------------------
				NPC_CHAR sTempLogBuf[1024];

				sprintf(sTempLogBuf, "[UMSP]  Set dev alarm: iChNo: %d.", pCfgBody->iChNo);
				NPC_F_LOG_SR_ShowInfo(sTempLogBuf);

				sprintf(sTempLogBuf, "[UMSP]  Set dev alarm: bIfEnable: %d.", pCfgBody->bIfEnable);
				NPC_F_LOG_SR_ShowInfo(sTempLogBuf);

				sprintf(sTempLogBuf, "[UMSP]  Set dev alarm: bFuncEnable_Level: %d.", pCfgBody->bFuncEnable_Level);
				NPC_F_LOG_SR_ShowInfo(sTempLogBuf);

				sprintf(sTempLogBuf, "[UMSP]  Set dev alarm: bFuncEnable_Region: %d.", pCfgBody->bFuncEnable_Region);
				NPC_F_LOG_SR_ShowInfo(sTempLogBuf);

				sprintf(sTempLogBuf, "[UMSP]  Set dev alarm: bFuncEnable_Photo: %d.", pCfgBody->bFuncEnable_Photo);
				NPC_F_LOG_SR_ShowInfo(sTempLogBuf);

				sprintf(sTempLogBuf, "[UMSP]  Set dev alarm: bFuncEnable_Record: %d.", pCfgBody->bFuncEnable_Record);
				NPC_F_LOG_SR_ShowInfo(sTempLogBuf);

				sprintf(sTempLogBuf, "[UMSP]  Set dev alarm: bFuncEnable_AlarmOut: %d.", pCfgBody->bFuncEnable_AlarmOut);
				NPC_F_LOG_SR_ShowInfo(sTempLogBuf);

				sprintf(sTempLogBuf, "[UMSP]  Set dev alarm: bFuncEnable_Beep: %d.", pCfgBody->bFuncEnable_Beep);
				NPC_F_LOG_SR_ShowInfo(sTempLogBuf);

				sprintf(sTempLogBuf, "Set dev alarm: bFuncEnable_WeekSect: %d.", pCfgBody->bFuncEnable_WeekSect);
				NPC_F_LOG_SR_ShowInfo(sTempLogBuf);

				sprintf(sTempLogBuf, "[UMSP]  Set dev alarm: iLevel: %d.", pCfgBody->iLevel);
				NPC_F_LOG_SR_ShowInfo(sTempLogBuf);

				if ( pCfgBody->bFuncEnable_Region )
				{
					strcpy(sTempLogBuf, "[UMSP]  Set dev alarm: dwRegion: ");
					for ( NPC_INT y=0; y<32; y++ )
					{
						sprintf(sTempLogBuf+strlen(sTempLogBuf), "0x%8.8X ", pCfgBody->dwRegion[y]);
					}
					NPC_F_LOG_SR_ShowInfo(sTempLogBuf);
				}

				sprintf(sTempLogBuf, "[UMSP]  Set dev alarm: bPhotoEnable: %d.", pCfgBody->bPhotoEnable);
				NPC_F_LOG_SR_ShowInfo(sTempLogBuf);

				sprintf(sTempLogBuf, "[UMSP]  Set dev alarm: bRecordEnable: %d.", pCfgBody->bRecordEnable);
				NPC_F_LOG_SR_ShowInfo(sTempLogBuf);

				sprintf(sTempLogBuf, "[UMSP]  Set dev alarm: iRecordDelayedTime: %d.", pCfgBody->iRecordDelayedTime);
				NPC_F_LOG_SR_ShowInfo(sTempLogBuf);

				sprintf(sTempLogBuf, "[UMSP]  Set dev alarm: bAlarmOutEnable: %d.", pCfgBody->bAlarmOutEnable);
				NPC_F_LOG_SR_ShowInfo(sTempLogBuf);

				sprintf(sTempLogBuf, "[UMSP]  Set dev alarm: iAlarmOutDelayedTime: %d.", pCfgBody->iAlarmOutDelayedTime);
				NPC_F_LOG_SR_ShowInfo(sTempLogBuf);

				sprintf(sTempLogBuf, "[UMSP]  Set dev alarm: bBeepEnable: %d.", pCfgBody->bBeepEnable);
				NPC_F_LOG_SR_ShowInfo(sTempLogBuf);
				
				if ( pCfgBody->bFuncEnable_WeekSect )
				{
					for ( NPC_INT k=0; k<7; k++ )
					{
						for ( NPC_INT m=0; m<NPC_D_PVM_CFG_REC_PLAN_TIME_SECT_NUM; m++ )
						{
							sprintf(sTempLogBuf, "[UMSP]  Set dev alarm: Time sect: bIfEnable[%d][%d]: %d.", 
								k, m, pCfgBody->tWeekSect[k].tTimeSect[m].bIfEnable);
							NPC_F_LOG_SR_ShowInfo(sTempLogBuf);

							sprintf(sTempLogBuf, "[UMSP]  Set dev alarm: Time sect: tBeginTime[%d][%d]: %2.2d:%2.2d:%2.2d.", 
								k, m, pCfgBody->tWeekSect[k].tTimeSect[m].tBeginTime.ucHour, 
								pCfgBody->tWeekSect[k].tTimeSect[m].tBeginTime.ucMin, 
								pCfgBody->tWeekSect[k].tTimeSect[m].tBeginTime.ucSec);
							NPC_F_LOG_SR_ShowInfo(sTempLogBuf);

							sprintf(sTempLogBuf, "[UMSP]  Set dev alarm: Time sect: tEndTime[%d][%d]: %2.2d:%2.2d:%2.2d.", 
								k, m, pCfgBody->tWeekSect[k].tTimeSect[m].tEndTime.ucHour, 
								pCfgBody->tWeekSect[k].tTimeSect[m].tEndTime.ucMin, 
								pCfgBody->tWeekSect[k].tTimeSect[m].tEndTime.ucSec);
							NPC_F_LOG_SR_ShowInfo(sTempLogBuf);
						}
					}
				}
			}
			break;

		case NPC_D_PVM_CFG_FUNCID_DEV_ALARM_PROBE:			//探头输入报警参数
			{
				PNPC_S_UMSP_CAMERA_DATA pCameraData;
				PNPC_S_PVM_IMD_CFG_DEV_ALARM_PROBE pCfgBody;

				//显示信息
				NPC_F_LOG_SR_ShowInfo("[UMSP]  Set config: dev alarm probe.");

				if ( in_iMsgDataLen != (NPC_INT)(
					sizeof(NPC_S_DPS_MSG_HEAD) + 
					sizeof(NPC_S_DPS_MSG_BODY_EVENT_E3_SET_CONFIG) + 
					sizeof(NPC_S_PVM_IMD_CFG_DEV_ALARM_PROBE)) )
				{
					//显示信息
					NPC_F_LOG_SR_ShowInfo("[UMSP]  Set config: dev alarm probe fail, cause: in_iMsgDataLen error.");

					in_pMsgHead->ucResult = NPC_D_MON_CSD_DPS_ERROR_CODE_PARAM_ERROR;

					bRet = FALSE;
					break;
				}

				pCfgBody = (PNPC_S_PVM_IMD_CFG_DEV_ALARM_PROBE)((PNPC_BYTE)pMsgBody + sizeof(NPC_S_DPS_MSG_BODY_EVENT_E3_SET_CONFIG));

				if ( pCfgBody->iChNo < 0 || pCfgBody->iChNo >= NPC_D_DPS_MAX_CHANNEL_NUM )
				{
					//显示信息
					NPC_F_LOG_SR_ShowInfo("[UMSP]  Set config: dev alarm probe fail, cause: iChNo error.");

					in_pMsgHead->ucResult = NPC_D_MON_CSD_DPS_ERROR_CODE_CHANNEL_NO_ERROR;

					bRet = FALSE;
					break;
				}

				pCameraData = &in_pMcSrvData->tCameraTable[pCfgBody->iChNo];

				pCameraData->tProbe = *pCfgBody;

				//---------------------------------------------------------------------------
				NPC_CHAR sTempLogBuf[256];

				sprintf(sTempLogBuf, "[UMSP]  Set dev alarm: iChNo: %d.", pCfgBody->iChNo);
				NPC_F_LOG_SR_ShowInfo(sTempLogBuf);

				sprintf(sTempLogBuf, "[UMSP]  Set dev alarm: bIfEnable: %d.", pCfgBody->bIfEnable);
				NPC_F_LOG_SR_ShowInfo(sTempLogBuf);

				sprintf(sTempLogBuf, "[UMSP]  Set dev alarm: bFuncEnable_SensorType: %d.", pCfgBody->bFuncEnable_SensorType);
				NPC_F_LOG_SR_ShowInfo(sTempLogBuf);

				sprintf(sTempLogBuf, "[UMSP]  Set dev alarm: bFuncEnable_Photo: %d.", pCfgBody->bFuncEnable_Photo);
				NPC_F_LOG_SR_ShowInfo(sTempLogBuf);

				sprintf(sTempLogBuf, "[UMSP]  Set dev alarm: bFuncEnable_Record: %d.", pCfgBody->bFuncEnable_Record);
				NPC_F_LOG_SR_ShowInfo(sTempLogBuf);

				sprintf(sTempLogBuf, "[UMSP]  Set dev alarm: bFuncEnable_AlarmOut: %d.", pCfgBody->bFuncEnable_AlarmOut);
				NPC_F_LOG_SR_ShowInfo(sTempLogBuf);

				sprintf(sTempLogBuf, "[UMSP]  Set dev alarm: bFuncEnable_Beep: %d.", pCfgBody->bFuncEnable_Beep);
				NPC_F_LOG_SR_ShowInfo(sTempLogBuf);

				sprintf(sTempLogBuf, "[UMSP]  Set dev alarm: bFuncEnable_WeekSect: %d.", pCfgBody->bFuncEnable_WeekSect);
				NPC_F_LOG_SR_ShowInfo(sTempLogBuf);

				sprintf(sTempLogBuf, "[UMSP]  Set dev alarm: iSensorType: %d.", pCfgBody->iSensorType);
				NPC_F_LOG_SR_ShowInfo(sTempLogBuf);

				sprintf(sTempLogBuf, "[UMSP]  Set dev alarm: bPhotoEnable: %d.", pCfgBody->bPhotoEnable);
				NPC_F_LOG_SR_ShowInfo(sTempLogBuf);

				sprintf(sTempLogBuf, "[UMSP]  Set dev alarm: bRecordEnable: %d.", pCfgBody->bRecordEnable);
				NPC_F_LOG_SR_ShowInfo(sTempLogBuf);

				sprintf(sTempLogBuf, "[UMSP]  Set dev alarm: iRecordDelayedTime: %d.", pCfgBody->iRecordDelayedTime);
				NPC_F_LOG_SR_ShowInfo(sTempLogBuf);

				sprintf(sTempLogBuf, "[UMSP]  Set dev alarm: bAlarmOutEnable: %d.", pCfgBody->bAlarmOutEnable);
				NPC_F_LOG_SR_ShowInfo(sTempLogBuf);

				sprintf(sTempLogBuf, "[UMSP]  Set dev alarm: iAlarmOutDelayedTime: %d.", pCfgBody->iAlarmOutDelayedTime);
				NPC_F_LOG_SR_ShowInfo(sTempLogBuf);

				sprintf(sTempLogBuf, "[UMSP]  Set dev alarm: bBeepEnable: %d.", pCfgBody->bBeepEnable);
				NPC_F_LOG_SR_ShowInfo(sTempLogBuf);
				
				if ( pCfgBody->bFuncEnable_WeekSect )
				{
					for ( NPC_INT k=0; k<7; k++ )
					{
						for ( NPC_INT m=0; m<NPC_D_PVM_CFG_REC_PLAN_TIME_SECT_NUM; m++ )
						{
							sprintf(sTempLogBuf, "[UMSP]  Set dev alarm: Time sect: bIfEnable[%d][%d]: %d.", 
								k, m, pCfgBody->tWeekSect[k].tTimeSect[m].bIfEnable);
							NPC_F_LOG_SR_ShowInfo(sTempLogBuf);

							sprintf(sTempLogBuf, "[UMSP]  Set dev alarm: Time sect: tBeginTime[%d][%d]: %2.2d:%2.2d:%2.2d.", 
								k, m, pCfgBody->tWeekSect[k].tTimeSect[m].tBeginTime.ucHour, 
								pCfgBody->tWeekSect[k].tTimeSect[m].tBeginTime.ucMin, 
								pCfgBody->tWeekSect[k].tTimeSect[m].tBeginTime.ucSec);
							NPC_F_LOG_SR_ShowInfo(sTempLogBuf);

							sprintf(sTempLogBuf, "[UMSP]  Set dev alarm: Time sect: tEndTime[%d][%d]: %2.2d:%2.2d:%2.2d.", 
								k, m, pCfgBody->tWeekSect[k].tTimeSect[m].tEndTime.ucHour, 
								pCfgBody->tWeekSect[k].tTimeSect[m].tEndTime.ucMin, 
								pCfgBody->tWeekSect[k].tTimeSect[m].tEndTime.ucSec);
							NPC_F_LOG_SR_ShowInfo(sTempLogBuf);
						}
					}
				}
			}
			break;

		case NPC_D_PVM_CFG_FUNCID_DEV_SYS_TIME:				//设备系统时间
			{
				PNPC_S_PVM_IMD_CFG_DEV_SYS_TIME pCfgBody;

				//显示信息
				NPC_F_LOG_SR_ShowInfo("[UMSP]  Set config: dev sys time.");

				if ( in_iMsgDataLen != (NPC_INT)(
					sizeof(NPC_S_DPS_MSG_HEAD) + 
					sizeof(NPC_S_DPS_MSG_BODY_EVENT_E3_SET_CONFIG) + 
					sizeof(NPC_S_PVM_IMD_CFG_DEV_SYS_TIME)) )
				{
					//显示信息
					NPC_F_LOG_SR_ShowInfo("[UMSP]  Set config: dev sys time fail, cause: in_iMsgDataLen error.");

					in_pMsgHead->ucResult = NPC_D_MON_CSD_DPS_ERROR_CODE_PARAM_ERROR;

					bRet = FALSE;
					break;
				}

				pCfgBody = (PNPC_S_PVM_IMD_CFG_DEV_SYS_TIME)((PNPC_BYTE)pMsgBody + sizeof(NPC_S_DPS_MSG_BODY_EVENT_E3_SET_CONFIG));

				//将NPC_S_TD_DATE_TIME转换为time_t时间
				in_pMcSrvData->tDevTime = NPC_F_SYS_TdTime_To_Timet(
					&pCfgBody->tDevTime		//TD时间
					);

				//---------------------------------------------------------------------------
				NPC_CHAR sTempLogBuf[256];
				NPC_CHAR sTempTimeStr[20];

				//将NPC_S_TD_DATE_TIME转换为时间字符串
				NPC_F_SYS_TdTime_To_TimeStr(
					&pCfgBody->tDevTime,		//TD时间
					sTempTimeStr				//时间字符串，格式为：HH:MM:SS
					);

				sprintf(sTempLogBuf, "[UMSP]  Set dev sys time: tDevTime: %s.", sTempTimeStr);
				NPC_F_LOG_SR_ShowInfo(sTempLogBuf);
			}
			break;
		case NPC_D_PVM_CFG_FUNCID_DEV_INFO:					//设备信息
			//设置配置参数中无此消息
			break;
		case NPC_D_PVM_CFG_FUNCID_CH_NAME:					//通道名称
			{
				PNPC_S_UMSP_CAMERA_DATA pCameraData;
				PNPC_S_PVM_IMD_CFG_CH_NAME pCfgBody;

				//显示信息
				NPC_F_LOG_SR_ShowInfo("[UMSP]  Set config: ch name.");

				if ( in_iMsgDataLen != (NPC_INT)(
					sizeof(NPC_S_DPS_MSG_HEAD) + 
					sizeof(NPC_S_DPS_MSG_BODY_EVENT_E3_SET_CONFIG) + 
					sizeof(NPC_S_PVM_IMD_CFG_CH_NAME)) )
				{
					//显示信息
					NPC_F_LOG_SR_ShowInfo("[UMSP]  Set config: ch name fail, cause: in_iMsgDataLen error.");

					in_pMsgHead->ucResult = NPC_D_MON_CSD_DPS_ERROR_CODE_PARAM_ERROR;

					bRet = FALSE;
					break;
				}

				pCfgBody = (PNPC_S_PVM_IMD_CFG_CH_NAME)((PNPC_BYTE)pMsgBody + sizeof(NPC_S_DPS_MSG_BODY_EVENT_E3_SET_CONFIG));

				if ( pCfgBody->iChNo < 0 || pCfgBody->iChNo >= NPC_D_DPS_MAX_CHANNEL_NUM )
				{
					//显示信息
					NPC_F_LOG_SR_ShowInfo("[UMSP]  Set config: ch name fail, cause: iChNo error.");

					in_pMsgHead->ucResult = NPC_D_MON_CSD_DPS_ERROR_CODE_CHANNEL_NO_ERROR;

					bRet = FALSE;
					break;
				}

				pCameraData = &in_pMcSrvData->tCameraTable[pCfgBody->iChNo];

				strcpy(pCameraData->sChName, pCfgBody->sChName);

				//---------------------------------------------------------------------------
				NPC_CHAR sTempLogBuf[256];

				sprintf(sTempLogBuf, "[UMSP]  Set ch name: sChName: %s.", pCfgBody->sChName);
				NPC_F_LOG_SR_ShowInfo(sTempLogBuf);
			}
			break;

		case NPC_D_PVM_CFG_FUNCID_CAMERA_PARAM:				//摄像机参数
			{
				PNPC_S_UMSP_CAMERA_DATA pCameraData;
				PNPC_S_PVM_IMD_CFG_CAMERA_PARAM pCfgBody;

				//显示信息
				NPC_F_LOG_SR_ShowInfo("[UMSP]  Set config: camera param.");

				if ( in_iMsgDataLen != (NPC_INT)(
					sizeof(NPC_S_DPS_MSG_HEAD) + 
					sizeof(NPC_S_DPS_MSG_BODY_EVENT_E3_SET_CONFIG) + 
					sizeof(NPC_S_PVM_IMD_CFG_CAMERA_PARAM)) )
				{
					//显示信息
					NPC_F_LOG_SR_ShowInfo("[UMSP]  Set config: camera param fail, cause: in_iMsgDataLen error.");

					in_pMsgHead->ucResult = NPC_D_MON_CSD_DPS_ERROR_CODE_PARAM_ERROR;

					bRet = FALSE;
					break;
				}

				pCfgBody = (PNPC_S_PVM_IMD_CFG_CAMERA_PARAM)((PNPC_BYTE)pMsgBody + sizeof(NPC_S_DPS_MSG_BODY_EVENT_E3_SET_CONFIG));

				if ( pCfgBody->iChNo < 0 || pCfgBody->iChNo >= NPC_D_DPS_MAX_CHANNEL_NUM )
				{
					//显示信息
					NPC_F_LOG_SR_ShowInfo("[UMSP]  Set config: camera param fail, cause: iChNo error.");

					in_pMsgHead->ucResult = NPC_D_MON_CSD_DPS_ERROR_CODE_CHANNEL_NO_ERROR;

					bRet = FALSE;
					break;
				}

				pCameraData = &in_pMcSrvData->tCameraTable[pCfgBody->iChNo];

				pCameraData->bIfPictureFlip = pCfgBody->bIfPictureFlip;

				//---------------------------------------------------------------------------
				NPC_CHAR sTempLogBuf[256];

				sprintf(sTempLogBuf, "[UMSP]  Set camera param: bIfPictureFlip: %d.", pCfgBody->bIfPictureFlip);
				NPC_F_LOG_SR_ShowInfo(sTempLogBuf);
			}
			break;

		case NPC_D_PVM_CFG_FUNCID_DEV_RECORD_PLAN:			//录像计划配置
			{
				PNPC_S_UMSP_CAMERA_DATA pCameraData;
				PNPC_S_PVM_IMD_CFG_DEV_RECORD_PLAN pCfgBody;

				//显示信息
				NPC_F_LOG_SR_ShowInfo("[UMSP]  Set config: dev record plan.");

				if ( in_iMsgDataLen != (NPC_INT)(
					sizeof(NPC_S_DPS_MSG_HEAD) + 
					sizeof(NPC_S_DPS_MSG_BODY_EVENT_E3_SET_CONFIG) + 
					sizeof(NPC_S_PVM_IMD_CFG_DEV_RECORD_PLAN)) )
				{
					//显示信息
					NPC_F_LOG_SR_ShowInfo("[UMSP]  Set config: dev record plan fail, cause: in_iMsgDataLen error.");

					in_pMsgHead->ucResult = NPC_D_MON_CSD_DPS_ERROR_CODE_PARAM_ERROR;

					bRet = FALSE;
					break;
				}

				pCfgBody = (PNPC_S_PVM_IMD_CFG_DEV_RECORD_PLAN)((PNPC_BYTE)pMsgBody + sizeof(NPC_S_DPS_MSG_BODY_EVENT_E3_SET_CONFIG));

				if ( pCfgBody->iChNo < 0 || pCfgBody->iChNo >= NPC_D_DPS_MAX_CHANNEL_NUM )
				{
					//显示信息
					NPC_F_LOG_SR_ShowInfo("[UMSP]  Set config: dev record plan fail, cause: iChNo error.");

					in_pMsgHead->ucResult = NPC_D_MON_CSD_DPS_ERROR_CODE_CHANNEL_NO_ERROR;

					bRet = FALSE;
					break;
				}

				pCameraData = &in_pMcSrvData->tCameraTable[pCfgBody->iChNo];

				pCameraData->tDevRecordPlan = *pCfgBody;

				//---------------------------------------------------------------------------
				NPC_CHAR sTempLogBuf[256];

				sprintf(sTempLogBuf, "[UMSP]  Set dev record plan: iSerialNo: %d.", pCfgBody->iChNo);
				NPC_F_LOG_SR_ShowInfo(sTempLogBuf);

				sprintf(sTempLogBuf, "[UMSP]  Set dev record plan: iRecState: %d.", pCfgBody->iRecState);
				NPC_F_LOG_SR_ShowInfo(sTempLogBuf);

				sprintf(sTempLogBuf, "[UMSP]  Set dev record plan: iPacketLength: %d.", pCfgBody->iPacketLength);
				NPC_F_LOG_SR_ShowInfo(sTempLogBuf);

				sprintf(sTempLogBuf, "[UMSP]  Set dev record plan: iPreRecord: %d.", pCfgBody->iPreRecord);
				NPC_F_LOG_SR_ShowInfo(sTempLogBuf);

				sprintf(sTempLogBuf, "[UMSP]  Set dev record plan: bOpenAudio: %d.", pCfgBody->bOpenAudio);
				NPC_F_LOG_SR_ShowInfo(sTempLogBuf);

				sprintf(sTempLogBuf, "[UMSP]  Set dev record plan: iRecStream: %d.", pCfgBody->iRecStream);
				NPC_F_LOG_SR_ShowInfo(sTempLogBuf);

				sprintf(sTempLogBuf, "[UMSP]  Set dev record plan: bOpenGeneral: %d.", pCfgBody->bOpenGeneral);
				NPC_F_LOG_SR_ShowInfo(sTempLogBuf);

				sprintf(sTempLogBuf, "[UMSP]  Set dev record plan: bOpenCheckAlarm: %d.", pCfgBody->bOpenCheckAlarm);
				NPC_F_LOG_SR_ShowInfo(sTempLogBuf);

				sprintf(sTempLogBuf, "[UMSP]  Set dev record plan: bOpenInputAlarm: %d.", pCfgBody->bOpenInputAlarm);
				NPC_F_LOG_SR_ShowInfo(sTempLogBuf);

				//if ( pCfgBody->iRecState == 2 )
				{
					for ( NPC_INT k=0; k<7; k++ )
					{
						for ( NPC_INT m=0; m<NPC_D_PVM_CFG_REC_PLAN_TIME_SECT_NUM; m++ )
						{
							sprintf(sTempLogBuf, "[UMSP]  Set dev record plan: Time sect: bOpenGeneral[%d][%d]: %d.", 
								k, m, pCfgBody->tWeekSect[k].tTimeSect[m].bOpenGeneral);
							NPC_F_LOG_SR_ShowInfo(sTempLogBuf);

							sprintf(sTempLogBuf, "[UMSP]  Set dev record plan: Time sect: bOpenCheckAlarm[%d][%d]: %d.", 
								k, m, pCfgBody->tWeekSect[k].tTimeSect[m].bOpenCheckAlarm);
							NPC_F_LOG_SR_ShowInfo(sTempLogBuf);

							sprintf(sTempLogBuf, "[UMSP]  Set dev record plan: Time sect: bOpenInputAlarm[%d][%d]: %d.", 
								k, m, pCfgBody->tWeekSect[k].tTimeSect[m].bOpenInputAlarm);
							NPC_F_LOG_SR_ShowInfo(sTempLogBuf);

							sprintf(sTempLogBuf, "[UMSP]  Set dev record plan: Time sect: tBeginTime[%d][%d]: %2.2d:%2.2d:%2.2d.", 
								k, m, pCfgBody->tWeekSect[k].tTimeSect[m].tBeginTime.ucHour, 
								pCfgBody->tWeekSect[k].tTimeSect[m].tBeginTime.ucMin, 
								pCfgBody->tWeekSect[k].tTimeSect[m].tBeginTime.ucSec);
							NPC_F_LOG_SR_ShowInfo(sTempLogBuf);

							sprintf(sTempLogBuf, "[UMSP]  Set dev record plan: Time sect: tEndTime[%d][%d]: %2.2d:%2.2d:%2.2d.", 
								k, m, pCfgBody->tWeekSect[k].tTimeSect[m].tEndTime.ucHour, 
								pCfgBody->tWeekSect[k].tTimeSect[m].tEndTime.ucMin, 
								pCfgBody->tWeekSect[k].tTimeSect[m].tEndTime.ucSec);
							NPC_F_LOG_SR_ShowInfo(sTempLogBuf);
						}
					}
				}
			}
			break;
		case NPC_D_PVM_CFG_FUNCID_ALARM_CLOUD_STORAGE:		//报警云存储配置
			{
				PNPC_S_UMSP_CAMERA_DATA pCameraData;
				PNPC_S_PVM_IMD_CFG_ALARM_CLOUD_STORAGE pCfgBody;

				//显示信息
				NPC_F_LOG_SR_ShowInfo("[UMSP]  Set config: alarm cloud storage.");

				if ( in_iMsgDataLen != (NPC_INT)(
					sizeof(NPC_S_DPS_MSG_HEAD) + 
					sizeof(NPC_S_DPS_MSG_BODY_EVENT_E3_SET_CONFIG) + 
					sizeof(NPC_S_PVM_IMD_CFG_ALARM_CLOUD_STORAGE)) )
				{
					//显示信息
					NPC_F_LOG_SR_ShowInfo("[UMSP]  Set config: alarm cloud storage fail, cause: in_iMsgDataLen error.");

					in_pMsgHead->ucResult = NPC_D_MON_CSD_DPS_ERROR_CODE_PARAM_ERROR;

					bRet = FALSE;
					break;
				}

				pCfgBody = (PNPC_S_PVM_IMD_CFG_ALARM_CLOUD_STORAGE)((PNPC_BYTE)pMsgBody + sizeof(NPC_S_DPS_MSG_BODY_EVENT_E3_SET_CONFIG));

				if ( pCfgBody->iChNo < 0 || pCfgBody->iChNo >= NPC_D_DPS_MAX_CHANNEL_NUM )
				{
					//显示信息
					NPC_F_LOG_SR_ShowInfo("[UMSP]  Set config: alarm cloud storage fail, cause: iChNo error.");

					in_pMsgHead->ucResult = NPC_D_MON_CSD_DPS_ERROR_CODE_CHANNEL_NO_ERROR;

					bRet = FALSE;
					break;
				}

				pCameraData = &in_pMcSrvData->tCameraTable[pCfgBody->iChNo];

				pCameraData->tAlarmCloudStorage = *pCfgBody;
			}
			break;

		case NPC_D_PVM_CFG_FUNCID_STORAGE_QUERY_SIZE:		//查询存储容量
			//设置配置参数中无此消息
			break;
		case NPC_D_PVM_CFG_FUNCID_STORAGE_FORMAT:			//格式化存储器
			{
				PNPC_S_PVM_IMD_CFG_STORAGE_FORMAT pCfgBody;

				//显示信息
				NPC_F_LOG_SR_ShowInfo("[UMSP]  Set config: storage format.");

				if ( in_iMsgDataLen != (NPC_INT)(
					sizeof(NPC_S_DPS_MSG_HEAD) + 
					sizeof(NPC_S_DPS_MSG_BODY_EVENT_E3_SET_CONFIG) + 
					sizeof(NPC_S_PVM_IMD_CFG_STORAGE_FORMAT)) )
				{
					//显示信息
					NPC_F_LOG_SR_ShowInfo("[UMSP]  Set config: storage format fail, cause: in_iMsgDataLen error.");

					in_pMsgHead->ucResult = NPC_D_MON_CSD_DPS_ERROR_CODE_PARAM_ERROR;

					bRet = FALSE;
					break;
				}

				pCfgBody = (PNPC_S_PVM_IMD_CFG_STORAGE_FORMAT)((PNPC_BYTE)pMsgBody + sizeof(NPC_S_DPS_MSG_BODY_EVENT_E3_SET_CONFIG));

				//---------------------------------------------------------------------------
				NPC_CHAR sTempLogBuf[256];

				//显示信息
				sprintf(sTempLogBuf, "[UMSP]  Format storage: iSerialNo: %d.", pCfgBody->iSerialNo);
				NPC_F_LOG_SR_ShowInfo(sTempLogBuf);
			}
			break;

		default:
			{
				//显示信息
				NPC_CHAR sTempLogBuf[128];
				sprintf(sTempLogBuf, "[UMSP]  Set config: error func id: %d.", pMsgBody->i_iCfgFuncId);
				NPC_F_LOG_SR_ShowInfo(sTempLogBuf);

				in_pMsgHead->ucResult = NPC_D_MON_CSD_DPS_ERROR_CODE_ID_ERROR;
				bRet = FALSE;
			}
			break;
		}
	}

	//CLEAR AREA
	{
		//发送消息给设备协议网络服务
		NPC_F_DPS_SendDevProData(
			in_pMcSrvData->hUmspProSrv,			//设备协议网络服务句柄
			in_pMsgDataBuf,						//设备协议数据缓冲区，包括NPC_S_DPS_MSG_HEAD
			in_iMsgDataLen						//设备协议数据长度
			);
	}
}

//处理协议事件：获取缺省配置参数
NPC_VOID	NPC_F_UMSP_MC_DoProEvent_EVENT_E3_GET_DEF_CONFIG(
	IN		PNPC_S_UMSP_MCSRV_DATA					in_pMcSrvData,				//主控服务数据
	IN		PNPC_S_DPS_MSG_HEAD						in_pMsgHead,				//消息头
	IN		PNPC_BYTE								in_pMsgDataBuf,				//消息数据缓冲区，包括NPC_S_DPS_MSG_HEAD
	IN		NPC_INT									in_iMsgDataLen				//消息数据长度
	)
{
	NPC_BOOL										bRet;
	PNPC_S_DPS_MSG_BODY_EVENT_E3_GET_DEF_CONFIG		pMsgBody;

	//INIT AREA
	{
		bRet = TRUE;
		pMsgBody = (PNPC_S_DPS_MSG_BODY_EVENT_E3_GET_DEF_CONFIG)((PNPC_BYTE)in_pMsgHead + sizeof(NPC_S_DPS_MSG_HEAD));

		in_pMsgHead->ucResult = 0;
		in_pMsgHead->ucMsgType = NPC_D_DPS_MSG_TYPE_RESPONSION;
	}

	//CODE AREA
	{
		if ( in_iMsgDataLen != (NPC_INT)(
			sizeof(NPC_S_DPS_MSG_HEAD) + 
			sizeof(NPC_S_DPS_MSG_BODY_EVENT_E3_GET_DEF_CONFIG) ) )
		{
			//显示信息
			NPC_F_LOG_SR_ShowInfo("[UMSP]  Get default config: dev codec fail, cause: in_iMsgDataLen error.");

			in_pMsgHead->ucResult = NPC_D_MON_CSD_DPS_ERROR_CODE_PARAM_ERROR;

			bRet = FALSE;
			goto _NPC_CLEAR;
		}

		switch ( pMsgBody->i_iCfgFuncId )
		{
		case NPC_D_PVM_CFG_FUNCID_DEV_NAME:					//设备名称
			//显示信息
			NPC_F_LOG_SR_ShowInfo("[UMSP]  Get default config: dev name.");

			in_pMsgHead->ucResult = NPC_D_MON_CSD_DPS_ERROR_CODE_NOT_SUPPORT_OP;
			bRet = FALSE;
			break;
		case NPC_D_PVM_CFG_FUNCID_DEV_IP:					//设备有线IP地址
			//显示信息
			NPC_F_LOG_SR_ShowInfo("[UMSP]  Get default config: dev ip.");

			in_pMsgHead->ucResult = NPC_D_MON_CSD_DPS_ERROR_CODE_NOT_SUPPORT_OP;
			bRet = FALSE;
			break;
		case NPC_D_PVM_CFG_FUNCID_DEV_WIFI:					//设备WIFI信息
			//显示信息
			NPC_F_LOG_SR_ShowInfo("[UMSP]  Get default config: dev wifi.");

			in_pMsgHead->ucResult = NPC_D_MON_CSD_DPS_ERROR_CODE_NOT_SUPPORT_OP;
			bRet = FALSE;
			break;
		case NPC_D_PVM_CFG_FUNCID_GET_WIFIAP_LIST:			//获取WIFIAP列表
			//显示信息
			NPC_F_LOG_SR_ShowInfo("[UMSP]  Get default config: get wifiap list.");

			in_pMsgHead->ucResult = NPC_D_MON_CSD_DPS_ERROR_CODE_NOT_SUPPORT_OP;
			bRet = FALSE;
			break;
		case NPC_D_PVM_CFG_FUNCID_DEV_CODEC:				//编码配置
			{
				struct _PacketData{
					NPC_S_DPS_MSG_HEAD								tMsgHead;
					NPC_S_DPS_MSG_BODY_EVENT_E3_GET_DEF_CONFIG		tMsgBody;
					NPC_S_PVM_IMD_CFG_DEV_CODEC						tCfgBody;
				} tPacketData = {0};

				//显示信息
				NPC_F_LOG_SR_ShowInfo("[UMSP]  Get default config: dev codec.");

				memcpy(&tPacketData, in_pMsgDataBuf, in_iMsgDataLen);

				if ( tPacketData.tMsgBody.i_iChNo < 0 || 
					tPacketData.tMsgBody.i_iChNo >= NPC_D_DPS_MAX_CHANNEL_NUM || 
					tPacketData.tMsgBody.i_iChNo >= in_pMcSrvData->iChNum )
				{
					//显示信息
					NPC_F_LOG_SR_ShowInfo("[UMSP]  Get default config: dev codec fail, cause: iChNo error.");

					in_pMsgHead->ucResult = NPC_D_MON_CSD_DPS_ERROR_CODE_CHANNEL_NO_ERROR;

					bRet = FALSE;
					goto _NPC_CLEAR;
				}

				tPacketData.tMsgHead.iMsgBodyLen = sizeof(tPacketData) - sizeof(NPC_S_DPS_MSG_HEAD);
				tPacketData.tMsgHead.ucMsgType = NPC_D_DPS_MSG_TYPE_RESPONSION;

				tPacketData.tMsgBody.io_iCfgDataLen = sizeof(tPacketData) - 
					sizeof(NPC_S_DPS_MSG_HEAD) - sizeof(NPC_S_DPS_MSG_BODY_EVENT_E3_GET_DEF_CONFIG);

				//--------------------------------------------------------------------------------
				//缺省参数为模拟参数，仅为演示使用
				tPacketData.tCfgBody.iCodecNum = 2;

				//主码流
				tPacketData.tCfgBody.tCodecInfo[0].bVideoEnable = TRUE;					//是否启用视频
				tPacketData.tCfgBody.tCodecInfo[0].bAudioEnable = FALSE;					//是否启用音频

				tPacketData.tCfgBody.tCodecInfo[0].tVideoFormat.iCodecId = NPC_D_MON_CSD_CODEC_ID_VEDIO_H264;			//视频编码ID，参见NPC_D_MON_CSD_CODEC_ID_VEDIO_*定义
				tPacketData.tCfgBody.tCodecInfo[0].tVideoFormat.iResolution = NPC_D_MON_CSD_VEDIO_RESOLUTION_D1;		//分辨率，参见NPC_D_MON_CSD_VEDIO_RESOLUTION_*定义
				tPacketData.tCfgBody.tCodecInfo[0].tVideoFormat.iBitRateControl = NPC_D_MON_CSD_BITRATECTRL_CBR;		//码流控制模式，参见NPC_D_MON_CSD_BITRATECTRL_*定义，如："CBR"固定码流，"VBR"可变码流，"MBR"混和码流
				tPacketData.tCfgBody.tCodecInfo[0].tVideoFormat.iBitRate = 1024;										//视频码流（kbps）
				tPacketData.tCfgBody.tCodecInfo[0].tVideoFormat.iFrameRate = 25;										//视频帧率，负数表示多秒一帧，比如-3表示3秒一帧，取值范围-5~30
				tPacketData.tCfgBody.tCodecInfo[0].tVideoFormat.iGOP = 50;												//I帧间隔
				tPacketData.tCfgBody.tCodecInfo[0].tVideoFormat.iQuality = 3;											//图像质量，只有在可变码流下才起作用，取值1-6，值越大，图像质量越好

				tPacketData.tCfgBody.tCodecInfo[0].tAudioFormat.iCodecId = NPC_D_MON_CSD_CODEC_ID_AUDIO_PCMA;			//音频编码ID，参见NPC_D_MON_CSD_CODEC_ID_AUDIO_*定义
				tPacketData.tCfgBody.tCodecInfo[0].tAudioFormat.iBitRate = 64;											//比特率（kbps）
				tPacketData.tCfgBody.tCodecInfo[0].tAudioFormat.iSampleRate = 8000;									//采样率
				tPacketData.tCfgBody.tCodecInfo[0].tAudioFormat.iMaxVolume = 6;										//最大音量

				//子码流
				tPacketData.tCfgBody.tCodecInfo[1].bVideoEnable = TRUE;						//是否启用视频
				tPacketData.tCfgBody.tCodecInfo[1].bAudioEnable = FALSE;					//是否启用音频

				tPacketData.tCfgBody.tCodecInfo[1].tVideoFormat.iCodecId = NPC_D_MON_CSD_CODEC_ID_VEDIO_H264;			//视频编码ID，参见NPC_D_MON_CSD_CODEC_ID_VEDIO_*定义
				tPacketData.tCfgBody.tCodecInfo[1].tVideoFormat.iResolution = NPC_D_MON_CSD_VEDIO_RESOLUTION_CIF;		//分辨率，参见NPC_D_MON_CSD_VEDIO_RESOLUTION_*定义
				tPacketData.tCfgBody.tCodecInfo[1].tVideoFormat.iBitRateControl = NPC_D_MON_CSD_BITRATECTRL_CBR;		//码流控制模式，参见NPC_D_MON_CSD_BITRATECTRL_*定义，如："CBR"固定码流，"VBR"可变码流，"MBR"混和码流
				tPacketData.tCfgBody.tCodecInfo[1].tVideoFormat.iBitRate = 256;										//视频码流（kbps）
				tPacketData.tCfgBody.tCodecInfo[1].tVideoFormat.iFrameRate = 12;										//视频帧率，负数表示多秒一帧，比如-3表示3秒一帧，取值范围-5~30
				tPacketData.tCfgBody.tCodecInfo[1].tVideoFormat.iGOP = 50;												//I帧间隔
				tPacketData.tCfgBody.tCodecInfo[1].tVideoFormat.iQuality = 3;											//图像质量，只有在可变码流下才起作用，取值1-6，值越大，图像质量越好

				tPacketData.tCfgBody.tCodecInfo[1].tAudioFormat.iCodecId = NPC_D_MON_CSD_CODEC_ID_AUDIO_PCMA;			//音频编码ID，参见NPC_D_MON_CSD_CODEC_ID_AUDIO_*定义
				tPacketData.tCfgBody.tCodecInfo[1].tAudioFormat.iBitRate = 64;											//比特率（kbps）
				tPacketData.tCfgBody.tCodecInfo[1].tAudioFormat.iSampleRate = 8000;									//采样率
				tPacketData.tCfgBody.tCodecInfo[1].tAudioFormat.iMaxVolume = 6;										//最大音量

				//-------------------------------------------------------------------------------
				//发送消息给设备协议网络服务
				NPC_F_DPS_SendDevProData(
					in_pMcSrvData->hUmspProSrv,		//设备协议网络服务句柄
					(PNPC_BYTE)&tPacketData,		//设备协议数据缓冲区，包括NPC_S_DPS_MSG_HEAD
					sizeof(tPacketData)				//设备协议数据长度
					);
			}
			break;

		case NPC_D_PVM_CFG_FUNCID_DEV_ALARM_MOTION_DETECT:	//移动侦测报警参数
			{
				struct _PacketData{
					NPC_S_DPS_MSG_HEAD								tMsgHead;
					NPC_S_DPS_MSG_BODY_EVENT_E3_GET_DEF_CONFIG		tMsgBody;
					NPC_S_PVM_IMD_CFG_DEV_ALARM_MOTION_DETECT		tCfgBody;
				} tPacketData = {0};

				//显示信息
				NPC_F_LOG_SR_ShowInfo("[UMSP]  Get default config: dev alarm motion detect.");

				memcpy(&tPacketData, in_pMsgDataBuf, in_iMsgDataLen);

				if ( tPacketData.tMsgBody.i_iChNo < 0 || 
					tPacketData.tMsgBody.i_iChNo >= NPC_D_DPS_MAX_CHANNEL_NUM || 
					tPacketData.tMsgBody.i_iChNo >= in_pMcSrvData->iChNum )
				{
					//显示信息
					NPC_F_LOG_SR_ShowInfo("[UMSP]  Get default config: dev alarm motion detect fail, cause: iChNo error.");

					in_pMsgHead->ucResult = NPC_D_MON_CSD_DPS_ERROR_CODE_CHANNEL_NO_ERROR;

					bRet = FALSE;
					goto _NPC_CLEAR;
				}

				tPacketData.tMsgHead.iMsgBodyLen = sizeof(tPacketData) - sizeof(NPC_S_DPS_MSG_HEAD);
				tPacketData.tMsgHead.ucMsgType = NPC_D_DPS_MSG_TYPE_RESPONSION;

				tPacketData.tMsgBody.io_iCfgDataLen = sizeof(tPacketData) - 
					sizeof(NPC_S_DPS_MSG_HEAD) - sizeof(NPC_S_DPS_MSG_BODY_EVENT_E3_GET_DEF_CONFIG);

				//--------------------------------------------------------------------------------
				//缺省参数为模拟参数，仅为演示使用
				tPacketData.tCfgBody.bIfEnable = FALSE;						//是否开启检测

				tPacketData.tCfgBody.bFuncEnable_Level = FALSE;				//功能是否有效：灵敏度，TRUE：灵敏度参数项有效，FALSE：为无效（下同）
				tPacketData.tCfgBody.bFuncEnable_Region = FALSE;				//功能是否有效：检测区域块
				tPacketData.tCfgBody.bFuncEnable_Photo = FALSE;				//功能是否有效：拍照
				tPacketData.tCfgBody.bFuncEnable_Record = FALSE;				//功能是否有效：录像
				tPacketData.tCfgBody.bFuncEnable_AlarmOut = FALSE;				//功能是否有效：报警输出
				tPacketData.tCfgBody.bFuncEnable_Beep = FALSE;					//功能是否有效：蜂鸣
				tPacketData.tCfgBody.bFuncEnable_WeekSect = FALSE;				//功能是否有效：检测时间段

				tPacketData.tCfgBody.iLevel = 5;								//灵敏度
				//tPacketData.tCfgBody.dwRegion[32];							//检测区域块，每一位为一个点（0表示不检测，1表示要检测，左边的点在低位），每个32位为一行（即一个双字），共32行，即32*32的矩形，可以根据实际图像比例选择部分点处理
				tPacketData.tCfgBody.bPhotoEnable = FALSE;						//拍照使能
				tPacketData.tCfgBody.bRecordEnable = FALSE;					//录像使能
				tPacketData.tCfgBody.iRecordDelayedTime = 5;					//录像延时时间（秒）
				tPacketData.tCfgBody.bAlarmOutEnable = FALSE;					//报警输出使能
				tPacketData.tCfgBody.iAlarmOutDelayedTime = 5;					//报警输出延时时间（秒）
				tPacketData.tCfgBody.bBeepEnable = FALSE;						//蜂鸣使能

				//检测时间段
				/*struct _WeekSect{
					struct _TimeSect{
						NPC_BOOL							bIfEnable;						//是否启用
						NPC_S_TD_TIME						tBeginTime;						//开始时间
						NPC_S_TD_TIME						tEndTime;						//结束时间
					} tTimeSect[NPC_D_PVM_CFG_CHECK_TIME_SECT_NUM];
				} tWeekSect[7];*/

				//-------------------------------------------------------------------------------
				//发送消息给设备协议网络服务
				NPC_F_DPS_SendDevProData(
					in_pMcSrvData->hUmspProSrv,		//设备协议网络服务句柄
					(PNPC_BYTE)&tPacketData,		//设备协议数据缓冲区，包括NPC_S_DPS_MSG_HEAD
					sizeof(tPacketData)				//设备协议数据长度
					);
			}
			break;
		case NPC_D_PVM_CFG_FUNCID_DEV_ALARM_PROBE:			//探头输入报警参数
			{
				struct _PacketData{
					NPC_S_DPS_MSG_HEAD								tMsgHead;
					NPC_S_DPS_MSG_BODY_EVENT_E3_GET_DEF_CONFIG		tMsgBody;
					NPC_S_PVM_IMD_CFG_DEV_ALARM_PROBE				tCfgBody;
				} tPacketData = {0};

				//显示信息
				NPC_F_LOG_SR_ShowInfo("[UMSP]  Get default config: dev alarm probe.");

				memcpy(&tPacketData, in_pMsgDataBuf, in_iMsgDataLen);

				if ( tPacketData.tMsgBody.i_iChNo < 0 || 
					tPacketData.tMsgBody.i_iChNo >= NPC_D_DPS_MAX_CHANNEL_NUM || 
					tPacketData.tMsgBody.i_iChNo >= in_pMcSrvData->iChNum )
				{
					//显示信息
					NPC_F_LOG_SR_ShowInfo("[UMSP]  Get default config: dev alarm probe fail, cause: iChNo error.");

					in_pMsgHead->ucResult = NPC_D_MON_CSD_DPS_ERROR_CODE_CHANNEL_NO_ERROR;

					bRet = FALSE;
					goto _NPC_CLEAR;
				}

				tPacketData.tMsgHead.iMsgBodyLen = sizeof(tPacketData) - sizeof(NPC_S_DPS_MSG_HEAD);
				tPacketData.tMsgHead.ucMsgType = NPC_D_DPS_MSG_TYPE_RESPONSION;

				tPacketData.tMsgBody.io_iCfgDataLen = sizeof(tPacketData) - 
					sizeof(NPC_S_DPS_MSG_HEAD) - sizeof(NPC_S_DPS_MSG_BODY_EVENT_E3_GET_DEF_CONFIG);

				//--------------------------------------------------------------------------------
				//缺省参数为模拟参数，仅为演示使用
				tPacketData.tCfgBody.bIfEnable = FALSE;							//是否开启检测

				tPacketData.tCfgBody.bFuncEnable_SensorType = FALSE;			//功能是否有效：传感器类型，TRUE：传感器类型参数项有效，FALSE：为无效（下同）
				tPacketData.tCfgBody.bFuncEnable_Photo = FALSE;					//功能是否有效：拍照
				tPacketData.tCfgBody.bFuncEnable_Record = FALSE;				//功能是否有效：录像
				tPacketData.tCfgBody.bFuncEnable_AlarmOut = FALSE;				//功能是否有效：报警输出
				tPacketData.tCfgBody.bFuncEnable_Beep = FALSE;					//功能是否有效：蜂鸣
				tPacketData.tCfgBody.bFuncEnable_WeekSect = FALSE;				//功能是否有效：检测时间段

				tPacketData.tCfgBody.iSensorType = 0;							//传感器类型，0：常闭，1：常开
				tPacketData.tCfgBody.bPhotoEnable = FALSE;						//拍照使能
				tPacketData.tCfgBody.bRecordEnable = FALSE;						//录像使能
				tPacketData.tCfgBody.iRecordDelayedTime = 5;					//录像延时时间（秒）
				tPacketData.tCfgBody.bAlarmOutEnable = FALSE;					//报警输出使能
				tPacketData.tCfgBody.iAlarmOutDelayedTime = 5;					//报警输出延时时间（秒）
				tPacketData.tCfgBody.bBeepEnable = FALSE;						//蜂鸣使能

				//检测时间段
				/*struct _WeekSect{
					struct _TimeSect{
						NPC_BOOL							bIfEnable;						//是否启用
						NPC_S_TD_TIME						tBeginTime;						//开始时间
						NPC_S_TD_TIME						tEndTime;						//结束时间
					} tTimeSect[NPC_D_PVM_CFG_CHECK_TIME_SECT_NUM];
				} tWeekSect[7];*/

				//-------------------------------------------------------------------------------
				//发送消息给设备协议网络服务
				NPC_F_DPS_SendDevProData(
					in_pMcSrvData->hUmspProSrv,		//设备协议网络服务句柄
					(PNPC_BYTE)&tPacketData,		//设备协议数据缓冲区，包括NPC_S_DPS_MSG_HEAD
					sizeof(tPacketData)				//设备协议数据长度
					);
			}
			break;

		case NPC_D_PVM_CFG_FUNCID_DEV_SYS_TIME:				//设备系统时间
			//显示信息
			NPC_F_LOG_SR_ShowInfo("[UMSP]  Get default config: dev sys time.");

			in_pMsgHead->ucResult = NPC_D_MON_CSD_DPS_ERROR_CODE_NOT_SUPPORT_OP;
			bRet = FALSE;
			break;
		case NPC_D_PVM_CFG_FUNCID_DEV_INFO:					//设备信息
			//不处理该消息，客户端通过登录时获取设备信息
			break;
		case NPC_D_PVM_CFG_FUNCID_CH_NAME:					//通道名称
			{
				struct _PacketData{
					NPC_S_DPS_MSG_HEAD								tMsgHead;
					NPC_S_DPS_MSG_BODY_EVENT_E3_GET_DEF_CONFIG		tMsgBody;
					NPC_S_PVM_IMD_CFG_CH_NAME						tCfgBody;
				} tPacketData = {0};

				//显示信息
				NPC_F_LOG_SR_ShowInfo("[UMSP]  Get default config: ch name.");

				memcpy(&tPacketData, in_pMsgDataBuf, in_iMsgDataLen);

				if ( tPacketData.tMsgBody.i_iChNo < 0 || 
					tPacketData.tMsgBody.i_iChNo >= NPC_D_DPS_MAX_CHANNEL_NUM || 
					tPacketData.tMsgBody.i_iChNo >= in_pMcSrvData->iChNum )
				{
					//显示信息
					NPC_F_LOG_SR_ShowInfo("[UMSP]  Get default config: ch name fail, cause: iChNo error.");

					in_pMsgHead->ucResult = NPC_D_MON_CSD_DPS_ERROR_CODE_CHANNEL_NO_ERROR;

					bRet = FALSE;
					goto _NPC_CLEAR;
				}

				tPacketData.tMsgHead.iMsgBodyLen = sizeof(tPacketData) - sizeof(NPC_S_DPS_MSG_HEAD);
				tPacketData.tMsgHead.ucMsgType = NPC_D_DPS_MSG_TYPE_RESPONSION;

				tPacketData.tMsgBody.io_iCfgDataLen = sizeof(tPacketData) - 
					sizeof(NPC_S_DPS_MSG_HEAD) - sizeof(NPC_S_DPS_MSG_BODY_EVENT_E3_GET_DEF_CONFIG);

				//--------------------------------------------------------------------------------
				//缺省参数为模拟参数，仅为演示使用
				sprintf(tPacketData.tCfgBody.sChName, "CH %2.2d", tPacketData.tMsgBody.i_iChNo);

				//-------------------------------------------------------------------------------
				//发送消息给设备协议网络服务
				NPC_F_DPS_SendDevProData(
					in_pMcSrvData->hUmspProSrv,		//设备协议网络服务句柄
					(PNPC_BYTE)&tPacketData,		//设备协议数据缓冲区，包括NPC_S_DPS_MSG_HEAD
					sizeof(tPacketData)				//设备协议数据长度
					);
			}
			break;

		case NPC_D_PVM_CFG_FUNCID_CAMERA_PARAM:				//摄像机参数
			{
				struct _PacketData{
					NPC_S_DPS_MSG_HEAD								tMsgHead;
					NPC_S_DPS_MSG_BODY_EVENT_E3_GET_DEF_CONFIG		tMsgBody;
					NPC_S_PVM_IMD_CFG_CAMERA_PARAM					tCfgBody;
				} tPacketData = {0};

				//显示信息
				NPC_F_LOG_SR_ShowInfo("[UMSP]  Get default config: camera param.");

				memcpy(&tPacketData, in_pMsgDataBuf, in_iMsgDataLen);

				if ( tPacketData.tMsgBody.i_iChNo < 0 || 
					tPacketData.tMsgBody.i_iChNo >= NPC_D_DPS_MAX_CHANNEL_NUM || 
					tPacketData.tMsgBody.i_iChNo >= in_pMcSrvData->iChNum )
				{
					//显示信息
					NPC_F_LOG_SR_ShowInfo("[UMSP]  Get default config: camera param fail, cause: iChNo error.");

					in_pMsgHead->ucResult = NPC_D_MON_CSD_DPS_ERROR_CODE_CHANNEL_NO_ERROR;

					bRet = FALSE;
					goto _NPC_CLEAR;
				}

				tPacketData.tMsgHead.iMsgBodyLen = sizeof(tPacketData) - sizeof(NPC_S_DPS_MSG_HEAD);
				tPacketData.tMsgHead.ucMsgType = NPC_D_DPS_MSG_TYPE_RESPONSION;

				tPacketData.tMsgBody.io_iCfgDataLen = sizeof(tPacketData) - 
					sizeof(NPC_S_DPS_MSG_HEAD) - sizeof(NPC_S_DPS_MSG_BODY_EVENT_E3_GET_DEF_CONFIG);

				//--------------------------------------------------------------------------------
				//缺省参数为模拟参数，仅为演示使用
				tPacketData.tCfgBody.bIfPictureFlip = FALSE;

				//-------------------------------------------------------------------------------
				//发送消息给设备协议网络服务
				NPC_F_DPS_SendDevProData(
					in_pMcSrvData->hUmspProSrv,		//设备协议网络服务句柄
					(PNPC_BYTE)&tPacketData,		//设备协议数据缓冲区，包括NPC_S_DPS_MSG_HEAD
					sizeof(tPacketData)				//设备协议数据长度
					);
			}
			break;

		case NPC_D_PVM_CFG_FUNCID_DEV_RECORD_PLAN:			//录像计划配置
			{
				struct _PacketData{
					NPC_S_DPS_MSG_HEAD								tMsgHead;
					NPC_S_DPS_MSG_BODY_EVENT_E3_GET_DEF_CONFIG		tMsgBody;
					NPC_S_PVM_IMD_CFG_DEV_RECORD_PLAN				tCfgBody;
				} tPacketData = {0};

				//显示信息
				NPC_F_LOG_SR_ShowInfo("[UMSP]  Get default config: dev record plan.");

				memcpy(&tPacketData, in_pMsgDataBuf, in_iMsgDataLen);

				if ( tPacketData.tMsgBody.i_iChNo < 0 || 
					tPacketData.tMsgBody.i_iChNo >= NPC_D_DPS_MAX_CHANNEL_NUM || 
					tPacketData.tMsgBody.i_iChNo >= in_pMcSrvData->iChNum )
				{
					//显示信息
					NPC_F_LOG_SR_ShowInfo("[UMSP]  Get default config: dev record plan fail, cause: iChNo error.");

					in_pMsgHead->ucResult = NPC_D_MON_CSD_DPS_ERROR_CODE_CHANNEL_NO_ERROR;

					bRet = FALSE;
					goto _NPC_CLEAR;
				}

				tPacketData.tMsgHead.iMsgBodyLen = sizeof(tPacketData) - sizeof(NPC_S_DPS_MSG_HEAD);
				tPacketData.tMsgHead.ucMsgType = NPC_D_DPS_MSG_TYPE_RESPONSION;

				tPacketData.tMsgBody.io_iCfgDataLen = sizeof(tPacketData) - 
					sizeof(NPC_S_DPS_MSG_HEAD) - sizeof(NPC_S_DPS_MSG_BODY_EVENT_E3_GET_DEF_CONFIG);

				//--------------------------------------------------------------------------------
				tPacketData.tCfgBody.iRecState = 0;							//录像状态，0：关闭，1：始终录像，2：配置录像

				tPacketData.tCfgBody.iPacketLength = 5;						//每个录像文件的录像时长，以分钟为单位
				tPacketData.tCfgBody.iPreRecord = 5;						//预录时长（秒）
				tPacketData.tCfgBody.bOpenAudio = FALSE;					//是否开启音频录像
				tPacketData.tCfgBody.iRecStream = 0;						//录像码流，0：主码流，1：子码流

				//以下三个参数在录像状态为始终录像时有效
				tPacketData.tCfgBody.bOpenGeneral = FALSE;					//是否开启常规录像（定时录像）
				tPacketData.tCfgBody.bOpenCheckAlarm = FALSE;				//是否开启检测报警录像（移动报警、视频遮挡、视频丢失等）
				tPacketData.tCfgBody.bOpenInputAlarm = FALSE;				//是否开启输入报警录像（指探头，如红外等）

				//-------------------------------------------------------------------------------
				//发送消息给设备协议网络服务
				NPC_F_DPS_SendDevProData(
					in_pMcSrvData->hUmspProSrv,		//设备协议网络服务句柄
					(PNPC_BYTE)&tPacketData,		//设备协议数据缓冲区，包括NPC_S_DPS_MSG_HEAD
					sizeof(tPacketData)				//设备协议数据长度
					);
			}
			break;
		case NPC_D_PVM_CFG_FUNCID_ALARM_CLOUD_STORAGE:		//报警云存储配置
			{
				struct _PacketData{
					NPC_S_DPS_MSG_HEAD								tMsgHead;
					NPC_S_DPS_MSG_BODY_EVENT_E3_GET_DEF_CONFIG		tMsgBody;
					NPC_S_PVM_IMD_CFG_ALARM_CLOUD_STORAGE			tCfgBody;
				} tPacketData = {0};

				//显示信息
				NPC_F_LOG_SR_ShowInfo("[UMSP]  Get default config: alarm cloud storage.");

				memcpy(&tPacketData, in_pMsgDataBuf, in_iMsgDataLen);

				tPacketData.tMsgHead.iMsgBodyLen = sizeof(tPacketData) - sizeof(NPC_S_DPS_MSG_HEAD);
				tPacketData.tMsgHead.ucMsgType = NPC_D_DPS_MSG_TYPE_RESPONSION;

				tPacketData.tMsgBody.io_iCfgDataLen = sizeof(tPacketData) - 
					sizeof(NPC_S_DPS_MSG_HEAD) - sizeof(NPC_S_DPS_MSG_BODY_EVENT_E3_GET_DEF_CONFIG);

				//--------------------------------------------------------------------------------
				tPacketData.tCfgBody.bOpenAlarmRecord = FALSE;				//是否开启报警录像
				tPacketData.tCfgBody.iPreRecordTime = 5;					//录像预录时长（秒），指录制报警产生之前的N秒的录像
				tPacketData.tCfgBody.iTotalRecordTime = 30;					//录像时长（秒）

				tPacketData.tCfgBody.bOpenAlarmPhotos = FALSE;				//是否开启报警拍照
				tPacketData.tCfgBody.iPhotosIntervalTime = 1;				//拍照间隔时间（毫秒）
				tPacketData.tCfgBody.iPhotosPicNum = 10;					//拍照张数

				//-------------------------------------------------------------------------------
				//发送消息给设备协议网络服务
				NPC_F_DPS_SendDevProData(
					in_pMcSrvData->hUmspProSrv,		//设备协议网络服务句柄
					(PNPC_BYTE)&tPacketData,		//设备协议数据缓冲区，包括NPC_S_DPS_MSG_HEAD
					sizeof(tPacketData)				//设备协议数据长度
					);
			}
			break;

		case NPC_D_PVM_CFG_FUNCID_STORAGE_QUERY_SIZE:		//查询存储容量
			{
				struct _PacketData{
					NPC_S_DPS_MSG_HEAD								tMsgHead;
					NPC_S_DPS_MSG_BODY_EVENT_E3_GET_DEF_CONFIG		tMsgBody;
					NPC_S_PVM_IMD_CFG_STORAGE_QUERY_SIZE			tCfgBody;
				} tPacketData = {0};

				//显示信息
				NPC_F_LOG_SR_ShowInfo("[UMSP]  Get default config: storage query size.");

				memcpy(&tPacketData, in_pMsgDataBuf, in_iMsgDataLen);

				tPacketData.tMsgHead.iMsgBodyLen = sizeof(tPacketData) - sizeof(NPC_S_DPS_MSG_HEAD);
				tPacketData.tMsgHead.ucMsgType = NPC_D_DPS_MSG_TYPE_RESPONSION;

				tPacketData.tMsgBody.io_iCfgDataLen = sizeof(tPacketData) - 
					sizeof(NPC_S_DPS_MSG_HEAD) - sizeof(NPC_S_DPS_MSG_BODY_EVENT_E3_GET_DEF_CONFIG);

				//--------------------------------------------------------------------------------
				tPacketData.tCfgBody.iStorageNum = 1;

				tPacketData.tCfgBody.tStorage[0].iSerialNo = 0;
				tPacketData.tCfgBody.tStorage[0].dwStorageSize = 200;
				tPacketData.tCfgBody.tStorage[0].dwRemainSize = 100;

				//--------------------------------------------------------------------------------
				//发送消息给设备协议网络服务
				NPC_F_DPS_SendDevProData(
					in_pMcSrvData->hUmspProSrv,			//设备协议网络服务句柄
					(PNPC_BYTE)&tPacketData,			//设备协议数据缓冲区，包括NPC_S_DPS_MSG_HEAD
					sizeof(tPacketData)					//设备协议数据长度
					);
			}
			break;
		case NPC_D_PVM_CFG_FUNCID_STORAGE_FORMAT:			//格式化存储器
			//不处理该消息，该消息用于设置参数使用
			break;

		default:
			{
				//显示信息
				NPC_CHAR sTempLogBuf[128];
				sprintf(sTempLogBuf, "[UMSP]  Get default config: error fund id: %d.", pMsgBody->i_iCfgFuncId);
				NPC_F_LOG_SR_ShowInfo(sTempLogBuf);

				in_pMsgHead->ucResult = NPC_D_MON_CSD_DPS_ERROR_CODE_ID_ERROR;
				bRet = FALSE;
			}
			break;
		}
	}

	//CLEAR AREA
	_NPC_CLEAR:
	{
		if ( !bRet )
		{
			//发送消息给设备协议网络服务
			NPC_F_DPS_SendDevProData(
				in_pMcSrvData->hUmspProSrv,			//设备协议网络服务句柄
				in_pMsgDataBuf,						//设备协议数据缓冲区，包括NPC_S_DPS_MSG_HEAD
				in_iMsgDataLen						//设备协议数据长度
				);
		}
	}
}

//处理协议事件：导入配置
NPC_VOID	NPC_F_UMSP_MC_DoProEvent_EVENT_E3_IEC_IMP_CONFIG(
	IN		PNPC_S_UMSP_MCSRV_DATA					in_pMcSrvData,				//主控服务数据
	IN		PNPC_S_DPS_MSG_HEAD						in_pMsgHead,				//消息头
	IN		PNPC_BYTE								in_pMsgDataBuf,				//消息数据缓冲区，包括NPC_S_DPS_MSG_HEAD
	IN		NPC_INT									in_iMsgDataLen				//消息数据长度
	)
{
	PNPC_S_DPS_MSG_BODY_EVENT_E3_IEC_IMP_CONFIG		pMsgBody;
	PNPC_CHAR										pCfgPos;
	NPC_INT											iCfgLen;

	pMsgBody = (PNPC_S_DPS_MSG_BODY_EVENT_E3_IEC_IMP_CONFIG)((PNPC_BYTE)in_pMsgHead + sizeof(NPC_S_DPS_MSG_HEAD));
	pCfgPos = (PNPC_CHAR)(pMsgBody + 1);
	iCfgLen = in_iMsgDataLen - (NPC_INT)(
		sizeof(NPC_S_DPS_MSG_HEAD) + 
		sizeof(NPC_S_DPS_MSG_BODY_EVENT_E3_IEC_IMP_CONFIG) );

	in_pMsgHead->ucResult = 0;
	in_pMsgHead->ucMsgType = NPC_D_DPS_MSG_TYPE_RESPONSION;

	if ( iCfgLen <= 0 )
	{
		in_pMsgHead->ucResult = NPC_D_MON_CSD_DPS_ERROR_CODE_OTHER;
	}
	else
	{
		if ( iCfgLen > sizeof(in_pMcSrvData->sImportCfgBuf) - 1 )
			iCfgLen = sizeof(in_pMcSrvData->sImportCfgBuf) - 1;

		memcpy(in_pMcSrvData->sImportCfgBuf, pCfgPos, iCfgLen);
		in_pMcSrvData->sImportCfgBuf[iCfgLen] = 0;
	}

	//发送消息给设备协议网络服务
	NPC_F_DPS_SendDevProData(
		in_pMcSrvData->hUmspProSrv,			//设备协议网络服务句柄
		in_pMsgDataBuf,						//设备协议数据缓冲区，包括NPC_S_DPS_MSG_HEAD
		in_iMsgDataLen						//设备协议数据长度
		);
}

//处理协议事件：导出配置
NPC_VOID	NPC_F_UMSP_MC_DoProEvent_EVENT_E3_IEC_EXP_CONFIG(
	IN		PNPC_S_UMSP_MCSRV_DATA					in_pMcSrvData,				//主控服务数据
	IN		PNPC_S_DPS_MSG_HEAD						in_pMsgHead,				//消息头
	IN		PNPC_BYTE								in_pMsgDataBuf,				//消息数据缓冲区，包括NPC_S_DPS_MSG_HEAD
	IN		NPC_INT									in_iMsgDataLen				//消息数据长度
	)
{
	NPC_BOOL										bRet;
	PNPC_S_DPS_MSG_BODY_EVENT_E3_IEC_EXP_CONFIG		pMsgBody;

	PNPC_BYTE										pSendDataBuf;
	NPC_INT											iSendDataBufSize;
	NPC_INT											iSendDataLen;

	PNPC_S_DPS_MSG_HEAD								pSendMsgHead;
	PNPC_S_DPS_MSG_BODY_EVENT_E3_IEC_EXP_CONFIG		pSendMsgBody;
	
	//INIT AREA
	{
		bRet = TRUE;
		pMsgBody = (PNPC_S_DPS_MSG_BODY_EVENT_E3_IEC_EXP_CONFIG)((PNPC_BYTE)in_pMsgHead + sizeof(NPC_S_DPS_MSG_HEAD));

		in_pMsgHead->ucResult = 0;
		in_pMsgHead->ucMsgType = NPC_D_DPS_MSG_TYPE_RESPONSION;

		pSendDataBuf = NULL;
		iSendDataBufSize = 0;
		iSendDataLen = 0;
	}

	//CODE AREA
	{
		//向缓冲区添加数据
		if ( !NPC_F_MEM_MG_AddDataToBuf(
			&pSendDataBuf,				//数据缓冲区指针
			&iSendDataBufSize,			//数据缓冲区大小
			&iSendDataLen,				//实际数据长度
			in_pMsgDataBuf,				//添加数据指针
			in_iMsgDataLen				//添加数据长度
			) )
		{
			in_pMsgHead->ucResult = NPC_D_MON_CSD_DPS_ERROR_CODE_OTHER;

			bRet = FALSE;
			goto _NPC_CLEAR;
		}

		if ( in_pMcSrvData->sImportCfgBuf[0] == 0 )
			strcpy(in_pMcSrvData->sImportCfgBuf, "{\"config\": \"data\"}");

		//向缓冲区添加数据
		if ( !NPC_F_MEM_MG_AddDataToBuf(
			&pSendDataBuf,									//数据缓冲区指针
			&iSendDataBufSize,								//数据缓冲区大小
			&iSendDataLen,									//实际数据长度
			(PNPC_BYTE)(in_pMcSrvData->sImportCfgBuf),		//添加数据指针
			strlen(in_pMcSrvData->sImportCfgBuf)			//添加数据长度
			) )
		{
			in_pMsgHead->ucResult = NPC_D_MON_CSD_DPS_ERROR_CODE_OTHER;

			bRet = FALSE;
			goto _NPC_CLEAR;
		}

		pSendMsgHead = (PNPC_S_DPS_MSG_HEAD)pSendDataBuf;
		pSendMsgBody = (PNPC_S_DPS_MSG_BODY_EVENT_E3_IEC_EXP_CONFIG)((PNPC_BYTE)pSendMsgHead + sizeof(NPC_S_DPS_MSG_HEAD));

		pSendMsgHead->iMsgBodyLen = iSendDataLen - sizeof(NPC_S_DPS_MSG_HEAD);

		pSendMsgBody->o_iCfgDataLen = strlen(in_pMcSrvData->sImportCfgBuf);

		//--------------------------------------------------------------------
		//发送消息给设备协议网络服务
		NPC_F_DPS_SendDevProData(
			in_pMcSrvData->hUmspProSrv,			//设备协议网络服务句柄
			pSendDataBuf,						//设备协议数据缓冲区，包括NPC_S_DPS_MSG_HEAD
			iSendDataLen						//设备协议数据长度
			);
	}

	//CLEARA REA
	_NPC_CLEAR:
	{
		if ( !bRet )
		{
			//发送消息给设备协议网络服务
			NPC_F_DPS_SendDevProData(
				in_pMcSrvData->hUmspProSrv,			//设备协议网络服务句柄
				in_pMsgDataBuf,						//设备协议数据缓冲区，包括NPC_S_DPS_MSG_HEAD
				in_iMsgDataLen						//设备协议数据长度
				);
		}

		if ( pSendDataBuf )
		{
			free(pSendDataBuf);
			pSendDataBuf = NULL;
		}
	}
}


//处理协议事件：查询网络信息
NPC_VOID	NPC_F_UMSP_MC_DoProEvent_EVENT_E3_QUERY_NET_INFO(
	IN		PNPC_S_UMSP_MCSRV_DATA					in_pMcSrvData,				//主控服务数据
	IN		PNPC_S_DPS_MSG_HEAD						in_pMsgHead,				//消息头
	IN		PNPC_BYTE								in_pMsgDataBuf,				//消息数据缓冲区，包括NPC_S_DPS_MSG_HEAD
	IN		NPC_INT									in_iMsgDataLen				//消息数据长度
	)
{
	NPC_BOOL										bRet;
	PNPC_S_DPS_MSG_BODY_EVENT_E3_QUERY_NET_INFO		pMsgBody;

	PNPC_BYTE										pSendDataBuf;
	NPC_INT											iSendDataBufSize;
	NPC_INT											iSendDataLen;

	NPC_S_DPS_MSG_BODY_EVENT_E3_NET_CARD_INFO		tNetCardInfo = {0};
	NPC_CHAR										sCfgFileName[256];
	NPC_CHAR										sKeyName[128];
	NPC_CHAR										sKeyValue[128];
	NPC_INT											i, iNetCardNum;

	PNPC_S_DPS_MSG_HEAD								pSendMsgHead;
	PNPC_S_DPS_MSG_BODY_EVENT_E3_QUERY_NET_INFO		pSendMsgBody;

	//INIT AREA
	{
		bRet = TRUE;
		pMsgBody = (PNPC_S_DPS_MSG_BODY_EVENT_E3_QUERY_NET_INFO)((PNPC_BYTE)in_pMsgHead + sizeof(NPC_S_DPS_MSG_HEAD));

		in_pMsgHead->ucResult = 0;
		in_pMsgHead->ucMsgType = NPC_D_DPS_MSG_TYPE_RESPONSION;

		pSendDataBuf = NULL;
		iSendDataBufSize = 0;
		iSendDataLen = 0;
	}

	//CODE AREA
	{
		//显示信息
		NPC_F_LOG_SR_ShowInfo((PNPC_CHAR)"[UMSP]  Query network info");

		//--------------------------------------------------------------------
		//向缓冲区添加数据
		if ( !NPC_F_MEM_MG_AddDataToBuf(
			&pSendDataBuf,				//数据缓冲区指针
			&iSendDataBufSize,			//数据缓冲区大小
			&iSendDataLen,				//实际数据长度
			in_pMsgDataBuf,				//添加数据指针
			in_iMsgDataLen				//添加数据长度
			) )
		{
			in_pMsgHead->ucResult = NPC_D_MON_CSD_DPS_ERROR_CODE_OTHER;

			bRet = FALSE;
			goto _NPC_CLEAR;
		}

		//--------------------------------------------------------------------
		strcpy(sCfgFileName, ".\\npc_umsp_server_config.ini");

		//网卡数
		iNetCardNum = GetPrivateProfileInt("NET", "NetCardNum", 0, sCfgFileName);

		for ( i=0; i<iNetCardNum; i++ )
		{
			memset(&tNetCardInfo, 0, sizeof(tNetCardInfo));

			//网卡名称
			sprintf(sKeyName, "NetCardName[%d]", i);
			GetPrivateProfileString("NET", sKeyName, "", tNetCardInfo.sNetCardName, sizeof(tNetCardInfo.sNetCardName), sCfgFileName);

			//MAC地址
			sprintf(sKeyName, "Mac[%d]", i);
			GetPrivateProfileString("NET", sKeyName, "", sKeyValue, sizeof(sKeyValue), sCfgFileName);

			//将字符串MAC地址转换成二进制MAC地址
			NPC_F_SYS_GetBitMacAddr(
				sKeyValue,				//字符串MAC地址
				tNetCardInfo.sMac		//二进制MAC地址
				);

			//是否打开DHCP，如果为TRUE，则后面三个参数无效
			sprintf(sKeyName, "IfOpenDhcp[%d]", i);
			tNetCardInfo.bIfOpenDhcp = GetPrivateProfileInt("NET", sKeyName, 0, sCfgFileName);

			//IP地址
			sprintf(sKeyName, "Ipaddr[%d]", i);
			GetPrivateProfileString("NET", sKeyName, "", tNetCardInfo.sIpaddr, sizeof(tNetCardInfo.sIpaddr), sCfgFileName);

			//子网掩码
			sprintf(sKeyName, "Submask[%d]", i);
			GetPrivateProfileString("NET", sKeyName, "", tNetCardInfo.sSubmask, sizeof(tNetCardInfo.sSubmask), sCfgFileName);

			//网关地址
			sprintf(sKeyName, "Gateway[%d]", i);
			GetPrivateProfileString("NET", sKeyName, "", tNetCardInfo.sGateway, sizeof(tNetCardInfo.sGateway), sCfgFileName);

			//向缓冲区添加数据
			if ( !NPC_F_MEM_MG_AddDataToBuf(
				&pSendDataBuf,					//数据缓冲区指针
				&iSendDataBufSize,				//数据缓冲区大小
				&iSendDataLen,					//实际数据长度
				(PNPC_BYTE)&tNetCardInfo,		//添加数据指针
				sizeof(tNetCardInfo)			//添加数据长度
				) )
			{
				in_pMsgHead->ucResult = NPC_D_MON_CSD_DPS_ERROR_CODE_OTHER;

				bRet = FALSE;
				goto _NPC_CLEAR;
			}
		}

		pSendMsgHead = (PNPC_S_DPS_MSG_HEAD)pSendDataBuf;
		pSendMsgBody = (PNPC_S_DPS_MSG_BODY_EVENT_E3_QUERY_NET_INFO)((PNPC_BYTE)pSendMsgHead + sizeof(NPC_S_DPS_MSG_HEAD));

		pSendMsgHead->iMsgBodyLen = iSendDataLen - sizeof(NPC_S_DPS_MSG_HEAD);

		pSendMsgBody->o_iNetCardNum = iNetCardNum;

		//--------------------------------------------------------------------
		//发送消息给设备协议网络服务
		NPC_F_DPS_SendDevProData(
			in_pMcSrvData->hUmspProSrv,			//设备协议网络服务句柄
			pSendDataBuf,						//设备协议数据缓冲区，包括NPC_S_DPS_MSG_HEAD
			iSendDataLen						//设备协议数据长度
			);
	}

	//CLEAR AREA
	_NPC_CLEAR:
	{
		if ( !bRet )
		{
			//发送消息给设备协议网络服务
			NPC_F_DPS_SendDevProData(
				in_pMcSrvData->hUmspProSrv,			//设备协议网络服务句柄
				in_pMsgDataBuf,						//设备协议数据缓冲区，包括NPC_S_DPS_MSG_HEAD
				in_iMsgDataLen						//设备协议数据长度
				);
		}

		if ( pSendDataBuf )
		{
			free(pSendDataBuf);
			pSendDataBuf = NULL;
		}
	}
}

//处理协议事件：修改网络地址
NPC_VOID	NPC_F_UMSP_MC_DoProEvent_EVENT_E3_MODIFY_NET_ADDR(
	IN		PNPC_S_UMSP_MCSRV_DATA					in_pMcSrvData,				//主控服务数据
	IN		PNPC_S_DPS_MSG_HEAD						in_pMsgHead,				//消息头
	IN		PNPC_BYTE								in_pMsgDataBuf,				//消息数据缓冲区，包括NPC_S_DPS_MSG_HEAD
	IN		NPC_INT									in_iMsgDataLen				//消息数据长度
	)
{
	NPC_BOOL										bRet;
	PNPC_S_DPS_MSG_BODY_EVENT_E3_MODIFY_NET_ADDR	pMsgBody;

	NPC_CHAR										sCfgFileName[256];
	NPC_CHAR										sKeyName[128];
	NPC_CHAR										sKeyValue[128];
//	NPC_BYTE										sTempMac[6];
	NPC_INT											i, iNetCardNum, iFindIndex;

	//INIT AREA
	{
		bRet = TRUE;
		pMsgBody = (PNPC_S_DPS_MSG_BODY_EVENT_E3_MODIFY_NET_ADDR)((PNPC_BYTE)in_pMsgHead + sizeof(NPC_S_DPS_MSG_HEAD));

		in_pMsgHead->ucResult = 0;
		in_pMsgHead->ucMsgType = NPC_D_DPS_MSG_TYPE_RESPONSION;
	}

	//CODE AREA
	{
		//显示信息
		NPC_CHAR sTempLogBuf[256];
		sprintf(sTempLogBuf, "[UMSP]  Modify net addr: net card name: %s.", 
			pMsgBody->i_sNetCardName
			);
		NPC_F_LOG_SR_ShowInfo(sTempLogBuf);

		//--------------------------------------------------------------------
		strcpy(sCfgFileName, ".\\npc_umsp_server_config.ini");

		//网卡数
		iNetCardNum = GetPrivateProfileInt("NET", "NetCardNum", 0, sCfgFileName);

		iFindIndex = -1;
		for ( i=0; i<iNetCardNum; i++ )
		{
			//网卡名称
			sprintf(sKeyName, "NetCardName[%d]", i);
			GetPrivateProfileString("NET", sKeyName, "", sKeyValue, sizeof(sKeyValue), sCfgFileName);

			if ( strcmp(sKeyValue, pMsgBody->i_sNetCardName) != 0 )
			{
				continue;
			}
/*
			//MAC地址
			sprintf(sKeyName, "Mac[%d]", i);
			GetPrivateProfileString("NET", sKeyName, "", sKeyValue, sizeof(sKeyValue), sCfgFileName);

			//将字符串MAC地址转换成二进制MAC地址
			NPC_F_SYS_GetBitMacAddr(
				sKeyValue,				//字符串MAC地址
				sTempMac				//二进制MAC地址
				);

			if ( memcmp(sTempMac, pMsgBody->i_sMac, 6) != 0 )
			{
				continue;
			}
*/
			iFindIndex = i;
			break;
		}

		if ( iFindIndex == -1 )
		{
			//显示信息
			NPC_F_LOG_SR_ShowInfo("[UMSP]  Modify net addr fail, cause: No corresponding network card was found..");

			in_pMsgHead->ucResult = NPC_D_MON_CSD_DPS_ERROR_CODE_RESOURCE_NOT_EXIST;
			goto _NPC_CLEAR;
		}

		//--------------------------------------------------------------------
		//是否打开DHCP，如果为TRUE，则后面三个参数无效
		sprintf(sKeyName, "IfOpenDhcp[%d]", iFindIndex);
		sprintf(sKeyValue, "%d", pMsgBody->i_bIfOpenDhcp);
		WritePrivateProfileString("NET", sKeyName, sKeyValue, sCfgFileName);

		//IP地址
		sprintf(sKeyName, "Ipaddr[%d]",iFindIndex);
		WritePrivateProfileString("NET", sKeyName, pMsgBody->i_sIpaddr, sCfgFileName);

		//子网掩码
		sprintf(sKeyName, "Submask[%d]", iFindIndex);
		WritePrivateProfileString("NET", sKeyName, pMsgBody->i_sSubmask, sCfgFileName);

		//网关地址
		sprintf(sKeyName, "Gateway[%d]", iFindIndex);
		WritePrivateProfileString("NET", sKeyName, pMsgBody->i_sGateway, sCfgFileName);
	}

	//CLEAR AREA
	_NPC_CLEAR:
	{
		//发送消息给设备协议网络服务
		NPC_F_DPS_SendDevProData(
			in_pMcSrvData->hUmspProSrv,			//设备协议网络服务句柄
			in_pMsgDataBuf,						//设备协议数据缓冲区，包括NPC_S_DPS_MSG_HEAD
			in_iMsgDataLen						//设备协议数据长度
			);
	}
}


//-----------------------------------------------------------------------------------
//打开串口
NPC_VOID	NPC_F_UMSP_MC_DoProEvent_EVENT_E3_COM_OPEN(
	IN		PNPC_S_UMSP_MCSRV_DATA					in_pMcSrvData,				//主控服务数据
	IN		PNPC_S_DPS_MSG_HEAD						in_pMsgHead,				//消息头
	IN		PNPC_BYTE								in_pMsgDataBuf,				//消息数据缓冲区，包括NPC_S_DPS_MSG_HEAD
	IN		NPC_INT									in_iMsgDataLen				//消息数据长度
	)
{
	NPC_BOOL										bRet;
	PNPC_S_DPS_MSG_BODY_EVENT_E3_COM_OPEN			pMsgBody;

	//INIT AREA
	{
		bRet = TRUE;
		pMsgBody = (PNPC_S_DPS_MSG_BODY_EVENT_E3_COM_OPEN)((PNPC_BYTE)in_pMsgHead + sizeof(NPC_S_DPS_MSG_HEAD));

		in_pMsgHead->ucResult = 0;
		in_pMsgHead->ucMsgType = NPC_D_DPS_MSG_TYPE_RESPONSION;
	}

	//CODE AREA
	{
		//显示信息
		NPC_CHAR sTempLogBuf[256];
		sprintf(sTempLogBuf, "[UMSP]  Open com: com type: %d, port no: %d.", 
			pMsgBody->i_iComType, pMsgBody->i_iPortNo);
		NPC_F_LOG_SR_ShowInfo(sTempLogBuf);

	}

	//CLEAR AREA
	//_NPC_CLEAR:
	{
		//发送消息给设备协议网络服务
		NPC_F_DPS_SendDevProData(
			in_pMcSrvData->hUmspProSrv,			//设备协议网络服务句柄
			in_pMsgDataBuf,						//设备协议数据缓冲区，包括NPC_S_DPS_MSG_HEAD
			in_iMsgDataLen						//设备协议数据长度
			);
	}
}

//关闭串口
NPC_VOID	NPC_F_UMSP_MC_DoProEvent_EVENT_E3_COM_CLOSE(
	IN		PNPC_S_UMSP_MCSRV_DATA					in_pMcSrvData,				//主控服务数据
	IN		PNPC_S_DPS_MSG_HEAD						in_pMsgHead,				//消息头
	IN		PNPC_BYTE								in_pMsgDataBuf,				//消息数据缓冲区，包括NPC_S_DPS_MSG_HEAD
	IN		NPC_INT									in_iMsgDataLen				//消息数据长度
	)
{
	NPC_BOOL										bRet;
	PNPC_S_DPS_MSG_BODY_EVENT_E3_COM_CLOSE			pMsgBody;

	//INIT AREA
	{
		bRet = TRUE;
		pMsgBody = (PNPC_S_DPS_MSG_BODY_EVENT_E3_COM_CLOSE)((PNPC_BYTE)in_pMsgHead + sizeof(NPC_S_DPS_MSG_HEAD));

		in_pMsgHead->ucResult = 0;
		in_pMsgHead->ucMsgType = NPC_D_DPS_MSG_TYPE_RESPONSION;
	}

	//CODE AREA
	{
		//显示信息
		NPC_CHAR sTempLogBuf[256];
		sprintf(sTempLogBuf, "[UMSP]  Close com: com type: %d, port no: %d.", 
			pMsgBody->i_iComType, pMsgBody->i_iPortNo);
		NPC_F_LOG_SR_ShowInfo(sTempLogBuf);
	}

	//CLEAR AREA
	//_NPC_CLEAR:
	{
		//发送消息给设备协议网络服务
		NPC_F_DPS_SendDevProData(
			in_pMcSrvData->hUmspProSrv,			//设备协议网络服务句柄
			in_pMsgDataBuf,						//设备协议数据缓冲区，包括NPC_S_DPS_MSG_HEAD
			in_iMsgDataLen						//设备协议数据长度
			);
	}
}

//发送数据(客户端发送给设备端)
NPC_VOID	NPC_F_UMSP_MC_DoProEvent_EVENT_E3_COM_SEND_DATA(
	IN		PNPC_S_UMSP_MCSRV_DATA					in_pMcSrvData,				//主控服务数据
	IN		PNPC_S_DPS_MSG_HEAD						in_pMsgHead,				//消息头
	IN		PNPC_BYTE								in_pMsgDataBuf,				//消息数据缓冲区，包括NPC_S_DPS_MSG_HEAD
	IN		NPC_INT									in_iMsgDataLen				//消息数据长度
	)
{
	NPC_BOOL										bRet;
	PNPC_S_DPS_MSG_BODY_EVENT_E3_COM_SEND_DATA		pMsgBody;

	//INIT AREA
	{
		bRet = TRUE;
		pMsgBody = (PNPC_S_DPS_MSG_BODY_EVENT_E3_COM_SEND_DATA)((PNPC_BYTE)in_pMsgHead + sizeof(NPC_S_DPS_MSG_HEAD));

		in_pMsgHead->ucResult = 0;
		in_pMsgHead->ucMsgType = NPC_D_DPS_MSG_TYPE_RESPONSION;
	}

	//CODE AREA
	{
		//显示信息
		NPC_CHAR sTempLogBuf[256];
		sprintf(sTempLogBuf, "[UMSP]  Client send com data: com type: %d, port no: %d, data len: %d.", 
			pMsgBody->i_iComType, pMsgBody->i_iPortNo, pMsgBody->i_iDataLen);
		NPC_F_LOG_SR_ShowInfo(sTempLogBuf);
	}

	//CLEAR AREA
	//_NPC_CLEAR:
	{
		//发送消息给设备协议网络服务
		NPC_F_DPS_SendDevProData(
			in_pMcSrvData->hUmspProSrv,			//设备协议网络服务句柄
			in_pMsgDataBuf,						//设备协议数据缓冲区，包括NPC_S_DPS_MSG_HEAD
			in_iMsgDataLen						//设备协议数据长度
			);
	}
}

//设备串口数据(设备端发送给客户端)(无应答)
NPC_VOID	NPC_F_UMSP_MC_DoProEvent_EVENT_E3_COM_SUBS_DATA(
	IN		PNPC_S_UMSP_MCSRV_DATA					in_pMcSrvData,				//主控服务数据
	IN		PNPC_S_DPS_MSG_HEAD						in_pMsgHead,				//消息头
	IN		PNPC_BYTE								in_pMsgDataBuf,				//消息数据缓冲区，包括NPC_S_DPS_MSG_HEAD
	IN		NPC_INT									in_iMsgDataLen				//消息数据长度
	)
{
}


//-----------------------------------------------------------------------------------
//增加用户
NPC_VOID	NPC_F_UMSP_MC_DoProEvent_EVENT_E3_UNS_ADD_USER(
	IN		PNPC_S_UMSP_MCSRV_DATA					in_pMcSrvData,				//主控服务数据
	IN		PNPC_S_DPS_MSG_HEAD						in_pMsgHead,				//消息头
	IN		PNPC_BYTE								in_pMsgDataBuf,				//消息数据缓冲区，包括NPC_S_DPS_MSG_HEAD
	IN		NPC_INT									in_iMsgDataLen				//消息数据长度
	)
{
	NPC_BOOL										bRet;
	PNPC_S_DPS_MSG_BODY_EVENT_E3_UNS_ADD_USER		pMsgBody;

	//INIT AREA
	{
		bRet = TRUE;
		pMsgBody = (PNPC_S_DPS_MSG_BODY_EVENT_E3_UNS_ADD_USER)((PNPC_BYTE)in_pMsgHead + sizeof(NPC_S_DPS_MSG_HEAD));

		in_pMsgHead->ucResult = 0;
		in_pMsgHead->ucMsgType = NPC_D_DPS_MSG_TYPE_RESPONSION;
	}

	//CODE AREA
	{
		//显示信息
		NPC_CHAR sTempLogBuf[256];
		sprintf(sTempLogBuf, "[UMSP]  Add user: user name: %s, user pwd: %s.", 
			pMsgBody->i_sUserName, pMsgBody->i_sUserPwd);
		NPC_F_LOG_SR_ShowInfo(sTempLogBuf);
	}

	//CLEAR AREA
	//_NPC_CLEAR:
	{
		//发送消息给设备协议网络服务
		NPC_F_DPS_SendDevProData(
			in_pMcSrvData->hUmspProSrv,			//设备协议网络服务句柄
			in_pMsgDataBuf,						//设备协议数据缓冲区，包括NPC_S_DPS_MSG_HEAD
			in_iMsgDataLen						//设备协议数据长度
			);
	}
}

//删除用户
NPC_VOID	NPC_F_UMSP_MC_DoProEvent_EVENT_E3_UNS_DEL_USER(
	IN		PNPC_S_UMSP_MCSRV_DATA					in_pMcSrvData,				//主控服务数据
	IN		PNPC_S_DPS_MSG_HEAD						in_pMsgHead,				//消息头
	IN		PNPC_BYTE								in_pMsgDataBuf,				//消息数据缓冲区，包括NPC_S_DPS_MSG_HEAD
	IN		NPC_INT									in_iMsgDataLen				//消息数据长度
	)
{
	NPC_BOOL										bRet;
	PNPC_S_DPS_MSG_BODY_EVENT_E3_UNS_DEL_USER		pMsgBody;

	//INIT AREA
	{
		bRet = TRUE;
		pMsgBody = (PNPC_S_DPS_MSG_BODY_EVENT_E3_UNS_DEL_USER)((PNPC_BYTE)in_pMsgHead + sizeof(NPC_S_DPS_MSG_HEAD));

		in_pMsgHead->ucResult = 0;
		in_pMsgHead->ucMsgType = NPC_D_DPS_MSG_TYPE_RESPONSION;
	}

	//CODE AREA
	{
		//显示信息
		NPC_CHAR sTempLogBuf[256];
		sprintf(sTempLogBuf, "[UMSP]  Add user: user name: %s.", 
			pMsgBody->i_sUserName);
		NPC_F_LOG_SR_ShowInfo(sTempLogBuf);
	}

	//CLEAR AREA
	//_NPC_CLEAR:
	{
		//发送消息给设备协议网络服务
		NPC_F_DPS_SendDevProData(
			in_pMcSrvData->hUmspProSrv,			//设备协议网络服务句柄
			in_pMsgDataBuf,						//设备协议数据缓冲区，包括NPC_S_DPS_MSG_HEAD
			in_iMsgDataLen						//设备协议数据长度
			);
	}
}

//获取用户权限列表
NPC_VOID	NPC_F_UMSP_MC_DoProEvent_EVENT_E3_UNS_GET_USER_POP(
	IN		PNPC_S_UMSP_MCSRV_DATA					in_pMcSrvData,				//主控服务数据
	IN		PNPC_S_DPS_MSG_HEAD						in_pMsgHead,				//消息头
	IN		PNPC_BYTE								in_pMsgDataBuf,				//消息数据缓冲区，包括NPC_S_DPS_MSG_HEAD
	IN		NPC_INT									in_iMsgDataLen				//消息数据长度
	)
{
	NPC_BOOL										bRet;
	PNPC_S_DPS_MSG_BODY_EVENT_E3_UNS_GET_USER_POP	pMsgBody;

	//INIT AREA
	{
		bRet = TRUE;
		pMsgBody = (PNPC_S_DPS_MSG_BODY_EVENT_E3_UNS_GET_USER_POP)((PNPC_BYTE)in_pMsgHead + sizeof(NPC_S_DPS_MSG_HEAD));

		in_pMsgHead->ucResult = 0;
		in_pMsgHead->ucMsgType = NPC_D_DPS_MSG_TYPE_RESPONSION;
	}

	//CODE AREA
	{
		//显示信息
		NPC_CHAR sTempLogBuf[256];
		sprintf(sTempLogBuf, "[UMSP]  Get user pop: user name: %s.", 
			pMsgBody->i_sUserName);
		NPC_F_LOG_SR_ShowInfo(sTempLogBuf);
	}

	//CLEAR AREA
	//_NPC_CLEAR:
	{
		//发送消息给设备协议网络服务
		NPC_F_DPS_SendDevProData(
			in_pMcSrvData->hUmspProSrv,			//设备协议网络服务句柄
			in_pMsgDataBuf,						//设备协议数据缓冲区，包括NPC_S_DPS_MSG_HEAD
			in_iMsgDataLen						//设备协议数据长度
			);
	}
}

//设置用户权限列表
NPC_VOID	NPC_F_UMSP_MC_DoProEvent_EVENT_E3_UNS_SET_USER_POP(
	IN		PNPC_S_UMSP_MCSRV_DATA					in_pMcSrvData,				//主控服务数据
	IN		PNPC_S_DPS_MSG_HEAD						in_pMsgHead,				//消息头
	IN		PNPC_BYTE								in_pMsgDataBuf,				//消息数据缓冲区，包括NPC_S_DPS_MSG_HEAD
	IN		NPC_INT									in_iMsgDataLen				//消息数据长度
	)
{
	NPC_BOOL										bRet;
	PNPC_S_DPS_MSG_BODY_EVENT_E3_UNS_SET_USER_POP	pMsgBody;

	//INIT AREA
	{
		bRet = TRUE;
		pMsgBody = (PNPC_S_DPS_MSG_BODY_EVENT_E3_UNS_SET_USER_POP)((PNPC_BYTE)in_pMsgHead + sizeof(NPC_S_DPS_MSG_HEAD));

		in_pMsgHead->ucResult = 0;
		in_pMsgHead->ucMsgType = NPC_D_DPS_MSG_TYPE_RESPONSION;
	}

	//CODE AREA
	{
		//显示信息
		NPC_CHAR sTempLogBuf[256];
		sprintf(sTempLogBuf, "[UMSP]  Set user pop: user name: %s.", 
			pMsgBody->i_sUserName);
		NPC_F_LOG_SR_ShowInfo(sTempLogBuf);
	}

	//CLEAR AREA
	//_NPC_CLEAR:
	{
		//发送消息给设备协议网络服务
		NPC_F_DPS_SendDevProData(
			in_pMcSrvData->hUmspProSrv,			//设备协议网络服务句柄
			in_pMsgDataBuf,						//设备协议数据缓冲区，包括NPC_S_DPS_MSG_HEAD
			in_iMsgDataLen						//设备协议数据长度
			);
	}
}


//-----------------------------------------------------------------------------------
//定制功能
NPC_VOID	NPC_F_UMSP_MC_DoProEvent_EVENT_E7_CUSTOM(
	IN		PNPC_S_UMSP_MCSRV_DATA					in_pMcSrvData,				//主控服务数据
	IN		PNPC_S_DPS_MSG_HEAD						in_pMsgHead,				//消息头
	IN		PNPC_BYTE								in_pMsgDataBuf,				//消息数据缓冲区，包括NPC_S_DPS_MSG_HEAD
	IN		NPC_INT									in_iMsgDataLen				//消息数据长度
	)
{
	NPC_BOOL										bRet;
	PNPC_S_DPS_MSG_BODY_EVENT_E7_CUSTOM				pMsgBody;
	PNPC_BYTE										pCusBodyPos;

	//INIT AREA
	{
		bRet = TRUE;
		pMsgBody = (PNPC_S_DPS_MSG_BODY_EVENT_E7_CUSTOM)((PNPC_BYTE)in_pMsgHead + sizeof(NPC_S_DPS_MSG_HEAD));
		pCusBodyPos = (PNPC_BYTE)pMsgBody + sizeof(NPC_S_DPS_MSG_BODY_EVENT_E7_CUSTOM);

		in_pMsgHead->ucResult = 0;
		in_pMsgHead->ucMsgType = NPC_D_DPS_MSG_TYPE_RESPONSION;
	}

	//CODE AREA
	{
		switch ( pMsgBody->i_iCusFuncId )
		{
		case NPC_D_DPS_CUSTOM_NI_FUNCID_QUERY_MP3_FILE_LIST:			//查询MP3文件列表
			bRet = NPC_F_UMSP_MC_DoCustomEvent_QUERY_MP3_FILE_LIST(in_pMcSrvData, in_pMsgHead, in_pMsgDataBuf, in_iMsgDataLen);
			break;
		case NPC_D_DPS_CUSTOM_NI_FUNCID_DELETE_MP3_FILE:				//删除MP3文件
			bRet = NPC_F_UMSP_MC_DoCustomEvent_DELETE_MP3_FILE(in_pMcSrvData, in_pMsgHead, in_pMsgDataBuf, in_iMsgDataLen);
			break;
		case NPC_D_DPS_CUSTOM_NI_FUNCID_START_PLAY_MP3_FILE:			//开始播放MP3文件
			bRet = NPC_F_UMSP_MC_DoCustomEvent_START_PLAY_MP3_FILE(in_pMcSrvData, in_pMsgHead, in_pMsgDataBuf, in_iMsgDataLen);
			break;
		case NPC_D_DPS_CUSTOM_NI_FUNCID_STOP_PLAY_MP3_FILE:				//停止播放MP3文件
			bRet = NPC_F_UMSP_MC_DoCustomEvent_STOP_PLAY_MP3_FILE(in_pMcSrvData, in_pMsgHead, in_pMsgDataBuf, in_iMsgDataLen);
			break;
		case NPC_D_DPS_CUSTOM_NI_FUNCID_CTRL_VOLUME:					//控制音量
			bRet = NPC_F_UMSP_MC_DoCustomEvent_CTRL_VOLUME(in_pMcSrvData, in_pMsgHead, in_pMsgDataBuf, in_iMsgDataLen);
			break;
		case NPC_D_DPS_CUSTOM_NI_FUNCID_START_UPLOAD_MP3_FILE:			//开始上传文件
			bRet = NPC_F_UMSP_MC_DoCustomEvent_START_UPLOAD_MP3_FILE(in_pMcSrvData, in_pMsgHead, in_pMsgDataBuf, in_iMsgDataLen);
			break;
		case NPC_D_DPS_CUSTOM_NI_FUNCID_STOP_UPLOAD_MP3_FILE:			//停止上传文件
			bRet = NPC_F_UMSP_MC_DoCustomEvent_STOP_UPLOAD_MP3_FILE(in_pMcSrvData, in_pMsgHead, in_pMsgDataBuf, in_iMsgDataLen);
			break;
		case NPC_D_DPS_CUSTOM_NI_FUNCID_UPLOAD_FILE_DATA:				//上传文件数据
			bRet = NPC_F_UMSP_MC_DoCustomEvent_UPLOAD_FILE_DATA(in_pMcSrvData, in_pMsgHead, in_pMsgDataBuf, in_iMsgDataLen);
			break;
		default:
			bRet = FALSE;
			break;
		}
	}

	//CLEAR AREA
	//_NPC_CLEAR:
	{
		if ( !bRet )
		{
			in_pMsgHead->ucResult = NPC_D_MON_CSD_DPS_ERROR_CODE_ID_ERROR;

			//发送消息给设备协议网络服务
			NPC_F_DPS_SendDevProData(
				in_pMcSrvData->hUmspProSrv,			//设备协议网络服务句柄
				in_pMsgDataBuf,						//设备协议数据缓冲区，包括NPC_S_DPS_MSG_HEAD
				in_iMsgDataLen						//设备协议数据长度
				);
		}
	}
}


//-----------------------------------------------------------------------------------
//查询能力集
NPC_VOID	NPC_F_UMSP_MC_DoProEvent_EVENT_E7_RMY_QUERY_ABILITY(
	IN		PNPC_S_UMSP_MCSRV_DATA					in_pMcSrvData,				//主控服务数据
	IN		PNPC_S_DPS_MSG_HEAD						in_pMsgHead,				//消息头
	IN		PNPC_BYTE								in_pMsgDataBuf,				//消息数据缓冲区，包括NPC_S_DPS_MSG_HEAD
	IN		NPC_INT									in_iMsgDataLen				//消息数据长度
	)
{
	NPC_BOOL										bRet;
	PNPC_S_DPS_MSG_BODY_EVENT_E7_RMY_QUERY_ABILITY	pMsgBody;

	//INIT AREA
	{
		bRet = TRUE;
		pMsgBody = (PNPC_S_DPS_MSG_BODY_EVENT_E7_RMY_QUERY_ABILITY)((PNPC_BYTE)in_pMsgHead + sizeof(NPC_S_DPS_MSG_HEAD));

		in_pMsgHead->ucResult = 0;
		in_pMsgHead->ucMsgType = NPC_D_DPS_MSG_TYPE_RESPONSION;
	}

	//CODE AREA
	{
		//显示信息
		NPC_F_LOG_SR_ShowInfo((PNPC_CHAR)"[UMSP]  Query ability");

		switch ( pMsgBody->i_iRequVerNo )
		{
		case 1:
			{
				struct _AbPacket{
					NPC_S_DPS_MSG_HEAD tMsgHead;
					NPC_S_DPS_MSG_BODY_EVENT_E7_RMY_QUERY_ABILITY tMsgBody;
					NPC_S_PVM_IMD_CFG_ABILITY_HEAD tCfgHead;
					NPC_S_PVM_IMD_CFG_ABILITY_DATA_V1 tCfgBody;
				} tAbPacket = {0};

				//-------------------------------------------------------------------
				memcpy(&tAbPacket, in_pMsgDataBuf, sizeof(NPC_S_DPS_MSG_HEAD) + sizeof(NPC_S_DPS_MSG_BODY_EVENT_E7_RMY_QUERY_ABILITY));

				tAbPacket.tMsgHead.iMsgBodyLen = sizeof(tAbPacket) - sizeof(NPC_S_DPS_MSG_HEAD);

				tAbPacket.tMsgBody.o_iAbilityDataLen = sizeof(NPC_S_PVM_IMD_CFG_ABILITY_HEAD) + sizeof(NPC_S_PVM_IMD_CFG_ABILITY_DATA_V1);

				//-------------------------------------------------------------------
				tAbPacket.tCfgHead.usAbVerNo = pMsgBody->i_iRequVerNo;							//能力集版本号
				tAbPacket.tCfgHead.usAbDataLen = sizeof(NPC_S_PVM_IMD_CFG_ABILITY_DATA_V1);		//能力集数据长度

				tAbPacket.tCfgBody.tNet.bIfCableNet = TRUE;						//是否有有线网络
				tAbPacket.tCfgBody.tNet.bIfWifiNet = FALSE;						//是否有WIFI网络

				//-------------------------------------------------------------------
				tAbPacket.tCfgBody.iCodecSchemeNum = 1;							//编码方案数

				tAbPacket.tCfgBody.tCodecScheme[0].bSupportAudio = TRUE;		//是否支持音频
				tAbPacket.tCfgBody.tCodecScheme[0].iStreamNum = 2;				//码流数，最大5个

				tAbPacket.tCfgBody.tCodecScheme[0].tSubCh[0].ucVedioCodecTable[0] = NPC_D_MON_CSD_CODEC_ID_VEDIO_H264;			//视频编码列表，如果值为0表示该项不存在，参见NPC_D_MON_CSD_CODEC_ID_VEDIO_*定义
				tAbPacket.tCfgBody.tCodecScheme[0].tSubCh[0].ucResolutionTable[0] = NPC_D_MON_CSD_VEDIO_RESOLUTION_720P;		//分辨率列表，如果值为0表示该项不存在，参见NPC_D_MON_CSD_VEDIO_RESOLUTION_*定义
				tAbPacket.tCfgBody.tCodecScheme[0].tSubCh[0].ucResolutionTable[1] = NPC_D_MON_CSD_VEDIO_RESOLUTION_D1;
				tAbPacket.tCfgBody.tCodecScheme[0].tSubCh[0].usBitRateStart = 128;					//视频码流范围开始（kbps）
				tAbPacket.tCfgBody.tCodecScheme[0].tSubCh[0].usBitRateEnd = 4096;					//视频码流范围结束（kbps）
				tAbPacket.tCfgBody.tCodecScheme[0].tSubCh[0].ucFrameRateStart = 2;					//视频帧率范围开始
				tAbPacket.tCfgBody.tCodecScheme[0].tSubCh[0].ucFrameRateEnd = 25;					//视频帧率范围结束
				tAbPacket.tCfgBody.tCodecScheme[0].tSubCh[0].ucGOPStart = 2;						//I帧间隔范围开始
				tAbPacket.tCfgBody.tCodecScheme[0].tSubCh[0].ucGOPEnd = 100;						//I帧间隔范围结束

				tAbPacket.tCfgBody.tCodecScheme[0].tSubCh[1].ucVedioCodecTable[0] = NPC_D_MON_CSD_CODEC_ID_VEDIO_H264;		//视频编码列表，如果值为0表示该项不存在，参见NPC_D_MON_CSD_CODEC_ID_VEDIO_*定义
				tAbPacket.tCfgBody.tCodecScheme[0].tSubCh[1].ucResolutionTable[0] = NPC_D_MON_CSD_VEDIO_RESOLUTION_VGA;		//分辨率列表，如果值为0表示该项不存在，参见NPC_D_MON_CSD_VEDIO_RESOLUTION_*定义
				tAbPacket.tCfgBody.tCodecScheme[0].tSubCh[1].ucResolutionTable[1] = NPC_D_MON_CSD_VEDIO_RESOLUTION_CIF;
				tAbPacket.tCfgBody.tCodecScheme[0].tSubCh[1].usBitRateStart = 128;					//视频码流范围开始（kbps）
				tAbPacket.tCfgBody.tCodecScheme[0].tSubCh[1].usBitRateEnd = 1024;					//视频码流范围结束（kbps）
				tAbPacket.tCfgBody.tCodecScheme[0].tSubCh[1].ucFrameRateStart = 2;					//视频帧率范围开始
				tAbPacket.tCfgBody.tCodecScheme[0].tSubCh[1].ucFrameRateEnd = 25;					//视频帧率范围结束
				tAbPacket.tCfgBody.tCodecScheme[0].tSubCh[1].ucGOPStart = 2;						//I帧间隔范围开始
				tAbPacket.tCfgBody.tCodecScheme[0].tSubCh[1].ucGOPEnd = 100;						//I帧间隔范围结束

				tAbPacket.tCfgBody.tCodecScheme[0].ucUseChTable[0] = 0;				//适用通道表
				tAbPacket.tCfgBody.tCodecScheme[0].ucUseChTable[1] = 1;				//适用通道表
				tAbPacket.tCfgBody.tCodecScheme[0].ucUseChTable[2] = 2;				//适用通道表
				tAbPacket.tCfgBody.tCodecScheme[0].ucUseChTable[3] = 3;				//适用通道表
				tAbPacket.tCfgBody.tCodecScheme[0].iUseChNum = 4;					//适用通道数

				//-------------------------------------------------------------------
				tAbPacket.tCfgBody.tAlarm.bMotionDetect = TRUE;						//是否支持移动侦测
				tAbPacket.tCfgBody.tAlarm.bProbeInput = TRUE;						//是否支持探头输入
				tAbPacket.tCfgBody.tAlarm.bBlindDetect = TRUE;						//是否支持视频遮挡侦测
				tAbPacket.tCfgBody.tAlarm.bLossDetect = TRUE;						//是否支持视频丢失侦测

				//-------------------------------------------------------------------
				//发送消息给设备协议网络服务
				NPC_F_DPS_SendDevProData(
					in_pMcSrvData->hUmspProSrv,			//设备协议网络服务句柄
					(PNPC_BYTE)&tAbPacket,				//设备协议数据缓冲区，包括NPC_S_DPS_MSG_HEAD
					sizeof(tAbPacket)					//设备协议数据长度
					);
			}
			break;
		default:
			in_pMsgHead->ucResult = NPC_D_MON_CSD_DPS_ERROR_CODE_NOT_SUPPORT_OP;
			bRet = FALSE;
			break;
		}
	}

	//CLEAR AREA
	//_NPC_CLEAR:
	{
		if ( !bRet )
		{
			//发送消息给设备协议网络服务
			NPC_F_DPS_SendDevProData(
				in_pMcSrvData->hUmspProSrv,			//设备协议网络服务句柄
				in_pMsgDataBuf,						//设备协议数据缓冲区，包括NPC_S_DPS_MSG_HEAD
				in_iMsgDataLen						//设备协议数据长度
				);
		}
	}
}

//重启设备(无包体)(无应答消息)
NPC_VOID	NPC_F_UMSP_MC_DoProEvent_EVENT_E7_RMY_REBOOT(
	IN		PNPC_S_UMSP_MCSRV_DATA					in_pMcSrvData,				//主控服务数据
	IN		PNPC_S_DPS_MSG_HEAD						in_pMsgHead,				//消息头
	IN		PNPC_BYTE								in_pMsgDataBuf,				//消息数据缓冲区，包括NPC_S_DPS_MSG_HEAD
	IN		NPC_INT									in_iMsgDataLen				//消息数据长度
	)
{
	//显示信息
	NPC_F_LOG_SR_ShowInfo((PNPC_CHAR)"[UMSP]  Reboot");
}

//停止设备(无包体)(无应答消息)
NPC_VOID	NPC_F_UMSP_MC_DoProEvent_EVENT_E7_RMY_STOP(
	IN		PNPC_S_UMSP_MCSRV_DATA					in_pMcSrvData,				//主控服务数据
	IN		PNPC_S_DPS_MSG_HEAD						in_pMsgHead,				//消息头
	IN		PNPC_BYTE								in_pMsgDataBuf,				//消息数据缓冲区，包括NPC_S_DPS_MSG_HEAD
	IN		NPC_INT									in_iMsgDataLen				//消息数据长度
	)
{
	//显示信息
	NPC_F_LOG_SR_ShowInfo((PNPC_CHAR)"[UMSP]  Stop");
}



//-----------------------------------------------------------------------------------
//查询设备版本信息
NPC_VOID	NPC_F_UMSP_MC_DoProEvent_EVENT_E7_DUP_QUERY_DEV_VER_INFO(
	IN		PNPC_S_UMSP_MCSRV_DATA					in_pMcSrvData,				//主控服务数据
	IN		PNPC_S_DPS_MSG_HEAD						in_pMsgHead,				//消息头
	IN		PNPC_BYTE								in_pMsgDataBuf,				//消息数据缓冲区，包括NPC_S_DPS_MSG_HEAD
	IN		NPC_INT									in_iMsgDataLen				//消息数据长度
	)
{
	NPC_BOOL											bRet;
	PNPC_S_DPS_MSG_BODY_EVENT_E7_DUP_QUERY_DEV_VER_INFO	pMsgBody;

	//INIT AREA
	{
		bRet = TRUE;
		pMsgBody = (PNPC_S_DPS_MSG_BODY_EVENT_E7_DUP_QUERY_DEV_VER_INFO)((PNPC_BYTE)in_pMsgHead + sizeof(NPC_S_DPS_MSG_HEAD));

		in_pMsgHead->ucResult = 0;
		in_pMsgHead->ucMsgType = NPC_D_DPS_MSG_TYPE_RESPONSION;
	}

	//CODE AREA
	{
		//显示信息
		NPC_F_LOG_SR_ShowInfo((PNPC_CHAR)"[UMSP]  Query dev ver info");

		pMsgBody->o_usModuleType[0] = 3;				//模块类型,0:未知类型,1:UI,2:SYS,3:UI&SYS
		pMsgBody->o_uiModuleVerNo[0] = 0x00000102;		//模块版本号,按顺序对应模块类型,格式为:x.x.x.x,每字节为一个号
	}

	//CLEAR AREA
	//_NPC_CLEAR:
	{
		//发送消息给设备协议网络服务
		NPC_F_DPS_SendDevProData(
			in_pMcSrvData->hUmspProSrv,			//设备协议网络服务句柄
			in_pMsgDataBuf,						//设备协议数据缓冲区，包括NPC_S_DPS_MSG_HEAD
			in_iMsgDataLen						//设备协议数据长度
			);
	}
}

//请求升级
NPC_VOID	NPC_F_UMSP_MC_DoProEvent_EVENT_E7_DUP_REQU_UPGRADE(
	IN		PNPC_S_UMSP_MCSRV_DATA					in_pMcSrvData,				//主控服务数据
	IN		PNPC_S_DPS_MSG_HEAD						in_pMsgHead,				//消息头
	IN		PNPC_BYTE								in_pMsgDataBuf,				//消息数据缓冲区，包括NPC_S_DPS_MSG_HEAD
	IN		NPC_INT									in_iMsgDataLen				//消息数据长度
	)
{
	NPC_BOOL											bRet;
	PNPC_S_DPS_MSG_BODY_EVENT_E7_DUP_REQU_UPGRADE		pMsgBody;

	//INIT AREA
	{
		bRet = TRUE;
		pMsgBody = (PNPC_S_DPS_MSG_BODY_EVENT_E7_DUP_REQU_UPGRADE)((PNPC_BYTE)in_pMsgHead + sizeof(NPC_S_DPS_MSG_HEAD));

		in_pMsgHead->ucResult = 0;
		in_pMsgHead->ucMsgType = NPC_D_DPS_MSG_TYPE_RESPONSION;
	}

	//CODE AREA
	{
		//显示信息
		NPC_F_LOG_SR_ShowInfo((PNPC_CHAR)"[UMSP]  Request upgrade");

		pMsgBody->o_uiUpgradeId = 44444;					//升级ID
	}

	//CLEAR AREA
	//_NPC_CLEAR:
	{
		//发送消息给设备协议网络服务
		NPC_F_DPS_SendDevProData(
			in_pMcSrvData->hUmspProSrv,			//设备协议网络服务句柄
			in_pMsgDataBuf,						//设备协议数据缓冲区，包括NPC_S_DPS_MSG_HEAD
			in_iMsgDataLen						//设备协议数据长度
			);
	}
}

//查询升级进度
NPC_VOID	NPC_F_UMSP_MC_DoProEvent_EVENT_E7_DUP_QUERY_PROGRESS(
	IN		PNPC_S_UMSP_MCSRV_DATA					in_pMcSrvData,				//主控服务数据
	IN		PNPC_S_DPS_MSG_HEAD						in_pMsgHead,				//消息头
	IN		PNPC_BYTE								in_pMsgDataBuf,				//消息数据缓冲区，包括NPC_S_DPS_MSG_HEAD
	IN		NPC_INT									in_iMsgDataLen				//消息数据长度
	)
{
	NPC_BOOL											bRet;
	PNPC_S_DPS_MSG_BODY_EVENT_E7_DUP_QUERY_PROGRESS		pMsgBody;

	//INIT AREA
	{
		bRet = TRUE;
		pMsgBody = (PNPC_S_DPS_MSG_BODY_EVENT_E7_DUP_QUERY_PROGRESS)((PNPC_BYTE)in_pMsgHead + sizeof(NPC_S_DPS_MSG_HEAD));

		in_pMsgHead->ucResult = 0;
		in_pMsgHead->ucMsgType = NPC_D_DPS_MSG_TYPE_RESPONSION;
	}

	//CODE AREA
	{
		//显示信息
		NPC_F_LOG_SR_ShowInfo((PNPC_CHAR)"[UMSP]  Query upgrade progress");

		pMsgBody->o_iUpgProgress = 40;
	}

	//CLEAR AREA
	//_NPC_CLEAR:
	{
		//发送消息给设备协议网络服务
		NPC_F_DPS_SendDevProData(
			in_pMcSrvData->hUmspProSrv,			//设备协议网络服务句柄
			in_pMsgDataBuf,						//设备协议数据缓冲区，包括NPC_S_DPS_MSG_HEAD
			in_iMsgDataLen						//设备协议数据长度
			);
	}
}

//查询传输位置
NPC_VOID	NPC_F_UMSP_MC_DoProEvent_EVENT_E7_DUP_QUERY_TRANS_POS(
	IN		PNPC_S_UMSP_MCSRV_DATA					in_pMcSrvData,				//主控服务数据
	IN		PNPC_S_DPS_MSG_HEAD						in_pMsgHead,				//消息头
	IN		PNPC_BYTE								in_pMsgDataBuf,				//消息数据缓冲区，包括NPC_S_DPS_MSG_HEAD
	IN		NPC_INT									in_iMsgDataLen				//消息数据长度
	)
{
	NPC_BOOL											bRet;
	PNPC_S_DPS_MSG_BODY_EVENT_E7_DUP_QUERY_TRANS_POS	pMsgBody;

	//INIT AREA
	{
		bRet = TRUE;
		pMsgBody = (PNPC_S_DPS_MSG_BODY_EVENT_E7_DUP_QUERY_TRANS_POS)((PNPC_BYTE)in_pMsgHead + sizeof(NPC_S_DPS_MSG_HEAD));

		in_pMsgHead->ucResult = 0;
		in_pMsgHead->ucMsgType = NPC_D_DPS_MSG_TYPE_RESPONSION;
	}

	//CODE AREA
	{
		//显示信息
		NPC_F_LOG_SR_ShowInfo((PNPC_CHAR)"[UMSP]  Query trans pos");

		pMsgBody->o_iTransPos = 4000;
	}

	//CLEAR AREA
	//_NPC_CLEAR:
	{
		//发送消息给设备协议网络服务
		NPC_F_DPS_SendDevProData(
			in_pMcSrvData->hUmspProSrv,			//设备协议网络服务句柄
			in_pMsgDataBuf,						//设备协议数据缓冲区，包括NPC_S_DPS_MSG_HEAD
			in_iMsgDataLen						//设备协议数据长度
			);
	}
}

//升级数据包
NPC_VOID	NPC_F_UMSP_MC_DoProEvent_EVENT_E7_DUP_UPGRADE_DATA(
	IN		PNPC_S_UMSP_MCSRV_DATA					in_pMcSrvData,				//主控服务数据
	IN		PNPC_S_DPS_MSG_HEAD						in_pMsgHead,				//消息头
	IN		PNPC_BYTE								in_pMsgDataBuf,				//消息数据缓冲区，包括NPC_S_DPS_MSG_HEAD
	IN		NPC_INT									in_iMsgDataLen				//消息数据长度
	)
{
	NPC_BOOL											bRet;
	PNPC_S_DPS_MSG_BODY_EVENT_E7_DUP_UPGRADE_DATA		pMsgBody;

	//INIT AREA
	{
		bRet = TRUE;
		pMsgBody = (PNPC_S_DPS_MSG_BODY_EVENT_E7_DUP_UPGRADE_DATA)((PNPC_BYTE)in_pMsgHead + sizeof(NPC_S_DPS_MSG_HEAD));

		in_pMsgHead->ucResult = 0;
		in_pMsgHead->ucMsgType = NPC_D_DPS_MSG_TYPE_RESPONSION;
	}

	//CODE AREA
	{
		//显示信息
		NPC_F_LOG_SR_ShowInfo((PNPC_CHAR)"[UMSP]  Upgrade data");
	}

	//CLEAR AREA
	//_NPC_CLEAR:
	{
		//发送消息给设备协议网络服务
		NPC_F_DPS_SendDevProData(
			in_pMcSrvData->hUmspProSrv,			//设备协议网络服务句柄
			in_pMsgDataBuf,						//设备协议数据缓冲区，包括NPC_S_DPS_MSG_HEAD
			in_iMsgDataLen						//设备协议数据长度
			);
	}
}


//-----------------------------------------------------------------------------------
//处理协议事件：音频帧数据
NPC_VOID	NPC_F_UMSP_MC_DoProEvent_MEDIA_E8_AUDIO_FRAME(
	IN		PNPC_S_UMSP_MCSRV_DATA					in_pMcSrvData,				//主控服务数据
	IN		PNPC_S_DPS_MSG_HEAD						in_pMsgHead,				//消息头
	IN		PNPC_BYTE								in_pMsgDataBuf,				//消息数据缓冲区，包括NPC_S_DPS_MSG_HEAD
	IN		NPC_INT									in_iMsgDataLen				//消息数据长度
	)
{
	NPC_BOOL										bRet;
	PNPC_S_DPS_MSG_BODY_MEDIA_E8_AUDIO_FRAME		pMsgBody;
	PNPC_BYTE										pMediaDataPos;
	NPC_INT											iMediaDataLen;
	PNPC_S_UMSP_CAMERA_DATA							pCameraData;
	NPC_BOOL										bIfLock;

	//INIT AREA
	{
		bRet = TRUE;
		pMsgBody = (PNPC_S_DPS_MSG_BODY_MEDIA_E8_AUDIO_FRAME)((PNPC_BYTE)in_pMsgHead + sizeof(NPC_S_DPS_MSG_HEAD));
		pMediaDataPos = (PNPC_BYTE)pMsgBody + sizeof(NPC_S_DPS_MSG_BODY_MEDIA_E8_AUDIO_FRAME);
		iMediaDataLen = in_iMsgDataLen - (
			sizeof(NPC_S_DPS_MSG_HEAD) + 
			sizeof(NPC_S_DPS_MSG_BODY_MEDIA_E8_AUDIO_FRAME) );

		in_pMsgHead->ucResult = 0;
		in_pMsgHead->ucMsgType = NPC_D_DPS_MSG_TYPE_RESPONSION;

		pCameraData = NULL;
		bIfLock = FALSE;
	}

	//CODE AREA
	{
		//printf("iMediaDataLen: %d\n", iMediaDataLen);

		/*if ( iMediaDataLen != 160 )
		{
			printf("iMediaDataLen: %d,  MediaData: %2.2x %2.2x %2.2x %2.2x \n", 
				iMediaDataLen, 
				pMediaDataPos[0], 
				pMediaDataPos[1], 
				pMediaDataPos[2], 
				pMediaDataPos[3]
				);

			FILE* f;
			NPC_CHAR sFileName[128];
			static int iFileNo = 0;

			sprintf(sFileName, "d:\\audio%d.pcm", iFileNo++);
			f = fopen(sFileName, "wb");
			if ( f )
			{
				fwrite(pMediaDataPos, 1, iMediaDataLen, f);

				fclose(f);
				f = NULL;
			}
		}

		{
			static FILE* f = fopen("d:\\audio_kkkkkkkkkkk.pcm", "wb");

			if ( f )
			{
				fwrite(pMediaDataPos, 1, iMediaDataLen, f);
			}
		}*/

		if ( iMediaDataLen <= 0 )
		{
			//写日志
			NPC_F_LOG_SR_WriteLog("[UMSP]  NPC_F_UMSP_MC_DoProEvent_MEDIA_E8_AUDIO_FRAME iMediaDataLen <= 0.");
			return;
		}

		//--------------------------------------------------------------------
		if ( pMsgBody->i_usChNo < 0 || pMsgBody->i_usChNo >= NPC_D_DPS_MAX_CHANNEL_NUM )
		{
			//写日志
			NPC_F_LOG_SR_WriteLog("[UMSP]  NPC_F_UMSP_MC_DoProEvent_MEDIA_E8_AUDIO_FRAME i_iChNo error.");

			in_pMsgHead->ucResult = NPC_D_MON_CSD_DPS_ERROR_CODE_CHANNEL_NO_ERROR;
			goto _NPC_CLEAR;
		}

		pCameraData = &(in_pMcSrvData->tCameraTable[pMsgBody->i_usChNo]);

		//加锁
		NPC_F_THREAD_Mutex_Lock(
			pCameraData->hWorkLock		//互斥锁句柄
			);
		bIfLock = TRUE;

		if ( pCameraData->bTalkIfOpen )
		{
			//输入对讲音频数据
			NPC_F_FILECAM_SDK_InputTalkAudioData(
				in_pMcSrvData->hFileCamera,				//文件摄像机句柄，由NPC_F_FILECAM_SDK_ConnectFilecam函数创建
				pMsgBody->i_usChNo,						//通道号
				pMediaDataPos,							//音频数据缓冲区
				iMediaDataLen							//音频数据长度
				);
		}
	}

	//CLEAR AREA
	_NPC_CLEAR:
	{
		if ( pCameraData && bIfLock )
		{
			//解锁
			NPC_F_THREAD_Mutex_Unlock(
				pCameraData->hWorkLock			//互斥锁句柄
				);
		}
	}
}


//-----------------------------------------------------------------------------------
//检测帧类型，返回帧类型，0：未检测到，1：I帧，2：P帧
NPC_INT		NPC_F_UMSP_MC_CheckFrameType(
	IN		PNPC_BYTE								in_pFrameDataBuf,			//帧数据缓冲区
	IN		NPC_INT									in_iFrameDataLen			//帧数据长度
	)
{
	PNPC_BYTE										pFindPos;
	PNPC_BYTE										pEndPos;
	NPC_BYTE										forbidden_zero_bit;
    NPC_BYTE										nal_ref_idc;
	NPC_BYTE										nal_unit_type;

	pFindPos = in_pFrameDataBuf;
	pEndPos = in_pFrameDataBuf + in_iFrameDataLen - 4;

	while ( pFindPos < pEndPos )
	{
		if ( *(PNPC_DWORD)pFindPos == 0x01000000 )
		{
			pFindPos += 4;
			
			forbidden_zero_bit = (*pFindPos & 0x80) >> 7;
			nal_ref_idc = (*pFindPos & 0x60) >> 5;
			nal_unit_type = (*pFindPos & 0x1F);

			switch ( nal_unit_type )
			{
			case 5:
			case 7:
			case 8:
				return 1; //1：I帧
			case 1:
				return 2; //2：P帧
			default:
				break;
			}
		}

		pFindPos++;
	}

	return 0;
}


//-----------------------------------------------------------------------------------
//开始录像
NPC_BOOL	NPC_F_UMSP_MC_StartRecord(
	IN		PNPC_S_UMSP_MCSRV_DATA					in_pMcSrvData,				//主控服务数据
	IN		PNPC_S_UMSP_CAMERA_DATA					in_pCameraData,				//摄像机数据
	IN		NPC_INT									in_iChNo,					//通道号
	IN		NPC_INT									in_iFrameRate				//帧率
	)
{
	NPC_CHAR										sRecFileName[256];
	NPC_CHAR										sTempTimeStr[20];
	NPC_CHAR										sCurTimeStr[20];
	NPC_S_NAV_AV_FORMAT								tAvFormat = {0};

	in_pCameraData->tRecStartTime = time(NULL);

	//将time_t类型的时间，格式化为YYYY-MM-DD HH24:MI:SS型的字符串
	NPC_F_SYS_Time_To_Str(
		in_pCameraData->tRecStartTime,		//时间值
		sTempTimeStr						//时间字符串
		);

	strftime(sCurTimeStr, 20, "%Y%m%d%H%M%S", localtime(&in_pCameraData->tRecStartTime));

	sprintf(sRecFileName, ".\\rec\\REC_%d_%s.nav", in_iChNo, sCurTimeStr);

	tAvFormat.bVedioFlag = TRUE;							//视频标志，TRUE表示有视频，FALSE表示没有视频
	tAvFormat.bAudioFlag = FALSE;							//音频标志，TRUE表示有音频，FALSE表示没有音频

	tAvFormat.iVedioCodecId = 1;							//视频编码参数，0：默认编码，1：H264，2：MPEG4，3：H263。。。
	tAvFormat.usVedioWidth = 352;							//视频宽度
	tAvFormat.usVedioHeight = 288;							//视频高度
	tAvFormat.iVedioBitRate = 128;							//视频码率参数，0表示为默认码率
	tAvFormat.iVedioFrameRate = in_iFrameRate;				//视频帧率参数，0表示为默认帧率

	//创建NAV文件
	in_pCameraData->hNavFile = NPC_F_NAV_CreateFile(
		sRecFileName,				//文件名
		1008,						//设备厂商
		0,							//录像编码格式，0：为标准格式，tRecFormat有效，其它为厂商原始格式，需要用厂商的SDK解码
		&tAvFormat,					//录像格式
		1000,						//时间戳参考值，默认为90000
		sTempTimeStr				//录制开始时间，格式为：YYYY-MM-DD HH:MM:SS
		);
	if ( in_pCameraData->hNavFile == NULL )
	{
		return FALSE;
	}

	return TRUE;
}

//停止录像
NPC_VOID	NPC_F_UMSP_MC_StopRecord(
	IN		PNPC_S_UMSP_MCSRV_DATA					in_pMcSrvData,				//主控服务数据
	IN		PNPC_S_UMSP_CAMERA_DATA					in_pCameraData				//摄像机数据
	)
{
	if ( in_pCameraData->hNavFile )
	{
		time_t tCurTime;
		NPC_CHAR sCurTimeStr[20];
		time_t dwRecTimeLen;

		tCurTime = time(NULL);

		//将time_t类型的时间，格式化为YYYY-MM-DD HH24:MI:SS型的字符串
		NPC_F_SYS_Time_To_Str(
			tCurTime,			//时间值
			sCurTimeStr			//时间字符串
			);

		dwRecTimeLen = tCurTime - in_pCameraData->tRecStartTime;

		//更新文件头信息
		NPC_F_NAV_UpdateFileHead_RecEndTime(
			in_pCameraData->hNavFile,		//NAV文件句柄
			sCurTimeStr,					//录制结束时间，格式为：YYYY-MM-DD HH:MM:SS
			dwRecTimeLen					//录制时长（秒）
			);

		//关闭NAV文件
		NPC_F_NAV_CloseFile(
			in_pCameraData->hNavFile				//NAV文件句柄
			);
		in_pCameraData->hNavFile = NULL;
	}
}


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
	)
{
	NPC_BOOL										bRet;
	HANDLE											hFind;
	WIN32_FIND_DATA									tFindFileData;
	NPC_CHAR										sFindPath[MAX_PATH];
	NPC_CHAR										TempFilePath[MAX_PATH];

	NPC_CHAR										sBeginTimeStr[20];
	NPC_CHAR										sEndTimeStr[20];

	NPC_CHAR										sBeginFileNameStr[128];
	NPC_CHAR										sEndFileNameStr[128];

	NPC_S_NAV_AV_FORMAT						tAvFormat;
	NPC_S_MON_MPS_FILE_INFO							tFileInfo;
	NPC_DWORD										dwRecTimeLen;
	NPC_INT											iFileNum;

	//INIT AREA
	{
		bRet = TRUE;
		hFind = INVALID_HANDLE_VALUE;
		*out_pFileNum = 0;
		*out_pFileInfoDataLen = 0;
		iFileNum = 0;
	}

	//CODE AREA
	{
		sprintf(sBeginTimeStr, "%4.4d%2.2d%2.2d%2.2d%2.2d%2.2d", 
			in_pStartTime->usYear,						//年
			in_pStartTime->usMonth,						//月
			in_pStartTime->ucDay,						//日
			in_pStartTime->ucHour,						//时
			in_pStartTime->ucMinute,					//分
			in_pStartTime->ucSecond						//秒
			);

		sprintf(sEndTimeStr, "%4.4d%2.2d%2.2d%2.2d%2.2d%2.2d", 
			in_pEndTime->usYear,						//年
			in_pEndTime->usMonth,						//月
			in_pEndTime->ucDay,							//日
			in_pEndTime->ucHour,						//时
			in_pEndTime->ucMinute,						//分
			in_pEndTime->ucSecond						//秒
			);

		sprintf(sFindPath, ".\\rec\\REC_%d_*.nav", in_iChNo);

		sprintf(sBeginFileNameStr, "REC_%d_%s.nav", in_iChNo, sBeginTimeStr);
		sprintf(sEndFileNameStr, "REC_%d_%s.nav", in_iChNo, sEndTimeStr);

		hFind = FindFirstFile(sFindPath, &tFindFileData);

		if ( hFind != INVALID_HANDLE_VALUE ) 
		{
			do{
				if ( !(tFindFileData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) )
				{
					if ( strcmp(tFindFileData.cFileName, sBeginFileNameStr) >= 0 && 
						strcmp(tFindFileData.cFileName, sEndFileNameStr) <= 0 )
					{
						sprintf(TempFilePath, ".\\rec\\%s", tFindFileData.cFileName);

						//读取录像文件信息
						if ( !NPC_F_UMSP_MC_ReadRecFileInfo(
							TempFilePath,				//文件名，包括路径
							&tAvFormat,					//文件格式
							sBeginTimeStr,				//录制开始时间，格式为：YYYY-MM-DD HH:MM:SS
							sEndTimeStr,				//录制结束时间，格式为：YYYY-MM-DD HH:MM:SS
							&dwRecTimeLen				//录制时长（秒）
							) )
						{
							//显示信息
							NPC_F_LOG_SR_ShowInfo("[UMSP]  Query record file fail, cause: Failed to read the video file information.");

							bRet = FALSE;
							goto _NPC_CLEAR;
						}

						//--------------------------------------------------------------------
						if ( dwRecTimeLen > 0 )
						{
							memset(&tFileInfo, 0, sizeof(tFileInfo));

							strcpy(tFileInfo.sFileFlag, tFindFileData.cFileName);			//文件标识，唯一标识该文件，打开文件时必须原样传回
							tFileInfo.iFileType = NPC_D_MON_CSD_FILE_TYPE_RECORD;			//文件类型，参见NPC_D_MON_CSD_FILE_TYPE_*定义
							tFileInfo.iCreateMode = NPC_D_MON_CSD_FILE_CREATE_MODE_TIMER;	//创建方式，参见NPC_D_MON_CSD_FILE_CREATE_MODE_*定义
							tFileInfo.iAlarmEvent = NPC_D_MON_CSD_ALARM_EVENT_ALL;			//报警事件，参见NPC_D_MON_CSD_ALARM_EVENT_*定义，只有创建方式为NPC_D_DPS_FILE_CREATE_MODE_ALARM时有效
							tFileInfo.dwFileSize = tFindFileData.nFileSizeLow / 1024;		//文件大小（单位：KB）

							//将字段串时间转换为NPC_S_TD_DATE_TIME时间
							NPC_F_SYS_StrTime_To_TdTime(
								sBeginTimeStr,					//时间字符串，格式为：YYYY-MM-DD HH24:MI:SS
								&tFileInfo.tStartTime			//TD时间
								);

							NPC_F_SYS_StrTime_To_TdTime(
								sEndTimeStr,					//时间字符串，格式为：YYYY-MM-DD HH24:MI:SS
								&tFileInfo.tEndTime				//TD时间
								);

							tFileInfo.dwFileTimeLen = dwRecTimeLen;

							tFileInfo.bVideoFlag = tAvFormat.bVedioFlag;					//视频标志，TRUE：表示有视频，FALSE：表示没有视频
							tFileInfo.bAudioFlag = tAvFormat.bAudioFlag;					//音频标志，TRUE：表示有音频，FALSE：表示没有音频
							tFileInfo.bPictureFlag = 0;										//图片标志，TRUE：表示为图片，FALSE：表示不是图片，如果该参数TRUE，则音视频参数无效

							tFileInfo.iVideoCodecId = NPC_D_MON_CSD_CODEC_ID_VEDIO_H264;	//视频编码ID，参见NPC_D_MON_CODEC_ID_*定义
							tFileInfo.iVideoFrameRate = tAvFormat.iVedioFrameRate;			//视频帧率
							tFileInfo.iVideoBitRate = tAvFormat.iVedioBitRate;				//视频码率（kbps）
							tFileInfo.usVideoWidth = tAvFormat.usVedioWidth;				//视频宽度
							tFileInfo.usVideoHeight = tAvFormat.usVedioHeight;				//视频高度

							tFileInfo.iAudioCodecId = NPC_D_MON_CSD_CODEC_ID_AUDIO_PCMA;	//音频编码ID，参见NPC_D_MON_CODEC_ID_*定义
							tFileInfo.iAudioBitRate = 64;									//音频码率
							tFileInfo.iAudioSampleRate = tAvFormat.iAudioSampleRate;		//音频采样率
							tFileInfo.iAudioChNum = 1;										//音频声道数

							//--------------------------------------------------------------------
							//向缓冲区添加数据
							if ( !NPC_F_MEM_MG_AddDataToBuf(
								out_ppFileInfoDataBuf,				//数据缓冲区指针
								out_pFileInfoDataBufSize,			//数据缓冲区大小
								out_pFileInfoDataLen,				//实际数据长度
								(PNPC_BYTE)&tFileInfo,				//添加数据指针
								sizeof(tFileInfo)					//添加数据长度
								) )
							{
								//显示信息
								NPC_F_LOG_SR_ShowInfo("[UMSP]  Query record file fail, cause: add data to buf fail.");

								bRet = FALSE;
								goto _NPC_CLEAR;
							}

							//--------------------------------------------------------------------
							iFileNum++;
						}
					}
				}
			}
			while( FindNextFile(hFind, &tFindFileData) != 0 );
		}

		*out_pFileNum = iFileNum;
	}

	//CLEAR AREA
	_NPC_CLEAR:
	{
		if ( hFind )
		{
			FindClose(hFind);
			hFind = NULL;
		}
	}

	return bRet;
}

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
	)
{
	NPC_BOOL										bRet;
	HANDLE											hFind;
	WIN32_FIND_DATA									tFindFileData;
	NPC_CHAR										sFindPath[MAX_PATH];
	NPC_CHAR										TempFilePath[MAX_PATH];

	NPC_CHAR										sBeginTimeStr[20];
	NPC_CHAR										sEndTimeStr[20];

	NPC_CHAR										sBeginFileNameStr[128];
	NPC_CHAR										sEndFileNameStr[128];

	NPC_S_NAV_AV_FORMAT								tAvFormat;
	NPC_S_MON_MPS_FILE_HEAD_INFO					tFileInfo;
	NPC_DWORD										dwRecTimeLen;
	NPC_INT											iFileNum;

	//INIT AREA
	{
		bRet = TRUE;
		hFind = INVALID_HANDLE_VALUE;
		*out_pFileNum = 0;
		*out_pFileInfoDataLen = 0;
		iFileNum = 0;
	}

	//CODE AREA
	{
		sprintf(sBeginTimeStr, "%4.4d%2.2d%2.2d%2.2d%2.2d%2.2d", 
			in_pStartTime->usYear,						//年
			in_pStartTime->usMonth,						//月
			in_pStartTime->ucDay,						//日
			in_pStartTime->ucHour,						//时
			in_pStartTime->ucMinute,					//分
			in_pStartTime->ucSecond						//秒
			);

		sprintf(sEndTimeStr, "%4.4d%2.2d%2.2d%2.2d%2.2d%2.2d", 
			in_pEndTime->usYear,						//年
			in_pEndTime->usMonth,						//月
			in_pEndTime->ucDay,							//日
			in_pEndTime->ucHour,						//时
			in_pEndTime->ucMinute,						//分
			in_pEndTime->ucSecond						//秒
			);

		sprintf(sFindPath, ".\\rec\\REC_%d_*.nav", in_iChNo);

		sprintf(sBeginFileNameStr, "REC_%d_%s.nav", in_iChNo, sBeginTimeStr);
		sprintf(sEndFileNameStr, "REC_%d_%s.nav", in_iChNo, sEndTimeStr);

		hFind = FindFirstFile(sFindPath, &tFindFileData);

		if ( hFind != INVALID_HANDLE_VALUE ) 
		{
			do{
				if ( !(tFindFileData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) )
				{
					if ( strcmp(tFindFileData.cFileName, sBeginFileNameStr) >= 0 && 
						strcmp(tFindFileData.cFileName, sEndFileNameStr) <= 0 )
					{
						sprintf(TempFilePath, ".\\rec\\%s", tFindFileData.cFileName);

						//读取录像文件信息
						if ( !NPC_F_UMSP_MC_ReadRecFileInfo(
							TempFilePath,				//文件名，包括路径
							&tAvFormat,					//文件格式
							sBeginTimeStr,				//录制开始时间，格式为：YYYY-MM-DD HH:MM:SS
							sEndTimeStr,				//录制结束时间，格式为：YYYY-MM-DD HH:MM:SS
							&dwRecTimeLen				//录制时长（秒）
							) )
						{
							//显示信息
							NPC_F_LOG_SR_ShowInfo("[UMSP]  Query record file fail, cause: Failed to read the video file information.");

							bRet = FALSE;
							goto _NPC_CLEAR;
						}

						//--------------------------------------------------------------------
						if ( dwRecTimeLen > 0 )
						{
							memset(&tFileInfo, 0, sizeof(tFileInfo));

							strcpy(tFileInfo.sFileFlag, tFindFileData.cFileName);			//文件标识，唯一标识该文件，打开文件时必须原样传回
							tFileInfo.usChNo = in_iChNo;									//通道号
							tFileInfo.ucStreamNo = in_iStreamNo;							//码流号
							tFileInfo.ucFileType = NPC_D_MON_CSD_FILE_TYPE_RECORD;			//文件类型，参见NPC_D_MON_CSD_FILE_TYPE_*定义
							tFileInfo.ucCreateMode = NPC_D_MON_CSD_FILE_CREATE_MODE_TIMER;	//创建方式，参见NPC_D_MON_CSD_FILE_CREATE_MODE_*定义
							tFileInfo.ucAlarmEvent = NPC_D_MON_CSD_ALARM_EVENT_ALL;			//报警事件，参见NPC_D_MON_CSD_ALARM_EVENT_*定义，只有创建方式为NPC_D_DPS_FILE_CREATE_MODE_ALARM时有效
							tFileInfo.uiFileSize = tFindFileData.nFileSizeLow / 1024;		//文件大小（单位：KB）

							//将字段串时间转换为NPC_S_TD_DATE_TIME时间
							NPC_F_SYS_StrTime_To_TdTime(
								sBeginTimeStr,					//时间字符串，格式为：YYYY-MM-DD HH24:MI:SS
								&tFileInfo.tStartTime			//TD时间
								);

							NPC_F_SYS_StrTime_To_TdTime(
								sEndTimeStr,					//时间字符串，格式为：YYYY-MM-DD HH24:MI:SS
								&tFileInfo.tEndTime				//TD时间
								);

							tFileInfo.uiFileTimeLen = dwRecTimeLen;

							tFileInfo.ucVideoFlag = tAvFormat.bVedioFlag;					//视频标志，TRUE：表示有视频，FALSE：表示没有视频
							tFileInfo.ucAudioFlag = tAvFormat.bAudioFlag;					//音频标志，TRUE：表示有音频，FALSE：表示没有音频

							tFileInfo.ucVideoCodecId = NPC_D_MON_CSD_CODEC_ID_VEDIO_H264;	//视频编码ID，参见NPC_D_MON_CODEC_ID_*定义
							tFileInfo.ucVideoFrameRate = tAvFormat.iVedioFrameRate;			//视频帧率
							tFileInfo.uiVideoBitRate = tAvFormat.iVedioBitRate;				//视频码率（kbps）
							tFileInfo.usVideoWidth = tAvFormat.usVedioWidth;				//视频宽度
							tFileInfo.usVideoHeight = tAvFormat.usVedioHeight;				//视频高度

							tFileInfo.usAudioCodecId = NPC_D_MON_CSD_CODEC_ID_AUDIO_PCMA;	//音频编码ID，参见NPC_D_MON_CODEC_ID_*定义
							tFileInfo.usAudioBitRate = 64;									//音频码率
							tFileInfo.uiAudioSampleRate = tAvFormat.iAudioSampleRate;		//音频采样率
							tFileInfo.usAudioChNum = 1;										//音频声道数

							//--------------------------------------------------------------------
							//向缓冲区添加数据
							if ( !NPC_F_MEM_MG_AddDataToBuf(
								out_ppFileInfoDataBuf,				//数据缓冲区指针
								out_pFileInfoDataBufSize,			//数据缓冲区大小
								out_pFileInfoDataLen,				//实际数据长度
								(PNPC_BYTE)&tFileInfo,				//添加数据指针
								sizeof(tFileInfo)					//添加数据长度
								) )
							{
								//显示信息
								NPC_F_LOG_SR_ShowInfo("[UMSP]  Query record file fail, cause: add data to buf fail.");

								bRet = FALSE;
								goto _NPC_CLEAR;
							}

							//--------------------------------------------------------------------
							iFileNum++;
						}
					}
				}
			}
			while( FindNextFile(hFind, &tFindFileData) != 0 );
		}

		*out_pFileNum = iFileNum;
	}

	//CLEAR AREA
	_NPC_CLEAR:
	{
		if ( hFind )
		{
			FindClose(hFind);
			hFind = NULL;
		}
	}

	return bRet;
}

//扩展查询文件
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
	)
{
	NPC_BOOL										bRet;
	HANDLE											hFind;
	WIN32_FIND_DATA									tFindFileData;
	NPC_CHAR										sFindPath[MAX_PATH];
	NPC_CHAR										TempFilePath[MAX_PATH];

	NPC_CHAR										sBeginTimeStr[20];
	NPC_CHAR										sEndTimeStr[20];

	NPC_CHAR										sBeginFileNameStr[128];
	NPC_CHAR										sEndFileNameStr[128];

	NPC_S_NAV_AV_FORMAT								tAvFormat;
	NPC_S_MON_MPS_FILE_RANGE_INFO					tFileInfo;
	NPC_DWORD										dwRecTimeLen;
	NPC_INT											iFileNum;

	//INIT AREA
	{
		bRet = TRUE;
		hFind = INVALID_HANDLE_VALUE;
		*out_pFileNum = 0;
		*out_pFileInfoDataLen = 0;
		iFileNum = 0;
	}

	//CODE AREA
	{
		sprintf(sBeginTimeStr, "%4.4d%2.2d%2.2d%2.2d%2.2d%2.2d", 
			in_pStartTime->usYear,						//年
			in_pStartTime->usMonth,						//月
			in_pStartTime->ucDay,						//日
			in_pStartTime->ucHour,						//时
			in_pStartTime->ucMinute,					//分
			in_pStartTime->ucSecond						//秒
			);

		sprintf(sEndTimeStr, "%4.4d%2.2d%2.2d%2.2d%2.2d%2.2d", 
			in_pEndTime->usYear,						//年
			in_pEndTime->usMonth,						//月
			in_pEndTime->ucDay,							//日
			in_pEndTime->ucHour,						//时
			in_pEndTime->ucMinute,						//分
			in_pEndTime->ucSecond						//秒
			);

		sprintf(sFindPath, ".\\rec\\REC_%d_*.nav", in_iChNo);

		sprintf(sBeginFileNameStr, "REC_%d_%s.nav", in_iChNo, sBeginTimeStr);
		sprintf(sEndFileNameStr, "REC_%d_%s.nav", in_iChNo, sEndTimeStr);

		hFind = FindFirstFile(sFindPath, &tFindFileData);

		if ( hFind != INVALID_HANDLE_VALUE ) 
		{
			do{
				if ( !(tFindFileData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) )
				{
					if ( strcmp(tFindFileData.cFileName, sBeginFileNameStr) >= 0 && 
						strcmp(tFindFileData.cFileName, sEndFileNameStr) <= 0 )
					{
						sprintf(TempFilePath, ".\\rec\\%s", tFindFileData.cFileName);

						//读取录像文件信息
						if ( !NPC_F_UMSP_MC_ReadRecFileInfo(
							TempFilePath,				//文件名，包括路径
							&tAvFormat,					//文件格式
							sBeginTimeStr,				//录制开始时间，格式为：YYYY-MM-DD HH:MM:SS
							sEndTimeStr,				//录制结束时间，格式为：YYYY-MM-DD HH:MM:SS
							&dwRecTimeLen				//录制时长（秒）
							) )
						{
							//显示信息
							NPC_F_LOG_SR_ShowInfo("[UMSP]  Query record file fail, cause: Failed to read the video file information.");

							bRet = FALSE;
							goto _NPC_CLEAR;
						}

						//--------------------------------------------------------------------
						if ( dwRecTimeLen > 0 )
						{
							memset(&tFileInfo, 0, sizeof(tFileInfo));

							tFileInfo.dwFileFlag = iFileNum + 1;				//标识
							tFileInfo.iCreateMode = NPC_D_MON_CSD_FILE_CREATE_MODE_TIMER;	//创建方式，参见NPC_D_MON_CSD_FILE_CREATE_M
							tFileInfo.iAlarmEvent = NPC_D_MON_CSD_ALARM_EVENT_ALL;			//报警事件，参见NPC_D_MON_CSD_ALARM_EVENT_*定义，只有创建方式为NPC_D_DPS_FILE_CREATE_MODE_ALARM时有效
							
#if 1
							if (tFileInfo.dwFileFlag == 3)
							{
								tFileInfo.iCreateMode = NPC_D_MON_CSD_FILE_CREATE_MODE_ALARM;	//创建方式，参见NPC_D_MON_CSD_FILE_CREATE_M
								tFileInfo.iAlarmEvent = NPC_D_MON_CSD_ALARM_EVENT_MOTION;		//报警事件，参见NPC_D_MON_CSD_ALARM_EVENT_*定义，只有创建方式为NPC_D_DPS_FILE_CREATE_MODE_ALARM时有效

							}
#endif
							//将字段串时间转换为NPC_S_TD_DATE_TIME时间
							NPC_F_SYS_StrTime_To_TdTime(
								sBeginTimeStr,					//时间字符串，格式为：YYYY-MM-DD HH24:MI:SS
								&tFileInfo.tStartTime			//TD时间
								);

							NPC_F_SYS_StrTime_To_TdTime(
								sEndTimeStr,					//时间字符串，格式为：YYYY-MM-DD HH24:MI:SS
								&tFileInfo.tEndTime				//TD时间
								);


							//--------------------------------------------------------------------
							//向缓冲区添加数据
							if ( !NPC_F_MEM_MG_AddDataToBuf(
								out_ppFileInfoDataBuf,				//数据缓冲区指针
								out_pFileInfoDataBufSize,			//数据缓冲区大小
								out_pFileInfoDataLen,				//实际数据长度
								(PNPC_BYTE)&tFileInfo,				//添加数据指针
								sizeof(tFileInfo)					//添加数据长度
								) )
							{
								//显示信息
								NPC_F_LOG_SR_ShowInfo("[UMSP]  Query record file fail, cause: add data to buf fail.");

								bRet = FALSE;
								goto _NPC_CLEAR;
							}

							//--------------------------------------------------------------------
							iFileNum++;
						}
					}
				}
			}
			while( FindNextFile(hFind, &tFindFileData) != 0 );
		}

		*out_pFileNum = iFileNum;
	}

	//CLEAR AREA
	_NPC_CLEAR:
	{
		if ( hFind )
		{
			FindClose(hFind);
			hFind = NULL;
		}
	}

	return bRet;
}

//查询文件,获取第一个文件信息
NPC_BOOL	NPC_F_UMSP_MC_NspFindFirstFile(
	IN		PNPC_S_UMSP_MCSRV_DATA					in_pMcSrvData,				//主控服务数据
	IN		NPC_INT									in_iChNo,					//通道号
	IN		NPC_INT									in_iStreamNo,				//码流号
	IN		PNPC_S_TD_DATE_TIME						in_pStartTime,				//开始时间
	IN		PNPC_S_TD_DATE_TIME						in_pEndTime,				//结束时间
	INOUT	PNPC_S_MON_MPS_FILE_HEAD_INFO			io_pFileInfo				//文件信息
	)
{
	NPC_BOOL										bRet;
	HANDLE											hFind;
	WIN32_FIND_DATA									tFindFileData;
	NPC_CHAR										sFindPath[MAX_PATH];
	NPC_CHAR										TempFilePath[MAX_PATH];

	NPC_CHAR										sBeginTimeStr[20];
	NPC_CHAR										sEndTimeStr[20];

	NPC_CHAR										sBeginFileNameStr[128];
	NPC_CHAR										sEndFileNameStr[128];

	NPC_S_NAV_AV_FORMAT								tAvFormat;
	NPC_DWORD										dwRecTimeLen;

	//INIT AREA
	{
		bRet = TRUE;
		hFind = INVALID_HANDLE_VALUE;;
	}

	//CODE AREA
	{
		sprintf(sBeginTimeStr, "%4.4d%2.2d%2.2d%2.2d%2.2d%2.2d", 
			in_pStartTime->usYear,						//年
			in_pStartTime->usMonth,						//月
			in_pStartTime->ucDay,						//日
			in_pStartTime->ucHour,						//时
			in_pStartTime->ucMinute,					//分
			in_pStartTime->ucSecond						//秒
			);

		sprintf(sEndTimeStr, "%4.4d%2.2d%2.2d%2.2d%2.2d%2.2d", 
			in_pEndTime->usYear,						//年
			in_pEndTime->usMonth,						//月
			in_pEndTime->ucDay,							//日
			in_pEndTime->ucHour,						//时
			in_pEndTime->ucMinute,						//分
			in_pEndTime->ucSecond						//秒
			);

		sprintf(sFindPath, ".\\rec\\REC_%d_*.nav", in_iChNo);

		sprintf(sBeginFileNameStr, "REC_%d_%s.nav", in_iChNo, sBeginTimeStr);
		sprintf(sEndFileNameStr, "REC_%d_%s.nav", in_iChNo, sEndTimeStr);

		hFind = FindFirstFile(sFindPath, &tFindFileData);

		if ( hFind != INVALID_HANDLE_VALUE ) 
		{
			if ( !(tFindFileData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) )
			{
				if ( strcmp(tFindFileData.cFileName, sBeginFileNameStr) >= 0 && 
					strcmp(tFindFileData.cFileName, sEndFileNameStr) <= 0 )
				{
					sprintf(TempFilePath, ".\\rec\\%s", tFindFileData.cFileName);
					
					//读取录像文件信息
					if ( !NPC_F_UMSP_MC_ReadRecFileInfo(
						TempFilePath,				//文件名，包括路径
						&tAvFormat,					//文件格式
						sBeginTimeStr,				//录制开始时间，格式为：YYYY-MM-DD HH:MM:SS
						sEndTimeStr,				//录制结束时间，格式为：YYYY-MM-DD HH:MM:SS
						&dwRecTimeLen				//录制时长（秒）
						) )
					{
						//显示信息
						NPC_F_LOG_SR_ShowInfo("[UMSP]  Query record file fail, cause: Failed to read the video file information.");
						
						bRet = FALSE;
						goto _NPC_CLEAR;
					}
					
					//--------------------------------------------------------------------
					if ( dwRecTimeLen > 0 )
					{
						memset(io_pFileInfo, 0, sizeof(NPC_S_MON_MPS_FILE_HEAD_INFO));
						
						strcpy(io_pFileInfo->sFileFlag, tFindFileData.cFileName);			//文件标识，唯一标识该文件，打开文件时必须原样传回
						io_pFileInfo->usChNo = in_iChNo;									//通道号
						io_pFileInfo->ucStreamNo = in_iStreamNo;							//码流号
						io_pFileInfo->ucFileType = NPC_D_MON_CSD_FILE_TYPE_RECORD;			//文件类型，参见NPC_D_MON_CSD_FILE_TYPE_*定义
						io_pFileInfo->ucCreateMode = NPC_D_MON_CSD_FILE_CREATE_MODE_TIMER;	//创建方式，参见NPC_D_MON_CSD_FILE_CREATE_MODE_*定义
						io_pFileInfo->ucAlarmEvent = NPC_D_MON_CSD_ALARM_EVENT_ALL;			//报警事件，参见NPC_D_MON_CSD_ALARM_EVENT_*定义，只有创建方式为NPC_D_DPS_FILE_CREATE_MODE_ALARM时有效
						io_pFileInfo->uiFileSize = tFindFileData.nFileSizeLow / 1024;		//文件大小（单位：KB）
						
						//将字段串时间转换为NPC_S_TD_DATE_TIME时间
						NPC_F_SYS_StrTime_To_TdTime(
							sBeginTimeStr,					//时间字符串，格式为：YYYY-MM-DD HH24:MI:SS
							&io_pFileInfo->tStartTime			//TD时间
							);
						
						NPC_F_SYS_StrTime_To_TdTime(
							sEndTimeStr,					//时间字符串，格式为：YYYY-MM-DD HH24:MI:SS
							&io_pFileInfo->tEndTime				//TD时间
							);
						
						io_pFileInfo->uiFileTimeLen = dwRecTimeLen;
						
						io_pFileInfo->ucVideoFlag = tAvFormat.bVedioFlag;					//视频标志，TRUE：表示有视频，FALSE：表示没有视频
						io_pFileInfo->ucAudioFlag = tAvFormat.bAudioFlag;					//音频标志，TRUE：表示有音频，FALSE：表示没有音频
						
						io_pFileInfo->ucVideoCodecId = NPC_D_MON_CSD_CODEC_ID_VEDIO_H264;	//视频编码ID，参见NPC_D_MON_CODEC_ID_*定义
						io_pFileInfo->ucVideoFrameRate = tAvFormat.iVedioFrameRate;			//视频帧率
						io_pFileInfo->uiVideoBitRate = tAvFormat.iVedioBitRate;				//视频码率（kbps）
						io_pFileInfo->usVideoWidth = tAvFormat.usVedioWidth;				//视频宽度
						io_pFileInfo->usVideoHeight = tAvFormat.usVedioHeight;				//视频高度
						
						io_pFileInfo->usAudioCodecId = NPC_D_MON_CSD_CODEC_ID_AUDIO_PCMA;	//音频编码ID，参见NPC_D_MON_CODEC_ID_*定义
						io_pFileInfo->usAudioBitRate = 64;									//音频码率
						io_pFileInfo->uiAudioSampleRate = tAvFormat.iAudioSampleRate;		//音频采样率
						io_pFileInfo->usAudioChNum = 1;										//音频声道数
					}
				}
			}
		}
	}

	//CLEAR AREA
	_NPC_CLEAR:
	{
		if ( hFind )
		{
			FindClose(hFind);
			hFind = NULL;
		}
	}

	return bRet;
}

//读取录像文件信息
NPC_BOOL	NPC_F_UMSP_MC_ReadRecFileInfo(
	IN		PNPC_CHAR								in_pFileName,			//文件名，包括路径
	OUT		PNPC_S_NAV_AV_FORMAT					out_pAvFormat,			//文件格式
	OUT		PNPC_CHAR								out_pRecBeginTime,		//录制开始时间，格式为：YYYY-MM-DD HH:MM:SS
	OUT		PNPC_CHAR								out_pRecEndTime,		//录制结束时间，格式为：YYYY-MM-DD HH:MM:SS
	OUT		PNPC_DWORD								out_pRecTimeLen			//录制时长（秒）
	)
{
	NPC_HANDLE										hRecFileHandle;

	NPC_INT											iDevVendor;				//设备厂商
	NPC_INT											iRecCodecFormat;		//录像编码格式，0：为标准格式，tRecFormat有效，其它为厂商原始格式，需要用厂商的SDK解码
	NPC_DWORD										dwTimestampRef;			//时间戳参考值，默认为90000

	//打开NAV文件
	hRecFileHandle = NPC_F_NAV_OpenFile(
		in_pFileName,				//文件名
		&iDevVendor,				//设备厂商
		&iRecCodecFormat,			//录像编码格式，0：为标准格式，tRecFormat有效，其它为厂商原始格式，需要用厂商的SDK解码
		out_pAvFormat,				//录像格式
		&dwTimestampRef,			//时间戳参考值，默认为90000
		out_pRecBeginTime,			//录制开始时间，格式为：YYYY-MM-DD HH:MM:SS
		out_pRecEndTime,			//录制结束时间，格式为：YYYY-MM-DD HH:MM:SS
		out_pRecTimeLen				//录制时长（秒）
		);
	if ( hRecFileHandle == NULL )
	{
		//写日志
		NPC_F_LOG_SR_WriteLog("[UMSP]  NPC_F_UMSP_MC_ReadRecFileInfo NPC_F_NAV_OpenFile fail.");
		return FALSE;
	}

	//关闭NAV文件
	NPC_F_NAV_CloseFile(
		hRecFileHandle					//NAV文件句柄
		);
	hRecFileHandle = NULL;

	return TRUE;
}



//-----------------------------------------------------------------------------------
//打开文件，返回码为NPC_D_MON_CSD_DPS_ERROR_CODE_*
NPC_INT		NPC_F_UMSP_MC_File_OpenFile(
	IN		PNPC_S_UMSP_MCSRV_DATA					in_pMcSrvData,				//主控服务数据
	IN		PNPC_CHAR								in_pFileFlag,				//文件标识，唯一标识该文件，打开文件时必须原样传回
	IN		NPC_DWORD								in_dwStartOffsetDataPos,	//开始读取数据位置（单位字节），指原始文件的读取位置
	IN		NPC_DWORD								in_dwStartOffsetTimePos,	//开始读取时间位置（单位秒），为相对时间，如果值大于文件总时长，则打开失败
	OUT		PNPC_DWORD								out_pFileReadId,			//返回文件读取ID
	OUT		PNPC_S_MON_MPS_FILE_INFO				out_pFileInfo				//文件信息
	)
{
	NPC_INT											iRet;
	PNPC_S_UMSP_FILE_WORK_DATA						pFileWorkData;

	NPC_INT											iDevVendor;
	NPC_INT											iRecCodecFormat;
	NPC_S_NAV_AV_FORMAT								tAvFormat;
	NPC_DWORD										dwTimestampRef;
	NPC_CHAR										sRecBeginTime[20];
	NPC_CHAR										sRecEndTime[20];
	NPC_DWORD										dwRecTimeLen;

	NPC_LONG										lFileSize;
	NPC_CHAR										sFilePath[256];

	NPC_INT											i;
	NPC_INT											iIndex;
	NPC_INT											iUseNum;

	//INIT AREA
	{
		iRet = 0;
		pFileWorkData = NULL;
	}

	//CODE AREA
	{
		//统计数量
		iUseNum = 0;
		for ( i=0; i<NPC_D_UMSP_MC_MAX_FILE_WORK_NUM; i++ )
		{
			if ( in_pMcSrvData->tFileWorkTable[i].bIfUsed )
			{
				iUseNum++;
			}
		}
		if ( iUseNum >= in_pMcSrvData->tConfig.iMaxFileConnNum )
		{
			//写日志
			NPC_F_LOG_SR_WriteLog("[UMSP]  NPC_F_UMSP_MC_File_OpenFile fail, cause: Maximum limit.");
			return NPC_D_MON_CSD_DPS_ERROR_CODE_RESOURCE_NO_IDLE;
		}

		//查找空闲位置
		iIndex = -1;
		for ( i=0; i<NPC_D_UMSP_MC_MAX_FILE_WORK_NUM; i++ )
		{
			if ( !in_pMcSrvData->tFileWorkTable[i].bIfUsed )
			{
				pFileWorkData = &in_pMcSrvData->tFileWorkTable[i];
				iIndex = i;
				break;
			}
		}
		if ( pFileWorkData == NULL )
		{
			//写日志
			NPC_F_LOG_SR_WriteLog("[UMSP]  NPC_F_UMSP_MC_File_OpenFile no idle pos.");
			return NPC_D_MON_CSD_DPS_ERROR_CODE_RESOURCE_NO_IDLE;
		}

		pFileWorkData->bIfUsed = TRUE;
		pFileWorkData->dwFileReadId = (rand()<<16) | iIndex;
		if ( pFileWorkData->dwFileReadId == 0 )
			pFileWorkData->dwFileReadId = 1 | iIndex;

		sprintf(sFilePath, ".\\rec\\%s", in_pFileFlag);

		//计算文件大小
		lFileSize = NPC_F_UMSP_MC_ComputeFileSize(
			sFilePath					//文件名（包括路径）
			);
		if ( lFileSize == -1 )
		{
			//写日志
			NPC_F_LOG_SR_WriteLog("[UMSP]  NPC_F_UMSP_MC_File_OpenFile NPC_F_UMSP_MC_ComputeFileSize fail.");

			iRet = NPC_D_MON_CSD_DPS_ERROR_CODE_RESOURCE_OPEN_FAIL;
			goto _NPC_CLEAR;
		}

		//打开NAV文件
		pFileWorkData->hNavFile = NPC_F_NAV_OpenFile(
			sFilePath,					//文件名
			&iDevVendor,				//设备厂商
			&iRecCodecFormat,			//录像编码格式，0：为标准格式，tRecFormat有效，其它为厂商原始格式，需要用厂商的SDK解码
			&tAvFormat,					//录像格式
			&dwTimestampRef,			//时间戳参考值，默认为90000
			sRecBeginTime,				//录制开始时间，格式为：YYYY-MM-DD HH:MM:SS
			sRecEndTime,				//录制结束时间，格式为：YYYY-MM-DD HH:MM:SS
			&dwRecTimeLen				//录制时长（秒）
			);
		if ( pFileWorkData->hNavFile == NULL )
		{
			//写日志
			NPC_F_LOG_SR_WriteLog("[UMSP]  NPC_F_UMSP_MC_File_OpenFile NPC_F_NAV_OpenFile fail.");

			iRet = NPC_D_MON_CSD_DPS_ERROR_CODE_RESOURCE_OPEN_FAIL;
			goto _NPC_CLEAR;
		}

		if ( dwRecTimeLen > 0 )
		{
			if ( in_dwStartOffsetTimePos >= dwRecTimeLen )
			{
				//写日志
				NPC_F_LOG_SR_WriteLog("[UMSP]  NPC_F_UMSP_MC_File_OpenFile in_dwStartOffsetTimePos fail.");

				iRet = NPC_D_MON_CSD_DPS_ERROR_CODE_PARAM_ERROR;
				goto _NPC_CLEAR;
			}

			//跳转到指定位置
			if ( !NPC_F_NAV_JumpCurPos(
				pFileWorkData->hNavFile,		//NAV文件句柄
				in_dwStartOffsetTimePos			//跳转到指定位置，该参数录像偏移时间（秒），相对于文件头的时长
				) )
			{
				//写日志
				NPC_F_LOG_SR_WriteLog("[UMSP]  NPC_F_UMSP_MC_File_OpenFile NPC_F_NAV_JumpCurPos fail.");

				iRet = NPC_D_MON_CSD_DPS_ERROR_CODE_OTHER;
				goto _NPC_CLEAR;
			}
		}

		*out_pFileReadId = pFileWorkData->dwFileReadId;

		memset(out_pFileInfo, 0, sizeof(NPC_S_MON_MPS_FILE_INFO));

		strcpy(out_pFileInfo->sFileFlag, in_pFileFlag);						//文件标识，唯一标识该文件，打开文件时必须原样传回
		out_pFileInfo->iFileType = NPC_D_MON_CSD_FILE_TYPE_RECORD;			//文件类型，参见NPC_D_MON_CSD_FILE_TYPE_*定义
		out_pFileInfo->iCreateMode = NPC_D_MON_CSD_FILE_CREATE_MODE_TIMER;	//创建方式，参见NPC_D_MON_CSD_FILE_CREATE_MODE_*定义
		out_pFileInfo->iAlarmEvent = NPC_D_MON_CSD_ALARM_EVENT_ALL;			//报警事件，参见NPC_D_MON_CSD_ALARM_EVENT_*定义，只有创建方式为NPC_D_MON_CSD_FILE_CREATE_MODE_ALARM时有效
		out_pFileInfo->dwFileTimeLen = dwRecTimeLen;						//文件总时长（秒），录像文件有效
		out_pFileInfo->dwFileSize = lFileSize/1024;							//文件大小（单位：KB）

		//将字段串时间转换为NPC_S_TD_DATE_TIME时间
		NPC_F_SYS_StrTime_To_TdTime(
			sRecBeginTime,					//时间字符串，格式为：YYYY-MM-DD HH24:MI:SS
			&out_pFileInfo->tStartTime		//TD时间
			);

		NPC_F_SYS_StrTime_To_TdTime(
			sRecEndTime,					//时间字符串，格式为：YYYY-MM-DD HH24:MI:SS
			&out_pFileInfo->tEndTime		//TD时间
			);

		out_pFileInfo->bVideoFlag = tAvFormat.bVedioFlag;					//视频标志，TRUE：表示有视频，FALSE：表示没有视频
		out_pFileInfo->bAudioFlag = tAvFormat.bAudioFlag;;					//音频标志，TRUE：表示有音频，FALSE：表示没有音频
		out_pFileInfo->bPictureFlag = 0;									//图片标志，TRUE：表示为图片，FALSE：表示不是图片，如果该参数TRUE，则音视频参数无效

		out_pFileInfo->iVideoCodecId = NPC_D_MON_CSD_CODEC_ID_VEDIO_H264;	//视频编码ID，参见NPC_D_MON_CODEC_ID_*定义
		out_pFileInfo->iVideoFrameRate = tAvFormat.iVedioFrameRate;			//视频帧率
		out_pFileInfo->iVideoBitRate = tAvFormat.iVedioBitRate;				//视频码率（kbps）
		out_pFileInfo->usVideoWidth = tAvFormat.usVedioWidth;				//视频宽度
		out_pFileInfo->usVideoHeight = tAvFormat.usVedioHeight;				//视频高度

		out_pFileInfo->iAudioCodecId = NPC_D_MON_CSD_CODEC_ID_AUDIO_PCMA;	//音频编码ID，参见NPC_D_MON_CODEC_ID_*定义
		out_pFileInfo->iAudioBitRate = 64;									//音频码率
		out_pFileInfo->iAudioSampleRate = tAvFormat.iAudioSampleRate;		//音频采样率
		out_pFileInfo->iAudioChNum = 1;										//音频声道数

		//--------------------------------------------------------------------
		pFileWorkData->iFileVideoFrameRate = tAvFormat.iVedioFrameRate;
		pFileWorkData->dwFileTimeLen = dwRecTimeLen;
		pFileWorkData->tFileBeginRecTime = NPC_F_SYS_Str_To_Time(sRecBeginTime);
	}

	//CLEAR AREA
	_NPC_CLEAR:
	{
		if ( iRet != 0 && pFileWorkData )
		{
			if ( pFileWorkData->hNavFile )
			{
				//关闭NAV文件
				NPC_F_NAV_CloseFile(
					pFileWorkData->hNavFile		//NAV文件句柄
					);
				pFileWorkData->hNavFile = NULL;
			}

			memset(pFileWorkData, 0, sizeof(NPC_S_UMSP_FILE_WORK_DATA));
		}
	}

	return iRet;
}

//关闭文件
NPC_VOID	NPC_F_UMSP_MC_File_CloseFile(
	IN		PNPC_S_UMSP_MCSRV_DATA					in_pMcSrvData,				//主控服务数据
	IN		NPC_DWORD								in_dwFileReadId				//文件读取ID
	)
{
	NPC_INT											iIndex;
	PNPC_S_UMSP_FILE_WORK_DATA						pFileWorkData;

	iIndex = in_dwFileReadId & 0xFFFF;
	if ( iIndex >= NPC_D_UMSP_MC_MAX_FILE_WORK_NUM )
		return;

	pFileWorkData = &(in_pMcSrvData->tFileWorkTable[iIndex]);
	if ( !pFileWorkData->bIfUsed )
		return;

	if ( pFileWorkData->dwFileReadId != in_dwFileReadId )
		return;

	if ( pFileWorkData->hNavFile )
	{
		//关闭NAV文件
		NPC_F_NAV_CloseFile(
			pFileWorkData->hNavFile		//NAV文件句柄
			);
		pFileWorkData->hNavFile = NULL;
	}

	memset(pFileWorkData, 0, sizeof(NPC_S_UMSP_FILE_WORK_DATA));
}

//定位文件，返回码为NPC_D_MON_CSD_DPS_ERROR_CODE_*
NPC_INT		NPC_F_UMSP_MC_File_SeekFile(
	IN		PNPC_S_UMSP_MCSRV_DATA					in_pMcSrvData,				//主控服务数据
	IN		NPC_DWORD								in_dwFileReadId,			//文件读取ID
	IN		NPC_DWORD								in_dwOffsetTimePos			//读取时间位置（单位秒），为相对时间
	)
{
	NPC_INT											iIndex;
	PNPC_S_UMSP_FILE_WORK_DATA						pFileWorkData;

	iIndex = in_dwFileReadId & 0xFFFF;
	if ( iIndex >= NPC_D_UMSP_MC_MAX_FILE_WORK_NUM )
	{
		//写日志
		NPC_F_LOG_SR_WriteLog("[UMSP]  NPC_F_UMSP_MC_File_SeekFile in_dwFileReadId fail.");
		return NPC_D_MON_CSD_DPS_ERROR_CODE_ID_ERROR;
	}

	pFileWorkData = &(in_pMcSrvData->tFileWorkTable[iIndex]);
	if ( !pFileWorkData->bIfUsed )
	{
		//写日志
		NPC_F_LOG_SR_WriteLog("[UMSP]  NPC_F_UMSP_MC_File_SeekFile bIfUsed fail.");
		return NPC_D_MON_CSD_DPS_ERROR_CODE_ID_ERROR;
	}

	if ( pFileWorkData->dwFileReadId != in_dwFileReadId )
	{
		//写日志
		NPC_F_LOG_SR_WriteLog("[UMSP]  NPC_F_UMSP_MC_File_SeekFile in_dwFileReadId error.");
		return NPC_D_MON_CSD_DPS_ERROR_CODE_ID_ERROR;
	}

	if ( pFileWorkData->hNavFile == NULL )
	{
		//写日志
		NPC_F_LOG_SR_WriteLog("[UMSP]  NPC_F_UMSP_MC_File_SeekFile hNavFile fail.");
		return NPC_D_MON_CSD_DPS_ERROR_CODE_RESOURCE_NOT_OPEN;
	}

	if ( in_dwOffsetTimePos >= pFileWorkData->dwFileTimeLen )
	{
		//写日志
		NPC_F_LOG_SR_WriteLog("[UMSP]  NPC_F_UMSP_MC_File_SeekFile in_dwOffsetTimePos fail.");
		return NPC_D_MON_CSD_DPS_ERROR_CODE_PARAM_ERROR;
	}

	//跳转到指定位置
	if ( !NPC_F_NAV_JumpCurPos(
		pFileWorkData->hNavFile,		//NAV文件句柄
		in_dwOffsetTimePos				//跳转到指定位置，该参数录像偏移时间（秒），相对于文件头的时长
		) )
	{
		//写日志
		NPC_F_LOG_SR_WriteLog("[UMSP]  NPC_F_UMSP_MC_File_SeekFile NPC_F_NAV_JumpCurPos fail.");
		return NPC_D_MON_CSD_DPS_ERROR_CODE_FILE_OP_FAIL;
	}

	return NPC_D_MON_CSD_DPS_ERROR_CODE_SUCCESS;
}

//获取文件的视频参数
NPC_BOOL	NPC_F_UMSP_MC_File_GetFileVideoParam(
	IN		PNPC_S_UMSP_MCSRV_DATA					in_pMcSrvData,				//主控服务数据
	IN		NPC_DWORD								in_dwFileReadId,			//文件读取ID
	OUT		PNPC_INT								out_pFrameRate,				//帧率
	OUT		PNPC_INT								out_pGOP					//GOP
	)
{
	NPC_INT											iIndex;
	PNPC_S_UMSP_FILE_WORK_DATA						pFileWorkData;

	iIndex = in_dwFileReadId & 0xFFFF;
	if ( iIndex >= NPC_D_UMSP_MC_MAX_FILE_WORK_NUM )
	{
		//写日志
		NPC_F_LOG_SR_WriteLog("[UMSP]  NPC_F_UMSP_MC_File_GetFileVideoParam in_dwFileReadId fail.");
		return FALSE;
	}

	pFileWorkData = &(in_pMcSrvData->tFileWorkTable[iIndex]);
	if ( !pFileWorkData->bIfUsed )
	{
		//写日志
		NPC_F_LOG_SR_WriteLog("[UMSP]  NPC_F_UMSP_MC_File_GetFileVideoParam bIfUsed fail.");
		return FALSE;
	}

	if ( pFileWorkData->dwFileReadId != in_dwFileReadId )
	{
		//写日志
		NPC_F_LOG_SR_WriteLog("[UMSP]  NPC_F_UMSP_MC_File_GetFileVideoParam in_dwFileReadId error.");
		return FALSE;
	}

	*out_pFrameRate = pFileWorkData->iFileVideoFrameRate;
	*out_pGOP = *out_pFrameRate * 2; //由于录像文件中没有GOP参数，所以这里固定写一个值

	return TRUE;
}

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
	)
{
	NPC_BOOL										bRet;
	PNPC_BYTE										pSendDataBuf;
	NPC_INT											iSendDataLen;
	PNPC_S_DPS_MSG_HEAD								pSendMsgHead;
	PNPC_S_DPS_MSG_BODY_EVENT_E2_FILE_READ			pSendMsgBody;

	//INIT AREA
	{
		bRet = TRUE;
		pSendDataBuf = NULL;
		iSendDataLen = 0;
	}

	//CODE AREA
	{
		if ( in_pMediaDataBuf == NULL || in_iMediaDataLen < 0 )
			in_iMediaDataLen = 0;

		iSendDataLen = in_iMsgDataLen + in_iMediaDataLen;
		pSendDataBuf = (PNPC_BYTE)malloc(iSendDataLen);
		if ( pSendDataBuf == NULL )
		{
			bRet = FALSE;
			goto _NPC_CLEAR;
		}

		pSendMsgHead = (PNPC_S_DPS_MSG_HEAD)pSendDataBuf;
		pSendMsgBody = (PNPC_S_DPS_MSG_BODY_EVENT_E2_FILE_READ)((PNPC_BYTE)pSendMsgHead + sizeof(NPC_S_DPS_MSG_HEAD));

		memcpy(pSendDataBuf, in_pMsgDataBuf, in_iMsgDataLen);

		if ( in_pMediaDataBuf && in_iMediaDataLen > 0 )
			memcpy(pSendDataBuf+in_iMsgDataLen, in_pMediaDataBuf, in_iMediaDataLen);

		pSendMsgHead->iMsgBodyLen = iSendDataLen - sizeof(NPC_S_DPS_MSG_HEAD);

		pSendMsgBody->o_ucRetDataType = in_iMediaType + 1;				//返回数据类型，1：视频，2：音频，3：图片数据
		if ( in_iMediaType == 0 )
		{
			switch ( in_cFrameType )
			{
			case 'I':
				pSendMsgBody->o_ucFrameType = NPC_D_MON_CSD_VIDEO_FRAME_TYPE_I;				//帧类型，视频有效，参见NPC_D_MON_CSD_VIDEO_FRAME_TYPE_*定义
				break;
			case 'P':
				pSendMsgBody->o_ucFrameType = NPC_D_MON_CSD_VIDEO_FRAME_TYPE_P;
				break;
			case 'B':
				pSendMsgBody->o_ucFrameType = NPC_D_MON_CSD_VIDEO_FRAME_TYPE_B;
				break;
			default:
				pSendMsgBody->o_ucFrameType = NPC_D_MON_CSD_VIDEO_FRAME_TYPE_P;
				break;
			}

			pSendMsgBody->o_ucMediaCodecId = NPC_D_MON_CSD_CODEC_ID_VEDIO_H264;			//媒体编码格式，参见NPC_D_MON_CSD_CODEC_ID_*定义
			pSendMsgBody->o_ucVideoFrameRate = in_pFileWorkData->iFileVideoFrameRate;	//视频帧率（为视频帧时有效）
		}
		else
		{
			pSendMsgBody->o_ucMediaCodecId = NPC_D_MON_CSD_CODEC_ID_AUDIO_PCMA;			//媒体编码格式，参见NPC_D_MON_CSD_CODEC_ID_*定义
		}

		pSendMsgBody->o_ucIfDataEnd = in_iEndFlag;									//是否结束，0：未结束，1：本次请求结束，2：文件结束
		pSendMsgBody->o_dwOffsetDataPos = 0;										//读取数据位置（单位字节），指原始文件的读取位置
		pSendMsgBody->o_dwOffsetTimePos = in_dwRecOffsetTime;						//读取时间位置（单位秒），为相对时间
		pSendMsgBody->o_dwDataLen = in_iMediaDataLen;								//数据长度

		//将time_t转换为NPC_S_TD_DATE_TIME时间
		NPC_F_SYS_Timet_To_TdTime(
			in_pFileWorkData->tFileBeginRecTime + in_dwRecOffsetTime,		//时间值
			&pSendMsgBody->o_tDataTime										//TD时间
			);

		/*//------------------------------------------------------------------------------------
		NPC_CHAR sTempLogBuf[2000];

		//写日志
		NPC_F_LOG_SR_WriteLog("=====================================.");

		sprintf(sTempLogBuf, "i_ucReqDataType: %d", pSendMsgBody->i_ucReqDataType);
		NPC_F_LOG_SR_WriteLog(sTempLogBuf);

		sprintf(sTempLogBuf, "i_ucReqDataSize: %d", pSendMsgBody->i_ucReqDataSize);
		NPC_F_LOG_SR_WriteLog(sTempLogBuf);

		sprintf(sTempLogBuf, "o_ucRetDataType: %d", pSendMsgBody->o_ucRetDataType);
		NPC_F_LOG_SR_WriteLog(sTempLogBuf);

		sprintf(sTempLogBuf, "o_ucFrameType: %d", pSendMsgBody->o_ucFrameType);
		NPC_F_LOG_SR_WriteLog(sTempLogBuf);

		sprintf(sTempLogBuf, "o_ucIfDataEnd: %d", pSendMsgBody->o_ucIfDataEnd);
		NPC_F_LOG_SR_WriteLog(sTempLogBuf);

		sprintf(sTempLogBuf, "o_ucMediaCodecId: %d", pSendMsgBody->o_ucMediaCodecId);
		NPC_F_LOG_SR_WriteLog(sTempLogBuf);

		sprintf(sTempLogBuf, "o_ucVideoFrameRate: %d", pSendMsgBody->o_ucVideoFrameRate);
		NPC_F_LOG_SR_WriteLog(sTempLogBuf);

		sprintf(sTempLogBuf, "o_dwOffsetTimePos: %d", pSendMsgBody->o_dwOffsetTimePos);
		NPC_F_LOG_SR_WriteLog(sTempLogBuf);

		sprintf(sTempLogBuf, "o_dwDataLen: %d", pSendMsgBody->o_dwDataLen);
		NPC_F_LOG_SR_WriteLog(sTempLogBuf);*/

		//------------------------------------------------------------------------------------
		//发送消息给设备协议网络服务
		if ( !NPC_F_DPS_SendDevProData(
			in_pMcSrvData->hUmspProSrv,			//设备协议网络服务句柄
			pSendDataBuf,						//设备协议数据缓冲区，包括NPC_S_DPS_MSG_HEAD
			iSendDataLen						//设备协议数据长度
			) )
		{
			bRet = FALSE;
		}
	}

	//CLEAR AREA
	_NPC_CLEAR:
	{
		if ( pSendDataBuf )
		{
			free(pSendDataBuf);
			pSendDataBuf = NULL;
		}
	}

	return bRet;
}

//查询文件工作数据（根据文件读ID）
PNPC_S_UMSP_FILE_WORK_DATA		NPC_F_UMSP_MC_File_QueryFileWorkDataByFileReadId(
	IN		PNPC_S_UMSP_MCSRV_DATA					in_pMcSrvData,				//主控服务数据
	IN		NPC_DWORD								in_dwFileReadId				//文件读取ID
	)
{
	PNPC_S_UMSP_FILE_WORK_DATA						pFileWorkData;
	NPC_INT											iIndex;

	iIndex = in_dwFileReadId & 0xFFFF;
	if ( iIndex >= NPC_D_UMSP_MC_MAX_FILE_WORK_NUM )
		return NULL;

	pFileWorkData = &(in_pMcSrvData->tFileWorkTable[iIndex]);
	if ( !pFileWorkData->bIfUsed )
		return NULL;

	if ( pFileWorkData->dwFileReadId != in_dwFileReadId )
		return NULL;

	return pFileWorkData;
}


//-----------------------------------------------------------------------------------
//计算文件大小
NPC_LONG	NPC_F_UMSP_MC_ComputeFileSize(
	IN		PNPC_CHAR								in_pFileName				//文件名（包括路径）
	)
{
	FILE*											f;
	NPC_LONG										lFileSize;

	f = fopen(in_pFileName, "rb");
	if ( f == NULL )
		return -1;

	//移动到文件的结尾
	if ( fseek(f, 0, SEEK_END) == -1 )
		return -1;

	//获得文件大小
	lFileSize = ftell(f);
	if ( lFileSize == -1 )
		return -1;

	return lFileSize;
}


//-----------------------------------------------------------------------------------
//处理报警状态
NPC_VOID	NPC_F_UMSP_MC_DoAlarmState(
	IN		PNPC_S_UMSP_MCSRV_DATA					in_pMcSrvData				//主控服务数据
	)
{
	NPC_INT											i;
	PNPC_S_UMSP_CAMERA_DATA							pCameraData;
	time_t											tCurTime;
	NPC_CHAR										sAlarmId[32];

	if ( !in_pMcSrvData->tConfig.bIfSendAlarmMsg )
		return;

	tCurTime = time(NULL);

	for ( i=0; i<in_pMcSrvData->iChNum; i++ )
	{
		pCameraData = &in_pMcSrvData->tCameraTable[i];

		if ( tCurTime - pCameraData->tPrecSendAlarmMsgTime >= in_pMcSrvData->tConfig.iSendAlarmIntervalTime )
		{
			pCameraData->tPrecSendAlarmMsgTime = tCurTime;

			//发送报警消息
			/*NPC_F_UMSP_MC_SendAlarmMsg(
				in_pMcSrvData,							//主控服务数据
				pCameraData,							//摄像机数据
				i,										//通道号
				NPC_D_MON_CSD_ALARM_EVENT_FAULT,		//报警事件
				1										//报警状态
				);*/

			/*
			//上报报警消息（只有布防的报警消息才发送给服务器）
			NPC_F_DPS_UpAlarmMsg(
				in_pMcSrvData->hUmspProSrv,				//设备协议网络服务句柄
				i,										//通道号
				NPC_D_MON_CSD_ALARM_EVENT_MOTION,		//报警事件，参见NPC_D_MON_CSD_ALARM_EVENT_*定义
				1,										//报警状态，0：停止，1：开始
				(PNPC_CHAR)"Mobile detection alarm"		//报警信息
				);

			//上报报警消息（只有布防的报警消息才发送给服务器）
			NPC_F_DPS_UpAlarmMsg(
				in_pMcSrvData->hUmspProSrv,				//设备协议网络服务句柄
				i,										//通道号
				NPC_D_MON_CSD_ALARM_EVENT_VIDEO_BLIND,	//报警事件，参见NPC_D_MON_CSD_ALARM_EVENT_*定义
				1,										//报警状态，0：停止，1：开始
				(PNPC_CHAR)"Video blind alarm"			//报警信息
				);

			//上报报警消息（只有布防的报警消息才发送给服务器）
			NPC_F_DPS_UpAlarmMsg(
				in_pMcSrvData->hUmspProSrv,				//设备协议网络服务句柄
				i,										//通道号
				NPC_D_MON_CSD_ALARM_EVENT_VIDEO_LOSS,	//报警事件，参见NPC_D_MON_CSD_ALARM_EVENT_*定义
				1,										//报警状态，0：停止，1：开始
				(PNPC_CHAR)"Video lost alarm"			//报警信息
				);

			//上报报警消息（只有布防的报警消息才发送给服务器）
			NPC_F_DPS_UpAlarmMsg(
				in_pMcSrvData->hUmspProSrv,				//设备协议网络服务句柄
				i,										//通道号
				NPC_D_MON_CSD_ALARM_EVENT_PROBE,		//报警事件，参见NPC_D_MON_CSD_ALARM_EVENT_*定义
				1,										//报警状态，0：停止，1：开始
				(PNPC_CHAR)"Probe alarm"				//报警信息
				);

			//上报报警消息（只有布防的报警消息才发送给服务器）
			NPC_F_DPS_UpAlarmMsg(
				in_pMcSrvData->hUmspProSrv,				//设备协议网络服务句柄
				i,										//通道号
				NPC_D_MON_CSD_ALARM_EVENT_PIR,			//报警事件，参见NPC_D_MON_CSD_ALARM_EVENT_*定义
				1,										//报警状态，0：停止，1：开始
				(PNPC_CHAR)"PIR alarm"					//报警信息
				);

			//上报报警消息（只有布防的报警消息才发送给服务器）
			NPC_F_DPS_UpAlarmMsg(
				in_pMcSrvData->hUmspProSrv,				//设备协议网络服务句柄
				i,										//通道号
				NPC_D_MON_CSD_ALARM_EVENT_DOORBELL,		//报警事件，参见NPC_D_MON_CSD_ALARM_EVENT_*定义
				1,										//报警状态，0：停止，1：开始
				(PNPC_CHAR)"Doorbell alarm"				//报警信息
				);
			*/

			//创建报警ID
			NPC_F_UMSP_MC_CreateAlarmId(
				in_pMcSrvData,			//主控服务数据
				sAlarmId				//报警ID
				);
			
			//发送报警消息（该函数发送的报警消息不经过筛选，直接发送给服务器）
			NPC_F_DPS_SendAlarmMsg(
				in_pMcSrvData->hUmspProSrv,				//设备协议网络服务句柄
				sAlarmId,								//报警ID，要求设备端唯一，比如采用时间值+序号，如20150301091022001
				i,										//通道号
				2,										//报警事件，参见NPC_D_MON_CSD_ALARM_EVENT_*定义
				(PNPC_CHAR)"Mobile detection alarm"		//报警信息
				);

/*
			//-------------------------------------------------------------------------------------
			//创建报警ID
			NPC_F_UMSP_MC_CreateAlarmId(
				in_pMcSrvData,			//主控服务数据
				sAlarmId				//报警ID
				);

			//发送报警消息（该函数发送的报警消息不经过筛选，直接发送给服务器）
			NPC_F_DPS_SendAlarmMsg(
				in_pMcSrvData->hUmspProSrv,				//设备协议网络服务句柄
				sAlarmId,								//报警ID，要求设备端唯一，比如采用时间值+序号，如20150301091022001
				i,										//通道号
				NPC_D_MON_CSD_ALARM_EVENT_MOTION,		//报警事件，参见NPC_D_MON_CSD_ALARM_EVENT_*定义
				(PNPC_CHAR)"Mobile detection alarm"		//报警信息
				);
			
			//------------------------------------------------------------
			//创建报警ID
			NPC_F_UMSP_MC_CreateAlarmId(
				in_pMcSrvData,			//主控服务数据
				sAlarmId				//报警ID
				);

			//发送报警消息（该函数发送的报警消息不经过筛选，直接发送给服务器）
			NPC_F_DPS_SendAlarmMsg(
				in_pMcSrvData->hUmspProSrv,					//设备协议网络服务句柄
				sAlarmId,									//报警ID，要求设备端唯一，比如采用时间值+序号，如20150301091022001
				i,											//通道号
				NPC_D_MON_CSD_ALARM_EVENT_VIDEO_BLIND,		//报警事件，参见NPC_D_MON_CSD_ALARM_EVENT_*定义
				(PNPC_CHAR)"Video blind alarm"				//报警信息
				);

			//------------------------------------------------------------
			//创建报警ID
			NPC_F_UMSP_MC_CreateAlarmId(
				in_pMcSrvData,			//主控服务数据
				sAlarmId				//报警ID
				);

			//发送报警消息（该函数发送的报警消息不经过筛选，直接发送给服务器）
			NPC_F_DPS_SendAlarmMsg(
				in_pMcSrvData->hUmspProSrv,					//设备协议网络服务句柄
				sAlarmId,									//报警ID，要求设备端唯一，比如采用时间值+序号，如20150301091022001
				i,											//通道号
				NPC_D_MON_CSD_ALARM_EVENT_VIDEO_LOSS,		//报警事件，参见NPC_D_MON_CSD_ALARM_EVENT_*定义
				(PNPC_CHAR)"Video lost alarm"				//报警信息
				);

			//------------------------------------------------------------
			//创建报警ID
			NPC_F_UMSP_MC_CreateAlarmId(
				in_pMcSrvData,			//主控服务数据
				sAlarmId				//报警ID
				);

			//发送报警消息（该函数发送的报警消息不经过筛选，直接发送给服务器）
			NPC_F_DPS_SendAlarmMsg(
				in_pMcSrvData->hUmspProSrv,					//设备协议网络服务句柄
				sAlarmId,									//报警ID，要求设备端唯一，比如采用时间值+序号，如20150301091022001
				i,											//通道号
				NPC_D_MON_CSD_ALARM_EVENT_PROBE,			//报警事件，参见NPC_D_MON_CSD_ALARM_EVENT_*定义
				(PNPC_CHAR)"Probe alarm"					//报警信息
				);

			//------------------------------------------------------------
			//创建报警ID
			NPC_F_UMSP_MC_CreateAlarmId(
				in_pMcSrvData,			//主控服务数据
				sAlarmId				//报警ID
				);

			//发送报警消息（该函数发送的报警消息不经过筛选，直接发送给服务器）
			NPC_F_DPS_SendAlarmMsg(
				in_pMcSrvData->hUmspProSrv,					//设备协议网络服务句柄
				sAlarmId,									//报警ID，要求设备端唯一，比如采用时间值+序号，如20150301091022001
				i,											//通道号
				NPC_D_MON_CSD_ALARM_EVENT_PIR,				//报警事件，参见NPC_D_MON_CSD_ALARM_EVENT_*定义
				(PNPC_CHAR)"PIR alarm"						//报警信息
				);

			//------------------------------------------------------------
			//创建报警ID
			NPC_F_UMSP_MC_CreateAlarmId(
				in_pMcSrvData,			//主控服务数据
				sAlarmId				//报警ID
				);

			//发送报警消息（该函数发送的报警消息不经过筛选，直接发送给服务器）
			NPC_F_DPS_SendAlarmMsg(
				in_pMcSrvData->hUmspProSrv,					//设备协议网络服务句柄
				sAlarmId,									//报警ID，要求设备端唯一，比如采用时间值+序号，如20150301091022001
				i,											//通道号
				NPC_D_MON_CSD_ALARM_EVENT_DOORBELL,			//报警事件，参见NPC_D_MON_CSD_ALARM_EVENT_*定义
				(PNPC_CHAR)"Doorbell alarm"					//报警信息
				);

			//------------------------------------------------------------
			//发送报警消息
			NPC_F_UMSP_MC_SendAlarmInfo(
				in_pMcSrvData,								//主控服务数据
				i,											//通道号
				NPC_D_MON_CSD_ALARM_EVENT_HDD_ERROR,		//报警事件
				(PNPC_CHAR)"Hard disk error"				//报警信息
				);

			//发送报警消息
			NPC_F_UMSP_MC_SendAlarmInfo(
				in_pMcSrvData,								//主控服务数据
				i,											//通道号
				NPC_D_MON_CSD_ALARM_EVENT_HDD_FULL,			//报警事件
				(PNPC_CHAR)"Hard disk full"					//报警信息
				);

			//------------------------------------------------------------
			//发送报警消息
			NPC_F_UMSP_MC_SendAlarmInfo(
				in_pMcSrvData,										//主控服务数据
				i,													//通道号
				NPC_D_MON_CSD_ALARM_EVENT_SMART_IPC_CROSS_LINE,		//报警事件
				(PNPC_CHAR)"Smart ipc cross line"					//报警信息
				);

			//发送报警消息
			NPC_F_UMSP_MC_SendAlarmInfo(
				in_pMcSrvData,											//主控服务数据
				i,														//通道号
				NPC_D_MON_CSD_ALARM_EVENT_SMART_IPC_AREA_INTRUSION,		//报警事件
				(PNPC_CHAR)"Smart ipc area intrusion"					//报警信息
				);

*/
		}
	}
}

//发送报警消息
NPC_BOOL	NPC_F_UMSP_MC_SendAlarmMsg(
	IN		PNPC_S_UMSP_MCSRV_DATA					in_pMcSrvData,				//主控服务数据
	IN		PNPC_S_UMSP_CAMERA_DATA					in_pCameraData,				//摄像机数据
	IN		NPC_INT									in_iChNo,					//通道号
	IN		NPC_INT									in_iAlarmEvent,				//报警事件
	IN		NPC_INT									in_iAlarmState				//报警状态
	)
{
	struct _PacketData{
		NPC_S_DPS_MSG_HEAD							tMsgHead;
		NPC_S_DPS_MSG_BODY_EVENT_E2_ALARM_INFO		tMsgBody;
	} tPacketData = {0};

	tPacketData.tMsgHead.iMsgFuncId = NPC_D_DPS_MSGID_EVENT_E2_ALARM_INFO;					//消息功能ID，参见NPC_D_DPS_MSGID_*定义
	tPacketData.tMsgHead.ucMsgType = NPC_D_DPS_MSG_TYPE_REQUEST;							//消息类型，参见NPC_D_DPS_MSG_TYPE_*定义
	tPacketData.tMsgHead.iMsgBodyLen = sizeof(NPC_S_DPS_MSG_BODY_EVENT_E2_ALARM_INFO);		//消息包体长度

	tPacketData.tMsgBody.i_iChNo = in_iChNo;												//通道号
	tPacketData.tMsgBody.i_iAlarmEvent = in_iAlarmEvent;									//报警事件，参见NPC_D_MON_CSD_ALARM_EVENT_*定义
	tPacketData.tMsgBody.i_iAlarmState = in_iAlarmState;									//报警状态，0：报警开始，1：报警结束

	//发送消息给设备协议网络服务
	if ( !NPC_F_DPS_SendDevProData(
		in_pMcSrvData->hUmspProSrv,			//设备协议网络服务句柄
		(PNPC_BYTE)&tPacketData,			//设备协议数据缓冲区，包括NPC_S_DPS_MSG_HEAD
		sizeof(tPacketData)					//设备协议数据长度
		) )
	{
		return FALSE;
	}

	return TRUE;
}

//发送关闭媒体流消息
NPC_BOOL	NPC_F_UMSP_MC_SendCloseMediaStreamMsg(
	IN		PNPC_S_UMSP_MCSRV_DATA					in_pMcSrvData,				//主控服务数据
	IN		NPC_INT									in_iChNo,					//通道号
	IN		NPC_INT									in_iStreamNo				//流号
	)
{
	struct _PacketData{
		NPC_S_DPS_MSG_HEAD									tMsgHead;
		NPC_S_DPS_MSG_BODY_EVENT_E2_CLOSE_MEDIA_STREAM		tMsgBody;
	} tPacketData = {0};

	tPacketData.tMsgHead.iMsgFuncId = NPC_D_DPS_MSGID_EVENT_E2_CLOSE_MEDIA_STREAM;					//消息功能ID，参见NPC_D_DPS_MSGID_*定义
	tPacketData.tMsgHead.ucMsgType = NPC_D_DPS_MSG_TYPE_REQUEST;									//消息类型，参见NPC_D_DPS_MSG_TYPE_*定义
	tPacketData.tMsgHead.iMsgBodyLen = sizeof(NPC_S_DPS_MSG_BODY_EVENT_E2_CLOSE_MEDIA_STREAM);		//消息包体长度

	tPacketData.tMsgBody.i_iChNo = in_iChNo;														//通道号
	tPacketData.tMsgBody.i_iStreamNo = in_iStreamNo;												//码流号，0：主码流，1：子码流

	//发送消息给设备协议网络服务
	if ( !NPC_F_DPS_SendDevProData(
		in_pMcSrvData->hUmspProSrv,			//设备协议网络服务句柄
		(PNPC_BYTE)&tPacketData,			//设备协议数据缓冲区，包括NPC_S_DPS_MSG_HEAD
		sizeof(tPacketData)					//设备协议数据长度
		) )
	{
		return FALSE;
	}

	return TRUE;
}

//发送关闭对讲消息
NPC_BOOL	NPC_F_UMSP_MC_SendCloseTalkMsg(
	IN		PNPC_S_UMSP_MCSRV_DATA					in_pMcSrvData,				//主控服务数据
	IN		NPC_INT									in_iChNo					//通道号
	)
{
	struct _PacketData{
		NPC_S_DPS_MSG_HEAD							tMsgHead;
		NPC_S_DPS_MSG_BODY_EVENT_E2_TALK_STOP		tMsgBody;
	} tPacketData = {0};

	tPacketData.tMsgHead.iMsgFuncId = NPC_D_DPS_MSGID_EVENT_E2_TALK_STOP;					//消息功能ID，参见NPC_D_DPS_MSGID_*定义
	tPacketData.tMsgHead.ucMsgType = NPC_D_DPS_MSG_TYPE_REQUEST;							//消息类型，参见NPC_D_DPS_MSG_TYPE_*定义
	tPacketData.tMsgHead.iMsgBodyLen = sizeof(NPC_S_DPS_MSG_BODY_EVENT_E2_TALK_STOP);		//消息包体长度

	tPacketData.tMsgBody.i_iTalkChNo = in_iChNo;											//通道号

	//发送消息给设备协议网络服务
	if ( !NPC_F_DPS_SendDevProData(
		in_pMcSrvData->hUmspProSrv,			//设备协议网络服务句柄
		(PNPC_BYTE)&tPacketData,			//设备协议数据缓冲区，包括NPC_S_DPS_MSG_HEAD
		sizeof(tPacketData)					//设备协议数据长度
		) )
	{
		return FALSE;
	}

	return TRUE;
}


//-----------------------------------------------------------------------------------
//修改用户密码，返回0表示成功，1：用户名错误，2：原密码错误，9：其它失败
NPC_INT		NPC_F_UMSP_MC_ModifyUserPwd(
	IN		PNPC_S_UMSP_MCSRV_DATA					in_pMcSrvData,				//主控服务数据
	IN		PNPC_CHAR								in_pUserName,				//用户名
	IN		PNPC_CHAR								in_pOldPwd,					//原密码
	IN		PNPC_CHAR								in_pNewPwd					//新密码
	)
{
	NPC_CHAR										sCfgFileName[256];
	NPC_CHAR										sKeyName[128];
	NPC_CHAR										sUserName[32];
	NPC_CHAR										sUserPwd[32];
	NPC_INT											i, iUserNum;

	strcpy(sCfgFileName, ".\\npc_umsp_server_config.ini");

	//用户数
	iUserNum = GetPrivateProfileInt("USER", "UserNum", 0, sCfgFileName);

	for ( i=0; i<iUserNum; i++ )
	{
		sprintf(sKeyName, "UserName[%d]", i);
		GetPrivateProfileString("USER", sKeyName, "", sUserName, sizeof(sUserName), sCfgFileName);

		sprintf(sKeyName, "UserPwd[%d]", i);
		GetPrivateProfileString("USER", sKeyName, "", sUserPwd, sizeof(sUserPwd), sCfgFileName);

		if ( strcmp(in_pUserName, sUserName) == 0 )
		{
			if ( strcmp(in_pOldPwd, sUserPwd) != 0 )
			{
				return 2;
			}

			sprintf(sKeyName, "UserPwd[%d]", i);
			WritePrivateProfileString("USER", sKeyName, in_pNewPwd, sCfgFileName);

			return 0;
		}
	}

	return 1;
}

//------------------------------------------------------------------------------------------
//查询MP3文件列表
NPC_BOOL	NPC_F_UMSP_MC_DoCustomEvent_QUERY_MP3_FILE_LIST(
	IN		PNPC_S_UMSP_MCSRV_DATA					in_pMcSrvData,				//主控服务数据
	IN		PNPC_S_DPS_MSG_HEAD						in_pMsgHead,				//消息头
	IN		PNPC_BYTE								in_pMsgDataBuf,				//消息数据缓冲区，包括NPC_S_DPS_MSG_HEAD
	IN		NPC_INT									in_iMsgDataLen				//消息数据长度
	)
{
	NPC_BOOL										bRet;
	PNPC_S_DPS_MSG_BODY_EVENT_E7_CUSTOM				pMsgBody;
	PNPC_BYTE										pCusBodyPos;

	PNPC_BYTE										pSendDataBuf;
	NPC_INT											iSendDataLen;

	NPC_INT											iMp3FileNum;
	NPC_INT											iPreMsgDataLen;

	PNPC_S_DPS_MSG_HEAD								pSendMsgHead;
	PNPC_S_DPS_MSG_BODY_EVENT_E7_CUSTOM				pSendMsgBody;

	PNPC_S_DPS_CUSTOM_NI_BODY_QUERY_MP3_FILE_LIST	pMp3FileList;
	PNPC_S_DPS_CUSTOM_NI_BODY_MP3_FILE_INFO			pMp3FileInfo;

	NPC_INT											i;

	//INIT AREA
	{
		bRet = TRUE;
		pMsgBody = (PNPC_S_DPS_MSG_BODY_EVENT_E7_CUSTOM)((PNPC_BYTE)in_pMsgHead + sizeof(NPC_S_DPS_MSG_HEAD));
		pCusBodyPos = (PNPC_BYTE)pMsgBody + sizeof(NPC_S_DPS_MSG_BODY_EVENT_E7_CUSTOM);

		pSendDataBuf = NULL;
		iSendDataLen = 0;
		iMp3FileNum = 0;

		in_pMsgHead->ucMsgType = NPC_D_DPS_MSG_TYPE_RESPONSION;
		in_pMsgHead->ucResult = 0;
	}

	//CODE AREA
	{
		//显示信息
		NPC_F_LOG_SR_ShowInfo("[UMSP]  Query mp3 file list.");

		iPreMsgDataLen = sizeof(NPC_S_DPS_MSG_HEAD) + 
			sizeof(NPC_S_DPS_MSG_BODY_EVENT_E7_CUSTOM);

		iMp3FileNum = 0;
		for ( i=0; i<in_pMcSrvData->iMp3FileNum; i++ )
		{
			if ( in_pMcSrvData->tMp3FileTable[i].bIfUsed )
			{
				iMp3FileNum++;
			}
		}

		iSendDataLen = 
			iPreMsgDataLen + 
			sizeof(NPC_S_DPS_CUSTOM_NI_BODY_QUERY_MP3_FILE_LIST) + 
			sizeof(NPC_S_DPS_CUSTOM_NI_BODY_MP3_FILE_INFO) * iMp3FileNum;
		pSendDataBuf = (PNPC_BYTE)malloc(iSendDataLen);
		if ( pSendDataBuf == NULL )
		{
			bRet = FALSE;
			goto _NPC_CLEAR;
		}
		memset(pSendDataBuf, 0, iSendDataLen);

		pSendMsgHead = (PNPC_S_DPS_MSG_HEAD)pSendDataBuf;
		pSendMsgBody = (PNPC_S_DPS_MSG_BODY_EVENT_E7_CUSTOM)((PNPC_BYTE)pSendMsgHead + sizeof(NPC_S_DPS_MSG_HEAD));
		pMp3FileList = (PNPC_S_DPS_CUSTOM_NI_BODY_QUERY_MP3_FILE_LIST)((PNPC_BYTE)pSendMsgBody + sizeof(NPC_S_DPS_MSG_BODY_EVENT_E7_CUSTOM));
		pMp3FileInfo = (PNPC_S_DPS_CUSTOM_NI_BODY_MP3_FILE_INFO)((PNPC_BYTE)pMp3FileList + sizeof(NPC_S_DPS_CUSTOM_NI_BODY_QUERY_MP3_FILE_LIST));

		memcpy(pSendDataBuf, in_pMsgDataBuf, iPreMsgDataLen);

		pMp3FileList->o_iFilNum = 0;
		for ( i=0; i<in_pMcSrvData->iMp3FileNum; i++ )
		{
			if ( in_pMcSrvData->tMp3FileTable[i].bIfUsed )
			{
				strcpy(pMp3FileInfo->sFileName, in_pMcSrvData->tMp3FileTable[i].sFileName);
				pMp3FileInfo->bIfPlaying = in_pMcSrvData->tMp3FileTable[i].bIfPlaying;

				pMp3FileInfo++;
				pMp3FileList->o_iFilNum++;
			}
		}

		pSendMsgBody->i_iBodyLen = sizeof(NPC_S_DPS_CUSTOM_NI_BODY_QUERY_MP3_FILE_LIST) + 
			sizeof(NPC_S_DPS_CUSTOM_NI_BODY_MP3_FILE_INFO) * pMp3FileList->o_iFilNum;

		pSendMsgHead->iMsgBodyLen = iSendDataLen - sizeof(NPC_S_DPS_MSG_HEAD);

		//-----------------------------------------------------------------------------
		//发送消息给设备协议网络服务
		NPC_F_DPS_SendDevProData(
			in_pMcSrvData->hUmspProSrv,			//设备协议网络服务句柄
			pSendDataBuf,						//设备协议数据缓冲区，包括NPC_S_DPS_MSG_HEAD
			iSendDataLen						//设备协议数据长度
			);
	}

	//CLEAR AREA
	_NPC_CLEAR:
	{
		if ( pSendDataBuf )
		{
			free(pSendDataBuf);
			pSendDataBuf = NULL;
		}
	}

	return bRet;
}

//删除MP3文件
NPC_BOOL	NPC_F_UMSP_MC_DoCustomEvent_DELETE_MP3_FILE(
	IN		PNPC_S_UMSP_MCSRV_DATA					in_pMcSrvData,				//主控服务数据
	IN		PNPC_S_DPS_MSG_HEAD						in_pMsgHead,				//消息头
	IN		PNPC_BYTE								in_pMsgDataBuf,				//消息数据缓冲区，包括NPC_S_DPS_MSG_HEAD
	IN		NPC_INT									in_iMsgDataLen				//消息数据长度
	)
{
	NPC_BOOL										bRet;
	PNPC_S_DPS_MSG_BODY_EVENT_E7_CUSTOM				pMsgBody;
	PNPC_S_DPS_CUSTOM_NI_BODY_DELETE_MP3_FILE		pCusBodyPos;
	NPC_INT											i;

	//INIT AREA
	{
		bRet = TRUE;
		pMsgBody = (PNPC_S_DPS_MSG_BODY_EVENT_E7_CUSTOM)((PNPC_BYTE)in_pMsgHead + sizeof(NPC_S_DPS_MSG_HEAD));
		pCusBodyPos = (PNPC_S_DPS_CUSTOM_NI_BODY_DELETE_MP3_FILE)((PNPC_BYTE)pMsgBody + sizeof(NPC_S_DPS_MSG_BODY_EVENT_E7_CUSTOM));

		in_pMsgHead->ucMsgType = NPC_D_DPS_MSG_TYPE_RESPONSION;
		in_pMsgHead->ucResult = 0;
	}

	//CODE AREA
	{
		//显示信息
		NPC_CHAR sTempLogBuf[512];
		sprintf(sTempLogBuf, "[UMSP]  Delete mp3 file, file name: %s.", pCusBodyPos->sFileName);
		NPC_F_LOG_SR_ShowInfo(sTempLogBuf);

		for ( i=0; i<in_pMcSrvData->iMp3FileNum; i++ )
		{
			if ( in_pMcSrvData->tMp3FileTable[i].bIfUsed && 
				strcmp(in_pMcSrvData->tMp3FileTable[i].sFileName, pCusBodyPos->sFileName) == 0 )
			{
				in_pMcSrvData->tMp3FileTable[i].bIfUsed = FALSE;
				break;
			}
		}
	}

	//CLEAR AREA
	//_NPC_CLEAR:
	{
		//发送消息给设备协议网络服务
		NPC_F_DPS_SendDevProData(
			in_pMcSrvData->hUmspProSrv,			//设备协议网络服务句柄
			in_pMsgDataBuf,						//设备协议数据缓冲区，包括NPC_S_DPS_MSG_HEAD
			in_iMsgDataLen						//设备协议数据长度
			);
	}

	return bRet;
}

//开始播放MP3文件
NPC_BOOL	NPC_F_UMSP_MC_DoCustomEvent_START_PLAY_MP3_FILE(
	IN		PNPC_S_UMSP_MCSRV_DATA					in_pMcSrvData,				//主控服务数据
	IN		PNPC_S_DPS_MSG_HEAD						in_pMsgHead,				//消息头
	IN		PNPC_BYTE								in_pMsgDataBuf,				//消息数据缓冲区，包括NPC_S_DPS_MSG_HEAD
	IN		NPC_INT									in_iMsgDataLen				//消息数据长度
	)
{
	NPC_BOOL										bRet;
	PNPC_S_DPS_MSG_BODY_EVENT_E7_CUSTOM				pMsgBody;
	PNPC_S_DPS_CUSTOM_NI_BODY_START_PLAY_MP3_FILE	pCusBodyPos;
	NPC_INT											i;

	//INIT AREA
	{
		bRet = TRUE;
		pMsgBody = (PNPC_S_DPS_MSG_BODY_EVENT_E7_CUSTOM)((PNPC_BYTE)in_pMsgHead + sizeof(NPC_S_DPS_MSG_HEAD));
		pCusBodyPos = (PNPC_S_DPS_CUSTOM_NI_BODY_START_PLAY_MP3_FILE)((PNPC_BYTE)pMsgBody + sizeof(NPC_S_DPS_MSG_BODY_EVENT_E7_CUSTOM));

		in_pMsgHead->ucMsgType = NPC_D_DPS_MSG_TYPE_RESPONSION;
		in_pMsgHead->ucResult = 0;
	}

	//CODE AREA
	{
		//显示信息
		NPC_CHAR sTempLogBuf[512];
		sprintf(sTempLogBuf, "[UMSP]  Start play mp3 file, file name: %s.", pCusBodyPos->sFileName);
		NPC_F_LOG_SR_ShowInfo(sTempLogBuf);

		for ( i=0; i<in_pMcSrvData->iMp3FileNum; i++ )
		{
			if ( in_pMcSrvData->tMp3FileTable[i].bIfUsed )
			{
				in_pMcSrvData->tMp3FileTable[i].bIfPlaying = FALSE;
			}
		}

		for ( i=0; i<in_pMcSrvData->iMp3FileNum; i++ )
		{
			if ( in_pMcSrvData->tMp3FileTable[i].bIfUsed && 
				strcmp(in_pMcSrvData->tMp3FileTable[i].sFileName, pCusBodyPos->sFileName) == 0 )
			{
				in_pMcSrvData->tMp3FileTable[i].bIfPlaying = TRUE;
				break;
			}
		}
	}

	//CLEAR AREA
	//_NPC_CLEAR:
	{
		//发送消息给设备协议网络服务
		NPC_F_DPS_SendDevProData(
			in_pMcSrvData->hUmspProSrv,			//设备协议网络服务句柄
			in_pMsgDataBuf,						//设备协议数据缓冲区，包括NPC_S_DPS_MSG_HEAD
			in_iMsgDataLen						//设备协议数据长度
			);
	}

	return bRet;
}

//停止播放MP3文件
NPC_BOOL	NPC_F_UMSP_MC_DoCustomEvent_STOP_PLAY_MP3_FILE(
	IN		PNPC_S_UMSP_MCSRV_DATA					in_pMcSrvData,				//主控服务数据
	IN		PNPC_S_DPS_MSG_HEAD						in_pMsgHead,				//消息头
	IN		PNPC_BYTE								in_pMsgDataBuf,				//消息数据缓冲区，包括NPC_S_DPS_MSG_HEAD
	IN		NPC_INT									in_iMsgDataLen				//消息数据长度
	)
{
	NPC_BOOL										bRet;
	PNPC_S_DPS_MSG_BODY_EVENT_E7_CUSTOM				pMsgBody;
	PNPC_S_DPS_CUSTOM_NI_BODY_STOP_PLAY_MP3_FILE	pCusBodyPos;
	NPC_INT											i;

	//INIT AREA
	{
		bRet = TRUE;
		pMsgBody = (PNPC_S_DPS_MSG_BODY_EVENT_E7_CUSTOM)((PNPC_BYTE)in_pMsgHead + sizeof(NPC_S_DPS_MSG_HEAD));
		pCusBodyPos = (PNPC_S_DPS_CUSTOM_NI_BODY_STOP_PLAY_MP3_FILE)((PNPC_BYTE)pMsgBody + sizeof(NPC_S_DPS_MSG_BODY_EVENT_E7_CUSTOM));

		in_pMsgHead->ucMsgType = NPC_D_DPS_MSG_TYPE_RESPONSION;
		in_pMsgHead->ucResult = 0;
	}

	//CODE AREA
	{
		//显示信息
		NPC_CHAR sTempLogBuf[512];
		sprintf(sTempLogBuf, "[UMSP]  Stop play mp3 file, file name: %s.", pCusBodyPos->sFileName);
		NPC_F_LOG_SR_ShowInfo(sTempLogBuf);

		for ( i=0; i<in_pMcSrvData->iMp3FileNum; i++ )
		{
			if ( in_pMcSrvData->tMp3FileTable[i].bIfUsed && 
				strcmp(in_pMcSrvData->tMp3FileTable[i].sFileName, pCusBodyPos->sFileName) == 0 )
			{
				in_pMcSrvData->tMp3FileTable[i].bIfPlaying = FALSE;
				break;
			}
		}
	}

	//CLEAR AREA
	//_NPC_CLEAR:
	{
		//发送消息给设备协议网络服务
		NPC_F_DPS_SendDevProData(
			in_pMcSrvData->hUmspProSrv,			//设备协议网络服务句柄
			in_pMsgDataBuf,						//设备协议数据缓冲区，包括NPC_S_DPS_MSG_HEAD
			in_iMsgDataLen						//设备协议数据长度
			);
	}

	return bRet;
}

//控制音量
NPC_BOOL	NPC_F_UMSP_MC_DoCustomEvent_CTRL_VOLUME(
	IN		PNPC_S_UMSP_MCSRV_DATA					in_pMcSrvData,				//主控服务数据
	IN		PNPC_S_DPS_MSG_HEAD						in_pMsgHead,				//消息头
	IN		PNPC_BYTE								in_pMsgDataBuf,				//消息数据缓冲区，包括NPC_S_DPS_MSG_HEAD
	IN		NPC_INT									in_iMsgDataLen				//消息数据长度
	)
{
	NPC_BOOL										bRet;
	PNPC_S_DPS_MSG_BODY_EVENT_E7_CUSTOM				pMsgBody;
	PNPC_S_DPS_CUSTOM_NI_BODY_CTRL_VOLUME			pCusBodyPos;
	NPC_INT											i;

	//INIT AREA
	{
		bRet = TRUE;
		pMsgBody = (PNPC_S_DPS_MSG_BODY_EVENT_E7_CUSTOM)((PNPC_BYTE)in_pMsgHead + sizeof(NPC_S_DPS_MSG_HEAD));
		pCusBodyPos = (PNPC_S_DPS_CUSTOM_NI_BODY_CTRL_VOLUME)((PNPC_BYTE)pMsgBody + sizeof(NPC_S_DPS_MSG_BODY_EVENT_E7_CUSTOM));

		in_pMsgHead->ucMsgType = NPC_D_DPS_MSG_TYPE_RESPONSION;
		in_pMsgHead->ucResult = 0;
	}

	//CODE AREA
	{
		//显示信息
		NPC_CHAR sTempLogBuf[512];
		sprintf(sTempLogBuf, "[UMSP]  Ctrl volume, file name: %s, volume: %d.", pCusBodyPos->i_sFileName, pCusBodyPos->i_iVolumeSize);
		NPC_F_LOG_SR_ShowInfo(sTempLogBuf);

		for ( i=0; i<in_pMcSrvData->iMp3FileNum; i++ )
		{
			if ( in_pMcSrvData->tMp3FileTable[i].bIfUsed && 
				strcmp(in_pMcSrvData->tMp3FileTable[i].sFileName, pCusBodyPos->i_sFileName) == 0 )
			{
				in_pMcSrvData->tMp3FileTable[i].iVolumeSize = pCusBodyPos->i_iVolumeSize;
				break;
			}
		}
	}

	//CLEAR AREA
	//_NPC_CLEAR:
	{
		//发送消息给设备协议网络服务
		NPC_F_DPS_SendDevProData(
			in_pMcSrvData->hUmspProSrv,			//设备协议网络服务句柄
			in_pMsgDataBuf,						//设备协议数据缓冲区，包括NPC_S_DPS_MSG_HEAD
			in_iMsgDataLen						//设备协议数据长度
			);
	}

	return bRet;
}

//开始上传文件
NPC_BOOL	NPC_F_UMSP_MC_DoCustomEvent_START_UPLOAD_MP3_FILE(
	IN		PNPC_S_UMSP_MCSRV_DATA					in_pMcSrvData,				//主控服务数据
	IN		PNPC_S_DPS_MSG_HEAD						in_pMsgHead,				//消息头
	IN		PNPC_BYTE								in_pMsgDataBuf,				//消息数据缓冲区，包括NPC_S_DPS_MSG_HEAD
	IN		NPC_INT									in_iMsgDataLen				//消息数据长度
	)
{
	NPC_BOOL										bRet;
	PNPC_S_DPS_MSG_BODY_EVENT_E7_CUSTOM				pMsgBody;
	PNPC_S_DPS_CUSTOM_NI_BODY_START_UPLOAD_MP3_FILE	pCusBodyPos;
	PNPC_S_UMSP_MP3_FILE_DATA						pMp3FileInfo;
	NPC_CHAR										sTempFileName[256];
	NPC_INT											iUpFileIndex;
	NPC_INT											i;

	//INIT AREA
	{
		bRet = TRUE;
		pMsgBody = (PNPC_S_DPS_MSG_BODY_EVENT_E7_CUSTOM)((PNPC_BYTE)in_pMsgHead + sizeof(NPC_S_DPS_MSG_HEAD));
		pCusBodyPos = (PNPC_S_DPS_CUSTOM_NI_BODY_START_UPLOAD_MP3_FILE)((PNPC_BYTE)pMsgBody + sizeof(NPC_S_DPS_MSG_BODY_EVENT_E7_CUSTOM));

		in_pMsgHead->ucMsgType = NPC_D_DPS_MSG_TYPE_RESPONSION;
		in_pMsgHead->ucResult = 0;
	}

	//CODE AREA
	{
		//显示信息
		NPC_CHAR sTempLogBuf[512];
		sprintf(sTempLogBuf, "[UMSP]  Start Upload mp3 file, file name: %s, file size: %dB.", pCusBodyPos->i_sFileName, pCusBodyPos->i_dwFileSize);
		NPC_F_LOG_SR_ShowInfo(sTempLogBuf);

		pMp3FileInfo = NULL;
		iUpFileIndex = -1;
		for ( i=0; i<in_pMcSrvData->iMp3FileNum; i++ )
		{
			if ( !in_pMcSrvData->tMp3FileTable[i].bIfUsed )
			{
				pMp3FileInfo = &in_pMcSrvData->tMp3FileTable[i];
				iUpFileIndex = i;
				break;
			}
		}
		if ( pMp3FileInfo == NULL )
		{
			if ( in_pMcSrvData->iMp3FileNum >= NPC_D_UMSP_MC_MAX_MP3_FILE_NUM )
			{
				//写日志
				NPC_F_LOG_SR_WriteLog("[UMSP]  Upload mp3 file fail, cause: upload file too mush.");

				in_pMsgHead->ucResult = NPC_D_MON_CSD_DPS_ERROR_CODE_PARAM_ERROR;
				goto _NPC_CLEAR;
			}
			pMp3FileInfo = &in_pMcSrvData->tMp3FileTable[in_pMcSrvData->iMp3FileNum];
			iUpFileIndex = in_pMcSrvData->iMp3FileNum;
			in_pMcSrvData->iMp3FileNum++;
		}

		pMp3FileInfo->bIfUsed = TRUE;
		pMp3FileInfo->bIfPlaying = FALSE;
		sprintf(pMp3FileInfo->sFileName, "M%d_%s", rand(), pCusBodyPos->i_sFileName);
		pMp3FileInfo->dwUpFileId = (rand()<<16) | iUpFileIndex;

		pCusBodyPos->o_dwUploadId = pMp3FileInfo->dwUpFileId;
		strcpy(pCusBodyPos->o_sSaveFileName, pMp3FileInfo->sFileName);

		sprintf(sTempFileName, "d:\\%s", pMp3FileInfo->sFileName);
		pMp3FileInfo->pSaveFile = fopen(sTempFileName, "wb");
		if ( pMp3FileInfo->pSaveFile == NULL )
		{
			//写日志
			NPC_F_LOG_SR_WriteLog("[UMSP]  Upload mp3 file fail, cause: open file fail.");

			in_pMsgHead->ucResult = NPC_D_MON_CSD_DPS_ERROR_CODE_RESOURCE_OPEN_FAIL;
			goto _NPC_CLEAR;
		}
	}

	//CLEAR AREA
	_NPC_CLEAR:
	{
		//发送消息给设备协议网络服务
		NPC_F_DPS_SendDevProData(
			in_pMcSrvData->hUmspProSrv,			//设备协议网络服务句柄
			in_pMsgDataBuf,						//设备协议数据缓冲区，包括NPC_S_DPS_MSG_HEAD
			in_iMsgDataLen						//设备协议数据长度
			);
	}

	return bRet;
}

//停止上传文件
NPC_BOOL	NPC_F_UMSP_MC_DoCustomEvent_STOP_UPLOAD_MP3_FILE(
	IN		PNPC_S_UMSP_MCSRV_DATA					in_pMcSrvData,				//主控服务数据
	IN		PNPC_S_DPS_MSG_HEAD						in_pMsgHead,				//消息头
	IN		PNPC_BYTE								in_pMsgDataBuf,				//消息数据缓冲区，包括NPC_S_DPS_MSG_HEAD
	IN		NPC_INT									in_iMsgDataLen				//消息数据长度
	)
{
	NPC_BOOL										bRet;
	PNPC_S_DPS_MSG_BODY_EVENT_E7_CUSTOM				pMsgBody;
	PNPC_S_DPS_CUSTOM_NI_BODY_STOP_UPLOAD_MP3_FILE	pCusBodyPos;
	PNPC_S_UMSP_MP3_FILE_DATA						pMp3FileInfo;
	NPC_INT											iUpFileIndex;

	//INIT AREA
	{
		bRet = TRUE;
		pMsgBody = (PNPC_S_DPS_MSG_BODY_EVENT_E7_CUSTOM)((PNPC_BYTE)in_pMsgHead + sizeof(NPC_S_DPS_MSG_HEAD));
		pCusBodyPos = (PNPC_S_DPS_CUSTOM_NI_BODY_STOP_UPLOAD_MP3_FILE)((PNPC_BYTE)pMsgBody + sizeof(NPC_S_DPS_MSG_BODY_EVENT_E7_CUSTOM));

		in_pMsgHead->ucMsgType = NPC_D_DPS_MSG_TYPE_RESPONSION;
		in_pMsgHead->ucResult = 0;
	}

	//CODE AREA
	{
		//显示信息
		NPC_CHAR sTempLogBuf[512];
		sprintf(sTempLogBuf, "[UMSP]  Stop upload mp3 file, upload ID: 0x%8.8X.", pCusBodyPos->i_dwUploadId);
		NPC_F_LOG_SR_ShowInfo(sTempLogBuf);

		iUpFileIndex = pCusBodyPos->i_dwUploadId & 0xFFFF;
		if ( iUpFileIndex >= in_pMcSrvData->iMp3FileNum )
		{
			//写日志
			NPC_F_LOG_SR_WriteLog("[UMSP]  Stop upload mp3 file fail, cuase: iUpFileIndex error.");

			in_pMsgHead->ucResult = NPC_D_MON_CSD_DPS_ERROR_CODE_ID_ERROR;
			goto _NPC_CLEAR;
		}

		pMp3FileInfo = &in_pMcSrvData->tMp3FileTable[iUpFileIndex];
		if ( pMp3FileInfo->dwUpFileId != pCusBodyPos->i_dwUploadId )
		{
			//写日志
			NPC_F_LOG_SR_WriteLog("[UMSP]  Stop upload mp3 file fail, cuase: i_dwUploadId error.");

			in_pMsgHead->ucResult = NPC_D_MON_CSD_DPS_ERROR_CODE_ID_ERROR;
			goto _NPC_CLEAR;
		}

		//---------------------------------------------------------
		pMp3FileInfo->dwUpFileId = 0;

		if ( pMp3FileInfo->pSaveFile )
		{
			fclose(pMp3FileInfo->pSaveFile);
			pMp3FileInfo->pSaveFile = NULL;
		}
	}

	//CLEAR AREA
	_NPC_CLEAR:
	{
		//发送消息给设备协议网络服务
		NPC_F_DPS_SendDevProData(
			in_pMcSrvData->hUmspProSrv,			//设备协议网络服务句柄
			in_pMsgDataBuf,						//设备协议数据缓冲区，包括NPC_S_DPS_MSG_HEAD
			in_iMsgDataLen						//设备协议数据长度
			);
	}

	return bRet;
}

//上传文件数据
NPC_BOOL	NPC_F_UMSP_MC_DoCustomEvent_UPLOAD_FILE_DATA(
	IN		PNPC_S_UMSP_MCSRV_DATA					in_pMcSrvData,				//主控服务数据
	IN		PNPC_S_DPS_MSG_HEAD						in_pMsgHead,				//消息头
	IN		PNPC_BYTE								in_pMsgDataBuf,				//消息数据缓冲区，包括NPC_S_DPS_MSG_HEAD
	IN		NPC_INT									in_iMsgDataLen				//消息数据长度
	)
{
	NPC_BOOL										bRet;
	PNPC_S_DPS_MSG_BODY_EVENT_E7_CUSTOM				pMsgBody;
	PNPC_S_DPS_CUSTOM_NI_BODY_UPLOAD_FILE_DATA		pCusBodyPos;
	PNPC_BYTE										pFileDataPos;
	NPC_INT											iFileDataLen;
	PNPC_S_UMSP_MP3_FILE_DATA						pMp3FileInfo;
	NPC_INT											iUpFileIndex;

	//INIT AREA
	{
		bRet = TRUE;
		pMsgBody = (PNPC_S_DPS_MSG_BODY_EVENT_E7_CUSTOM)((PNPC_BYTE)in_pMsgHead + sizeof(NPC_S_DPS_MSG_HEAD));
		pCusBodyPos = (PNPC_S_DPS_CUSTOM_NI_BODY_UPLOAD_FILE_DATA)((PNPC_BYTE)pMsgBody + sizeof(NPC_S_DPS_MSG_BODY_EVENT_E7_CUSTOM));
		pFileDataPos = (PNPC_BYTE)pCusBodyPos + sizeof(NPC_S_DPS_CUSTOM_NI_BODY_UPLOAD_FILE_DATA);
		iFileDataLen = in_iMsgDataLen - (
			sizeof(NPC_S_DPS_MSG_HEAD) + 
			sizeof(NPC_S_DPS_MSG_BODY_EVENT_E7_CUSTOM) + 
			sizeof(NPC_S_DPS_CUSTOM_NI_BODY_UPLOAD_FILE_DATA)
			);

		in_pMsgHead->ucMsgType = NPC_D_DPS_MSG_TYPE_RESPONSION;
		in_pMsgHead->ucResult = 0;
	}

	//CODE AREA
	{
		//显示信息
		NPC_CHAR sTempLogBuf[512];
		sprintf(sTempLogBuf, "[UMSP]  Upload mp3 file data, upload ID: 0x%8.8X, data len: %d.", 
			pCusBodyPos->i_dwUploadId, pCusBodyPos->i_iDataLen);
		NPC_F_LOG_SR_ShowInfo(sTempLogBuf);

		iUpFileIndex = pCusBodyPos->i_dwUploadId & 0xFFFF;
		if ( iUpFileIndex >= in_pMcSrvData->iMp3FileNum )
		{
			//写日志
			NPC_F_LOG_SR_WriteLog("[UMSP]  Stop upload mp3 file fail, cause: iUpFileIndex error.");

			in_pMsgHead->ucResult = NPC_D_MON_CSD_DPS_ERROR_CODE_ID_ERROR;
			goto _NPC_CLEAR;
		}

		pMp3FileInfo = &in_pMcSrvData->tMp3FileTable[iUpFileIndex];
		if ( pMp3FileInfo->dwUpFileId != pCusBodyPos->i_dwUploadId )
		{
			//写日志
			NPC_F_LOG_SR_WriteLog("[UMSP]  Stop upload mp3 file fail, cause: i_dwUploadId error.");

			in_pMsgHead->ucResult = NPC_D_MON_CSD_DPS_ERROR_CODE_ID_ERROR;
			goto _NPC_CLEAR;
		}

		//----------------------------------------------------------------------------
		//保存文件
		if ( pMp3FileInfo->pSaveFile )
		{
			fwrite(pFileDataPos, 1, iFileDataLen, pMp3FileInfo->pSaveFile);
		}

		//----------------------------------------------------------------------------
		in_pMsgHead->iMsgBodyLen = 
			sizeof(NPC_S_DPS_MSG_BODY_EVENT_E7_CUSTOM) + 
			sizeof(NPC_S_DPS_CUSTOM_NI_BODY_UPLOAD_FILE_DATA);

		pMsgBody->i_iBodyLen = sizeof(NPC_S_DPS_CUSTOM_NI_BODY_UPLOAD_FILE_DATA);

		in_iMsgDataLen = 
			sizeof(NPC_S_DPS_MSG_HEAD) + 
			sizeof(NPC_S_DPS_MSG_BODY_EVENT_E7_CUSTOM) + 
			sizeof(NPC_S_DPS_CUSTOM_NI_BODY_UPLOAD_FILE_DATA);
	}

	//CLEAR AREA
	_NPC_CLEAR:
	{
		//发送消息给设备协议网络服务
		NPC_F_DPS_SendDevProData(
			in_pMcSrvData->hUmspProSrv,			//设备协议网络服务句柄
			in_pMsgDataBuf,						//设备协议数据缓冲区，包括NPC_S_DPS_MSG_HEAD
			in_iMsgDataLen						//设备协议数据长度
			);
	}

	return bRet;
}


//------------------------------------------------------------------------------------------
//关闭所有视频流
NPC_BOOL	NPC_F_UMSP_MC_CloseVideoStream(
	IN		PNPC_S_UMSP_MCSRV_DATA					in_pMcSrvData				//主控服务数据
	)
{
	NPC_INT											i;
	PNPC_S_UMSP_CAMERA_DATA							pCameraData;

	for ( i=0; i<NPC_D_DPS_MAX_CHANNEL_NUM+1; i++ )
	{
		pCameraData = &in_pMcSrvData->tCameraTable[i];

		if ( !pCameraData->bIfUsed || pCameraData->hWorkLock == NULL )
			continue;

		//加锁
		NPC_F_THREAD_Mutex_Lock(
			pCameraData->hWorkLock		//互斥锁句柄
			);

		if ( pCameraData->bIfUsed )
		{
			if ( pCameraData->bMainStreamOpenFlag )
			{
				//发送关闭媒体流消息
				NPC_F_UMSP_MC_SendCloseMediaStreamMsg(
					in_pMcSrvData,		//主控服务数据
					i,					//通道号
					0					//流号
					);
			}

			if ( pCameraData->bSubStreamOpenFlag )
			{
				//发送关闭媒体流消息
				NPC_F_UMSP_MC_SendCloseMediaStreamMsg(
					in_pMcSrvData,		//主控服务数据
					i,					//通道号
					1					//流号
					);
			}

			if ( pCameraData->bTalkIfOpen )
			{
				//发送关闭对讲消息
				NPC_F_UMSP_MC_SendCloseTalkMsg(
					in_pMcSrvData,		//主控服务数据
					i					//通道号
					);
			}

			//------------------------------------------------------------------
			pCameraData->bMainStreamOpenFlag = FALSE;
			pCameraData->bSubStreamOpenFlag = FALSE;
			pCameraData->bTalkIfOpen = FALSE;

			//结束预览
			NPC_F_FILECAM_SDK_StopRealPlay(
				in_pMcSrvData->hFileCamera,			//文件摄像机句柄
				i									//通道号
				);
			pCameraData->bIfUsed = FALSE;

			//停止录像
			NPC_F_UMSP_MC_StopRecord(
				in_pMcSrvData,			//主控服务数据
				pCameraData				//摄像机数据
				);
		}

		//解锁
		NPC_F_THREAD_Mutex_Unlock(
			pCameraData->hWorkLock			//互斥锁句柄
			);
	}

	return TRUE;
}



//------------------------------------------------------------------------------------------
//创建报警ID
NPC_VOID	NPC_F_UMSP_MC_CreateAlarmId(
	IN		PNPC_S_UMSP_MCSRV_DATA					in_pMcSrvData,				//主控服务数据
	OUT		PNPC_CHAR								out_pAlarmId				//报警ID
	)
{
	NPC_TIME_T										tCurTime;
	NPC_CHAR										sTimeStr[20];

	tCurTime = time(NULL);
	strftime(sTimeStr, 20, "%Y%m%d%H%M%S", localtime(&tCurTime));

	sprintf(out_pAlarmId, "UAI%s%4.4d", sTimeStr, in_pMcSrvData->uiAlarmSeqNo++);
}

//发送报警消息
NPC_VOID	NPC_F_UMSP_MC_SendAlarmInfo(
	IN		PNPC_S_UMSP_MCSRV_DATA					in_pMcSrvData,				//主控服务数据
	IN		NPC_INT									in_iChNo,					//通道号
	IN		NPC_INT									in_iAlarmEvent,				//报警事件
	IN		PNPC_CHAR								in_pAlarmInfo				//报警信息
	)
{
	NPC_CHAR										sAlarmId[32];

	//创建报警ID
	NPC_F_UMSP_MC_CreateAlarmId(
		in_pMcSrvData,			//主控服务数据
		sAlarmId				//报警ID
		);

	//发送报警消息（该函数发送的报警消息不经过筛选，直接发送给服务器）
	NPC_F_DPS_SendAlarmMsg(
		in_pMcSrvData->hUmspProSrv,		//设备协议网络服务句柄
		sAlarmId,						//报警ID，要求设备端唯一，比如采用时间值+序号，如20150301091022001
		in_iChNo,						//通道号
		in_iAlarmEvent,					//报警事件，参见NPC_D_MON_CSD_ALARM_EVENT_*定义
		in_pAlarmInfo					//报警信息
		);
}

