/*----------------------------------------------------------------------------
 * Copyright(c)  :  NPC CORPORTAION All Rights Reserved                       
 * FileName      :  NPC_PT_Server.h
 * Version       :  1.0
 * Author        :  CCH
 * DateTime      :  2012-08-10
 * Description   :  启动服务模块
 *----------------------------------------------------------------------------*/

#ifndef __NPC_PT_SERVER_H
#define __NPC_PT_SERVER_H

//导出导入定义
#ifdef _NPC_PT_EXPORTS
	#define NPC_PT_API __declspec(dllexport)
#else
	#define NPC_PT_API __declspec(dllimport)
#endif



//--------------------------------------------------------------------------------
#ifdef __cplusplus
extern "C"
{
#endif

//启动服务
NPC_PT_API bool NPC_SR_StartService(int argc, char* argv[]);

//停止服务
NPC_PT_API void NPC_SR_StopService();


#ifdef __cplusplus
}
#endif


#endif

