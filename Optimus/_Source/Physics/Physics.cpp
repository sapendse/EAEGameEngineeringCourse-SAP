#include "Physics.h"
#include "..\Timer\TimeSystem.h"
#include "..\Renderer\cFPSCamera.h"

void Optimus::Physics::clearList()
{
	const unsigned int count = m_listOfPhysicsObjects.size();
	for( unsigned int i= 0; i< count; i++ )
	{
		if( m_listOfPhysicsObjects[i] )
			m_listOfPhysicsObjects[i].~SharedPointer();
	}
	m_listOfPhysicsObjects.resize( 0 );
}

bool Optimus::Physics::Initialize()
{
	clearList();
	m_gravity = Vector3( 0.0f, -400.0f, 0.0f );
	return true;
}

bool Optimus::Physics::addActor( SharedPointer<GameObject> i_gameObject )
{
	m_listOfPhysicsObjects.push_back( i_gameObject );
	return true;
}

bool Optimus::Physics::removeActor( SharedPointer<GameObject> i_gameObject )
{
	unsigned int searchCount = m_listOfPhysicsObjects.size();
	unsigned int to_be_removed;
	for( unsigned int i=0; i<searchCount; i++ )
	{
		if( m_listOfPhysicsObjects[i]->getID() == i_gameObject->getID() )
		{
			to_be_removed = i;
			m_listOfPhysicsObjects.erase( m_listOfPhysicsObjects.begin() + to_be_removed );
			break;
		}
	}
	return true;
}

bool Optimus::Physics::Update( F32 deltaTime )
{
	if( m_listOfPhysicsObjects.size() != 0 )
	{
		unsigned int count = m_listOfPhysicsObjects.size();
		for( unsigned int i=0; i< count; i++ )
		{
			if( m_listOfPhysicsObjects[i] )
			{
				if( m_listOfPhysicsObjects[i]->getPhysics() )
				{
					if( !m_listOfPhysicsObjects[i]->getGrounded() )
						tempAcc = m_listOfPhysicsObjects[i]->getAcceleration() + m_gravity;
					else
						tempAcc = m_listOfPhysicsObjects[i]->getAcceleration();
					tempVel = m_listOfPhysicsObjects[i]->getVelocity();
					tempPos = m_listOfPhysicsObjects[i]->getPosition();
					tempVel += tempAcc * deltaTime;
					tempPos += tempVel * deltaTime;

					m_listOfPhysicsObjects[i]->setVelocity( tempVel );
					m_listOfPhysicsObjects[i]->setPosition( tempPos );

					tempPos += ( tempVel * 2.1f ) * deltaTime;
					m_listOfPhysicsObjects[i]->setNextPos( tempPos );
				}
			}
		}
	}
	return true;
}

bool Optimus::Physics::PostUpdate( F32 deltaTime, float i_collisionTime )
{
	if( m_listOfPhysicsObjects.size() != 0 )
	{
		unsigned int count = m_listOfPhysicsObjects.size();

		for( unsigned int i=0; i< count; i++ )
		{
			if( m_listOfPhysicsObjects[i] )
			{	
				tempAcc = m_listOfPhysicsObjects[i]->getAcceleration();
				tempVel = m_listOfPhysicsObjects[i]->getVelocity();
				tempPos = m_listOfPhysicsObjects[i]->getPosition();
				tempVel += tempAcc * ( deltaTime * i_collisionTime );
				tempPos += tempVel * ( deltaTime * i_collisionTime );

				m_listOfPhysicsObjects[i]->setVelocity( tempVel );
				m_listOfPhysicsObjects[i]->setPosition( tempPos );
			}
		}
	}
	return true;
}

bool Optimus::Physics::Shutdown()
{
	//clearList();
	m_listOfPhysicsObjects.~vector();
	tempAcc.~Vector3();
	tempVel.~Vector3();
	tempPos.~Vector3();
	delete m_instance;
	return true;
}