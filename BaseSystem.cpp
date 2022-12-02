#include "BaseSystem.h"
#include "WinApp.h"
#include "DirectXCommon.h"
#include "TextureManager.h"
#include "Input.h"

WinApp* BaseSystem::m_winApp = nullptr;
DirectXCommon* BaseSystem::m_diXCom = nullptr;
TextureManager* BaseSystem::m_texMana = nullptr;
Input* BaseSystem::m_input = nullptr;

void BaseSystem::Initalize(int windowWidth, int windowHeight, const std::string& windowTitle)
{
	m_winApp = WinApp::GetInstance();
	m_winApp->Initialize(windowWidth, windowHeight, windowTitle);

	m_diXCom = DirectXCommon::GetInstance();
	m_diXCom->Initialize(m_winApp);

	m_texMana = TextureManager::GetInstance();
	m_texMana->Initialize(m_diXCom);

	m_input = Input::GetInstance();
	m_input->Initialize(m_winApp);
}

void BaseSystem::BeginFrame()
{
	m_diXCom->PreDraw();
	m_input->Update();
}

void BaseSystem::EndFrame()
{
	m_diXCom->PostDraw();
}

void  BaseSystem::Finalize()
{
	m_winApp->Finalize();
}