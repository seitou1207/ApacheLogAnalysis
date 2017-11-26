/*
作成者:         宮崎 星冬(みやざき せいとう/Seito Miyazaki)
作成開始日:     2017/11/24
作成終了日:     2017/11/26
プログラム名:   apache.exe
メールアドレス: seitou1207@gmail.com
使用コンパイラ: Visual Studio Express 2015

株式会社フィックスポイント　プログラミング試験課題

*/

#include "stdafx.h"

using namespace std;

//以下全て初期化

dayTime::dayTime()
{
	hour = 0;
	day = 1;
	month = 1;
	year = 2017;
}

aLog::aLog() { strcpy_s(aRemoteAdd, "255.255.255.255"); }

timeAdd::timeAdd() { count = 0; }
