
#include "Engine.h"

int main() {

    // start an engine
    Engine engine;

	while (engine.isRunning())
	{
		//update
		engine.update();
		//render
		engine.render();


	}

	return 0;
}
















