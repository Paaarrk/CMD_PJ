
/*
	CommandPrmpt_One.cpp
	���α׷� ����: ��� ������Ʈ�� ���
*/
#include <iostream>
#include <cstdlib>
#include <tchar.h>
#include <locale.h>
#include <Windows.h>


#define STR_LEN		256
#define CMD_TOKEN_NUM	10

TCHAR ERROR_CMD[] = _T("'%s'��(��) ������ �� �ִ� ���α׷��� �ƴմϴ�. \n");
TCHAR PROCESS_LAUNCHED[] = _T("'%s' ��(��) ����Ǿ����ϴ�. \n");

int CmdProcessing(void);
TCHAR* StrLower(TCHAR*);

// notepad ����
//void Notepad(); -> ����: ��ɾ ������

int _tmain(int agrc, TCHAR* argv[])
{
	//�ѱ� �Է� ������ �ϱ� ����
	_tsetlocale(LC_ALL, _T("Korean"));

	DWORD isExit;
	while (1)
	{
		isExit = CmdProcessing();
		if (isExit == TRUE)
		{
			_fputts(_T("��ɾ� ó���� �����մϴ�. \n"), stdout);
			break;
		}
	}
	return 0;
}

TCHAR cmdString[STR_LEN];
TCHAR cmdTokenList[CMD_TOKEN_NUM][STR_LEN];
TCHAR seps[] = _T(" ,\t\n");
TCHAR* context = nullptr;

/*
�Լ�: TCHAR int CmdProcessing(void)
���: ��ɾ �Է� �޾Ƽ� �ش� ��ɾ �����Ǿ� �ִ� ����� �����Ѵ�.
	  exit�� �ԷµǸ� TRUE�� ��ȯ�ϰ� �̴� ���α׷��� ����� �̷������.
*/

int CmdProcessing(void)
{
	_fputts(_T("Best command prompt>> "), stdout);
	_getts_s(cmdString);

	TCHAR* token = _tcstok_s(cmdString, seps, &context);
	int tokenNum = 0;
	while (token != NULL)
	{
		_tcscpy_s(cmdTokenList[tokenNum++], StrLower(token));
		token = _tcstok_s(NULL, seps, &context);
	}

	if( !_tcscmp(cmdTokenList[0], _T("exit")))
	{
		return TRUE;
	}
	else if (!_tcscmp(cmdTokenList[0], _T("����")))
	{
		return TRUE;
	}
	else
	{
		STARTUPINFO si = { 0, };
		PROCESS_INFORMATION pi;
		si.cb = sizeof(si);
		//SetCurrentDirectory(_T("C:\\WINDOWS\\system32"));
		bool isRun = CreateProcess(
		NULL, cmdTokenList[0], NULL, NULL, TRUE, 0, NULL, NULL, &si, &pi
		);

		if (isRun == FALSE)
		{
			_tprintf_s(ERROR_CMD, cmdTokenList[0]);
		}
		else
		{
			_tprintf_s(PROCESS_LAUNCHED, cmdTokenList[0]);
			CloseHandle(pi.hProcess);
			CloseHandle(pi.hThread);
		}
	}
	return 0;
}

/*
�Լ�: TCHAR* StrLower(TCHAR*)
���: ���ڿ��� ���� �����ϴ� ��� �빮�ڸ� �ҹ��ڷ� �����Ѵ�.
	  ����� ���ڿ��� �����͸� ��ȯ�Ѵ�.
*/
TCHAR* StrLower(TCHAR* pStr)
{
	TCHAR* ret = pStr;

	while (*pStr)
	{
		if (_istupper(*pStr))
			*pStr = _totlower(*pStr);
		pStr++;
	}

	return ret;
}

/*
�Լ�: void Notepad(void)
���: ���� ���͸��� system32�� �ٲٰ�  notepad.exe�� �����Ѵ�.
*/
//void Notepad()
//{
//	STARTUPINFO si = { 0, };
//	PROCESS_INFORMATION pi;
//	si.cb = sizeof(si);
//
//	TCHAR command[] = _T("notepad.exe");
//	SetCurrentDirectory(_T("C:\\WINDOWS\\system32"));
//
//	ZeroMemory(&pi, sizeof(pi));
//	CreateProcess(
//		NULL, command, NULL, NULL, TRUE, 0, NULL, NULL, &si, &pi
//	);
//}