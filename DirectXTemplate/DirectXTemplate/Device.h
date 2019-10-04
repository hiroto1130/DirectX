#ifndef DEVICE_H
#define DEVICE_H

#include"DirectX.h"

class Device
{
private:

struct INPUTSTATE
{
	DWORD now;
	DWORD pushed;
};

// キーボード //
static const int MAX_KEY_NUMBER = 256;
// 入力キーを入れる値
BYTE KeyState[MAX_KEY_NUMBER];
// 256の配列を持つ
const int MASK_NUM = 0x80;

// マテリアルの数
DWORD dwNumMaterials = 0;

public:

HRESULT BuildDxDevice(HWND* hInst, DirectX* directX);

void UpdateKeyStatus(DirectX* directX);

bool GetKeyStatus(int KeyNumber);

void Render(DirectX directX);

//DirectX* directX;
HRESULT InitDinput(HWND*, DirectX*);
HRESULT InitD3d(HWND*, DirectX*);
};



#endif // !1