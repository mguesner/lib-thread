#ifndef LIBTHREAD_H
#define LIBTHREAD_H

#include <vector>
#include <thread>
#include <list>
#include <condition_variable>

class LibThread
{
public:
	LibThread();
	void Init(int nb = 8);
	void Await();
	void AwaitAll();
	void AddJob(int);
	~LibThread();
	
private:
	enum e_state
	{
		RUNNING,
		PAUSE,
		END
	};
	typedef struct 				data
	{
		std::thread 			thd;
		e_state					state;
		std::list<int>			tasks;
		int						nbTasks;
		std::mutex				mutex;
	}							t_data;
	int nb;
	std::condition_variable cv;
	std::mutex mtx;
	std::vector<t_data*>	tab;
	void launcher(int);
	int takeTask(int);
};

#endif