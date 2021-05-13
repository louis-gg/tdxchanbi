#include "stdafx.h"
#include "TCalcFuncSets.h"
#include "ChanAnalyze.h"

//生成的dll及相关依赖dll请拷贝到通达信安装目录的T0002/dlls/下面,再在公式管理器进行绑定

 
void TestPlugin1(int DataLen,float* pfOUT,float* pfINa,float* pfINb,float* pfINc)
{
	
	OutputDebugString("初始化输出值");

	OutputDebugString("初始化缠论");
	ChanAnalyze chan = ChanAnalyze();
	chan.InitChan(DataLen, pfINa, pfINb, pfINc);
	char outStr[1024] = { 0 };
	sprintf(outStr, "fxsize:%d,bisize:%d,klinesize:%d,mergesize:%d", chan.fxs.size(), chan.bis.size(), chan.klines.size(), chan.klinesMerge.size());
	OutputDebugString(outStr);
	OutputDebugString("结束缠论初始化");
	for (int i = 0; i < chan.bis.size(); i++) 
	{
		Bi currentB = chan.bis[i];
		pfOUT[currentB.bifx.k2.index] = currentB.mark;
	}
	OutputDebugString("结束分析");
		
		
}

void TestPlugin2(int DataLen,float* pfOUT,float* pfINa,float* pfINb,float* pfINc)
{
	for(int i=0;i<DataLen;i++)
	{
		pfOUT[i]=pfINa[i]+pfINb[i]+pfINc[i];
		pfOUT[i]=pfOUT[i]/3;
	}
}


//加载的函数
PluginTCalcFuncInfo g_CalcFuncSets[] = 
{
	{1,(pPluginFUNC)&TestPlugin1},
	{2,(pPluginFUNC)&TestPlugin2},
	{0,NULL},
};

//导出给TCalc的注册函数
BOOL RegisterTdxFunc(PluginTCalcFuncInfo** pFun)
{
	if(*pFun==NULL)
	{
		(*pFun)=g_CalcFuncSets;
		return TRUE;
	}
	return FALSE;
}
