/*----------------------------------------------------------------------------
 * Copyright(c)  :  CORPORTAION  All Rights Reserved                       
 * FileName      :  NPC_DPS_ConfigAcc.h
 * Version       :  1.0
 * Author        :  CCH
 * DateTime      :  2013-12-24
 * Description   :  NPC系统配置读写接口
 *----------------------------------------------------------------------------*/

#ifndef __NPC_DPS_CONFIGACC_H
#define __NPC_DPS_CONFIGACC_H

#include "NPC_TypeDef.h"

#ifdef _WIN32
	#ifdef _NPC_DPS_CONFIGACC_EXPORTS
		#define NPC_CONFIGACC_API __declspec(dllexport)
	#else
		#ifdef _NPC_DPS_CONFIGACC_NON
			#define NPC_CONFIGACC_API
		#else
			#define NPC_CONFIGACC_API __declspec(dllimport)
		#endif
	#endif
#else
	#define NPC_CONFIGACC_API
#endif


///////////////////////////////////////////////////////////////////////////////
#ifdef __cplusplus
extern "C"
{
#endif

//读取字符串配置
NPC_CONFIGACC_API	NPC_BOOL	NPC_F_DPS_CA_GetString(
	IN		PNPC_CHAR					in_pMainKeyName,		//主键名称
	IN		PNPC_CHAR					in_pSubKeyName,			//子键名称
	IN		PNPC_CHAR					in_pDefaultValue,		//缺省值
	OUT		PNPC_CHAR					out_pReturnBuf,			//返回缓冲区
	IN		NPC_INT						in_iBufSize,			//缓冲区大小
	IN		PNPC_CHAR					in_pFileName			//配置参数文件名称
	);

//读取整形值
NPC_CONFIGACC_API	NPC_INT		NPC_F_DPS_CA_GetInt(
	IN		PNPC_CHAR					in_pMainKeyName,		//主键名称
	IN		PNPC_CHAR					in_pSubKeyName,			//子键名称
	IN		NPC_INT						in_iDefaultValue,		//缺省值
	IN		PNPC_CHAR					in_pFileName			//配置参数文件名称
	);

//读取浮点值
NPC_CONFIGACC_API	NPC_DOUBLE	NPC_F_DPS_CA_GetDouble(
	IN		PNPC_CHAR					in_pMainKeyName,		//主键名称
	IN		PNPC_CHAR					in_pSubKeyName,			//子键名称
	IN		NPC_DOUBLE					in_dDefaultValue,		//缺省值
	IN		PNPC_CHAR					in_pFileName			//配置参数文件名称
	);


//--------------------------------------------------------------------------------
//设置（写）字符串值
NPC_CONFIGACC_API	NPC_BOOL	NPC_F_DPS_CA_SetString(
	IN		PNPC_CHAR					in_pMainKeyName,		//主键名称
	IN		PNPC_CHAR					in_pSubKeyName,			//子键名称
	IN		PNPC_CHAR					in_pKeyValue,			//键值
	IN		PNPC_CHAR					in_pFileName			//配置参数文件名称
	);

//设置（写）整形值
NPC_CONFIGACC_API	NPC_BOOL	NPC_F_DPS_CA_SetInt(
	IN		PNPC_CHAR					in_pMainKeyName,		//主键名称
	IN		PNPC_CHAR					in_pSubKeyName,			//子键名称
	IN		NPC_INT						in_iKeyValue,			//键值
	IN		PNPC_CHAR					in_pFileName			//配置参数文件名称
	);

//设置（写）浮点值
NPC_CONFIGACC_API	NPC_BOOL	NPC_F_DPS_CA_SetDouble(
	IN		PNPC_CHAR					in_pMainKeyName,		//主键名称
	IN		PNPC_CHAR					in_pSubKeyName,			//子键名称
	IN		NPC_DOUBLE					in_dKeyValue,			//键值
	IN		PNPC_CHAR					in_pFileName			//配置参数文件名称
	);


#ifdef __cplusplus
}
#endif

#endif

