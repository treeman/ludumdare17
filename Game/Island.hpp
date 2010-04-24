#pragma once

#include "Tree/Sprite.hpp"
#include "Tree/Vec2D.hpp"

class Island {
public:
	Island( boost::shared_ptr<Sprite> spr, Vec2D pos );
	
	void Render();
private:
	Vec2D pos;
	boost::shared_ptr<Sprite> spr;
};
