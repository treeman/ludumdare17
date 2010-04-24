#pragma once

#include "Hge/Hge.hpp"
#include "Tree/GameState.hpp"
#include "Tree/Sprite.hpp"
#include "Island.hpp"
#include "Action.hpp"

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
	
	void GotoNextIsland();
	void GotoPrevIsland();
	void GotoIsland();
	
	typedef std::vector<boost::shared_ptr<Island> > Islands;
	Islands islands;
	size_t curr_island;
	
	void LoadIslands( std::string file ) throw( Error::lua_error & );
	
	boost::shared_ptr<Action> action;
	
	HgeObj hge;
};
