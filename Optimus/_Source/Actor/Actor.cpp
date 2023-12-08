#include "Actor.h"
#include "ActorController.h"
#include "CollisionController.h"
#include "..\World\World.h"
#include "..\Collision\Collision.h"

Optimus::Actor::Actor( void ) :
	m_gameObject( ),
	m_physics( false ),
	m_rendering( false ),
	m_collision( false ),
	a_Controller( 0 ),
	isAlive( true )
{
	ID++;
	m_gameObject = SharedPointer<GameObject>();
	SharedPointer<Actor> temp = SharedPointer<Actor>( this );
	World::getInstance()->addToWorld( temp );
}

Optimus::Actor::Actor( F32 i_width, F32 i_height, bool i_physics, bool i_rendering, bool i_collision, bool i_player, const char* i_textureName, float i_rotation, float i_velocity, float i_acceleration ) :
	m_physics( i_physics ),
	m_rendering( i_rendering ),
	m_collision( i_collision ),
	a_Controller( 0 ),	
	m_isPlayer( i_player ),
	isAlive( true )
{
	ID++;
	m_gameObject = SharedPointer<GameObject>( OPT_NEW GameObject( Vector3( 0.0f,0.0f,0.0f ), Vector3( i_velocity ), Vector3( i_acceleration ), 1.0f, i_width, i_height, i_textureName, i_rotation, i_player, m_physics, m_rendering, m_collision ) );
	SharedPointer<Actor> temp = SharedPointer<Actor>( this );
	World::getInstance()->addToWorld( temp);
}

Optimus::Actor:: Actor( const SharedPointer<GameObject> i_newObject, ActorControllerI *start_Controller ) :
	m_gameObject( i_newObject ),
	a_Controller( start_Controller ),
	isAlive( true )
{
	ID++;
	m_gameObject = SharedPointer<GameObject>( i_newObject );
	SharedPointer<Actor> temp = SharedPointer<Actor>( this );
	World::getInstance()->addToWorld( temp );
}

void Optimus::Actor::setActorController ( ActorControllerI *new_Controller ) {

	a_Controller = new_Controller;
}

const Optimus::ActorControllerI* Optimus::Actor::getActorController( void ) const {
	return a_Controller;
}

void Optimus::Actor::setIsAlive()
{
	if( isAlive == true )
		isAlive = false;
	else
		isAlive = true;
}

bool Optimus::Actor::getIsAlive()
{
	return isAlive;
}

void Optimus::Actor::Update( void )
{
	if( a_Controller )
		a_Controller->UpdateActor( *this );
}

Optimus::Actor::~Actor()
{
	//delete (this->a_Controller);
	//delete( &this->m_gameObject );
	//if( &this->m_gameObject )
	//	delete( &this->m_gameObject );
}