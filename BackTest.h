#pragma once
#ifndef BackTest_h
#define BackTest_h
#include "Portfolio.h"
#include "Stock.h"
#include<iostream>
#include <time.h>
#include <ctime>
#include <sstream>
#include <iomanip>  
#pragma warning(disable : 4996) 
using namespace std;

float Test(Portfolio* best_Portfolio)
{
	int cash = 1000000;
	Stock mySPY = best_Portfolio->get_mySPY();
	vector<Stock*> stock_list = best_Portfolio->get_Stock_list();
	vector<float> weight_list;
	best_Portfolio->total_share_held_cal();
	for (vector<Stock*>::iterator it= stock_list.begin();it!= stock_list.end();it++)
	{
		weight_list.push_back(((*it)->get_share_held())/ (best_Portfolio->get_total_share_held()));
	}
	vector<Trade*> SPY_trades = mySPY.get_trades();
	vector<int>  all_day;
	vector<float> adjusted_close_vector;
	vector<float> open_vector;
	vector<float> SPY_adjusted_close_vector;
	vector<float> SPY_open_vector;
	vector<int> month_vector_stock;
	int action = 0;
	for (vector<Trade*>::iterator it = SPY_trades.begin(); it != SPY_trades.end()-1;++it)
	{
		string date = (*it)->get_date();
		std::tm t = {};
		std::istringstream ss(date);
		ss >> std::get_time(&t, "%Y-%m-%d");
		std::mktime(&t);
		int get_month = t.tm_mon+1;
		int day = t.tm_wday;
		
		string date_1 = (*(it +1))->get_date();
		std::tm t_1 = {};
		std::istringstream ss_1(date_1);
		ss_1 >> std::get_time(&t_1, "%Y-%m-%d");
		std::mktime(&t_1);
		int get_month_1 = t_1.tm_mon + 1;

		int position = std::distance(SPY_trades.begin(), it);
		if (get_month<=8)// month)
		{
			for (vector<Stock*>::iterator itr = stock_list.begin(); itr != stock_list.end(); itr++)
			{
				vector<Trade*> myTrade = (*itr)->get_trades();
				vector <float> portfolio_price;
				int first_day = t.tm_wday;
				int next_day = t_1.tm_wday;
				if ((first_day > next_day))
				{
					action = -1;
					float adjusted_close = (*myTrade[position]).get_close();
					float SPY_adjusted_close = (*SPY_trades[position]).get_close();
					adjusted_close_vector.push_back(adjusted_close);
					SPY_adjusted_close_vector.push_back(SPY_adjusted_close);
				}
				else if (first_day < next_day && action == 0)// 1<2
				{
					//cout << day << endl;
					float open = (*myTrade[position]).get_open();
					float SPY_open = (*SPY_trades[position]).get_open();
					open_vector.push_back(open);
					SPY_open_vector.push_back(SPY_open);
					month_vector_stock.push_back(get_month);
				}
				else
				{
					continue;
				}

			}
			action += 1;

		}
		
	}
	// Here first calculate the weekly return
	vector<int> share_list;
	vector<float> excess_return_list(0);
	vector<int> month_vector;
	for (int i = 0; i !=adjusted_close_vector.size(); i = i + 10)
	{
		month_vector.push_back(month_vector_stock[i]);
		share_list.clear();
		float left_cash = cash;
		for (int j = 0; j != stock_list.size(); j++)
		{
			int share_1 = (int)((cash *weight_list[j]) / open_vector[i + j]);
			share_list.push_back(share_1);
			left_cash = left_cash - share_1 * open_vector[i + j];
		}
		float portfolio_total_return = 0;
		float cash_invested = cash - left_cash;
		float share_for_SPY = cash_invested / SPY_open_vector[i];
		for (int k=0;k!= share_list.size();k++)
		{
			portfolio_total_return += share_list[k] * adjusted_close_vector[i + k];
			cout << share_list[k] << endl;
		}
		
		float SPY_total_return = share_for_SPY * SPY_adjusted_close_vector[i];
		float excess_return = portfolio_total_return - SPY_total_return;
		excess_return_list.push_back(excess_return);
		cout <<"Weekly excess return " <<excess_return  << endl;
	}
	
	//Here calculate the monthly excess return
	int month = month_vector[0];
	while (month< month_vector[month_vector.size()-1]+1)
	{
		float month_excess_return = 0;
		for (int i = 0; i != excess_return_list.size(); i += 1)
		{
			if (month_vector[i] == month)
			{
				month_excess_return += excess_return_list[i];
			}
		}
		cout << "The month " << month << " has excess return " << month_excess_return<<endl;
		month += 1;
	}
	return vector_average(excess_return_list);

		
}
#endif   