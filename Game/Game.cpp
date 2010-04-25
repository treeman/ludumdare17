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

Game::Game() : fnt( new hgeFont( "fnt/arial10.fnt" ) ), visitor( new hgeFont( "fnt/visitor20.fnt" ) ),
	sprite_loader( new SpriteLoader() ), get_points( "sound/get_points.wav" ), lose_points( "sound/lose_points.wav" ),
	next_island( "sound/next_island.wav" ), prev_island( "sound/prev_island.wav" ), victory( "sound/victory.wav" )
{
	TWEAKS->Load( "tweaks.lua" );
	
	sprite_loader->Load( "sprites.lua" );
	
	dude = sprite_loader->Get( "dude" );
	
	action.reset( new Action( sprite_loader->Get( "valid_action" ), sprite_loader->Get( "invalid_action" ),
		sprite_loader->Get( "action_shadow" ) ) );
		
	LoadIslands( "islands.lua" );
	
	points = 0;
	
	background.reset( new hgeSprite( 0, 0, 0, 800, 600 - sky_height ) );
	
//	background->SetColor( 0xff1fae3e, 0 );
//	background->SetColor( 0xff1fae3e, 1 );
//	background->SetColor( 0xff3863ab, 2 );
//	background->SetColor( 0xff3863ab, 3 );

	background->SetColor( 0xff1e4b5a );
	
	sky_spr.reset( new hgeSprite( 0, 0, 0, 800, sky_height ) );
	
//	sky_spr->SetColor( 4283177216, 0 );
//	sky_spr->SetColor( 0xff4c1945, 1 );
//	sky_spr->SetColor( 0xff28bf48, 2 );
//	sky_spr->SetColor( 0xff28bf48, 3 );

	sky_spr->SetColor( 0xff4c1945 );
	
	star_spr.reset( new Sprite( "gfx/star.png", 0, 0, 9, 9 ) );
	
	for( int i = 0; i < num_stars; i++ )
	{
		star_x[i] = hge->Random_Float( 0, 800 );
		star_y[i] = hge->Random_Float( 0, stars_height );
		star_s[i] = hge->Random_Float( 0.4f, 1.0f );
	}
	
	has_victory = false;
	victory_flag = sprite_loader->Get( "victory_flag" );
}

bool Game::HandleEvent( hgeInputEvent &event )
{
	if( has_victory ) {
		if( event.type == INPUT_KEYDOWN ) {
			Tree::Game::Instance()->Exit();
		}
	}
	else {
		action->HandleEvent( event );
	}
	return true;
}

void Game::Update( float dt )
{
//	L_ << "begin update";

	if( has_victory ) return;

	AddPoints( action->GetPoints() );
	MakeAction( action->GetAction() );
	
	action->Update( dt );
	
	for( int i = 0; i < num_stars; ++i )
	{
		float a = 1.0f - star_y[i] / stars_height;
		a *= hge->Random_Float( 0.6f, 1.0f );
		star_a[i] = a;
	}
	
//	L_ << "end update";
}
void Game::Render()
{
	background->Render(0,sky_height);
	
	sky_spr->Render(0,0);
	
	for( int i = 0; i < num_stars; ++i ) 
	{
		DWORD col = TWEAKS->GetFloat( "star" );
		float alpha = star_a[i] * 255.0f;
		col = SETA( col, alpha );
		star_spr->spr->SetColor( col );
		star_spr->spr->RenderEx( star_x[i], star_y[i], 0.0f, star_s[i] );
	}
		
	BOOST_FOREACH( boost::shared_ptr<Island> i, islands ) {
		i->Render();
	}
	
	const Vec2D dude_pos = islands.at(curr_island)->GetPos();
	action->Render( dude_pos.x, dude_pos.y );
	dude->spr->Render( dude_pos.x, dude_pos.y );
	
	//fnt->printf( 400, 5, HGETEXT_LEFT, "curr_island: %i", curr_island );
	
	//visitor->printf( 650, 5, HGETEXT_LEFT, "Points: %i", points );
	
	if( has_victory ) {
		victory_flag->spr->Render( 265, 147 );
	}
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
						std::string action;
						
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
						
						lua_pushstring( L, "action" );
						lua_gettable( L, -2 );
						if( lua_isstring( L, -1 ) ) {
							action = lua_tostring( L, -1 );
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
							island->events.push_back( Event( type, points, duration, action ) );
						}
					}
				}
				lua_pop( L, 1 );
				
				islands.push_back( island );
			}
		}
	}
	
	std::sort( islands.begin(), islands.end(), island_order );
	
	curr_island = 0;
	GotoIsland();
}

void Game::Victory()
{
	Tree::Game::Instance()->Pop();
}

void Game::AddPoints( int p )
{
	if( p > 0 ) {
		hge->Effect_PlayEx( get_points, 20 );
	}
	else if( p < 0 ) {
		hge->Effect_PlayEx( lose_points, 20 );
	}
	points += p;
}

void Game::MakeAction( std::string a )
{
	if( a == "" ) return;
	
	if( a == "next" ) {
		GotoNextIsland();
	}
	else if( a == "prev" ) {
		GotoPrevIsland();
	}
	else if( a == "victory" ) {
		has_victory = true;
		hge->Effect_Play( victory );
	}
}
