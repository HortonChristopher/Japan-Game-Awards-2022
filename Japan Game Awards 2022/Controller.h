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
