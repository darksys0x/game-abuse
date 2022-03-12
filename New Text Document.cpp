#include <stdio.h>
#include <Windows.h>
#include <tlhelp32.h>
#include <shlwapi.h>

#pragma comment(lib, "Shlwapi.lib")


//1. get procId of the target process.
DWORD getProccessesMemory(PCTSTR procID) {
	HANDLE hsnapProcess = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS,0);
	if (hsnapProcess != INVALID_HANDLE_VALUE) {
		PROCESSENTRY32 hamadProcEntry;
		hamadProcEntry.dwSize = sizeof(hamadProcEntry);
		BOOL p = Process32First(hsnapProcess, &hamadProcEntry);
		DWORD processiD = 0;
		while (p)
		{
			char* find = (char*)StrStrIW(hamadProcEntry.szExeFile, procID);
			//printf("the value of find %p\n", find);
			

			if (find) {
				printf("the value of find %c\n", *find);
				printf("the process id = %d | %ws\n", hamadProcEntry.th32ProcessID, hamadProcEntry.szExeFile);
				processiD = hamadProcEntry.th32ProcessID;
				break;
			}
			p = Process32Next(hsnapProcess, &hamadProcEntry);

			

		}
		CloseHandle(hsnapProcess);
		return processiD;
	}
}

//2. get module baceAddress 

BYTE getModuleBasAddress(DWORD procID, PCTSTR moduleName) {
	HANDLE hamadModule = CreateToolhelp32Snapshot(TH32CS_SNAPMODULE | TH32CS_SNAPMODULE32, procID);
	MODULEENTRY32 me;
	me.dwSize = sizeof(me);
	BOOL m = Module32First(hamadModule, &me);
	BYTE* moduleAddress = 0;
	while (m)
	{
		char* find = (char*)StrStrIW(me.szModule, moduleName);

		if (find) {
			printf("*** found the module\n");
			printf("the module name = %ws | basAddress %x\n", me.szModule, me.modBaseAddr);
			moduleAddress = me.modBaseAddr;
			break;
		}
		m = Module32Next(hamadModule, &me);
	}
	CloseHandle(hamadModule);
	return *moduleAddress;
}


int main() {
	//funconallity of program:

	//1. get procId of the target process.
	DWORD procID = getProccessesMemory(L"xxx.exe");
	printf("the process ID is %d\n", procID);


	//2. get module baceAddress 
	/*char* h =(char*) */
	BYTE* h = (BYTE*)getModuleBasAddress(procID, L"xxx.exe");
	printf("the baseAddress is = %x ", *(BYTE*)h);



	
















	
	getchar();
	return 0;
}