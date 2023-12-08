#ifndef GAME_OBJECT_H
#define GAME_OBJECT_H

#include "../Math/Vector3.h"
#include "..\Utilities\Target.h"
#include "CollisionController.h"
#include "TriggerCallback.h"
#include "MessageCallback.h"
#include "..\Collision\AABB.h"
#include "..\Renderer\cMesh.h"
#include <vector>

namespace Optimus
{

	class CollisionControllerI;
	class TriggerCallbackI;
	class MessageCallbackI;

struct GameObject{
		private:
			//Hot Data
			Optimus::cMesh*					m_mesh;
			const char*						m_texture;
			Vector3							m_position, m_velocity, m_acceleration, m_worldPosition, m_minBound, m_maxBound, m_nextPos;
			static U64						ID;
			F32								m_mass, m_width, m_height, m_rotation, m_speed;
			CollisionControllerI*			a_CollisionController;
			TriggerCallbackI*				t_Controller;
			MessageCallbackI*				m_Controller;
			bool							m_isPlayer, m_isGrounded, m_rotated, m_physics, m_renderer, m_collision, m_isOnHardSurface;
			std::string						m_tag;

		public:						
			//Constructors

			//Main Constructor
			GameObject( Vector3& i_position, Vector3 i_velocity, Vector3 i_acceleration, float i_mass, F32 i_width, F32 i_height, const char* i_texture, float i_rotation, bool i_isPlayer, bool i_physics, bool i_renderer, bool i_collision ) :
			m_position ( i_position ),
			m_velocity ( i_velocity ),
			m_acceleration ( i_acceleration ),
			m_mass( i_mass ),
			m_width( i_width ),
			m_height( i_height ),
			m_texture( i_texture ),
			m_rotation( i_rotation ),
			m_isPlayer( i_isPlayer ),
			m_physics( i_physics ),
			m_renderer( i_renderer ),
			m_collision( i_collision ),
			m_mesh( NULL ),
			m_isOnHardSurface( false )
			{
				ID++;
				a_CollisionController = NULL;
				t_Controller = NULL;
				m_Controller = NULL;
			}

			//Constructors
			//Default Constructor
			GameObject()
			{
				m_position = Vector3( 0.0f ,0.0f ,0.0f );
				m_velocity = Vector3( 0.0f ,0.0f ,0.0f );
				m_acceleration = Vector3( 0.0f ,0.0f ,0.0f );

				m_mass = 1.0f;
				m_texture = NULL;
				ID++;
				m_rotation = 0.0f;
			}

			~GameObject()
			{
				delete this->a_CollisionController;
				delete this->t_Controller;
			}

			//Getters
			Vector3 getPosition(){ return	m_position; }
			Vector3 getNextPos() { return	m_nextPos; }
			Vector3 getVelocity(){ return m_velocity; }
			Vector3 getAcceleration(){ return m_acceleration; }
			float getMass() { return m_mass; }
			const char* getTextureName() { return m_texture; }
			F32 getWidth() { return m_width; }
			F32 getHeight() { return m_height; }
			F32 GetSpeed() { return m_speed; }
			U64 getID() { return ID; }
			float getRotation() { return m_rotation; }
			bool getPlayer() { return m_isPlayer; }
			bool getGrounded() { return m_isGrounded; }
			bool getHasRotated() { return m_rotated; }
			bool getPhysics() { return m_physics; }
			bool getRenderer() { return m_renderer; }
			bool getCollision() { return m_collision; }
			bool getSurface() { return m_isOnHardSurface; }
			cMesh*	getMesh() { return m_mesh; }
			Vector3 getMinBound() { return m_minBound; }
			Vector3 getMaxBound() { return m_maxBound; }
			Vector3 getWorldPosition() 
			{ 
				if( m_mesh != NULL )
					return m_worldPosition; 
				else
				{
					m_worldPosition = ( m_minBound + m_maxBound ) / 2.0f;
					return m_worldPosition;
				}
			}
			std::string getTag() { return m_tag; }

			//Setters
			void setPosition( const Vector3& i_position ) 
			{ 
				m_position = i_position; 
				if( m_mesh != NULL )
				{
					m_mesh->setPosition( D3DXVECTOR3( m_position.x(), m_position.y(), m_position.z() ) ); 
					m_minBound = m_mesh->m_minBound;
					m_maxBound = m_mesh->m_maxBound;
				}
			}
			void setNextPos( const Vector3& i_nextPos ) { m_nextPos = i_nextPos; }
			void setTranslation( const Vector3& i_position ) 
			{ 
				m_position = i_position; 
				if( m_mesh != NULL )
				{
					m_mesh->setTranslation( D3DXVECTOR3( m_position.x(), m_position.y(), m_position.z() ) ); 
					m_minBound = m_mesh->m_minBound;
					m_maxBound = m_mesh->m_maxBound;
				}
			}
			void setWorldPosition( const Vector3& i_worldPosition )
			{
				m_worldPosition = i_worldPosition;
				if( m_mesh != NULL )
				{
					m_mesh->setWorldPosition( m_worldPosition );
					m_minBound = m_mesh->m_minBound;
					m_maxBound = m_mesh->m_maxBound;
				}
			}
			void SetSpeed( F32 i_speed ) { m_speed = i_speed; }
			void setBounds( const Vector3& i_minBound, const Vector3& i_maxBound ) { m_minBound = i_minBound; m_maxBound = i_maxBound; }
			void setVelocity( const Vector3& i_velocity ) { m_velocity = i_velocity; }
			void setHasRotated ( bool i_rotated ) { m_rotated = i_rotated; }
			bool setPhysics( bool i_physics ) { m_physics = i_physics; }
			bool setRenderer( bool i_renderer ) { m_renderer = i_renderer; }
			bool setCollision( bool i_collision ) { m_collision = i_collision; }
			void setSurface( bool i_value ) { m_isOnHardSurface = i_value; }
			void setAcceleration( const Vector3& i_acceleration ){ m_acceleration = i_acceleration; }
			void setRotation( float i_rotation ) 
			{ 
				m_rotation = i_rotation;  
				i_rotation = -i_rotation;
				if( m_mesh != NULL )
					m_mesh->setRotation( i_rotation ); 
			}
			void setMass( float i_mass ) { m_mass = i_mass; }
			void setPlayer( bool i_isPlayer ) { m_isPlayer = i_isPlayer; }
			void setGrounded( bool i_isGrounded ) { m_isGrounded = i_isGrounded; }
			void setCollisionController( CollisionControllerI* new_Controller ) { a_CollisionController = new_Controller; }
			const CollisionControllerI* getCollisionController( void ) const { return a_CollisionController; }
			void setTriggerController( TriggerCallbackI *new_Controller ){ t_Controller = new_Controller; }
			const TriggerCallbackI* getTriggerController( void ) const  { return t_Controller; }
			void setMessageController( MessageCallbackI *new_Controller ){ m_Controller = new_Controller; }
			const MessageCallbackI* getMessageController( void ) const  { return m_Controller; }
			void setMesh( Optimus::cMesh* i_mesh ) 
			{ 
				m_mesh =  i_mesh; 
				if( m_mesh != NULL )
				{
					m_worldPosition = ( m_mesh->m_minBound + m_mesh->m_maxBound ) / 2.0f;
					m_minBound = m_mesh->m_minBound;
					m_maxBound = m_mesh->m_maxBound;
				}
			}
			void setTag( std::string i_tag )
			{
				m_tag = i_tag;
			}

			void Update( SharedPointer<GameObject>& i_collider, SharedPointer<GameObject>& i_collideTo, Vector3& i_normal )
			{
					if( a_CollisionController != NULL )
						a_CollisionController->OnCollision( i_collider, i_collideTo, i_normal );
			}

			void OnTrigger( SharedPointer<GameObject>& i_gameObject, SharedPointer<GameObject>& i_trigger, bool i_input )
			{
				if( i_trigger->getTriggerController() != NULL )
				{
					if( i_input == true )
					{
						const_cast<TriggerCallbackI*>(i_trigger->getTriggerController())->OnEnter( i_gameObject, i_trigger );
					}
					if( i_input == false )
					{
						const_cast<TriggerCallbackI*>(i_trigger->getTriggerController())->OnExit( i_gameObject, i_trigger );
					}
				}
			}
		};

}

#endif