#pragma once

#ifndef INPUT_H_
#define INPUT_H_


//キーボタンの種類 Key button type
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

//コントローラの初期化処理 Controller initialization process
bool InitInput();

//コントローラの終了処理 Controller termination processing
void ReleaseInput();

//コントローラーの更新処理 Controller update process
void UpdateInput();

//ボタンが押されているかの判定 Judgment whether the button is pressed
bool IsButtonPush(ButtonKind button);

//ボタンの離した瞬間を判定 Judge the moment when the button is released
bool IsButtonUp(ButtonKind button);

//ボタンの押した瞬間の判定 Judgment at the moment the button is pressed
bool IsButtonDown(ButtonKind button);

#endif
