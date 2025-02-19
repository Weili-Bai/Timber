#include "Position.h"
#include <iostream>
void Position::setTreePosition(Sprite& tree,const RenderWindow& window) {
	auto x = window.getSize().x;
	x = x * 1.0 / 2;
	tree.setOrigin(tree.getLocalBounds().left + tree.getLocalBounds().getSize().x / 2.0, 0);
	tree.setPosition(x, 0);
}
void Position::setDeadPosition(Sprite& sprite, const RenderWindow&window) {
	auto size = sprite.getLocalBounds().getSize();
	int temp = 0 + rand() % 100;
	float ratio = temp * 1.0 / 100;
	float y = window.getSize().y * ratio;
	sprite.setPosition(-1.5 * size.x,y);
}
void Position::setInsectPosition(Sprite& insect, const RenderWindow&window,float low,float high) {
	float y = window.getSize().y;
	int temp = 0+rand() % 101;
	float ratio = temp * 1.0 / 100;
	ratio = low + (high - low) * ratio;
	y = y * ratio;
	auto x = insect.getLocalBounds().getSize().x;
	insect.setPosition(-1.5*x, y);
}
void Position::setCloudPosition(Sprite& cloud, const RenderWindow& window, float low, float high) {
	float x = window.getSize().x;
	float y = window.getSize().y;
	int temp = 0 + rand() % 101;
	float ratio = temp * 1.0 / 100;
	ratio = low + (high - low) * ratio;
	y = y * ratio;
	auto X = cloud.getLocalBounds().getSize().x;
	cloud.setPosition(1.1 * X+x, y);
}
void Position::setPlayerPosition(Sprite& player, const Sprite& tree, const std::pair<float, float>&scale, int side) {
	const float padding = 20.5f;
	float treeX = tree.getPosition().x;
	float treeWidth = tree.getLocalBounds().width * scale.first;
	float treeHeight = tree.getLocalBounds().height * scale.second;
	float playerHeight = player.getLocalBounds().height;
	float playerWidth = player.getLocalBounds().width;
	player.setOrigin(0, playerHeight);
	float playerX;
	if (!side) {
		player.setOrigin(playerWidth, playerHeight);
		playerX = treeX - treeWidth / 2 - padding;
	}
	else {
		playerX = treeX + treeWidth / 2 + padding;
	}
	player.setPosition(playerX, treeHeight);
}
void Position::setAxePosition(Sprite& axe, const Sprite& tree, const std::pair<float, float>&scale, int side) {
	float treeHeight = tree.getLocalBounds().height*scale.second;
	float axeY = treeHeight - 50;
	float treeX = tree.getPosition().x - tree.getLocalBounds().width * scale.first / 2;
	float axeX;
	if (!side) {

		axeX = treeX;
		axe.setScale(-1*scale.first, scale.second);
	}
	else {
		axeX = treeX + tree.getLocalBounds().width * scale.first;
		axe.setScale(scale.first, scale.second);
	}
	axe.setPosition(axeX, axeY);
}
void Position::setBranchPosition(Sprite& branch, const Sprite& tree, const std::pair<float, float>& scale, Branch side,int height) {
	float treeMid = tree.getPosition().x;
	float treeWidth = tree.getLocalBounds().width * scale.first;
	treeWidth /= 2.0f;
	float branchWidth = branch.getLocalBounds().width*scale.first;
	if (side ==Branch:: LEFT) {
		branch.setPosition((treeMid-treeWidth), height);
		branch.setScale(-1 * scale.first, scale.second);
	}
	else if (side ==Branch::RIGHT) {
		branch.setPosition((treeMid + treeWidth), height);
		branch.setScale( scale.first, scale.second);
	}
	else {
		branch.setPosition(-1000, -1000);
	}
}
void Position::setSelectionPosition(RectangleShape& selection, const Text& message, const std::pair<float, float>& scale) {
	auto messageBounds = message.getLocalBounds();
	selection.setSize({ messageBounds.width + 10,messageBounds.height + 10 });
	selection.setOrigin(message.getOrigin());
	selection.setPosition(message.getPosition().x-1, message.getPosition().y-1);
	selection.setFillColor(Color::Transparent);
	selection.setOutlineColor(Color::Black);
	selection.setOutlineThickness(3);
}
bool Position::isCloudDead(const Sprite& cloud, const RenderWindow& window) {
	auto x = cloud.getPosition().x;
	auto X = cloud.getLocalBounds().getSize().x;
	auto y = cloud.getPosition().y;
	auto yBound = window.getSize().y;
	if (x + X < 0 || y>yBound) {
		return true;
	}
	return false;
}
bool Position::isInsectDead(const Sprite& insect, const RenderWindow& window) {
	auto windowX = window.getSize().x;
	auto windowY = window.getSize().y;
	auto x = insect.getPosition().x;
	auto y = insect.getPosition().y;
	if (x > windowX || y > windowY)
		return true;
	return false;
}
bool Position::isModeSelected(const Vector2i& mouse, const Text& mode) {
	auto center = mode.getPosition();
	auto width = mode.getLocalBounds().width;
	auto height = mode.getLocalBounds().height;
	float xLower = center.x - width;
	float xUpper = center.x + width;
	float yLower = center.y - height;
	float yUpper = center.y + height;
	float mouseX = mouse.x;
	float mouseY = mouse.y;
	if (mouseX >= xLower && mouseX <= xUpper && mouseY >= yLower && mouseY <= yUpper) {
		return true;
	}
	return false;
}