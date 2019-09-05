#pragma once
#ifndef AddMissing_h
#define AddMissing_h
#include "Stock.h"
#include "Trade.h"
#include <vector>
#include <numeric>
#include <limits>
#include <iostream>
using namespace std;


void fill_missing_return(map<string,Stock*> & sp500_Stock)//need more
{
	Stock* mySPY = sp500_Stock["SPY"];
	mySPY->cal_daily_return_map();
	vector < Trade*> myTrades = mySPY->get_trades();
	map<string, float> myTrades_return = mySPY->get_daily_return_map();
	map<string,Stock*>  new_sp500_Stock;
	for (map<string,Stock*>::iterator it = sp500_Stock.begin(); it != sp500_Stock.end(); ++it)
	{
		if ((it->second)->get_trades().size() == myTrades.size())
		{
			continue;
		}

		Stock* stock = it->second;
		string begin_date = (stock->get_trades()[0])->get_date();
		string end_date = stock->get_trades()[stock->get_trades().size() - 1]->get_date();
		int position_1 = std::distance(std::begin(myTrades_return), myTrades_return.find(begin_date));
		stock->cal_daily_return_map();
		if(myTrades.end() - myTrades.begin() -position_1 ==(it->second)->get_trades().size())
			continue;
		for (vector<Trade*>::iterator itr = myTrades.begin() + position_1+1; itr != myTrades.end(); itr++)
		{
			
			string date = (*itr)->get_date();
			if ((stock->get_daily_return_map()).count(date) == 0)
			{
				(stock->get_daily_return_map())[(*itr)->get_date()] = (stock->get_daily_return_map())[(*(itr - 1))->get_date()];
			}
		}

	}
	cout << "addmissing finished" << endl;

}
#endif
