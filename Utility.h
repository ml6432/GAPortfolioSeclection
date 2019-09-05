#pragma once
#ifndef Utility_h
#define Utility_h
#include <iostream>
#include <vector>
#include <numeric>
#include "Stock.h"

using namespace std;
typedef vector<float> Vector;
typedef vector<Vector> Matrix;

ostream& operator<<(ostream& os, const vector<float>& v)
{
	os << " ";
	for (unsigned int i = 0; i < v.size(); i++) {
		os << v[i];
		if (i != v.size())
			os << ", ";
	}
	os << "\n";
	return os;
}
ostream& operator<<(ostream& os, const vector<string>& v)
{
	os << " ";
	for (unsigned int i = 0; i < v.size(); i++) {
		os << v[i];
		if (i != v.size())
			os << ", ";
	}
	os << "\n";
	return os;
}

vector<float> operator*(const vector<float>& v1, float s)
{
	long z = v1.size();
	vector<float> U(z);
	for (long i = 0; i < z; i++) U[i] = s * v1[i];
	return U;
}
vector<float> operator+(const vector<float>& v1, const vector<float>& v2)
{
	long s = v1.size();
	vector<float> U(s);
	for (long i = 0; i < s; i++) U[i] = v1[i] + v2[i];
	return U;
}

vector<float> operator-(const vector<float>& v1, float k)
{
	long s = v1.size();
	vector<float> U(s);
	for (long i = 0; i < s; i++) U[i] = v1[i] - k;
	return U;
}
vector<float> operator-(const vector<float>& v1, const vector<float>& v2)
{
	long s = v1.size();
	vector<float> U(s);
	for (long i = 0; i < s; i++) U[i] = v1[i] - v2[i];
	return U;
}
vector<float> operator/(const vector<float>& v1, long s)
{
	long z = v1.size();
	vector<float> U(z);
	for (long i = 0; i < z; i++) U[i] = v1[i] / s;
	return U;
}
vector<float> operator*(const vector<float>& v1, const vector<float>& v2)
{
	long z = v1.size();
	vector<float> U(z);
	for (long i = 0; i < z; i++) U[i] = v2[i] * v1[i];
	return U;
}


Vector operator*(const Matrix& C, const Vector& V)
{
	int d = C.size();
	Vector W(d);
	for (int j = 0; j < d; j++)
	{
		W[j] = 0.0;
		for (int l = 0; l < d; l++) W[j] = W[j] + C[j][l] * V[l];
	}
	return W;
}
typedef map<string, float> maps;
maps operator-(maps & m1,  maps &m2)
{
	maps myMap;
	for (maps::iterator it = m1.begin(); it != m1.end(); it++)
	{
		myMap[it->first] = m1[it->first] - m2[it->first];
	}
	return myMap;
}


float vector_average(const vector<float> v)
{
	float average = 0;
	average=accumulate(v.begin(), v.end(), 0.0 / v.size()) / v.size();
	return average;
}

#endif