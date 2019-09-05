#pragma once
#ifndef GenA_h
#define GenA_h
#include <string>
#include <stdlib.h>
#include <iostream>
#include <time.h>
#include <math.h>
#include <vector>
#include <algorithm>
#include <random>
#include "Portfolio.h"
using namespace std;

#define MUTATION_RATE             0.03
#define POP_SIZE                  100       //must be an even number
#define CHROMO_LENGTH             10
#define MAX_ALLOWABLE_GENERATIONS   1000

//returns a float between 0 & 1
#define RANDOM_NUM      ((float)rand()/RAND_MAX)

typedef vector<float> Vector;

//Detail your design of all the classes, your crossover and mutation logic. Specify any unique or improvement to our proposal.
void GetSectorStockTicker(vector<string>&portfolio_ticker_list, vector<vector<string>>&sector_category)
{
	for (vector<vector<string>>::iterator it = sector_category.begin(); it != sector_category.end(); ++it)
	{
		// int randNum = rand()%(max-min + 1) + min
		int Rand_Num = rand() % (sector_category.size());
		string ticker = (*it)[Rand_Num];
		portfolio_ticker_list.push_back(ticker);
	}
	for (int i = 9; i != portfolio_ticker_list.size() - 1; i++)
	{
		portfolio_ticker_list.pop_back();
	}

}

void GetRandomStockTicker( vector<string>&portfolio_ticker_list, vector <string> &SP_ticker)
{
	int length = 10;
	std::random_device rd;
	std::mt19937 g(rd());
	std::shuffle(SP_ticker.begin()+1, SP_ticker.end(), g);
	for (vector<string>::iterator it = SP_ticker.begin()+1; it != SP_ticker.begin() + length+1; ++it)
		portfolio_ticker_list.push_back(*it);
}
void Mutate(Portfolio &offspring, map<string, Stock*> &sp500_Stock)
{
	int num = rand() % 100 + 1;
	int len = 10;
	Stock mySPY = offspring.get_mySPY();
	vector<string>SP_ticker = offspring.get_SP_ticker();
	vector<Stock*> new_portfolio_Stock_list;
	vector<Stock*> portfolio_Stock_list = offspring.get_Stock_list();
	vector<string> portfolio_ticker_list = offspring.get_portfolio_ticker_list();
	sort(portfolio_ticker_list.begin(), portfolio_ticker_list.end());
	for (int i = 1; i < portfolio_ticker_list.size(); i++)
		if (portfolio_ticker_list[i - 1] == portfolio_ticker_list[i])
		{
			bool btn = true;
			while (btn)
			{
				string ticker_1 = SP_ticker[rand() % (SP_ticker.size() - 1) + 1];
				if (std::find(portfolio_ticker_list.begin(), portfolio_ticker_list.end(), ticker_1) != portfolio_ticker_list.end())
				{
				}
				else
				{
					portfolio_ticker_list[i] = ticker_1;
					btn = false;
				}
			}
		}
	for (vector<string>::iterator it = portfolio_ticker_list.begin(); it != portfolio_ticker_list.end(); ++it)
	{
		new_portfolio_Stock_list.push_back(sp500_Stock[*it]);
	}

	if (num < 4)
	{
		new_portfolio_Stock_list.clear();
		int randNum = rand() % (SP_ticker.size() - 1) + 1;
		int randNum_1 = rand() % (SP_ticker.size() - 1) + 1;
		
		string ticker = (SP_ticker[randNum]);
		string ticker_ = (SP_ticker[randNum_1]);
		
		int rand_portfolio = rand() % (portfolio_Stock_list.size() - 1) + 1;
		portfolio_Stock_list.pop_back();
		portfolio_ticker_list.pop_back();
		portfolio_Stock_list.pop_back();
		portfolio_ticker_list.pop_back();
		portfolio_Stock_list.push_back(sp500_Stock[ticker]);
		portfolio_ticker_list.push_back(ticker);
		portfolio_Stock_list.push_back(sp500_Stock[ticker_]);
		portfolio_ticker_list.push_back(ticker_);

		for (int i = 1; i < portfolio_ticker_list.size(); i++)
			if (portfolio_ticker_list[i - 1] == portfolio_ticker_list[i])
			{
				bool btn = true;
				while (btn)
				{
					string ticker_1 = SP_ticker[rand() % (SP_ticker.size() - 1) + 1];
					if (std::find(portfolio_ticker_list.begin(), portfolio_ticker_list.end(), ticker_1) != portfolio_ticker_list.end())
					{
						btn = true;
					}
					else
					{
						portfolio_ticker_list[i] = ticker_1;
						btn = false;
					}
				}
			}
		std::random_device rd;
		std::mt19937 g(rd());
		std::shuffle(portfolio_ticker_list.begin(), portfolio_ticker_list.end(), g);
		for (vector<string>::iterator it = portfolio_ticker_list.begin(); it != portfolio_ticker_list.end(); ++it)
		{
			new_portfolio_Stock_list.push_back(sp500_Stock[*it]);
		}
	}
	//cout << "portoflio list   " << portfolio_ticker_list << endl;
	offspring.update_list(new_portfolio_Stock_list, portfolio_ticker_list);
	return;
}

void Crossover(Portfolio &offspring1, Portfolio &offspring2)
{
	std::random_device rd;
	std::mt19937 g(rd());
	Stock mySPY = offspring1.get_mySPY();
	vector<string> SP_ticker = offspring1.get_SP_ticker();
	int crossover = (int)(RANDOM_NUM * CHROMO_LENGTH);
	vector<Stock*> portfolio_stock_list_1 = offspring1.get_Stock_list();
	vector<Stock*> portfolio_stock_list_2 = offspring2.get_Stock_list();
	std::shuffle(portfolio_stock_list_1.begin(), portfolio_stock_list_1.end(), g);
	std::shuffle(portfolio_stock_list_2.begin(), portfolio_stock_list_2.end(), g);
	vector<Stock*> new_Stock_list_1, new_Stock_list_2;
	vector<string> new_ticker_list_1, new_ticker_list_2;
	for (vector<Stock*>::iterator it = portfolio_stock_list_1.begin(); it != portfolio_stock_list_1.begin() + crossover; ++it)
	{
		new_Stock_list_1.push_back(*it);
		new_ticker_list_1.push_back((*it)->get_symbol());
	}
	for (vector<Stock*>::iterator it = portfolio_stock_list_2.begin(); it != portfolio_stock_list_2.begin() + crossover; ++it)
	{
		new_Stock_list_2.push_back(*it);
		new_ticker_list_2.push_back((*it)->get_symbol());
	}
	for (vector<Stock*>::iterator it = portfolio_stock_list_2.begin() + crossover; it != portfolio_stock_list_2.end(); ++it)
	{
		new_Stock_list_1.push_back(*it);
		new_ticker_list_1.push_back((*it)->get_symbol());
	}
	for (vector<Stock*>::iterator it = portfolio_stock_list_1.begin() + crossover; it != portfolio_stock_list_1.end(); ++it)
	{
		new_Stock_list_2.push_back(*it);
		new_ticker_list_2.push_back((*it)->get_symbol());
	}
	offspring1.update_list(new_Stock_list_1, new_ticker_list_1);
	offspring2.update_list(new_Stock_list_2, new_ticker_list_2);
}

void New_Population(vector<Portfolio*> &Population)
{
	std::sort(Population.begin(), Population.end(),Portfolio::compByFitness);
	int j = Population.size() - 1;
	for (int i = 0; i < Population.size()*0.2; i += 2)
	{
		Population[i] = Population[j];
		Population[i + 1] = Population[j];
		j -= 1;
	}
	std::sort(Population.begin(), Population.end(), Portfolio::compByFitness);
	
	for (vector<Portfolio*>::iterator it = Population.begin(); it != Population.end(); it++)
	{
		cout << (*it)->get_fitness();
		cout << (*it)->get_portfolio_ticker_list()<<endl;
	}
}
#endif
