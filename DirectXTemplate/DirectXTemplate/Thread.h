#ifndef THREAD_H
#define THREAD_H

#include<windows.h>

class Thread
{
private:
	static HANDLE th;
	DWORD result;

public:

	void MakeThread();	//1000ミリ秒（1秒）おきにループ
	void CloseThread();

	DWORD WINAPI MainThread(LPVOID* Data);


};



#endif