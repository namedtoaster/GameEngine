
#include "main.h"

InputTestApp::InputTestApp(HINSTANCE hInstance,const string& winCaption) : IBaseEngine(hInstance,winCaption)
{
	ZeroMemory(buffer,32);
}
InputTestApp::~InputTestApp()
{
	UIManager::Delete();
}

int InputTestApp::Run()
{
	Load();

	/*RedirectIOToConsole();

	char Buffer[256];
	std::cin.getline(Buffer,256,'}');
	system("cls");*/

	CheckBoxData checkBox1;
	checkBox1.m_checked = false;
	checkBox1.m_checked = true;
	checkBox1.m_time = 0;
	checkBox1.m_Callback = &InputTestApp::LuaConsole;
		
	checkBox1.m_pos[0].x = 200;
	checkBox1.m_pos[0].y = 200;

	checkBox1.m_pos[1].x = 300;
	checkBox1.m_pos[1].y = 300;

	checkBox1.m_str = "Open Lua Console";

	UIManager& ui = UIManager::Instance();
	ui.AddCheckBox(checkBox1);

	while(Update())
	{
		StartCounter();
		m_pInput->Poll();

		ui.Update(this->m_fDT);

		m_pRendering->Begin();
			
		ui.Render();
		/*if(m_pInput->MouseClick(0))
		{
			POINT p;
			m_pInput->MousePos(p);
			wsprintf(buffer,"Mouse X: %d\nMouse Y:%d",p.x,p.y);
			m_pRendering->DrawString(buffer);
		}*/

		m_pRendering->End();
		m_pRendering->Present();
		EndCounter();
	}

	return 0;
}

void InputTestApp::Load()
{
	PluginManager& pluginManager = PluginManager::Instance();
	m_pInput  = static_cast<IKMInput*>(pluginManager.LoadDLL("DirectX Input DLL.dll"));
	m_pRendering = static_cast<IRenderingPlugin*>(pluginManager.LoadDLL("DX9 Rendering.dll"));
}


// The DLL files do not like the singletons. This is why when we pass a singleton
// instance into a dll, it appears null. I need to check this out more...

// the current fix gets rid of using a singleton and uses Reference Counting.

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nShowCmd)
{
	// I want to put all of this Initialization into a dll
	g_pEngine = new InputTestApp(hInstance,"Lua Console");

	int returnCode = g_pEngine->Run();

	PluginManager::Delete();
	g_pEngine->Release();

	return returnCode;
}
				
																								  