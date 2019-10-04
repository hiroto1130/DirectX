#include <windows.h>
#include <stdio.h>

#define MSG(m) {\
	MessageBoxA(NULL,m,NULL,MB_OK);}

//ウィンドウハンドル
HWND hwnd;
//インスタンスハンドル
HINSTANCE hinst;

//ウィンドウ横幅
#define WIDTH 500
#define HEIGHT 300

bool flag = true;
// スレッド関数(スレッド関数は常にこの形)
// この関数はLPTHREAD_START_ROUTINE型の為キャストが必要となる

DWORD WINAPI Thread(LPVOID* data)
{
	int count = 0;
	char buf[1000];
	while (flag) {

		//カウントをウィンドウタイトルに表示
		sprintf_s(buf, "%s%d", data, count);
		SetWindowText(hwnd, buf);

		++count;

		//1000ミリ秒（1秒）おきにループ
		Sleep(6000);
	}
	// 手動でスレッドを終わらせる場合にはこの関数を呼ぶ
	// return などは自動で呼ばれるため問題はない
	ExitThread(0);
}


LRESULT CALLBACK WinProc(HWND hwnd, UINT msg, WPARAM wp, LPARAM lp)
{

	static HANDLE th;
	DWORD result;
	switch (msg) {
	case WM_DESTROY:
		PostQuitMessage(0);
		return 0;
	case WM_CREATE:
		//スレッドを作成
		th = CreateThread(
			0,                                // 0 でデフォルトの値 
			0,                                // 0 でデフォルトの値
			(LPTHREAD_START_ROUTINE)Thread,   // スレッド実行時の関数 
			(LPVOID)"カウント数表示：",       // スレッド実行時の関数の引数
			0,                                // 実行タイミング      
			NULL                              // スレッド識別子を保存するためのDWORD型のポインタの指定
		);

		return 0;

	case WM_CLOSE:
		//フラグをfalseにしてスレッドを終了させる。
		flag = false;
		while (1) {
			// スレッドが終わったかチェックする関数
			// 終了していたら終了コードが第二引数に入る
			GetExitCodeThread(
				th,       // スレッドハンドル
				&result   // 終了ステータス(終了すれば STILL_ACTIVE 以外の値が入る)
			);

			//終わったらハンドルを閉じる。
			if (STILL_ACTIVE != result) {
				//closehandleで閉じる。
				CloseHandle(th);
				//ループを抜ける。
				break;
			}
		}
		//ウィンドウを破棄
		DestroyWindow(hwnd);

		return 0;

	}
	return DefWindowProc(hwnd, msg, wp, lp);
}

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nShowCmd)
{
	MSG msg;
	WNDCLASS wc;

	wc.style = CS_HREDRAW | CS_VREDRAW;
	wc.lpfnWndProc = WinProc;
	wc.cbClsExtra = wc.cbWndExtra = 0;
	wc.hInstance = hInstance;
	wc.hCursor = wc.hIcon = NULL;
	wc.hbrBackground = (HBRUSH)GetStockObject(BLACK_BRUSH);
	wc.lpszClassName = "test";
	wc.lpszMenuName = NULL;

	if (!RegisterClass(&wc)) {
		MSG("クラスの登録失敗");
		return -1;
	}

	hwnd = CreateWindowA("test", "テストウィンドウ", WS_VISIBLE | WS_CAPTION | WS_SYSMENU | WS_MINIMIZEBOX,
		0, 0, 400, 400, NULL, NULL, hInstance, NULL);

	if (hwnd == NULL) {
		MSG("ウィンドウ作成失敗");
		return -1;
	}

	//インスタンスハンドル
	hinst = hInstance;

	//エラーチェック用変数
	int check;

	while (check = GetMessage(&msg, NULL, 0, 0)) {
		if (check == -1) {
			break;
		}
		DispatchMessage(&msg);
	}

	//クラス解放
	UnregisterClass("test", hinst);

	return 0;

}