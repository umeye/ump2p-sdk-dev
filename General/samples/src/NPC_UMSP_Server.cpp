/*----------------------------------------------------------------------------
 * Copyright(c)  :  NPC CORPORTAION All Rights Reserved                       
 * FileName      :  NPC_UMSP_Server.cpp
 * Version       :  1.0
 * Author        :  CCH
 * DateTime      :  2013-08-10
 * Description   :  UMSP服务器
 *----------------------------------------------------------------------------*/

#include "NPC_PT_Server.h"
#include "NPC_UMSP_McServer.h"

PNPC_S_UMSP_MCSRV_DATA pMcSrvData = NULL;

//启动服务
NPC_PT_API bool NPC_SR_StartService(int argc, char* argv[])
{
	bool bRet;

	//INIT AREA
	{
		bRet = true;
	}

	//CODE AREA
	{
		if ( pMcSrvData )
			return true;

		//设置是否使用英文显示日志头
		NPC_F_LOG_SR_SetIfShowEnglishLogHead(
			TRUE		//是否显示英文日志头
			);

		pMcSrvData = (PNPC_S_UMSP_MCSRV_DATA)malloc(sizeof(NPC_S_UMSP_MCSRV_DATA));
		if ( pMcSrvData == NULL )
		{
			bRet = false;
			goto _NPC_CLEAR;
		}
		memset(pMcSrvData, 0, sizeof(NPC_S_UMSP_MCSRV_DATA));

		//启动主控服务
		if ( !NPC_F_UMSP_MC_StartMcServer(
			pMcSrvData			//主控服务数据
			) )
		{
			bRet = false;
			goto _NPC_CLEAR;
		}

		//写日志
		NPC_F_LOG_SR_WriteLog("[UMSP]  Start service success.");
	}

	//CLEAR AREA
	_NPC_CLEAR:
	{
		if ( !bRet )
		{
			//停止服务
			NPC_SR_StopService();
		}
	}

	return bRet;
}

//停止服务
NPC_PT_API void NPC_SR_StopService()
{
	if ( pMcSrvData )
	{
		//停止主控服务
		NPC_F_UMSP_MC_StopMcServer(
			pMcSrvData			//主控服务数据
			);

		free(pMcSrvData);
		pMcSrvData = NULL;

		//写日志
		NPC_F_LOG_SR_WriteLog("Stop service success.");
	}
}



