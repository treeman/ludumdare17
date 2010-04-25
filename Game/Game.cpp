#include "Game.hpp"
#include "Tree/Game.hpp"
#include "Tree/Log.hpp"
#include "Tree/Tweaks.hpp"

#include <boost/foreach.hpp>
#include <boost/bind.hpp>

bool island_order( boost::shared_ptr<Island> i1, boost::shared_ptr<Island> i2 )
{
	return i1->num < i2->num;
}

Game::Game() : fnt( new hgeFont( "fnt/arial10.fnt" ) ), sprite_loader( new SpriteLoader() )
{
	TWEAKS->Load( "tweaks.lua" );
	
	sprite_loader->Load( "sprites.lua" );
	
	dude = sprite_loader->Get( "dude" );
	
	background.reset( new hgeSprite( 0, 0, 0, 800, 600 ) );
	background->SetColor( 0xff8e6803 );
	
	action.reset( new Action( sprite_loader->Get( "valid_action" ), sprite_loader->Get( "invalid_action" ),
		sprite_loader->Get( "action_shadow" ) ) );
		
	LoadIslands( "islands.lua" );
}

bool Game::HandleEvent( hgeInputEvent &event )
{
	action->HandleEvent( event );
	return true;
}

void Game::Update( float dt )
{
//	L_ << "begin update";
	
	action->Update( dt );
	
//	L_ << "end update";
}
void Game::Render()
{
	background->Render(0,0);
	
	BOOST_FOREACH( boost::shared_ptr<Island> i, islands ) {
		i->Render();
	}
	
	const Vec2D dude_pos = islands.at(curr_island)->GetPos();
	action->Render( dude_pos.x, dude_pos.y );
	dude->spr->Render( dude_pos.x, dude_pos.y );
	
	fnt->printf( 400, 5, HGETEXT_LEFT, "curr_island: %i", curr_island );
}

void Game::GotoNextIsland()
{
	++curr_island;
	if(curr_island > islands.size() - 1) { curr_island = islands.size() - 1; }
	else { GotoIsland(); }
}
void Game::GotoPrevIsland()
{
	if( curr_island != 0 ) {
		--curr_island;
		GotoIsland(); 
	}
}
void Game::GotoIsland()
{
	BOOST_FOREACH( Event e, islands.at(curr_island)->events ) {
		action->Add( e );
	}
}

void Game::LoadIslands( std::string lua_file ) throw( Error::lua_error & )
{
	LuaState L;
	
	if( luaL_dofile( L, lua_file.c_str() ) ) {
		const char *str = lua_tostring( L, -1 );
		lua_pop( L, -1 );
		throw( Error::lua_error( str ) );
	}
	
	lua_getglobal( L, "_G" );
	for( lua_pushnil( L ); lua_next( L, -2 ); lua_pop( L, 1 ) ) 
	{
		if( lua_istable( L, -1 ) ) 
		{
			//try to load an island
			std::string sprite_name;
			float x, y;
			float x_off = 0, y_off = 0; //dude offset
			int num;
			
			bool is_valid = true;
			
			lua_pushstring( L, "sprite_name" );
			lua_gettable( L, -2 );
			if( lua_isstring( L, -1 ) ) {
				sprite_name = lua_tostring( L, -1 );
			} else { is_valid = false; }
			lua_pop( L, 1 );
			
			lua_pushstring( L, "x" );
			lua_gettable( L, -2 );
			if( lua_isnumber( L, -1 ) ) {
				x = (float)lua_tonumber( L, -1 );
			} else { is_valid = false; }
			lua_pop( L, 1 );
			
			lua_pushstring( L, "y" );
			lua_gettable( L, -2 );
			if( lua_isnumber( L, -1 ) ) {
				y = (float)lua_tonumber( L, -1 );
			} else { is_valid = false; }
			lua_pop( L, 1 );
			
			lua_pushstring( L, "x_off" );
			lua_gettable( L, -2 );
			if( lua_isnumber( L, -1 ) ) {
				x_off = (float)lua_tonumber( L, -1 );
			}
			lua_pop( L, 1 );
			
			lua_pushstring( L, "y_off" );
			lua_gettable( L, -2 );
			if( lua_isnumber( L, -1 ) ) {
				y_off = (float)lua_tonumber( L, -1 );
			}
			lua_pop( L, 1 );
			
			lua_pushstring( L, "num" );
			lua_gettable( L, -2 );
			if( lua_isnumber( L, -1 ) ) {
				num = (int)lua_tonumber( L, -1 );
			} else { is_valid = false; }
			lua_pop( L, 1 );
			
			//everything set okay
			//I do it like this so we don't continue before poping the stack back
			if( is_valid ) {
				boost::shared_ptr<Island> island( new Island( sprite_loader->Get( sprite_name ), 
					Vec2D( x, y ), x_off, y_off, num ) );
				
				//add all events we can find
				lua_pushstring( L, "events" );
				lua_gettable( L, -2 );
				if( lua_istable( L, -1 ) ) 
				{
					for( lua_pushnil( L ); lua_next( L, -2 ); lua_pop( L, 1 ) ) 
					{
						int points = 0;
						float duration;
						std::string str;
						boost::function<void()> callback = 0;
						
						bool is_valid = true;
						
						lua_pushstring( L, "points" );
						lua_gettable( L, -2 );
						if( lua_isnumber( L, -1 ) ) {
							points = (int)lua_tonumber( L, -1 );
						}
						lua_pop( L, 1 );
						
						lua_pushstring( L, "duration" );
						lua_gettable( L, -2 );
						if( lua_isnumber( L, -1 ) ) {
							duration = (float)lua_tonumber( L, -1 );
						} else { is_valid = false; }
						lua_pop( L, 1 );
						
						lua_pushstring( L, "str" );
						lua_gettable( L, -2 );
						if( lua_isstring( L, -1 ) ) {
							str = lua_tostring( L, -1 );
						} else { is_valid = false; }
						lua_pop( L, 1 );
						
						// valid action is a string, either "next" or "prev" or empty
						lua_pushstring( L, "action" );
						lua_gettable( L, -2 );
						if( lua_isstring( L, -1 ) ) {
							std::string action = lua_tostring( L, -1 );
							if( action == "prev" ) {
								callback = boost::bind( &Game::GotoPrevIsland, this );
							}
							else if( action == "next" ) {
								callback = boost::bind( &Game::GotoNextIsland, this );
							}
						}
						lua_pop( L, 1 );
						
						if( is_valid ) {
							ActionType type;
							if( points < 0 ) {
								type = INVALID_ACTION;
							}
							else {
								type = VALID_ACTION;
							}
							island->events.push_back( Event( type, points, duration, callback ) );
						}
					}
				}
				lua_pop( L, 1 );
				
				islands.push_back( island );
			}
		}
	}
	
	std::sort( islands.begin(), islands.end(), island_order );
	BOOST_FOREACH( boost::shared_ptr<Island> i, islands ) {
		L_ << "i: " << i->num;
	}
	
	curr_island = 0;
	GotoIsland();
}
