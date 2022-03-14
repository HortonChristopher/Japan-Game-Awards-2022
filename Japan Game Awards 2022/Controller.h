#pragma once

#ifndef INPUT_H_
#define INPUT_H_


//�L�[�{�^���̎�� Key button type
enum ButtonKind
{
	UpButton,
	DownButton,
	LeftButton,
	RightButton,
	Button_A,
	Button_B,
	Button_X,
	Button_Y,
	Button_LB,
	Button_RB,
	Button_LeftMenu,
	Button_RightMenu,
	Button_LeftStick,
	Button_RightStick,
	ButtonKindMax,
};

enum ButtonState
{
	ButtonStateNone,
	ButtonStateDown,
	ButtonStatePush,
	ButtonStateUp,
	ButtonStateMax,
};

//�R���g���[���̏��������� Controller initialization process
bool InitInput();

//�R���g���[���̏I������ Controller termination processing
void ReleaseInput();

//�R���g���[���[�̍X�V���� Controller update process
void UpdateInput();

//�{�^����������Ă��邩�̔��� Judgment whether the button is pressed
bool IsButtonPush(ButtonKind button);

//�{�^���̗������u�Ԃ𔻒� Judge the moment when the button is released
bool IsButtonUp(ButtonKind button);

//�{�^���̉������u�Ԃ̔��� Judgment at the moment the button is pressed
bool IsButtonDown(ButtonKind button);

#endif
