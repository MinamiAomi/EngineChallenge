#include "Input.h"

#include <cassert>

#pragma comment(lib,"dinput8.lib")
#pragma comment(lib,"dxguid.lib")

Input* Input::GetInstance() 
{
	static Input instance;
	return &instance;
}

void Input::Initialize(WinApp* winApp)
{
	HRESULT result = S_FALSE;
	// DirectInputの初期化
	result = DirectInput8Create(
		winApp->GetHInstance(), DIRECTINPUT_HEADER_VERSION, 
		IID_IDirectInput8, (void**)&m_directInput, nullptr);
	assert(SUCCEEDED(result));
	// キーボードデバイスの生成
	result = m_directInput->CreateDevice(GUID_SysKeyboard, &m_keyboard, NULL);
	assert(SUCCEEDED(result));
	// 入力データー形式のセット
	result = m_keyboard->SetDataFormat(&c_dfDIKeyboard);
	assert(SUCCEEDED(result));
	// 排他制御レベルのセット
	result = m_keyboard->SetCooperativeLevel(
		winApp->GetHwnd(), DISCL_FOREGROUND | DISCL_NONEXCLUSIVE | DISCL_NOWINKEY);
	assert(SUCCEEDED(result));
}

void Input::Update() {
	memcpy(mPreKeys, mKeys, kKeyDataNum);
	// キーボード情報の取得開始
	m_keyboard->Acquire();
	// 全キーの入力状態を取得する
	m_keyboard->GetDeviceState(sizeof(mKeys), mKeys);
}