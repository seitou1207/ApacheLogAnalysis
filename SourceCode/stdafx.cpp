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

//�ȉ��S�ď�����

dayTime::dayTime()
{
	hour = 0;
	day = 1;
	month = 1;
	year = 2017;
}

aLog::aLog() { strcpy_s(aRemoteAdd, "255.255.255.255"); }

timeAdd::timeAdd() { count = 0; }
