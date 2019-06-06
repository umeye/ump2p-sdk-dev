/*----------------------------------------------------------------------------
 * Copyright(c)  :  NPC CORPORTAION All Rights Reserved                       
 * FileName      :  NPC_SYS_FuncShare.cpp
 * Version       :  1.1
 * Author        :  CCH
 * DateTime      :  2012-01-05
 * Description   :  NPC系统移置共享模块
 *----------------------------------------------------------------------------*/

#include "NPC_SYS_FuncShare.h"

#ifdef WINAPI_FAMILY
#include <synchapi.h>
#include <winbase.h>
#include <windows.h>
#include "WindowsPhoneRuntimeComponent1.h"
#endif


//--------------------------------------------------------------------------------
//获取开机时长（毫秒）
NPC_DWORD	NPC_F_SYS_GetTickCount()
{
#ifdef _WIN32
	#ifdef WINAPI_FAMILY
		return GetTickCount64();
	#else
		return GetTickCount();
	#endif
#elif __APPLE__
	struct timeval tv;
	gettimeofday(&tv, NULL);
	return (tv.tv_sec * 1000 + tv.tv_usec / 1000000);
#else
    struct timespec ts;
    clock_gettime(CLOCK_MONOTONIC, &ts);
    return (ts.tv_sec * 1000 + ts.tv_nsec / 1000000);
#endif
}

//线程休眠N毫秒
NPC_VOID	NPC_F_SYS_Sleep(
	IN		NPC_INT							in_iTimeLen				//休眠时长（毫秒）
	)
{
#ifdef _WIN32
#ifdef WINAPI_FAMILY
	WindowsPhoneRuntimeComponent1::OvalUtil::Sleep(in_iTimeLen);
#else
	Sleep(in_iTimeLen);
#endif
#else
	usleep(in_iTimeLen*1000);
#endif
}


//将time_t类型的时间，格式化为YYYY-MM-DD HH24:MI:SS型的字符串
NPC_VOID	NPC_F_SYS_Time_To_Str(
	IN		time_t							in_tTimeValue,			//时间值
	OUT		PNPC_CHAR						out_pTimeStr			//时间字符串
	)
{
	struct tm tm1;

#ifdef WIN32
	tm1 = *localtime(&in_tTimeValue);
#else
	localtime_r(&in_tTimeValue, &tm1);
#endif

	sprintf(
		out_pTimeStr, 
		"%4.4d-%2.2d-%2.2d %2.2d:%2.2d:%2.2d",
		tm1.tm_year+1900, tm1.tm_mon+1, tm1.tm_mday,
		tm1.tm_hour, tm1.tm_min,tm1.tm_sec);
}

//将YYYY-MM-DD HH24:MI:SS格式的时间字符串，转换为time_t型的时间
time_t		NPC_F_SYS_Str_To_Time(
	IN		PNPC_CHAR						in_pTimeStr				//时间字符串
	)
{
	struct tm tm1 = {0};
	time_t time1;
	NPC_CHAR sTempStr[20];

	sscanf(in_pTimeStr, "%4d-%2d-%2d %2d:%2d:%2d",     
				 &tm1.tm_year, 
				 &tm1.tm_mon, 
				 &tm1.tm_mday, 
				 &tm1.tm_hour, 
				 &tm1.tm_min,
				 &tm1.tm_sec);

	sprintf(
		sTempStr, 
		"%4.4d-%2.2d-%2.2d %2.2d:%2.2d:%2.2d",
		tm1.tm_year, tm1.tm_mon, tm1.tm_mday,
		tm1.tm_hour, tm1.tm_min, tm1.tm_sec);

	//0x7FFFFFFF = 2038-01-19 11:14:07
	if ( strcmp(sTempStr, "2038-01-19 11:14:07") > 0 )
	{
		return -1;
	}

	tm1.tm_year -= 1900;
	tm1.tm_mon --;

	tm1.tm_isdst=-1;

	time1 = mktime(&tm1);

	return time1;
}

//将时间参数转换为time_t
time_t		NPC_F_SYS_TimeParam_To_Timet(
	IN		NPC_INT							in_iYear,				//年
	IN		NPC_INT							in_iMonth,				//月
	IN		NPC_INT							in_iDay,				//日
	IN		NPC_INT							in_iHour,				//时
	IN		NPC_INT							in_iMinute,				//分
	IN		NPC_INT							in_iSecond				//秒
	)
{
	struct tm tm1 = {0};
	time_t time1;

	tm1.tm_year = in_iYear;
	tm1.tm_mon = in_iMonth;
	tm1.tm_mday = in_iDay;
	tm1.tm_hour = in_iHour;
	tm1.tm_min = in_iMinute;
	tm1.tm_sec = in_iSecond;
       
	tm1.tm_year -= 1900;
	tm1.tm_mon --;

	tm1.tm_isdst = -1;

	time1 = mktime(&tm1);

	return time1;
}

//将time_t转换为时间参数
NPC_VOID	NPC_F_SYS_Timet_To_TimeParam(
	IN		time_t							in_tTimeValue,			//时间值
	OUT		PNPC_INT						out_pYear,				//年
	OUT		PNPC_INT						out_pMonth,				//月
	OUT		PNPC_INT						out_pDay,				//日
	OUT		PNPC_INT						out_pHour,				//时
	OUT		PNPC_INT						out_pMinute,			//分
	OUT		PNPC_INT						out_pSecond				//秒
	)
{
	struct tm tm1;

#ifdef WIN32
	tm1 = *localtime(&in_tTimeValue);
#else
	localtime_r(&in_tTimeValue, &tm1);
#endif

	*out_pYear = tm1.tm_year+1900;
	*out_pMonth = tm1.tm_mon+1;
	*out_pDay = tm1.tm_mday;
	*out_pHour = tm1.tm_hour;
	*out_pMinute = tm1.tm_min;
	*out_pSecond = tm1.tm_sec;
}

//将time_t转换为NPC_S_TD_DATE_TIME时间
NPC_VOID	NPC_F_SYS_Timet_To_TdTime(
	IN		time_t							in_tTimeValue,			//时间值
	OUT		PNPC_S_TD_DATE_TIME				out_pTdTime				//TD时间
	)
{
	NPC_INT									iYear;
	NPC_INT									iMonth;
	NPC_INT									iDay;
	NPC_INT									iHour;
	NPC_INT									iMinute;
	NPC_INT									iSecond;

	//将time_t转换为时间参数
	NPC_F_SYS_Timet_To_TimeParam(
		in_tTimeValue,		//时间值
		&iYear,				//年
		&iMonth,			//月
		&iDay,				//日
		&iHour,				//时
		&iMinute,			//分
		&iSecond			//秒
		);

	out_pTdTime->usYear = iYear;
	out_pTdTime->usMonth = iMonth;
	out_pTdTime->ucDay = iDay;
	out_pTdTime->ucHour = iHour;
	out_pTdTime->ucMinute = iMinute;
	out_pTdTime->ucSecond = iSecond;
	out_pTdTime->dwMinsecond = 0;
}

//将字段串时间转换为NPC_S_TD_DATE_TIME时间
NPC_VOID	NPC_F_SYS_StrTime_To_TdTime(
	IN		PNPC_CHAR						in_pTimeStr,			//时间字符串，格式为：YYYY-MM-DD HH24:MI:SS
	OUT		PNPC_S_TD_DATE_TIME				out_pTdTime				//TD时间
	)
{
	NPC_INT iYear, iMon, iDay, iHour, iMin, iSec;

	sscanf(in_pTimeStr, "%4d-%2d-%2d %2d:%2d:%2d",     
				 &iYear, 
				 &iMon, 
				 &iDay, 
				 &iHour, 
				 &iMin,
				 &iSec);

	out_pTdTime->usYear = iYear;
	out_pTdTime->usMonth = iMon;
	out_pTdTime->ucDay = iDay;
	out_pTdTime->ucHour = iHour;
	out_pTdTime->ucMinute = iMin;
	out_pTdTime->ucSecond = iSec;
	out_pTdTime->dwMinsecond = 0;
}

//将NPC_S_TD_DATE_TIME转换为time_t时间
time_t		NPC_F_SYS_TdTime_To_Timet(
	IN		PNPC_S_TD_DATE_TIME				in_pTdTime				//TD时间
	)
{
	//将时间参数转换为time_t
	return NPC_F_SYS_TimeParam_To_Timet(
		in_pTdTime->usYear,				//年
		in_pTdTime->usMonth,			//月
		in_pTdTime->ucDay,				//日
		in_pTdTime->ucHour,				//时
		in_pTdTime->ucMinute,			//分
		in_pTdTime->ucSecond			//秒
		);
}

//将NPC_S_TD_DATE_TIME转换为时间字符串
NPC_VOID	NPC_F_SYS_TdTime_To_TimeStr(
	IN		PNPC_S_TD_DATE_TIME				in_pTdTime,				//TD时间
	OUT		PNPC_CHAR						out_pTimeStr			//时间字符串
	)
{
	sprintf(out_pTimeStr, "%4.4d-%2.2d-%2.2d %2.2d:%2.2d:%2.2d",    
		in_pTdTime->usYear,				//年
		in_pTdTime->usMonth,			//月
		in_pTdTime->ucDay,				//日
		in_pTdTime->ucHour,				//时
		in_pTdTime->ucMinute,			//分
		in_pTdTime->ucSecond			//秒
		);
}

//将时间字符串转换为NPC_S_TD_TIME
NPC_VOID	NPC_F_SYS_Str_To_Time(
	IN		PNPC_CHAR						in_pTimeStr,				//时间字符串，格式为：HH:MM:SS
	OUT		PNPC_S_TD_TIME					out_pTimeObj				//时间对像
	)
{
	NPC_CHAR sTempBuf[12];

	//跳过空格或TAB符
	NPC_F_SYS_StrOp_JumpSpace(
		&in_pTimeStr		//字符串
		);
	if ( in_pTimeStr == NULL )
		return;

	//时
	//获取一段字符串，返回0表示成功，1表示未找到，2表示返回缓冲区太小
	if ( NPC_F_SYS_StrOp_GetSectString(
		&in_pTimeStr,			//原字符串，返回提取之后的位置
		(PNPC_CHAR)":",			//查询字符串
		sizeof(sTempBuf),		//返回的字符串缓冲区大小
		sTempBuf				//返回的字符串缓冲区，由上层分配
		) != 0 )
	{
		return;
	}
	out_pTimeObj->ucHour = atoi(sTempBuf);

	//分
	//获取一段字符串，返回0表示成功，1表示未找到，2表示返回缓冲区太小
	if ( NPC_F_SYS_StrOp_GetSectString(
		&in_pTimeStr,			//原字符串，返回提取之后的位置
		(PNPC_CHAR)":",			//查询字符串
		sizeof(sTempBuf),		//返回的字符串缓冲区大小
		sTempBuf				//返回的字符串缓冲区，由上层分配
		) != 0 )
	{
		return;
	}
	out_pTimeObj->ucMin = atoi(sTempBuf);

	//秒
	//获取一段字符串，返回0表示成功，1表示未找到，2表示返回缓冲区太小
	if ( NPC_F_SYS_StrOp_GetSectString(
		&in_pTimeStr,			//原字符串，返回提取之后的位置
		(PNPC_CHAR)":",			//查询字符串
		sizeof(sTempBuf),		//返回的字符串缓冲区大小
		sTempBuf				//返回的字符串缓冲区，由上层分配
		) != 0 )
	{
		return;
	}
	out_pTimeObj->ucSec = atoi(sTempBuf);
}

//将NPC_S_TD_TIME转换时间字符串
NPC_VOID	NPC_F_SYS_Time_To_Str(
	IN		PNPC_S_TD_TIME					in_pTimeObj,				//时间对像
	OUT		PNPC_CHAR						out_pTimeStr				//时间字符串，格式为：HH:MM:SS
	)
{
	sprintf(out_pTimeStr, "%2.2d:%2.2d:%2.2d", 
		in_pTimeObj->ucHour, 
		in_pTimeObj->ucMin, 
		in_pTimeObj->ucSec
		);
}

//计算两个时间之间的时长
NPC_DWORD	NPC_F_SYS_ComputeTdTimeLen(
	IN		PNPC_S_TD_DATE_TIME				in_pBeginTime,			//开始时间
	IN		PNPC_S_TD_DATE_TIME				in_pEndTime				//结束时间
	)
{
	time_t									tBeginTime;
	time_t									tEndTime;

	//将NPC_S_TD_DATE_TIME转换为time_t时间
	tBeginTime = NPC_F_SYS_TdTime_To_Timet(
		in_pBeginTime				//TD时间
		);

	tEndTime = NPC_F_SYS_TdTime_To_Timet(
		in_pEndTime					//TD时间
		);

	if ( tBeginTime > tEndTime )
		return tBeginTime - tEndTime;

	return tEndTime - tBeginTime;
}

//计算较验和
NPC_USHORT	NPC_F_SYS_CheckSum(
	IN		PNPC_BYTE						in_pDataBuf,			//数据缓冲区
	IN		NPC_INT							in_iDataLen				//数据长度
	)
{
	PNPC_USHORT								pDataPos;
	NPC_DWORD								dwCkSum;

	pDataPos = (PNPC_USHORT)in_pDataBuf;
	dwCkSum = 0;

	while ( in_iDataLen > 1 )
	{
		dwCkSum += *pDataPos++;
		in_iDataLen -= sizeof(NPC_USHORT);
	}

	if ( in_iDataLen )
	{
		dwCkSum += *(PNPC_BYTE)pDataPos;
	}

	dwCkSum = (dwCkSum >> 16) + (dwCkSum & 0xffff);
	dwCkSum += (dwCkSum >> 16);

	return (NPC_USHORT)(~dwCkSum);
}


//获取IP地址（根据IP地址值主机顺序）
NPC_VOID	NPC_F_SYS_GetIpaddrStrByHost(
	IN		NPC_DWORD						in_dwIpaddr,			//IP地址值
	OUT		PNPC_CHAR						out_pIpaddrStr			//IP地址字符串
	)
{
	PNPC_BYTE								pAddrPos;

	pAddrPos = (PNPC_BYTE)&in_dwIpaddr;

	sprintf(out_pIpaddrStr, "%d.%d.%d.%d", pAddrPos[3], pAddrPos[2], pAddrPos[1], pAddrPos[0]);
}

//把字符串IP地址转换成主机顺序的IP地址值
NPC_DWORD	NPC_F_SYS_IpaddrStrToHostIpaddr(
	IN		PNPC_CHAR						in_pIpaddrStr			//IP地址字符串
	)
{
	NPC_DWORD								dwIpaddrValue;
	PNPC_BYTE								pAddrPos;
	NPC_INT									iIpTable[4];

	sscanf(in_pIpaddrStr, "%d.%d.%d.%d", &iIpTable[0], &iIpTable[1], &iIpTable[2], &iIpTable[3]);

	pAddrPos = (PNPC_BYTE)&dwIpaddrValue;

	pAddrPos[0] = iIpTable[3];
	pAddrPos[1] = iIpTable[2];
	pAddrPos[2] = iIpTable[1];
	pAddrPos[3] = iIpTable[0];

	return dwIpaddrValue;
}

//将二进制MAC地址转换成字符串MAC地址
NPC_VOID	NPC_F_SYS_GetStringMacAddr(
	IN		PNPC_BYTE						in_pBitMac,				//二进制MAC地址
	OUT		PNPC_CHAR						out_pStrMac				//字符串MAC地址
	)
{
	sprintf(out_pStrMac, "%2.2X-%2.2X-%2.2X-%2.2X-%2.2X-%2.2X", 
		in_pBitMac[0], 
		in_pBitMac[1], 
		in_pBitMac[2], 
		in_pBitMac[3], 
		in_pBitMac[4], 
		in_pBitMac[5]
		);
}

//将字符串MAC地址转换成二进制MAC地址
NPC_VOID	NPC_F_SYS_GetBitMacAddr(
	IN		PNPC_CHAR						in_pStrMac,				//字符串MAC地址
	OUT		PNPC_BYTE						out_pBitMac				//二进制MAC地址
	)
{
	NPC_INT									i;
	NPC_INT									iTempValue;
	PNPC_CHAR								pos;

	pos = in_pStrMac;

	for ( i=0; i<6; i++ )
	{
		sscanf(pos, "%x", &iTempValue);
		out_pBitMac[i] = iTempValue;

		pos += 3;
	}
}

//较验字符串MAC地址格式是否正确
NPC_BOOL	NPC_F_SYS_CheckMacStrAddr(
	IN		PNPC_CHAR						in_pStrMac				//字符串MAC地址，格式为XX-XX-XX-XX-XX-XX
	)
{
	NPC_INT									iStrMacLen;
	NPC_INT									i;

	iStrMacLen = strlen(in_pStrMac);

	if ( iStrMacLen != 17 )
	{
		return FALSE;
	}

	for ( i=0; i<iStrMacLen; i += 3 )
	{
		if ( !(in_pStrMac[i] >= '0' && 
			in_pStrMac[i] <= '9' || 
			in_pStrMac[i] >= 'a' && 
			in_pStrMac[i] <= 'f' || 
			in_pStrMac[i] >= 'A' && 
			in_pStrMac[i] <= 'F') )
		{
			return FALSE;
		}

		if ( !(in_pStrMac[i+1] >= '0' && 
			in_pStrMac[i+1] <= '9' || 
			in_pStrMac[i+1] >= 'a' && 
			in_pStrMac[i+1] <= 'f' || 
			in_pStrMac[i+1] >= 'A' && 
			in_pStrMac[i+1] <= 'F') )
		{
			return FALSE;
		}

		if ( in_pStrMac[i+2] != '-' && 
			in_pStrMac[i+2] != 0 )
		{
			return FALSE;
		}
	}

	return TRUE;
}

//将二进制数组数据转换成十六进制字符串
NPC_BOOL	NPC_F_SYS_BitDataToHexStr(
	IN		PNPC_BYTE						in_pBitDataBuf,			//二进制数据缓冲区
	IN		NPC_INT							in_iBitDataLen,			//二进制数据长度
	OUT		PNPC_CHAR*						out_ppHexStrBuf			//十六进制字符串缓冲区，由该函数分配内存，上层释放内存
	)
{
	NPC_INT									i;
	PNPC_CHAR								pHexStrBuf;
	NPC_INT									iHexStrBufSize;
	PNPC_CHAR								pos;

	iHexStrBufSize = in_iBitDataLen * 3 + 1;
	pHexStrBuf = (PNPC_CHAR)malloc(iHexStrBufSize);
	if ( pHexStrBuf == NULL )
		return FALSE;
	memset(pHexStrBuf, 0, iHexStrBufSize);

	pos = pHexStrBuf;

	for ( i=0; i<in_iBitDataLen; i++ )
	{
		sprintf(pos, "%2.2X ", in_pBitDataBuf[i]);
		pos += 3;
	}

	*out_ppHexStrBuf = pHexStrBuf;

	return TRUE;
}

//将十六进制字符串转换成无符号整型值
NPC_DWORD	NPC_F_SYS_HexStrToDWORD(
	IN		PNPC_CHAR						in_pHexStrBuf			//十六进制字符串缓冲区，如0x12345678，或12345678
	)
{
	NPC_DWORD								dwValue;

	sscanf(in_pHexStrBuf, "%x", (PNPC_INT)&dwValue);

	return dwValue;
}

//替换字符串中的指定位置
NPC_BOOL	NPC_F_SYS_ReplaceString(
	IN		PNPC_CHAR						in_pStrBuf,				//字符串缓冲区
	IN		NPC_INT							in_iBeginPos,			//开始位置
	IN		NPC_INT							in_iOldLen,				//原长度
	IN		PNPC_CHAR						in_pNewValue			//新值
	)
{
	/*NPC_INT									iStrLen;
	NPC_INT									iNewLen;
	NPC_INT									iOldSyEndLen;

	iStrLen = strlen(in_pStrBuf);
	iNewLen = strlen(in_pNewValue);

	if ( in_iBeginPos < 0 )
		in_iBeginPos = 0;
	else if ( in_iBeginPos > iStrLen )
		in_iBeginPos = iStrLen;

	if ( in_iOldLen < 0 )
		in_iOldLen = 0;
	else if ( in_iOldLen > iStrLen - in_iBeginPos )
		in_iOldLen = iStrLen - in_iBeginPos;

	if ( in_iOldLen != iNewLen )
	{
		iOldSyEndLen = iStrLen - (in_iBeginPos + in_iOldLen);
		if ( iOldSyEndLen > 0 )
		{
			memcpy(
				in_pStrBuf+in_iBeginPos+iNewLen, 
				in_pStrBuf+in_iBeginPos+in_iOldLen, 
				iOldSyEndLen);
		}
	}

	memcpy(in_pStrBuf+in_iBeginPos, in_pNewValue, iNewLen);

	*(in_pStrBuf+iStrLen+iNewLen-in_iOldLen) = 0;*/

	PNPC_CHAR								pTempDataBuf;
	NPC_INT									iStrLen;
	NPC_INT									iNewLen;

	iStrLen = strlen(in_pStrBuf);
	iNewLen = strlen(in_pNewValue);

	if ( in_iBeginPos < 0 )
		in_iBeginPos = 0;
	else if ( in_iBeginPos > iStrLen )
		in_iBeginPos = iStrLen;

	if ( in_iOldLen < 0 )
		in_iOldLen = 0;
	else if ( in_iOldLen > iStrLen - in_iBeginPos )
		in_iOldLen = iStrLen - in_iBeginPos;

	pTempDataBuf = (PNPC_CHAR)malloc(iStrLen+1);
	if ( pTempDataBuf == NULL )
		return FALSE;

	strcpy(pTempDataBuf, in_pStrBuf);

	strcpy(in_pStrBuf+in_iBeginPos, in_pNewValue);
	strcpy(in_pStrBuf+in_iBeginPos+iNewLen, pTempDataBuf+in_iBeginPos+in_iOldLen);

	free(pTempDataBuf);
	pTempDataBuf = NULL;

	return TRUE;
}

//替换JSON的结点值，只替换第一个结点的值
NPC_BOOL	NPC_F_SYS_ReplaceJsonValue(
	IN		PNPC_CHAR						in_pJsonDocBuf,			//JSON文档缓冲区
	IN		PNPC_CHAR						in_pNodeName,			//结点名字，不包括名字的引号
	IN		PNPC_CHAR						in_pNodeNewValue		//结点新值，包括值的引号
	)
{
	PNPC_CHAR								pFindPos;
	PNPC_CHAR								pCurPos;
	PNPC_CHAR								pValueBeginPos;
	PNPC_CHAR								pValueEndPos;
	NPC_CHAR								sTempNodeName[128];

	if ( strlen(in_pNodeName) >= sizeof(sTempNodeName) )
		return FALSE;

	sprintf(sTempNodeName, "\"%s\"", in_pNodeName);

	//查找名字
	pFindPos = strstr(in_pJsonDocBuf, sTempNodeName);
	if ( pFindPos == NULL )
		return FALSE;

	//查找：
	pCurPos = pFindPos + strlen(sTempNodeName);
	while ( *pCurPos )
	{
		if ( *pCurPos == ':' )
		{
			break;
		}

		pCurPos++;
	}
	if ( *pCurPos == 0 )
		return FALSE;

	//跳过空格
	pCurPos++;
	while ( *pCurPos == ' ' )
	{
		pCurPos++;
	}

	//查找，
	pValueBeginPos = pCurPos;
	while ( *pCurPos )
	{
		if ( *pCurPos == ',' )
		{
			break;
		}

		pCurPos++;
	}
	if ( *pCurPos == 0 )
	{
		//表示为最后一个结点
		pValueEndPos = pValueBeginPos + strlen(pValueBeginPos);
	}
	else
	{
		pValueEndPos = pCurPos;
	}

	//-------------------------------------------------------------------------
	//替换字符串中的指定位置
	if ( !NPC_F_SYS_ReplaceString(
		in_pJsonDocBuf,						//字符串缓冲区
		pValueBeginPos - in_pJsonDocBuf,	//开始位置
		pValueEndPos - pValueBeginPos,		//原长度
		in_pNodeNewValue					//新值
		) )
	{
		return FALSE;
	}

	return TRUE;
}

//替换字符串
NPC_BOOL	NPC_F_SYS_ReplaceStringEx(
	IN		PNPC_CHAR						in_pStrBuf,				//字符串缓冲区
	IN		PNPC_CHAR						in_pOldStr,				//源字符串
	IN		PNPC_CHAR						in_pNewStr				//新字符串
	)
{
	PNPC_CHAR								pOldStrPos;
	NPC_INT									iOldBeginPos;
	NPC_INT									iOldStrLen;

	pOldStrPos = strstr(in_pStrBuf, in_pOldStr);
	if ( pOldStrPos == NULL )
		return FALSE;

	iOldBeginPos = pOldStrPos - in_pStrBuf;
	iOldStrLen = strlen(in_pOldStr);
	
	//替换字符串中的指定位置
	return NPC_F_SYS_ReplaceString(
		in_pStrBuf,				//字符串缓冲区
		iOldBeginPos,			//开始位置
		iOldStrLen,				//原长度
		in_pNewStr				//新值
		);
}

//忽略大小写比较字符串
NPC_INT		NPC_F_SYS_StrCaseCmp(
	IN		PNPC_CHAR						in_pStr1,				//字符串1
	IN		PNPC_CHAR						in_pStr2				//字符串2
	)
{
#ifdef _WIN32
	return stricmp(in_pStr1, in_pStr2);
#else
	return strcasecmp(in_pStr1, in_pStr2);
#endif
}

//跳过空格或TAB符
NPC_VOID	NPC_F_SYS_StrOp_JumpSpace(
	INOUT	PNPC_CHAR*						inout_ppStr				//字符串
	)
{
	while ( **inout_ppStr )
	{
		if ( !(**inout_ppStr == ' ' || **inout_ppStr == '\t') )
		{
			break;
		}

		(*inout_ppStr)++;
	}
}

//获取一段字符串，返回0表示成功，1表示未找到，2表示返回缓冲区太小
NPC_INT		NPC_F_SYS_StrOp_GetSectString(
	INOUT	PNPC_CHAR*						inout_ppSrcStr,			//原字符串，返回提取之后的位置
	IN		PNPC_CHAR						in_pFindStr,			//查询字符串
	IN		NPC_INT							in_iGetStrBufSize,		//返回的字符串缓冲区大小
	OUT		PNPC_CHAR						out_pGetStrBuf			//返回的字符串缓冲区，由上层分配
	)
{
	PNPC_CHAR								pFindPos;
	NPC_INT									iGetStrLen;

	if ( **inout_ppSrcStr == 0 )
		return 1;

	pFindPos = strstr(*inout_ppSrcStr, in_pFindStr);
	if ( pFindPos == NULL )
	{
		if ( (NPC_INT)strlen(*inout_ppSrcStr) >= in_iGetStrBufSize )
			return 2;

		strcpy(out_pGetStrBuf, *inout_ppSrcStr);
		(*inout_ppSrcStr) += strlen(*inout_ppSrcStr);
	}
	else
	{
		iGetStrLen = pFindPos - *inout_ppSrcStr;
		if ( iGetStrLen >= in_iGetStrBufSize )
			return 2;

		memcpy(out_pGetStrBuf, *inout_ppSrcStr, iGetStrLen);
		out_pGetStrBuf[iGetStrLen] = 0;

		*inout_ppSrcStr = pFindPos + strlen(in_pFindStr);
	}

	return 0;
}

//去除左空格或TAB符
NPC_VOID	NPC_F_SYS_RemoveLeftSpace(
	IN		PNPC_CHAR						in_pStr					//字符串
	)
{
	PNPC_CHAR								pPos;
	PNPC_CHAR								pEndPos;
	NPC_INT									iSyLen;

	if ( *in_pStr == 0 )
		return;

	if ( !(*in_pStr == ' ' || *in_pStr == '\t') )
		return;

	iSyLen = 0;
	pPos = in_pStr;
	pEndPos = in_pStr + strlen(in_pStr) - 1;

	while ( pPos <= pEndPos )
	{
		if ( !(*pPos == ' ' || *pPos == '\t') )
		{
			iSyLen = strlen(pPos);
			break;
		}
		pPos++;
	}
	if ( iSyLen > 0 )
		memcpy(in_pStr, pPos, iSyLen);
	in_pStr[iSyLen] = 0;
}

//去除右空格或TAB符
NPC_VOID	NPC_F_SYS_RemoveRightSpace(
	IN		PNPC_CHAR						in_pStr					//字符串
	)
{
	PNPC_CHAR								pPos;

	if ( *in_pStr == 0 )
		return;

	pPos = in_pStr + strlen(in_pStr) - 1;
	while ( pPos >= in_pStr )
	{
		if ( !(*pPos == ' ' || *pPos == '\t') )
			break;

		*pPos = 0;
		pPos++;
	}
}

//去除左右空格或TAB符
NPC_VOID	NPC_F_SYS_RemoveTwoWaySpace(
	IN		PNPC_CHAR						in_pStr					//字符串
	)
{
	//去除右空格或TAB符
	NPC_F_SYS_RemoveRightSpace(
		in_pStr		//字符串
		);

	//去除左空格或TAB符
	NPC_F_SYS_RemoveLeftSpace(
		in_pStr		//字符串
		);
}


//--------------------------------------------------------------------------------------------------
//读取一个BIT
NPC_BYTE	NPC_F_SYS_MG_get_one_bit(NPC_BYTE* &ppBytePos, NPC_INT &iBitPos, NPC_INT &iSyByteNum)
{
	NPC_BYTE bt = *ppBytePos;

	bt = (bt << iBitPos);
	bt = (bt >> 7);

	if ( ++iBitPos >= 8 )
	{
		iBitPos = 0;
		ppBytePos++;
		iSyByteNum--;
	}

	return bt;
}

//读取一段BIT
NPC_UINT	NPC_F_SYS_MG_read_bits(NPC_BYTE* &ppBytePos, NPC_INT &iBitPos, NPC_INT &iSyByteNum, NPC_INT iReadBitNum)
{
	NPC_UINT uiRet;

	uiRet = 0;

	for ( NPC_INT i=0; i<iReadBitNum; i++ )
	{
		uiRet = uiRet << 1;

		uiRet += NPC_F_SYS_MG_get_one_bit(ppBytePos, iBitPos, iSyByteNum);
	}

	return uiRet;
}

//读取UE
NPC_UINT	NPC_F_SYS_MG_get_ue(NPC_BYTE* &ppBytePos, NPC_INT &iBitPos, NPC_INT &iSyByteNum)
{
	NPC_INT leadingZeroBits;
	NPC_UINT codeNum;
	NPC_UINT b;

	leadingZeroBits = -1;

	for( b = 0; !b; leadingZeroBits++ )
	{
		b = NPC_F_SYS_MG_get_one_bit(ppBytePos, iBitPos, iSyByteNum);
	}

	codeNum = (1 << leadingZeroBits) - 1 + NPC_F_SYS_MG_read_bits(ppBytePos, iBitPos, iSyByteNum, leadingZeroBits);

	return codeNum;
}


//--------------------------------------------------------------------------------------------------
//获取随机数
NPC_UINT	NPC_F_SYS_GetRandomNumber()
{
	return rand();
}

//二进制编码
NPC_BOOL	NPC_F_SYS_BIT_Encode(
	IN		PNPC_BYTE						in_pBitDataBuf,			//二进制数据缓冲区
	IN		NPC_INT							in_iBitDataLen,			//二进制数据长度
	IN		PNPC_CHAR						in_pMapTable,			//映射表
	OUT		PNPC_CHAR						out_pCodecStrBuf		//编码字符串，空间为in_iBitDataLen*2
	)
{
	NPC_UINT								uiTempByte;
	PNPC_BYTE								pos;
	NPC_INT									iSyLen;
	NPC_INT									iBitPos;
	NPC_INT									iIdIndex;

	if ( strlen(in_pMapTable) < 32 )
		return FALSE;

	pos = in_pBitDataBuf;
	iSyLen = in_iBitDataLen;
	iBitPos = 0;
	iIdIndex = 0;

	while ( iSyLen > 0 )
	{
		uiTempByte = NPC_F_SYS_MG_read_bits(pos, iBitPos, iSyLen, 5);
		if ( uiTempByte >= 32 )
		{
			return FALSE;
		}

		out_pCodecStrBuf[iIdIndex++] = in_pMapTable[uiTempByte];
	}
	out_pCodecStrBuf[iIdIndex] = 0;

	return TRUE;
}

//解析编码为二进制数据
NPC_BOOL	NPC_F_SYS_BIT_Decode(
	IN		PNPC_CHAR						in_pCodecStr,			//编码字符串
	IN		PNPC_CHAR						in_pMapTable,			//映射表
	OUT		PNPC_BYTE						out_pBitDataBuf,		//二进制数据缓冲区
	OUT		PNPC_INT						out_pBitDataLen			//二进制数据长度
	)
{
	NPC_BYTE								ucTempByte;
	NPC_INT									iCodecLen;
	NPC_INT									iSyBitNum;
	NPC_INT									i, k;

	if ( strlen(in_pMapTable) != 32 )
		return FALSE;

	iCodecLen = strlen(in_pCodecStr);
	if ( iCodecLen <= 0 )
		return FALSE;

	iSyBitNum = 0;

	for ( k=0; k<iCodecLen; k++ )
	{
		ucTempByte = 0;
		for ( i=0; i<32; i++ )
		{
			if ( in_pCodecStr[k] == in_pMapTable[i] )
			{
				ucTempByte = i;
				break;
			}
		}
		if ( i == 32 )
			return FALSE;
	}

	return TRUE;
}


//--------------------------------------------------------------------------------
//查询字符串
NPC_BOOL	NPC_F_SYS_QueryStr(
	IN		PNPC_BYTE						in_pFindDataBuf,		//待查询的数据缓冲区
	IN		NPC_INT							in_iFindDataLen,		//待查询的数据长度
	IN		PNPC_CHAR						in_pFindStr,			//查询字符串
	OUT		PNPC_BYTE*						out_ppFindPos,			//查询到的位置
	OUT		PNPC_INT						out_pSyDataLen			//剩余数据长度
	)
{
	PNPC_BYTE								pCurPos;
	PNPC_BYTE								pEndPos;
	NPC_INT									iFindStrLen;

	if ( in_pFindDataBuf == NULL || in_iFindDataLen <= 0 || in_pFindStr == NULL )
		return FALSE;

	iFindStrLen = strlen(in_pFindStr);
	if ( iFindStrLen <= 0 )
		return FALSE;

	if ( in_iFindDataLen < iFindStrLen )
		return FALSE;

	pCurPos = in_pFindDataBuf;
	pEndPos = in_pFindDataBuf + in_iFindDataLen - iFindStrLen;

	switch ( iFindStrLen )
	{
	case 1:
		while ( pCurPos <= pEndPos )
		{
			if ( *pCurPos == in_pFindStr[0] )
			{
				*out_ppFindPos = pCurPos;
				*out_pSyDataLen = in_pFindDataBuf + in_iFindDataLen - pCurPos;
				return TRUE;
			}

			pCurPos++;
		}
		break;
	case 2:
		while ( pCurPos <= pEndPos )
		{
			if ( *pCurPos == in_pFindStr[0] && 
				*(pCurPos+1) == in_pFindStr[1] )
			{
				*out_ppFindPos = pCurPos;
				*out_pSyDataLen = in_pFindDataBuf + in_iFindDataLen - pCurPos;
				return TRUE;
			}

			pCurPos++;
		}
		break;
	case 3:
		while ( pCurPos <= pEndPos )
		{
			if ( *pCurPos == in_pFindStr[0] && 
				*(pCurPos+1) == in_pFindStr[1] && 
				*(pCurPos+2) == in_pFindStr[2] )
			{
				*out_ppFindPos = pCurPos;
				*out_pSyDataLen = in_pFindDataBuf + in_iFindDataLen - pCurPos;
				return TRUE;
			}

			pCurPos++;
		}
		break;
	case 4:
		while ( pCurPos <= pEndPos )
		{
			if ( *pCurPos == in_pFindStr[0] && 
				*(pCurPos+1) == in_pFindStr[1] && 
				*(pCurPos+2) == in_pFindStr[2] && 
				*(pCurPos+3) == in_pFindStr[3] )
			{
				*out_ppFindPos = pCurPos;
				*out_pSyDataLen = in_pFindDataBuf + in_iFindDataLen - pCurPos;
				return TRUE;
			}

			pCurPos++;
		}
		break;
	default:
		while ( pCurPos <= pEndPos )
		{
			if ( memcmp(pCurPos, in_pFindStr, iFindStrLen) == 0 )
			{
				*out_ppFindPos = pCurPos;
				*out_pSyDataLen = in_pFindDataBuf + in_iFindDataLen - pCurPos;
				return TRUE;
			}

			pCurPos++;
		}
		break;
	}

	return FALSE;
}

//提取一行数据
NPC_BOOL	NPC_F_SYS_GetLineData(
	IN		PNPC_BYTE						in_pFindDataBuf,		//待查询的数据缓冲区
	IN		NPC_INT							in_iFindDataLen,		//待查询的数据长度
	OUT		PNPC_INT						out_pLineDataLen,		//行数据长度
	OUT		PNPC_BYTE*						out_ppNextFindPos,		//下一个查询位置
	OUT		PNPC_INT						out_pSyDataLen			//剩余数据长度
	)
{
	PNPC_BYTE								pFindPos;
	NPC_INT									iSyLen;

	pFindPos = NULL;
	iSyLen = 0;

	//查询字符串
	if ( !NPC_F_SYS_QueryStr(
		in_pFindDataBuf,		//待查询的数据缓冲区
		in_iFindDataLen,		//待查询的数据长度
		(PNPC_CHAR)"\r\n",		//查询字符串
		&pFindPos,				//查询到的位置
		&iSyLen					//剩余数据长度
		) )
	{
		return FALSE;
	}

	*out_pLineDataLen = pFindPos - in_pFindDataBuf + 2;

	*out_ppNextFindPos = pFindPos + 2;
	*out_pSyDataLen = iSyLen - 2;

	return TRUE;
}



