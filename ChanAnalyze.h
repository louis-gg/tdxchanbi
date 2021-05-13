#pragma once
using namespace std;
#include <vector>
#include "Kline.h"
#include "fx.h"
#include "Bi.h"
class ChanAnalyze
{
public :
	vector<Kline> klines; // 元素K线表
	vector<Kline> klinesMerge; // 包含处理后的K线表
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

