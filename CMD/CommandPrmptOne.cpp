
/*
	CommandPrmpt_One.cpp
	프로그램 설명: 명령 프롬프트의 골격
*/
#include <iostream>
#include <cstdlib>
#include <tchar.h>
#include <locale.h>
#include <Windows.h>


#define STR_LEN		256
#define CMD_TOKEN_NUM	10

TCHAR ERROR_CMD[] = _T("'%s'은(는) 실행할 수 있는 프로그램이 아닙니다. \n");
TCHAR PROCESS_LAUNCHED[] = _T("'%s' 이(가) 실행되었습니다. \n");

int CmdProcessing(int a);
TCHAR* StrLower(TCHAR*);
BOOL MakeStartBuffer(void);

// notepad 실행
//void Notepad(); -> 문제: 명령어가 많아짐


TCHAR cmdString[STR_LEN];
TCHAR cmdTokenList[CMD_TOKEN_NUM][STR_LEN];
TCHAR seps[] = _T(" ,\t\n");
TCHAR* context = nullptr;
TCHAR cmdBuffer[STR_LEN];
TCHAR space[] = _T(" ");
typedef enum { ARGCON = 1, ARGCOFF = 2};
int _tmain(int argc, TCHAR* argv[])
{
	//한글 입력 가능케 하기 위해
	_tsetlocale(LC_ALL, _T("Korean"));
	_tprintf(_T("나의 CMD \n01.21: v1.1, start, echo추가\n\n"));
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
			_fputts(_T("명령어 처리를 종료합니다. \n"), stdout);
			return 0;
		}
	}
	while (1)
	{
		isExit = CmdProcessing(ARGCOFF);
		if (isExit == TRUE)
		{
			_fputts(_T("명령어 처리를 종료합니다. \n"), stdout);
			break;
		}
	}
	return 0;
}

/*
함수: TCHAR int CmdProcessing(void)
기능: 명령어를 입력 받아서 해당 명령어에 지정되어 있는 기능을 수행한다.
	  exit가 입력되면 TRUE를 반환하고 이는 프로그램의 종료로 이루어진다.
*/

int CmdProcessing(int a)
{
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
		{	//에코 뒤의 문자열은 소문자 변환 하지 않음.
			_tcscpy_s(cmdTokenList[tokenNum++], token);
			token = _tcstok_s(NULL, seps, &context);
		}
		else {
			if (!_tcscmp(_T("echo"), StrLower(token)))
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


	// 버퍼 비우기
	for (DWORD i = 0; i < 256; i++)
		cmdBuffer[i] = 0;

	for (DWORD i = 0; i < 256; i++)
		cmdString[i] = 0;

	for (DWORD i = 0; i < 2560; i++)
		*(*cmdTokenList + i) = 0;

	return 0;
}
/*
함수: TCHAR* StrLower(TCHAR*)
기능: 문자열의 내에 존재하는 모든 대문자를 소문자로 변경한다.
	  변경된 문자열의 포인터를 반환한다.
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
함수 : BOOL MakeStartBuffer(void)
기능 : cmdString의 첫 인자 start를 cmd로 바꾼 문자열을 cmdBuffer에 넣는다.
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

/*
함수: void Notepad(void)
기능: 현재 디렉터리를 system32로 바꾸고  notepad.exe를 실행한다.
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