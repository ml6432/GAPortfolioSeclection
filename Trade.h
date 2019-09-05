#pragma once
#ifndef Trade_h
#define Trade_h
#include <iostream>
#include <string>
#include <map>
#include <fstream>
#include <vector>
#include <stdio.h>
#include "Utility.h"

#define NUM_OF_STOCKS 505

using namespace std;
class Trade
{
private:
	string date;
	float open;
	float high;
	float low;
	float close;
	float adjusted_close;
	int volume;
	float daily_return;
public:
	Trade(string date_, float open_, float high_, float low_, float close_, float adjusted_close_, int volume_) :
		date(date_), open(open_), high(high_), low(low_), close(close_), adjusted_close(adjusted_close_), volume(volume_)
	{}
	Trade(string date_, float open_, float high_, float low_, float close_, float adjusted_close_, int volume_, float daily_return_) :
		date(date_), open(open_), high(high_), low(low_), close(close_), adjusted_close(adjusted_close_), volume(volume_), daily_return(daily_return_)
	{}
	~Trade() {}
	friend ostream & operator << (ostream & out, const Trade & t)
	{
		out << "Date: " << t.date << " Open: " << t.open << " High: " << t.high << " Low: " << t.low << " Close: " << t.close << " Adjusted_Close: " << t.adjusted_close << " Volume: " << t.volume << " daily_return: " << t.daily_return << endl;
		return out;
	}
	float get_adjusted_close()
	{
		return adjusted_close;
	}
	float get_open()
	{
		return open;
	}
	float get_close()
	{
		return close;
	}
	string get_date()
	{
		return date;
	}
	float get_daily_return()
	{
		return daily_return;
	}

};
#endif
