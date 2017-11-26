/*
作成者:         宮崎 星冬(みやざき せいとう/Seito Miyazaki)
作成開始日:     2017/11/24
作成終了日:     2017/11/26
プログラム名:   apache.exe
メールアドレス: seitou1207@gmail.com
使用コンパイラ: Visual Studio Express 2015

株式会社フィックスポイント　プログラミング試験課題

*/

#pragma once

#include <stdio.h>
#include <tchar.h>
#include <cstdlib>
#include <cctype>
#include <iostream>
#include <iomanip>
#include <fstream>
#include <string>
#include <vector>
#include <algorithm>

using namespace std;

//グローバルコンスタント
const int AD_LENGTH = 256;

//構造・クラス定義
struct dayTime
{
public:
	dayTime();

	unsigned int hour,
		day,
		month,
		year;

	bool operator== (const dayTime& r)
	{
		if (this->hour == r.hour && this->day == r.day &&
			this->month == r.month && this->year == r.year)
			return true;
		else return false;
	}

	bool operator< (const dayTime& r)
	{
		if (this->year < r.year)
			return true;
		else if (this->year == r.year && this->month < r.month)
			return true;
		else if (this->year == r.year && this->month == r.month &&
				 this->day < r.day)
			return true;
		else if (this->year == r.year && this->month == r.month &&
				 this->day == r.day && this->hour < r.hour)
			return true;
		else return false;
	}
	bool operator<= (const dayTime& r)
	{
		if (this->year < r.year)
			return true;
		else if (this->year == r.year && this->month < r.month)
			return true;
		else if (this->year == r.year && this->month == r.month &&
				 this->day < r.day)
			return true;
		else if (this->year == r.year && this->month == r.month &&
				 this->day == r.day && this->hour <= r.hour)
			return true;
		else return false;
	}
};

class aLog
{
public:
	aLog();

	char aRemoteAdd[AD_LENGTH];
	dayTime aTime;
};

struct timeAdd
{
	timeAdd();
	dayTime aTime;
	int  count;
	bool operator< (const timeAdd& r)
	{
		if (this->aTime < r.aTime) return true;
		return false;
	}
};

struct countAdd
{
	string cRemoteAdd;
	int  count;
	bool operator== (const countAdd& r)
	{
		if (this->count == r.count && this->cRemoteAdd == r.cRemoteAdd) return true;
		else return false;
	}
	static bool adPrefLT(const countAdd& l, const countAdd& r)
	{
		if (l.cRemoteAdd < r.cRemoteAdd) return true;
		else return false;
	}
	static bool coPrefLT(const countAdd& l, const countAdd& r)
	{
		if (l.count < r.count) return true;
		else if (l.count == r.count && l.cRemoteAdd < r.cRemoteAdd) return true;
		else return false;
	}
	static bool coPrefRT(const countAdd& l, const countAdd& r)
	{
		if (l.count > r.count) return true;
		else if (l.count == r.count && l.cRemoteAdd < r.cRemoteAdd) return true;
		else return false;
	}
	bool operator< (const countAdd& r)
	{
		return coPrefLT(*this, r);
	}
};
