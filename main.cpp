#include "WinApp.h"
#include "DirectXCommon.h"

#include <DirectXMath.h>
using namespace DirectX;

// Windowsアプリのエントリーポイント(main関数)
int MAIN
{

	WinApp* winApp = WinApp::GetInstance();
	winApp->Initialize();

	DirectXCommon* directXCommon = DirectXCommon::GetInstance();
	directXCommon->Initialize(winApp);
	

	XMFLOAT3 vertices[] = 
	{	{ -0.5f, -0.5f, 0.0f },
		{ -0.5f,  0.5f, 0.0f },
		{  0.5f, -0.5f, 0.0f }	};

	UINT sizeVB = static_cast<UINT>(sizeof(XMFLOAT3) * _countof(vertices));


	while (!winApp->WindowQUit()) {

		directXCommon->PreDraw();



		directXCommon->PostDraw();
	}

	winApp->Finalize();

	return 0;
}
