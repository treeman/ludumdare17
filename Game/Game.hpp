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
	boost::shared_ptr<hgeFont> visitor;
	
	boost::shared_ptr<SpriteLoader> sprite_loader;
	
	void GotoNextIsland();
	void GotoPrevIsland();
	void GotoIsland();
	
	typedef std::vector<boost::shared_ptr<Island> > Islands;
	Islands islands;
	size_t curr_island;
	
	void Victory();
	
	void LoadIslands( std::string file ) throw( Error::lua_error & );
	
	boost::shared_ptr<Action> action;
	
	void AddPoints( int points );
	void MakeAction( std::string action );
	int points;
	
	EffectObj get_points;
	EffectObj lose_points;
	EffectObj next_island;
	EffectObj prev_island;
	EffectObj victory;
	
	boost::shared_ptr<hgeSprite> sky_spr;
	
	boost::shared_ptr<Sprite> star_spr;
	
	static const int num_stars = 100;
	static const int stars_height = 120;
	static const int sky_height = 250;
	
	float star_x[num_stars];
    float star_y[num_stars];
    float star_s[num_stars];
    float star_a[num_stars];
    
    bool has_victory;
    boost::shared_ptr<Sprite> victory_flag;
	
	HgeObj hge;
};
