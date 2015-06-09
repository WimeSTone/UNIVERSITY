#include <windows.h>
#include <tchar.h>
#include <conio.h>

void PrintFileInfo(char *path);

int main(int argc, char **argv)
{
	if (argc != 2)
	{
		_tprintf(TEXT("Usage: lab3.exe fileName"));
		return 0;
	}
	PrintFileInfo(argv[1]);
	return 0;
}

void PrintFileInfo(char *path)
{
	_BY_HANDLE_FILE_INFORMATION fileInfo;
	SYSTEMTIME sysTime;
	HANDLE file; 
	
	file = CreateFileA(path, GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, OPEN_EXISTING, NULL);
	if (GetFileInformationByHandle(file, &fileInfo) == 0)
	{
		_tprintf(TEXT("GetFileInformationByHandle error\n"));
		CloseHandle(file);
	}
	
	FileTimeToSystemTime(&fileInfo.ftCreationTime, &sysTime);
	_tprintf(TEXT("Creation time: "));
	_tprintf(TEXT("%u.%u.%u,%u:%u:%u\n"), sysTime.wDay, sysTime.wMonth, 
				sysTime.wYear, sysTime.wHour, sysTime.wMinute, sysTime.wSecond);
	FileTimeToSystemTime(&fileInfo.ftLastAccessTime, &sysTime);
	_tprintf(TEXT("Access time: "));
	_tprintf(TEXT("%u.%u.%u,%u:%u:%u\n"), sysTime.wDay, sysTime.wMonth, 
				sysTime.wYear, sysTime.wHour, sysTime.wMinute, sysTime.wSecond);
	FileTimeToSystemTime(&fileInfo.ftLastWriteTime, &sysTime);
	_tprintf(TEXT("Last write time: "));
	_tprintf(TEXT("%u.%u.%u,%u:%u:%u\n"), sysTime.wDay, sysTime.wMonth, 
				sysTime.wYear, sysTime.wHour, sysTime.wMinute, sysTime.wSecond);
	_tprintf(TEXT("File size: %u bytes"), GetFileSize(file, NULL));
	CloseHandle(file);
}
