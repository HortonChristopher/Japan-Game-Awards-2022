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
	Button_1,
	Button_2,
	Button_3,
	Button_4,
	Button_5,
	Button_6,
	Button_7,
	Button_8,
	Button_9,
	Button_10,
	Button_11,
	Button_12,
	Button_13,
	Button_14,
	Button_15,
	Button_16,
	Button_17,
	Button_18,
	Button_19,
	Button_20,
	Button_21,
	Button_22,
	Button_23,
	Button_24,
	Button_25,
	Button_26,
	Button_27,
	Button_28,
	Button_29,
	Button_30,
	Button_31,
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
