#include "Game.h"
using namespace sf;
int main() {
	srand(time(0));
	Game game("config.txt");
	game.start();
	return 0;
}