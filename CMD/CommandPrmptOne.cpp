
/*
	CommandPrmpt_One.cpp
	���α׷� ����: ��� ������Ʈ�� ���
*/
#include <iostream>
#include <cstdlib>
#include <tchar.h>
#include <locale.h>
#include <Windows.h>
#include <TlHelp32.h>


#define STR_LEN		256
#define CMD_TOKEN_NUM	10

TCHAR ERROR_CMD[] = _T("'%s'��(��) ������ �� �ִ� ���α׷��� �ƴմϴ�. \n");
TCHAR PROCESS_LAUNCHED[] = _T("'%s' ��(��) ����Ǿ����ϴ�. \n");

int CmdProcessing(int a);
TCHAR* StrLower(TCHAR*);
BOOL MakeStartBuffer(void);

// notepad ����
//void Notepad(); -> ����: ��ɾ ������


TCHAR cmdString[STR_LEN];
TCHAR cmdTokenList[CMD_TOKEN_NUM][STR_LEN];
TCHAR seps[] = _T(" ,\t\n");
TCHAR* context = nullptr;
TCHAR cmdBuffer[STR_LEN];
TCHAR space[] = _T(" ");
typedef enum { ARGCON = 1, ARGCOFF = 2};
typedef enum {_INVALID_HANDLE_VALUE = 100, _PROCESS32_ERROR = 101, _NOT_FOUND = 102, _KILL_FAILED};

int InvokeLP();
int InvokeKP();

const TCHAR* error[] = { _T("_INVALID_HANDLE_VALUE"), _T("_PROCESS32_ERROR"), _T("_NOT_FOUND"), _T("_KILL_FAILED")};
int _tmain(int argc, TCHAR* argv[])
{
	//�ѱ� �Է� ������ �ϱ� ����
	_tsetlocale(LC_ALL, _T("Korean"));
	_tprintf(_T("���� CMD \n01.21: v1.1, start, echo�߰�\n\n"));
	_tprintf(_T("���� CMD \n02.12: v1.2, lp, kp�߰�\n\n"));
	int count = 1;
	while (argc > count)
	{
		_tcscat_s(cmdString, argv[count++]);
		_tcscat_s(cmdString, space);
	}
	DWORD isExit;
	if (argc > 1)
	{
		isExit = CmdProcessing(ARGCON);
		if (isExit == TRUE)
		{
			_fputts(_T("��ɾ� ó���� �����մϴ�. \n"), stdout);
			return 0;
		}
	}
	while (1)
	{
		isExit = CmdProcessing(ARGCOFF);
		if (isExit == TRUE)
		{
			_fputts(_T("��ɾ� ó���� �����մϴ�. \n"), stdout);
			break;
		}
	}
	return 0;
}

/*
�Լ�: TCHAR int CmdProcessing(void)
���: ��ɾ �Է� �޾Ƽ� �ش� ��ɾ �����Ǿ� �ִ� ����� �����Ѵ�.
	  exit�� �ԷµǸ� TRUE�� ��ȯ�ϰ� �̴� ���α׷��� ����� �̷������.
*/

int CmdProcessing(int a)
{
	int result = TRUE;
	if (a == ARGCON)
		goto ARGC_ON;
	_fputts(_T("Best command prompt>> "), stdout);
	_getts_s(cmdString);

ARGC_ON:
	BOOL isEcho = FALSE;
	TCHAR* token = _tcstok_s(cmdString, seps, &context);
	int tokenNum = 0;
	while (token != NULL)
	{
		if (isEcho == TRUE)
		{	//���� ���� ���ڿ��� �ҹ��� ��ȯ ���� ����.
			_tcscpy_s(cmdTokenList[tokenNum++], token);
			token = _tcstok_s(NULL, seps, &context);
		}
		else {
			if (!_tcscmp(_T("echo"), StrLower(token)))
				isEcho = TRUE;
			else if (!_tcscmp(_T("kp"), StrLower(token)))
				isEcho = TRUE;
			_tcscpy_s(cmdTokenList[tokenNum++], StrLower(token));
			token = _tcstok_s(NULL, seps, &context);
		}
	}

	if( !_tcscmp(cmdTokenList[0], _T("exit")))
	{
		return TRUE;
	}
	else if (!_tcscmp(cmdTokenList[0], _T("start")))
	{
		STARTUPINFO si = { 0, };
		PROCESS_INFORMATION pi;
		si.cb = sizeof(si);
		MakeStartBuffer();
		if (CreateProcess(
			NULL, cmdBuffer	, NULL, NULL, TRUE, CREATE_NEW_CONSOLE, NULL, NULL, &si, &pi
		)) {
			CloseHandle(pi.hProcess);
			CloseHandle(pi.hThread);
		}
		else
		{
			_tprintf_s(ERROR_CMD, cmdTokenList[0]);
		}
			
	}
	else if (!_tcscmp(cmdTokenList[0], _T("echo")))
	{

		DWORD cnt = 1;
		while (cnt < 10)
		{
			if (cmdTokenList[cnt] == nullptr)
				break;
			_tcscat_s(cmdBuffer, cmdTokenList[cnt++]);
			_tcscat_s(cmdBuffer, space);
		}
		_tprintf_s(_T("%s\n\n"), cmdBuffer);

	}
	else if (!_tcscmp(cmdTokenList[0], _T("lp")))
	{
		result = InvokeLP();
		if (result != TRUE)
		{
			_tprintf_s(_T("%d : %s\n"), result, error[result-100]);
		}
	}
	else if (!_tcscmp(cmdTokenList[0], _T("kp")))
	{
		result = InvokeKP();
		if (result != TRUE)
		{
			_tprintf_s(_T("%d : %s\n"), result, error[result - 100]);
		}
	}
	else
	{
		STARTUPINFO si = { 0, };
		PROCESS_INFORMATION pi;
		si.cb = sizeof(si);
		//SetCurrentDirectory(_T("C:\\WINDOWS\\system32"));
		bool isRun = CreateProcess(
			NULL, cmdTokenList[0], NULL, NULL, TRUE, CREATE_NEW_CONSOLE, NULL, NULL, &si, &pi
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


	// ���� ����
	for (DWORD i = 0; i < 256; i++)
		cmdBuffer[i] = 0;

	for (DWORD i = 0; i < 256; i++)
		cmdString[i] = 0;

	for (DWORD i = 0; i < 2560; i++)
		*(*cmdTokenList + i) = 0;

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
�Լ� : BOOL MakeStartBuffer(void)
��� : cmdString�� ù ���� start�� cmd�� �ٲ� ���ڿ��� cmdBuffer�� �ִ´�.
*/
BOOL MakeStartBuffer(void)
{
	BOOL success = FALSE;
	cmdBuffer[0] = _T('c');
	cmdBuffer[1] = _T('m');
	cmdBuffer[2] = _T('d');
	cmdBuffer[3] = _T(' ');
	cmdBuffer[4] = _T('\0');
	DWORD cnt = 1;
	while (cnt < 10)
	{
		if (cmdTokenList[cnt] == nullptr)
			break;
		_tcscat_s(cmdBuffer, cmdTokenList[cnt++]);
		_tcscat_s(cmdBuffer, space);
	}
	success = TRUE;

	return success;
}

static int InvokeLP()
{
	int count = 0;
	HANDLE hProcessSnap =
		CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
	if (hProcessSnap == INVALID_HANDLE_VALUE)
	{
		_tprintf_s(_T("CreateToolhelp32Snapshot error! "));
		return _INVALID_HANDLE_VALUE;
	}

	PROCESSENTRY32 pe32;
	pe32.dwSize = sizeof(pe32);

	if (!Process32First(hProcessSnap, &pe32))
	{
		_tprintf_s(_T("Process32First error! \n"));
		CloseHandle(hProcessSnap);
		return _PROCESS32_ERROR;
	}

	do {
		_tprintf_s(_T("%40s\t%5d\n"), pe32.szExeFile, pe32.th32ProcessID);
		count++;
	} while (Process32Next(hProcessSnap, &pe32));
	_tprintf_s(_T("Total Process Count: %d\n"), count);
	CloseHandle(hProcessSnap);
	return TRUE;
}

static int InvokeKP()
{
	int ProcessId = 0;
	HANDLE hProcessSnap =
		CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
	if (hProcessSnap == INVALID_HANDLE_VALUE)
	{
		_tprintf_s(_T("CreateToolhelp32Snapshot error! "));
		return _INVALID_HANDLE_VALUE;
	}

	PROCESSENTRY32 pe32;
	pe32.dwSize = sizeof(pe32);

	if (!Process32First(hProcessSnap, &pe32))
	{
		_tprintf_s(_T("Process32First error! \n"));
		CloseHandle(hProcessSnap);
		return _PROCESS32_ERROR;
	}
	
	do {
		if (_tcscmp(pe32.szExeFile, cmdTokenList[1]) == 0)
		{
			ProcessId = pe32.th32ProcessID;
			break;
		}
	} while (Process32Next(hProcessSnap, &pe32));
	if (ProcessId == 0)
	{
		CloseHandle(hProcessSnap);
		return _NOT_FOUND;
	}
	HANDLE hOpen = OpenProcess(PROCESS_ALL_ACCESS, FALSE, ProcessId);
	BOOL isKill = TerminateProcess(hOpen, -1);
	CloseHandle(hProcessSnap);
	CloseHandle(hOpen);
	if (isKill == FALSE)
		return _KILL_FAILED;
	return TRUE;
}