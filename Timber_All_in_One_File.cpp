#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <iostream>
#include <ctime>
using namespace sf;

void updateBranches();
const int NUM_BRANCHES = 6;
Sprite branches[NUM_BRANCHES];
enum class side {LEFT,RIGHT,NONE};
side branchPositions[NUM_BRANCHES];
int main() {
	srand(time(0));
	VideoMode mode(1920, 1080);
	RenderWindow window(mode, "Timber",Style::Fullscreen);
	Texture tBackground;
	if (!tBackground.loadFromFile("graphics/background.png")) {
		std::cout << "Background texture load failure!\n";
		exit(-1);
	}
	Texture treeT;
	if (!treeT.loadFromFile("graphics/tree.png")) {
		std::cout << "Tree texture load failure!\n";
		exit(-1);
	}
	Sprite sBackground;
	Sprite treeS;
	Texture beeT;
	beeT.loadFromFile("graphics/insect.png");
	Sprite beeS;
	bool isBeeAlive = false;
	beeS.setTexture(beeT);
	beeS.setPosition(0, 800);
	float beeSpeed = 1.0f;
	sBackground.setTexture(tBackground);
	treeS.setTexture(treeT);
	treeS.setPosition(810, 0);
	Texture cloudT;
	cloudT.loadFromFile("graphics/cloud.png");
	Sprite cloud1, cloud2, cloud3;
	cloud1.setTexture(cloudT);
	cloud2.setTexture(cloudT);
	cloud3.setTexture(cloudT);
	cloud1.setPosition(0, 0);
	cloud2.setPosition(0, 250);
	cloud3.setPosition(0, 500);
	bool isCloud1Alive = false;
	bool isCloud2Alive = false;
	bool isCloud3Alive = false;
	float cloud1Speed = 0.0f;
	float cloud2Speed = 0.0f;
	float cloud3Speed = 0.0f;
	Clock clock;
	Font myFont;
	if (!myFont.loadFromFile("fonts/KOMIKAP_.ttf")) {
		exit(-1);
	}
	Text messege;
	messege.setFont(myFont);
	messege.setString("Press Enter to Start!");
	messege.setCharacterSize(75);
	messege.setFillColor(Color::White);
	FloatRect bound = messege.getLocalBounds();
	float x = bound.left + bound.width / 2.0f;
	float y = bound.top + bound.height / 2.0f;
	messege.setOrigin(x, y);
	
	int score = 0;
	Text scoreText;
	scoreText.setFont(myFont);
	scoreText.setFillColor(Color::Black);
	scoreText.setPosition(20, 20);
	scoreText.setCharacterSize(100);
	RectangleShape timeBar;
	float timeBarStartWidth = 400;
	float timeBarHeight = 80;
	timeBar.setSize({ timeBarStartWidth ,timeBarHeight });
	timeBar.setFillColor(Color::Red);
	timeBar.setPosition(1920 / 2 - timeBarStartWidth / 2, 980);
	Time gameTimeTotal;
	float timeRemaining = 5.0f;
	float timeBarWidthPerSecond = timeBarStartWidth / timeRemaining;
	bool isPaused = true;
	Texture branchT;
	branchT.loadFromFile("graphics/branch.png");
	Texture playerT;
	playerT.loadFromFile("graphics/player.png");
	Sprite player;
	player.setTexture(playerT);
	player.setPosition(580, 720);
	side playerSide = side::LEFT;
	Texture dieT;
	dieT.loadFromFile("graphics/RIP.png");
	Sprite die;
	die.setTexture(dieT);
	die.setPosition(600, 860);
	Texture axeT;
	axeT.loadFromFile("graphics/axe.png");
	Sprite axe;
	axe.setTexture(axeT);
	axe.setPosition(700, 830);
	const float AXE_LEFT = 700;
	const float AXE_RIGHT = 1075;
	Texture logT;
	logT.loadFromFile("graphics/log.png");
	Sprite log;
	log.setTexture(logT);
	log.setPosition(810, 720);
	bool isLogAlive = false;
	float logSpeedX = 1000;
	float logSpeedY = -1500;
	for (int i = 0; i < NUM_BRANCHES; i++) {
		branches[i].setTexture(branchT);
		branches[i].setPosition(-2000, -2000);
		branches[i].setOrigin(220, 20);
	}
	bool acceptInput = false;
	SoundBuffer buffer1,buffer2,buffer3;
	Sound death, chop, timeout;
	buffer1.loadFromFile("sound/chop.wav");
	chop.setBuffer(buffer1);
	buffer2.loadFromFile("sound/death.wav");
	death.setBuffer(buffer2);
	buffer3.loadFromFile("sound/out_of_time.wav");
	timeout.setBuffer(buffer3);
	while (window.isOpen()) {
		Event event;
		while (window.pollEvent(event)) {
			if (event.type == Event::Closed)
				window.close();
			if(event.key.code==Keyboard::Escape)
				window.close();
			if (event.type == Event::KeyReleased && !isPaused) {
				acceptInput = true;
				axe.setPosition(2000, axe.getPosition().y);
			}
		}
		if (Keyboard::isKeyPressed(Keyboard::Return)) {
			isPaused = false;
			timeRemaining = 5;
			for (int i = 0; i < NUM_BRANCHES; i++) {
				branchPositions[i] = side::NONE;
			}
			die.setPosition(675, 2000);
			acceptInput = true;
		}
		scoreText.setString("Score = " + std::to_string(score));
		if (!isPaused) {
			if (acceptInput) {
				if (Keyboard::isKeyPressed(Keyboard::Right)) {
					chop.play();
					playerSide = side::RIGHT;
					score++;
					timeRemaining += 2.0f / score + 0.15f;
					axe.setPosition(AXE_RIGHT, axe.getPosition().y);
					player.setPosition(1200, 720);
					updateBranches();
					log.setPosition(810, 720);
					logSpeedX = -5000;
					isLogAlive = true;
					acceptInput = false;
				}
				if (Keyboard::isKeyPressed(Keyboard::Left)) {
					chop.play();
					playerSide = side::LEFT;
					score++;
					timeRemaining += 2.0f / score + 0.15f;
					axe.setPosition(AXE_LEFT, axe.getPosition().y);
					player.setPosition(580, 720);
					updateBranches();
					log.setPosition(810, 720);
					logSpeedX = 5000;
					isLogAlive = true;
					acceptInput = false;
				}
			}
			Time dt = clock.restart();
			if (isLogAlive) {
				auto temp = log.getPosition();
				x = temp.x + logSpeedX * dt.asSeconds();
				y = temp.y + logSpeedY * dt.asSeconds();
				log.setPosition(x, y);
				if (temp.x < -100 || temp.x>2000) {
					isLogAlive = false;
					log.setPosition(810, 720);
				}
			}
			
			timeRemaining -= dt.asSeconds();
			timeBar.setSize({ timeBarWidthPerSecond * timeRemaining,timeBarHeight });
			if (timeRemaining <= 0.0f) {
				timeout.play();
				isPaused = true;
				messege.setString("Out of Time!\nGame Over!");
				bound = messege.getLocalBounds();
				x = bound.left + bound.width / 2;
				y = bound.top + bound.height / 2;
				messege.setOrigin(x, y);
			}
			if (!isBeeAlive) {
				beeSpeed = 200 + rand() % 200;
				float height = 500 + rand() % 500;
				beeS.setPosition(0, height);
				isBeeAlive = true;
			}
			else {
				beeS.setPosition(beeS.getPosition().x + beeSpeed * dt.asSeconds(), beeS.getPosition().y);
				if (beeS.getPosition().x > 2000) {
					isBeeAlive = false;
				}
			}
			if (!isCloud2Alive) {
				cloud2Speed = rand() % 200;
				float height = -150 + rand() % 300;
				cloud2.setPosition(1920, height);
				isCloud2Alive = true;
			}
			else {
				auto p = cloud2.getPosition();
				cloud2.setPosition(p.x - cloud2Speed * dt.asSeconds(), p.y);
				float width = cloudT.getSize().x;
				if (p.x <= -width)
					isCloud2Alive = false;
			}
			if (!isCloud1Alive) {
				cloud1Speed = rand() % 200;
				float height = -150 + rand() % 300;
				cloud1.setPosition(1920, height);
				isCloud1Alive = true;
			}
			else {
				auto p = cloud1.getPosition();
				cloud1.setPosition(p.x - cloud1Speed * dt.asSeconds(), p.y);
				float width = cloudT.getSize().x;
				if (p.x <= -width)
					isCloud1Alive = false;
			}
			if (!isCloud3Alive) {
				cloud3Speed = rand() % 200;
				float height = -150 + rand() % 300;
				cloud3.setPosition(1920, height);
				isCloud3Alive = true;
			}
			else {
				auto p = cloud3.getPosition();
				cloud3.setPosition(p.x - cloud3Speed * dt.asSeconds(), p.y);
				float width = cloudT.getSize().x;
				if (p.x <= -width)
					isCloud3Alive = false;
			}
			for (int i = 0; i < NUM_BRANCHES; i++) {
				float height = i * 150;
				if (branchPositions[i] == side::LEFT) {
					branches[i].setPosition(610, height);
					branches[i].setRotation(180);
				}
				else if (branchPositions[i] == side::RIGHT) {
					branches[i].setPosition(1330, height);
				}
				else {
					branches[i].setPosition(3000, height);
				}
			}
			if (branchPositions[5] == playerSide) {
				isPaused = true;
				acceptInput = false;
				die.setPosition(525, 760);
				player.setPosition(2000, 660);
				messege.setString("Squished!\nGame Over!");
				bound = messege.getLocalBounds();
				x = bound.left + bound.width / 2;
				y = bound.top + bound.height / 2;
				messege.setOrigin(x, y);
				death.play();
			}
		}
		messege.setPosition(1920 * 0.5f, 1080 * 0.5f);
		
		window.clear();
		
		window.draw(sBackground);
		window.draw(cloud1);
		window.draw(cloud2);
		window.draw(cloud3);
		for (int i = 0; i < NUM_BRANCHES; i++) {
			window.draw(branches[i]);
		}
		window.draw(treeS);
		window.draw(player);
		window.draw(axe);
		window.draw(log);
		window.draw(die);
		window.draw(beeS);
		window.draw(scoreText);
		window.draw(timeBar);
		if(isPaused)
			window.draw(messege);
		window.display();
	}
	return 0;
}
void updateBranches() {
	for (int j = NUM_BRANCHES - 1; j > 0; j--) {
		branchPositions[j] = branchPositions[j - 1];
	}
	int num = rand() % 3;
	switch (num) {
	case 0:
		branchPositions[0] = side::LEFT;
		break;
	case 1:
		branchPositions[0] = side::RIGHT;
		break;
	case 2:
		branchPositions[0] = side::NONE;
		break;
	}
	
}