#include <iostream>
#include <windows.h>

#include "psapi.h"
#include "ITaskManager.h"

#ifdef Win
#include "WindowsTaskManager.h"
#else
#include "UnixTaskManager.h"
#endif

/*
1) ����������� ������ ���������� ��������� � ��������� �� ���������������(+), ����� ����������(+), ��������������� �������������, ��������� 
	��������, �������� ������� ������������ ����������, ����������� ������� ��������, �������� ���������� ������, ������������� ���������;
2)(+) ����������� ���������� ���������� ���������� �������: ����� ���������� ������(+), ����� ��������� ���������� ������(+), 
	������� ������ ����� ��������(+), ������ ����������� ��������(+);
3)(+) ����������� ���������� ������������ ����������
*/

int main (int argc, char **argv) {
	setlocale(LC_ALL, "Russian");

#ifdef Win
	WindowsTaskManager manager;
#else
	UnixTaskManager manager;
#endif;

	bool stop = false;
	while (!stop) {
		std::cout << std::endl;
		std::cout << "Processes info - 1" << std::endl;
		std::cout << "Memory info - 2" << std::endl;
		std::cout << "Exit - 3" << std::endl;
		int choice = 0;
		std::cin >> choice;
		
		manager.clearConsole ();

		switch (choice) {
		case 1:
			manager.getAllProcesses ();
			manager.outputProcessesInfo ();
			
			if (manager.outputActions ())
				break;
			manager.outputProcessesInfo ();

			break;
		case 2:
			manager.outputSystemInfo ();
			break;
		case 3:
			stop = true;
			break;
		default:
			std::cout << "Wrong input." << std::endl;
		}
	}
	
	std::cin.get();
	return 0;
}