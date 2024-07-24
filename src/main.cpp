#include "SDL3/SDL.h"

#include "pong.h"

int main(int argc, char **argv)
{
		Pong app;
		try {
			app.gameLoop();
		}
		catch (const char* e) {
			std::cout << e;
			return -1;
		}

    return 0;
}