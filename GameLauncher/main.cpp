#include "Game.h"

#include <iostream>
#include <ctime>

using namespace std;

int main(int size, char** cmd)
{
	if(size < 2)
	{
		cout<<"Invalid Number of command line arguments"<<endl;
		cin.get();
		return 1;
	}

	char* pState = cmd[1];

	//unsigned int seed = FileManager::Instance().GetSeedFromLog();

	// seed the random number generator 
	srand (time(0));

	Game myGame;
	myGame.SetNextState(pState);
	return myGame.Run();
}