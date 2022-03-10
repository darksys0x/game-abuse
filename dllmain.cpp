// dllmain.cpp : Defines the entry point for the DLL application.
#include "pch.h"
#include <stdio.h>
#include <Psapi.h>
#include <stdlib.h>

//read the [player object from data section
//140 - f0 = offsie of gun = 1
struct Player
{
    char padding[0xEC]; // ofset = 0
    DWORD health; // offset = 0xEC
    char padding2[0x2c]; // offset = 0xF0
    DWORD firearmStore; // offset = 0x11C
    char padding3[0x20]; 
    DWORD gun; // offset = 0x140
   
 
    


};

void theGun() {
    HANDLE Exe_baceAddress = GetModuleHandle(L"xxx.exe");
    if (Exe_baceAddress) {
        printf("getBaseAddress\n");

        DWORD objectAddress =  *(DWORD*) ((DWORD)Exe_baceAddress + 0x187C0C);
        Player* p = (Player*)objectAddress;
        p->gun = 0x64;
        printf("the gun bing %u | %d", p->gun, p->gun);
    }

}

void healthAddress() {
    HANDLE baseAddressOf_exe = GetModuleHandle(L"xxx.exe");
    if (baseAddressOf_exe) {
        printf("get Bases Address Of health\n");
        DWORD objctAddress = *(DWORD*)((DWORD)baseAddressOf_exe + 0x187C0C);
        Player* p = (Player*)objctAddress;
        p->health = 0x14;
        printf("health has been changed to = %u\n", p->health);
    }
}


void DisplayConsole() {
    if (AllocConsole())
    {
        freopen("CONIN$", "r", stdin);
        freopen("CONOUT$", "w", stdout);
        freopen("CONOOUT$", "w", stderr);
    }
}


void firearmStore() {
    HMODULE baseAddress = GetModuleHandle(L"xxx.exe");
    if (baseAddress) {
        printf("Get firearmStore address = %p\n", baseAddress);
        DWORD objectAddress = *(DWORD*)((DWORD)baseAddress + 0x17B0B8);
        //*(DWORD*)(objectAddress + 0x11C) = 1000;
        Player* p = (Player*)objectAddress;
        p->firearmStore = 0x3E8;
        printf("stor change %u\n", p->firearmStore);
    }
}


BOOL APIENTRY DllMain(HMODULE hModule,
    DWORD  ul_reason_for_call,
    LPVOID lpReserved
)
{
    switch (ul_reason_for_call)
    {
    case DLL_PROCESS_ATTACH:
    {
        DisplayConsole();
        theGun();
        healthAddress();
        firearmStore();

        break;
    }
    case DLL_THREAD_ATTACH:
    case DLL_THREAD_DETACH:
    case DLL_PROCESS_DETACH:
        break;
    }
    return TRUE;
}

