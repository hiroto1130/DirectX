#include <Windows.h>
#include"Window.h"
#include"Engine.h"
#include"Singleton.h"
#include"Device.h"
#include"DrawTexture.h"

int WINAPI WinMain(HINSTANCE hInst, HINSTANCE hPrevInstance,LPSTR lpCmpLine, INT nCmdShow)
{
	
	HWND* hWnd;
	Window& window = singleton<Window>::get_instance();
    DirectX& directX = singleton<DirectX>::get_instance();
	Device& device = singleton<Device>::get_instance();
	Texture& texture = singleton<Texture>::get_instance();
	
	Texture::TEXTUREDATA textureData;

	if ((hWnd = InitEngine(1280, 960, hInst, &directX,window,device)) == NULL)
	{
		return 0;
	}

	timeBeginPeriod(1);
	//今の時間をtimeに保存。
	DWORD time = timeGetTime();
	DWORD prevtime = 0;

	texture.LoadTexture("Title8.png", &textureData.m_pTexture[1], 0, &directX);

	MSG msg;
	ZeroMemory(&msg, sizeof(msg));
	while (msg.message != WM_QUIT)
	{
		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
		{
			if (msg.message != WM_QUIT)
			{
				TranslateMessage(&msg);
				DispatchMessage(&msg);
			}
			else
			{
				break;
			}
		}
		else
		{
			texture.DrawTexture(300, 250, 250, 250, 0, 0, 1, 1, &textureData.m_pTexture[1], directX);
			// device.Render(&directX);
			time = timeGetTime();

		}
		prevtime = time;
	}

	timeEndPeriod(1);

	// エンジン終了
	EndEngine(directX);

	//　クラスの開放処理
	SingletonFinalizer::finalize();

	return 0;
}

/*
#include <windows.h>
#include <d3dx9.h>

#define SAFE_RELEASE(p) { if(p) { (p)->Release(); (p)=NULL; } }

LPDIRECT3D9 pD3d;
LPDIRECT3DDEVICE9 pDevice;
LPD3DXMESH pMesh = NULL;
D3DMATERIAL9* pMeshMaterials = NULL;
LPDIRECT3DTEXTURE9* pMeshTextures = NULL;
DWORD dwNumMaterials = 0;

LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);
HRESULT InitD3d(HWND);
VOID Render();
VOID FreeDx();

//
//INT WINAPI WinMain( HINSTANCE hInst,HINSTANCE hPrevInst,LPSTR szStr,INT iCmdShow)
//アプリケーションのエントリー関数
INT WINAPI WinMain(HINSTANCE hInst, HINSTANCE hPrevInst, LPSTR szStr, INT iCmdShow)
{
	HWND hWnd = NULL;
	MSG msg;
	// ウィンドウの初期化
	static char szAppName[] = "Chapter4-2";
	WNDCLASSEX  wndclass;

	wndclass.cbSize = sizeof(wndclass);
	wndclass.style = CS_HREDRAW | CS_VREDRAW;
	wndclass.lpfnWndProc = WndProc;
	wndclass.cbClsExtra = 0;
	wndclass.cbWndExtra = 0;
	wndclass.hInstance = hInst;
	wndclass.hIcon = LoadIcon(NULL, IDI_APPLICATION);
	wndclass.hCursor = LoadCursor(NULL, IDC_ARROW);
	wndclass.hbrBackground = (HBRUSH)GetStockObject(BLACK_BRUSH);
	wndclass.lpszMenuName = NULL;
	wndclass.lpszClassName = szAppName;
	wndclass.hIconSm = LoadIcon(NULL, IDI_APPLICATION);

	RegisterClassEx(&wndclass);

	hWnd = CreateWindow(szAppName, szAppName, WS_OVERLAPPEDWINDOW,
		0, 0, 800, 600, NULL, NULL, hInst, NULL);

	ShowWindow(hWnd, SW_SHOW);
	UpdateWindow(hWnd);
	// ダイレクト３Dの初期化関数を呼ぶ
	if (FAILED(InitD3d(hWnd)))
	{
		return 0;
	}
	// メッセージループ
	ZeroMemory(&msg, sizeof(msg));
	while (msg.message != WM_QUIT)
	{
		if (PeekMessage(&msg, NULL, 0U, 0U, PM_REMOVE))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
		else
		{
			Render();
		}
	}
	// メッセージループから抜けたらオブジェクトを全て開放する
	FreeDx();
	// OSに戻る（アプリケーションを終了する）
	return (INT)msg.wParam;
}

//
//LRESULT CALLBACK WndProc(HWND hWnd,UINT iMsg,WPARAM wParam,LPARAM lParam)
// ウィンドウプロシージャ関数
LRESULT CALLBACK WndProc(HWND hWnd, UINT iMsg, WPARAM wParam, LPARAM lParam)
{
	switch (iMsg)
	{
	case WM_DESTROY:
		PostQuitMessage(0);
		return 0;
	case WM_KEYDOWN:
		switch ((CHAR)wParam)
		{
		case VK_ESCAPE:
			PostQuitMessage(0);
			return 0;
		}
		break;
	}
	return DefWindowProc(hWnd, iMsg, wParam, lParam);
}

//
//HRESULT InitD3d(HWND hWnd)
//ダイレクト3Dの初期化関数
HRESULT InitD3d(HWND hWnd)
{
	// 「Direct3D」オブジェクトの作成
	if (NULL == (pD3d = Direct3DCreate9(D3D_SDK_VERSION)))
	{
		MessageBox(0, "Direct3Dの作成に失敗しました", "", MB_OK);
		return E_FAIL;
	}
	// 「DIRECT3Dデバイス」オブジェクトの作成
	D3DPRESENT_PARAMETERS d3dpp;
	ZeroMemory(&d3dpp, sizeof(d3dpp));
	d3dpp.BackBufferFormat = D3DFMT_UNKNOWN;
	d3dpp.BackBufferCount = 1;
	d3dpp.SwapEffect = D3DSWAPEFFECT_DISCARD;
	d3dpp.Windowed = TRUE;
	d3dpp.EnableAutoDepthStencil = TRUE;
	d3dpp.AutoDepthStencilFormat = D3DFMT_D16;

	if (FAILED(pD3d->CreateDevice(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, hWnd,
		D3DCREATE_MIXED_VERTEXPROCESSING,
		&d3dpp, &pDevice)))
	{
		MessageBox(0, "HALモードでDIRECT3Dデバイスを作成できません\nREFモードで再試行します", NULL, MB_OK);
		if (FAILED(pD3d->CreateDevice(D3DADAPTER_DEFAULT, D3DDEVTYPE_REF, hWnd,
			D3DCREATE_MIXED_VERTEXPROCESSING,
			&d3dpp, &pDevice)))
		{
			MessageBox(0, "DIRECT3Dデバイスの作成に失敗しました", NULL, MB_OK);
			return E_FAIL;
		}
	}
	// Xファイルからメッシュをロードする	
	LPD3DXBUFFER pD3DXMtrlBuffer = NULL;

	if (FAILED(D3DXLoadMeshFromX("Chips.x", D3DXMESH_SYSTEMMEM,
		pDevice, NULL, &pD3DXMtrlBuffer, NULL,
		&dwNumMaterials, &pMesh)))
	{
		MessageBox(NULL, "Xファイルの読み込みに失敗しました", NULL, MB_OK);
		return E_FAIL;
	}
	D3DXMATERIAL* d3dxMaterials = (D3DXMATERIAL*)pD3DXMtrlBuffer->GetBufferPointer();
	pMeshMaterials = new D3DMATERIAL9[dwNumMaterials];
	pMeshTextures = new LPDIRECT3DTEXTURE9[dwNumMaterials];

	for (DWORD i = 0; i < dwNumMaterials; i++)
	{
		pMeshMaterials[i] = d3dxMaterials[i].MatD3D;
		pMeshMaterials[i].Ambient = pMeshMaterials[i].Diffuse;
		pMeshTextures[i] = NULL;
		if (d3dxMaterials[i].pTextureFilename != NULL &&
			lstrlen(d3dxMaterials[i].pTextureFilename) > 0)
		{
			if (FAILED(D3DXCreateTextureFromFile(pDevice,
				d3dxMaterials[i].pTextureFilename,
				&pMeshTextures[i])))
			{
				MessageBox(NULL, "テクスチャの読み込みに失敗しました", NULL, MB_OK);
			}
		}
	}
	pD3DXMtrlBuffer->Release();
	// Zバッファー処理を有効にする
	pDevice->SetRenderState(D3DRS_ZENABLE, TRUE);
	// ライトを有効にする
	pDevice->SetRenderState(D3DRS_LIGHTING, TRUE);
	// アンビエントライト（環境光）を設定する
	pDevice->SetRenderState(D3DRS_AMBIENT, 0x00111111);
	// スペキュラ（鏡面反射）を有効にする
	pDevice->SetRenderState(D3DRS_SPECULARENABLE, TRUE);
	return S_OK;
}

//
//VOID Render()
//Xファイルから読み込んだメッシュをレンダリングする関数
VOID Render()
{
	//ワールドトランスフォーム（絶対座標変換）
	D3DXMATRIXA16 matWorld, matRotation;
	D3DXMatrixRotationY(&matWorld, timeGetTime() / 3000.0f);
	D3DXMatrixRotationX(&matRotation, 0.5f);
	D3DXMatrixMultiply(&matWorld, &matWorld, &matRotation);
	pDevice->SetTransform(D3DTS_WORLD, &matWorld);
	// ビュートランスフォーム（視点座標変換）

	D3DXVECTOR3 vecEyePt(0.0f, 1.0f, -3.0f); //カメラ（視点）位置
	D3DXVECTOR3 vecLookatPt(0.0f, 0.0f, 0.0f);//注視位置
	D3DXVECTOR3 vecUpVec(0.0f, 1.0f, 0.0f);//上方位置
	D3DXMATRIXA16 matView;
	D3DXMatrixLookAtLH(&matView, &vecEyePt, &vecLookatPt, &vecUpVec);
	pDevice->SetTransform(D3DTS_VIEW, &matView);
	// プロジェクショントランスフォーム（射影変換）
	D3DXMATRIXA16 matProj;
	D3DXMatrixPerspectiveFovLH(&matProj, D3DX_PI / 4, 1.0f, 1.0f, 100.0f);
	pDevice->SetTransform(D3DTS_PROJECTION, &matProj);
	// ライトをあてる 白色で鏡面反射ありに設定
	D3DXVECTOR3 vecDirection(0, 0, 1);
	D3DLIGHT9 light;
	ZeroMemory(&light, sizeof(D3DLIGHT9));
	light.Type = D3DLIGHT_DIRECTIONAL;
	light.Diffuse.r = 1.0f;
	light.Diffuse.g = 1.0f;
	light.Diffuse.b = 1.0f;
	light.Specular.r = 1.0f;
	light.Specular.g = 1.0f;
	light.Specular.b = 1.0f;
	D3DXVec3Normalize((D3DXVECTOR3*)& light.Direction, &vecDirection);
	light.Range = 200.0f;
	pDevice->SetLight(0, &light);
	pDevice->LightEnable(0, TRUE);
	// レンダリング
	pDevice->Clear(0, NULL, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER,
		D3DCOLOR_XRGB(100, 100, 100), 1.0f, 0);

	if (SUCCEEDED(pDevice->BeginScene()))
	{
		for (DWORD i = 0; i < dwNumMaterials; i++)
		{
			pDevice->SetMaterial(&pMeshMaterials[i]);
			pDevice->SetTexture(0, pMeshTextures[i]);
			pMesh->DrawSubset(i);
		}
		pDevice->EndScene();
	}
	pDevice->Present(NULL, NULL, NULL, NULL);
}

//
//VOID FreeDx()
// 作成したDirectXオブジェクトの開放
VOID FreeDx()
{
	SAFE_RELEASE(pMesh);
	SAFE_RELEASE(pDevice);
	SAFE_RELEASE(pD3d);
}

*/