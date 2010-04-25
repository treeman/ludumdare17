#pragma once

#include "Tree/Sprite.hpp"
#include "Tree/Vec2D.hpp"
#include "Action.hpp"

class Island {
public:
	Island( boost::shared_ptr<Sprite> spr, Vec2D pos, float x_off, float y_off, int num );
	
	Vec2D GetPos();
	
	typedef std::vector<Event> Events;
	Events events;
	
	int num;
	
	void Render();
private:
	Vec2D pos;
	boost::shared_ptr<Sprite> spr;
	float x_off, y_off;
};
