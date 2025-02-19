#ifndef POSITION_H
#define POSITION_H
#include "SFML/Graphics.hpp"
#include "Branch.h"
using namespace sf;
class Position
{
public:
	void setSelectionPosition(RectangleShape&, const Text&, const std::pair<float, float>&);
	void setTreePosition(Sprite&,const RenderWindow&);
	void setDeadPosition(Sprite&, const RenderWindow&);
	void setInsectPosition(Sprite&, const RenderWindow&,float, float);
	void setCloudPosition(Sprite&, const RenderWindow&,float,float);
	void setPlayerPosition(Sprite&, const Sprite&,const std::pair<float,float>&, int);
	void setAxePosition(Sprite&,const Sprite&, const std::pair<float, float>&, int);
	void setBranchPosition(Sprite&, const Sprite&, const std::pair<float, float>&, Branch,int);
	bool isCloudDead(const Sprite&, const RenderWindow&);
	bool isInsectDead(const Sprite&, const RenderWindow&);
	bool isModeSelected(const Vector2i&,const Text&);
};
#endif