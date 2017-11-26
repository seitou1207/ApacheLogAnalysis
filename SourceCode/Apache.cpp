/*
�쐬��:         �{�� ���~(�݂₴�� �����Ƃ�/Seito Miyazaki)
�쐬�J�n��:     2017/11/24
�쐬�I����:     2017/11/26
�v���O������:   apache.exe
���[���A�h���X: seitou1207@gmail.com
�g�p�R���p�C��: Visual Studio Express 2015

������Ѓt�B�b�N�X�|�C���g�@�v���O���~���O�����ۑ�

*/

#include "stdafx.h"

using namespace std;

int getMonth(char*); //���̕����񁨐��l�ւ̕ϊ��p
void invalidDate() { cout << " Error: Invalid date.\n"; }; //�G���[���b�Z�[�W�\���p

int main()
{
	ifstream file; //���O�t�@�C������̎�荞�ݗp
	string fileName, buffer; //�t�@�C�����p�A�t�@�C������̈ꎞ��荞�ݗp
	aLog accessLog; //�t�@�C������̈ꎞ��荞�ݗp
	timeAdd tBuf; //����
	countAdd coBuf; //����
	vector<timeAdd> tAdCount;  //���ԑѕʕ\���p
	vector<countAdd> cAdCount; //�����[�g�A�h���X�ʕ\���p
	dayTime sD, eD; //�����w��p
	bool tf,		  //���[�v�Ǘ��Ȃ�
	 	 aSD = false, //�����w��p
		 aED = false, //�����w��p
		 fL = true;   //�e�t�@�C���̎�荞�݉񐔔��ʗp
	char cBuf[5]; //���[�U���͎�荞�ݗp
	unsigned int pos, pos2, //��؂�ʒu
				 tNumElem, cNumElem, //�v�f��
				 numFiles, //�t�@�C����
				 maxAdLen = 0, //�ő�̃A�h���X��
				 rCount; //�ǂݎ�背�R�[�h��

	cout << "=================================\n"
		 << " Apache Log Analysis Program\n"
		 << " Ver 1.0\n"
		 << " (c)2017 Seito Miyazaki\n"
		 << "=================================\n";

	//�����w��m�F
	do { //�J�n�����w��
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
					pos2 = buffer.find("/", pos + 1); //���'/'���擾

					if (pos < 2 || pos > 4) //�N��2-4��
					{
						invalidDate();
						tf = true;
					}

					else if (pos2 - pos == 1 || pos2 - pos > 3) //����1-2��
					{
						invalidDate();
						tf = true;
					}

					else if (buffer.size() - pos2 < 2 || buffer.size() - pos2 > 3) //����1-2��
					{
						invalidDate();
						tf = true;
					}

					else {
						//����������֘A�̏���
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
						//���֘A�̏��������܂�
						else
						{
							//�������猎�֘A�̏���
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
							//���֘A�̏��������܂�
							else 
							{
								//��������N�֘A�̏���
								for (unsigned int count = 0; count < pos; count++)
									cBuf[count] = buffer[count];
								cBuf[pos] = '\0';

								//�����񂩂�N�ւ̕ϊ�����
								if (pos == 4) //4�����͎�
									sD.year = 1000 * (cBuf[0] - '0') + 100 * (cBuf[1] - '0') + 10 * (cBuf[2] - '0') + (cBuf[3] - '0');

								else if (pos == 3) //3�����͎�(1900-2899�N)
								{
									if (cBuf[0] == '9') // 1900-1999�N
										sD.year = 1000 + 100 * (cBuf[0] - '0') + 10 * (cBuf[1] - '0') + (cBuf[2] - '0');

									else // 2000-2899�N
										sD.year = 2000 + 100 * (cBuf[0] - '0') + 10 * (cBuf[1] - '0') + (cBuf[2] - '0');
								}

								else if (cBuf[0] == '9' && cBuf[1] >= '5') //2�����͎�(1995-1999�N)
									sD.year = 1900 + 10 * (cBuf[0] - '0') + (cBuf[1] - '0');

								else //2�����͎�(2000-2094�N)
									sD.year = 2000 + 10 * (cBuf[0] - '0') + (cBuf[1] - '0');

								if (sD.day == 29 && sD.month == 2) //�[�N����
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
								//�N�֘A���������܂�
								//�ȉ����[�U�m�F
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
								tf��false�̂܂܂ł���΂��ׂẴe�X�g���N���A���Ă���̂Ń��[�v�E�o
								true�ɕς��ۂɂ�(���[�U�m�F������)�G���[���b�Z�[�W�\��
								*/
							}
						}
					}
				}
			}
		} while (tf);
		aSD = true; //�J�n�����w��ON
		sD.hour = 0;
	}

	do { //�I�������w��
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
					pos2 = buffer.find("/", pos + 1); //���'/'���擾

					if (pos < 2 || pos > 4) //�N��2-4��
					{
						invalidDate();
						tf = true;
					}

					else if (pos2 - pos == 1 || pos2 - pos > 3) //����1-2��
					{
						invalidDate();
						tf = true;
					}

					else if (buffer.size() - pos2 < 2 || buffer.size() - pos2 > 3) //����1-2��
					{
						invalidDate();
						tf = true;
					}

					else {
						//����������֘A�̏���
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
						//���֘A�̏��������܂�
						else
						{
							//�������猎�֘A�̏���
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
							//���֘A�̏��������܂�
							else
							{
								//��������N�֘A�̏���
								for (unsigned int count = 0; count < pos; count++)
									cBuf[count] = buffer[count];
								cBuf[pos] = '\0';

								//�����񂩂�N�ւ̕ϊ�����
								if (pos == 4) //4�����͎�
									eD.year = 1000 * (cBuf[0] - '0') + 100 * (cBuf[1] - '0') + 10 * (cBuf[2] - '0') + (cBuf[3] - '0');

								else if (pos == 3) //3�����͎�(1900-2899�N)
								{
									if (cBuf[0] == '9') // 1900-1999�N
										eD.year = 1000 + 100 * (cBuf[0] - '0') + 10 * (cBuf[1] - '0') + (cBuf[2] - '0');

									else // 2000-2899�N
										eD.year = 2000 + 100 * (cBuf[0] - '0') + 10 * (cBuf[1] - '0') + (cBuf[2] - '0');
								}

								else if (cBuf[0] == '9' && cBuf[1] >= '5') //2�����͎�(1995-1999�N)
									eD.year = 1900 + 10 * (cBuf[0] - '0') + (cBuf[1] - '0');

								else //2�����͎�(2000-2094�N)
									eD.year = 2000 + 10 * (cBuf[0] - '0') + (cBuf[1] - '0');

								if (eD.day == 29 && eD.month == 2) //�[�N����
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
								//�N�֘A���������܂�
								//�ȉ����[�U�m�F
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
								tf��false�̂܂܂ł���΂��ׂẴe�X�g���N���A���Ă���̂Ń��[�v�E�o
								true�ɕς��ۂɂ�(���[�U�m�F������)�G���[���b�Z�[�W�\��
								*/
							}
						}
					}
				}
			}
		} while (tf);
		aED = true; //�I�������w��ON
		eD.hour = 23;
	}
	//���t�w�肱���܂�

	//�t�@�C����荞��
	do
	{
		tf = false;
		cout << " How many files do you wish to process?: "; //�t�@�C�����m�F
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

	for (unsigned int fCount = 0; fCount < numFiles; fCount++) //�w��t�@�C�����̓ǂݍ��݊J�n
	{
		
		do
		{
			//������
			rCount = 0;
			fL = true;
			
			cout << " Enter the name of the file (#" << fCount + 1 << "/" << numFiles << "): ";
			getline(cin, fileName); //�t�@�C�����擾

			file.open(fileName.c_str()); //�t�@�C���I�[�v��

			if (!file)
				cout << " Failed to open " << fileName << endl; //�I�[�v�����s

		} while (!file); //�I�[�v�������ŒE�o

		try
		{
			while (!file.eof()) //�t�@�C������1�s�ǂݎ��
			{
				if (fL)
				{
					cout << " 0 record(s) found."; 
					fL = false; //����ǂݍ��ݏI��
				}
	
				getline(file, buffer);
				if (buffer.size() <= 1) //�󔒍s����
					continue;

				if(buffer.find(" ") == string::npos)
					throw "Failed to detect remote address.";
				else pos = buffer.find(" "); //�ŏ��̋󔒂̈ʒu������

				for (unsigned int count = 0; count < pos; count++)
					accessLog.aRemoteAdd[count] = buffer[count]; //�ŏ��̋󔒂܂�(=�����[�g�A�h���X)���擾

				accessLog.aRemoteAdd[pos] = '\0'; //NULL�����t��
				if (maxAdLen < pos)
					maxAdLen = pos;

				if(buffer.find("[") == string::npos)
					throw "Failed to detect date."; 
				else pos = buffer.find("["); //�����̊J�n�n�_���擾

				if(buffer.find("/") == string::npos)
					throw "Failed to detect day.";
				else pos2 = buffer.find("/"); //�ŏ���'/'(=��)���擾

				for (unsigned int count = pos + 1; count < pos2; count++)
					cBuf[count - pos - 1] = buffer[count];

				cBuf[pos2 - pos - 1] = '\0'; //NULL�����t��

				//���t�̕�����𐔒l�ɕϊ�
				if (pos2 - pos == 3) //����2���̏ꍇ
					accessLog.aTime.day = 10 * (cBuf[0] - '0') + (cBuf[1] - '0');

				else if (pos2 - pos == 2) //����1���̏ꍇ
					accessLog.aTime.day = cBuf[0] - '0';

				else
					throw "Invalid day detected."; //�s���ȓ�
				//�����܂�

				if (buffer.find("/", pos2 + 1) == string::npos)
					throw "Failed to detect month.";
				else pos = buffer.find("/", pos2 + 1); //��ڂ�'/'(=��)���擾

				for (unsigned int count = pos2 + 1; count < pos; count++)
					cBuf[count - pos2 - 1] = buffer[count];

				cBuf[pos - pos2 - 1] = '\0'; //NULL�����t��

				accessLog.aTime.month = getMonth(cBuf); //������𐔒l�ɕϊ�

				for (unsigned int count = 0; count < 4; count++)
					cBuf[count] = buffer[count + pos + 1]; //�N���擾(4���Ɖ���)

				cBuf[4] = '\0'; //NULL�����t��
				accessLog.aTime.year = 1000 * (cBuf[0] - '0') + 100 * (cBuf[1] - '0') + 10 * (cBuf[2] - '0') + (cBuf[3] - '0');
				//������𐔒l�ɕϊ�

				if(buffer.find(":") == string::npos)
					throw "Failed to detect hour.";
				else pos = buffer.find(":");

				if(buffer.find(":", pos + 1) == string::npos)
					throw "Failed to detect hour.";
				else pos2 = buffer.find(":", pos + 1); //���':'(="��")���擾

				for (unsigned int count = pos + 1; count < pos2; count++)
					cBuf[count - pos - 1] = buffer[count];

				cBuf[pos2 - pos - 1] = '\0'; //NULL�����t��

				//���Ԃ̕�����𐔒l�ɕϊ�
				if (pos2 - pos == 3) //����2���̏ꍇ
					accessLog.aTime.hour = 10 * (cBuf[0] - '0') + (cBuf[1] - '0');

				else if (pos2 - pos == 2) //����1���̏ꍇ
					accessLog.aTime.hour = cBuf[0] - '0';

				else
					throw "Invalid day detected."; //�s���Ȏ�
				//�����܂�

				//���t�w��Ɋւ��鏈��
				if (aSD && accessLog.aTime < sD)
					continue; //�J�n�����O

				if (aED && eD < accessLog.aTime)
					continue; //�I��������

				//���ԑѕʕ\���ւ̒ǉ�
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
						break; //����̎��ԑт͂͂ЂƂ�
					}
				}

				if (tf)
					tAdCount.push_back(tBuf);
				//�����܂�

				//�����[�g�A�h���X�ʕ\���ւ̒ǉ�
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
						break; //����̃����[�g�A�h���X�͂ЂƂ�
					}
				}

				if (tf)
					cAdCount.push_back(coBuf);
				//�����܂�
				cout << "\r " << ++rCount << " record(s) found."; //���R�[�h�ǉ��������J�E���g����
				//���̃f�[�^��ǂݎ��
			}
		}
		catch (char* errMsg) //��O����
		{
			//�t�@�C���`���Ɋւ���G���[
			cout << "\n There was an error while processing the file " << fileName << ". \n"
			 	 << " Description: " << errMsg << endl
				 << " Please check the log file to ensure it is correct. \n";
			file.close();
			cout << endl;
			continue; //���̃t�@�C����
		}
		catch (...)
		{
			//���̑��̃G���[
			cout << "\n There was an error while processing the file " << fileName << ". \n"
				 << " Please check the log file to ensure that it is correct. \n";
			system("Pause");
			return EXIT_FAILURE;
		}
		cout << endl << endl;
		file.close(); //�t�@�C���N���[�Y
	}

	//���ԑѕʕ\���̑O����
	sort(tAdCount.begin(), tAdCount.end()); 
	tNumElem = tAdCount.size();

	cNumElem = cAdCount.size(); //�����[�g�A�h���X�ʕ\���̑O����

	cout << " Processing completed.";

	if (tNumElem == 0 || cNumElem == 0) //���R�[�h0��
	{
		cout << "\n No record found.\n ";
		system("Pause");
		return EXIT_SUCCESS;
	}
	else do //���R�[�h1���ȏ�
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
			//���ԑѕʕ\��
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
			//�����[�g�A�h���X�ʕ\��(�A�h���X��)
			sort(cAdCount.begin(), cAdCount.end(), countAdd::adPrefLT);

			cout << "\n List of every remote address (sort with address)\n";
			for (unsigned int count = 0; count < maxAdLen + 9; count++) //��������
				cout << "=";
			cout << "\n";

			for (unsigned int count = 0; count < cNumElem; count++)
				cout << " "
					 << left  << setw(maxAdLen) << cAdCount[count].cRemoteAdd << " "
					 << right << setw(7) << cAdCount[count].count << endl;

			for (unsigned int count = 0; count < maxAdLen + 9; count++) //��������
				cout << "=";
			cout << "\n ";
			system("pause");
			cout << endl;
		}

		else if (cBuf[0] == 'F')
		{
			//�����[�g�A�h���X�ʕ\��(�A�N�Z�X����)
			sort(cAdCount.begin(), cAdCount.end(), countAdd::coPrefRT);

			cout << "\n List of every remote address (sort with frequency)\n";
			for (unsigned int count = 0; count < maxAdLen + 9; count++) //��������
				cout << "=";
			cout << "\n";

			for (unsigned int count = 0; count < cNumElem; count++)
				cout << " "
					 << left  << setw(maxAdLen) << cAdCount[count].cRemoteAdd << " "
					 << right << setw(7)  << cAdCount[count].count << endl;

			for (unsigned int count = 0; count < maxAdLen + 9; count++) //��������
				cout << "=";
			cout << "\n ";
			system("pause");
			cout << endl;
		}
	} while (cBuf[0] != 'E'); //���[�U���I����I��������E�o

	//�v���O�����I��
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
