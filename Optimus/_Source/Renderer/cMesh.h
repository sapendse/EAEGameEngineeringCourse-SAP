#ifndef _MESH_H
#define _MESH_H

#include <d3d9.h>
#include <d3d9types.h>
#include <d3dx9.h>
#include <d3dx9math.h>

#include <assert.h>
#include <fstream>
#include "cLighting.h"
#include "cTime.h"
#include "..\Math\Vector3.h"
#include "VertexFormat.h"
#include "File Parsers\cEffectsParser.h"

namespace Optimus
{

	class cMesh
	{
		char* m_effectExpected;
		char* m_textureExpected;
		char* m_normaMapTexture;
		D3DXVECTOR3 m_position;
		float m_rotation;
		IDirect3DVertexBuffer9* vertexBuffer;
		IDirect3DIndexBuffer9* indexBuffer;
		IDirect3DVertexShader9* vertexShader;
		ID3DXConstantTable* vertexShaderConstantTable;
		IDirect3DPixelShader9* fragmentShader;
		ID3DXConstantTable* fragmentShaderConstantTable;
		IDirect3DTexture9* m_texture;
		IDirect3DTexture9* m_normalTexture;
		IDirect3DDevice9* m_direct3dDevice;

		cLighting* m_lighting;
		int m_solid, m_ID;
		long m_dataReadTillNow;
		float m_specExpo, m_alphaValue;
		cEffects* m_effect;

	public:
		Vector3 m_maxBound, m_minBound;
		unsigned int m_primitiveCount;
		unsigned int m_indexCount;
		bool		 m_player;
		bool		 m_dynamic, m_hardSurface;

		//Constructor
		cMesh( );

		//Destructor
		~cMesh();

		//Getters
		const float getRotation();
		const D3DXVECTOR3 getPosition();
		const Vector3 getWorldPosition();
		//IDirect3DTexture9* getNormalTexture() { return m_normalTexture; }
		//IDirect3DTexture9* getTexture() { return m_texture; }

		//Setters
		void setPosition( const D3DXVECTOR3& i_position );
		void setTranslation( const D3DXVECTOR3 & i_offset );
		void setRotation( float & i_rotateBy );
		void setWorldPosition( Optimus::Vector3& i_worldPosition );

		//Methods
		bool Initialize( IDirect3DDevice9* i_direct3dDevice );
		void Update();
		void LoadTransforms( D3DXMATRIX i_viewTransform, D3DXMATRIX i_projectedTransform, cLighting *i_light );
		void Draw();
		void ShutDown();
		void loadEffect();
		void loadTexture();
		void setSolid( int i_solid );
		void setDynamic( bool i_dynamic );
		int getSolid();
		int getData(){ return m_dataReadTillNow; }
		void setData( int i_value ){ m_dataReadTillNow = i_value; }
		float getSpecExpo() { return m_specExpo; }
		float getAlpha() { return m_alphaValue; }
		cEffects* getEffect() { return m_effect; }
		std::string getEffects() { return m_effectExpected; }
		std::string getTexture() { return m_textureExpected; }
		std::string getNormalMapTexture() { return m_normaMapTexture; }
		IDirect3DDevice9* getDevice() { return m_direct3dDevice; }
		int	getID() { return m_ID; }
		cLighting* getLighting() { return m_lighting; }
		bool parseFiles( const char* i_meshFileName, unsigned int i );
	};

}

#endif