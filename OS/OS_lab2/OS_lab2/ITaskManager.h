#pragma once

#define Win

typedef unsigned long ulong;

#include <algorithm>
#include <vector>
#include <string>
#include <iostream>

struct Process {
	std::string name;
	int id;
	std::string accountDomainName;
	std::string accountGroupName;
	static inline bool nameComp			(const Process &proc1, const Process &proc2) {
		return proc1.name.compare(proc2.name) < 0;
	}
	static inline bool idComp			(const Process &proc1, const Process &proc2) {
		return proc1.id < proc2.id;
	}
};



class ITaskManager {
public:
	virtual void getAllProcesses		()				= 0;
	virtual void outputSystemInfo		()				= 0;
	virtual void clearConsole			()				= 0;

	void outputProcessesInfo			() {
		for (int i = 0, size = processes.size(); i < size; ++i) {
			std::cout << processes[i].id << " - " << processes[i].name.c_str() 
				<< "; Domain = " << processes[i].accountDomainName << " Group = " << processes[i].accountGroupName << std::endl;
		}	
	}

	bool outputActions					() {
		std::cout << std::endl;
		std::cout << "Actions : " << std::endl;
		std::cout << "Sort by name - 1 " << std::endl;
		std::cout << "Sort by id - 2" << std::endl;
		std::cout << "Update - 3" << std::endl;
		std::cout << "Go to main menu - 4" << std::endl;
		int choice = 0;
		std::cin >> choice;
		
		clearConsole();

		switch (choice) {
		case 1:
			sortByName();
			break;
		case 2:
			sortById();
			break;
		case 3:
			updateProcesses();
			break;
		case 4:
			return true;
			break;
		default:
			break;
		}

		return false;
	}

	inline void updateProcesses			()				{ 
		getAllProcesses(); 
		clearConsole(); 
		outputProcessesInfo(); 
		outputActions();
	}
public:
	inline	ulong kb2mb					(ulong kb)		{ return kb / 1024; }

public:
	inline	void sortByName				()				{ std::sort(processes.begin(), processes.end(), Process::nameComp); }
	inline	void sortById				()				{ std::sort(processes.begin(), processes.end(), Process::idComp); }

protected:
	std::vector<Process> processes;
};