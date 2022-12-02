#pragma once
#include <string>

class BaseSystem
{

private:
	static class WinApp* m_winApp;
	static class DirectXCommon* m_diXCom;
	static class TextureManager* m_texMana;
	static class Input* m_input;

public:

	static void Initalize(int windowWidth = 1280, int windowHeight = 720, const std::string& windowTitle = "DirectXGame");
	static void BeginFrame();
	static void EndFrame();
	static void Finalize();

	static class WinApp* GetWinApp() { return m_winApp; }
	static class DirectXCommon* GetDirectXCommon() { return m_diXCom; }
	static class TextureManager* GetTextureMamager() { return m_texMana; }


private:
	BaseSystem() = default;
	~BaseSystem() = default;
	BaseSystem(const BaseSystem&) = delete;
	const BaseSystem& operator=(const BaseSystem&) = delete;
};

