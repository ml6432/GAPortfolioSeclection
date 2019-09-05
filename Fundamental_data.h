#pragma once
#ifndef Fundamental_data_h
#define Fundamental_data_h
#include <iostream>
#include <string>
#include <map>
#include <fstream>
#include <vector>
#include <stdio.h>
#include "Utility.h"

#define NUM_OF_STOCKS 505

using namespace std;

class Fundamental_Data
{
private:
	float PERatio;
	float DividendYield;
	float high_52weeks;
	float low_52weeks;
	float MA_50days;
	float MA_200days;
	float beta;
	float ProfitMargin;
	float ReturnOnAssetsTTM;
	float ReturnOnEquityTTM;
public:
	Fundamental_Data()
	{}
	Fundamental_Data(float high_52weeks_, float low_52weeks_, float MA_50days_, float MA_200days_, float DividendYield_, float PERatio_, float beta_, float ProfitMargin_, float ReturnOnAssetsTTM_, float ReturnOnEquityTTM_) :
		high_52weeks(high_52weeks_), low_52weeks(low_52weeks_), MA_50days(MA_50days_), MA_200days(MA_200days_), DividendYield(DividendYield_), PERatio(PERatio_), beta(beta_), ProfitMargin( ProfitMargin_),ReturnOnAssetsTTM(ReturnOnAssetsTTM_), ReturnOnEquityTTM(ReturnOnEquityTTM_)
	{}
	~Fundamental_Data() {}
	float get_DividendYield()
	{
		return DividendYield;
	}
	float get_beta()
	{
		return beta;
	}
	float get_ProfitMargin()
	{
		return ProfitMargin;
	}
	float get_ReturnOnAssetsTTM()
	{
		return ReturnOnAssetsTTM;
	}
	float get_ReturnOnEquityTTM()
	{
		return ReturnOnEquityTTM;
	}
	float get_high_52weeks()
	{
		return  high_52weeks;
	}
	float get_MA_50days()
	{
		return MA_50days;
	}
	float get_MA_200days()
	{
		return MA_200days;
	}
	float get_PERatio()
	{
		return PERatio;
	}
};
#endif