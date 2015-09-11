#include "LibThread.hpp"
#include <iostream>
#include <unistd.h>

LibThread::LibThread()
{

}

void LibThread::launcher(int i)
{
	std::cout << i << std::endl;
	tab[i]->mutex.lock();
	std::cout << "Launch thread " << i << ".\n";
	while (tab[i]->state != END)
	{
		if (!tab[i]->nbTasks)
		{
			std::cout << "Thread " << i << " is waiting...\n";
			tab[i]->state = PAUSE;
			std::unique_lock<std::mutex> lck(mtx);
			tab[i]->mutex.unlock();
			cv.wait(lck);
			if (tab[i]->state != END)
				tab[i]->state = RUNNING;
			tab[i]->mutex.lock();
			std::cout << "Thread " << i << " wake up with .\n";
		}
		else
		{
			// auto task = tab[i]->tasks.front();
			tab[i]->tasks.pop_front();
			tab[i]->nbTasks--;
			// std::cout << "Executing task " << task << "...\n";
			sleep(1);
			// std::cout << "Done.\n";
		}
	}
}

void LibThread::Init(int nb)
{
	this->nb = nb;
	for (int i = 0; i < nb; ++i)
	{
		t_data *d = new t_data();
		d->thd = std::thread(&LibThread::launcher, this, i);
		tab.push_back(d);
	}
	sleep(1);
}

void LibThread::Await()
{
	for (int i = 0; i < nb; ++i)
	{
		if (tab[i]->state != PAUSE)
		{
			std::cout << "Waiting end of thread " << i << "...\n";
			tab[i]->mutex.lock();
			tab[i]->mutex.unlock();
		}
	}
	std::cout << "All thread sleeping.\n";
}

void LibThread::AwaitAll()
{

}

void LibThread::AddJob(int job)
{
	int cur = 0;
	int nbTasks = tab[0]->nbTasks;
	for (int i = 0; i < nb; ++i)
		if (tab[i]->nbTasks < nbTasks)
			cur = i;
	tab[cur]->tasks.push_back(job);
	tab[cur]->nbTasks++;
	cv.notify_all();
}

LibThread::~LibThread()
{
	for (int i = 0; i < nb; ++i)
	{
		tab[i]->state = END;
	}
	cv.notify_all();
	for (int i = 0; i < nb; ++i)
	{
		tab[i]->thd.join();
		delete tab[i];
	}
}
