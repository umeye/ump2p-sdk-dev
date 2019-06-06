/*----------------------------------------------------------------------------
 * Copyright(c)  :  NPC CORPORTAION All Rights Reserved                       
 * FileName      :  NPC_SYS_FuncShare.h
 * Version       :  1.1
 * Author        :  CCH
 * DateTime      :  2012-01-05
 * Description   :  NPC系统移置共享模块
 *----------------------------------------------------------------------------*/

#ifndef __NPC_SYS_FUNCSHARE_H
#define __NPC_SYS_FUNCSHARE_H

#include "NPC_TypeDef.h"


//获取开机时长（毫秒）
NPC_DWORD	NPC_F_SYS_GetTickCount();

//线程休眠N毫秒
NPC_VOID	NPC_F_SYS_Sleep(
	IN		NPC_INT							in_iTimeLen				//休眠时长（毫秒）
	);

//将time_t类型的时间，格式化为YYYY-MM-DD HH24:MI:SS型的字符串
NPC_VOID	NPC_F_SYS_Time_To_Str(
	IN		time_t							in_tTimeValue,			//时间值
	OUT		PNPC_CHAR						out_pTimeStr			//时间字符串
	);

//将YYYY-MM-DD HH24:MI:SS格式的时间字符串，转换为time_t型的时间
time_t		NPC_F_SYS_Str_To_Time(
	IN		PNPC_CHAR						in_pTimeStr				//时间字符串
	);

//将时间参数转换为time_t
time_t		NPC_F_SYS_TimeParam_To_Timet(
	IN		NPC_INT							in_iYear,				//年
	IN		NPC_INT							in_iMonth,				//月
	IN		NPC_INT							in_iDay,				//日
	IN		NPC_INT							in_iHour,				//时
	IN		NPC_INT							in_iMinute,				//分
	IN		NPC_INT							in_iSecond				//秒
	);

//将time_t转换为时间参数
NPC_VOID	NPC_F_SYS_Timet_To_TimeParam(
	IN		time_t							in_tTimeValue,			//时间值
	OUT		PNPC_INT						out_pYear,				//年
	OUT		PNPC_INT						out_pMonth,				//月
	OUT		PNPC_INT						out_pDay,				//日
	OUT		PNPC_INT						out_pHour,				//时
	OUT		PNPC_INT						out_pMinute,			//分
	OUT		PNPC_INT						out_pSecond				//秒
	);

//将time_t转换为NPC_S_TD_DATE_TIME时间
NPC_VOID	NPC_F_SYS_Timet_To_TdTime(
	IN		time_t							in_tTimeValue,			//时间值
	OUT		PNPC_S_TD_DATE_TIME				out_pTdTime				//TD时间
	);

//将字段串时间转换为NPC_S_TD_DATE_TIME时间
NPC_VOID	NPC_F_SYS_StrTime_To_TdTime(
	IN		PNPC_CHAR						in_pTimeStr,			//时间字符串，格式为：YYYY-MM-DD HH24:MI:SS
	OUT		PNPC_S_TD_DATE_TIME				out_pTdTime				//TD时间
	);

//将NPC_S_TD_DATE_TIME转换为time_t时间
time_t		NPC_F_SYS_TdTime_To_Timet(
	IN		PNPC_S_TD_DATE_TIME				in_pTdTime				//TD时间
	);

//将NPC_S_TD_DATE_TIME转换为时间字符串
NPC_VOID	NPC_F_SYS_TdTime_To_TimeStr(
	IN		PNPC_S_TD_DATE_TIME				in_pTdTime,				//TD时间
	OUT		PNPC_CHAR						out_pTimeStr			//时间字符串
	);

//将时间字符串转换为NPC_S_TD_TIME
NPC_VOID	NPC_F_SYS_Str_To_Time(
	IN		PNPC_CHAR						in_pTimeStr,				//时间字符串，格式为：HH:MM:SS
	OUT		PNPC_S_TD_TIME					out_pTimeObj				//时间对像
	);

//将NPC_S_TD_TIME转换时间字符串
NPC_VOID	NPC_F_SYS_Time_To_Str(
	IN		PNPC_S_TD_TIME					in_pTimeObj,				//时间对像
	OUT		PNPC_CHAR						out_pTimeStr				//时间字符串，格式为：HH:MM:SS
	);

//计算两个时间之间的时长
NPC_DWORD	NPC_F_SYS_ComputeTdTimeLen(
	IN		PNPC_S_TD_DATE_TIME				in_pBeginTime,			//开始时间
	IN		PNPC_S_TD_DATE_TIME				in_pEndTime				//结束时间
	);

//计算较验和
NPC_USHORT	NPC_F_SYS_CheckSum(
	IN		PNPC_BYTE						in_pDataBuf,			//数据缓冲区
	IN		NPC_INT							in_iDataLen				//数据长度
	);

//获取IP地址（根据IP地址值主机顺序）
NPC_VOID	NPC_F_SYS_GetIpaddrStrByHost(
	IN		NPC_DWORD						in_dwIpaddr,			//IP地址值
	OUT		PNPC_CHAR						out_pIpaddrStr			//IP地址字符串
	);

//把字符串IP地址转换成主机顺序的IP地址值
NPC_DWORD	NPC_F_SYS_IpaddrStrToHostIpaddr(
	IN		PNPC_CHAR						in_pIpaddrStr			//IP地址字符串
	);

//将二进制MAC地址转换成字符串MAC地址
NPC_VOID	NPC_F_SYS_GetStringMacAddr(
	IN		PNPC_BYTE						in_pBitMac,				//二进制MAC地址
	OUT		PNPC_CHAR						out_pStrMac				//字符串MAC地址
	);

//将字符串MAC地址转换成二进制MAC地址
NPC_VOID	NPC_F_SYS_GetBitMacAddr(
	IN		PNPC_CHAR						in_pStrMac,				//字符串MAC地址
	OUT		PNPC_BYTE						out_pBitMac				//二进制MAC地址
	);

//较验字符串MAC地址格式是否正确
NPC_BOOL	NPC_F_SYS_CheckMacStrAddr(
	IN		PNPC_CHAR						in_pStrMac				//字符串MAC地址，格式为XX-XX-XX-XX-XX-XX
	);

//将二进制数组数据转换成十六进制字符串
NPC_BOOL	NPC_F_SYS_BitDataToHexStr(
	IN		PNPC_BYTE						in_pBitDataBuf,			//二进制数据缓冲区
	IN		NPC_INT							in_iBitDataLen,			//二进制数据长度
	OUT		PNPC_CHAR*						out_ppHexStrBuf			//十六进制字符串缓冲区，由该函数分配内存，上层释放内存
	);

//将十六进制字符串转换成无符号整型值
NPC_DWORD	NPC_F_SYS_HexStrToDWORD(
	IN		PNPC_CHAR						in_pHexStrBuf			//十六进制字符串缓冲区，如0x12345678，或12345678
	);

//替换字符串中的指定位置
NPC_BOOL	NPC_F_SYS_ReplaceString(
	IN		PNPC_CHAR						in_pStrBuf,				//字符串缓冲区
	IN		NPC_INT							in_iBeginPos,			//开始位置
	IN		NPC_INT							in_iOldLen,				//原长度
	IN		PNPC_CHAR						in_pNewValue			//新值
	);

//替换JSON的结点值，只替换第一个结点的值
NPC_BOOL	NPC_F_SYS_ReplaceJsonValue(
	IN		PNPC_CHAR						in_pJsonDocBuf,			//JSON文档缓冲区
	IN		PNPC_CHAR						in_pNodeName,			//结点名字
	IN		PNPC_CHAR						in_pNodeNewValue		//结点新值，包括值的引号
	);

//替换字符串
NPC_BOOL	NPC_F_SYS_ReplaceStringEx(
	IN		PNPC_CHAR						in_pStrBuf,				//字符串缓冲区
	IN		PNPC_CHAR						in_pOldStr,				//源字符串
	IN		PNPC_CHAR						in_pNewStr				//新字符串
	);

//忽略大小写比较字符串
NPC_INT		NPC_F_SYS_StrCaseCmp(
	IN		PNPC_CHAR						in_pStr1,				//字符串1
	IN		PNPC_CHAR						in_pStr2				//字符串2
	);

//跳过空格或TAB符
NPC_VOID	NPC_F_SYS_StrOp_JumpSpace(
	INOUT	PNPC_CHAR*						inout_ppStr				//字符串
	);

//获取一段字符串，返回0表示成功，1表示未找到，2表示返回缓冲区太小
NPC_INT		NPC_F_SYS_StrOp_GetSectString(
	INOUT	PNPC_CHAR*						inout_ppSrcStr,			//原字符串，返回提取之后的位置
	IN		PNPC_CHAR						in_pFindStr,			//查询字符串
	IN		NPC_INT							in_iGetStrBufSize,		//返回的字符串缓冲区大小
	OUT		PNPC_CHAR						out_pGetStrBuf			//返回的字符串缓冲区，由上层分配
	);

//去除左空格或TAB符
NPC_VOID	NPC_F_SYS_RemoveLeftSpace(
	IN		PNPC_CHAR						in_pStr					//字符串
	);

//去除右空格或TAB符
NPC_VOID	NPC_F_SYS_RemoveRightSpace(
	IN		PNPC_CHAR						in_pStr					//字符串
	);

//去除左右空格或TAB符
NPC_VOID	NPC_F_SYS_RemoveTwoWaySpace(
	IN		PNPC_CHAR						in_pStr					//字符串
	);


//--------------------------------------------------------------------------------------------------
//读取一个BIT
NPC_BYTE	NPC_F_SYS_MG_get_one_bit(NPC_BYTE* &ppBytePos, NPC_INT &iBitPos, NPC_INT &iSyByteNum);

//读取一段BIT
NPC_UINT	NPC_F_SYS_MG_read_bits(NPC_BYTE* &ppBytePos, NPC_INT &iBitPos, NPC_INT &iSyByteNum, NPC_INT iReadBitNum);

//读取UE
NPC_UINT	NPC_F_SYS_MG_get_ue(NPC_BYTE* &ppBytePos, NPC_INT &iBitPos, NPC_INT &iSyByteNum);


//--------------------------------------------------------------------------------------------------
//获取随机数
NPC_UINT	NPC_F_SYS_GetRandomNumber();

//二进制编码
NPC_BOOL	NPC_F_SYS_BIT_Encode(
	IN		PNPC_BYTE						in_pBitDataBuf,			//二进制数据缓冲区
	IN		NPC_INT							in_iBitDataLen,			//二进制数据长度
	IN		PNPC_CHAR						in_pMapTable,			//映射表
	OUT		PNPC_CHAR						out_pCodecStrBuf		//编码字符串，空间为in_iBitDataLen*2
	);

//解析编码为二进制数据
NPC_BOOL	NPC_F_SYS_BIT_Decode(
	IN		PNPC_CHAR						in_pCodecStr,			//编码字符串
	IN		PNPC_CHAR						in_pMapTable,			//映射表
	OUT		PNPC_BYTE						out_pBitDataBuf,		//二进制数据缓冲区
	OUT		PNPC_INT						out_pBitDataLen			//二进制数据长度
	);


//--------------------------------------------------------------------------------
//查询字符串
NPC_BOOL	NPC_F_SYS_QueryStr(
	IN		PNPC_BYTE						in_pFindDataBuf,		//待查询的数据缓冲区
	IN		NPC_INT							in_iFindDataLen,		//待查询的数据长度
	IN		PNPC_CHAR						in_pFindStr,			//查询字符串
	OUT		PNPC_BYTE*						out_ppFindPos,			//查询到的位置
	OUT		PNPC_INT						out_pSyDataLen			//剩余数据长度
	);

//提取一行数据
NPC_BOOL	NPC_F_SYS_GetLineData(
	IN		PNPC_BYTE						in_pFindDataBuf,		//待查询的数据缓冲区
	IN		NPC_INT							in_iFindDataLen,		//待查询的数据长度
	OUT		PNPC_INT						out_pLineDataLen,		//行数据长度
	OUT		PNPC_BYTE*						out_ppNextFindPos,		//下一个查询位置
	OUT		PNPC_INT						out_pSyDataLen			//剩余数据长度
	);


#endif

