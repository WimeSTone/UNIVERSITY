#pragma once

#include "ITaskManager.h"

class UnixTaskManager : public ITaskManager {
typedef ITaskManager inherited;

public:
	virtual void getAllProcesses		() {};
	virtual void outputProcessesInfo	() {};
	virtual void outputSystemInfo		() {};
};