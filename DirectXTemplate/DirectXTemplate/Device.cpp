#include"Device.h"


HRESULT Device::BuildDxDevice(HWND* hInst, DirectX* directX)
{
	//// InitD3d が初期化されているかどうか ////
	if (FAILED(InitD3d(hInst, directX)))
	{
		return E_FAIL;
	}

	//// InitDinput 初期化されているかどうか ////
	if (FAILED(InitDinput(hInst, directX)))
	{
		return E_FAIL;
	}

	//// Direct3DCreate9 が ////
	if (directX->pDirect3d == NULL)
	{
		return E_FAIL;
	}
	return S_OK;
}

HRESULT Device::InitD3d(HWND* hInst, DirectX* directX)
{
	if (NULL == (directX->pDirect3d = Direct3DCreate9(D3D_SDK_VERSION)))
	{
		return E_FAIL;
	}

	//// バッファ設定 ////

	D3DPRESENT_PARAMETERS d3dpp;
	ZeroMemory(&d3dpp, sizeof(d3dpp));
	d3dpp.BackBufferFormat = D3DFMT_UNKNOWN;
	d3dpp.BackBufferCount = 1;
	d3dpp.SwapEffect = D3DSWAPEFFECT_DISCARD;
	d3dpp.Windowed = true;
	d3dpp.EnableAutoDepthStencil = TRUE;
	d3dpp.AutoDepthStencilFormat = D3DFMT_D16;

	//// デバイス生成 ////

	if (FAILED(directX->pDirect3d->CreateDevice(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, *hInst,
		D3DCREATE_MIXED_VERTEXPROCESSING, &d3dpp, &directX->pDevice)))
	{
		return E_FAIL;
	}

	////    ////

	// Xファイルからメッシュをロードする	
	LPD3DXBUFFER pD3DXMtrlBuffer = NULL;

	if (FAILED(D3DXLoadMeshFromX("Chips.x", D3DXMESH_SYSTEMMEM,
		directX->pDevice, NULL, &pD3DXMtrlBuffer, NULL,
		&dwNumMaterials, &directX->pMesh)))
	{
		MessageBox(NULL, "Xファイルの読み込みに失敗しました", NULL, MB_OK);
		return E_FAIL;
	}
	D3DXMATERIAL* d3dxMaterials = (D3DXMATERIAL*)pD3DXMtrlBuffer->GetBufferPointer();
	directX->pMeshMaterials = new D3DMATERIAL9[dwNumMaterials];
	directX->pMeshTextures = new LPDIRECT3DTEXTURE9[dwNumMaterials];

	for (DWORD i = 0; i < dwNumMaterials; i++)
	{
		directX->pMeshMaterials[i] = d3dxMaterials[i].MatD3D;
		directX->pMeshMaterials[i].Ambient = directX->pMeshMaterials[i].Diffuse;
		directX->pMeshTextures[i] = NULL;
		if (d3dxMaterials[i].pTextureFilename != NULL &&
			lstrlen(d3dxMaterials[i].pTextureFilename) > 0)
		{
			if (FAILED(D3DXCreateTextureFromFile(directX->pDevice,
				d3dxMaterials[i].pTextureFilename,
				&directX->pMeshTextures[i])))
			{
				MessageBox(NULL, "テクスチャの読み込みに失敗しました", NULL, MB_OK);
			}
		}
	}

	pD3DXMtrlBuffer->Release();
	// Zバッファー処理を有効にする
	directX->pDevice->SetRenderState(D3DRS_ZENABLE, TRUE);
	// ライトを有効にする
	directX->pDevice->SetRenderState(D3DRS_LIGHTING, TRUE);
	// アンビエントライト（環境光）を設定する
	directX->pDevice->SetRenderState(D3DRS_AMBIENT, 0x00111111);
	// スペキュラ（鏡面反射）を有効にする
	directX->pDevice->SetRenderState(D3DRS_SPECULARENABLE, TRUE);

	return S_OK;

}

////// directinput //////

HRESULT Device::InitDinput(HWND* hInst, DirectX* directX)
{
	HRESULT hr;

	//// DirectInput8の作成 ////
	if (FAILED(hr = DirectInput8Create(GetModuleHandle(NULL), DIRECTINPUT_VERSION, IID_IDirectInput8, (VOID * *)& directX->pDinput, NULL)))
	{
		return hr;
	}

	//// InputDeviceを作成 ////         * どの入力デバイスから情報を受け取るか
	if (FAILED(hr = directX->pDinput->CreateDevice(GUID_SysKeyboard, &directX->pkey, NULL)))
	{
		return hr;
	}

	//// デバイスのフォーマット設定,形式 ////　　　* キーボード c_dfDIKeyboard マウス c_dfDIMouse
	if (FAILED(hr = directX->pkey->SetDataFormat(&c_dfDIKeyboard)))
	{
		return hr;
	}

	//// 協調レベルを設定 ////    * フォアグラウンド、バックグラウンド 排他的、非排他的 
	if (FAILED(hr = directX->pkey->SetCooperativeLevel(*hInst, DISCL_NONEXCLUSIVE | DISCL_BACKGROUND)))
	{
		return hr;
	}

	//// 権限の取得 ////
	directX->pkey->Acquire();

	return S_OK;
}

////// キーステータス更新関数 //////
void Device::UpdateKeyStatus(DirectX* directX)
{
	HRESULT hr = directX->pkey->Acquire();

	if ((hr == DI_OK) || (hr == S_FALSE))
	{
		directX->pkey->GetDeviceState(sizeof(KeyState), &KeyState);
	}

}

////// キー入力関数 //////
bool Device::GetKeyStatus(int KeyNumber)
{

	if (KeyState[KeyNumber] & MASK_NUM)
	{

		return true;// 入力されたら true を返す

	}
	return false;// 入力されていなければ false を返す

}

//VOID Render()
//Xファイルから読み込んだメッシュをレンダリングする関数
void Device::Render(DirectX* directX)
{
	//ワールドトランスフォーム（絶対座標変換）
	D3DXMATRIXA16 matWorld, matRotation;
	D3DXMatrixRotationY(&matWorld, timeGetTime() / 3000.0f);
	D3DXMatrixRotationX(&matRotation, 0.5f);
	D3DXMatrixMultiply(&matWorld, &matWorld, &matRotation);
	directX->pDevice->SetTransform(D3DTS_WORLD, &matWorld);
	// ビュートランスフォーム（視点座標変換）

	D3DXVECTOR3 vecEyePt(0.0f, 1.0f, -3.0f); //カメラ（視点）位置
	D3DXVECTOR3 vecLookatPt(0.0f, 0.0f, 0.0f);//注視位置
	D3DXVECTOR3 vecUpVec(0.0f, 1.0f, 0.0f);//上方位置
	D3DXMATRIXA16 matView;
	D3DXMatrixLookAtLH(&matView, &vecEyePt, &vecLookatPt, &vecUpVec);
	directX->pDevice->SetTransform(D3DTS_VIEW, &matView);
	// プロジェクショントランスフォーム（射影変換）
	D3DXMATRIXA16 matProj;
	D3DXMatrixPerspectiveFovLH(&matProj, D3DX_PI / 4, 1.0f, 1.0f, 100.0f);
	directX->pDevice->SetTransform(D3DTS_PROJECTION, &matProj);
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
	directX->pDevice->SetLight(0, &light);
	directX->pDevice->LightEnable(0, TRUE);
	// レンダリング
	directX->pDevice->Clear(0, NULL, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER,
		D3DCOLOR_XRGB(100, 100, 100), 1.0f, 0);

	if (SUCCEEDED(directX->pDevice->BeginScene()))
	{
		for (DWORD i = 0; i < dwNumMaterials; i++)
		{
			directX->pDevice->SetMaterial(&directX->pMeshMaterials[i]);
			directX->pDevice->SetTexture(0, directX->pMeshTextures[i]);
			directX->pMesh->DrawSubset(i);
		}
		directX->pDevice->EndScene();
	}
	directX->pDevice->Present(NULL, NULL, NULL, NULL);
}