#ifndef LOAD_CONFIG_FILE_H
#define LOAD_CONFIG_FILE_H
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include "SFML/Graphics.hpp"
#include "SFML/Audio.hpp"
using namespace sf;
enum readType { WINDOW,BACKGROUND,TREE,BRANCH,AXE,CLOUD,INSECT,LOG,PLAYER,RIP,FONT };
class LoadConfigFile
{
public:
	const int TYPE = 11;
	std::pair<float, float>getScale(const RenderWindow&, const Texture&);
	std::vector<std::istringstream> readFile(const std::string&);
	std::vector<std::istringstream> readSound(const std::string&);
};
#endif