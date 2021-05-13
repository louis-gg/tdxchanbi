#include "stdafx.h"
#include "TCalcFuncSets.h"
#include "ChanAnalyze.h"

//���ɵ�dll���������dll�뿽����ͨ���Ű�װĿ¼��T0002/dlls/����,���ڹ�ʽ���������а�

 
void TestPlugin1(int DataLen,float* pfOUT,float* pfINa,float* pfINb,float* pfINc)
{
	
	OutputDebugString("��ʼ�����ֵ");

	OutputDebugString("��ʼ������");
	ChanAnalyze chan = ChanAnalyze();
	chan.InitChan(DataLen, pfINa, pfINb, pfINc);
	char outStr[1024] = { 0 };
	sprintf(outStr, "fxsize:%d,bisize:%d,klinesize:%d,mergesize:%d", chan.fxs.size(), chan.bis.size(), chan.klines.size(), chan.klinesMerge.size());
	OutputDebugString(outStr);
	OutputDebugString("�������۳�ʼ��");
	for (int i = 0; i < chan.bis.size(); i++) 
	{
		Bi currentB = chan.bis[i];
		pfOUT[currentB.bifx.k2.index] = currentB.mark;
	}
	OutputDebugString("��������");
		
		
}

void TestPlugin2(int DataLen,float* pfOUT,float* pfINa,float* pfINb,float* pfINc)
{
	for(int i=0;i<DataLen;i++)
	{
		pfOUT[i]=pfINa[i]+pfINb[i]+pfINc[i];
		pfOUT[i]=pfOUT[i]/3;
	}
}


//���صĺ���
PluginTCalcFuncInfo g_CalcFuncSets[] = 
{
	{1,(pPluginFUNC)&TestPlugin1},
	{2,(pPluginFUNC)&TestPlugin2},
	{0,NULL},
};

//������TCalc��ע�ắ��
BOOL RegisterTdxFunc(PluginTCalcFuncInfo** pFun)
{
	if(*pFun==NULL)
	{
		(*pFun)=g_CalcFuncSets;
		return TRUE;
	}
	return FALSE;
}
