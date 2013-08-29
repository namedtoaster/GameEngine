
#include "Game.h"
#include "FileManager.h"
#include "RTTI.h"
#include <string>
#include <ctime>
#include <iomanip>
#include <iostream>

#define GLFW_DLL
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtx/transform.hpp>

using namespace std;

Game::Game() : m_pConsole(nullptr), m_bConsoleEnabled(false),
	m_fDT(0.0f), m_pRenderer(nullptr), m_pInput(nullptr)
{
	// Initialize
	if(glfwInit() < 1)
	{
		throw string("Failed to init glfw");
	}

	m_plugins.SetAS(m_vm.GetScriptEngine());
	LoadPlugins();

	m_pInput->LoadKeyBindFile("bind.txt");

	RegisterScript();
}

Game::~Game()
{
	m_StateMachine.RemoveState(*this);
	glfwTerminate();
}

const std::string& Game::GetCurrentState() const
{
	return m_StateMachine.GetState().GetType()->GetName();
}

void Game::SetNextState(const std::string& state)
{
	if(m_NextState.empty())
	{
		// If the current state is null, or if the new state is different than the current
		if(!m_StateMachine.HasState() || m_StateMachine.GetState().GetType()->GetName() != state)
		{
			m_NextState = state;
		}
	}
}

int Game::Run()
{
	double prevTimeStamp = 0.0;
	double currentTimeStamp = 0.0;

	m_timer.Start();

	// Loop while the use has not quit
	while(!m_pInput->KeyDown(GLFW_KEY_ESCAPE) && !glfwWindowShouldClose(glfwGetCurrentContext()))
	{
		currentTimeStamp = m_timer.GetTime();
		m_fDT = currentTimeStamp - prevTimeStamp;
		prevTimeStamp = currentTimeStamp;

		m_pInput->Poll();

		// Update the game
		Update();

		// Render the game
		Draw();
	}

	return 0;
}

void Game::Update()
{
	static bool bSync = false;
	m_info.Update(m_fDT);

	// If There has been a state change,
	if(!m_NextState.empty())
	{
		// switch states
		m_StateMachine.SetState(m_NextState,*this);

		// Reset next state
		m_NextState.clear();
	}

	if(m_pInput->KeyDown(GLFW_KEY_F8))
	{
		m_pRenderer->EnableVSync(bSync);
		bSync = !bSync;
	}

	/*if(m_pInput->KeyDown(KeyCode::F11))
	{
		m_pRenderer->ToggleFullscreen();
	}

	if(m_pInput->GetKeyDown() == KeyCode::TILDE)
	{
		m_bConsoleEnabled = !m_bConsoleEnabled;
		m_pInput->Reset();
	}

	if(m_pInput->KeyDown(KeyCode::ESCAPE,false))
	{
		m_fEscTime += m_fDT;

		if(m_fEscTime > 1.5)
		{
			// todo: fix this somehow
			//PostQuitMessage(0);
		}
	}
	else
	{
		m_fEscTime = 0.0;
	}*/

	if(m_bConsoleEnabled)
	{
		//m_pConsole->Update(*m_pInput,m_fDT);
	}
	else
	{
		m_StateMachine.GetState().Update(*this);
	}
}

void Game::Draw()
{
	DrawFPS();
	//DrawCursor();
	//DrawSelectionRect();

	/*if(m_pInput->MouseClick(0,false))
	{
		std::stringstream stream;
		stream << m_pRenderer->GetDisplayModeStr(0) << endl;
		stream << m_pInput->MouseZ();
	}*/

	if(m_bConsoleEnabled)
	{
		//m_pConsole->Render(*m_pRenderer);
	}
	else
	{
		// render the current state

		m_StateMachine.GetState().Draw(*this);
	}

	//std::stringstream stream;
	//stream << this->GetDt() << endl;

	//m_pRenderer->DrawString(stream.str().c_str(),glm::vec3(0.0f,0.0f,-30.0f),glm::vec2(10.0f),glm::vec3(1.0f));

	// Present the screen
	m_pRenderer->Present();
}

void Game::DrawFPS()
{
	std::ostringstream out;
	out <<"FPS: " << GetFps() << endl;
	//out <<width<<"x"<<height<<endl;
	//out <<m_fDT<<endl;

	m_pRenderer->DrawString(out.str().c_str(),::glm::vec3(-90,90,-5),glm::vec2(10.0f),glm::vec3(0.0f,1.0f,0.0f));
}

void Game::DrawCursor()
{
	const ::glm::vec2& pos = m_pInput->GetTransformedMousePos();

	glm::mat4 T = glm::translate(pos.x + 2,pos.y - 3,0.0f);
	T = glm::scale(T, 7.0f,8.5f,1.0f);

	//m_pRenderer->DrawSprite("cursor",T);

}

void Game::DrawSelectionRect()
{
	Math::AABB R;
	if(m_pInput->GetSelectedRect(R))
	{
		glm::vec3 pos[] =
		{
			glm::vec3(R.min.x,R.min.y,0.0f),
			glm::vec3(R.min.x,R.max.y,0.0f),
			glm::vec3(R.max.x,R.max.y,0.0f),
			glm::vec3(R.max.x,R.min.y,0.0f),
			glm::vec3(R.min.x,R.min.y,0.0f)
		};

		m_pRenderer->DrawLine(pos,sizeof(pos) / sizeof(glm::vec3));
	}
}

IRenderer& Game::GetRenderer()
{
	return *m_pRenderer;
}
IKMInput& Game::GetInput()
{
	return *m_pInput;
}
PluginManager& Game::GetPM()
{
	return m_plugins;
}
asVM& Game::GetAs()
{
	return m_vm;
}
double Game::GetDt() const
{
	return m_fDT;
}
unsigned int Game::GetFps() const
{
	return m_info.GetFPS();
}

void Game::LoadPlugins()
{
	m_plugins.FreeAllPlugins();

	IPlugin* pPlugin = m_plugins.LoadDLL("renderer");
	assert(pPlugin != nullptr);
	assert(pPlugin->GetPluginType() == DLLType::Rendering); // check to make sure the renderer is actually the renderer

	m_pRenderer = static_cast<IRenderer*>(pPlugin);

	pPlugin = m_plugins.LoadDLL("input");
	assert(pPlugin != nullptr);
	assert(pPlugin->GetPluginType() == DLLType::Input); // check to make sure the input is actually the input plugin

	m_pInput = static_cast<IKMInput*>(pPlugin);
}

void Game::StartTimer()
{
	m_timer.Reset();
	m_timer.Start();
}

void Game::EndTimer()
{
	m_timer.Stop();
	m_fDT =  m_timer.GetTime();
}

void Game::RegisterScript()
{
	auto pEngine = m_vm.GetScriptEngine();

	DBAS(pEngine->RegisterObjectType("Game",0,asOBJ_REF | asOBJ_NOHANDLE));
	DBAS(pEngine->RegisterObjectMethod("Game","void SetNextState(const string& in)",asMETHOD(Game,SetNextState),asCALL_THISCALL));
	DBAS(pEngine->RegisterObjectMethod("Game","const string& GetCurrentState() const",asMETHOD(Game,GetCurrentState),asCALL_THISCALL));
	DBAS(pEngine->RegisterGlobalProperty("Game game",(void*)this));

	pEngine->Release();
}

GameInfo::GameInfo() : m_fTimeElapsed(0.0), m_uiFrames(0), m_uiFPS(0)
{
	
}

unsigned int GameInfo::GetFPS() const
{
	return m_uiFPS;
}

void GameInfo::Update(double dt)
{
	UpdateFPS(dt);
}

void GameInfo::UpdateFPS(double dt)
{
	static double tim1 = 0;
	static double tim2 = 0;
	static double diff = 0;

	m_fTimeElapsed += dt;
	++m_uiFrames;

	if ( m_uiFrames % 61 == 1 )
	{
		tim1 = m_fTimeElapsed;
	}
	else if(m_uiFrames % 61 == 0)
	{
		tim1 = tim2;
		tim2 = m_fTimeElapsed;
		diff = abs(tim1 - tim2);
	}

	m_uiFPS = ((61 / diff)+0.5f);
}