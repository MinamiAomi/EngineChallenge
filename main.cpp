#include "WinApp.h"
#include "DirectXCommon.h"


// Windows�A�v���̃G���g���[�|�C���g(main�֐�)
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
