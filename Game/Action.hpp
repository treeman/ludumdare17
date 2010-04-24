#pragma once

#include <boost/function.hpp>

#include "Tree/Sprite.hpp"
#include "Tree/Inputhandler.hpp"

enum ActionType {
	VALID_ACTION,
	INVALID_ACTION
};

struct Event {
	Event( ActionType _type, float _duration, boost::function<void()> _callback ) :
		type( _type ), duration( _duration ), callback( _callback ), timer( 0 )
	{ }
	
	ActionType type;
	float duration;
	boost::function<void()> callback;
	float timer;
};

class Action : public Tree::InputHandler {
public:
	Action( boost::shared_ptr<Sprite> valid, boost::shared_ptr<Sprite> invalid,
		boost::shared_ptr<Sprite> shadow );
	
	void Add( Event &e );
	void Add( ActionType type, float duration, boost::function<void()> callback );
	
	bool HandleEvent( hgeInputEvent &e );
	
	void Update( float dt );
	void Render( float x, float y );
private:
	typedef std::vector<Event> Events;
	Events events;

	boost::shared_ptr<Sprite> valid_action;
	boost::shared_ptr<Sprite> invalid_action;
	boost::shared_ptr<Sprite> shadow;
};
