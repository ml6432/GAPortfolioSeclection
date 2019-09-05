#include <iostream>
#include <string>
#include <map>
#include <fstream>
#include <vector>
#include <stdio.h>

#include "curl_easy.h"
#include "curl_form.h"
#include "curl_ios.h"
#include "curl_exception.h"

#include "json/json.h"
#include <sqlite3.h>

#include "Database.h"
#include "Stock.h"
#include "GenA.h"
#include "Portfolio.h"
#include "Utility.h"
#include "AddMissing.h"
#include "BackTest.h"
#include <cstdlib>

#include <time.h>
#include <ctime>
#include <iomanip>
#include <iostream>
#include <sstream>

#include <stdio.h>
#include <sstream>
#define MUTATION_RATE             0.03
#define POP_SIZE                  100    
#define CHROMO_LENGTH             10
#define MAX_ALLOWABLE_GENERATIONS   2
#pragma warning(disable : 4996) 
using namespace std;

int main(void)
{
	//json list of maps outer, inner maps    sqlite, call by reference, the datatype is pointer to  sqlite, when open DB, DB file load in the memory, 
	//pointer of the pointer, & pass the address, real double, int integer, char character
	// const call by reference , rand()%100, global pointer
	//  Input stream class to operate on strings  get_time(valid pointer to the std::tm object,format)   mktime Structure holding a calendar date and time broken down into its components.
	//// 使用random_device生成seed then g UniformRandomBitGenerator
	vector<string> best_Portfolio_ticker;
	vector<Stock*> best_Portfolio_Stock;
	Stock* mySPY = nullptr;
	Portfolio* best_Portfolio=nullptr;
	string api_token = "5ba84ea974ab42.45160048";
	vector <string> SP_ticker;
	SP_ticker.push_back("SPY");
	const char * stockDB_name = "Stocks.db";
	int count = 0;
	sqlite3 * stockDB = NULL;
	map <string, Stock*> sp500_Stock;
	vector<float> weight_list;
	vector<float> share_held_list;
	vector<float> RiskFreeRate_vector;
	vector<float> dividend_vector;
	vector<float> PE_vector;
	vector<float> profit_vector;
	vector<float> ROE_vector;
	vector<float> ROA_vector;

	float sp500_dividend = 0;
	float sp500_PE = 0;
	float sp500_profit = 0;
	float sp500_ROE = 0;
	float sp500_ROA = 0;
	weight_list.push_back(1);
	share_held_list.push_back(3186981124);
	map<string, vector<string>> Communication_Services, Consumer_Discretionary, Consumer_Staples, Energy, Financials, Health_Care, Industrials,
		Information_Technology, Materials, Real_Estate, Utilities, Unassigned;
	vector<map<string, vector<string>>> sector_category{ Communication_Services,Consumer_Discretionary,Consumer_Staples, Energy,
	Financials,Health_Care,Industrials,Information_Technology,Information_Technology,Materials,
	Real_Estate,Utilities, Unassigned };

	if (OpenDatabase(stockDB_name, stockDB) == -1)
		return -1;
	map<string, float> RiskFreeRate;

	string sp500_select_table = "SELECT * FROM New_SP500;";
	//！！test
	DisplayNewSP500Ticker(sp500_select_table.c_str(), stockDB, SP_ticker, weight_list, share_held_list, sector_category);

	CloseDatabase(stockDB);
	bool a = true;
	int attempt = 0;

	while (a)
	{
		string str;
		cout << endl;
		cout << "Pick a choice from the list:" << endl << endl
			<< "1.Retrieve SP500 ticker and save to database" << endl
			<< "2.Retrieve new SP500 ticker and save to database" << endl
			<< "3.Retrieve trade data and save to database" << endl
			<< "4.Retrieve fundamental data and save to database" << endl
			<< "5.Retrieve risk free rate and save to database" << endl
			<< "6.Get stock data from database" << endl
			<< "7.Portfolio Selection by genetic algorithm" << endl
			<< "8.Back test"<<endl
			<< "9.Probation test" << endl
			<< "10.Exit your program" << endl << endl;
		cin >> str;

		int answer;
		answer = std::stoi(str);
		switch (answer)
		{
		case 1:
		{
			cout << endl << "Retrieve SP500 ticker and save to database..." << endl << endl;

			if (OpenDatabase(stockDB_name, stockDB) == -1)
				return -1;

			std::string sp500_drop_table = "DROP TABLE IF EXISTS SP500;";
			if (DropTable(sp500_drop_table.c_str(), stockDB) == -1)
				return -1;

			string sp500_create_table = "CREATE TABLE SP500 (id INT PRIMARY KEY NOT NULL, symbol CHAR(20) NOT NULL, name CHAR(20) NOT NULL, sector CHAR(20) NOT NULL);";

			if (CreateTable(sp500_create_table.c_str(), stockDB) == -1)
				return -1;

			string sp500_data_request = "https://pkgstore.datahub.io/core/s-and-p-500-companies/constituents_json/data/64dd3e9582b936b0352fdd826ecd3c95/constituents_json.json";
			//string sp500_data_request = "https://datahub.io/core/s-and-p-500-companies/r/0.html";
			Json::Value sp500_root;   // will contains the root value after parsing.
			if (RetrieveMarketData(sp500_data_request, sp500_root) == -1)
				return -1;
			if (PopulateSP500Table(sp500_root, stockDB, SP_ticker) == -1)
				return -1;
			cout << sp500_root << endl;
			string sp500_select_table = "SELECT * FROM SP500;";
			if (DisplayTable(sp500_select_table.c_str(), stockDB) == -1)
				return -1;
			CloseDatabase(stockDB);

			cout << "Retrieve successfully! Thank you for your patience!" << endl << endl;
			attempt++;
			break;
		}
		case 2:
		{
			cout << endl << "Retrieve new SP500 ticker and save to database..." << endl << endl;

			if (OpenDatabase(stockDB_name, stockDB) == -1)
				return -1;

			std::string New_sp500_drop_table = "DROP TABLE IF EXISTS New_SP500;";
			if (DropTable(New_sp500_drop_table.c_str(), stockDB) == -1)
				return -1;

			string New_sp500_create_table = "CREATE TABLE New_SP500 (id INT PRIMARY KEY NOT NULL, symbol CHAR(20) NOT NULL, name CHAR(20) NOT NULL, sector CHAR(20) NOT NULL,weight REAL NOT NULL, share_held REAL NOT NULL);";

			if (CreateTable(New_sp500_create_table.c_str(), stockDB) == -1)
				return -1;
			if (GetTickerData(stockDB) == -1)
				return -1;

			string New_sp500_select_table = "SELECT * FROM New_SP500;";
			if (DisplayTable(New_sp500_select_table.c_str(), stockDB) == -1)
				return -1;
			CloseDatabase(stockDB);

			cout << "Retrieve successfully! Thank you for your patience!" << endl << endl;
			attempt++;
			break;
		}
		case 3:
		{

			cout << endl << "Retrieve trade data and save to database" << endl << endl;
			if (OpenDatabase(stockDB_name, stockDB) == -1)
				return -1;
			string stock_url_common = "https://eodhistoricaldata.com/api/eod/";
			string api_token = "5ba84ea974ab42.45160048";
			string stock_start_date = "2008-01-01";
			string stock_end_date = "2019-08-01";
			for (vector<string> ::iterator it = SP_ticker.begin(); it != SP_ticker.end(); it++)
			{
				cout << *it << endl;
				string stockDB_symbol = *it;
				if (*it == "BRK_B")
					*it = "BRK-B";
				if (*it == "BF_B")
					*it = "BF-B";
				if (*it == "LUK")
					continue;
				if (*it == "MON")
					continue;
				
				//https://eodhistoricaldata.com/api/eod/BRK.B.US?from=2008-01-01&to=2019-06-06&api_token=5ba84ea974ab42.45160048&period=d&fmt=json
				string stockDB_data_request = stock_url_common + *it + ".US?" +
					"from=" + stock_start_date + "&to=" + stock_end_date + "&api_token=" + api_token + "&period=d&fmt=json";

				if (stockDB_symbol == "ALL")
					stockDB_symbol = "Stock_ALL";
				int pos = stockDB_symbol.find('-');
				if (pos != std::string::npos)
					stockDB_symbol.replace(pos, 1, 1, '_');


				std::string stockDB_drop_table = "DROP TABLE IF EXISTS " + stockDB_symbol + ";";
				if (DropTable(stockDB_drop_table.c_str(), stockDB) == -1)
					return -1;

				string stockDB_create_table = "CREATE TABLE " + stockDB_symbol
					+ "(id INT PRIMARY KEY NOT NULL,"
					+ "symbol CHAR(20) NOT NULL,"
					+ "date CHAR(20) NOT NULL,"
					+ "open REAL NOT NULL,"
					+ "high REAL NOT NULL,"
					+ "low REAL NOT NULL,"
					+ "close REAL NOT NULL,"
					+ "adjusted_close REAL NOT NULL,"
					+ "volume REAL NOT NULL,"
					+ "daily_return REAL NOT NULL);";

				if (CreateTable(stockDB_create_table.c_str(), stockDB) == -1)
					return -1;

				Json::Value stockDB_root;   // will contains the root value after parsing.
				if (RetrieveMarketData(stockDB_data_request, stockDB_root) == -1)
					return -1;
				if (PopulateStockTable(stockDB_root, stockDB_symbol, stockDB, 0) == -1)
					return -1;

				string stockDB_select_table = "SELECT * FROM " + stockDB_symbol + ";";
				if (DisplayTable(stockDB_select_table.c_str(), stockDB) == -1)
					return -1;

			}
			CloseDatabase(stockDB);

			break;
		}
		case 4:
		{
			cout << endl << "Retrieve fundamental data and save to database..." << endl << endl;
			if (OpenDatabase(stockDB_name, stockDB) == -1)
				return -1;

			//stock fundamental data
			//https://eodhistoricaldata.com/api/fundamentals/AAPL.US?api_token=5ba84ea974ab42.45160048&fmt=json
			string stock_Fundamental_url_common = "https://eodhistoricaldata.com/api/fundamentals/";
			std::string stock_fundamental_drop_table = "DROP TABLE IF EXISTS stock_fundamental;";
			if (DropTable(stock_fundamental_drop_table.c_str(), stockDB) == -1)
				return -1;
			string stock_fundamental_create_table = "CREATE TABLE stock_fundamental (id INT PRIMARY KEY NOT NULL, symbol CHAR(20) NOT NULL, High_52Week REAL NOT NULL,Low_52Week REAL NOT NULL,MA_50Day REAL NOT NULL,MA_200Day REAL NOT NULL,DividendYield REAL NOT NULL,PERatio REAL NOT NULL,Beta REAL NOT NULL,ProfitMargin REAL NOT NULL,ReturnOnAssetsTTM REAL NOT NULL,ReturnOnEquityTTM REAL NOT NULL); ";

			if (CreateTable(stock_fundamental_create_table.c_str(), stockDB) == -1)
				return -1;


			for (vector<string> ::iterator it = SP_ticker.begin(); it != SP_ticker.end(); it++)
			{
				
				if (*it == "BRK_B")
					*it = "BRK-B";
				if (*it == "BF_B")
					*it = "BF-B";
				if (*it == "LUK")
					continue;
				if (*it == "MON")
					continue;

				Stock myStock(*it);

				string stockDB_symbol = *it;

				if (stockDB_symbol == "ALL")
					stockDB_symbol = "Stock_ALL";
				


				// creat database for fundamental data
				string FundamentalDB_data_request = stock_Fundamental_url_common + *it + ".US?" +
					"&api_token=" + api_token;
				Json::Value stockFD_root;   // will contains the root value after parsing.
				if (RetrieveMarketData(FundamentalDB_data_request, stockFD_root) == -1)
					return -1;
				cout << count << endl;

				if (PopulateFundamentalDataTable(stockFD_root, *it, stockDB, count) == -1)
					return -1;
				cout << stockFD_root << endl;
				string stock_fundamental_select_table = "SELECT * FROM stock_fundamental;";
				if (DisplayTable(stock_fundamental_select_table.c_str(), stockDB) == -1)
					return -1;
				count++;

			}
			CloseDatabase(stockDB);

			cout << "Retrieve successfully! Thank you for your patience!" << endl << endl;
			attempt++;


			break;
		}
		case 5:
		{
			cout << endl << "Retrieve risk free rate and save to database..." << endl << endl;

			if (OpenDatabase(stockDB_name, stockDB) == -1)
				return -1;

			std::string RiskFreeRate_drop_table = "DROP TABLE IF EXISTS RiskFreeRate;";
			if (DropTable(RiskFreeRate_drop_table.c_str(), stockDB) == -1)
				return -1;
			string RiskFreeRate_create_table = "CREATE TABLE RiskFreeRate (id INT PRIMARY KEY NOT NULL, rf CHAR(20) NOT NULL, date CHAR(20) NOT NULL, open REAL NOT NULL, high REAL NOT NULL, low REAL NOT NULL, close REAL NOT NULL, adjusted_close REAL NOT NULL);";

			if (CreateTable(RiskFreeRate_create_table.c_str(), stockDB) == -1)
				return -1;

			string RiskFreeRate_data_request = "https://eodhistoricaldata.com/api/eod/TNX.INDX?from=2008-01-01&to=2019-06-06&api_token=5ba84ea974ab42.45160048&period=d&fmt=json";
			Json::Value RiskFreeRate_root;   // will contains the root value after parsing.
			if (RetrieveMarketData(RiskFreeRate_data_request, RiskFreeRate_root) == -1)
				return -1;
			if (PopulateRiskFreeRateTable(RiskFreeRate_root, stockDB) == -1)
				return -1;
			cout << RiskFreeRate_root << endl;
			string RiskFreeRate_select_table = "SELECT * FROM RiskFreeRate;";
			if (DisplayTable(RiskFreeRate_select_table.c_str(), stockDB) == -1)
				return -1;
			CloseDatabase(stockDB);

			cout << "Retrieve successfully! Thank you for your patience!" << endl << endl;

			break;
		}
		case 6:
		{
			if (OpenDatabase(stockDB_name, stockDB) == -1)
				return -1;
			for (vector<string> ::iterator it = SP_ticker.begin(); it != SP_ticker.end(); it++)
			{

				//cout << *it << endl;
				if (*it == "BRK.B")
					*it = "BRKB";
				if (*it == "BF.B")
					*it = "BF_B";
				if (*it == "ALL")
					*it = "Stock_ALL";
				int position = std::distance(SP_ticker.begin(), it);
				Stock *myStock = new Stock(*it);
				myStock->addWeight(weight_list[position]);
				myStock->addShareHeld(share_held_list[position]);
				string stockDB_symbol = *it;
				string stockDB_select_table = "SELECT * FROM " + stockDB_symbol + " WHERE strftime(\'%Y-%m-%d\', date) between \"2008-01-02\" and \"2018-12-31\";";
				if (DisplayTradeData(stockDB_select_table.c_str(), stockDB, myStock) == -1)
					return -1;
				string stock_fundamental_select_table = "SELECT * FROM stock_fundamental;";
				int num = std::distance(SP_ticker.begin(), it);
		
				if (DisplayFundamentalData(stock_fundamental_select_table.c_str(), stockDB, myStock, num, dividend_vector,  PE_vector, profit_vector,ROA_vector,ROE_vector) == -1)
					return -1;
				string RiskFreeRate_select_table = "SELECT * FROM RiskFreeRate WHERE strftime(\'%Y-%m-%d\', date) between \"2008-01-02\" and \"2018-12-31\";";
				if (DisplayRiskFreeRateData(RiskFreeRate_select_table.c_str(), stockDB, RiskFreeRate, RiskFreeRate_vector) == -1)
					return -1;
				myStock->addRiskFreeRate(RiskFreeRate_vector);
				sp500_Stock[*it] = myStock;
			    

			}
			sp500_dividend = vector_average(dividend_vector);
			sp500_PE = vector_average(PE_vector);
			sp500_profit = vector_average(profit_vector);
			sp500_ROA = vector_average(ROA_vector);
			sp500_ROE = vector_average(ROE_vector);
		

			break;
		}
		case 7:
		{
			//fill_missing_return(sp500_Stock);
			cout << "Portfolio selection" << endl;
			Stock* mySPY = sp500_Stock["SPY"];
			//seed the random number generator
			srand((int)time(NULL));
			//just loop endlessly until user gets bored :0)
			while (true)
			{
				//storage for our population of chromosomes.
				vector<Portfolio*> Population;

				    for (int i = 0; i < POP_SIZE; i++)
				{

					vector<string>portfolio_ticker_list(0);
					vector<Stock*>portfolio_stock_list(0);
					//GetSectorStockTicker(portfolio_ticker_list, sector_category);
					GetRandomStockTicker(portfolio_ticker_list, SP_ticker);
					
					for (vector<string>::iterator it = portfolio_ticker_list.begin(); it != portfolio_ticker_list.end(); ++it)
					{
						portfolio_stock_list.push_back(sp500_Stock[*it]);
					}
					Portfolio *myPortfolio = new Portfolio(portfolio_stock_list);
					myPortfolio->addPortfolio_para(*mySPY, SP_ticker, portfolio_ticker_list, sp500_dividend,sp500_PE, sp500_profit, sp500_ROA, sp500_ROE);
					Population.push_back(myPortfolio);
				}
					
					int GenerationsRequiredToFindASolution = 0;

					//we will set this flag if a solution has been found
					bool bFound = false;

					//enter the main GA loop
					while (!bFound)
					{
						cout << "===============================" << endl;
						cout << "The " << GenerationsRequiredToFindASolution << " generation" << endl;
						cout << "===============================" << endl;

						// test and update the fitness of every chromosome in the 
						// population
						
					    for (int i = 0; i < POP_SIZE; i++)
						{

								Population[i]->cal_fitness();
						}		   
						std::sort(Population.begin(), Population.end(), Portfolio::compByFitness);
						vector<Portfolio*> temp;
						vector<Portfolio*> new_pop;
						vector<Portfolio> no_change;
						
						New_Population(Population);
						std::random_device rd;
						std::mt19937 g(rd());// get random number uniformly and then based on this value generate the mt19937 is method to generate random value
						for (vector<Portfolio*>::iterator it = Population.begin(); it != Population.begin() + Population.size()*0.95; it++)
						{
							
							temp.push_back(*it);
							
						}
						for (vector<Portfolio*>::iterator it = Population.begin() + Population.size()*0.95; it != Population.end(); it++)
						{
			
							no_change.push_back(*(*it));
							
						}
						std::shuffle(temp.begin(), temp.end(), g);
						
						for (vector<Portfolio*>::iterator it = temp.begin()+1; it < temp.end()-1; it = it + 2)
						{
							Portfolio* offspring1 = *it;
							Portfolio* offspring2 = *(it-1);
							Crossover(*offspring1, *offspring2);
							Mutate(*offspring1, sp500_Stock);
							Mutate(*offspring2, sp500_Stock);

							new_pop.push_back(offspring1);
							new_pop.push_back(offspring2);

						}
				
						for (int i=0; i!= new_pop.size();i++)
							
						{
							Population[i] = new_pop[i];
						}
						for (int i = 0; i != no_change.size(); i++)

						{
							vector<string> ticker_list=no_change[i].get_portfolio_ticker_list(); 
							Portfolio *myPortfolio = new Portfolio(no_change[i].get_Stock_list());
							myPortfolio->addPortfolio_para(*mySPY, SP_ticker, ticker_list, sp500_dividend,sp500_PE, sp500_profit, sp500_ROA, sp500_ROE);
							Population[Population.size() - 1 - i] = myPortfolio;//&no_change[i];
						}

						cout << "finished children generation================================" << endl;
						++GenerationsRequiredToFindASolution;
						if (GenerationsRequiredToFindASolution > MAX_ALLOWABLE_GENERATIONS)
						{
							cout << Population[99]->get_portfolio_ticker_list();
							best_Portfolio = Population[99];
							bFound = true;
						}

					}
					break;
					cout << "\n\n\n";

			}//end while
			break;
		}
		case 8:
		{
			
			mySPY = sp500_Stock["SPY"];
			mySPY->clear_trade();
			string stockDB_symbol = "SPY";
			string stockDB_select_table = "SELECT * FROM " + stockDB_symbol + " WHERE strftime(\'%Y-%m-%d\', date) between \"2019-01-01\" and \"2019-08-01\";";
			if (DisplayTradeData(stockDB_select_table.c_str(), stockDB, mySPY) == -1)
				return -1;
			//vector<string> temp_stock_ticker{ "CMG","DTE","ETR","KLAC","NCLH","NFLX","PNW" , "RE", "RMD","ZTS" };
			//vector<string> temp_stock_ticker{ "AZO", "WELL", "CCL","DLR","KO","NEM","PVH","PXD","SJM" }; // reGA
			vector<string> temp_stock_ticker{ "ALLE", "AZO", "DVA","HLT","MKC","PH","SYY","TSN","TWTR","WHR" };
			//vector<string> temp_stock_ticker{ "ANET", "CNP", "INFO","INTC","IT","IVZ","PH","SCHW","SEE","WFC" };//"WFC" change fitness function;
			//vector<string> temp_stock_ticker{ "BEN", "CTXS", "CXO", "DD", "DFS", "F", "FOX", "PNW", "RL", "TRIP" };
			//vector<string> temp_stock_ticker{ "COO", "EMR", "HAS", "HFC", "LMT", "MTD", "PM", "PSX", "T", "XEL" };//test// COF, DVA, EXPE, PGR, PKG, PM, RL, T, CMS, XEL,// EMR, HAS, HFC, LMT, MTD, PM, PSX, T, XEL,///ANTM, CB, CNP, EMR, HFC, FCX, MTD, PM, PSX, T,
			//CTXS, CXO, DD, DFS, F, FOX, RL, TRIP, UAA, UAL, jensens alpha;CTXS, CXO, DD, ECL, F, FOX, RL, TRIP, UAA, ZION,//166 BEN, CTXS, CXO, DD, DFS, F, FOX, PNW, RL, TRIP
			vector<Stock*> temp_stock;//Stock* mySPY = sp500_Stock["SPY"];

			for (vector<string>::iterator it = temp_stock_ticker.begin(); it != temp_stock_ticker.end(); ++it)
			{
				temp_stock.push_back(sp500_Stock[*it]);
			}
			best_Portfolio = new Portfolio(temp_stock);
			//myPortfolio(portfolio_stock_list);
			best_Portfolio->addPortfolio_para(*mySPY, SP_ticker, temp_stock_ticker, sp500_dividend,2,3,4,5);







			best_Portfolio_ticker = best_Portfolio->get_portfolio_ticker_list();
			best_Portfolio_Stock = best_Portfolio->get_Stock_list();
			vector<Stock*> new_best_Portfolio_Stock;
			//string sp500_select_table = "SELECT * FROM SP500;";
					//DisplaySP500Ticker(sp500_select_table.c_str(), stockDB, ticker_list);
			if (OpenDatabase(stockDB_name, stockDB) == -1)
				return -1;

			for (vector<Stock*> ::iterator it = best_Portfolio_Stock.begin(); it != best_Portfolio_Stock.end(); it++)//SP_ticker.end()
			{
				cout << (*it)->get_symbol() << endl;
				string stockDB_symbol = (*it)->get_symbol();
				cout << (*it)->get_symbol() << endl;
				(*it)->clear_trade();
				string stockDB_select_table = "SELECT * FROM " + stockDB_symbol + " WHERE strftime(\'%Y-%m-%d\', date) between \"2019-01-02\" and \"2019-08-01\";";
				if (DisplayTradeData(stockDB_select_table.c_str(), stockDB, *it) == -1)
					return -1;

				new_best_Portfolio_Stock.push_back(*it);//sp500_Stock.push_back(myStock);

			}
			best_Portfolio->update_list(new_best_Portfolio_Stock, best_Portfolio_ticker);


			Stock* mySPY = sp500_Stock["SPY"];
			mySPY->clear_trade();
			stockDB_symbol = "SPY";
			stockDB_select_table = "SELECT * FROM " + stockDB_symbol + " WHERE strftime(\'%Y-%m-%d\', date) between \"2019-01-02\" and \"2019-08-01\";";
			if (DisplayTradeData(stockDB_select_table.c_str(), stockDB, mySPY) == -1)
				return -1;

			best_Portfolio->add_mySPY(*mySPY);
			////SPY haven't change!!!
			int month = 1;
			float excess_return = 1;
			int cash = 1000000;

			excess_return = Test(best_Portfolio);
			//cout << month << " " << excess_return << endl;

			//cout << month << endl;
			break;
		}
		case 9:
		{
		
			vector<Stock*> new_best_Portfolio_Stock;
			if (OpenDatabase(stockDB_name, stockDB) == -1)
				return -1;

			for (vector<Stock*> ::iterator it = best_Portfolio_Stock.begin(); it != best_Portfolio_Stock.end(); it++)//SP_ticker.end()
			{
				string stockDB_symbol = (*it)->get_symbol();
				cout << (*it)->get_symbol() << endl;
				(*it)->clear_trade();
				string stockDB_select_table = "SELECT * FROM " + stockDB_symbol + " WHERE strftime(\'%Y-%m-%d\', date) between \"2019-07-01\" and \"2019-07-30\";";
				if (DisplayTradeData(stockDB_select_table.c_str(), stockDB, *it) == -1)
					return -1;

				new_best_Portfolio_Stock.push_back(*it);


			}
			best_Portfolio->update_list(new_best_Portfolio_Stock, best_Portfolio_ticker);


			Stock* mySPY = sp500_Stock["SPY"];
			mySPY->clear_trade();
			string stockDB_symbol = "SPY";
			string stockDB_select_table = "SELECT * FROM " + stockDB_symbol + " WHERE strftime(\'%Y-%m-%d\', date) between \"2019-07-01\" and \"2019-07-30\";";
			if (DisplayTradeData(stockDB_select_table.c_str(), stockDB, mySPY) == -1)
				return -1;

			best_Portfolio->add_mySPY(*mySPY);
			////SPY haven't change!!!
			int month = 7;
			float excess_return = 1;
			int cash = 1000000;
			excess_return = Test(best_Portfolio);	
			break;

		}
		case 10:
		{
			for (int i = 0; i < best_Portfolio_Stock.size(); i++)
			{
				delete (best_Portfolio_Stock[i]);
			}
			best_Portfolio_Stock.clear();
			delete mySPY;
			delete best_Portfolio;
			a = false;
			cout << endl << "successfully exit!" << endl << endl;
			exit(0);
			break;
		default:
			cout << endl << "Bad choice! Please try again: " << endl;

		}
		}
		}

		system("pause");
		return 0;
	}



