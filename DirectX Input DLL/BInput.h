// Programmed By Bryce Mehring


#ifndef _DINPUT_
#define _DINPUT_
#pragma once

// This is the DirectX Input Plug-in class
// All that's needed is too implement the functions provided in the abstract interface
#define PLUGIN_EXPORTS
#include "PluginManager.h"

class DirectInput : public IKMInput
{
public:


	// IRender
	virtual DLLType GetType() const;
	virtual void About();

	// DirectX's version of poll 
	virtual void Poll(UINT msg, WPARAM wParam, LPARAM lparam);
	
	// Keyboard
	virtual bool KeyDown(unsigned char Key);
	virtual char GetKeyDown() const;
	virtual bool IsKeyDown() const; // true if WM_CHAR is thrown

	// Mouse
	virtual void MousePos(POINT&);
	virtual bool MouseClick(int Button);

	virtual int MouseX();
	virtual int MouseY();
	virtual int MouseZ();

	virtual void RegisterScript();

private:

	DirectInput(PluginManager& mgr);
	virtual ~DirectInput();

	// Plug-in Manager
	PluginManager& m_mgr;

	// Interface
	IDirectInput8* m_pDirectInput;

	// Keyboard
	IDirectInputDevice8* m_pKeyboard;
	char m_KeyboardState[256]; 
	char m_Key;

	bool m_keyDown;

	// Mouse
	IDirectInputDevice8* m_pMouse;
	DIMOUSESTATE2 m_MouseState;
	POINT m_MousePos;

	friend PLUGINDECL IPlugin* CreatePlugin(PluginManager&);
};

#endif
