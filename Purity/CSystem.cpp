#include "Purity.h"
#ifdef ENABLE_SYSTEM
#include <windows.h>

CSystem::CSystem(CHandler* m_chand)
{
	REGISTERCMD(this,&CSystem::specs,"sys","info", m_sysSpecs, CSystem);
}


unsigned __int64 CSystem::CycleCount()
{
   ULARGE_INTEGER t0;
   __asm
   {
      _emit  0x0f
      _emit  0x31
      MOV t0.LowPart, EAX
      MOV t0.HighPart, EDX
   }
   return *((__int64*)&t0);
}

void CSystem::__cpuid (int *CPUInfo, int InfoType)
{
    __asm
    {
        mov eax, InfoType
        cpuid
        mov esi, CPUInfo
        mov [esi], eax
        mov [esi+4], ebx
        mov [esi+8], ecx
        mov [esi+12], edx
    }
}
int CSystem::GetCpuSpeed()
{
   unsigned __int64 start, stop;
   unsigned int total;
   start = CycleCount();
   Sleep(1000);
   stop = CycleCount();
   stop = stop - start;
   total = (unsigned)(stop/1000000);
   return total;
}
void CSystem::specs(const msgBuffer *args)
{
	__int64 i64FreeBytes=0, i64TotalBytes=0;//, i64Uptime;
	MEMORYSTATUS memStatus;
	GlobalMemoryStatus (&memStatus);
	GetDiskFreeSpaceEx(NULL,NULL,(PULARGE_INTEGER)&i64TotalBytes,(PULARGE_INTEGER)&i64FreeBytes);
	if(args->ircProtocol)
		args->ircProtocol->ircRaw("PRIVMSG %s :CPU Speed: %dmhz RAM: %ldMB/%ldMB Storage:%dGB/%d GB\r\n" ,
					  args->args.at(2).c_str(), GetCpuSpeed(), memStatus.dwAvailPhys/1048576
					  ,memStatus.dwTotalPhys/1048576, i64FreeBytes/1073741824, i64TotalBytes/1073741824);
 
	/*

	  //get the thread number
   //int iThreadNum = (int)lpParams;

   // enable delay to prefent excess Flood
   //threads[iThreadNum].irc->bDelay = true;

   int iScreenX = 0, iScreenY = 0, iScreenBPP = 0, iScreenRefresh = 0;

   HDC hdcDesktop;
   HWND hwndDesktop;
   
   // get screen resolution
   hwndDesktop    = GetDesktopWindow();
   hdcDesktop     = GetDC(hwndDesktop);
   iScreenX       = GetDeviceCaps(hdcDesktop,HORZRES);
   iScreenY       = GetDeviceCaps(hdcDesktop,VERTRES);
   iScreenBPP     = GetDeviceCaps(hdcDesktop,BITSPIXEL);
   iScreenRefresh = GetDeviceCaps(hdcDesktop,VREFRESH);

   //release screen resolution
   ReleaseDC(hwndDesktop,hdcDesktop);
   
     // messsage resolution information
   printf("(Resolution): %dx%d %dBit %dHz\n", iScreenX, iScreenY, iScreenBPP, iScreenRefresh);


   // allocate buffer for cpu information
   char *szCpu = (char*)malloc(128);

   if (szCpu)
   {
      // get cpu information
      GetCPUInfo(szCpu);

      printf("(CPU): %s\n", szCpu); // messsage cpu information

      free(szCpu);
   }

   // alloc memory for memory info structure
   MEMORYSTATUS *mem = (MEMORYSTATUS*)malloc(sizeof(MEMORYSTATUS));
   if (mem)
   {
        //fill allocated memory with zero's
      ZeroMemory(mem,sizeof(MEMORYSTATUS));

      mem->dwLength = sizeof(MEMORYSTATUS);

      //get memory info
      GlobalMemoryStatus(mem);
      
      // messsage memory information
      printf("(Ram Memory): %dMB Total, %dMB Free %d%s In Use\n", mem->dwTotalPhys/ 1048576+1, mem->dwAvailPhys / 1048576+1,mem->dwMemoryLoad,"%");

      printf("(Page Memory): %dMB Total, %dMB Free\n", mem->dwTotalPageFile/ 1048576, mem->dwAvailPageFile / 1048576);     

      printf("(Virtual Memory): %dMB Total, %dMB Free\n", mem->dwTotalVirtual/ 1048576, mem->dwAvailVirtual / 1048576);
      
      // free allocated memory info
      free(mem);
   }

   // messsage uptime information
   printf("(Uptime): %s", TimeStats(GetTickCount() / 1000));

   /// alloc memory for operating system info structure
   OSVERSIONINFO *os = (OSVERSIONINFO*)malloc(sizeof(OSVERSIONINFO));
   if (os)
   {
      //fill allocated memory with zero's
      ZeroMemory(os,sizeof(OSVERSIONINFO));

      os->dwOSVersionInfoSize = sizeof(OSVERSIONINFO);

      //get operating system info
      GetVersionEx(os);
      
      //msg operating system information
      if (os->dwMajorVersion == 4 && !os->dwMinorVersion)
      {
         
         if (os->dwPlatformId == VER_PLATFORM_WIN32_WINDOWS)
            printf("(OS): %s\n", "95");

         if (os->dwPlatformId == VER_PLATFORM_WIN32_NT)
            printf("(OS): %s\n", "NT");
      }
      else if (os->dwMajorVersion == 4 && os->dwMinorVersion == 10)
         printf("(OS): Windows 98 %s (%d.%d, Build: %d)\n", os->szCSDVersion, os->dwMajorVersion, os->dwMinorVersion, os->dwBuildNumber);

      else if (os->dwMajorVersion == 4 && os->dwMinorVersion == 90)
         printf("(OS): Windows 98SE %s (%d.%d, Build: %d)", os->szCSDVersion, os->dwMajorVersion, os->dwMinorVersion, os->dwBuildNumber);
      
      else if (os->dwMajorVersion == 5 && !os->dwMinorVersion)
         printf("(OS): Windows 2000 %s (%d.%d, Build: %d)\n", os->szCSDVersion, os->dwMajorVersion, os->dwMinorVersion, os->dwBuildNumber);

      else if (os->dwMajorVersion == 5 && os->dwMinorVersion == 1)
         printf("(OS): Windows XP %s (%d.%d, Build: %d)\n", os->szCSDVersion, os->dwMajorVersion, os->dwMinorVersion, os->dwBuildNumber);

      else if (os->dwMajorVersion == 5 && os->dwMinorVersion == 2)
         printf("(OS): Windows 2003 %s (%d.%d, Build: %d)\n", os->szCSDVersion, os->dwMajorVersion, os->dwMinorVersion, os->dwBuildNumber);

      //free operating system info
      free(os);
   }
   // clear and exit thread;

	*/
 
}

void CSystem::GetCPUInfo(char *CPUBrandString)
{

    int CPUInfo[4] = {-1};
    unsigned   nExIds, i;

    // Calling __cpuid with 0x80000000 as the InfoType argument
    // gets the number of valid extended IDs.
    __cpuid(CPUInfo, 0x80000000);
    nExIds = CPUInfo[0];

    // Get the information associated with each extended ID.
    for (i=0x80000000; i<=nExIds; ++i)
    {
        __cpuid(CPUInfo, i);

        // Interpret CPU brand string and cache information.
        if  (i == 0x80000002)
            memcpy(CPUBrandString, CPUInfo, sizeof(CPUInfo));
        else if  (i == 0x80000003)
            memcpy(CPUBrandString + 16, CPUInfo, sizeof(CPUInfo));
        else if  (i == 0x80000004)
            memcpy(CPUBrandString + 32, CPUInfo, sizeof(CPUInfo));
    }
   
   return;
}

char *CSystem::TimeStats(int iTime)
{
   static char szUpTime[128] = {0};
   DWORD dwDays, dwHours, dwMins, dwSecs;
   
   dwDays  = iTime / 86400;
   dwHours = (iTime % 86400) / 3600;
   dwMins  = ((iTime % 86400) % 3600) / 60;
   dwSecs  = ((iTime % 86400) % 3600 %60);

   _snprintf(szUpTime,sizeof(szUpTime),"%dd %dh %dm %ds\n",dwDays, dwHours, dwMins, dwSecs);
   
   return (szUpTime);   
}

#endif