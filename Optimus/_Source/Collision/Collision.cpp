#include "Collision.h"
#include "..\Utilities\Utilities.h"
#include "..\Renderer\cTPSCamera.h"
#include "..\Renderer\cScene.h"
#include "..\Utilities\DebugLines.h"

#define EPSILON 0.2f

OctreeBuilder::OctreeCreator* OctreeBuilder::OctreeCreator::m_instance = NULL;

void Optimus::Collision::clearList()
{
	const unsigned int count = m_listOfCollidableObjects.size();
	for( unsigned int i= 0; i< count; i++ )
	{
		if( m_listOfCollidableObjects[i] )
			m_listOfCollidableObjects[i].~SharedPointer();
	}
	m_listOfCollidableObjects.resize( 0 );
}

bool Optimus::Collision::Initialize()
{
	clearList();
	collision = false;
	goodTimeToStop = 10000.0f;
	normal = Vector3( 0.0f );
	OctreeBuilder::OctreeCreator::getInstance()->ReadOctree();
	return true;
}

bool Optimus::Collision::addActor( SharedPointer<GameObject> i_gameObject )
{
	m_listOfCollidableObjects.push_back( i_gameObject );
	return true;
}

bool Optimus::Collision::removeActor( SharedPointer<GameObject> i_gameObject )
{
	unsigned int searchCount = m_listOfCollidableObjects.size();
	unsigned int to_be_removed;
	for( unsigned int i=0; i<searchCount; i++ )
	{
		if( m_listOfCollidableObjects[i]->getID() == i_gameObject->getID() )
		{
			to_be_removed = i;
			m_listOfCollidableObjects.erase( m_listOfCollidableObjects.begin() + to_be_removed );
			break;
		}
	}
	return true;
}

Optimus::Vector3 Optimus::Collision::findRelativeVelocity( Vector3& i_first, Vector3& i_second )
{
		return ( i_first - i_second );
}


void Optimus::Collision::setAABB( unsigned int i, unsigned int j, F32 deltaTime )
{
	Vector3 tempAccI, tempVelJ, tempVelI, tempPosNextI, tempPosI;
	tempVelI = findRelativeVelocity( m_listOfCollidableObjects[i]->getVelocity() , m_listOfCollidableObjects[j]->getVelocity() ); //Calculate relative velocity of i to j.
	
	//Next position calculation
	tempAccI = m_listOfCollidableObjects[i]->getAcceleration();
	tempPosNextI = m_listOfCollidableObjects[i]->getPosition();

	//tempVelI += tempAccI * deltaTime;
	tempPosNextI += tempVelI * deltaTime;

	//Blow up box 1 and blow down and move box 2
	aabb1.m_halfX +=  aabb2.m_halfX;
	aabb1.m_halfY +=  aabb2.m_halfY;

	aabb2.m_halfX -= aabb2.m_halfX;
	aabb2.m_halfY -= aabb2.m_halfY;
	aabb2.m_center = tempPosNextI;
}

Optimus::Vector3 Optimus::Collision::setOOBB( unsigned int i_count, unsigned int i, unsigned int j, F32 deltaTime )
{
	float pi = 3.141529f;
	Matrix ObjToWorld1 = Matrix::getTransformMatrix( m_listOfCollidableObjects[j]->getRotation(), m_listOfCollidableObjects[j]->getPosition() );
	Matrix ObjToWorld2 = Matrix::getTransformMatrix( m_listOfCollidableObjects[i]->getRotation(), m_listOfCollidableObjects[i]->getPosition() );
	Matrix WorldToObject1 = ObjToWorld1.inverse();
	Matrix WorldToObject2 = ObjToWorld2.inverse();
	Matrix RotWorldToObj1 = WorldToObject1.getRotationMatrix();
	Matrix RotWorldToObj2 = WorldToObject2.getRotationMatrix();
	//aabb1 is always in his own space; aabb2 changes.
	if( i_count == 1 )
	{
		aabb1.m_center = aabb1.m_center * WorldToObject1; // aabb1 -> j; aabb2 -> i
		aabb2.m_center = aabb2.m_center * WorldToObject1;
		Vector3 temp1 = m_listOfCollidableObjects[j]->getVelocity() * RotWorldToObj1;
		Vector3 temp2 = m_listOfCollidableObjects[i]->getVelocity() * RotWorldToObj1;
		Vector3 temp3 = m_listOfCollidableObjects[i]->getAcceleration() *RotWorldToObj1;

		Vector3 temp = findRelativeVelocity( temp2, temp1 ); //Relative velocity of i wrt j
		temp += temp3 * deltaTime;
		Vector3 nextPositionI = m_listOfCollidableObjects[i]->getPosition() * WorldToObject1 + temp* deltaTime;

		aabb2.m_rotation -= aabb1.m_rotation;
		aabb2.m_halfX = aabb2.m_halfX * fabs(cos( aabb2.m_rotation * pi / 180.0f )) + aabb2.m_halfY * fabs(sin( aabb2.m_rotation * pi /180.0f ));
		aabb2.m_halfY = aabb2.m_halfY * fabs(cos( aabb2.m_rotation * pi / 180.0f )) + aabb2.m_halfX * fabs(sin( aabb2.m_rotation * pi /180.0f ));
		aabb2.m_center = nextPositionI;

		aabb1.m_rotation -= aabb1.m_rotation;
		aabb1.m_halfX += aabb2.m_halfX;
		aabb1.m_halfY += aabb2.m_halfY;

		aabb2.m_halfX -= aabb2.m_halfX;
		aabb2.m_halfY -= aabb2.m_halfY;
		return ( m_listOfCollidableObjects[i]->getPosition() * WorldToObject1 );
	}
	else
	{
		aabb1.m_center = aabb2.m_center * WorldToObject2; //aabb1 -> i; aabb2 -> j
		aabb2.m_center = aabb1.m_center * WorldToObject2;
		Vector3 temp1 = m_listOfCollidableObjects[j]->getVelocity() * RotWorldToObj2;
		Vector3 temp2 = m_listOfCollidableObjects[i]->getVelocity() * RotWorldToObj2;
		Vector3 temp3 = m_listOfCollidableObjects[j]->getAcceleration() *RotWorldToObj2;

		Vector3 temp = findRelativeVelocity( temp1, temp2 ); //Relative velocity of j wrt i
		temp += temp3 * deltaTime;
		Vector3 nextPositionI = m_listOfCollidableObjects[j]->getPosition() * WorldToObject2 + temp*deltaTime;

		aabb2.m_rotation -= aabb1.m_rotation;				
		aabb2.m_halfX = aabb2.m_halfX * fabs(cos( aabb2.m_rotation * pi / 180.0f )) + aabb2.m_halfY * fabs(sin( aabb2.m_rotation * pi /180.0f ));
		aabb2.m_halfY = aabb2.m_halfY * fabs(cos( aabb2.m_rotation * pi / 180.0f )) + aabb2.m_halfX * fabs(sin( aabb2.m_rotation * pi /180.0f ));

		aabb1.m_rotation -= aabb1.m_rotation;
		aabb1.m_halfX += aabb2.m_halfX;
		aabb1.m_halfY += aabb2.m_halfY;	

		aabb2.m_halfX -= aabb2.m_halfX;
		aabb2.m_halfY -= aabb2.m_halfY;
		aabb2.m_center = nextPositionI;

		return ( m_listOfCollidableObjects[j]->getPosition() * WorldToObject2 );
	}
}

void Optimus::Collision::resetAABB( unsigned int i, unsigned int j )
{
	aabb1.m_center = m_listOfCollidableObjects[j]->getPosition();
	aabb1.m_halfX = m_listOfCollidableObjects[j]->getWidth()/2;
	aabb1.m_halfY = m_listOfCollidableObjects[j]->getHeight()/2;
	aabb1.m_halfZ = 0.0f;
	aabb1.m_rotation = m_listOfCollidableObjects[j]->getRotation();

	aabb2.m_center = m_listOfCollidableObjects[i]->getPosition();
	aabb2.m_halfX = m_listOfCollidableObjects[i]->getWidth()/2;
	aabb2.m_halfY = m_listOfCollidableObjects[i]->getHeight()/2;
	aabb2.m_halfZ = 0.0f;
	aabb2.m_rotation = m_listOfCollidableObjects[i]->getRotation();
}

bool Optimus::Collision::Update( F32 deltaTime )
{
	normal = Vector3( 0.0f );
	//for( unsigned int i =0; i< m_listOfCollidableObjects.size(); i++ )
	//{
	//	if( m_listOfCollidableObjects[i]->getCollision() )
	//	{
	//		for( unsigned int j =i+1; j <m_listOfCollidableObjects.size(); j++ )
	//		{
	//			if( m_listOfCollidableObjects[j]->getCollision() )
	//			{
	//				//Reset the boxes to default for the frame.
	//				resetAABB( i, j );
	//				if( isCloseEnough( m_listOfCollidableObjects[i]->getRotation(), 0.0f, 0.001f ) && isCloseEnough( m_listOfCollidableObjects[j]->getRotation(), 0.0f, 0.001f ) )
	//				{
	//					float ticks = 0;

	//					collision = false;
	//					//Calculates relative velocity, and next position.
	//					setAABB( i,j, deltaTime );

	//					collision = checkAABBCollision( m_listOfCollidableObjects[i]->getPosition() );
	//			
	//					if( collision == true )
	//					{
	//						respondToCollision( i, j );
	//					}
	//				}
	//				else
	//				{
	//					bool collision1, collision2;
	//					Vector3 prevPos;
	//					//Calculates everything related to OOBB; once.
	//					prevPos = setOOBB( 1, i, j, deltaTime );
	//					collision1 = checkAABBCollision( prevPos );

	//					resetAABB( i , j );

	//					prevPos = setOOBB( 2, i,j, deltaTime );
	//					collision2 = checkAABBCollision( prevPos );

	//					if( collision1 && collision2 )
	//					{
	//						collision = true;
	//						Matrix ObjToWorld1 = Matrix::getTransformMatrix( m_listOfCollidableObjects[j]->getRotation(), m_listOfCollidableObjects[j]->getPosition() );
	//						Matrix ObjToWorld2 = Matrix::getTransformMatrix( m_listOfCollidableObjects[i]->getRotation(), m_listOfCollidableObjects[i]->getPosition() );
	//						Matrix WorldToObject1 = ObjToWorld1.inverse();
	//						Matrix WorldToObject2 = ObjToWorld2.inverse();
	//						-normal;
	//						m_listOfCollidableObjects[j]->Update( m_listOfCollidableObjects[j], normal );
	//						-normal;
	//						normal = normal * ObjToWorld2;
	//						normal = normal * WorldToObject1;
	//						m_listOfCollidableObjects[i]->Update( m_listOfCollidableObjects[i], normal );
	//					}
	//					else
	//					{
	//						collision = false;
	//					}
	//				}
	//			}
	//		}
	//	}
	//}
	float width, height, depth;
	Optimus::Vector3 obj1MaxBound, obj1MinBound, obj2Center, obj2Half;
	if( m_listOfCollidableObjects.size() != 0 )
	{
		for( unsigned int i = 0; i < m_listOfCollidableObjects.size() - 1; ++i )
		{
			if( m_listOfCollidableObjects[i]->getCollisionController() != NULL )
			{
				for( unsigned int j = i + 1; j < m_listOfCollidableObjects.size(); ++j )
				{
					if( m_listOfCollidableObjects[j]->getCollisionController() != NULL )
					{
						obj2Center = ( m_listOfCollidableObjects[i]->getMesh()->m_maxBound + m_listOfCollidableObjects[i]->getMesh()->m_minBound ) / 2.0f;
						obj2Half = ( m_listOfCollidableObjects[i]->getMesh()->m_maxBound - m_listOfCollidableObjects[i]->getMesh()->m_minBound ) / 2.0f;
						width = obj2Half.x();
						height = obj2Half.y();
						depth = obj2Half.z();
						obj1MaxBound = m_listOfCollidableObjects[j]->getMesh()->m_maxBound;
						obj1MinBound = m_listOfCollidableObjects[j]->getMesh()->m_minBound;

						obj1MaxBound.x( obj1MaxBound.x() + width/2.0f );
						obj1MaxBound.y( obj1MaxBound.y() + height/2.0f );
						obj1MaxBound.z( obj1MaxBound.z() + depth/2.0f );

						obj1MinBound.x( obj1MinBound.x() - width/2.0f );
						obj1MinBound.y( obj1MinBound.y() - height/2.0f );
						obj1MinBound.z( obj1MinBound.z() - depth/2.0f );

						if( obj1MinBound.x() <= m_listOfCollidableObjects[i]->getPosition().x() && m_listOfCollidableObjects[i]->getPosition().x() <= obj1MaxBound.x() &&
							obj1MinBound.z() <= m_listOfCollidableObjects[i]->getPosition().z() && m_listOfCollidableObjects[i]->getPosition().z() <= obj1MaxBound.z() )
						{
							if( m_listOfCollidableObjects[j]->getCollisionController() )
							{
								m_listOfCollidableObjects[j]->Update( m_listOfCollidableObjects[j], m_listOfCollidableObjects[i], normal );
							}
						}
					}
					else
					{
						obj2Center = ( m_listOfCollidableObjects[i]->getMaxBound() + m_listOfCollidableObjects[i]->getMinBound() ) / 2.0f;
						obj2Half = ( m_listOfCollidableObjects[i]->getMaxBound() - m_listOfCollidableObjects[i]->getMinBound() ) / 2.0f;
						width = obj2Half.x();
						height = obj2Half.y();
						depth = obj2Half.z();
						obj1MaxBound = m_listOfCollidableObjects[j]->getMaxBound();
						obj1MinBound = m_listOfCollidableObjects[j]->getMinBound();

						obj1MaxBound.x( obj1MaxBound.x() + width/2.0f );
						obj1MaxBound.y( obj1MaxBound.y() + height/2.0f );
						obj1MaxBound.z( obj1MaxBound.z() + depth/2.0f );

						obj1MinBound.x( obj1MinBound.x() - width/2.0f );
						obj1MinBound.y( obj1MinBound.y() - height/2.0f );
						obj1MinBound.z( obj1MinBound.z() - depth/2.0f );

						if( obj1MinBound.x() <= m_listOfCollidableObjects[i]->getPosition().x() && m_listOfCollidableObjects[i]->getPosition().x() <= obj1MaxBound.x() &&
							obj1MinBound.z() <= m_listOfCollidableObjects[i]->getPosition().z() && m_listOfCollidableObjects[i]->getPosition().z() <= obj1MaxBound.z() )
						{
							if( m_listOfCollidableObjects[j]->getTriggerController() )
							{
								m_listOfCollidableObjects[j]->OnTrigger( m_listOfCollidableObjects[i], m_listOfCollidableObjects[j], true );
							}
						}
					}
				}
			}
		}
	}
	bool noPlayer = true;

	for( unsigned int i = 0; i < m_listOfCollidableObjects.size(); i++ )
	{
		if( m_listOfCollidableObjects[i] != NULL )
		{
			if( m_listOfCollidableObjects[i]->getPlayer() )
			{
				noPlayer = false;
				if( m_listOfCollidableObjects[i]->getHasRotated() )
				{
					cTPSCamera::getInstance()->DealWithRotation( m_listOfCollidableObjects[i]->getRotation() );
					m_listOfCollidableObjects[i]->setHasRotated( false );
				}
				CameraCollisionDetection( i );
		
				PlayerCollisionDetection( i );	
		
				cTPSCamera::getInstance()->MoveTowards();

				m_listOfCollidableObjects[i]->setGrounded( cTPSCamera::getInstance()->m_collisionHappened_Ground );
			}
			if( i == 1 ) //AI POV
			{
				/*noPlayer = false;
				if( m_listOfCollidableObjects[i]->getHasRotated() )
				{
					cTPSCamera::getInstance()->DealWithRotation( m_listOfCollidableObjects[i]->getRotation() );
					m_listOfCollidableObjects[i]->setHasRotated( false );
				}
				CameraCollisionDetection( i );
		*/
				PlayerCollisionDetection( i );	
		/*
				cTPSCamera::getInstance()->MoveTowards();

				m_listOfCollidableObjects[i]->setGrounded( cTPSCamera::getInstance()->m_collisionHappened_Ground );*/
			}
		}
	}
	if( noPlayer )
		cTPSCamera::getInstance()->m_freeFormCamera = true;
	if( m_listOfCollidableObjects.size() == 0 )
	{
		cTPSCamera::getInstance()->m_freeFormCamera = true;
	}
	return true;
}

bool Optimus::Collision::checkAABBCollision( Vector3& i_prevPos )
{
	float xenter, xexit, yenter, yexit;
	bool result = false, xDecide = false, yDecide = false;
	//Object has a velocity in X
	if( aabb2.getCenter().x() - i_prevPos.x() != 0 )
	{
		xenter = ( ( aabb1.getCenter().x()-aabb1.m_halfX ) - ( i_prevPos.x() ) ) / ( ( aabb2.getCenter().x() ) - ( i_prevPos.x() ) );
		xexit = ( ( aabb1.getCenter().x()+aabb1.m_halfX ) - ( i_prevPos.x() ) ) / ( ( aabb2.getCenter().x() ) - ( i_prevPos.x() ) );
	}
	//Object moving vertically
	else
	{
		xenter = xexit = aabb2.getCenter().x();
	}

	//Object has a velocity in Y
	if( aabb2.getCenter().y() - i_prevPos.y() != 0 )
	{
		yenter = ( ( aabb1.getCenter().y()-aabb1.m_halfY ) - ( i_prevPos.y() ) ) / ( ( aabb2.getCenter().y() ) - ( i_prevPos.y() ) );
		yexit = ( ( aabb1.getCenter().y()+aabb1.m_halfY ) - ( i_prevPos.y() ) ) / ( ( aabb2.getCenter().y() ) - ( i_prevPos.y() ) );
	}
	//Object moving horizontally
	else
	{
		yenter = yexit = aabb2.getCenter().y();
	}

	//Swap if assumed direction is wrong.
	if( xenter > xexit )
	{
		float temp = xexit;
		xexit = xenter;
		xenter = temp;

		xDecide = true;
	}
	//Swap if assumed direction is wrong.
	if( yenter > yexit )
	{
		float temp = yexit;
		yexit = yenter;
		yenter = temp;

		yDecide = true;
	}
	

	if( ( ( xenter > 0 && xexit < 1 ) || ( yenter > 0 && yenter < 1 ) || ( yexit > 0 && yexit < 1 ) || ( xexit > 0 && xexit < 1 ) || ( xenter == xexit ) || ( yenter == yexit ) ) )
	{
		//Possibility of Collision
		//Special Cases
		if( xenter == xexit ) //Moving vertically
		{
			if( yenter > 0 && yenter < 1 )
			{
				if( xenter > ( aabb1.getCenter().x() - aabb1.m_halfX ) && xenter < ( aabb1.getCenter().x() + aabb1.m_halfX ) )
				{
					goodTimeToStop = yenter;
					if( yDecide )
						normal = Vector3( 0, -1, 0 );
					else
						normal = Vector3( 0, 1, 0 );
					result=  true;
				}
				else
				{
					result = false;
				}
			}
		}
		else if( yenter == yexit ) //Moving horizontally
		{
			if( xenter > 0 && xenter < 1 )
			{
				if( yenter > ( aabb1.getCenter().y() - aabb1.m_halfY ) && yenter < ( aabb1.getCenter().y() + aabb1.m_halfY ) )
				{
					goodTimeToStop = xenter;
					if( xDecide )
						normal = Vector3( 1, 0, 0 );
					else
						normal = Vector3( -1, 0, 0 );
					result= true;
				}
				else
				{
					result = false;
				}
			}
		}
		if( xenter == yenter )
		{
			goodTimeToStop = xenter;
			result = true;
		}		
		if( xexit == yexit )
		{
			if( xenter == yenter )
			{
				goodTimeToStop = xenter;
				result = true;
			}
			else if( xenter > yenter )
			{
				goodTimeToStop = yenter;
				result = true;
			}
			else
			{
				goodTimeToStop = xenter;
				result= true;
			}
		}
		//Normal Cases
		if( yexit > xexit )
		{
			if( yenter > xenter  && ( ( yenter > 0 && yenter < goodTimeToStop ) || ( xenter > 0 && xenter < goodTimeToStop ) ) )
			{
				if( xexit > yenter && ( ( yenter > 0 && yenter <1 ) || ( xexit > 0 && xexit <1 ) ) )		//First Exit > Last Enter
				{
					goodTimeToStop = yenter;
					if( yDecide )
						normal = Vector3( 0.0f, -1.0f, 0.0f );
					else
						normal = Vector3( 0.0f, 1.0f, 0.0f );
					result = true;
				}
				else
				{
					result = false;
				}
			}
			else if( xenter > yenter && ( ( yenter > 0 && yenter < goodTimeToStop ) || ( xenter > 0 && xenter < goodTimeToStop ) ) )
			{
				if( xexit > xenter && ( ( xenter > 0 && xenter <1 ) || ( xexit > 0 && xexit <1 ) )  )		//First Exit > Last Enter
				{
					goodTimeToStop = xenter;
					if( xDecide )
						normal = Vector3( 1.0f, 0.0f, 0.0f );
					else
						normal = Vector3( -1.0f, 0.0f, 0.0f );
					result = true;
				}
				else
				{
					result = false;
				}
			}
		}
		else if( xexit > yexit )
		{
			if( xenter > yenter && ( ( yenter > 0 && yenter < goodTimeToStop ) || (xenter > 0 && xenter <goodTimeToStop ) ) )
			{
				if( yexit > xenter && ( ( xenter > 0 && xenter <1 ) || ( yexit > 0 && yexit <1 ) )  )		//First Exit > Last Enter
				{
					goodTimeToStop = xenter;
					if( xDecide )
						normal = Vector3( 1.0f, 0.0f, 0.0f );
					else
						normal = Vector3( -1.0f, 0.0f, 0.0f );
					result = true;
				}
				else
				{
					result = false;
				}
			}
			else if( yenter > xenter && ( ( yenter > 0 && yenter <goodTimeToStop ) || ( xenter > 0 && xenter < goodTimeToStop ) ) )
			{
				if( yexit > yenter && ( ( yenter > 0 && yenter <1 ) || ( yexit > 0 && yexit <1 ) )  )		//First Exit > Last Enter
				{
					goodTimeToStop = yenter;
					if( yDecide )
						normal = Vector3( 0.0f, -1.0f, 0.0f );
					else
						normal = Vector3( 0.0f, 1.0f, 0.0f );
					result = true;
				}
				else
				{
					result = false;
				}
			}
		}
	}
	else if( xenter < 0 && yenter < 0   && xexit > 1 && yexit > 1 ) //Object is inside.
	{
		goodTimeToStop = -1.0f;
		if( xenter > yenter )
		{
			if( xDecide )
				normal = Vector3( 1.0f, 0.0f, 0.0f );
			else
				normal = Vector3( -1.0f, 0.0f, 0.0f );
		}
		else
		{
			if( yDecide )
				normal = Vector3( 0.0f, -1.0f, 0.0f );
			else
				normal = Vector3( 0.0f, 1.0f, 0.0f );
		}
		result= true;
	}
	return result;
}

bool Optimus::Collision::Shutdown()
{
	//clearList();
	OctreeBuilder::OctreeCreator::getInstance()->Shutdown();
	m_listOfCollidableObjects.~vector();
	delete m_instance;
	return true;
}

void Optimus::Collision::setNormal( float i_x, float i_y, float i_z )
{
	normal.x( i_x );
	normal.y( i_y );
	normal.z( i_z );
}

void Optimus::Collision::respondToCollision( unsigned int i, unsigned int j )
{
	if( m_listOfCollidableObjects[i]->getCollisionController() != NULL && m_listOfCollidableObjects[j]->getCollisionController() != NULL )
	{
		Physics::getInstance()->PostUpdate( TimeSystem::getInstance()->getDeltaTime(), goodTimeToStop );
		m_listOfCollidableObjects[i]->Update( m_listOfCollidableObjects[i], m_listOfCollidableObjects[j], normal );
		-normal;
		m_listOfCollidableObjects[j]->Update( m_listOfCollidableObjects[j], m_listOfCollidableObjects[i], normal );
	}
	else if( m_listOfCollidableObjects[i]->getCollisionController() == NULL && m_listOfCollidableObjects[j]->getCollisionController() != NULL )
	{
		m_listOfCollidableObjects[j]->OnTrigger( m_listOfCollidableObjects[j], m_listOfCollidableObjects[i], true );
	}
	else if( m_listOfCollidableObjects[i]->getCollisionController() != NULL && m_listOfCollidableObjects[j]->getCollisionController() == NULL )
	{
		m_listOfCollidableObjects[i]->OnTrigger( m_listOfCollidableObjects[i], m_listOfCollidableObjects[j], true );
	}
}

int Optimus::Collision::IntersectSegmentTriangle( Vector3& p, Vector3& q, Vector3& a, Vector3& b, Vector3& c, float &u, float &v, float &w, float &t )
{
	Vector3 ab = b - a;
    Vector3 ac = c - a;
    Vector3 qp = p - q;

    // Compute triangle normal. Can be precalculated or cached if
    // intersecting multiple segments against the same triangleyo
    Vector3 n = Vector3::crossProd( ab, ac );  //Cross(ab, ac);

    // Compute denominator d. If d <= 0, segment is parallel to or points
    // away from triangle, so exit early
    float d = Vector3::dotProd( qp, n );  //Dot(qp, n);
    if (d <= 0.0f) return 0;

    // Compute intersection t value of pq with plane of triangle. A ray
    // intersects if 0 <= t. Segment intersects if 0 <= t <= 1. Delay
    // dividing by d until intersection has been found to pierce triangle
    Vector3 ap = p - a;
    t = Vector3::dotProd( ap, n );  //Dot(ap, n);
    if (t < 0.0f) return 0;
    if (t > d) return 0; // For segment; exclude this code line for a ray test

    // Compute barycentric coordinate components and test if within bounds
    Vector3 e = Vector3::crossProd( qp, ap );  //Cross(qp, ap);
    v = Vector3::dotProd( ac, e );  //Dot(ac, e);
    if (v < 0.0f || v > d) return 0;
    w = -Vector3::dotProd( ab, e );  //-Dot(ab, e);
    if (w < 0.0f || v + w > d) return 0;

    // Segment/ray intersects triangle. Perform delayed division and
    // compute the last barycentric coordinate component
    float ood = 1.0f / d;
    t *= ood;
    v *= ood;
    w *= ood;
    u = 1.0f - v - w;
    return 1; 
}

int Optimus::Collision::IntersectSegmentAABB(Vector3& p1, Vector3& p2, Vector3& center, Vector3& halfDim)
{
    Vector3 c = center;  // Box center-point
    Vector3 e = halfDim;              // Box halflength extents
    Vector3 m = (p1 + p2) * 0.5f;        // Segment midpoint
    Vector3 d = p1 - m;                 // Segment halflength vector
    m = m - c;                         // Translate box and segment to origin
    
    // Try world coordinate axes as separating axes
    float adx = abs(d.x());
    if (abs(m.x()) > e.x() + adx) return 0;
    float ady = abs(d.y());
    if (abs(m.y()) > e.y() + ady) return 0;
    float adz = abs(d.z());
    if (abs(m.z()) > e.z() + adz) return 0;

    // Add in an epsilon term to counteract arithmetic errors when segment is
    // (near) parallel to a coordinate axis
    adx += EPSILON;
    ady += EPSILON;
    adz += EPSILON;

    // Try cross products of segment direction vector with coordinate axes
    if (abs(m.y() * d.z() - m.z() * d.y()) > e.y() * adz + e.z() * ady) return 0;
    if (abs(m.z() * d.x() - m.x() * d.z()) > e.x() * adz + e.z() * adx) return 0;
    if (abs(m.x() * d.y() - m.y() * d.x()) > e.x() * ady + e.y() * adx) return 0;
    
    // No separating axis found; segment must be overlapping AABB
    return 1;
} 

void Optimus::Collision::PlayerCollisionDetection( unsigned int i_index )
{
	Optimus::Vector3 pos, lookAt, speed, groundRay, nextForPos, nextBackPos;
	UpdateInfo( i_index, pos, lookAt, speed, groundRay,nextForPos, nextBackPos );
	std::vector<OctreeBuilder::Triangle> triangleList;
	FindTrianglesToCollide( pos, groundRay, OctreeBuilder::OctreeCreator::getInstance()->m_octree->m_rootNode, triangleList );
	float u,v,w,t;
	bool groundNot = false;

	//Check On Hard Surface
	for( unsigned int i = 0; i < cSceneHandler::getInstance()->m_opaqueEnts.size(); i++ )
	{
		if( cSceneHandler::getInstance()->m_opaqueEnts[i]->m_hardSurface )
		{
			Optimus::Vector3 tempMin = cSceneHandler::getInstance()->m_opaqueEnts[i]->m_minBound;
			Optimus::Vector3 tempMax = cSceneHandler::getInstance()->m_opaqueEnts[i]->m_maxBound + Optimus::Vector3( 0.0f, 100.0f, 0.0f );
			Optimus::Vector3 tempPos = ( m_listOfCollidableObjects[i_index]->getMinBound() + m_listOfCollidableObjects[i_index]->getMaxBound() ) / 2.0f;
			if( tempPos > tempMin && tempPos < tempMax )
			{
				m_listOfCollidableObjects[i_index]->setSurface( true );
				break;
			}
		}
		else
		{
			m_listOfCollidableObjects[i_index]->setSurface( false );
		}
	}

	//cSceneHandler::getInstance()->AddDebugLine( pos, nextForPos, D3DCOLOR_XRGB( 255,0,0 ) );

	if( triangleList.size() != 0 )
	{
		for( unsigned int i = 0; i < triangleList.size(); i++ )
		{
			Optimus::Vector3 first = triangleList[i].m_first;
			Optimus::Vector3 second = triangleList[i].m_second;
			Optimus::Vector3 third = triangleList[i].m_third;

			if( IntersectSegmentTriangle( pos, groundRay, first, second, third, u, v, w, t ) == 1 )
			{
				cTPSCamera::getInstance()->m_collisionHappened_Ground = true;
					
				groundNot = true;

				Optimus::Vector3 intersectionPoint = first * u+ second * v+ third * w; // m_player->m_gameObject->getPosition() + i_ray;
				intersectionPoint.y( intersectionPoint.y() + cTPSCamera::getInstance()->m_playerHeight );// - (  ( 1 - t ) * std::sqrt( Optimus::Vector3::magSqr( i_ray ) ) ) );
				m_listOfCollidableObjects[i_index]->setPosition( intersectionPoint );
				UpdateInfo( i_index, pos, lookAt, speed, groundRay,nextForPos, nextBackPos );
			}
		}
	}
		
	triangleList.clear();
	FindTrianglesToCollide( pos, nextForPos, OctreeBuilder::OctreeCreator::getInstance()->m_octree->m_rootNode, triangleList );
	if( triangleList.size() != 0 )
	{
		for( unsigned int i = 0; i < triangleList.size(); i++ )
		{
			Optimus::Vector3 first = triangleList[i].m_first;
			Optimus::Vector3 second = triangleList[i].m_second;
			Optimus::Vector3 third = triangleList[i].m_third;

			if( IntersectSegmentTriangle( pos, nextForPos, first, second, third, u, v, w, t ) == 1 )
			{
				cTPSCamera::getInstance()->m_collisionHappened_Wall = true;

				Optimus::Vector3 n3 = first - third;
				Optimus::Vector3 n4 = second - third;
				Optimus::Vector3 normal = Optimus::Vector3::crossProd( n3, n4 );
				normal = normal.normalise();

				float dotProd = Optimus::Vector3::dotProd( m_listOfCollidableObjects[i_index]->getVelocity() * -1, normal.normalise() );
			
				Optimus::Vector3 temp = normal.normalise() * 2 * dotProd - m_listOfCollidableObjects[i_index]->getVelocity() * -1;
				Optimus::Vector3 tempPos = m_listOfCollidableObjects[i_index]->getPosition();
				tempPos+= temp* Optimus::TimeSystem::getInstance()->getDeltaTime();
				m_listOfCollidableObjects[i_index]->setPosition( tempPos );
				UpdateInfo( i_index, pos, lookAt, speed, groundRay,nextForPos, nextBackPos );
			}
		}
	}

	triangleList.clear();
	FindTrianglesToCollide( pos, nextBackPos, OctreeBuilder::OctreeCreator::getInstance()->m_octree->m_rootNode, triangleList );
	if( triangleList.size() != 0 )
	{
		for( unsigned int i = 0; i < triangleList.size(); i++ )
		{
			Optimus::Vector3 first = triangleList[i].m_first;
			Optimus::Vector3 second = triangleList[i].m_second;
			Optimus::Vector3 third = triangleList[i].m_third;

			if( IntersectSegmentTriangle( pos, nextBackPos, first, second, third, u, v, w, t ) == 1 )
			{
				cTPSCamera::getInstance()->m_collisionHappened_Wall = true;

				Optimus::Vector3 n3 = first - third;
				Optimus::Vector3 n4 = second - third;
				Optimus::Vector3 normal = Optimus::Vector3::crossProd( n3, n4 );
				normal = normal.normalise();

				float dotProd = Optimus::Vector3::dotProd( m_listOfCollidableObjects[i_index]->getVelocity() * -1, normal.normalise() );
			
				Optimus::Vector3 temp = normal.normalise() * 2 * dotProd - m_listOfCollidableObjects[i_index]->getVelocity() * -1;
				Optimus::Vector3 tempPos = m_listOfCollidableObjects[i_index]->getPosition();
				tempPos+= temp* Optimus::TimeSystem::getInstance()->getDeltaTime();
				m_listOfCollidableObjects[i_index]->setPosition( tempPos );
				UpdateInfo( i_index, pos, lookAt, speed, groundRay,nextForPos, nextBackPos );
			}
		}
	}
	triangleList.clear();
	if( groundNot == false )
	{
		cTPSCamera::getInstance()->m_collisionHappened_Ground = false;
	}
}

void Optimus::Collision::CameraCollisionDetection( unsigned int i_index )
{
	float fpsrot = cTPSCamera::getInstance()->m_yawRotation;
	float i_angle = m_listOfCollidableObjects[i_index]->getRotation() - fpsrot;

	cTPSCamera::getInstance()->setLookAtPosition( D3DXVECTOR3( m_listOfCollidableObjects[i_index]->getPosition().x(),
									m_listOfCollidableObjects[i_index]->getPosition().y()+ 30.0f,
									m_listOfCollidableObjects[i_index]->getPosition().z() ) );

	float magnitude = 150.0f;
	Optimus::Vector3 forward = Optimus::Vector3( cTPSCamera::getInstance()->m_forward.x, cTPSCamera::getInstance()->m_forward.y, cTPSCamera::getInstance()->m_forward.z );
	forward = forward * ( -magnitude );
	forward.y( forward.y() + 100.0f );

	std::vector<OctreeBuilder::Triangle> triangleList;

	Optimus::Vector3 ray = m_listOfCollidableObjects[i_index]->getPosition() + forward;
	FindTrianglesToCollide( m_listOfCollidableObjects[i_index]->getPosition(), ray, OctreeBuilder::OctreeCreator::getInstance()->m_octree->m_rootNode, triangleList );
	float u,v,t,w, d = 1.0f;

	if( triangleList.size() != 0 )
	{
		for( unsigned int i = 0; i < triangleList.size(); i++ )
		{
			Optimus::Vector3 first = triangleList[i].m_first;
			Optimus::Vector3 second = triangleList[i].m_second;
			Optimus::Vector3 third = triangleList[i].m_third;

			if( Optimus::Collision::getInstance()->IntersectSegmentTriangle( m_listOfCollidableObjects[i_index]->getPosition(), ray, first, second, third, u, v, w, t ) == 1 )
			{
				if( t < d )
					d = t;
			}
		}
	}

	if( d > 0 && d < 1 )
	{
		cTPSCamera::getInstance()->m_canSee = false;
		cTPSCamera::getInstance()->setMoveTo( D3DXVECTOR3( ( m_listOfCollidableObjects[i_index]->getPosition().x() + forward.x() * d ),
			( m_listOfCollidableObjects[i_index]->getPosition().y() + forward.y() ),
			( m_listOfCollidableObjects[i_index]->getPosition().z() + forward.z() *  d ) ) );
	}
	else
	{
		cTPSCamera::getInstance()->m_canSee = true;
		cTPSCamera::getInstance()->setMoveTo( D3DXVECTOR3( ray.x(), ray.y(), ray.z() ) );
	}
}

void Optimus::Collision::FindTrianglesToCollide( Optimus::Vector3& a, Optimus::Vector3& b, OctreeBuilder::OctreeNode* i_node, std::vector<OctreeBuilder::Triangle>& i_triangleList  )
{
	Optimus::Vector3 minBound = i_node->m_center - i_node->m_halfDimension;
	Optimus::Vector3 maxBound = i_node->m_center + i_node->m_halfDimension;

	if( IntersectSegmentAABB( a, b, i_node->m_center, i_node->m_halfDimension ) )
	{
		if( i_node->m_listOrderedPoints.size() != 0 )
		{
			for( unsigned int j = 0; j < i_node->m_listOrderedPoints.size(); j++ )
			{
				i_triangleList.push_back( i_node->m_listOrderedPoints[j] );
			}
		}

		//cSceneHandler::getInstance()->AddDebugOctreeBox( minBound, maxBound );

		if( !i_node->m_isLeafNode )
		{
			for( int i = 0; i < 8; i++ )
			{		
				FindTrianglesToCollide( a, b, i_node->m_children[i], i_triangleList );
			}
		}
	}
	else
	{
		return;
	}
}

void Optimus::Collision::UpdateInfo( unsigned int i_index, Optimus::Vector3& i_pos, Optimus::Vector3& i_lookAt, Optimus::Vector3& i_speed, Optimus::Vector3& i_groundRay, Optimus::Vector3& i_for, Optimus::Vector3& i_back )
{
	i_pos = m_listOfCollidableObjects[i_index]->getPosition();
	i_lookAt = Optimus::Vector3( cTPSCamera::getInstance()->m_forward.x, cTPSCamera::getInstance()->m_forward.y, cTPSCamera::getInstance()->m_forward.z );
	i_speed = i_lookAt * m_listOfCollidableObjects[i_index]->GetSpeed();
	i_speed += m_listOfCollidableObjects[i_index]->getAcceleration() * Optimus::TimeSystem::getInstance()->getDeltaTime();

	i_groundRay = i_pos;
	i_groundRay.y( i_pos.y() - cTPSCamera::getInstance()->m_playerHeight );
	i_for = i_pos + i_speed * Optimus::TimeSystem::getInstance()->getDeltaTime();	
	i_back = i_pos - i_speed * Optimus::TimeSystem::getInstance()->getDeltaTime();
}