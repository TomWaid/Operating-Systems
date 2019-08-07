#include <unistd.h>
#include <sys/wait.h>
#include <iostream>
#include <queue>
#include <functional>
#include <vector>
using namespace std;

struct Process //creates processes
{
	int pid = 0; //PID id
	int exectime = 0;   // CPU Burst time required
	int priority = 0; // Priority of this process
	bool flag = false;
};

struct Compare //compares two processes
{
	bool operator()(const Process &lhs, const Process &rhs) const
	{
		return lhs.priority > rhs.priority;
	}

};

struct Config //configures processes associated with quantum  
{
	int quantum = 0;
	vector<Process> processes = {};
};

struct Event //creates processes associated with events
{
	int pid = 0;
	int exec_time = 0;
	int priority = 0;
	bool f = false;
};

struct Schedule //creates scheduler
{
	vector<Event> events = {};
};

	    int main()
		{
			auto config = [&] {
				Config config;
				std::cin >> config.quantum;
				int pid = 0;
				int exec_time = 0;
				int priority = 0;
				bool f = false;
				while (std::cin >> exec_time >> priority)
				{
					Process proc;
					proc.pid = pid;
					proc.exectime = exec_time;
					proc.priority = priority;
					proc.flag = f;
					config.processes.push_back(proc);
					++pid;
				}
				return config;
			}();

			auto list = [&] {
				priority_queue<Process, vector<Process>, Compare> list;
				for (auto const& proc : config.processes)
				{
					list.push(proc);
				}
				return list;
			}(); 

			Schedule schedule;
			while (!list.empty()) //checks if list is not empty
			{
				auto proc = list.top();
				list.pop();

				auto eve = Event();
				eve.pid = proc.pid;
				eve.priority = proc.priority;

				if ((proc.exectime - config.quantum) > 0) //checks if difference between execution time and quantum is greater than zero
				{
					eve.exec_time = config.quantum;
					eve.f = true;
					Process new_proc(proc);
					new_proc.exectime = (proc.exectime - config.quantum);
					list.push(new_proc);
				}
				else
				{
					eve.exec_time = proc.exectime;
					//eve.f = false;
				}

				schedule.events.push_back(eve);
			}
			cout << "Schedule:" << endl;
			for (auto&& e : schedule.events) 
			{
				std::cout << "(" << e.pid << ", " << e.exec_time << ", " << e.priority << ")," <<  std::endl;
			}		
			for (auto && e : schedule.events)
			{
				pid_t pid = 0;
				if ((pid = fork()) == 0)
				{
					cout << " Process " <<  e.pid << ":" << " Exection= " << e.exec_time <<"," <<  " Priority= " << e.priority << endl;
					sleep(e.exec_time);
					if(e.f == false) 
					{
					
						cout << "Process" << e.pid << "ends." << endl;
						
					}
					break;
				}
				else 
				{
					wait(NULL);
				}
			}
			
			return 0;
		}

		
	


