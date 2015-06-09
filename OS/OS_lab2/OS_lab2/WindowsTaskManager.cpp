#include "WindowsTaskManager.h"

void WindowsTaskManager::getAllProcesses () {
	// Get the list of process identifiers.
	ulong aProcesses[1024], cbNeeded, cProcesses;
	if (!EnumProcesses(aProcesses, sizeof(aProcesses), &cbNeeded))
		return;
	// Calculate how many process identifiers were returned.
	cProcesses = cbNeeded / sizeof(ulong);
	// Clearing
	processes.clear();
	// Print the name and process identifier for each process.
	for (unsigned int i = 0; i < cProcesses; ++i) {
		getProcessNameAndId (aProcesses[i]);
		getAccountInfo(aProcesses[i]);
	}
}

void WindowsTaskManager::getProcessNameAndId (ulong process_id) {
	char szProcessName[MAX_PATH] = "unknown";
	// Get a handle to the process.
	HANDLE hProcess = OpenProcess(	PROCESS_QUERY_INFORMATION |
									PROCESS_VM_READ,
									FALSE, 
									process_id );
	// Get the process name.
	if (NULL != hProcess ) {
		HMODULE hMod;
		ulong cbNeeded;
		if (EnumProcessModules(hProcess, &hMod, sizeof(hMod), &cbNeeded)) {
			GetModuleBaseName(hProcess, hMod, szProcessName, sizeof(szProcessName));
		} else { 
			return;
		}
	} else {
		return;
	}

	Process proc;
	proc.name = std::string(szProcessName);
	proc.id = process_id;
	processes.push_back(proc);

	CloseHandle(hProcess);
}

void WindowsTaskManager::outputSystemInfo () {
	SYSTEM_INFO siSysInfo;
	// Copying the hardware information to the SYSTEM_INFO structure. 
	GetSystemInfo(&siSysInfo); 
	std::cout << "Page size: "					<< siSysInfo.dwPageSize				<< " kb"	<< std::endl; 
	
	MEMORYSTATUS mem_status;
	GlobalMemoryStatus (&mem_status);
	std::cout << "Total physical memory: "		<< kb2mb(mem_status.dwTotalPhys)	<< " mb"	<< std::endl;
	std::cout << "Available physical memory: "	<< kb2mb(mem_status.dwAvailPhys)	<< " mb"	<< std::endl;
	std::cout << "Total page file: "			<< kb2mb(mem_status.dwTotalPageFile)<< " mb"	<< std::endl;
	std::cout << "Available page file: "		<< kb2mb(mem_status.dwAvailPageFile)<< " mb"	<< std::endl;
	std::cout << "Total virtual memory: "		<< kb2mb(mem_status.dwTotalVirtual)	<< " mb"	<< std::endl;
	std::cout << "Available virtual memory: "	<< kb2mb(mem_status.dwAvailVirtual)	<< " mb"	<< std::endl;
}

void WindowsTaskManager::clearConsole		() {
	system("cls");
}

void WindowsTaskManager::getAccountInfo		(ulong process_id) {
	ulong dwSize = 0, dwResult = 0;
    HANDLE hToken;
    PTOKEN_GROUPS pGroupInfo;
    SID_NAME_USE SidType;
    char lpName[256];
    char lpDomain[256];
    PSID pSID = NULL;
    SID_IDENTIFIER_AUTHORITY SIDAuth = SECURITY_NT_AUTHORITY;
       
    // Open a handle to the access token for the calling process.
    OpenProcessToken(GetCurrentProcess(), TOKEN_QUERY, &hToken);
    
	// Call GetTokenInformation to get the buffer size.
    GetTokenInformation(hToken, TokenGroups, NULL, dwSize, &dwSize);

    // Allocate the buffer.
    pGroupInfo = (PTOKEN_GROUPS) GlobalAlloc( GPTR, dwSize );

    // Call GetTokenInformation again to get the group information.
	GetTokenInformation(hToken, TokenGroups, pGroupInfo, dwSize, &dwSize);
    
    // Create a SID for the BUILTIN\Administrators group.
    AllocateAndInitializeSid(&SIDAuth, 2, SECURITY_BUILTIN_DOMAIN_RID, DOMAIN_ALIAS_RID_ADMINS, 0, 0, 0, 0, 0, 0, &pSID); 
  
    // Loop through the group SIDs looking for the administrator SID.
    for(int i = 0, size = pGroupInfo->GroupCount; i < size; ++i) {
        if (EqualSid(pSID, pGroupInfo->Groups[i].Sid)) {
            // Lookup the account name and print it.
            dwSize = 256;
            LookupAccountSid(NULL, pGroupInfo->Groups[i].Sid, lpName, &dwSize, lpDomain, &dwSize, &SidType);
			//std::cout << "Current user is a member of the " << lpDomain << " \\ " << lpName << " group" << std::endl;

			if (processes.size()) {
				processes[processes.size() - 1].accountDomainName = std::string(lpDomain);
				processes[processes.size() - 1].accountGroupName = std::string(lpName);
			}
			// Find out whether the SID is enabled in the token.
            /*
			if (pGroupInfo->Groups[i].Attributes & SE_GROUP_ENABLED)
                std::cout << "The group SID is enabled." << std::endl;
            else if (pGroupInfo->Groups[i].Attributes & SE_GROUP_USE_FOR_DENY_ONLY)
                std::cout << "The group SID is a deny-only SID." << std::endl;
            else 
                std::cout << "The group SID is not enabled." << std::endl;
			*/
        }
    }

    if (pSID)
        FreeSid(pSID);
    if ( pGroupInfo )
        GlobalFree( pGroupInfo );
}