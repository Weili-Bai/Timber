#include "LoadConfigFile.h"
std::vector<std::istringstream> LoadConfigFile::readFile(const std::string&fileName) {
	std::ifstream input;
	input.open(fileName);
	std::vector<std::istringstream>result(TYPE);
	std::string line;
	std::istringstream ss;
	while (!input.eof()) {
		getline(input, line);
		ss = std::istringstream(line);
		switch (line[0]) {
		case'w':
		case 'W':
			result[WINDOW] = std::move(ss);
			break;
		case't':
		case'T':
			result[TREE] = std::move(ss);
			break;
		case 'b':
		case 'B':
			result[BRANCH] = std::move(ss);
			break;
		case 'a':
		case 'A':
			result[AXE] = std::move(ss);
			break;
		case 'c':
		case 'C':
			result[CLOUD] = std::move(ss);
			break;
		case 'i':
		case 'I':
			result[INSECT] = std::move(ss);
			break;
		case 'p':
		case 'P':
			result[LOG] = std::move(ss);
			break;
		case 'm':
		case 'M':
			result[PLAYER] = std::move(ss);
			break;
		case 'd':
		case'D':
			result[RIP] = std::move(ss);
			break;
		case 'l':
		case 'L':
			result[BACKGROUND] = std::move(ss);
			break;
		case'f':
		case'F':
			result[FONT] = std::move(ss);
			break;
		default:
			break;
		}
	}
	input.close();
	return result;
}
std::pair<float, float>LoadConfigFile::getScale(const RenderWindow& window, const Texture& texture) {
	auto windowSize = window.getSize();
	auto textureSize = texture.getSize();
	float xScale = windowSize.x * 1.0 / textureSize.x;
	float yScale = windowSize.y * 1.0 / textureSize.y;
	return { xScale,yScale };
}
std::vector<std::istringstream> LoadConfigFile::readSound(const std::string&fileName) {
	std::vector<std::istringstream>result;
	std::ifstream input;
	std::string line;
	std::istringstream ss;
	input.open(fileName);
	while (!input.eof()) {
		getline(input, line);
		if (line[0] == 'S' || line[0] == 's') {
			ss = std::istringstream(line);
			result.push_back(std::move(ss));
		}
	}
	input.close();
	return result;
}