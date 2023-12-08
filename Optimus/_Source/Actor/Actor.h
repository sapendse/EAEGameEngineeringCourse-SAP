#ifndef __ACTOR_CLASS_H
#define __ACTOR_CLASS_H

#include "..\Math\Vector3.h"
#include "GameObject.h"
#include "..\Helpers\SharedPointer.h"

namespace Optimus{

class ActorControllerI;
struct GameObject;

class Actor {	
private:
	ActorControllerI*			a_Controller;
	
public:
	SharedPointer<GameObject>	m_gameObject;
	static U64					ID;

private:
	bool						isAlive, m_isPlayer, m_rotated;

public:
	
	bool						m_physics, m_rendering, m_collision;
	
	Actor( void );
	Actor( F32 i_width, F32 i_height, bool i_physics, bool i_rendering, bool i_collision, bool i_player, const char* i_textureName, float i_rotation, float i_velocity, float i_acceleration );
	Actor ( const SharedPointer<GameObject> i_newObject,  ActorControllerI *start_Controller = 0 );

	void setActorController ( ActorControllerI *new_Controller );
	const ActorControllerI* getActorController( void ) const;

	void setIsAlive();
	bool getIsAlive();
	U64 getID() { return ID; }
	void setHasRotated( bool i_rotated ) { m_rotated = i_rotated; this->m_gameObject->setHasRotated( m_rotated ); }
	bool getHasRotated() { return m_rotated; }
	void setPlayer( bool i_isPlayer ) { m_isPlayer = i_isPlayer; }
	bool getPlayer() { return m_isPlayer; }

	void Update( void );
	~Actor();
};

}

#endif