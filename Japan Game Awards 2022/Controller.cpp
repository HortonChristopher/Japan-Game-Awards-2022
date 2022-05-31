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

static LPDIRECTINPUT8 g_InputInterface;							//!< DIRECTINPUT8�̃|�C���^
static LPDIRECTINPUTDEVICE8 g_GamePadDevice;					//!< DIRECTINPUTDEVICE8�̃|�C���^
static ButtonState g_ButtonStates[ButtonKind::ButtonKindMax];

//WinApp* win = nullptr;

// ���̓C���^�[�t�F�[�X�̍쐬 Creating an input interface
bool CreateInputInterface();

// �Q�[���p�b�h�f�o�C�X�̍쐬 Creating a gamepad device
bool CreateGamePadDevice();

// �Q�[���p�b�h�̍X�V Gamepad updates
void UpdateGamePad();

//�Q�[���p�b�h�f�o�C�X�̍쐬-�f�o�C�X�񋓂̌��ʂ��󂯎��\���� Creating a gamepad device-a structure that receives the results of device enumeration
struct DeviceEnumParameter
{
	LPDIRECTINPUTDEVICE8* GamePadDevice;
	int FindCount;
};

BOOL SetUpCooperativeLevel(LPDIRECTINPUTDEVICE8 device)
{
	//�������[�h�̐ݒ� Cooperative mode setting
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
	// �f�o�C�X����������ĂȂ� No device generated
	if (g_GamePadDevice == nullptr)
	{
		return false;
	}
	
	// ����J�n Start control
	if (FAILED(g_GamePadDevice->Acquire()))
	{
		return false;
	}

	DIDEVCAPS cap;
	g_GamePadDevice->GetCapabilities(&cap);
	// �|�[�����O���� Polling judgment
	if (cap.dwFlags & DIDC_POLLEDDATAFORMAT)
	{
		DWORD error = GetLastError();
		// �|�[�����O�J�n Start polling
		/*
			Poll��Acquire�̑O�ɍs���Ƃ���Ă������A Poll was supposed to be done before Acquire,
			Acquire�̑O�Ŏ��s����Ǝ��s�����̂� When I ran it before Acquire, it failed.
			��Ŏ��s����悤�ɂ��� I tried to execute it later
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
	// �����[�h���Βl���[�h�Ƃ��Đݒ� Set axis mode as absolute mode
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

	// X���̒l�͈̔͐ݒ� X-axis value range setting
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

	// Y���̒l�͈̔͐ݒ� Y-axis value range setting
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

	// ���ɔ������Ă���Ȃ�I�� Finish if you have already found
	if (parameter->FindCount >= 1)
	{
		return DIENUM_STOP;
	}

	// �f�o�C�X���� Device generation
	HRESULT hr = g_InputInterface->CreateDevice(
		lpddi->guidInstance,
		parameter->GamePadDevice,
		NULL);

	if (FAILED(hr))
	{
		return DIENUM_STOP;
	}

	// ���̓t�H�[�}�b�g�̎w�� Specifying the input format
	device = *parameter->GamePadDevice;
	hr = device->SetDataFormat(&c_dfDIJoystick);

	if (FAILED(hr))
	{
		return DIENUM_STOP;
	}

	// �v���p�e�B�̐ݒ� Property settings
	if (SetUpGamePadProperty(device) == false)
	{
		return DIENUM_STOP;
	}

	// �������x���̐ݒ� Coordination level setting
	if (SetUpCooperativeLevel(device) == false)
	{
		return DIENUM_STOP;
	}

	// ���������J�E���g Count the number of discoveries
	parameter->FindCount++;

	return DIENUM_CONTINUE;
}

bool InitInput()
{
	// �C���^�[�t�F�[�X�쐬 Interface creation
	if (CreateInputInterface() == false)
	{
		return false;
	}

	// �f�o�C�X�쐬 Device creation
	if (CreateGamePadDevice() == false)
	{
		
		ReleaseInput();
		return false;
	}

	// ���͏��̏����� Initialization of input information
	for (int i = 0; i < ButtonKind::ButtonKindMax; i++)
	{
		g_ButtonStates[i] = ButtonState::ButtonStateNone;
	}

	return true;
}

void ReleaseInput()
{
	// �f�o�C�X�̉�� Release the device
	if (g_GamePadDevice != nullptr)
	{
		// ������~
		g_GamePadDevice->Unacquire();
		g_GamePadDevice->Release();
		g_GamePadDevice = nullptr;
	}

	// �C���^�[�t�F�[�X�̉�� Interface release
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
	// �C���^�[�t�F�[�X�쐬 Interface creation
	HRESULT ret = DirectInput8Create(
		GetModuleHandle(nullptr),	// �C���X�^���X�n���h�� Instance handle
		DIRECTINPUT_VERSION,		// DirectInput�̃o�[�W���� DirectInput version
		IID_IDirectInput8,			// �g�p����@�\ Functions to use
		(void**)&g_InputInterface,	// �쐬���ꂽ�C���^�[�t�F�[�X����p For created interface assignment
		NULL						// NULL�Œ� Null fixed
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

	// GAMEPAD�𒲂ׂ� Examine GAMEPAD
	g_InputInterface->EnumDevices(
		DI8DEVTYPE_GAMEPAD,			// ��������f�o�C�X�̎�� Type of device to search
		DeviceFindCallBack,			// �������Ɏ��s����֐� Function to execute at the time of discovery
		&parameter,					// �֐��ɓn���l Value to pass to the function
		DIEDFL_ATTACHEDONLY			// �������@ retrieval method
	);

	// JOYSTICK�𒲂ׂ� Examine JOYSTICK
	g_InputInterface->EnumDevices(
		DI8DEVTYPE_JOYSTICK,
		DeviceFindCallBack,
		&parameter,
		DIEDFL_ATTACHEDONLY
	);

	// �ǂ���������邱�Ƃ��o���Ȃ������玸�s Failure if neither can be found
	if (parameter.FindCount == 0)
	{
		return false;
	}

	int count = 0;
	// ����J�n Start control
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

	// �f�o�C�X�擾 Device acquisition
	HRESULT hr = g_GamePadDevice->GetDeviceState(sizeof(DIJOYSTATE), &pad_data);
	if (FAILED(hr))
	{
		// �ēx����J�n Start control again
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
	// �X�e�B�b�N���� Stick judgment
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

	// �\���L�[���� Cross key judgment
	if (pad_data.rgdwPOV[0] != 0xFFFFFFFF)
	{
		float rad = pad_data.rgdwPOV[0] * 3.14159265f / 180.0f;

		// �{����x��cos�Ay��sin�����ǁArgdwPOV��0���ォ��n�܂�̂ŁA Originally, x is cos and y is sin, but rgdwPOV starts from 0, so
		// cos��sin���t�ɂ��������s�������� It is convenient to reverse cos and sin
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

	 ////�\���L�[���� Cross key judgment
		// if (pad_data.rgdwPOV[0] != 0xFFFFFFFF)
		// {
		//	 //�������S�Ă����� Write all eight directions
		//	 switch (pad_data.rgdwPOV[0])
		//	 {
		//		 //�� Up
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

	// �{�^������ Button judgment
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

	// ���͏�񂩂�{�^���̏�Ԃ��X�V���� Update the button status from the input information
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
