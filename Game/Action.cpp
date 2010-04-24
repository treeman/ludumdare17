#include "Action.hpp"
#include "Tree/Log.hpp"

Action::Action( boost::shared_ptr<Sprite> valid, boost::shared_ptr<Sprite> invalid,
	boost::shared_ptr<Sprite> _shadow ) :
	valid_action( valid ), invalid_action( invalid ), shadow( _shadow )
{
	
}
void Action::Add( Event &e )
{
	events.push_back( e );
}

bool Action::HandleEvent( hgeInputEvent &e )
{
	if( events.empty() ) return true;
	
	if( e.type == INPUT_KEYDOWN ) {
		
		events.begin()->callback();
		events.erase( events.begin() );
	}
	
	return false;
}
void Action::Update( float dt )
{
	if( events.empty() ) return;
	
	Events::iterator e = events.begin();
	e->timer += dt;
	if( e->duration < e->timer ) {
		events.erase( e );
	}
}
void Action::Render( float x, float y )
{
	if( events.empty() ) return;
	
	shadow->spr->Render( x + 1, y - 9 );
	
	Events::iterator e = events.begin();
	
	const float ys = 20;
	
	if( e->type == VALID_ACTION ) {
		valid_action->spr->Render( x, y - 10 );
	}
	else {
		invalid_action->spr->Render( x, y - 10 );
	}
}
