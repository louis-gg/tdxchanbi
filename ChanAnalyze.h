#pragma once
using namespace std;
#include <vector>
#include "Kline.h"
#include "fx.h"
#include "Bi.h"
class ChanAnalyze
{
public :
	vector<Kline> klines; // Ԫ��K�߱�
	vector<Kline> klinesMerge; // ����������K�߱�
	vector<Bi> bis;
	vector<fx> fxs;
	ChanAnalyze() {};
	void InitChan(int DataLen, float* pfHigh, float* pfLow, float* pfClose);
private:
	void InitKlines(int DataLen, float* pfHigh, float* pfLow, float* pfClose);
	void InitKlineMerge();
	void InitFxs();
	void InitBis();
	bool HasKline(int startIndex, int endIndex);

};

