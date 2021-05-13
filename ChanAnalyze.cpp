#include "stdafx.h"
#include "ChanAnalyze.h"

//初始化缠
void ChanAnalyze::InitChan(int DataLen, float* pfHigh, float* pfLow, float* pfClose)
{
	this->InitKlines(DataLen, pfHigh, pfLow, pfClose);
	this->InitKlineMerge();
	this->InitFxs();
	this->InitBis();
}

//初始化K线
void ChanAnalyze::InitKlines(int DataLen, float* pfHigh, float* pfLow, float* pfClose)
{
	if (DataLen > 0) 
	{
		for (int i = 0; i < DataLen; i++) 
		{
			float currentHigh = pfHigh[i];
			float currentLow = pfLow[i];
			float currentClose = pfClose[i];
			int index = i;
			Kline currentKline = Kline(index, currentHigh, currentLow, currentClose);
			this->klines.push_back(currentKline);

		}
	}
	else 
	{
		OutputDebugStringA("DataLen is zero!");
	}
}


//K线合并
void ChanAnalyze::InitKlineMerge()
{
	int klineLen = this->klines.size();
	if (klineLen <= 0) 
	{
		OutputDebugStringA("klineLen is zero!");
		return;
	}

	this->klinesMerge.push_back(this->klines[0]);
	int direct = -1;
	for (int i = 1; i < klineLen; i++) 
	{
		Kline currentK = this->klines[i];
		Kline lastK = this->klinesMerge.back();
		if (this->klinesMerge.size() == 1) 
		{
			if ((currentK.high > lastK.high) && (currentK.low > lastK.low)) 
			{
				direct = 1;
				this->klinesMerge.push_back(currentK);

			}
			else if ((currentK.high < lastK.high) && (currentK.low < lastK.low)) 
			{
				direct = 0;
				this->klinesMerge.push_back(currentK);
			}
			else 
			{
				this->klinesMerge.pop_back();
				this->klinesMerge.push_back(currentK);
			}
		}
		else 
		{
			int lenMerge = this->klinesMerge.size();
			Kline lastLastk = this->klinesMerge[lenMerge - 2];
			if ((lastLastk.high < lastK.high) && (lastLastk.low < lastK.low))
			{
				direct = 1;
			}
			else 
			{
				direct = 0;
			}
			if ((lastK.high >= currentK.high)&& (lastK.low <= currentK.low) && (direct != -1))
			{
				if (direct == 1) 
				{
					Kline mergeK = Kline(lastK.index, lastK.high, currentK.low, currentK.close);
					this->klinesMerge.pop_back();
					this->klinesMerge.push_back(mergeK);
				}
				else 
				{
					Kline mergeK = Kline(lastK.index, currentK.high, lastK.low, currentK.close);
					this->klinesMerge.pop_back();
					this->klinesMerge.push_back(mergeK);
				}
				continue;
			}

			if ((lastK.high <= currentK.high) && (lastK.low >= currentK.low) && (direct != -1))
			{
				if (direct == 1)
				{
					Kline mergeK = Kline(i, currentK.high, lastK.low, currentK.close);
					
					this->klinesMerge.pop_back();
					this->klinesMerge.push_back(mergeK);
				}
				else
				{
					Kline mergeK = Kline(i, lastK.high, currentK.low, currentK.close);
					this->klinesMerge.pop_back();
					this->klinesMerge.push_back(mergeK);
				}
				continue;
			}
			this->klinesMerge.push_back(currentK);
			
		}
	}
}

//处理分型

void ChanAnalyze::InitFxs() 
{
	int mergeLen = this->klinesMerge.size();
	if (mergeLen >= 2) 
	{
		//第一个分型k1是没有的
		Kline k1 = Kline();
		Kline k2 = this->klinesMerge[0];
		Kline k3 = this->klinesMerge[2];
		if (k2.low < k3.low) 
		{
			fx fx1 = fx(k1, k2, k3, -1);
			this->fxs.push_back(fx1);
		}
		else 
		{
			fx fx1 = fx(k1, k2, k3, 1);
			this->fxs.push_back(fx1);
		}

		for (int i = 2; i < mergeLen; i++) 
		{
			Kline k1 = this->klinesMerge[i - 2];
			Kline k2 = this->klinesMerge[i - 1];
			Kline k3 = this->klinesMerge[i];
			if (k1.low < k2.low && k3.low < k2.low && k1.high < k2.high && k3.high < k2.high) 
			{
				fx fx1 = fx(k1, k2, k3, 1);
				this->fxs.push_back(fx1);
			}
			if (k1.low > k2.low && k3.low > k2.low && k1.high > k2.high && k3.high > k2.high)
			{
				fx fx1 = fx(k1, k2, k3, -1);
				this->fxs.push_back(fx1);
			}
		}

	}else
	{
		OutputDebugStringA("mergeLen is less then 2!");
	}

}
bool ChanAnalyze::HasKline(int startIndex, int endIndex) 
{
	int mergeLen = this->klinesMerge.size();
	int klineLen = this->klines.size();
	if (startIndex < endIndex && startIndex < klineLen && endIndex < klineLen)
	{
		bool bstart = false;
		for (int i = 0; i < mergeLen; i++) 
		{
			Kline currentk = this->klinesMerge[i];
			if (currentk.index == startIndex) 
			{
				bstart = true;
				continue;
			}
			if (bstart && currentk.index < endIndex) 
			{
				return true;
			}
		}
	}
	return false;
}
//处理笔
void ChanAnalyze::InitBis()
{
	int fxLen = this->fxs.size();
	if (fxLen >= 2) 
	{

		Bi currentB = Bi(this->fxs[0]);
		this->bis.push_back(currentB);
		for (int i = 1; i < fxLen; i++) 
		{
			fx currentFx = this->fxs[i];
			Bi lastBi = this->bis.back();
			if (lastBi.mark == -1) 
			{
				//延续
				if (currentFx.mark == -1) 
				{
					if (currentFx.k2.low < lastBi.bifx.k2.low) 
					{
						currentB = Bi(currentFx);
						this->bis.pop_back();
						this->bis.push_back(currentB);
					}
				}
				else 
				{
					if (lastBi.bifx.k3.index < currentFx.k1.index) 
					{
						float lastHigh = lastBi.bifx.k2.high > lastBi.bifx.k3.high ? lastBi.bifx.k2.high : lastBi.bifx.k3.high;
						if (lastBi.bifx.k1.index != -1) 
						{
							lastHigh = lastHigh > lastBi.bifx.k1.high ? lastHigh : lastBi.bifx.k1.high;
						}
						float currentLow = currentFx.k2.low < currentFx.k3.low ? currentFx.k2.low : currentFx.k3.low;
						currentLow = currentLow < currentFx.k1.low ? currentLow : currentFx.k1.low;
						if (lastHigh < currentFx.k2.high && currentLow > lastBi.bifx.k2.low && this->HasKline(lastBi.bifx.k3.index,currentFx.k1.index))
						{
							currentB = Bi(currentFx);
							this->bis.push_back(currentFx);
						}
					}
				}
			}else 
			{
				//延续
				if (currentFx.mark == 1) 
				{
					if (currentFx.k2.high > lastBi.bifx.k2.high)
					{
						currentB = Bi(currentFx);
						this->bis.pop_back();
						this->bis.push_back(currentB);
					}

				}
				else
				{
					if (lastBi.bifx.k3.index < currentFx.k1.index) 
					{
						float lastLow = lastBi.bifx.k2.low < lastBi.bifx.k3.low ? lastBi.bifx.k2.low : lastBi.bifx.k3.low;
						if (lastBi.bifx.k1.index != -1)
						{
							lastLow = lastLow < lastBi.bifx.k1.low ? lastLow : lastBi.bifx.k1.low;
						}
						float currentHigh = currentFx.k2.high > currentFx.k3.high ? currentFx.k2.high : currentFx.k3.high;
						currentHigh = currentHigh > currentFx.k1.high ? currentHigh : currentFx.k1.high;
						if (lastLow > currentFx.k2.low && currentHigh < lastBi.bifx.k2.high && this->HasKline(lastBi.bifx.k3.index, currentFx.k1.index))
						{
							currentB = Bi(currentFx);
							this->bis.push_back(currentFx);
						}
					}

				}
				
				
			}
		}
	}
	else 
	{
		OutputDebugStringA("fxLen is less then 2!");
	}
}
