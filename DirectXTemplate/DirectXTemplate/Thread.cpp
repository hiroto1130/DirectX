#include "Thread.h"


DWORD WINAPI Thread::MainThread(LPVOID* data)
{
	//1000ミリ秒（1秒）おきにループ
	Sleep(1000);
	MessageBox(0, "Sleep処理が終了しました", NULL, MB_OK);

	ExitThread(0);

}

void Thread::MakeThread()
{
	////スレッドを作成
	//th = CreateThread(
	//	0,                                // 0 でデフォルトの値 
	//	0,                                // 0 でデフォルトの値
	//	(LPTHREAD_START_ROUTINE)MainThread,   // スレッド実行時の関数 
	//	(LPVOID)"カウント数表示：",       // スレッド実行時の関数の引数
	//	0,                                // 実行タイミング      
	//	NULL                              // スレッド識別子を保存するためのDWORD型のポインタの指定
	//);

}

void Thread::CloseThread()
{


}