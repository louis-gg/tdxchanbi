#pragma once
class Kline
{
	public :
		int index;
		float high;
		float low;
		float close;
		Kline() { index = -1; high = 0; low = 0; close = 0; };
		Kline(int index, float high, float low, float close);
};

