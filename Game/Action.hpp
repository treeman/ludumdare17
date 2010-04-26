#pragma once

#include <boost/function.hpp>

#include "Tree/Sprite.hpp"
#include "Tree/Inputhandler.hpp"

struct Event {
	Event( float _duration, std::string _action ) :
		duration( _duration ), action( _action ), timer( 0 )
	{ }
	void Reset() { timer = 0; }
	
	float duration;
	std::string action;
	float timer;
};

class Action : public Tree::InputHandler {
public:
	Action( boost::shared_ptr<Sprite> valid, boost::shared_ptr<Sprite> invalid,
		boost::shared_ptr<Sprite> shadow );
	
	void Add( Event &e );
	void Clear();
	
	//had these as callbacks but segfaulted when calling
	//who cares? just make it work
	std::string GetAction();
	
	bool HandleEvent( hgeInputEvent &e );
	
	void Update( float dt );
	void Render( float x, float y );
private:
	typedef std::vector<Event> Events;
	Events events;

	boost::shared_ptr<Sprite> valid_action;
	boost::shared_ptr<Sprite> invalid_action;
	boost::shared_ptr<Sprite> shadow;
	
	std::string action;
};
