#include "Island.hpp"

Island::Island( boost::shared_ptr<Sprite> _spr, Vec2D _pos ) : spr(_spr), pos(_pos)
{
	
}

void Island::Render()
{
	spr->spr->Render( pos.x, pos.y );
}
