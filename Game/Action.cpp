#include "Action.hpp"
#include "Tree/Log.hpp"
#include "Tree/Tweaks.hpp"

Action::Action( boost::shared_ptr<Sprite> valid, boost::shared_ptr<Sprite> invalid,
	boost::shared_ptr<Sprite> _shadow ) :
	valid_action( valid ), invalid_action( invalid ), shadow( _shadow )
{
	valid->spr->SetHotSpot( valid->spr->GetWidth() / 2, valid->spr->GetHeight() / 2 );
	invalid->spr->SetHotSpot( invalid->spr->GetWidth() / 2, invalid->spr->GetHeight() / 2 );
	shadow->spr->SetHotSpot( shadow->spr->GetWidth() / 2, shadow->spr->GetHeight() / 2 );
}
void Action::Add( Event &e )
{
	events.push_back( e );
}
void Action::Clear()
{
	events.clear();
}

std::string Action::GetAction()
{
	return action;
}

bool Action::HandleEvent( hgeInputEvent &e )
{
	if( events.empty() ) return true;
	
	if( e.type == INPUT_KEYDOWN ) {
		
		Events::iterator e = events.begin();
		
		action = e->action;
		
		events.erase( e );
	}
	
	return false;
}
void Action::Update( float dt )
{
	action = "";
	
	if( events.empty() ) return;
	
	events[0].timer += dt;
	if( events[0].duration < events[0].timer ) {
		events[0].Reset();
		if( events.size() > 1 ) {
			events.push_back( events[0] );
			events.erase( events.begin() );
		}
	}
}
void Action::Render( float x, float y )
{
	if( events.empty() ) return;
	
	x += TWEAKS->GetFloat( "button_x_offset" );
	
	const float button_offset = TWEAKS->GetFloat( "button_y_offset" );
	
	Events::iterator e = events.begin();
	
	const float ys = TWEAKS->GetFloat( "button_distance" );
	
	const float half = e->duration / 2;
	
	const float shadow_size = std::abs( half - e->timer );
	
	if( shadow_size != 0 ) {
		shadow->spr->RenderEx( x, y + button_offset + 1, 0, shadow_size );
	}
	
	const float dist = ( half - std::abs( half - e->timer ) ) * ys;
	
	const float button_y = y - dist + button_offset;
	
	if( e->action == "prev" ) {
		invalid_action->spr->Render( x, button_y );
	}
	else {
		valid_action->spr->Render( x, button_y );
	}
}
