#include "Island.hpp"

Island::Island( boost::shared_ptr<Sprite> _spr, Vec2D _pos, float _x_off, float _y_off, int _num ) : 
	spr(_spr), pos(_pos), x_off(_x_off), y_off(_y_off), num(_num)
{
	
}

Vec2D Island::GetPos()
{
	return Vec2D( pos.x + x_off, pos.y + y_off );
}

void Island::Render()
{
	spr->spr->Render( pos.x, pos.y );
}
