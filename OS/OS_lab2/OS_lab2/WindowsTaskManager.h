#pragma once

#include <iostream>
#include <windows.h>

#include "psapi.h"

#include "ITaskManager.h"

class WindowsTaskManager : public ITaskManager {
typedef ITaskManager inherited;

public:
	virtual void getAllProcesses		();
	virtual void outputSystemInfo		();
	virtual void clearConsole			();

private:
	void getProcessNameAndId			(ulong process_id);
	void getAccountInfo					(ulong process_id);
};