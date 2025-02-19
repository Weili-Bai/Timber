#ifndef GAME_H
#define GAME_H
#include "LoadConfigFile.h"
#include "Position.h"
#include "Speed.h"
#include "Branch.h"
#include "Difficulty.h"
#include <cmath>
using namespace sf;
class Game
{
public:
	Game(const std::string&);
	void start();
private:
	const int TYPE = 10;
	const int M_TYPE = 7;
	const int S_TYPE = 2;
	const int O_TYPE = 3;
	enum MESSAGE { START, SCORE, TIMEOUT, DEATH, PAUSE,EASY,HARD };
	enum SIDE { LEFT, RIGHT };
	enum SOUND { CHOP, DEATH_SOUND, OUT_OF_TIME };
	bool isRunning;
	bool isPaused;
	bool isTimeout;
	bool isDead;
	bool isFullScreen;
	int playerSide;
	unsigned int score;
	unsigned int numBranches;
	float timeRemaining;
	float widthPerSecond;
	float volumn;
	std::pair<float, float>scale;

	RectangleShape timeBar;
	RectangleShape selection;
	VideoMode mode;
	std::string title;
	RenderWindow window;
	Font font;
	LoadConfigFile load;
	Position position;
	Difficulty difficulty;

	std::vector<std::istringstream> data;
	std::vector<Sprite>sprites;
	std::vector<Texture>textures;
	std::vector<bool>areAlive;
	std::vector<int>quantities;
	std::vector<Branch>branchePosition;
	std::vector<Sprite>branches;
	std::vector<Text>messages;
	std::vector<SoundBuffer>soundBuffers;
	std::vector<Sound>sounds;
	std::vector<std::pair<float, float>>speeds;
	std::vector<std::pair<float, float>>spawnBounds;

	bool isPlayerDead();
	void loadTexture(const std::string&, Texture&);
	void initVariables();
	void initWindowSpritesMessages();
	void initTimeBar();
	void initSound();
	void updateBranches(int);
	void timeIncrement();
};
#endif