#include <stdio.h>
#include <Windows.h>
#include <tlhelp32.h>
#include <shlwapi.h>
//#pragma comment(lib, "Shlwapi.lib")
#pragma comment(	lib, "Shlwapi.lib")


DWORD memoryProcess(WCHAR* ExeTarget) {
	DWORD procID = 0;
	HANDLE getProcess = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
	PROCESSENTRY32 pe;
	pe.dwSize = sizeof(pe);
	BOOL p = Process32First(getProcess, &pe);
	while (p) {
		char* find =(char*)StrStrIW(pe.szExeFile, ExeTarget);
		if (find) {
			printf("the address is %d | process name %p\n", pe.th32ProcessID, pe.szExeFile);
			procID = pe.th32ProcessID;
			break;
		}

		p = Process32Next(getProcess, &pe);

	}

	CloseHandle(getProcess);
	return procID;

}


DWORD getDll(DWORD prcessID, WCHAR* nameModule) {
	HANDLE handleModule = CreateToolhelp32Snapshot(TH32CS_SNAPMODULE | TH32CS_SNAPMODULE32, prcessID);
	BYTE* modleBasAddress = 0;
	if (handleModule != INVALID_HANDLE_VALUE) {

		MODULEENTRY32 me;
		me.dwSize = sizeof(me);
		BOOL m = Module32First(handleModule, &me);
		while (m)
		{
			char* find = (char*)StrStrIW(me.szModule, nameModule);
			if (find) {
				printf("the module result is %ws | %p\n", me.szModule, me.modBaseAddr);
				modleBasAddress = (BYTE*)me.modBaseAddr;
				break;
			}

		}
		CloseHandle(handleModule);
	}

	return (DWORD)modleBasAddress;
}

void changeHealth(DWORD processId, DWORD moduleBasseAdd) {
	HANDLE openPr = OpenProcess(PROCESS_VM_READ | PROCESS_VM_WRITE | PROCESS_VM_OPERATION, TRUE, processId);
	if (openPr) {
		DWORD firstAddress = moduleBasseAdd + 0x17B0B8;
		DWORD first = NULL;
		ReadProcessMemory(openPr, (void*)firstAddress, &first, sizeof(firstAddress), NULL);
		DWORD sacandAddress = first + 0x000000EC;
		DWORD scand = NULL;
		ReadProcessMemory(openPr, (void*)sacandAddress, &scand, sizeof(sacandAddress), NULL);
		printf("health %d\n", scand);

		DWORD health2 = 150;
		WriteProcessMemory(openPr, (void*)sacandAddress, &health2, sizeof(health2), NULL);
		printf("the new value of health is %d\n", health2);
		CloseHandle(openPr);
	}
	else {
		printf(" failed to open process\n");
	}
}



int main() {
	WCHAR exeNam[] = L"xxxx.exe";
	DWORD processID = memoryProcess(exeNam);
	/*DWORD proc*/

	DWORD moduleBasseAdd = getDll(processID, exeNam);
	changeHealth(processID, moduleBasseAdd); 

	
	getchar();
	return 0; 
}