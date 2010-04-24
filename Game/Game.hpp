#pragma once

#include "Hge/Hge.hpp"
#include "Tree/GameState.hpp"
#include "Tree/Sprite.hpp"
#include "Island.hpp"

//this demo tests some things :)
class Game : public Tree::GameState {
public:
	Game();
	
	bool HandleEvent( hgeInputEvent& );
	
	void Update( float dt );
	void Render();
private:
	boost::shared_ptr<hgeSprite> background;
	boost::shared_ptr<Sprite> dude;
	
	boost::shared_ptr<hgeFont> fnt;
	
	boost::shared_ptr<SpriteLoader> sprite_loader;
	
	typedef std::vector<boost::shared_ptr<Island> > Islands;
	Islands islands;
	
	HgeObj hge;
};
