
#pragma once
#ifndef Database_h
#define Database_h

#include "Stock.h"
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
#include <cstdlib>
using namespace std;

static size_t WriteCallback(void *contents, size_t size, size_t nmemb, void *userp)
{
	((std::string*)userp)->append((char*)contents, size * nmemb);
	return size * nmemb;
}


int RetrieveMarketData(string url_request, Json::Value & root)
{
	std::string readBuffer;

	//global initiliation of curl before calling a function
	curl_global_init(CURL_GLOBAL_ALL);

	//creating session handle
	CURL * myHandle;

	// Wel store the result of CURL’s webpage retrieval, for simple error checking.
	CURLcode result;

	// notice the lack of major error-checking, for brevity
	myHandle = curl_easy_init();

	curl_easy_setopt(myHandle, CURLOPT_URL, url_request.c_str());
	//curl_easy_setopt(myHandle, CURLOPT_URL, "https://eodhistoricaldata.com/api/eod/AAPL.US?from=2018-01-05&to=2019-02-10&api_token=5ba84ea974ab42.45160048&period=d&fmt=json");

	//adding a user agent
	curl_easy_setopt(myHandle, CURLOPT_USERAGENT, "Mozilla/5.0 (Windows; U; Windows NT 6.1; rv:2.2) Gecko/20110201");
	curl_easy_setopt(myHandle, CURLOPT_SSL_VERIFYPEER, 0);
	curl_easy_setopt(myHandle, CURLOPT_SSL_VERIFYHOST, 0);
	curl_easy_setopt(myHandle, CURLOPT_VERBOSE, 1);

	// send all data to this function  
	curl_easy_setopt(myHandle, CURLOPT_WRITEFUNCTION, WriteCallback);

	// we pass our 'chunk' struct to the callback function */
	curl_easy_setopt(myHandle, CURLOPT_WRITEDATA, &readBuffer);

	//perform a blocking file transfer
	result = curl_easy_perform(myHandle);

	// check for errors 
	if (result != CURLE_OK) {
		fprintf(stderr, "curl_easy_perform() failed: %s\n",
			curl_easy_strerror(result));
	}
	else {
		//cout << readBuffer << endl;
		//
		// Now, our chunk.memory points to a memory block that is chunk.size
		// bytes big and contains the remote file.
		//
		// Do something nice with it!
		//

		// https://github.com/open-source-parsers/jsoncpp
		// Using JsonCpp in your project
		// Amalgamated source
		// https ://github.com/open-source-parsers/jsoncpp/wiki/Amalgamated

		//json parsing

		Json::CharReaderBuilder builder;
		Json::CharReader * reader = builder.newCharReader();
		string errors;

		bool parsingSuccessful = reader->parse(readBuffer.c_str(), readBuffer.c_str() + readBuffer.size(), &root, &errors);
		if (not parsingSuccessful)
		{
			// Report failures and their locations
			// in the document.
			cout << "Failed to parse JSON" << std::endl
				<< readBuffer
				<< errors << endl;
			system("pause");
			return -1;
		}
		std::cout << "\nSucess parsing json\n" << root << endl;

	}

	//End a libcurl easy handle.This function must be the last function to call for an easy session
	curl_easy_cleanup(myHandle);
	return 0;
}

int RetrieveFundamentalData(string url_request)
{
	std::string readBuffer;

	//global initiliation of curl before calling a function
	curl_global_init(CURL_GLOBAL_ALL);

	//creating session handle
	CURL * myHandle;

	// We’ll store the result of CURL’s webpage retrieval, for simple error checking.
	CURLcode result;

	// notice the lack of major error-checking, for brevity
	myHandle = curl_easy_init();

	curl_easy_setopt(myHandle, CURLOPT_URL, url_request.c_str());
	//curl_easy_setopt(myHandle, CURLOPT_URL, "https://eodhistoricaldata.com/api/eod/AAPL.US?from=2018-01-05&to=2019-02-10&api_token=5ba84ea974ab42.45160048&period=d&fmt=json");

	//adding a user agent
	curl_easy_setopt(myHandle, CURLOPT_USERAGENT, "Mozilla/5.0 (Windows; U; Windows NT 6.1; rv:2.2) Gecko/20110201");
	curl_easy_setopt(myHandle, CURLOPT_SSL_VERIFYPEER, 0);
	curl_easy_setopt(myHandle, CURLOPT_SSL_VERIFYHOST, 0);
	curl_easy_setopt(myHandle, CURLOPT_VERBOSE, 1);

	// send all data to this function  
	curl_easy_setopt(myHandle, CURLOPT_WRITEFUNCTION, WriteCallback);

	// we pass our 'chunk' struct to the callback function */
	curl_easy_setopt(myHandle, CURLOPT_WRITEDATA, &readBuffer);

	//perform a blocking file transfer
	result = curl_easy_perform(myHandle);

	// check for errors 
	if (result != CURLE_OK) {
		fprintf(stderr, "curl_easy_perform() failed: %s\n",
			curl_easy_strerror(result));
	}
	else {
		cout << readBuffer << endl;
		//
		// Now, our chunk.memory points to a memory block that is chunk.size
		// bytes big and contains the remote file.
		//
		// Do something nice with it!
		//

		// https://github.com/open-source-parsers/jsoncpp
		// Using JsonCpp in your project
		// Amalgamated source
		// https ://github.com/open-source-parsers/jsoncpp/wiki/Amalgamated

		//json parsing
	}

	//End a libcurl easy handle.This function must be the last function to call for an easy session
	curl_easy_cleanup(myHandle);
	return 0;
}
int OpenDatabase(const char * name, sqlite3 * & db)
{
	int rc = 0;
	//char *error = NULL;
	// Open Database
	cout << "Opening database: " << name << endl;
	rc = sqlite3_open("Stocks.db", &db);
	if (rc)
	{
		cerr << "Error opening SQLite3 database: " << sqlite3_errmsg(db) << endl;
		sqlite3_close(db);
		system("pause");
		return -1;
	}
	cout << "Opened database: " << name << endl;
	return 0;

}

void CloseDatabase(sqlite3 *db)
{
	cout << "Closing a database ..." << endl;
	sqlite3_close(db);
	cout << "Closed a database" << endl << endl;
}

int DropTable(const char * sql_drop_table, sqlite3 *db)
{
	// Drop the table if exists
	if (sqlite3_exec(db, sql_drop_table, 0, 0, 0) != SQLITE_OK) { // or == -- same effect
		std::cout << "SQLite can't drop sessions table" << std::endl;
		sqlite3_close(db);
		system("pause");
		return -1;
	}
	return 0;
}

int CreateTable(const char *sql_create_table, sqlite3 *db)
{
	int rc = 0;
	char *error = NULL;
	// Create the table
	cout << "Creating a table..." << endl;
	rc = sqlite3_exec(db, sql_create_table, NULL, NULL, &error);
	if (rc)
	{
		cerr << "Error executing SQLite3 statement: " << sqlite3_errmsg(db) << endl << endl;
		sqlite3_free(error);
		system("pause");
		return -1;
	}
	cout << "Created a table." << endl;
	return 0;
}

int InsertTable(const char *sql_insert, sqlite3 *db)
{
	int rc = 0;
	char *error = NULL;
	// Execute SQL
	cout << "Inserting a value into a table ..." << endl;
	rc = sqlite3_exec(db, sql_insert, NULL, NULL, &error);
	if (rc)
	{
		cerr << "Error executing SQLite3 statement: " << sqlite3_errmsg(db) << endl << endl;
		sqlite3_free(error);
		system("pause");
		return -1;
	}
	cout << "Inserted a value into the table." << endl;
	return 0;
}

int DisplayTable(const char *sql_select, sqlite3 *db)
{
	int rc = 0;
	char *error = NULL;

	// Display MyTable
	//cout << "Retrieving values in a table ..." << endl;
	char **results = NULL;
	int rows, columns;
	// A result table is memory data structure created by the sqlite3_get_table() interface.
	// A result table records the complete query results from one or more queries.
	sqlite3_get_table(db, sql_select, &results, &rows, &columns, &error);
	if (rc)
	{
		cerr << "Error executing SQLite3 query: " << sqlite3_errmsg(db) << endl << endl;
		sqlite3_free(error);
		system("pause");
		return -1;
	}

	// Display Table
	for (int rowCtr = 0; rowCtr <= rows; ++rowCtr)
	{
		for (int colCtr = 0; colCtr < columns; ++colCtr)
		{
			// Determine Cell Position
			int cellPosition = (rowCtr * columns) + colCtr;

			// Display Cell Value
			cout.width(12);
			cout.setf(ios::left);
			cout << results[cellPosition] << " ";
		}

		// End Line
		//cout << endl;

		// Display Separator For Header
		if (0 == rowCtr)
		{
			for (int colCtr = 0; colCtr < columns; ++colCtr)
			{
				cout.width(12);
				cout.setf(ios::left);
				cout << "~~~~~~~~~~~~ ";
			}
			//cout << endl;
		}
	}
	// This function properly releases the value array returned by sqlite3_get_table()
	sqlite3_free_table(results);
	return 0;
}

int DisplayFundamentalData(const char *sql_select, sqlite3 *db, Stock* myStock, int num, vector<float> & dividend_vector, vector<float> &PE_vector,vector<float> &profit_vector, vector<float> &ROA_vector, vector<float> &ROE_vector)
{
	string PE;

	float High_52Week = 0, Low_52Week = 0, MA_50Day = 0, MA_200Day = 0, DividendYield = 0, PERatio = 0, beta = 0,  ProfitMargin=0, ReturnOnAssetsTTM=0, ReturnOnEquityTTM=0;
	int rc = 0;
	char *error = NULL;

	// Display MyTable
	//cout << "Retrieving values in a table ..." << endl;
	char **results = NULL;
	int rows, columns;
	// A result table is memory data structure created by the sqlite3_get_table() interface.
	// A result table records the complete query results from one or more queries.
	sqlite3_get_table(db, sql_select, &results, &rows, &columns, &error);
	if (rc)
	{
		cerr << "Error executing SQLite3 query: " << sqlite3_errmsg(db) << endl << endl;
		sqlite3_free(error);
		system("pause");
		return -1;
	}

	// Display Table
	int rowCtr = num + 1;
	for (int colCtr = 0; colCtr < columns; ++colCtr)
	{

		// Determine Cell Position
		int cellPosition = (rowCtr * columns) + colCtr;
		if (colCtr == 2)
			High_52Week = atof(results[cellPosition]);
		else if (colCtr == 3)
			Low_52Week = atof(results[cellPosition]);
		else if (colCtr == 4)
			MA_50Day = atof(results[cellPosition]);
		else if (colCtr == 5)
			MA_200Day = atof(results[cellPosition]);
		else if (colCtr == 6)
			DividendYield = atof(results[cellPosition]);
		else if (colCtr == 7)
			PERatio = atof(results[cellPosition]);
		else if (colCtr == 8)
			beta = atof(results[cellPosition]);
		else if (colCtr == 9)
			ProfitMargin = atof(results[cellPosition]);
		else if (colCtr == 10)
			ReturnOnAssetsTTM = atof(results[cellPosition]);
		else if (colCtr == 11)
			ReturnOnEquityTTM = atof(results[cellPosition]);
		//ProfitMargin,ReturnOnAssetsTTM,ReturnOnEquityTTM

	}
	dividend_vector.push_back(DividendYield);
	if (PERatio == 100000)
		PERatio = -1;
	if (MA_50Day == 0)
		MA_50Day = 1;
	if (High_52Week == 0)
		High_52Week = 1000;
	if (MA_200Day == 0)
		MA_200Day = 1;
	if (ProfitMargin == 100000)
		ProfitMargin = 0;
	if (ReturnOnAssetsTTM == 100000)
		ReturnOnAssetsTTM = 0;
	if (ReturnOnEquityTTM == 100000)
		ReturnOnEquityTTM = 0;

	PE_vector.push_back(PERatio);
	profit_vector.push_back(ProfitMargin);
	ROA_vector.push_back(ReturnOnAssetsTTM);
	ROE_vector.push_back(ReturnOnEquityTTM);
	Fundamental_Data* aFundamental_Data=new Fundamental_Data(High_52Week, Low_52Week, MA_50Day, MA_200Day, DividendYield, PERatio, beta, ProfitMargin, ReturnOnAssetsTTM, ReturnOnEquityTTM);
	myStock->addFundamental(aFundamental_Data);
	//cout << (myStock->Get_fundamentals())->get_MA_50days() << endl;
	// This function properly releases the value array returned by sqlite3_get_table()
	sqlite3_free_table(results);
	return 0;
}
int DisplaySP500Ticker(const char *sql_select, sqlite3 *db, vector<string>& ticker_list)
{

	string ticker;

	int rc = 0;
	char *error = NULL;

	// Display MyTable
	//cout << "Retrieving values in a table ..." << endl;
	char **results = NULL;
	int rows, columns;
	// A result table is memory data structure created by the sqlite3_get_table() interface.
	// A result table records the complete query results from one or more queries.
	sqlite3_get_table(db, sql_select, &results, &rows, &columns, &error);
	if (rc)
	{
		cerr << "Error executing SQLite3 query: " << sqlite3_errmsg(db) << endl << endl;
		sqlite3_free(error);
		system("pause");
		return -1;
	}

	// Display Table
	for (int rowCtr = 1; rowCtr <= rows; ++rowCtr)
	{
		// Determine Cell Position

		int cellPosition = (rowCtr * columns) + 1;
		ticker = (results[cellPosition]);
		ticker_list.push_back(ticker);


		// End Line
		//cout << endl;


	}
	// This function properly releases the value array returned by sqlite3_get_table()
	sqlite3_free_table(results);
	return 0;
}
int DisplayNewSP500Ticker(const char *sql_select, sqlite3 *db, vector<string>& ticker_list, vector<float>&weight_list, vector<float> &share_held_list, vector<map<string, vector<string>>> &sector_category)
{

	string ticker, sector;
	float weight = 0;
	float share_held = 0;
	int rc = 0;
	char *error = NULL;

	// Display MyTable
	//cout << "Retrieving values in a table ..." << endl;
	char **results = NULL;
	int rows, columns;
	// A result table is memory data structure created by the sqlite3_get_table() interface.
	// A result table records the complete query results from one or more queries.
	sqlite3_get_table(db, sql_select, &results, &rows, &columns, &error);
	if (rc)
	{
		cerr << "Error executing SQLite3 query: " << sqlite3_errmsg(db) << endl << endl;
		sqlite3_free(error);
		system("pause");
		return -1;
	}

	// Display Table
	for (int rowCtr = 1; rowCtr <= rows; ++rowCtr)
	{

		// Determine Cell Position

		//int cellPosition = (rowCtr * columns) + 1;
		//ticker = (results[cellPosition]);
		//ticker_list.push_back(ticker);
		for (int colCtr = 0; colCtr < columns; ++colCtr)
		{
			// Determine Cell Position
			int cellPosition = (rowCtr * columns) + colCtr;
			if (colCtr == 1)
				ticker = results[cellPosition];
			else if (colCtr == 3)
				sector = results[cellPosition];
			else if (colCtr == 4)
				weight = atof(results[cellPosition]);
			else if (colCtr == 5)
				share_held = atof(results[cellPosition]);


		}

		if (ticker == "BRK.B")
			ticker = "BRK_B";
		if (ticker == "BF.B")
			ticker = "BF_B";
		if (ticker == "FOXA")
			continue;
		if (ticker == "LIN")
			continue;
		if (ticker == "DOW")
			continue;
		if (ticker == "CTVA")
			continue;
		if (ticker == "CPRI")
			continue;
		if (ticker == "AMCR")
			continue;
		ticker_list.push_back(ticker);
		weight_list.push_back(weight);
		share_held_list.push_back(share_held);
		if (sector == "Communication Services")
			sector_category[0]["Communication_Services"].push_back(ticker);
		else if (sector == "Consumer Discretionary")
			sector_category[1]["Consumer_Discretionary"].push_back(ticker);
		else if (sector == "Consumer Staples")
			sector_category[2]["Consumer_Staples"].push_back(ticker);
		else if (sector == "Energy")
			sector_category[3]["Energy"].push_back(ticker);
		else if (sector == "Financials")
			sector_category[4]["Financials"].push_back(ticker);
		else if (sector == "Health Care")
			sector_category[5]["Health_Care"].push_back(ticker);
		else if (sector == "Industrials")
			sector_category[6]["Industrials"].push_back(ticker);
		else if (sector == "Information Technology")
			sector_category[7]["Information_Technology"].push_back(ticker);
		else if (sector == "Materials")
			sector_category[8]["Materials"].push_back(ticker);
		else if (sector == "Real Estate")
			sector_category[9]["Real_Estate"].push_back(ticker);
		else if (sector == "Utilities")
			sector_category[10]["Utilities"].push_back(ticker);
		else if (sector == "Unassigned")
			sector_category[11]["Unassigned"].push_back(ticker);

	}

	//for (vector<vector<string>>::iterator it = sector_category.begin(); it != sector_category.end(); ++it)
	//	cout << *it << endl;

	// This function properly releases the value array returned by sqlite3_get_table()
	sqlite3_free_table(results);
	return 0;
}
int DisplayRiskFreeRateData(const char *sql_select, sqlite3 *db, map<string, float> &RiskFreeRate,vector<float>& RiskFreeRate_vector)
{
	string date;
	float adjusted_close = 0;
	int rc = 0;
	char *error = NULL;

	// Display MyTable
	//cout << "Retrieving values in a table ..." << endl;
	char **results = NULL;
	int rows, columns;
	// A result table is memory data structure created by the sqlite3_get_table() interface.
	// A result table records the complete query results from one or more queries.
	sqlite3_get_table(db, sql_select, &results, &rows, &columns, &error);
	if (rc)
	{
		cerr << "Error executing SQLite3 query: " << sqlite3_errmsg(db) << endl << endl;
		sqlite3_free(error);
		system("pause");
		return -1;
	}

	// Display Table
	for (int rowCtr = 0; rowCtr <= rows; ++rowCtr)
	{

		for (int colCtr = 0; colCtr < columns; ++colCtr)
		{
			// Determine Cell Position
			int cellPosition = (rowCtr * columns) + colCtr;
			if (colCtr == 2)
				date = (results[cellPosition]);
			else if (colCtr == 7)
				adjusted_close = atof(results[cellPosition]);
		}
		RiskFreeRate[date] = adjusted_close;
		RiskFreeRate_vector.push_back(pow(1+adjusted_close*0.01,1/252.0)-1);
		// End Line
		//cout << endl;

	}
	// This function properly releases the value array returned by sqlite3_get_table()
	sqlite3_free_table(results);
	return 0;

}
int DisplayTradeData(const char *sql_select, sqlite3 *db, Stock* myStock)
{
	int volume = 0;
	string date;
	float open = 0, high = 0, low = 0, close = 0, adjusted_close = 0, daily_return = 0;
	int rc = 0;
	vector<Trade> trades;
	char *error = NULL;

	// Display MyTable
	//cout << "Retrieving values in a table ..." << endl;
	char **results = NULL;
	int rows, columns;
	// A result table is memory data structure created by the sqlite3_get_table() interface.
	// A result table records the complete query results from one or more queries.
	sqlite3_get_table(db, sql_select, &results, &rows, &columns, &error);
	if (rc)
	{
		cerr << "Error executing SQLite3 query: " << sqlite3_errmsg(db) << endl << endl;
		sqlite3_free(error);
		system("pause");
		return -1;
	}

	// Display Table
	for (int rowCtr = 1; rowCtr <= rows; ++rowCtr)
	{

		for (int colCtr = 0; colCtr < columns; ++colCtr)
		{

			// Determine Cell Position
			int cellPosition = (rowCtr * columns) + colCtr;
			if (colCtr == 2)
			{

				date = (results[cellPosition]);


			}
			else if (colCtr == 3)
			{
				open = atof(results[cellPosition]);

			}
			else if (colCtr == 4)
				high = atof(results[cellPosition]);
			else if (colCtr == 5)
				low = atof(results[cellPosition]);
			else if (colCtr == 6)
				close = atof(results[cellPosition]);
			else if (colCtr == 7)
				adjusted_close = atof(results[cellPosition]);
			else if (colCtr == 8)
				volume = atoi(results[cellPosition]);
			else if (colCtr == 9)
				daily_return = atof(results[cellPosition]);


		}

		// End Line
		//cout << endl;
		Trade* aTrade=new Trade(date, open, high, low, close, adjusted_close, volume, daily_return);
		//cout << *aTrade << endl;
		myStock->addTrade(aTrade);
	}
	//myStock.addTrade(trades);
	// This function properly releases the value array returned by sqlite3_get_table()
	sqlite3_free_table(results);
	return 0;
}

int PopulateFundamentalDataTable(const Json::Value & root, string symbol, sqlite3 *db, int num)
{
	string PE;
	string date;
	float High_52Week = 0, Low_52Week = 0, MA_50Day = 0, MA_200Day = 0, DividendYield = 0, PERatio = 0, ProfitMargin=0, ReturnOnAssetsTTM=0, ReturnOnEquityTTM=0;
	float beta = 0;
	int volume;
	Stock myStock(symbol);
	int count = num;

	for (Json::Value::const_iterator itr = root.begin(); itr != root.end(); itr++)
	{
		//	cout << *itr << endl;
		if (itr.key().asString() == "Technicals")
			for (Json::Value::const_iterator inner = (*itr).begin(); inner != (*itr).end(); inner++)
			{
				//cout << inner.key() << ": " << *inner << endl;

				if (inner.key().asString() == "52WeekHigh")
				{
					if ((inner->asString()).empty())
					{
						High_52Week = 100000;
					}
					else
						High_52Week = (float)atof((inner->asCString()));
				}
				else if (inner.key().asString() == "52WeekLow")
				{
					if ((inner->asString()).empty())
					{
						Low_52Week = 100000;
					}
					else
						Low_52Week = (float)atof((inner->asCString()));
				}
				else if (inner.key().asString() == "Beta")
				{
					if ((inner->asString()).empty())
					{
						beta = 100000;
					}
					else
						beta = (float)atof((inner->asCString()));
				}
				else if (inner.key().asString() == "50DayMA")
				{
					if ((inner->asString()).empty())
					{
						MA_50Day = 1;
					}
					else
						MA_50Day = (float)atof((inner->asCString()));
				}
				else if (inner.key().asString() == "200DayMA")
				{
					if ((inner->asString()).empty())
					{
						MA_200Day = 1;
					}
					else
						MA_200Day = (float)atof((inner->asCString()));
				}

			}
		else if (itr.key().asString() == "Highlights")
			for (Json::Value::const_iterator inner_1 = (*itr).begin(); inner_1 != (*itr).end(); inner_1++)
			{
				if ((inner_1.key().asString() == "DividendYield") || (inner_1.key().asString() == "Yield"))
				{
					if ((inner_1->asString()).empty())
					{
						DividendYield = 100000;
					}
					else
						DividendYield = (float)atof((inner_1->asCString()));

				}
				else if (inner_1.key().asString() == "PERatio")
				{

					if ((inner_1->asString()).empty())
					{
						PERatio = 100000;
					}
					else
						PERatio = (float)atof((inner_1->asCString()));
				}
				else if (inner_1.key().asString() == "ProfitMargin")
				{

					if ((inner_1->asString()).empty())
					{
						ProfitMargin = 0;
					}
					else
						ProfitMargin = (float)atof((inner_1->asCString()));
				}
				else if (inner_1.key().asString() == "ReturnOnAssetsTTM")
				{

					if ((inner_1->asString()).empty())
					{
						ReturnOnAssetsTTM = 0;
					}
					else
						ReturnOnAssetsTTM = (float)atof((inner_1->asCString()));
				}
				else if (inner_1.key().asString() == "ReturnOnEquityTTM")
				{

					if ((inner_1->asString()).empty())
					{
						ReturnOnEquityTTM = 0;
					}
					else
						ReturnOnEquityTTM = (float)atof((inner_1->asCString()));
				}

			}
	}

	//dividend_vector.push_back(DividendYield);
	

	// Execute SQL
	char stock_fundamental_insert_table[512];
	sprintf_s(stock_fundamental_insert_table, "INSERT INTO stock_fundamental (id, symbol,High_52Week, Low_52Week, MA_50Day, MA_200Day, DividendYield, PERatio,Beta,ProfitMargin,ReturnOnAssetsTTM,ReturnOnEquityTTM) VALUES(%d, \"%s\", %f, %f, %f, %f, %f, %f, %f,%f,%f,%f)", count, symbol.c_str(), High_52Week, Low_52Week, MA_50Day, MA_200Day, DividendYield, PERatio, beta, ProfitMargin, ReturnOnAssetsTTM, ReturnOnEquityTTM);
	if (InsertTable(stock_fundamental_insert_table, db) == -1)
		return -1;



	//cout << myStock;

	return 0;
}
int PopulateStockTable(const Json::Value & root, string symbol, sqlite3 *db, int num)
{
	string date;
	float open, high, low, close, adjusted_close, volume,daily_return = 0;
    //unsigned int volume;
	Stock myStock(symbol);
	int count = num;
	cout << count;
	for (Json::Value::const_iterator itr = root.begin(); itr != root.end(); itr++)
	{
		cout << *itr << endl;
		for (Json::Value::const_iterator inner = (*itr).begin(); inner != (*itr).end(); inner++)
		{
			//cout << inner.key() << ": " << *inner << endl;

			if (inner.key().asString() == "adjusted_close")
			{

				adjusted_close = (float)(inner->asDouble());
				//cout << "adjusted_close" << endl;
			}
			else if (inner.key().asString() == "close")
			{ 
				close = (float)(inner->asDouble());
				//cout << "close" << endl;
			}
			else if (inner.key() == "date")
			{
				date = inner->asString();
				//cout << "date" << endl;
			}
			else if (inner.key().asString() == "high")
			{
				high = (float)(inner->asDouble());
			    //cout << "high" << endl;
		     }
			else if (inner.key().asString() == "low")
			{ 
				low = (float)(inner->asDouble());
				//cout << low << endl;
			}
			else if (inner.key() == "open")
			{
				open = (float)(inner->asDouble());
				//cout << open << endl;
			}
			else if (inner.key().asString() == "volume")
			{
				volume = (float)(inner->asDouble());//(inner->asInt());
				//cout << volume << endl;
			}
			else
			{
				cout << "Invalid json field" << endl;
				system("pause");
				return -1;
			}
		}
		volume = (int)volume;
		//U[i] = fundamentals.get_DividendYield() + (v3[i].get_adjusted_close() - v2[i].get_adjusted_close()) / v2[i].get_adjusted_close();
		Trade* aTrade=new Trade(date, open, high, low, close, adjusted_close, volume);
		myStock.addTrade(aTrade);

		
		daily_return = myStock.daily_return(count);
		
		count++;
		cout << count<<endl;
		// Execute SQL
		char stockDB_insert_table[512];
		sprintf_s(stockDB_insert_table, "INSERT INTO %s (id, symbol, date, open, high, low, close, adjusted_close, volume,daily_return) VALUES(%d, \"%s\", \"%s\", %f, %f, %f, %f, %f, %f,%f)", symbol.c_str(), count, symbol.c_str(), date.c_str(), open, high, low, close, adjusted_close, volume, daily_return);
		if (InsertTable(stockDB_insert_table, db) == -1)
			return -1;
	}
	//cout << myStock;

	return 0;
}
int PopulateSP500Table(const Json::Value & root, sqlite3 *db, vector <string> &SP_ticker)
{
	int count = 0;
	string name, symbol, sector;
	for (Json::Value::const_iterator itr = root.begin(); itr != root.end(); itr++)
	{
		cout << *itr << endl;
		for (Json::Value::const_iterator inner = (*itr).begin(); inner != (*itr).end(); inner++)
		{
			//cout << inner.key() << ": " << *inner << endl;

			if (inner.key().asString() == "Name")
				name = inner->asString();
			else if (inner.key().asString() == "Sector")
				sector = inner->asString();
			else if (inner.key() == "Symbol")
			{
				symbol = inner->asString();
				SP_ticker.push_back(symbol);
			}
			else
			{
				cout << "Invalid json field" << endl;
				system("pause");
				return -1;
			}
		}
		count++;

		// Execute SQL
		char sp500_insert_table[512];
		sprintf_s(sp500_insert_table, "INSERT INTO SP500 (id, symbol, name, sector) VALUES(%d, \"%s\", \"%s\", \"%s\")", count, symbol.c_str(), name.c_str(), sector.c_str());
		if (InsertTable(sp500_insert_table, db) == -1)
			return -1;
	}
	return 0;
}
int GetTickerData(sqlite3 *db)//, vector<string> & stock_ticker, vector<float> &stock_weight)
{
	int count = -1;
	fstream file;
	file.open("SPYAllHoldings.csv");
	string Name, ticker, sector, weight, share_held;
	//float weight, share_held;
	getline(file, Name);
	while (file.good()) {
		//Stock information;	
		getline(file, Name, ',');
		if (Name == "") continue;

		getline(file, ticker, ',');

		getline(file, weight, ',');

		getline(file, sector, ',');
		//cout << sector << endl;
		getline(file, share_held);
		//cout << share_held << endl;

		count += 1;
		// Execute SQL
		char New_sp500_insert_table[512];
		sprintf_s(New_sp500_insert_table, "INSERT INTO New_SP500 (id, symbol, name, sector,weight,share_held) VALUES(%d, \"%s\", \"%s\", \"%s\", %f, %f)", count, ticker.c_str(), Name.c_str(), sector.c_str(), atof(weight.c_str()), atof(share_held.c_str()));
		if (InsertTable(New_sp500_insert_table, db) == -1)
			return -1;
	}
	file.close();
	return 0;
}
int PopulateRiskFreeRateTable(const Json::Value & root, sqlite3 *db)
{
	string date;
	float open, high, low, close, adjusted_close;
	int count = 0;
	for (Json::Value::const_iterator itr = root.begin(); itr != root.end(); itr++)
	{
		cout << *itr << endl;
		for (Json::Value::const_iterator inner = (*itr).begin(); inner != (*itr).end(); inner++)
		{
			//cout << inner.key() << ": " << *inner << endl;

			if (inner.key().asString() == "adjusted_close")
				adjusted_close = (float)(inner->asDouble());
			else if (inner.key().asString() == "close")
				close = (float)(inner->asDouble());
			else if (inner.key() == "date")
				date = inner->asString();
			else if (inner.key().asString() == "high")
				high = (float)(inner->asDouble());
			else if (inner.key().asString() == "low")
				low = (float)(inner->asDouble());
			else if (inner.key() == "open")
				open = (float)(inner->asDouble());

		}

		count++;
		// Execute SQL
		char RiskFreeRate_insert_table[512];  //(id, symbol, date, open, high, low, close, adjusted_close, volume) VALUES(%d, \"%s\", \"%s\", %f, %f, %f, %f, %f, %d)", symbol.c_str(), count, symbol.c_str(), date.c_str(), open, high, low, close, adjusted_close, volume);
		sprintf_s(RiskFreeRate_insert_table, "INSERT INTO RiskFreeRate (id, rf, date, open, high, low, close, adjusted_close) VALUES(%d, \"%s\", \"%s\", %f, %f, %f, %f, %f)", count, "rf", date.c_str(), open, high, low, close, adjusted_close);
		if (InsertTable(RiskFreeRate_insert_table, db) == -1)
			return -1;
	}
	return 0;
}
#endif
