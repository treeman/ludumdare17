#pragma once

#include <boost/function.hpp>

#include "Tree/Sprite.hpp"
#include "Tree/Inputhandler.hpp"

enum ActionType {
	VALID_ACTION,
	INVALID_ACTION
};

struct Event {
	Event( ActionType _type, int _points, float _duration, std::string _action ) :
		type( _type ), points( _points ), duration( _duration ), action( _action ), timer( 0 )
	{ }
	void Reset() { timer = 0; }
	
	ActionType type;
	int points;
	float duration;
	std::string action;
	float timer;
};

class Action : public Tree::InputHandler {
public:
	Action( boost::shared_ptr<Sprite> valid, boost::shared_ptr<Sprite> invalid,
		boost::shared_ptr<Sprite> shadow );
	
	void Add( Event &e );
	
	//had these as callbacks but segfaulted when calling
	//who cares? just make it work
	int GetPoints();
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
	
	int points;
	std::string action;
};
