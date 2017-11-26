/*
�쐬��:         �{�� ���~(�݂₴�� �����Ƃ�/Seito Miyazaki)
�쐬�J�n��:     2017/11/24
�쐬�I����:     2017/11/26
�v���O������:   apache.exe
���[���A�h���X: seitou1207@gmail.com
�g�p�R���p�C��: Visual Studio Express 2015

������Ѓt�B�b�N�X�|�C���g�@�v���O���~���O�����ۑ�

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

//�O���[�o���R���X�^���g
const int AD_LENGTH = 256;

//�\���E�N���X��`
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
