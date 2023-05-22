#define _CRT_SECURE_NO_WARNINGS
#define WIN32_LEAN_AND_MEAN
#include <iostream>
#include <Windows.h>
#include <wincrypt.h>
#include <thread>
#include <fstream>
#include "MemMan.h"





using namespace  std;

MemMan MemClass;

int Protect_process();


int PID = MemClass.getProcess("GitHubDesktop.exe"); // or your Game


ofstream fout("dll_list.txt", ios::app);

int dllF()
{
	while (true)
	{

		fout;
		// Получаем дескриптор процесса
		HANDLE hProcess = OpenProcess(PROCESS_ALL_ACCESS, FALSE, PID);
		if (hProcess == NULL)
		{
			std::cout << "Error: cannot open process" << std::endl;
			return 1;
		}

		HMODULE hMods[1024];
		DWORD cbNeeded;


		for (int i = 0; i < (sizeof(HMODULE)); i++)
		{
			// Получаем путь к каждой загруженной DLL
			TCHAR szModName[MAX_PATH];

			std::cout << "DLL " << szModName << " was loaded" << std::endl;

			if (fout.is_open())
			{



				fout << "DLL " << szModName << " was loaded" << std::endl;
			}
		}
		fout.close();
		// Закрываем дескриптор процесса
		CloseHandle(hProcess);

		Sleep(4500);
	}
}








int main()
{
	setlocale(LC_ALL, "Russian");
	thread th(dllF);
	thread th1(Protect_process);




	th.join();
	th1.join();

	//cin >> NULL;

	while (true)
	{
		Sleep(6);
	}
	return 0;
}

int Protect_process()
{
	HANDLE hProcess = OpenProcess(PROCESS_ALL_ACCESS, FALSE, PID);
	DWORD dwPriorityClass = GetPriorityClass(hProcess);

	// Устанавливаем процессу максимальный приоритет
	SetPriorityClass(hProcess, REALTIME_PRIORITY_CLASS);

	// Устанавливаем процессу защиту от вмешательства других программ
	SetProcessAffinityMask(hProcess, 1);

	// Защищаем процесс от завершения другими программами
	SetProcessShutdownParameters(0x100, SHUTDOWN_NORETRY);

	// Защищаем процесс от изменения приоритета другими программами
	HANDLE hToken;
	TOKEN_PRIVILEGES tp;
	OpenProcessToken(hProcess, TOKEN_ADJUST_PRIVILEGES | TOKEN_QUERY, &hToken);
	LookupPrivilegeValue(NULL, SE_DEBUG_NAME, &tp.Privileges[0].Luid);
	tp.PrivilegeCount = 1;
	tp.Privileges[0].Attributes = SE_PRIVILEGE_ENABLED;
	AdjustTokenPrivileges(hToken, FALSE, &tp, 0, NULL, NULL);

	// Восстанавливаем приоритет процесса
	SetPriorityClass(hProcess, dwPriorityClass);

	if (hProcess)
	{
		cout << "\nprocess protected!\n";
	}


	return 0;
}
