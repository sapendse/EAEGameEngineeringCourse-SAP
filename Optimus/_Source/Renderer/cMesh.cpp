#include "cMesh.h"
#include "UserInput.h"
#include "cTime.h"
#include "..\Utilities\New.h"
#include "cTPSCamera.h"
#include <string>

namespace Optimus{

	cMesh::cMesh( ):
	m_position( 0.0f , 0.0f, 0.0f ),
	m_dataReadTillNow( 0 ),
	m_rotation( 0.0f ),
	vertexBuffer(NULL),
	indexBuffer(NULL),
	vertexShader(NULL),
	vertexShaderConstantTable(NULL),
	fragmentShader(NULL),
	fragmentShaderConstantTable(NULL),
	m_texture(NULL),
	m_normalTexture( NULL ),
	m_direct3dDevice(NULL),
	m_player( false ),
	m_dynamic( false ),
	m_hardSurface( false )
	{
		m_effectExpected = OPT_NEW char[50];
		m_textureExpected = OPT_NEW char[50];
		m_normaMapTexture = OPT_NEW char[50];
	}

	bool cMesh::Initialize( IDirect3DDevice9* i_direct3dDevice )
	{
		m_direct3dDevice = i_direct3dDevice;
		return true;
	}

	const D3DXVECTOR3 cMesh::getPosition()
	{
		return m_position;
	}

	const float cMesh::getRotation()
	{
		return m_rotation;
	}

	void cMesh::setRotation( float & i_rotateBy )
	{
		m_rotation= i_rotateBy;
	}

	void cMesh::setPosition( const  D3DXVECTOR3& i_position )
	{
		D3DXVECTOR3 difference = i_position - m_position;
		m_minBound += Optimus::Vector3( difference.x, difference.y, difference.z );
		m_maxBound += Optimus::Vector3( difference.x, difference.y, difference.z );
		m_position = i_position;
	}

	void cMesh::setWorldPosition( Optimus::Vector3& i_worldPosition )
	{
		Optimus::Vector3 currentWorldPos = ( m_maxBound + m_minBound ) / 2.0f;
		currentWorldPos = i_worldPosition - currentWorldPos;
		m_minBound += Optimus::Vector3( currentWorldPos.x(), currentWorldPos.y(), currentWorldPos.z() );
		m_maxBound += Optimus::Vector3( currentWorldPos.x(), currentWorldPos.y(), currentWorldPos.z() );
		D3DXVECTOR3 pos = D3DXVECTOR3( currentWorldPos.x(), currentWorldPos.y(), currentWorldPos.z() );
		m_position += pos;
	}

	const Vector3 cMesh::getWorldPosition()
	{
		Optimus::Vector3 currentWorldPos = ( m_maxBound + m_minBound ) / 2.0f;
		return currentWorldPos;
	}

	void cMesh::setTranslation( const  D3DXVECTOR3& i_offset )
	{
		m_position += i_offset;
		m_minBound += Optimus::Vector3( i_offset.x, i_offset.y, i_offset.z );
		m_maxBound += Optimus::Vector3( i_offset.x, i_offset.y, i_offset.z );
	}

	void cMesh::setDynamic( bool i_dynamic )
	{
		m_dynamic = i_dynamic;
	}

	void cMesh::LoadTransforms( D3DXMATRIX i_viewTransform, D3DXMATRIX i_projectedTransform, cLighting *i_light )
	{
		m_lighting =i_light;
		{
			{
				D3DXHANDLE variable;
				{
					D3DXHANDLE topLevelVariable = NULL;
					const char* variableName = "g_modelToWorldTransform";
					variable = vertexShaderConstantTable->GetConstantByName( topLevelVariable, variableName );
				}
				if ( variable != NULL )
				{
					D3DXMATRIX modelToWorldTransform;
					
					D3DXMatrixRotationY( &modelToWorldTransform, getRotation() );

					D3DXMATRIX translation;
					D3DXMatrixTranslation( &translation, getPosition().x, getPosition().y, getPosition().z );
					modelToWorldTransform = modelToWorldTransform * translation;

					HRESULT result = vertexShaderConstantTable->SetMatrix( m_direct3dDevice, variable, &modelToWorldTransform );
					assert( SUCCEEDED( result ) );
				}
				else
				{
					// If the variable isn't used by the shader it won't be included in the compiled version;
					// this can make it hard to know whether to know a NULL handle is an error or not.
					// It is up to you how to handle this
				}
			}

			{
				D3DXHANDLE variable;
				{
					D3DXHANDLE topLevelVariable = NULL;
					const char* variableName = "g_worldToViewTransform";
					variable = vertexShaderConstantTable->GetConstantByName( topLevelVariable, variableName );
				}
				if ( variable != NULL )
				{
					D3DXMATRIX worldToViewTransform;
					
					worldToViewTransform = cTPSCamera::getInstance()->GetWorldToViewTransform();

					m_direct3dDevice->SetTransform( D3DTS_VIEW, &worldToViewTransform );

					HRESULT result = vertexShaderConstantTable->SetMatrix( m_direct3dDevice, variable, & worldToViewTransform );
					assert( SUCCEEDED( result ) );
				}
				else
				{
					// If the variable isn't used by the shader it won't be included in the compiled version;
					// this can make it hard to know whether to know a NULL handle is an error or not.
					// It is up to you how to handle this
				}
			}

			{
				D3DXHANDLE variable;
				{
					D3DXHANDLE topLevelVariable = NULL;
					const char* variableName = "g_viewToProjectionTransform";
					variable = vertexShaderConstantTable->GetConstantByName( topLevelVariable, variableName );
				}
				if ( variable != NULL )
				{
					//D3DXMATRIX viewToProjectionTransform;
					//
					//D3DXMatrixPerspectiveFovLH( & viewToProjectionTransform, D3DX_PI / 4, 1.333f, 0.1f, 10000.0f );

					HRESULT result = vertexShaderConstantTable->SetMatrix( m_direct3dDevice, variable, & i_projectedTransform );
					assert( SUCCEEDED( result ) );
				}
				else
				{
					// If the variable isn't used by the shader it won't be included in the compiled version;
					// this can make it hard to know whether to know a NULL handle is an error or not.
					// It is up to you how to handle this
				}
			}
			
			{
				D3DXHANDLE variable;
				{
					{
						D3DXHANDLE topLevelVariable = NULL;
						const char* variableName = "g_pointLightPosition";
						variable = fragmentShaderConstantTable->GetConstantByName( topLevelVariable, variableName );
					}
					if( variable != NULL )
					{
						float float_array[] = { m_lighting->getPosition().x, m_lighting->getPosition().y, m_lighting->getPosition().z };
						HRESULT result = fragmentShaderConstantTable->SetFloatArray( m_direct3dDevice, variable, float_array, 3);
						assert( SUCCEEDED( result ) );
					}
					else
					{
					}
				}
			}

			{
				D3DXHANDLE variable;
				{
					D3DXHANDLE topLevelVariable = NULL;
					const char* variableName = "g_parameters_PointLight";
					variable = fragmentShaderConstantTable->GetConstantByName( topLevelVariable, variableName );
				}
				if ( variable != NULL )
				{
					HRESULT result = fragmentShaderConstantTable->SetVector( m_direct3dDevice, variable, &m_lighting->getParams());
					assert( SUCCEEDED( result ) );
				}
				else
				{
				}
			}

			//Setting att factor
			{
				D3DXHANDLE variable;
				{  
					D3DXHANDLE topLevelVariable = NULL;
					const char* variableName = "g_attenuationFactor";
					variable = fragmentShaderConstantTable->GetConstantByName( topLevelVariable, variableName );
				}
				if ( variable != NULL )
				{
					HRESULT result = fragmentShaderConstantTable->SetFloat( m_direct3dDevice, variable, i_light->getAttFac());
					assert( SUCCEEDED( result ) );
				}
			}

			//Setting the specularExponent
			{
				D3DXHANDLE variable;
				{  
					D3DXHANDLE topLevelVariable = NULL;
					const char* variableName = "specExpo";
					variable = fragmentShaderConstantTable->GetConstantByName( topLevelVariable, variableName );
				}
				if ( variable != NULL )
				{
					HRESULT result = fragmentShaderConstantTable->SetFloat( m_direct3dDevice, variable, this->getSpecExpo());
					assert( SUCCEEDED( result ) );
				}
			}

			//Setting Alpha
			{
				D3DXHANDLE variable;
				{  
					D3DXHANDLE topLevelVariable = NULL;
					const char* variableName = "g_alphaValue";
					variable = fragmentShaderConstantTable->GetConstantByName( topLevelVariable, variableName );
				}
				if ( variable != NULL )
				{
					HRESULT result = fragmentShaderConstantTable->SetFloat( m_direct3dDevice, variable, this->getAlpha());
					assert( SUCCEEDED( result ) );
				}
			}

			//Setting the worldToView transform in the fragment shader for specular lighting
			{
		
				D3DXHANDLE variable;
				{
					D3DXHANDLE topLevelVariable = NULL;
					const char* variableName = "g_worldToViewTransform";
					variable = fragmentShaderConstantTable->GetConstantByName( topLevelVariable, variableName );
				}
				if ( variable != NULL )
				{
					HRESULT result = fragmentShaderConstantTable->SetMatrix( m_direct3dDevice, variable, &i_viewTransform );
					assert( SUCCEEDED( result ) );
				}
			}

			{
				// Set the elapsed time and transforms and textures into the shaders' uniform variables
				// (we will discuss this more in later classes)
			
				float secondsElapsed = g_time.GetTotalSecondsElapsed();
				{
					D3DXHANDLE variable;
					{
						D3DXHANDLE topLevelVariable = NULL;
						const char* variableName = "g_secondsElapsed";
						variable = vertexShaderConstantTable->GetConstantByName( topLevelVariable, variableName );
					}
					if ( variable != NULL )
					{
						HRESULT result = vertexShaderConstantTable->SetFloat( m_direct3dDevice, variable, secondsElapsed );
						assert( SUCCEEDED( result ) );
					}
					else
					{
						// If the variable isn't used by the shader it won't be included in the compiled version;
						// this can make it hard to know whether to know a NULL handle is an error or not.
						// It is up to you how to handle this
					}
				}
			
				{
					D3DXHANDLE variable;
					{
						D3DXHANDLE topLevelVariable = NULL;
						const char* variableName = "g_secondsElapsed";
						variable = fragmentShaderConstantTable->GetConstantByName( topLevelVariable, variableName );
					}
					if ( variable != NULL )
					{
						HRESULT result = fragmentShaderConstantTable->SetFloat( m_direct3dDevice, variable, secondsElapsed );
						assert( SUCCEEDED( result ) );
					}
					else
					{
						// If the variable isn't used by the shader it won't be included in the compiled version;
						// this can make it hard to know whether to know a NULL handle is an error or not.
						// It is up to you how to handle this
					}
				}

				if( this->m_player )
				{
					D3DXHANDLE variable;
					{
						D3DXHANDLE topLevelVariable = NULL;
						const char* variableName = "g_transform_modelToWorld";
						variable = vertexShaderConstantTable->GetConstantByName( topLevelVariable, variableName );
					}
					if ( variable != NULL )
					{
						D3DXMATRIX transform_modelToWorld;
						D3DXMatrixIdentity( &transform_modelToWorld );

						HRESULT result = vertexShaderConstantTable->SetMatrix(m_direct3dDevice, variable, &transform_modelToWorld);
						assert( SUCCEEDED( result ) );
					}
					else
					{
						//LogMessage( "Failed to find g_transform_modelToWorld" );
					}
				}
			}
		}
	}

	void cMesh::Update()
	{
		/*D3DXVECTOR3 offset(0.0f, 0.0f, 0.0f);
        if( UserInput::IsKeyPressed( 'S' ))
        {
                offset.y -= 1.0f;
        }
        if( UserInput::IsKeyPressed( 'D' ))
        {
                offset.x += 1.0f;
        }
        if( UserInput::IsKeyPressed( 'W' ))
        {
                offset.y += 1.0f;
        }
        if( UserInput::IsKeyPressed( 'A' ))
        {
                offset.x -= 1.0f;
        }

        float unitsPerSecond = 8.0f;

		//offset *= (unitsPerSecond * g_time.GetSecondsElapsedThisFrame() );
		//m_rotation += 1.5f * g_time.GetSecondsElapsedThisFrame();

        setTranslation(offset);*/
		
		if( this->getEffect()->getRenderState() == "PARTIAL" )
		{
			m_direct3dDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
			m_direct3dDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA); 
			m_direct3dDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA); 
			m_direct3dDevice->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD); 
		}
		else if( this->getEffect()->getRenderState() == "ADD" )
		{
			m_direct3dDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
			m_direct3dDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_ONE); 
			m_direct3dDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_ONE); 
			m_direct3dDevice->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD); 
		}
		else
		{
			m_direct3dDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, FALSE);
		}
	}

	void cMesh::loadEffect()
	{
	// Set the shader programs	
		HRESULT result = m_direct3dDevice->SetVertexShader( vertexShader );
		assert( SUCCEEDED( result ) );
		result = m_direct3dDevice->SetPixelShader( fragmentShader );
		assert( SUCCEEDED( result ) );
	}

	void cMesh::loadTexture()
	{
		{
			unsigned int index = -1;
			{
				D3DXHANDLE variable;
				{
					D3DXHANDLE topLevelVariable = NULL;
					const char* variableName = "g_diffuseSampler";
					variable = fragmentShaderConstantTable->GetConstantByName( topLevelVariable, variableName );
				}
				if ( variable != NULL )
				{
					index = fragmentShaderConstantTable->GetSamplerIndex( variable );
					
				}
				if( index != (unsigned int) -1 )
				{
					HRESULT result = m_direct3dDevice->SetTexture(static_cast<DWORD>(index), m_texture);	
					if(result == NULL)
					{
						//LogMessage("Failed to set texture");
					}
				}
			}
		}
		
		{
			unsigned int index = -1;
			{
				D3DXHANDLE variable;
				{
					D3DXHANDLE topLevelVariable = NULL;
					const char* variableName = "g_normalMapping";
					variable = fragmentShaderConstantTable->GetConstantByName( topLevelVariable, variableName );
				}
				if ( variable != NULL )
				{
					index = fragmentShaderConstantTable->GetSamplerIndex( variable );
					
				}
				if( index != (unsigned int)-1 )
				{
					HRESULT result = m_direct3dDevice->SetTexture(static_cast<DWORD>(index), m_normalTexture);	
					if(result == NULL)
					{
						//LogMessage("Failed to set normal map texture");
					}
				}
			}
		}
	}

	void cMesh::Draw()
	{
		// Bind a specific vertex buffer to the device as a data source
		{
			// There can be multiple streams of data feeding the display adaptor at the same time
			unsigned int streamIndex = 0;
			// It's possible to start streaming data in the middle of a vertex buffer
			unsigned int bufferOffset = 0;
			// The "stride" defines how large a single vertex is in the stream of data
			unsigned int bufferStride = sizeof( s_vertex );
			HRESULT result = m_direct3dDevice->SetStreamSource( streamIndex, vertexBuffer, bufferOffset, bufferStride );
			assert( SUCCEEDED( result ) );
			result = m_direct3dDevice->SetIndices( indexBuffer );
			assert( SUCCEEDED( result ) );
		}
		////Setting indices
		//{
		//	m_direct3dDevice->SetIndices(indexBuffer);
		//}

		// Render objects from the current streams
		{
			// We are using triangles as the "primitive" type,
			// and we have defined the vertex buffer as a triangle list
			// (meaning that every triangle is defined by three vertices)
			D3DPRIMITIVETYPE primitiveType;
			if( getSolid() == 1 )
				primitiveType = D3DPT_TRIANGLELIST;
			else
				primitiveType = D3DPT_LINELIST;
			// It's possible to start rendering primitives in the middle of the stream
			unsigned int indexOfFirstVertexToRender = 0;
			// We are currently only rendering a single triangle
			unsigned int primitiveCountToRender = m_primitiveCount;
			HRESULT result = m_direct3dDevice->DrawIndexedPrimitive( primitiveType, indexOfFirstVertexToRender, 0, m_indexCount, 0, primitiveCountToRender );
			assert( SUCCEEDED( result ) );
		}
	}

	void cMesh::setSolid( int i_solid )
	{
		m_solid = i_solid;
	}

	int cMesh::getSolid()
	{
		return m_solid;
	}

	cMesh::~cMesh()
	{
		ShutDown();
	}

	void cMesh::ShutDown()
	{
		if( vertexBuffer )
		{
			vertexBuffer->Release();
			vertexBuffer = NULL;
		}
		if( fragmentShaderConstantTable )
		{
			fragmentShaderConstantTable->Release();
			fragmentShaderConstantTable = NULL;
		}
		if( fragmentShader )
		{
			fragmentShader->Release();
			fragmentShader = NULL;
		}
		if( vertexShader )
		{
			vertexShader->Release();
			vertexShader = NULL;
		}
		if( vertexShaderConstantTable )
		{
			vertexShaderConstantTable->Release();
			vertexShaderConstantTable = NULL;
		}
		if( indexBuffer )
		{
			indexBuffer->Release();
			indexBuffer = NULL;
		}
		if( m_effect )
		{
			delete m_effect;
			m_effect = NULL;
		}
		if( m_effectExpected )
		{
			delete m_effectExpected;
			m_effectExpected = NULL;
		}
		if( m_textureExpected )
		{
			delete m_textureExpected;
			m_textureExpected = NULL;
		}
		if( m_normaMapTexture )
		{
			delete m_normaMapTexture;
			m_normaMapTexture = NULL;
		}
	}

	bool cMesh::parseFiles( const char* i_meshfileName, unsigned int i )
	{
		std::string meshFileToBeUsed, eomf;
		std::string meshfileName = i_meshfileName;
		
		unsigned int effectFileLength, textureFileLength, normalMapFileLength, countOfEOMF = 0;
		
		if( strcmp( i_meshfileName, "../Assets/Meshes/player.bin" ) == 0 )
		{
			//Is Player.
			m_player = true;
		}

		std::ifstream meshfile( i_meshfileName, std::ifstream::in | std::ifstream::binary );

		if( i != 0 )
		{
			char* temp = OPT_NEW char[m_dataReadTillNow];
			meshfile.read( reinterpret_cast<char*>(temp), m_dataReadTillNow );
			delete temp;
		}

		meshfile.read(  reinterpret_cast<char*>(&effectFileLength), sizeof( unsigned int ) );
		//m_effectExpected = OPT_NEW char[effectFileLength];
		meshfile.read(  reinterpret_cast<char*>(m_effectExpected), ( effectFileLength + 1 ) );
		meshfile.read(  reinterpret_cast<char*>(&textureFileLength), sizeof( unsigned int ) );
		//m_textureExpected = OPT_NEW char[textureFileLength];
		meshfile.read(  reinterpret_cast<char*>(m_textureExpected), ( textureFileLength + 1 ) );
		meshfile.read(  reinterpret_cast<char*>(&normalMapFileLength), sizeof( unsigned int ) );
		//m_normaMapTexture = OPT_NEW char[normalMapFileLength];
		meshfile.read(  reinterpret_cast<char*>(m_normaMapTexture), ( normalMapFileLength + 1 ) );
		meshfile.read(  reinterpret_cast<char*>(&m_specExpo), sizeof( unsigned int ) );
		meshfile.read(  reinterpret_cast<char*>(&m_alphaValue), sizeof( unsigned int ) );
		meshfile.read(  reinterpret_cast<char*>(&m_primitiveCount), sizeof( unsigned int ) );
		meshfile.read(  reinterpret_cast<char*>(&m_indexCount), sizeof( unsigned int ) );

		m_specExpo = 50;
		m_alphaValue = 1;
		/* Read all the files here */

		// Create a vertex buffer
		{
			// A triangle has three vertices
			unsigned int bufferSize = m_primitiveCount * sizeof( s_vertex );
			// The usage tells Direct3D how this vertex buffer will be used
			DWORD usage = 0;
			{
				// Our code will only ever write to the buffer
				usage |= D3DUSAGE_WRITEONLY;
				// The type of vertex processing should match what was specified when the device interface was created
				{
					D3DDEVICE_CREATION_PARAMETERS deviceCreationParameters;
					HRESULT result = m_direct3dDevice->GetCreationParameters( &deviceCreationParameters );
					if ( SUCCEEDED( result ) )
					{
						DWORD vertexProcessingType = deviceCreationParameters.BehaviorFlags &
							( D3DCREATE_HARDWARE_VERTEXPROCESSING | D3DCREATE_MIXED_VERTEXPROCESSING | D3DCREATE_SOFTWARE_VERTEXPROCESSING );
						usage |= ( vertexProcessingType != D3DCREATE_SOFTWARE_VERTEXPROCESSING ) ?
							0 : D3DUSAGE_SOFTWAREPROCESSING;
					}
					else
					{
						return false;
					}
				}
			}
			// We will define our own vertex format
			DWORD useSeparateVertexDeclaration = 0;
			// Place the vertex buffer into memory that Direct3D thinks is the most appropriate
			D3DPOOL useDefaultPool = D3DPOOL_DEFAULT;
			HANDLE* notUsed = NULL;

			HRESULT result = m_direct3dDevice->CreateVertexBuffer( bufferSize, usage, useSeparateVertexDeclaration, useDefaultPool,
				&vertexBuffer, notUsed );
			if ( FAILED( result ) )
			{
				return false;
			}
	
			{
				unsigned int bufferSize = m_indexCount * sizeof( unsigned int );
				D3DFORMAT format = D3DFMT_INDEX32;
				D3DPOOL useDefaultPool = D3DPOOL_DEFAULT;
				HANDLE* notUsed = NULL;

				HRESULT result = m_direct3dDevice->CreateIndexBuffer( bufferSize, usage, format, useDefaultPool,
						&indexBuffer, notUsed );
				if ( FAILED( result ) )
				{
						return false;
				}
			}
		}

		// Fill the vertex buffer with the triangle's vertices
		{
			// Before the vertex buffer can be changed it must be "locked"
			s_vertex* vertexData;
			{
				unsigned int lockEntireBuffer = 0;
				DWORD useDefaultLockingBehavior = 0;
				HRESULT result = vertexBuffer->Lock( lockEntireBuffer, lockEntireBuffer,
					reinterpret_cast<void**>( &vertexData ), useDefaultLockingBehavior );
				if ( FAILED( result ) )
				{
					return false;
				}
			}
			// Before the index buffer can be changed it must be "locked"
			unsigned int* indexData;
			{
				unsigned int lockEntireBuffer = 0;
				DWORD useDefaultLockingBehavior = 0;
				HRESULT result = indexBuffer->Lock( lockEntireBuffer, lockEntireBuffer,
					reinterpret_cast<void**>( &indexData ), useDefaultLockingBehavior );
				if ( FAILED( result ) )
				{
					return false;
				}
			}

			// Fill the vertex buffer
			{
				//char letter= 'a';
				//float data =1.0f;
				//char line[sizeof( unsigned int )*2];
				//unsigned int i =0, index =0, r=0,g=0,b=0;

				for( unsigned int c=0; c< m_primitiveCount; c++ )
				{
					s_vertex temp;
					meshfile.read( reinterpret_cast<char*>(&temp), sizeof( s_vertex ) );
					vertexData[c] = temp;
				}

				for( unsigned int c =0; c< m_indexCount; c++ )
				{
					unsigned int temp1;
					meshfile.read( reinterpret_cast<char*>(&temp1), sizeof( unsigned int ) );
					indexData[c] = temp1;
				}
				
			}

			meshfile.read( reinterpret_cast<char*>(&m_maxBound), sizeof( Optimus::Vector3 ) );
			meshfile.read( reinterpret_cast<char*>(&m_minBound), sizeof( Optimus::Vector3 ) );
			meshfile.read( reinterpret_cast<char*>(&m_ID), sizeof( unsigned int ) );
			meshfile.read( reinterpret_cast<char*>(&m_dynamic), sizeof( bool ) );

			m_dataReadTillNow += ( ( 8 + m_indexCount ) * sizeof( unsigned int ) + ( m_primitiveCount * sizeof( s_vertex ) ) + effectFileLength +
				normalMapFileLength + textureFileLength + 3 + 2 * sizeof( Optimus::Vector3 ) + sizeof( bool ) );
			// The vertex buffer must be "unlocked" before it can be used
			{
				HRESULT result = vertexBuffer->Unlock();
				if ( FAILED( result ) )
				{
					return false;
				}
			}

			// The index buffer must be "unlocked" before it can be used
			{
				HRESULT result = indexBuffer->Unlock();
				if ( FAILED( result ) )
				{
					return false;
				}
			}
		}
		

		//Load texture from a file
		if( strcmp( m_textureExpected, "NULL" ) != 0 )
		{
			std::string tex = m_textureExpected;
			tex = "../Assets/FBXs/ctf_v12-Embed.fbm/"+ tex;

			HRESULT hr = D3DXCreateTextureFromFile( m_direct3dDevice, tex.c_str(), &m_texture );
		}

		std::string effectNeeded = m_effectExpected;
		std::string s_effectExpected = "../Assets/Effects/" + effectNeeded + ".txt";
		m_effect = OPT_NEW cEffects( s_effectExpected.c_str() );		
		std::string vertexShaderToBeUsed;
		std::string fragmentShaderToBeUsed;
		m_effect->readThisFile();
		vertexShaderToBeUsed = m_effect->getVertexShader();
		fragmentShaderToBeUsed = m_effect->getFragmentShader();
		if( strcmp( m_normaMapTexture, "null" ) != 0 )
		{
			HRESULT hr = D3DXCreateTextureFromFile( m_direct3dDevice, m_normaMapTexture, &m_normalTexture );
		}

		// Create the vertex shader
		{
			// Load the compiled vertex shader from disk
			void* compiledVertexShader;
			{
				// Open the file
				const char* fileName = vertexShaderToBeUsed.c_str();
				HANDLE fileHandle;
				{
					DWORD desiredAccess = GENERIC_READ;
					DWORD otherProgramsCanStillReadTheFile = FILE_SHARE_READ;
					SECURITY_ATTRIBUTES* useDefaultSecurity = NULL;
					DWORD onlySucceedIfFileExists = OPEN_EXISTING;
					DWORD useDefaultAttributes = FILE_ATTRIBUTE_NORMAL;
					HANDLE dontUseTemplateFile = NULL;
					fileHandle = CreateFile( fileName, desiredAccess, otherProgramsCanStillReadTheFile,
						useDefaultSecurity, onlySucceedIfFileExists, useDefaultAttributes, dontUseTemplateFile );
					if ( fileHandle == INVALID_HANDLE_VALUE )
					{
						return false;
					}
				}
				// Get the file's size
				DWORD fileSize;
				{
					LARGE_INTEGER fileSize_integer;
					if ( GetFileSizeEx( fileHandle, &fileSize_integer ) == TRUE )
					{
						// This is unsafe, because the file's size could be bigger than a DWORD
						fileSize = static_cast<DWORD>( fileSize_integer.QuadPart );
					}
					else
					{
						CloseHandle( fileHandle );
						return false;
					}
				}
				// Read the file's contents into temporary memory
				compiledVertexShader = malloc( fileSize );
				{
					DWORD bytesReadCount;
					OVERLAPPED* readSynchronously = NULL;
					if ( ReadFile( fileHandle, compiledVertexShader, fileSize,
						&bytesReadCount, readSynchronously ) == TRUE )
					{
						CloseHandle( fileHandle );
					}
					else
					{
						free( compiledVertexShader );
						CloseHandle( fileHandle );
						return false;
					}
				}
			}
			// Create the vertex shader object
			{
				HRESULT result = m_direct3dDevice->CreateVertexShader( reinterpret_cast<DWORD*>( compiledVertexShader ), &vertexShader );
				if ( SUCCEEDED( result ) )
				{
					// Get the constant table from the vertex shader
					result = D3DXGetShaderConstantTable( reinterpret_cast<DWORD*>( compiledVertexShader ), &vertexShaderConstantTable );
					if ( SUCCEEDED( result ) )
					{
						// Set the default values
						result = vertexShaderConstantTable->SetDefaults( m_direct3dDevice );
						if ( FAILED( result ) )
						{
						}
					}
					free( compiledVertexShader );
					if ( FAILED( result ) )
					{
						return false;
					}
				}
				else
				{
					free( compiledVertexShader );
					return false;
				}
			}
		}

		// Create the fragment shader
		{
			// Load the compiled fragment shader from disk
			void* compiledFragmentShader;
			{
				// Open the file
				const char* fileName = fragmentShaderToBeUsed.c_str();
				HANDLE fileHandle;
				{
					DWORD desiredAccess = GENERIC_READ;
					DWORD otherProgramsCanStillReadTheFile = FILE_SHARE_READ;
					SECURITY_ATTRIBUTES* useDefaultSecurity = NULL;
					DWORD onlySucceedIfFileExists = OPEN_EXISTING;
					DWORD useDefaultAttributes = FILE_ATTRIBUTE_NORMAL;
					HANDLE dontUseTemplateFile = NULL;
					fileHandle = CreateFile( fileName, desiredAccess, otherProgramsCanStillReadTheFile,
						useDefaultSecurity, onlySucceedIfFileExists, useDefaultAttributes, dontUseTemplateFile );
					if ( fileHandle == INVALID_HANDLE_VALUE )
					{
						return false;
					}
				}
				// Get the file's size
				DWORD fileSize;
				{
					LARGE_INTEGER fileSize_integer;
					if ( GetFileSizeEx( fileHandle, &fileSize_integer ) == TRUE )
					{
						// This is unsafe, because the file's size could be bigger than a DWORD
						fileSize = static_cast<DWORD>( fileSize_integer.QuadPart );
					}
					else
					{
						CloseHandle( fileHandle );
						return false;
					}
				}
				// Read the file's contents into temporary memory
				compiledFragmentShader = malloc( fileSize );
				{
					DWORD bytesReadCount;
					OVERLAPPED* readSynchronously = NULL;
					if ( ReadFile( fileHandle, compiledFragmentShader, fileSize,
						&bytesReadCount, readSynchronously ) == TRUE )
					{
						CloseHandle( fileHandle );
					}
					else
					{
						free( compiledFragmentShader );
						CloseHandle( fileHandle );
						return false;
					}
				}
			}
			// Create the fragment shader object
			{
				HRESULT result = m_direct3dDevice->CreatePixelShader( reinterpret_cast<DWORD*>( compiledFragmentShader ), &fragmentShader );
				if ( SUCCEEDED( result ) )
				{
					// Get the constant table from the fragment shader
					result = D3DXGetShaderConstantTable( reinterpret_cast<DWORD*>( compiledFragmentShader ), &fragmentShaderConstantTable );
					if ( SUCCEEDED( result ) )
					{
						// Set the default values
						result = fragmentShaderConstantTable->SetDefaults( m_direct3dDevice );
						if ( FAILED( result ) )
						{
						}
					}
					free( compiledFragmentShader );
					if ( FAILED( result ) )
					{
						return false;
					}
				}
				else
				{
					free( compiledFragmentShader );
					return false;
				}
			}
		}
		return true;
	}
}