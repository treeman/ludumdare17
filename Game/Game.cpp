#include "Game.hpp"
#include "Tree/Game.hpp"
#include "Tree/Log.hpp"
#include "Tree/Tweaks.hpp"

#include <boost/foreach.hpp>

Game::Game() : fnt( new hgeFont( "fnt/arial10.fnt" ) ), sprite_loader( new SpriteLoader() )
{
	TWEAKS->Load( "tweaks.lua" );
	
	sprite_loader->Load( "sprites.lua" );
	
	dude = sprite_loader->Get( "dude" );
	
	background.reset( new hgeSprite( 0, 0, 0, 800, 600 ) );
	background->SetColor( 0xff8e6803 );
	
	boost::shared_ptr<Island> island( new Island( sprite_loader->Get( "tropic" ), Vec2D( 40, 70 ) ) );
	islands.push_back( island );
}

bool Game::HandleEvent( hgeInputEvent &event )
{
	if( event.type == INPUT_KEYDOWN ) {
		switch( event.key ) {
		}
	}
	
	return true;
}

void Game::Update( float dt )
{
//	L_ << "begin update";
	
//	L_ << "end update";
}
void Game::Render()
{
	background->Render(0,0);
	
	BOOST_FOREACH( boost::shared_ptr<Island> i, islands ) {
		i->Render();
	}
	
	dude->spr->Render(10,10);
}
