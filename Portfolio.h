#pragma once
#ifndef Portfolio_h
#define Portfolio_h
#include "Stock.h"
#include <vector>
#include <map>
#include <numeric>
#include <limits>
#include "Utility.h"
//https://patents.google.com/patent/US6484152B1/en
class Portfolio
{
private:
	Stock mySPY;
	float fitness;
	vector<string> SP_ticker;
	vector<Stock*> Stock_list;
	vector<string> portfolio_ticker_list;
	float total_share_held;
	
	vector<float> portfolio_part_return;
	float portfolio_adjuested_beta;
	vector<float> average_RiskFreeRate;
	vector<float> SPY_average_daily_return;
	float sp500_average_dividend;
	float sp500_average_PE;
	float sp500_average_profit;
	float sp500_average_ROA;
	float sp500_average_ROE;
	map<string, float> portfolio_daily_return;
	vector<float> portfolio_return_vector;
public:
	Portfolio()
	{}
	Portfolio(vector<Stock*> Stock_list_) :Stock_list(Stock_list_)
	{}
	void addPortfolio_para(Stock &mySPY_, vector<string> &SP_ticker_, vector<string> &portfolio_ticker_list_, float sp500_average_dividend_, float sp500_average_PE_, float sp500_average_profit_,float sp500_average_ROA_,float sp500_average_ROE_)
	{
		mySPY = mySPY_;
		SP_ticker = SP_ticker_;
		portfolio_ticker_list = portfolio_ticker_list_;
		sp500_average_dividend = sp500_average_dividend_;
		sp500_average_PE =  sp500_average_PE_;
		sp500_average_profit = sp500_average_profit_;
		sp500_average_ROA = sp500_average_ROA_;
		sp500_average_ROE = sp500_average_ROE_;
	}
	~Portfolio() {}
	void update_list(vector<Stock*> Stock_list_, vector<string> portfolio_ticker_list_)
	{
		Stock_list = Stock_list_;
		portfolio_ticker_list = portfolio_ticker_list_;
	}
	void add_mySPY(Stock SPY)
	{
		mySPY = SPY;
	}
	
	vector<string> get_SP_ticker()
	{
		return  SP_ticker;
	}
	vector<Stock*> get_Stock_list()
	{
		return Stock_list;
	}
	Stock get_mySPY()
	{
		return  mySPY;
	}
	vector<string> get_portfolio_ticker_list()
	{
		return  portfolio_ticker_list;
	}
	float get_total_share_held()
	{
		return total_share_held;
	}
	void cal_portfolio_return()
	{
		portfolio_daily_return.clear();
		total_share_held_cal();
		std::sort(Stock_list.begin(), Stock_list.end(), Stock::compBySize);
		for (vector<Stock*>::iterator itr = Stock_list.begin(); itr != Stock_list.end(); itr++)
		{
			(*itr)->cal_daily_return_map();
		}
		for (vector<Stock*>::iterator it = Stock_list.begin(); it != Stock_list.end(); it++)
		{
			if (it == Stock_list.begin())
			{
				map<string, float> stock_daily_return_map = (*it)->get_daily_return_map();

				for (map<string, float>::iterator itr = stock_daily_return_map.begin(); itr != stock_daily_return_map.end(); itr++)
				{
					portfolio_daily_return[itr->first] += portfolio_daily_return[itr->first] + (itr->second)*((*it)->get_share_held()) / total_share_held;// *(((*itr)->get_share_held()) / total_share_held);
				}
			}
			else
			{
				if (((*it)->get_trades()).size() == (((*(it - 1))->get_trades()).size()))
				{
					total_share_held -= (*it)->get_share_held();
					continue;
				}

				int len = ((*it)->get_daily_return_vector()).size();
				Stock* before = *(it - 1);
				int len_1 = before->get_daily_return_vector().size();

				map<string, float> stock_daily_return_map = (*it)->get_daily_return_map();

				for (map<string, float>::iterator itr = next(stock_daily_return_map.begin(),len_1); itr != stock_daily_return_map.end(); itr++)
				{
					portfolio_daily_return[itr->first] += portfolio_daily_return[itr->first] + (itr->second)*((*it)->get_share_held()) / total_share_held;// *(((*itr)->get_share_held()) / total_share_held);
					
				}
			}
		}

	}
	
	map<string,float> get_portfolio_daily_return()
	{
		return portfolio_daily_return;
	}
	void cal_portfolio_return_vector()
	{
		portfolio_return_vector.clear();
		for (map<string, float>::iterator it = portfolio_daily_return.begin(); it != portfolio_daily_return.end(); ++it)
		{
			portfolio_return_vector.push_back(it->second);
		}
	}
	vector<float> get_portfolio_return_vector()
	{
		return portfolio_return_vector;
	}
	

	float portfolio_risk()
	{
		vector<float> ri_rmean2;
		float return_mean = vector_average(portfolio_return_vector);
		for (map<string, float>::iterator it = portfolio_daily_return.begin(); it != portfolio_daily_return.end(); ++it)
		{
			ri_rmean2.push_back((it->second - return_mean)*(it->second - return_mean));
		}
		return sqrt(vector_average(ri_rmean2));
	}
	void cal_portfolio_adjuested_beta()
	{	
		float beta = 0;
		total_share_held_cal();
		vector<float> RiskFreeRate = mySPY.get_RiskFreeRate_vector();
		mySPY.cal_daily_return_vector();
		vector<float> SPY_daily_return = mySPY.get_daily_return_vector();
		std::sort(Stock_list.begin(), Stock_list.end(), Stock::compBySize);
		vector<float> temp_vector;
		//int i = 0;
		float average_risk_free = 0;
		float average_spy = 0;
		for (vector<Stock*>::iterator it = Stock_list.begin(); it != Stock_list.end(); ++it)
		{
			if (it == Stock_list.begin())
			{
				(*it)->cal_daily_return_vector();
				float temp = vector_average((*it)->get_daily_return_vector());
				int len = (*it)->get_daily_return_vector().size();
				average_risk_free = accumulate(RiskFreeRate.end()-len, RiskFreeRate.end(), 0.0 / len) / len;
				average_spy = accumulate(SPY_daily_return.end()-len, SPY_daily_return.end(), 0.0 / len) / len;
				beta+= ((*it)->get_share_held())*(temp- average_risk_free) / (average_spy - average_risk_free);
				
			}
			else
			{
				(*it)->cal_daily_return_vector();
				float temp = vector_average((*it)->get_daily_return_vector());
				if (((*it)->get_trades()).size() == (((*(it - 1))->get_trades()).size()))
				{
					beta += ((*it)->get_share_held())* (temp - average_risk_free) / (average_spy - average_risk_free);
				}
				else
				{
					int len = (*it)->get_daily_return_vector().size();
					average_risk_free = accumulate(RiskFreeRate.end() - len, RiskFreeRate.end(), 0.0 / len) / len;
					average_spy = accumulate(SPY_daily_return.end() - len, SPY_daily_return.end(), 0.0 / len) / len;
					beta += ((*it)->get_share_held())* (temp - average_risk_free) / (average_spy - average_risk_free);//((*it)->get_share_held())*
					
				}
			}
		}
		portfolio_adjuested_beta = beta*0.67/ portfolio_part_return.size() + 0.33;

	}
	float get_portfolio_adjuested_beta()
	{
		return portfolio_adjuested_beta;
	}
	
	float Jensens_Alpha()
	{
		float Jensens_Alpha = 0;
		float portfolio_average = vector_average(portfolio_return_vector);
		float risk_free_average = vector_average(mySPY.get_RiskFreeRate_vector());
		mySPY.cal_daily_return_vector();
		float SPY_average= vector_average(mySPY.get_daily_return_vector());
		Jensens_Alpha = portfolio_average - (risk_free_average + get_portfolio_adjuested_beta()*(SPY_average - risk_free_average));
		
		return (Jensens_Alpha );
		
	}
	float Sharpe_Ratio()
	{
		float sharpe_ratio = 0;
		float portfolio_average=vector_average(portfolio_return_vector);
		float risk_free_average = vector_average(mySPY.get_RiskFreeRate_vector());
		float excess_return = portfolio_average - risk_free_average;
	    sharpe_ratio = excess_return / portfolio_risk();
		cout <<"sharpe ratio" << sharpe_ratio*10 << endl;
		return sharpe_ratio*10;
	}
	float InformationRatio()
	{
		float InformationRatio = 0;
		mySPY.cal_daily_return_vector();
		vector<float> SPY_daily_return = mySPY.get_daily_return_vector();
		float SPY_average = vector_average(SPY_daily_return);
		float portfolio_average = vector_average(portfolio_return_vector);
		float TrackingError = 0;
		vector<float> difference = portfolio_return_vector - SPY_daily_return;
		vector<float> difference_2 = difference * difference;
		TrackingError= sqrt(vector_average(difference_2));
		InformationRatio = vector_average(difference) / TrackingError;
		return InformationRatio*50/4;
	}
	void total_share_held_cal()
	{
		total_share_held = 0;
		for (vector<Stock*>::iterator it = Stock_list.begin(); it != Stock_list.end(); it++)
			total_share_held += (*it)->get_share_held();
	}
	float High52WeekRatio()
	{
		total_share_held_cal();
		float weighted_High52WeekRatio = 0;
		for (vector<Stock*>::iterator it = Stock_list.begin(); it != Stock_list.end(); it++)
		{
			float average_adjusted_price = vector_average((*it)->get_close_price_vector());
			float hightWeekRatio = average_adjusted_price / ((*it)->Get_fundamentals())->get_high_52weeks();
			weighted_High52WeekRatio += hightWeekRatio * ((*it)->get_share_held() );
		}
		cout << "finish weighted_High52WeekRatio cal" << weighted_High52WeekRatio / total_share_held << endl;
		return weighted_High52WeekRatio / total_share_held;
	}
	float weightedMA50MA200_ratio()
	{
		total_share_held_cal();
		float weightedMA50MA200_ratio = 0;
		for (vector<Stock*>::iterator it = Stock_list.begin(); it != Stock_list.end(); it++)
		{
			weightedMA50MA200_ratio += ((*it)->get_share_held())*(((*it)->Get_fundamentals())->get_MA_50days()) / (((*it)->Get_fundamentals())->get_MA_200days());// / total_share_held);
		}
		
		return (weightedMA50MA200_ratio/ total_share_held-1)*10;

	}
	float weighted_dividend_yield()
	{
		total_share_held_cal();
		float weightedDividendYield = 0;
		for (vector<Stock*>::iterator it = Stock_list.begin(); it != Stock_list.end(); it++)
		{
			 weightedDividendYield += (((*it)->Get_fundamentals())->get_DividendYield()- 0.03)*((*it)->get_share_held() )/ 0.03;
		}
	    cout << " weightedDividendYield cal " << weightedDividendYield  / total_share_held << endl;
		return weightedDividendYield / total_share_held;
	}
	float weighted_ROE()
	{
		total_share_held_cal();
		float weightedROE = 0;
		for (vector<Stock*>::iterator it = Stock_list.begin(); it != Stock_list.end(); it++)
		{
			weightedROE += (((*it)->Get_fundamentals())->get_ReturnOnEquityTTM())*((*it)->get_share_held());
			cout <<(*it)->get_symbol()<< " weightedROE cal " << (((*it)->Get_fundamentals())->get_ReturnOnEquityTTM()) << endl;
		}
		cout << " weightedROE cal " << weightedROE*0.1 / total_share_held << endl;
		return weightedROE*0.1 / total_share_held;
	}
	float weighted_profit()
	{
		total_share_held_cal();
		float weightedProfit = 0;
		for (vector<Stock*>::iterator it = Stock_list.begin(); it != Stock_list.end(); it++)
		{
			weightedProfit += (((*it)->Get_fundamentals())->get_ProfitMargin())*((*it)->get_share_held());
			//cout << " weightedDividendYield cal " << ((*it)->Get_fundamentals())->get_DividendYield() << endl;
		}
		cout << " weighted_profit cal " << weightedProfit / total_share_held << endl;
		return weightedProfit / total_share_held/ sp500_average_profit;
	}
	float weighted_ROA()
	{
		total_share_held_cal();
		float weightedROA = 0;
		for (vector<Stock*>::iterator it = Stock_list.begin(); it != Stock_list.end(); it++)
		{
			weightedROA += (((*it)->Get_fundamentals())->get_ReturnOnAssetsTTM())*((*it)->get_share_held());
		}

		return weightedROA*6/ total_share_held;
	}
	float weighted_PERatio()
	{
		float weighted_PERatio = 0;
		for (vector<Stock*>::iterator it = Stock_list.begin(); it != Stock_list.end(); it++)
		{
			float PERatio = ((*it)->Get_fundamentals())->get_PERatio();

			if (PERatio < 7)
			{
				weighted_PERatio += 0 * ((*it)->get_share_held());
			}
			else if (PERatio > sp500_average_PE)
			{
				weighted_PERatio += 0 * ((*it)->get_share_held());
			}
			else 
			{
				weighted_PERatio += 1 * ((*it)->get_share_held());
			}

		}
		cout << "finish weighted_PERatio cal" << weighted_PERatio / total_share_held << endl;
		return weighted_PERatio / total_share_held;
	}
	float weighted_MAROC()
	{
		total_share_held_cal();
		
		float weighted_MAROC = 0;
		for (vector<Stock*>::iterator it = Stock_list.begin(); it != Stock_list.end(); it++)
		{
			cout << (*it)->get_symbol() << endl;
		  float MAROC = (*it)->MAROC();
		  if(MAROC<1)
             weighted_MAROC = 0.2* ((*it)->get_share_held());
		  else if(MAROC < 1.02)
			 weighted_MAROC = 0.4* ((*it)->get_share_held());
		  else
			  weighted_MAROC = 0.6* ((*it)->get_share_held());

		}
		cout << "finish weighted ROC cal" << weighted_MAROC / total_share_held << endl;
		return weighted_MAROC / total_share_held;
	}
	
	void cal_fitness()
	{
		fitness = 0;
		cal_portfolio_return();
		cal_portfolio_return_vector();
		fitness = InformationRatio()+ weightedMA50MA200_ratio() + weighted_ROA();
		cout <<    "finish fitness cal  " << fitness << endl;
	}
	float get_fitness()
	{
		return fitness;

	}
	static bool compByFitness(Portfolio* x, Portfolio* y)
	{
		return x->get_fitness() < y->get_fitness();
	}


};
#endif

