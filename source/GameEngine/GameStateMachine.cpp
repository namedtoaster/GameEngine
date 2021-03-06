#include "Game.h"
#include "Log.h"
#include "ResourceFileLoader.h"
#include <GLFW/glfw3.h>

using namespace std;

GameStateMachine::GameStateMachine() : m_pCurrentState(nullptr) {}

void GameStateMachine::SetState(const std::string& state, Game& game)
{
	RemoveState(game);

	IPlugin* pPlugin = game.GetPM().LoadPlugin("./plugin/" + state + '/' + state);
	assert(pPlugin->GetPluginType() == DLLType::Game);

	LoadResourceFile(string(pPlugin->GetName()) + ".r",game,"./plugin/" + string(pPlugin->GetName()));

	m_pCurrentState = static_cast<IGameState*>(pPlugin);
	m_pCurrentState->Init(game);

	// Update change to log
	Log::Instance().Write("Changing state to: " + state);

	// update window caption
	glfwSetWindowTitle(glfwGetCurrentContext(),state.c_str());

}

void GameStateMachine::RemoveState(Game& game)
{
	if(m_pCurrentState != nullptr)
	{
		m_pCurrentState->Destroy(game);
		m_pCurrentState = nullptr;
		game.GetPM().FreePlugin(DLLType::Game);
	}
}
