#include "lab3.h"
#include <windows.h> 
#include <stdio.h>

using namespace std;

#define THREADCOUNT 9 

HANDLE Thread[THREADCOUNT];
HANDLE Mutex;
HANDLE SemB, SemC, SemE;
DWORD ThreadID;

unsigned int lab3_thread_graph_id()
{
	return 8;
}

const char* lab3_unsynchronized_threads()
{
	return "deg";
}

const char* lab3_sequential_threads()
{
	return "bce";
}

char const* texts[] = { "a", "b", "c", "d", "e", "f", "g", "h", "i" };

DWORD WINAPI threads_adfi(LPVOID text)
{
	for (int i = 0; i < 3; ++i)
	{
		WaitForSingleObject(Mutex, INFINITE);
		cout << (char const*)text << flush;
		ReleaseMutex(Mutex);
		computation();
	}
	return 0;
}

DWORD WINAPI thread_b(LPVOID text)
{

	for (int i = 0; i < 3; ++i)
	{
		WaitForSingleObject(SemB, INFINITE);
		WaitForSingleObject(Mutex, INFINITE);
		cout << (char const*)text << flush;
		ReleaseMutex(Mutex);
		computation();
		ReleaseSemaphore(SemC, 1, NULL);
	}

	return 0;
}

DWORD WINAPI thread_c(LPVOID text)
{
	for (int i = 0; i < 3; ++i)
	{
		WaitForSingleObject(SemC, INFINITE);
		WaitForSingleObject(Mutex, INFINITE);
		cout << (char const*)text << flush;
		ReleaseMutex(Mutex);
		computation();
		ReleaseSemaphore(SemE, 1, NULL);
	}
	return 0;
}

DWORD WINAPI thread_h(LPVOID text)
{
	for (int i = 0; i < 3; ++i)
	{
		WaitForSingleObject(Mutex, INFINITE);
		cout << (char const*)text << flush;
		ReleaseMutex(Mutex);
		computation();
	}

	WaitForSingleObject(Thread[5], INFINITE); //f

	for (int i = 0; i < 3; ++i)
	{
		WaitForSingleObject(Mutex, INFINITE);
		cout << (char const*)text << flush;
		ReleaseMutex(Mutex);
		computation();
	}

	return 0;
}
DWORD WINAPI thread_g(LPVOID text)
{
	for (int i = 0; i < 3; ++i)
	{
		WaitForSingleObject(Mutex, INFINITE);
		cout << (char const*)text << flush;
		ReleaseMutex(Mutex);
		computation();
	}

	WaitForSingleObject(Thread[3], INFINITE); //d
	WaitForSingleObject(Thread[4], INFINITE); //e

	Thread[5] = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)threads_adfi, (void*)texts[5], 0, &ThreadID); // f
	if (Thread[5] == NULL) 
	{ 
		cout << "CreateThread error:  " << texts[5] << GetLastError() << endl; 
		return 1;
	}

	Thread[7] = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)thread_h, (void*)texts[7], 0, &ThreadID); //h
	if (Thread[7] == NULL) 
	{ 
		cout << "CreateThread error:  " << texts[7] << GetLastError() << endl; return 1;
	}

	for (int i = 0; i < 3; ++i)
	{
		WaitForSingleObject(Mutex, INFINITE);
		cout << (char const*)text << flush;
		ReleaseMutex(Mutex);
		computation();
	}

	WaitForSingleObject(Thread[5], INFINITE); //f

	Thread[8] = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)threads_adfi, (void*)texts[8], 0, &ThreadID); //i
	if (Thread[8] == NULL) 
	{ 
		cout << "CreateThread error:  " << texts[8] << GetLastError() << endl;
		return 1;
	}

	for (int i = 0; i < 3; ++i)
	{
		WaitForSingleObject(Mutex, INFINITE);
		cout << (char const*)text << flush;
		ReleaseMutex(Mutex);
		computation();
	}

	return 0;
}

DWORD WINAPI thread_e(LPVOID text)
{
	Thread[1] = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)thread_b, (void*)texts[1], 0, &ThreadID); // b
	if (Thread[1] == NULL)
	{
		cout << "CreateThread error: " << texts[1] << GetLastError() << endl;
		return 1;
	}

	Thread[2] = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)thread_c, (void*)texts[2], 0, &ThreadID); // c
	if (Thread[2] == NULL)
	{
		cout << "CreateThread error: " << texts[2] << GetLastError() << endl;
		return 1;
	}

	for (int i = 0; i < 3; ++i)
	{
		WaitForSingleObject(SemE, INFINITE);
		WaitForSingleObject(Mutex, INFINITE);
		cout << (char const*)text << flush;
		ReleaseMutex(Mutex);
		computation();
		ReleaseSemaphore(SemB, 1, NULL);
	}

	WaitForSingleObject(Thread[1], INFINITE); //b
	WaitForSingleObject(Thread[2], INFINITE); //c

	Thread[3] = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)threads_adfi, (void*)texts[3], 0, &ThreadID); //d
	if (Thread[3] == NULL) 
	{ 
		cout << "CreateThread error:  " << texts[3] << GetLastError() << endl; 
		return 1; 
	}

	Thread[6] = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)thread_g, (void*)texts[6], 0, &ThreadID); //g
	if (Thread[6] == NULL) 
	{ 
		cout << "CreateThread error:  " << texts[6] << GetLastError() << endl;	
		return 1;
	}

	for (int i = 0; i < 3; ++i)
	{
		WaitForSingleObject(Mutex, INFINITE);
		cout << (char const*)text << flush;
		ReleaseMutex(Mutex);
		computation();
	}

	return 0;
}

int lab3_init()
{
	Mutex = CreateMutex(NULL, FALSE, NULL);
	if (Mutex == NULL) 
	{ 
		cout << "CreateMutex error " << GetLastError() << endl;
		return 1;
	}

	SemB = CreateSemaphore(NULL, 1, 1, NULL);
	if (SemB == NULL) 
	{
		cout << "CreateSemaphore error: SemB" << GetLastError() << endl;
		return 1; 
	}

	SemC = CreateSemaphore(NULL, 0, 1, NULL);
	if (SemC == NULL) 
	{ 
		cout << "CreateSemaphore error: SemC" << GetLastError() << endl; 
		return 1; 
	}

	SemE = CreateSemaphore(NULL, 0, 1, NULL);
	if (SemE == NULL) 
	{ 
		cout << "CreateSemaphore error: SemE" << GetLastError() << endl; 
		return 1; 
	}

	Thread[0] = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)threads_adfi, (void*)texts[0], 0, &ThreadID); // a
	if (Thread[0] == NULL)
	{
		cout << "CreateThread error: " << texts[0] << GetLastError() << endl;
		return 1;
	}

	WaitForSingleObject(Thread[0], INFINITE); //a

	Thread[4] = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)thread_e, (void*)texts[4], 0, &ThreadID); // e
	if (Thread[4] == NULL)
	{
		cout << "CreateThread error: " << texts[4] << GetLastError() << endl;
		return 1;
	}

	WaitForSingleObject(Thread[4], INFINITE); //e
	WaitForSingleObject(Thread[6], INFINITE); //g
	WaitForSingleObject(Thread[7], INFINITE); //h
	WaitForSingleObject(Thread[8], INFINITE); //i

	CloseHandle(SemE);
	CloseHandle(SemB);
	CloseHandle(SemC);
	CloseHandle(Mutex);

	cout << endl;

	return 0;
}