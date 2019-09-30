#ifndef DIRECTX_H
#define DIRECTX_H

#include<windows.h>
#include<d3dx9.h>
#include<tchar.h>
#include<dinput.h>

class DirectX
{
public:

	LPDIRECT3D9 pDirect3d;
	LPDIRECT3DDEVICE9 pDevice;
	LPDIRECTINPUT8 pDinput;
	LPDIRECTINPUTDEVICE8 pkey;


};

#endif