#include <windows.h>
#include <tchar.h>
#include <conio.h>

#define GBYTE 1073741824.0
#define MBYTE 1048576.0
#define KBYTE 1024.0

VOID PrintVolumeInfo(WCHAR *Volume);

int main(int argc, char **argv)
{
	WCHAR buf[100];
	DWORD len = GetLogicalDriveStrings(100, buf);
	for (WCHAR *path = buf; *path; path += wcslen(path)+1)
	{
		if (GetDriveType(path) != DRIVE_FIXED)
			continue;
		PrintVolumeInfo(path);
	}
	_getch();
	return 0;
}

VOID PrintVolumeInfo(WCHAR *Volume)
{
	WCHAR FileSysName[256];
	DWORD FileSysFlags;
	DWORD SectorsPerCluster;
	DWORD BytesPerSector;
	DWORD NumberOfFreeClusters;
	DWORD TotalNumberOfClusters;
	        
	BOOL result = GetVolumeInformation(Volume, NULL, 256, NULL, NULL,
										&FileSysFlags, FileSysName, 256);
	if (result)
	{
GetDiskFreeSpace (Volume, &SectorsPerCluster, &BytesPerSector, 
		&NumberOfFreeClusters, &TotalNumberOfClusters);
_tprintf(TEXT("Logical disk %s "), Volume);
_tprintf(TEXT("\nFile system: %s"), FileSysName);
_tprintf(TEXT("\nBytes per sector: %d"), BytesPerSector);
_tprintf(TEXT("\nSectors per cluster: %d"), SectorsPerCluster);
_tprintf(TEXT("\nNumber of free clusters: %d"), NumberOfFreeClusters);
_tprintf(TEXT("\nTotal number of clusters: %d\n"), TotalNumberOfClusters);
_tprintf(TEXT("\nTotal space: %.2lf GB"), TotalNumberOfClusters / GBYTE * SectorsPerCluster * BytesPerSector);
_tprintf(TEXT("\nFree space: %.2lf GB\n"), NumberOfFreeClusters / GBYTE * SectorsPerCluster * BytesPerSector);
_tprintf(TEXT("\nFree space(%s): %.2f%s\n"), "%", 100.0 * NumberOfFreeClusters / TotalNumberOfClusters, "%");
_tprintf(TEXT("-----------------------------------------------------------------\n"));
	}
}
