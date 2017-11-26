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

int getMonth(char*); //月の文字列→数値への変換用
void invalidDate() { cout << " Error: Invalid date.\n"; }; //エラーメッセージ表示用

int main()
{
	ifstream file; //ログファイルからの取り込み用
	string fileName, buffer; //ファイル名用、ファイルからの一時取り込み用
	aLog accessLog; //ファイルからの一時取り込み用
	timeAdd tBuf; //同上
	countAdd coBuf; //同上
	vector<timeAdd> tAdCount;  //時間帯別表示用
	vector<countAdd> cAdCount; //リモートアドレス別表示用
	dayTime sD, eD; //日時指定用
	bool tf,		  //ループ管理など
	 	 aSD = false, //日時指定用
		 aED = false, //日時指定用
		 fL = true;   //各ファイルの取り込み回数判別用
	char cBuf[5]; //ユーザ入力取り込み用
	unsigned int pos, pos2, //区切り位置
				 tNumElem, cNumElem, //要素数
				 numFiles, //ファイル数
				 maxAdLen = 0, //最大のアドレス長
				 rCount; //読み取りレコード数

	cout << "=================================\n"
		 << " Apache Log Analysis Program\n"
		 << " Ver 1.0\n"
		 << " (c)2017 Seito Miyazaki\n"
		 << "=================================\n";

	//日時指定確認
	do { //開始日時指定
		cout << " Do you want to specify the start date?(Y or N): ";
		cin  >> cBuf[0];
		cin.ignore(4096, '\n');
		cBuf[0] = toupper(cBuf[0]);
	} while (cBuf[0] != 'Y' && cBuf[0] != 'N');

	if (cBuf[0] == 'Y')
	{
		do { 
			tf = false;
			cout << " Enter the start date (format: YYYY/MM/DD): ";
			getline(cin, buffer);

			if (buffer.size() > 10)
			{
				invalidDate();
				tf = true;
			}
				
			else {
				int countS = 0;
				for (unsigned int count = 0; !tf && count < buffer.size(); count++)
				{
					if (buffer[count] == '/')
					{
						countS++;
						if (countS > 2)
						{
							invalidDate();
							tf = true;
						}
					}

					else if (buffer[count] < '0' || buffer[count] > '9')
					{
						invalidDate();
						tf = true;
					}
				}

				if (countS != 2)
				{
					invalidDate();
					tf = true;
				}

				else
				{ 
					pos  = buffer.find("/"); 
					pos2 = buffer.find("/", pos + 1); //二つの'/'を取得

					if (pos < 2 || pos > 4) //年は2-4桁
					{
						invalidDate();
						tf = true;
					}

					else if (pos2 - pos == 1 || pos2 - pos > 3) //月は1-2桁
					{
						invalidDate();
						tf = true;
					}

					else if (buffer.size() - pos2 < 2 || buffer.size() - pos2 > 3) //日は1-2桁
					{
						invalidDate();
						tf = true;
					}

					else {
						//ここから日関連の処理
						for (unsigned int count = pos2 + 1; count < buffer.size(); count++)
							cBuf[count - pos2 - 1] = buffer[count];

						cBuf[buffer.size() - pos2 - 1] = '\0';

						if (buffer.size() - pos2 == 3)
							sD.day = 10 * (cBuf[0] - '0') + (cBuf[1] - '0');

						else
							sD.day = cBuf[0] - '0';

						if (sD.day == 0 || sD.day > 31)
						{
							invalidDate();
							tf = true;
						}
						//日関連の処理ここまで
						else
						{
							//ここから月関連の処理
							for (unsigned int count = pos + 1; count < pos2; count++)
								cBuf[count - pos - 1] = buffer[count];

							cBuf[pos2 - pos - 1] = '\0';

							if (pos2 - pos == 3)
								sD.month = 10 * (cBuf[0] - '0') + (cBuf[1] - '0');

							else
								sD.month = cBuf[0] - '0';

							if (sD.month == 0 || sD.month > 12)
							{
								invalidDate();
								tf = true;
							}

							else if (sD.day == 31)
							{
								if (sD.month == 2 || sD.month == 4 || sD.month == 6 || sD.month == 9 || sD.month == 11)
								{
									invalidDate();
									tf = true;
								}
							}

							else if (sD.month == 2 && sD.day == 30)
							{
								invalidDate();
								tf = true;
							}
							//月関連の処理ここまで
							else 
							{
								//ここから年関連の処理
								for (unsigned int count = 0; count < pos; count++)
									cBuf[count] = buffer[count];
								cBuf[pos] = '\0';

								//文字列から年への変換処理
								if (pos == 4) //4桁入力時
									sD.year = 1000 * (cBuf[0] - '0') + 100 * (cBuf[1] - '0') + 10 * (cBuf[2] - '0') + (cBuf[3] - '0');

								else if (pos == 3) //3桁入力時(1900-2899年)
								{
									if (cBuf[0] == '9') // 1900-1999年
										sD.year = 1000 + 100 * (cBuf[0] - '0') + 10 * (cBuf[1] - '0') + (cBuf[2] - '0');

									else // 2000-2899年
										sD.year = 2000 + 100 * (cBuf[0] - '0') + 10 * (cBuf[1] - '0') + (cBuf[2] - '0');
								}

								else if (cBuf[0] == '9' && cBuf[1] >= '5') //2桁入力時(1995-1999年)
									sD.year = 1900 + 10 * (cBuf[0] - '0') + (cBuf[1] - '0');

								else //2桁入力時(2000-2094年)
									sD.year = 2000 + 10 * (cBuf[0] - '0') + (cBuf[1] - '0');

								if (sD.day == 29 && sD.month == 2) //閏年処理
								{
									if (sD.year % 4 != 0)
									{
										invalidDate();
										tf = true;
									}

									else if (sD.year % 100 == 0 && sD.year % 400 != 0)
									{
										invalidDate();
										tf = true;
										continue;
									}
								}
								//年関連処理ここまで
								//以下ユーザ確認
								do {
									cout << " The start date is: " << sD.year << "/" << sD.month << "/" << sD.day << endl
									     << " Is this correct?(Y or N): ";
									cin  >> cBuf[0];
									cin.ignore(4096, '\n');
									cBuf[0] = toupper(cBuf[0]);
								} while (cBuf[0] != 'Y' && cBuf[0] != 'N'); 

								if (cBuf[0] == 'N')
									tf = true;
								/*
								tfがfalseのままであればすべてのテストをクリアしているのでループ脱出
								trueに変わる際には(ユーザ確認時除き)エラーメッセージ表示
								*/
							}
						}
					}
				}
			}
		} while (tf);
		aSD = true; //開始日時指定ON
		sD.hour = 0;
	}

	do { //終了日時指定
		cout << " Do you want to specify the end date?(Y or N): ";
		cin  >> cBuf[0];
		cin.ignore(4096, '\n');
		cBuf[0] = toupper(cBuf[0]);
	} while (cBuf[0] != 'Y' && cBuf[0] != 'N');

	if (cBuf[0] == 'Y')
	{
		do {
			tf = false;
			cout << " Enter the end date (format: YYYY/MM/DD): ";
			getline(cin, buffer);

			if (buffer.size() > 10)
			{
				invalidDate();
				tf = true;
			}

			else {
				int countS = 0;
				for (unsigned int count = 0; !tf && count < buffer.size(); count++)
				{
					if (buffer[count] == '/')
					{
						countS++;
						if (countS > 2)
						{
							invalidDate();
							tf = true;
						}
					}

					else if (buffer[count] < '0' || buffer[count] > '9')
					{
						invalidDate();
						tf = true;
					}
				}

				if (countS != 2)
				{
					invalidDate();
					tf = true;
				}

				else
				{
					pos = buffer.find("/");
					pos2 = buffer.find("/", pos + 1); //二つの'/'を取得

					if (pos < 2 || pos > 4) //年は2-4桁
					{
						invalidDate();
						tf = true;
					}

					else if (pos2 - pos == 1 || pos2 - pos > 3) //月は1-2桁
					{
						invalidDate();
						tf = true;
					}

					else if (buffer.size() - pos2 < 2 || buffer.size() - pos2 > 3) //日は1-2桁
					{
						invalidDate();
						tf = true;
					}

					else {
						//ここから日関連の処理
						for (unsigned int count = pos2 + 1; count < buffer.size(); count++)
							cBuf[count - pos2 - 1] = buffer[count];

						cBuf[buffer.size() - pos2 - 1] = '\0';

						if (buffer.size() - pos2 == 3)
							eD.day = 10 * (cBuf[0] - '0') + (cBuf[1] - '0');

						else
							eD.day = cBuf[0] - '0';

						if (eD.day == 0 || eD.day > 31)
						{
							invalidDate();
							tf = true;
						}
						//日関連の処理ここまで
						else
						{
							//ここから月関連の処理
							for (unsigned int count = pos + 1; count < pos2; count++)
								cBuf[count - pos - 1] = buffer[count];

							cBuf[pos2 - pos - 1] = '\0';

							if (pos2 - pos == 3)
								eD.month = 10 * (cBuf[0] - '0') + (cBuf[1] - '0');

							else
								eD.month = cBuf[0] - '0';

							if (eD.month == 0 || eD.month > 12)
							{
								invalidDate();
								tf = true;
							}

							else if (eD.day == 31)
							{
								if (eD.month == 2 || eD.month == 4 || eD.month == 6 || eD.month == 9 || eD.month == 11)
								{
									invalidDate();
									tf = true;
								}
							}

							else if (eD.month == 2 && eD.day == 30)
							{
								invalidDate();
								tf = true;
							}
							//月関連の処理ここまで
							else
							{
								//ここから年関連の処理
								for (unsigned int count = 0; count < pos; count++)
									cBuf[count] = buffer[count];
								cBuf[pos] = '\0';

								//文字列から年への変換処理
								if (pos == 4) //4桁入力時
									eD.year = 1000 * (cBuf[0] - '0') + 100 * (cBuf[1] - '0') + 10 * (cBuf[2] - '0') + (cBuf[3] - '0');

								else if (pos == 3) //3桁入力時(1900-2899年)
								{
									if (cBuf[0] == '9') // 1900-1999年
										eD.year = 1000 + 100 * (cBuf[0] - '0') + 10 * (cBuf[1] - '0') + (cBuf[2] - '0');

									else // 2000-2899年
										eD.year = 2000 + 100 * (cBuf[0] - '0') + 10 * (cBuf[1] - '0') + (cBuf[2] - '0');
								}

								else if (cBuf[0] == '9' && cBuf[1] >= '5') //2桁入力時(1995-1999年)
									eD.year = 1900 + 10 * (cBuf[0] - '0') + (cBuf[1] - '0');

								else //2桁入力時(2000-2094年)
									eD.year = 2000 + 10 * (cBuf[0] - '0') + (cBuf[1] - '0');

								if (eD.day == 29 && eD.month == 2) //閏年処理
								{
									if (eD.year % 4 != 0)
									{
										invalidDate();
										tf = true;
									}

									else if (eD.year % 100 == 0 && eD.year % 400 != 0)
									{
										invalidDate();
										tf = true;
										continue;
									}
								}
								//年関連処理ここまで
								//以下ユーザ確認
								do {
									cout << " The end date is: " << eD.year << "/" << eD.month << "/" << eD.day << endl
										 << " Is this correct?(Y or N): ";
									cin  >> cBuf[0];
									cin.ignore(4096, '\n');
									cBuf[0] = toupper(cBuf[0]);
								} while (cBuf[0] != 'Y' && cBuf[0] != 'N');

								if (cBuf[0] == 'N')
									tf = true;
								/*
								tfがfalseのままであればすべてのテストをクリアしているのでループ脱出
								trueに変わる際には(ユーザ確認時除き)エラーメッセージ表示
								*/
							}
						}
					}
				}
			}
		} while (tf);
		aED = true; //終了日時指定ON
		eD.hour = 23;
	}
	//日付指定ここまで

	//ファイル取り込み
	do
	{
		tf = false;
		cout << " How many files do you wish to process?: "; //ファイル数確認
		getline(cin, buffer);
		for (unsigned int count = 0; count < buffer.size(); count++)
		{
			if (buffer[count] < '0' || buffer[count] > '9')
			{
				tf = true;
				break;
			}
		}

		if (tf)
			continue;

		numFiles = atoi(buffer.c_str());
		
		if (numFiles == 0)
			tf = true;
	} while (tf);

	for (unsigned int fCount = 0; fCount < numFiles; fCount++) //指定ファイル数の読み込み開始
	{
		
		do
		{
			//初期化
			rCount = 0;
			fL = true;
			
			cout << " Enter the name of the file (#" << fCount + 1 << "/" << numFiles << "): ";
			getline(cin, fileName); //ファイル名取得

			file.open(fileName.c_str()); //ファイルオープン

			if (!file)
				cout << " Failed to open " << fileName << endl; //オープン失敗

		} while (!file); //オープン成功で脱出

		try
		{
			while (!file.eof()) //ファイルから1行読み取り
			{
				if (fL)
				{
					cout << " 0 record(s) found."; 
					fL = false; //初回読み込み終了
				}
	
				getline(file, buffer);
				if (buffer.size() <= 1) //空白行処理
					continue;

				if(buffer.find(" ") == string::npos)
					throw "Failed to detect remote address.";
				else pos = buffer.find(" "); //最初の空白の位置を検索

				for (unsigned int count = 0; count < pos; count++)
					accessLog.aRemoteAdd[count] = buffer[count]; //最初の空白まで(=リモートアドレス)を取得

				accessLog.aRemoteAdd[pos] = '\0'; //NULL文字付加
				if (maxAdLen < pos)
					maxAdLen = pos;

				if(buffer.find("[") == string::npos)
					throw "Failed to detect date."; 
				else pos = buffer.find("["); //日時の開始地点を取得

				if(buffer.find("/") == string::npos)
					throw "Failed to detect day.";
				else pos2 = buffer.find("/"); //最初の'/'(=日)を取得

				for (unsigned int count = pos + 1; count < pos2; count++)
					cBuf[count - pos - 1] = buffer[count];

				cBuf[pos2 - pos - 1] = '\0'; //NULL文字付加

				//日付の文字列を数値に変換
				if (pos2 - pos == 3) //日が2桁の場合
					accessLog.aTime.day = 10 * (cBuf[0] - '0') + (cBuf[1] - '0');

				else if (pos2 - pos == 2) //日が1桁の場合
					accessLog.aTime.day = cBuf[0] - '0';

				else
					throw "Invalid day detected."; //不正な日
				//ここまで

				if (buffer.find("/", pos2 + 1) == string::npos)
					throw "Failed to detect month.";
				else pos = buffer.find("/", pos2 + 1); //二つ目の'/'(=月)を取得

				for (unsigned int count = pos2 + 1; count < pos; count++)
					cBuf[count - pos2 - 1] = buffer[count];

				cBuf[pos - pos2 - 1] = '\0'; //NULL文字付加

				accessLog.aTime.month = getMonth(cBuf); //文字列を数値に変換

				for (unsigned int count = 0; count < 4; count++)
					cBuf[count] = buffer[count + pos + 1]; //年を取得(4桁と仮定)

				cBuf[4] = '\0'; //NULL文字付加
				accessLog.aTime.year = 1000 * (cBuf[0] - '0') + 100 * (cBuf[1] - '0') + 10 * (cBuf[2] - '0') + (cBuf[3] - '0');
				//文字列を数値に変換

				if(buffer.find(":") == string::npos)
					throw "Failed to detect hour.";
				else pos = buffer.find(":");

				if(buffer.find(":", pos + 1) == string::npos)
					throw "Failed to detect hour.";
				else pos2 = buffer.find(":", pos + 1); //二つの':'(="時")を取得

				for (unsigned int count = pos + 1; count < pos2; count++)
					cBuf[count - pos - 1] = buffer[count];

				cBuf[pos2 - pos - 1] = '\0'; //NULL文字付加

				//時間の文字列を数値に変換
				if (pos2 - pos == 3) //時が2桁の場合
					accessLog.aTime.hour = 10 * (cBuf[0] - '0') + (cBuf[1] - '0');

				else if (pos2 - pos == 2) //時が1桁の場合
					accessLog.aTime.hour = cBuf[0] - '0';

				else
					throw "Invalid day detected."; //不正な時
				//ここまで

				//日付指定に関する処理
				if (aSD && accessLog.aTime < sD)
					continue; //開始日より前

				if (aED && eD < accessLog.aTime)
					continue; //終了日より後

				//時間帯別表示への追加
				tBuf.aTime = accessLog.aTime;
				tBuf.count = 1;
				tf = true;
				tNumElem = tAdCount.size();

				for (unsigned int count = 0; count < tNumElem; count++)
				{
					if (tAdCount[count].aTime == tBuf.aTime)
					{
						tAdCount[count].count += tBuf.count;
						tf = false;
						break; //同一の時間帯ははひとつに
					}
				}

				if (tf)
					tAdCount.push_back(tBuf);
				//ここまで

				//リモートアドレス別表示への追加
				coBuf.cRemoteAdd = accessLog.aRemoteAdd;
				coBuf.count = 1;
				tf = true;
				cNumElem = cAdCount.size();

				for (unsigned int count = 0; count < cNumElem; count++)
				{
					if (cAdCount[count].cRemoteAdd == coBuf.cRemoteAdd)
					{
						cAdCount[count].count += coBuf.count;
						tf = false;
						break; //同一のリモートアドレスはひとつに
					}
				}

				if (tf)
					cAdCount.push_back(coBuf);
				//ここまで
				cout << "\r " << ++rCount << " record(s) found."; //レコード追加成功→カウント増加
				//次のデータを読み取り
			}
		}
		catch (char* errMsg) //例外処理
		{
			//ファイル形式に関するエラー
			cout << "\n There was an error while processing the file " << fileName << ". \n"
			 	 << " Description: " << errMsg << endl
				 << " Please check the log file to ensure it is correct. \n";
			file.close();
			cout << endl;
			continue; //次のファイルへ
		}
		catch (...)
		{
			//その他のエラー
			cout << "\n There was an error while processing the file " << fileName << ". \n"
				 << " Please check the log file to ensure that it is correct. \n";
			system("Pause");
			return EXIT_FAILURE;
		}
		cout << endl << endl;
		file.close(); //ファイルクローズ
	}

	//時間帯別表示の前処理
	sort(tAdCount.begin(), tAdCount.end()); 
	tNumElem = tAdCount.size();

	cNumElem = cAdCount.size(); //リモートアドレス別表示の前処理

	cout << " Processing completed.";

	if (tNumElem == 0 || cNumElem == 0) //レコード0件
	{
		cout << "\n No record found.\n ";
		system("Pause");
		return EXIT_SUCCESS;
	}
	else do //レコード1件以上
	{
		cout << "\n Please select how to see the data.\n"
			 << " T ... List # of requests for every hour\n"
			 << " R ... List every remote address (sort with address)\n"
			 << " F ... List every remote address (sort with frequency)\n"
			 << " E ... Terminate the program\n"
			 << " Choose: ";
		cin  >> cBuf[0];
		cin.ignore(4096, '\n');
		cBuf[0] = toupper(cBuf[0]);

		if (cBuf[0] == 'T')
		{
			//時間帯別表示
			cout << "\n # of requests for every hour\n"
				 << "=================================\n";

			for (unsigned int count = 0; count < tNumElem; count++)
				cout << " "
					 << setw(4) << tAdCount[count].aTime.year << "/"
					 << setw(2) << tAdCount[count].aTime.month << "/"
					 << setw(2) << tAdCount[count].aTime.day << " "
					 << setw(2) << tAdCount[count].aTime.hour << ":00-"
					 << setw(2) << tAdCount[count].aTime.hour << ":59 "
					 << setw(9) << tAdCount[count].count << endl;

			cout << "=================================\n";
			system("pause");
			cout << endl;
		}

		else if (cBuf[0] == 'R')
		{
			//リモートアドレス別表示(アドレス順)
			sort(cAdCount.begin(), cAdCount.end(), countAdd::adPrefLT);

			cout << "\n List of every remote address (sort with address)\n";
			for (unsigned int count = 0; count < maxAdLen + 9; count++) //長さ調節
				cout << "=";
			cout << "\n";

			for (unsigned int count = 0; count < cNumElem; count++)
				cout << " "
					 << left  << setw(maxAdLen) << cAdCount[count].cRemoteAdd << " "
					 << right << setw(7) << cAdCount[count].count << endl;

			for (unsigned int count = 0; count < maxAdLen + 9; count++) //長さ調節
				cout << "=";
			cout << "\n ";
			system("pause");
			cout << endl;
		}

		else if (cBuf[0] == 'F')
		{
			//リモートアドレス別表示(アクセス数順)
			sort(cAdCount.begin(), cAdCount.end(), countAdd::coPrefRT);

			cout << "\n List of every remote address (sort with frequency)\n";
			for (unsigned int count = 0; count < maxAdLen + 9; count++) //長さ調節
				cout << "=";
			cout << "\n";

			for (unsigned int count = 0; count < cNumElem; count++)
				cout << " "
					 << left  << setw(maxAdLen) << cAdCount[count].cRemoteAdd << " "
					 << right << setw(7)  << cAdCount[count].count << endl;

			for (unsigned int count = 0; count < maxAdLen + 9; count++) //長さ調節
				cout << "=";
			cout << "\n ";
			system("pause");
			cout << endl;
		}
	} while (cBuf[0] != 'E'); //ユーザが終了を選択したら脱出

	//プログラム終了
    return EXIT_SUCCESS;
}

int getMonth(char* mChar)
{
	string sBuf(mChar);
	if (sBuf == "Jan") return 1;
	else if (sBuf == "Feb") return 2;
	else if (sBuf == "Mar") return 3;
	else if (sBuf == "Apr") return 4;
	else if (sBuf == "May") return 5;
	else if (sBuf == "Jun") return 6;
	else if (sBuf == "Jul") return 7;
	else if (sBuf == "Aug") return 8;
	else if (sBuf == "Sep") return 9;
	else if (sBuf == "Oct") return 10;
	else if (sBuf == "Nov") return 11;
	else if (sBuf == "Dec") return 12;
	else throw "Invalid month detected.";
}
