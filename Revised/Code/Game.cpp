#include "Game.h"
bool Game::isPlayerDead() {
	Branch bSide = branchePosition[5];
	if (bSide == Branch::LEFT && playerSide == SIDE::LEFT) {
		return true;
	}
	if (bSide == Branch::RIGHT && playerSide == SIDE::RIGHT) {
		return true;
	}
	return false;
}
void Game::loadTexture(const std::string& fileName, Texture& texture) {
	if (!texture.loadFromFile(fileName)) {
		std::cout << "Cannot find " << fileName << "!\n";
		exit(-1);
	}
}
void Game::initVariables(){
	isRunning = false;
	isPaused = false;
	score = 0;
	numBranches = 6;
	volumn = 50.0f;
	messages = std::vector<Text>(M_TYPE);
	sprites = std::vector<Sprite>(TYPE);
	textures = std::vector<Texture>(TYPE);
	areAlive = std::vector<bool>(TYPE);
	speeds = std::vector<std::pair<float, float>>(TYPE);
	spawnBounds = std::vector<std::pair<float, float>>(TYPE);
	quantities = std::vector<int>(TYPE);
	branchePosition = std::vector<Branch>(numBranches);
	branches = std::vector<Sprite>(numBranches);
	soundBuffers = std::vector<SoundBuffer>(O_TYPE);
	sounds = std::vector<Sound>(O_TYPE);
	difficulty = Difficulty(Difficulty::DIFFICULTY::EASY);
}
void Game::initWindowSpritesMessages() {
	std::istringstream info = std::move(data[WINDOW]);
	std::string dummy, content;
	info >> dummy >> mode.width >> mode.height >> title >> isFullScreen;
	if (isFullScreen) {
		window.create(mode, title, Style::Fullscreen);
	}
	else {
		window.create(mode, title);
	}
	int quantity;
	float spawnLower, spawnUpper, xSpeed, ySpeed;
	for (int i = WINDOW + 1; i < FONT; i++) {
		info = std::move(data[i]);
		info >> dummy >> content;
		loadTexture(content, textures[i]);
		if (i == BACKGROUND) {
			scale = load.getScale(window, textures[i]);
		}
		if (i == CLOUD || i == INSECT) {
			info >> quantity >> spawnLower >> spawnUpper >> xSpeed >> ySpeed;
			speeds[i] = { xSpeed, ySpeed };
			spawnBounds[i] = { spawnLower, spawnUpper };
			quantities[i] = quantity;
		}
		if (i == LOG) {
			info >> xSpeed >> ySpeed;
			speeds[i] = { xSpeed, ySpeed };
		}
		sprites[i].setTexture(textures[i]);
		sprites[i].setScale(scale.first, scale.second);
		if (i == BRANCH) {
			for (int j = 0; j < numBranches; j++) {
				branches[j].setTexture(textures[i]);
				branches[j].setScale(scale.first, scale.second);
			}
		}
	}
	position.setTreePosition(sprites[TREE], window);
	int fontSize;
	info = std::move(data[FONT]);
	info >> dummy >> content >> fontSize;
	if (!font.loadFromFile(content)) {
		std::cout << "Cannot find font from " << content << "!\n";
		exit(-1);
	}
	for (int i = 0; i < M_TYPE; i++) {
		messages[i].setFont(font);
		messages[i].setFillColor(Color::White);
		switch (i) {
		case START:
			messages[i].setString("SELECT A MODE\nPRESS ENTER TO START!");
			break;
		case SCORE:
			messages[i].setString("SCORE: " + std::to_string(score));
			break;
		case TIMEOUT:
			messages[i].setString("TIMEOUT! SELECT A MODE\nPRESS ENTER TO START!");
			break;
		case DEATH:
			messages[i].setString("YOU DIED! SELECT A MODE\nPRESS ENTER TO START!");
			break;
		case PAUSE:
			messages[i].setString("PAUSED!\nPRESS P TO RESUME!");
			break;
		case EASY:
			messages[i].setString("EASY");
			break;
		case HARD:
			messages[i].setString("HARD");
			break;
		}
		messages[i].setScale(scale.first, scale.second);
		messages[i].setCharacterSize(fontSize);
		auto messageBounds = messages[i].getLocalBounds();
		if (i != SCORE && i!=EASY && i!=HARD) {
			messages[i].setOrigin(messageBounds.left + messageBounds.width / 2, messageBounds.top + messageBounds.height / 2);
			messages[i].setPosition(mode.width / 2, mode.height / 2);
		}
		else if(i==SCORE) {
			messages[i].setPosition(0.125/3*mode.width, 0.125/3*mode.height);
		}
		else {
			messages[i].setOrigin(messageBounds.left + messageBounds.width / 2, messageBounds.top + messageBounds.height / 2);
			if (i == EASY) {
				messages[i].setFillColor(Color::Green);
				messages[i].setPosition(0.25 * mode.width, 0.25 * mode.height);
			}
			else {
				messages[i].setFillColor(Color::Red);
				messages[i].setPosition(0.75 * mode.width, 0.25 * mode.height);
			}
		}
	}
}
void Game::initTimeBar() {
	timeRemaining = difficulty.timeLimit;
	float startWidth = 5.0 * mode.width / 24;
	float startHeight = mode.height/16.0;
	widthPerSecond = startWidth / timeRemaining;
	timeBar.setSize(Vector2f(startWidth, startHeight));
	timeBar.setFillColor(Color::Red);
	timeBar.setPosition((mode.width / 2 - startWidth / 2), (sprites[TREE].getLocalBounds().height*scale.second)+10);
}
void Game::initSound() {
	std::string dummy,name, content;
	for (int i = 0; i < data.size(); i++) {
		auto info = std::move(data[i]);
		info >> dummy >> name >> content;
		switch (name[0]) {
		case 'd':
		case 'D':
			if (!soundBuffers[DEATH_SOUND].loadFromFile(content)) {
				std::cout << "Cannot find " << content << "!\n";
				exit(-1);
			}
			sounds[DEATH_SOUND].setBuffer(soundBuffers[DEATH_SOUND]);
			sounds[DEATH_SOUND].setVolume(volumn/2);
			break;
		case 'c':
		case 'C':
			if (!soundBuffers[CHOP].loadFromFile(content)) {
				std::cout << "Cannot find " << content << "!\n";
				exit(-1);
			}
			sounds[CHOP].setBuffer(soundBuffers[CHOP]);
			sounds[CHOP].setVolume(volumn);
			break;
		case 'o':
		case 'O':
			if (!soundBuffers[OUT_OF_TIME].loadFromFile(content)) {
				std::cout << "Cannot find " << content << "!\n";
				exit(-1);
			}
			sounds[OUT_OF_TIME].setBuffer(soundBuffers[OUT_OF_TIME]);
			sounds[OUT_OF_TIME].setVolume(volumn/1.2);
			break;
		}
	}
}
void Game::updateBranches(int diff) {
	for (int i = numBranches - 1; i > -1; i--) {
		branchePosition[i] = branchePosition[i - 1];
	}
	int r = 0 + rand()%diff;
	switch (r)
	{
	case 0:
		branchePosition[0] = Branch::LEFT;
		break;
	case 1:
		branchePosition[0] = Branch::RIGHT;
		break;
	default:
		branchePosition[0] = Branch::NONE;
		break;
	}
}
void Game::timeIncrement() {
	float timeIncrement = timeRemaining*1.0 / score * 5.0f;
	if(timeIncrement<0.1)
		timeIncrement = 0.1;
	if (timeRemaining + timeIncrement >= difficulty.timeLimit) {
		timeRemaining = difficulty.timeLimit;
	}
	else {
		timeRemaining += timeIncrement;
	}
}
Game::Game(const std::string&fileName) {
	initVariables();
	data = load.readFile(fileName);
	initWindowSpritesMessages();
	data = load.readSound(fileName);
	initSound();
	initTimeBar();
}
void Game::start() {
	Clock clock;
	std::vector<Speed>cloudSpeed(quantities[CLOUD]);
	std::vector<Speed>insectSpeed(quantities[INSECT]);
	std::vector<Sprite>clouds(quantities[CLOUD]);
	std::vector<Sprite>insects(quantities[INSECT]);
	for (int i = 0; i < clouds.size(); i++) {
		clouds[i].setTexture(textures[CLOUD]);
		clouds[i].setScale(sprites[CLOUD].getScale());
	}
	for (int i = 0; i < insects.size(); i++) {
		insects[i].setTexture(textures[INSECT]);
		insects[i].setScale(sprites[INSECT].getScale());
	}
	bool leftKey = false;
	bool rightKey = false;
	while (window.isOpen()) {
		Event event;
		while (window.pollEvent(event)) {
			if (event.type == Event::Closed) {
				window.close();
			}
			if (!isRunning && event.type == Event::MouseButtonPressed) {
				if (event.mouseButton.button == Mouse::Left) {
					Vector2i mousePos = Mouse::getPosition(window);
					if (position.isModeSelected(mousePos, messages[EASY])) {
						difficulty = Difficulty(Difficulty::DIFFICULTY::EASY);
					}
					if (position.isModeSelected(mousePos, messages[HARD])) {
						difficulty = Difficulty(Difficulty::DIFFICULTY::HARD);
					}
				}
			}
			if (event.type == Event::KeyPressed) {
				if (event.key.code == Keyboard::Escape) {
					window.close();
				}
				if (event.key.code == Keyboard::Return && !isRunning) {
					isRunning = true;
					isTimeout = false;
					isDead = false;
					score = 0;
					int side = 0 + rand() % 2;
					if (!side) {
						branchePosition[0] = Branch::LEFT;
					}
					else {
						branchePosition[0] = Branch::RIGHT;
					}
					for (int i = 1; i < numBranches; i++) {
						branchePosition[i] = Branch::NONE;
					}
					 position.setDeadPosition(sprites[RIP], window);
				}
				if (event.key.code == Keyboard::P && isRunning) {
					isPaused = !isPaused;
				}
				if (isRunning && !isPaused&&!isDead&&!isTimeout&&event.key.code == Keyboard::Left&&!leftKey) {
					playerSide = SIDE::LEFT;
					score += difficulty.bonusRate;
					timeIncrement();
					updateBranches(difficulty.branchInterval);
					sprites[LOG].setPosition(sprites[PLAYER].getPosition());
					sounds[CHOP].play();
					leftKey = true;
				}
				if (isRunning && !isPaused && !isDead && !isTimeout && event.key.code == Keyboard::Right && !rightKey) {
					playerSide = SIDE::RIGHT;
					score += difficulty.bonusRate;
					timeIncrement();
					updateBranches(difficulty.branchInterval);
					sprites[LOG].setPosition(sprites[PLAYER].getPosition());
					sounds[CHOP].play();
					rightKey = true;
				}
			}
			if (event.type == Event::KeyReleased) {
				if (event.key.code == Keyboard::Left) {
					leftKey = false;
				}
				if (event.key.code == Keyboard::Right) {
					rightKey = false;
				}
			}
		}
		if (isRunning) {
			for (int i = BRANCH; i < TYPE; i++) {
				areAlive[i] = true;
			}
		}
		Time deltaTime = clock.restart();
		window.clear();
		for (int i = WINDOW + 1; i < TYPE; i++) {
			if (i >= 3 && !areAlive[i]) {
				position.setDeadPosition(sprites[i],window);
				if (i == CLOUD) {
					for (int j = 0; j < clouds.size(); j++) {
						position.setCloudPosition(clouds[j], window, spawnBounds[i].first, spawnBounds[i].second);
						cloudSpeed[j] = Speed(speeds[i].first, speeds[i].second);
					}
				}
				if (i == INSECT) {
					for (int j = 0; j < insects.size(); j++) {
						position.setInsectPosition(insects[j], window, spawnBounds[i].first, spawnBounds[i].second);
						insectSpeed[j] = Speed(speeds[i].first, speeds[i].second);
					}
				}
			}
			if (i != CLOUD && i != INSECT&&i!=AXE) {
				window.draw(sprites[i]);
				auto curr = sprites[i].getPosition();
				curr.x += speeds[i].first * deltaTime.asSeconds();
				curr.y += speeds[i].second * deltaTime.asSeconds();
				sprites[i].setPosition(curr);
				if (curr.x >= window.getSize().x || curr.x < -100 || curr.y >= window.getSize().y || curr.y < -100) {
					position.setDeadPosition(sprites[i], window);
				}
			}
			else {
				for (int j = 0; j < clouds.size(); j++) {
					window.draw(clouds[j]);
				}
				for (int j = 0; j < insects.size(); j++) {
					window.draw(insects[j]);
				}
			}
			
			if (isRunning&&!isPaused&&!isTimeout&&!isDead) {
				if (isPlayerDead()) {
					sounds[DEATH_SOUND].play();
					isDead = true;
					isRunning = false;
				}
				if (Keyboard::isKeyPressed(Keyboard::Left)) {
					position.setAxePosition(sprites[AXE], sprites[TREE], scale, playerSide);
					window.draw(sprites[AXE]);
				}
				if (Keyboard::isKeyPressed(Keyboard::Right)) {
					position.setAxePosition(sprites[AXE], sprites[TREE], scale, playerSide);
					window.draw(sprites[AXE]);
				}
				timeRemaining -= deltaTime.asSeconds()/6.0;
				timeBar.setSize(Vector2f(widthPerSecond * timeRemaining, timeBar.getSize().y));
				if (i == CLOUD) {
					for (int j = 0; j < clouds.size(); j++) {
						auto curr = clouds[j].getPosition();
						curr.x -= cloudSpeed[j].x * deltaTime.asSeconds();
						curr.y += cloudSpeed[j].y * deltaTime.asSeconds();
						clouds[j].setPosition(curr);
						if (position.isCloudDead(clouds[j], window)) {
							position.setCloudPosition(clouds[j], window, spawnBounds[i].first, spawnBounds[i].second);
							cloudSpeed[j] = Speed(speeds[i].first, speeds[i].second);
						}
					}

				}
				if (i == INSECT) {
					for (int j = 0; j < insects.size(); j++) {
						auto curr = insects[j].getPosition();
						curr.x += insectSpeed[j].x * deltaTime.asSeconds();
						curr.y += insectSpeed[j].y * deltaTime.asSeconds();
						insects[j].setPosition(curr);
						if (position.isInsectDead(insects[j], window)) {
							position.setInsectPosition(insects[j], window, spawnBounds[i].first, spawnBounds[i].second);
							insectSpeed[j] = Speed(speeds[i].first, speeds[i].second);
						}
					}
	
				}
				if (i == PLAYER) {
					position.setPlayerPosition(sprites[PLAYER], sprites[TREE], scale,playerSide);
				}
				if (i == BRANCH) {
					for (int j = 0; j < numBranches; j++) {
						position.setBranchPosition(branches[j], sprites[TREE], scale, branchePosition[j], j* sprites[TREE].getLocalBounds().height* scale.second / numBranches);
					}
					for (int j = 0; j < numBranches; j++) {
						window.draw(branches[j]);
					}
				}
			}
			
		}
		window.draw(sprites[TREE]);
		if (!isRunning&&!isDead&&!isTimeout) {
			window.draw(messages[START]);
		}
		if(isRunning) {
			messages[SCORE].setString("SCORE: " + std::to_string(score));
			window.draw(messages[SCORE]);
			window.draw(timeBar);
			if (timeRemaining <= 0) {
				isTimeout = true;
				isRunning = false;
				sounds[OUT_OF_TIME].play();
			}
		}
		if (isPaused) {
			window.draw(messages[PAUSE]);
			window.draw(messages[SCORE]);
		}
		if (isTimeout) {
			window.draw(messages[TIMEOUT]);
			window.draw(messages[SCORE]);
			initTimeBar();
		}
		if (isDead) {
			window.draw(messages[DEATH]);
			position.setPlayerPosition(sprites[RIP], sprites[TREE], scale, playerSide);
			position.setDeadPosition(sprites[PLAYER], window);
			window.draw(sprites[RIP]);
			window.draw(messages[SCORE]);
			initTimeBar();
		}
		if (!isRunning) {
			window.draw(messages[EASY]);
			window.draw(messages[HARD]);
			if (difficulty.type==Difficulty::EASY) {
				position.setSelectionPosition(selection, messages[EASY],scale);
			}
			else {
				position.setSelectionPosition(selection, messages[HARD],scale);
			}
			window.draw(selection);
		}
		window.display();
	}
}