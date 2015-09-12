#ifndef LIBTHREAD_H
#define LIBTHREAD_H

#include <vector>
#include <thread>
#include <list>
#include <utility>
#include <condition_variable>

typedef unsigned (*job)(void *);

class LibThread
{
public:
	LibThread();
	void Init(int nb = 4);
	void Await();
	void AwaitAll();
	unsigned int AddJob(job j, void *e);
	~LibThread();
	
private:
	enum e_state
	{
		RUNNING,
		PAUSE,
		END
	};

	typedef struct				s_job
	{
		job						j;
		void					*e;
		unsigned int			id;
	}							t_job;

	typedef struct 				data
	{
		std::thread 			thd;
		e_state					state;
		std::list<t_job>		tasks;
		int						nbTasks;
		std::mutex				mutex;
	}							t_data;

	int nb;
	int id;
	std::condition_variable cv;
	std::mutex mtx;
	std::vector<t_data*>	tab;
	void launcher(int);
	int takeTask(int);
};

#endif