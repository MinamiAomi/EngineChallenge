#include "WinApp.h"
#include "DirectXCommon.h"


// Windowsアプリのエントリーポイント(main関数)
int MAIN
{




	WinApp* winApp = WinApp::GetInstance();
	winApp->Initialize();

	DirectXCommon* directXCommon = DirectXCommon::GetInstance();
	directXCommon->Initialize(winApp);
	

	while (!winApp->WindowQUit()) {


	}

	winApp->Finalize();

	return 0;
}
