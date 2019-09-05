#pragma once
#ifndef Stock_h
#define Stock_h
#include <iostream>
#include <string>
#include <map>
#include <fstream>
#include <vector>
#include <stdio.h>
#include "Utility.h"
#include "Trade.h"
#include "Fundamental_data.h"

#define NUM_OF_STOCKS 505

using namespace std;
class Stock
{
private:
	string symbol;
	vector<Trade*> trades;
	float weight;
	float share_held;
	Fundamental_Data* fundamentals;
	map<string, float> RiskFreeRate;
	map<string, float> daily_return_map;
	vector<float> daily_return_vector;
	vector<float> RiskFreeRate_vector;
	vector<float> ROC_vector;
public:
	Stock()
	{}
	
	Stock(string symbol_, float weight_) :symbol(symbol_), weight(weight_)
	{}
	Stock(string symbol_) :symbol(symbol_)
	{}
	~Stock() {}
	void addFundamental(Fundamental_Data * aFundamental_Data)
	{
		fundamentals = aFundamental_Data;
	}
	void addTrade(Trade *aTrade)
	{
		trades.push_back(aTrade);
	}
	void clear_trade()
	{
		trades.clear();
	}
	void addWeight(float weight_)
	{
		weight = weight_;
	}
	void addRiskFreeRate(vector<float> & RiskFreeRate_vector_)
	{
		RiskFreeRate_vector = RiskFreeRate_vector_;
	}
	void addShareHeld(float share_held_)
	{
		share_held = share_held_;
	}
	vector<Trade*> get_trades()
	{
		return trades;
	}
	string get_symbol()
	{
		return symbol;
	}
	map<string, float> Get_RiskFreeRate()
	{
		return RiskFreeRate;
	}
	Fundamental_Data* Get_fundamentals()
	{
		return fundamentals;
	}

	friend ostream & operator << (ostream & out, const Stock & s)
	{
		out << "Symbol: " << s.symbol << endl;
		for (vector<Trade*>::const_iterator itr = s.trades.begin(); itr != s.trades.end(); itr++)
			out << *(*itr);
		return out;
	}
	float get_weight()
	{
		return weight;
	}
	float get_share_held()
	{
		return share_held;
	}
	vector<float>get_RiskFreeRate_vector()
	{
		return RiskFreeRate_vector;
	}
	float daily_return(int num)
	{
		if (num == 0)
		{
			return 0;
		}
		float daily_return = (trades[num]->get_adjusted_close() - trades[num - 1]->get_adjusted_close()) / trades[num - 1]->get_adjusted_close();
		return daily_return;

	}
	void cal_daily_return_vector()
	{
		daily_return_vector.clear();
		for (int i = 1; i < trades.size(); i++)
		{
			daily_return_vector.push_back(trades[i]->get_daily_return());// trades[i].get_daily_return();
		}
		
	}
	vector<float> cal_daily_close_vector()
	{
		vector<float> daily_close_vector(0);
		int size = trades.size();
		for (int i = 1; i < 253; i++)
		{
			daily_close_vector.push_back(trades[size-i]->get_close());// trades[i].get_daily_return();
		}
		return daily_close_vector;
	
	}
	
	float cal_MA()
	{
		const int NUMBERS_SIZE = ROC_vector.size();
		int windowSize = 6;
		double sum = 0.0;
		float movingAverage = 0.0;
		for (int i = 0; i <= (ROC_vector.size() - windowSize); i++)
		{
			sum = 0.0;
			for (int j = i; j < i + windowSize; j++)
				sum += ROC_vector[j];
			movingAverage = sum / windowSize;
		}
		return movingAverage;
	}
	void cal_ROC_vector()
	{
	   ROC_vector.clear();
	   vector<float> daily_close_vector=cal_daily_close_vector();
	   for(vector<float>::iterator it=daily_close_vector.begin()+13; it!=daily_close_vector.end();++it)
	      ROC_vector.push_back((*it-*(it-12))*100/(*(it-12)));
	}
	float MAROC()
	{
		cal_ROC_vector();
		float MA_for_ROC = cal_MA();
		vector<float>MA_ROC;
		for (vector<float>::iterator it = ROC_vector.begin(); it != ROC_vector.end(); ++it)
		{
			MA_ROC.push_back((*it)/ MA_for_ROC);
		}

		return vector_average(MA_ROC);
	}
	
	void cal_daily_return_map()
	{
		for (int i = 1; i < trades.size(); i++)
		{
			daily_return_map[trades[i]->get_date()] = trades[i]->get_daily_return();
		}
	}
	map<string, float> get_daily_return_map()
	{
		return daily_return_map;

	}
	vector< float> get_daily_return_vector()
	{
		return daily_return_vector;
	}

	vector<float> get_close_price_vector()
	{
		vector<float> close_price_vector(0);
		for (vector<Trade*> ::iterator it = trades.end() - 125; it != trades.end(); ++it)
		{
			close_price_vector.push_back((*it)->get_close());
		}
		return close_price_vector;
	}
	static bool compBySize(Stock* x, Stock* y)
	{
		return ((x->get_trades()).size() < (y->get_trades()).size());
	}

};
#endif
