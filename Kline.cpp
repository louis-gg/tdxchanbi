#include "stdafx.h"
#include "Kline.h"
Kline::Kline(int index, float high, float low, float close) 
{
	this->index = index;
	this->high = high;
	this->low = low;
	this->close = close;
}