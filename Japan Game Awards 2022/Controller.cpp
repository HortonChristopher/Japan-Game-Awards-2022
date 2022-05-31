#define DIRECTINPUT_VERSION 0x0800
#include <dinput.h>
#include <Windows.h>
#include <stdio.h>
#include <math.h>

#include "WinApp.h"
#include "Controller.h"

#include <assert.h>

#pragma comment(lib, "dinput8.lib")
#pragma comment(lib, "dxguid.lib")

static LPDIRECTINPUT8 g_InputInterface;							//!< DIRECTINPUT8のポインタ
static LPDIRECTINPUTDEVICE8 g_GamePadDevice;					//!< DIRECTINPUTDEVICE8のポインタ
static ButtonState g_ButtonStates[ButtonKind::ButtonKindMax];

//WinApp* win = nullptr;

// 入力インターフェースの作成 Creating an input interface
bool CreateInputInterface();

// ゲームパッドデバイスの作成 Creating a gamepad device
bool CreateGamePadDevice();

// ゲームパッドの更新 Gamepad updates
void UpdateGamePad();

//ゲームパッドデバイスの作成-デバイス列挙の結果を受け取る構造体 Creating a gamepad device-a structure that receives the results of device enumeration
struct DeviceEnumParameter
{
	LPDIRECTINPUTDEVICE8* GamePadDevice;
	int FindCount;
};

BOOL SetUpCooperativeLevel(LPDIRECTINPUTDEVICE8 device)
{
	//協調モードの設定 Cooperative mode setting
	if (FAILED(device->SetCooperativeLevel(
		FindWindowA(WINDOW_CLASS_NAME, nullptr),
		DISCL_EXCLUSIVE | DISCL_FOREGROUND)
	))
	{
		return false;
	}

	return true;
}

BOOL StartGamePadControl()
{
	// デバイスが生成されてない No device generated
	if (g_GamePadDevice == nullptr)
	{
		return false;
	}
	
	// 制御開始 Start control
	if (FAILED(g_GamePadDevice->Acquire()))
	{
		return false;
	}

	DIDEVCAPS cap;
	g_GamePadDevice->GetCapabilities(&cap);
	// ポーリング判定 Polling judgment
	if (cap.dwFlags & DIDC_POLLEDDATAFORMAT)
	{
		DWORD error = GetLastError();
		// ポーリング開始 Start polling
		/*
			PollはAcquireの前に行うとされていたが、 Poll was supposed to be done before Acquire,
			Acquireの前で実行すると失敗したので When I ran it before Acquire, it failed.
			後で実行するようにした I tried to execute it later
		*/
		if (FAILED(g_GamePadDevice->Poll()))
		{
			return false;
		}
	}

	return true;
}

BOOL SetUpGamePadProperty(LPDIRECTINPUTDEVICE8 device)
{
	// 軸モードを絶対値モードとして設定 Set axis mode as absolute mode
	DIPROPDWORD diprop;
	ZeroMemory(&diprop, sizeof(diprop));
	diprop.diph.dwSize = sizeof(diprop);
	diprop.diph.dwHeaderSize = sizeof(diprop.diph);
	diprop.diph.dwHow = DIPH_DEVICE;
	diprop.diph.dwObj = 0;
	diprop.dwData = DIPROPAXISMODE_ABS;
	if (FAILED(device->SetProperty(DIPROP_AXISMODE, &diprop.diph)))
	{
		return false;
	}

	// X軸の値の範囲設定 X-axis value range setting
	DIPROPRANGE diprg;
	ZeroMemory(&diprg, sizeof(diprg));
	diprg.diph.dwSize = sizeof(diprg);
	diprg.diph.dwHeaderSize = sizeof(diprg.diph);
	diprg.diph.dwHow = DIPH_BYOFFSET;
	diprg.diph.dwObj = DIJOFS_X;
	diprg.lMin = -1000;
	diprg.lMax = 1000;
	if (FAILED(device->SetProperty(DIPROP_RANGE, &diprg.diph)))
	{
		return false;
	}

	// Y軸の値の範囲設定 Y-axis value range setting
	diprg.diph.dwObj = DIJOFS_Y;
	if (FAILED(device->SetProperty(DIPROP_RANGE, &diprg.diph)))
	{
		return false;
	}

	return true;
}

BOOL CALLBACK DeviceFindCallBack(LPCDIDEVICEINSTANCE lpddi, LPVOID pvRef)
{
	DeviceEnumParameter* parameter = (DeviceEnumParameter*)pvRef;
	LPDIRECTINPUTDEVICE8 device = nullptr;

	// 既に発見しているなら終了 Finish if you have already found
	if (parameter->FindCount >= 1)
	{
		return DIENUM_STOP;
	}

	// デバイス生成 Device generation
	HRESULT hr = g_InputInterface->CreateDevice(
		lpddi->guidInstance,
		parameter->GamePadDevice,
		NULL);

	if (FAILED(hr))
	{
		return DIENUM_STOP;
	}

	// 入力フォーマットの指定 Specifying the input format
	device = *parameter->GamePadDevice;
	hr = device->SetDataFormat(&c_dfDIJoystick);

	if (FAILED(hr))
	{
		return DIENUM_STOP;
	}

	// プロパティの設定 Property settings
	if (SetUpGamePadProperty(device) == false)
	{
		return DIENUM_STOP;
	}

	// 協調レベルの設定 Coordination level setting
	if (SetUpCooperativeLevel(device) == false)
	{
		return DIENUM_STOP;
	}

	// 発見数をカウント Count the number of discoveries
	parameter->FindCount++;

	return DIENUM_CONTINUE;
}

bool InitInput()
{
	// インターフェース作成 Interface creation
	if (CreateInputInterface() == false)
	{
		return false;
	}

	// デバイス作成 Device creation
	if (CreateGamePadDevice() == false)
	{
		
		ReleaseInput();
		return false;
	}

	// 入力情報の初期化 Initialization of input information
	for (int i = 0; i < ButtonKind::ButtonKindMax; i++)
	{
		g_ButtonStates[i] = ButtonState::ButtonStateNone;
	}

	return true;
}

void ReleaseInput()
{
	// デバイスの解放 Release the device
	if (g_GamePadDevice != nullptr)
	{
		// 制御を停止
		g_GamePadDevice->Unacquire();
		g_GamePadDevice->Release();
		g_GamePadDevice = nullptr;
	}

	// インターフェースの解放 Interface release
	if (g_InputInterface != nullptr)
	{
		g_InputInterface->Release();
		g_InputInterface = nullptr;
	}
}

void UpdateInput()
{
	UpdateGamePad();
}

bool IsButtonPush(ButtonKind button)
{
	if (g_ButtonStates[button] == ButtonState::ButtonStatePush)
	{
		return true;
	}

	return false;
}

bool IsButtonUp(ButtonKind button)
{
	if (g_ButtonStates[button] == ButtonState::ButtonStateUp)
	{
		return true;
	}

	return false;
}

bool IsButtonDown(ButtonKind button)
{
	if (g_ButtonStates[button] == ButtonState::ButtonStateDown)
	{
		return true;
	}

	return false;
}

bool CreateInputInterface()
{
	// インターフェース作成 Interface creation
	HRESULT ret = DirectInput8Create(
		GetModuleHandle(nullptr),	// インスタンスハンドル Instance handle
		DIRECTINPUT_VERSION,		// DirectInputのバージョン DirectInput version
		IID_IDirectInput8,			// 使用する機能 Functions to use
		(void**)&g_InputInterface,	// 作成されたインターフェース代入用 For created interface assignment
		NULL						// NULL固定 Null fixed
	);

	if (FAILED(ret))
	{
		return false;
	}

	return true;
}

bool CreateGamePadDevice()
{
	DeviceEnumParameter parameter;

	parameter.FindCount = 0;
	parameter.GamePadDevice = &g_GamePadDevice;

	// GAMEPADを調べる Examine GAMEPAD
	g_InputInterface->EnumDevices(
		DI8DEVTYPE_GAMEPAD,			// 検索するデバイスの種類 Type of device to search
		DeviceFindCallBack,			// 発見時に実行する関数 Function to execute at the time of discovery
		&parameter,					// 関数に渡す値 Value to pass to the function
		DIEDFL_ATTACHEDONLY			// 検索方法 retrieval method
	);

	// JOYSTICKを調べる Examine JOYSTICK
	g_InputInterface->EnumDevices(
		DI8DEVTYPE_JOYSTICK,
		DeviceFindCallBack,
		&parameter,
		DIEDFL_ATTACHEDONLY
	);

	// どちらも見つけることが出来なかったら失敗 Failure if neither can be found
	if (parameter.FindCount == 0)
	{
		return false;
	}

	int count = 0;
	// 制御開始 Start control
	while (StartGamePadControl() == false)
	{
		Sleep(100);
		count++;
		if (count >= 5)
		{
			break;
		}
	}

	return true;
}

void UpdateGamePad()
{
	if (g_GamePadDevice == nullptr)
	{
		return;
	}

	DIJOYSTATE pad_data;

	// デバイス取得 Device acquisition
	HRESULT hr = g_GamePadDevice->GetDeviceState(sizeof(DIJOYSTATE), &pad_data);
	if (FAILED(hr))
	{
		// 再度制御開始 Start control again
		if (FAILED(g_GamePadDevice->Acquire()))
		{
			for (int i = 0; i < ButtonKind::ButtonKindMax; i++)
			{
				g_ButtonStates[i] = ButtonState::ButtonStateNone;
			}
			g_GamePadDevice->Poll();
		}
		return;
	}

	bool is_push[ButtonKind::ButtonKindMax];
	// スティック判定 Stick judgment
	int unresponsive_range = 200;
	if (pad_data.lX < -unresponsive_range)
	{
		is_push[ButtonKind::LeftButton] = true;
	}
	else if (pad_data.lX > unresponsive_range)
	{
		is_push[ButtonKind::RightButton] = true;
	}

	if (pad_data.lY < -unresponsive_range)
	{
		is_push[ButtonKind::UpButton] = true;
	}
	else if (pad_data.lY > unresponsive_range)
	{
		is_push[ButtonKind::DownButton] = true;
	}

	// 十字キー判定 Cross key judgment
	if (pad_data.rgdwPOV[0] != 0xFFFFFFFF)
	{
		float rad = pad_data.rgdwPOV[0] * 3.14159265f / 180.0f;

		// 本来はxがcos、yがsinだけど、rgdwPOVは0が上から始まるので、 Originally, x is cos and y is sin, but rgdwPOV starts from 0, so
		// cosとsinを逆にした方が都合がいい It is convenient to reverse cos and sin
		float x = sinf(rad);
		float y = cosf(rad);

		if (x < -0.01f)
		{
			is_push[ButtonKind::LeftButton] = true;
		}
		else if (x > 0.01f)
		{
			is_push[ButtonKind::RightButton] = true;
		}

		if (y > 0.01f)
		{
			is_push[ButtonKind::UpButton] = true;
		}
		else if (y < -0.01f)
		{
			is_push[ButtonKind::DownButton] = true;
		}
	}

	 ////十字キー判定 Cross key judgment
		// if (pad_data.rgdwPOV[0] != 0xFFFFFFFF)
		// {
		//	 //八方向全てを書く Write all eight directions
		//	 switch (pad_data.rgdwPOV[0])
		//	 {
		//		 //上 Up
		//	 case 0:
		//		 is_push[ButtonKind::UpButton] = true;
		//		 break;

		//	 case 4500:
		//		 is_push[ButtonKind::UpButton] = true;
		//		 is_push[ButtonKind::RightButton] = true;
		//		 break;

		//	 case 9000:
		//		 is_push[ButtonKind::RightButton] = true;
		//		 break;

		//	 case 13500:
		//		 is_push[ButtonKind::DownButton] = true;
		//		 is_push[ButtonKind::RightButton] = true;
		//		 break;

		//	 case 18000:
		//		 is_push[ButtonKind::DownButton] = true;
		//		 break;

		//	 case 22500:
		//		 is_push[ButtonKind::DownButton] = true;
		//		 is_push[ButtonKind::LeftButton] = true;
		//		 break;

		//	 case 27000:
		//		 is_push[ButtonKind::LeftButton] = true;
		//		 break;

		//	 case 31500:

		//		 is_push[ButtonKind::UpButton] = true;
		//		 is_push[ButtonKind::LeftButton] = true;
		//		 break;
		//	 }
		// }

	// ボタン判定 Button judgment
	for (int i = 0; i < 32; i++)
	{
		if (!(pad_data.rgbButtons[i] & 0x80))
		{
			continue;
		}

		switch (i)
		{
		case 0:
			is_push[ButtonKind::Button_A] = true;
			break;
		case 1:
			is_push[ButtonKind::Button_B] = true;
			break;
		case 2:
			is_push[ButtonKind::Button_X] = true;
			break;
		case 3:
			is_push[ButtonKind::Button_Y] = true;
			break;
		case 4:
			is_push[ButtonKind::Button_LB] = true;
			break;
		case 5:
			is_push[ButtonKind::Button_RB] = true;
			break;
		case 6:
			is_push[ButtonKind::Button_LeftMenu] = true;
			break;
		case 7:
			is_push[ButtonKind::Button_RightMenu] = true;
			break;
		case 8:
			is_push[ButtonKind::Button_LeftStick] = true;
			break;
		case 9:
			is_push[ButtonKind::Button_RightStick] = true;
			break;
		}
	}

	// 入力情報からボタンの状態を更新する Update the button status from the input information
	for (int i = 0; i < ButtonKind::ButtonKindMax; i++)
	{
		if (is_push[i] == true)
		{
			if (g_ButtonStates[i] == ButtonState::ButtonStateNone)
			{
				g_ButtonStates[i] = ButtonState::ButtonStateDown;
			}
			else
			{
				g_ButtonStates[i] = ButtonState::ButtonStatePush;
			}
		}
		else
		{
			if (g_ButtonStates[i] == ButtonState::ButtonStatePush)
			{
				g_ButtonStates[i] = ButtonState::ButtonStateUp;
			}
			else
			{
				g_ButtonStates[i] = ButtonState::ButtonStateNone;
			}
		}
	}
}
