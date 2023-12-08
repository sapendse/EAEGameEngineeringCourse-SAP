#include "FBXImporter.h"
#include <iostream>
#include <fstream>
#include <assert.h>
#include <string>

HRESULT FBXImporter::FBXImporter::InitiateFBX( std::string i_fileName )
{
	m_fileName = i_fileName;
	FbxImporter*	pImporter =	FbxImporter::Create( m_pFBXSDKManager,"" );
	FbxScene*		pFbxScene =	FbxScene::Create( m_pFBXSDKManager,"" );

	std::string fullName = "../Assets/FBXs/";
	fullName += m_fileName;
	fullName += ".fbx";

	std::string _fName = fullName;

	bool bSuccess = pImporter->Initialize( _fName.c_str(), -1, m_pFBXSDKManager->GetIOSettings() ); //Insert file path
	if( !bSuccess )
		return E_FAIL;

	bSuccess = pImporter->Import( pFbxScene );
	if( !bSuccess )
		return E_FAIL;

	FbxAxisSystem directXAxis( FbxAxisSystem::eDirectX );
	directXAxis.ConvertScene( pFbxScene );

	FbxGeometryConverter geometryConverter( m_pFBXSDKManager );
    geometryConverter.Triangulate( pFbxScene, true );

	// Split meshes per material, so that we only have one material per mesh
	geometryConverter.SplitMeshesPerMaterial( pFbxScene, true );

	pImporter->Destroy();

	FbxNode* pFbxRootNode = pFbxScene->GetRootNode();

	//LoadFBX( pFbxRootNode, myFile );
	DisplayContent( pFbxScene );
	//myFile << pOutVertexVector.size() << "\n" << "Vertex Positions: \n";

	//for( unsigned int i = 0; i < pOutVertexVector.size(); i++ )
	//{
	//	//myFile << pOutVertexVector[i].x() << "," << pOutVertexVector[i].y() << "," << pOutVertexVector[i].z() <<"\n";
	//}

	//myFile << "Normals: \n";

	{
		WriteToFile( m_listMeshes );
	}

	return S_OK;
}

// Non-recursive DisplayContent( SCENE )
void FBXImporter::FBXImporter::DisplayContent( FbxScene* i_pScene )
{
    FbxNode* lNode = i_pScene->GetRootNode();
    if(lNode)
    {
        for( int i = 0; i < lNode->GetChildCount(); i++ )
        {
            // Call recursive DisplayContent() once for each child of the root node
            DisplayContent( lNode->GetChild(i) );
	    }
    }
}
 
// Recursively call DisplayContent( NODE ) for each node below the root node
void FBXImporter::FBXImporter::DisplayContent( FbxNode* i_pNode)
{
	//Check Attribute type here
	FbxNodeAttribute::EType AttributeType = i_pNode->GetNodeAttribute()->GetAttributeType();

	if(AttributeType == FbxNodeAttribute::eMesh)
	{
		//Parse node here
		ParseNode( i_pNode );
	}
    for( int i = 0; i < i_pNode->GetChildCount(); i++ )
    {
        DisplayContent( i_pNode->GetChild(i) );
    }
}

void FBXImporter::FBXImporter::ParseNode( FbxNode* i_pNode )
{
	FbxMesh* pMesh = (FbxMesh*) i_pNode->GetNodeAttribute();

	pMesh->SplitPoints( FbxLayerElement::eNormal );

	FbxVector4* pVertices = pMesh->GetControlPoints();

	s_vertex tempVertex;
	s_mesh	 tempMesh;

	tempMesh.m_textureName = NULL;

	s_vertex* vertexList = OPT_NEW s_vertex[pMesh->GetControlPointsCount()];

	char* _textureName = OPT_NEW char[100];
	_textureName[0] = 0;

	for( int i = 0; i< pMesh->GetElementMaterialCount(); i++ )
	{
		FbxGeometryElementMaterial *elementMaterial = pMesh->GetElementMaterial( i );

		int materialsCount = i_pNode->GetSrcObjectCount<FbxSurfaceMaterial>();

		for( int itr = 0; itr < materialsCount; itr++ )
		{
			FbxSurfaceMaterial* materials = i_pNode->GetSrcObject<FbxSurfaceMaterial>(itr);

			for( int c = 0; c < FbxLayerElement::sTypeTextureCount; c++ )
			{
				FbxProperty textureProperty = materials->FindProperty( FbxLayerElement::sTextureChannelNames[c] );
				if( textureProperty.IsValid() )
				{
					int totalTexture = textureProperty.GetSrcObjectCount<FbxTexture>();
					for( int j = 0; j < totalTexture; j++ )
					{
						FbxLayeredTexture *layeredTexture = textureProperty.GetSrcObject<FbxLayeredTexture>( j );
						if( layeredTexture )
						{
							FbxLayeredTexture *layeredTexture = textureProperty.GetSrcObject<FbxLayeredTexture>( j );
							int totalLayeredTexture = layeredTexture->GetSrcObjectCount<FbxTexture>();
							for( int k = 0; k < totalLayeredTexture; k++ )
							{
								FbxTexture *texture = layeredTexture->GetSrcObject<FbxTexture>( k );
								if( texture )
								{
									FbxFileTexture *fileTexture = FbxCast<FbxFileTexture>( texture );
									std::string textureName = fileTexture->GetFileName();
									int start = textureName.find_last_of( "\\/" );
									int last = textureName.size();
									textureName = textureName.substr( start + 1, last - start );

									//strcpy_s(_textureName, std::strlen( _textureName ), textureName.c_str());
									strcpy( _textureName, textureName.c_str() );
								}
							}
						}
						else
						{
							FbxTexture *texture = textureProperty.GetSrcObject<FbxTexture>( j );
							if( texture )
							{
								if( strcmp(textureProperty.GetName(), "DiffuseColor") != 0 )
									continue;
								FbxFileTexture *fileTexture = FbxCast<FbxFileTexture>( texture );
								std::string textureName = fileTexture->GetFileName();
								int start = textureName.find_last_of( "\\/" );
								int last = textureName.size();
								textureName = textureName.substr( start + 1, last - start );

								//strcpy_s(_textureName, std::strlen( _textureName ), textureName.c_str());
								strcpy( _textureName, textureName.c_str() );
							}
						}
					}
				}
			}
		}
	}

	//FbxVector4* pVertices = pMesh->GetControlPoints();
	m_indices = OPT_NEW unsigned int[ pMesh->GetPolygonVertexCount() ];
	m_indices = ( unsigned int* )pMesh->GetPolygonVertices();

	for( int i = 0; i < pMesh->GetControlPointsCount(); i++ )
	{
		tempVertex.x = (float)pVertices[i].mData[0];
		tempVertex.y = (float)pVertices[i].mData[1];
		tempVertex.z = (float)pVertices[i].mData[2];
		vertexList[i] = tempVertex;
	}

	FbxStringList uvSetNames;
	pMesh->GetUVSetNames(uvSetNames);

	//Getting normals and uvs
	for( int iPoly = 0 ; iPoly < pMesh->GetPolygonCount(); iPoly++ )
	{
		for( int iPolyVertex= 0 ; iPolyVertex < 3 ; iPolyVertex++)
		{
			int index = pMesh->GetPolygonVertex(iPoly, iPolyVertex);

			FbxVector4 normal;
			pMesh->GetPolygonVertexNormal(iPoly, iPolyVertex, normal);
			vertexList[index].nx = (float) normal.mData[0];
			vertexList[index].ny = (float) normal.mData[1];
			vertexList[index].nz = (float) normal.mData[2];

			FbxVector2 uv;
			bool pUnmapped; 
			pMesh->GetPolygonVertexUV( iPoly, iPolyVertex, uvSetNames[0], uv, pUnmapped );
			vertexList[index].u = (float) uv.mData[0];
			vertexList[index].v = (float) uv.mData[1];
		}
	}
	tempMesh.m_dynamic = false;
	tempMesh.m_primitiveCount = pMesh->GetControlPointsCount();
	tempMesh.m_indexCount = pMesh->GetPolygonVertexCount();
	tempMesh.m_indices = m_indices;
	tempMesh.m_vertices = vertexList;
	
	FindBounds( tempMesh );

	if(strlen(_textureName) != 0)
	{
		tempMesh.m_textureName = OPT_NEW char[strlen(_textureName) + 1];
		strcpy_s(tempMesh.m_textureName, std::strlen( tempMesh.m_textureName), _textureName);
	}
	else
	{
		tempMesh.m_textureName = NULL;
	}


	m_listMeshes.push_back( tempMesh );
}

void FBXImporter::FBXImporter::FindBounds( s_mesh& i_mesh )
{
	float _minX, _minY, _minZ, _maxX, _maxY, _maxZ, _tempX, _tempY, _tempZ;

	_minX = _minY = _minZ = ( float )INT_MAX;
	_maxX = _maxY = _maxZ = -( float )INT_MAX;

	for( unsigned int i = 0; i < i_mesh.m_primitiveCount; i++ )
	{
		_tempX = i_mesh.m_vertices[i].x;
		_tempY = i_mesh.m_vertices[i].y;
		_tempZ = i_mesh.m_vertices[i].z;

		if( _tempX > _maxX )
		{
			_maxX = _tempX;
		}
		if( _tempY > _maxY )
		{
			_maxY = _tempY;
		}
		if( _tempZ > _maxZ )
		{
			_maxZ = _tempZ;
		}
		if( _tempX < _minX )
		{
			_minX = _tempX;
		}
		if( _tempY < _minY )
		{
			_minY = _tempY;
		}
		if( _tempZ < _minZ )
		{
			_minZ = _tempZ;
		}
	}

	Optimus::Vector3 tempMin = Optimus::Vector3( _minX, _minY, _minZ ), tempMax = Optimus::Vector3( _maxX, _maxY, _maxZ );

	i_mesh.m_min = tempMin;
	i_mesh.m_max = tempMax;
}

void FBXImporter::FBXImporter::UpdateSceneFile( std::string i_meshFileName )
{
	unsigned int accessCount = 1;
	int* _meshCount;
	
	std::ifstream inFile;
	std::string* temp;
	inFile.open( "../Assets/Scenes/MainScene.txt" );
	char c = 'a';
	while( !inFile.eof() )
	{
		inFile >> c;
		if( c == '>' )
		{
			inFile >> accessCount;
			_meshCount = OPT_NEW int[accessCount];
			temp = OPT_NEW std::string[accessCount];
		}
		else if( c == '-' )
		{
			for( unsigned int i = 0; i < accessCount; i++ )
			{
				inFile >> _meshCount[i];
			}
		}
		else if( c == ':' )
		{
			for( unsigned int i = 0; i < accessCount; i++ )
			{
				inFile>> temp[i];
			}
		}
	}
	inFile.close();

	//Check if the new mesh file exists
	bool _hasChanged = true;
	for( unsigned int i = 0; i < accessCount; i++ )
	{
		if( strcmp( temp[i].c_str(), i_meshFileName.c_str() ) == 0 )
		{
			//New File added by the FBX Importer.
			_hasChanged = false;
		}
	}
	
	if( _hasChanged )
	{
		accessCount += 1;

		std::string* _meshFilesToWrite = OPT_NEW std::string[accessCount];
		int* _meshCountArray = OPT_NEW int[accessCount];
		for( unsigned int i = 0; i < accessCount; i++ )
		{
			if( i < accessCount - 1 )
			{
				_meshFilesToWrite[i] = temp[i];
				_meshCountArray[i] = _meshCount[i];
			}
			else
			{
				_meshFilesToWrite[i] = i_meshFileName;
				_meshCountArray[i] = m_listMeshes.size();
			}
		}
	
		//Write to Scene file
		//Camera and Light Info is unchanged
		//Access, mesh counts and Mesh names would change.
		std::string sceneFileName = "../Assets/Scenes/MainScene.txt"; //The main and only scene file.
		std::ofstream sceneFile( sceneFileName.c_str(), std::ofstream::out );
		sceneFile <<"Camera\nPosition\tRotation_\n";
		sceneFile << "0.0 0.0 0.0	0.0\n\n";
		sceneFile << "Light\nPosition\tColour\tIntensity\tAttFac=\n";
		sceneFile << "0.0 0.0 0.0   0.2 0.5 0.8   0.03 	 0.09\n\n";
		sceneFile<<"Access Count > " << accessCount;
		sceneFile<<"\n\nMesh Count-";
		for( unsigned int i = 0; i < (unsigned int)accessCount; i++ )
		{
			sceneFile << "\n" << _meshCountArray[i];
		}
		sceneFile<<"\n\n";
		sceneFile<<"Meshes:";
		for( unsigned int i = 0; i < (unsigned int)accessCount; i++ )
		{
			sceneFile<< "\n" << _meshFilesToWrite[i];
		}
		sceneFile.close();
	}
}

void FBXImporter::FBXImporter::WriteToFile( std::vector<s_mesh> i_listMeshes )
{
	unsigned int textureLength, effectLength = 14, normalLength = 4, specExpo = 50, alphaValue = 1;
	
	std::string text = ".txt";
	std::string binary = ".bin";
	std::string meshFileName = "../Assets/Meshes/";

	meshFileName += m_fileName;
	meshFileName += binary;

	std::string textFile = "../Assets/Meshes/";
	textFile += m_fileName;
	textFile += text;

	UpdateSceneFile( meshFileName );
	//const char* fileName = "../Assets/Scenes/playersceneinfo.txt";
	//const char* newFileName = "../Assets/Meshes/player.bin";
	//const char* txtFileName = "../Assets/Meshes/playerTXT.txt";
	
	std::string defTexName = "NULL";
	char* defEffName = "effectSpecular";
	std::string defNormName = "null";
	
	
	//targetFile.open( fileName.c_str() );
	
	std::ofstream targetFile ( meshFileName, std::ios::binary | std::ios::out );
	std::ofstream txtFile( textFile, std::ios::out );
	//targetFile.open( newFileName.c_str() );

	for( unsigned int i = 0; i < i_listMeshes.size(); ++i )
	{
		i_listMeshes[i].m_ID = i;
		txtFile << "Mesh Number: " << i <<"\n";
		targetFile.write( reinterpret_cast< const char*>(&effectLength), sizeof( unsigned int ) );
		char* effectFile = "effectSpecular";
		targetFile.write( reinterpret_cast< const char*>(effectFile), ( effectLength + 1 ) );
		txtFile<<effectLength <<"\n";		
		txtFile<<effectFile <<"\n";
		if(i_listMeshes[i].m_textureName != NULL)
		{
			textureLength = std::strlen(i_listMeshes[i].m_textureName);
			targetFile.write( reinterpret_cast< const char*>(&textureLength), sizeof( unsigned int ));
			targetFile.write( reinterpret_cast< const char*>(i_listMeshes[i].m_textureName), ( textureLength + 1 ) );
			txtFile << textureLength <<"\n";
			txtFile << m_listMeshes[i].m_textureName <<"\n";
		}
		else
		{
			textureLength = 4;
			targetFile.write( reinterpret_cast< const char*>(&textureLength), sizeof( unsigned int ) );
			char* textureFile = "NULL";
			targetFile.write( reinterpret_cast< const char*>(textureFile), ( textureLength + 1 ) );
			txtFile << textureLength <<"\n";
			txtFile << textureFile <<"\n";
		}
		targetFile.write( reinterpret_cast< const char*>(&normalLength), sizeof( unsigned int ) );
		char* normalFile = "null";
		targetFile.write( reinterpret_cast< const char*>(normalFile), ( normalLength + 1 ) );
		txtFile << normalLength <<"\n";
		txtFile << normalFile <<"\n";
		targetFile.write( reinterpret_cast< const char*>(&specExpo), sizeof( unsigned int ) );
		targetFile.write( reinterpret_cast< const char*>(&alphaValue), sizeof( unsigned int ) );
		targetFile.write( reinterpret_cast< const char*>(&i_listMeshes[i].m_primitiveCount), sizeof( unsigned int ) );
		targetFile.write( reinterpret_cast< const char*>(&i_listMeshes[i].m_indexCount), sizeof( unsigned int ) );
		txtFile << "Spec Expo ="<< specExpo;
		txtFile << "\n";
		txtFile<< "Alpha Value ="<< alphaValue;
		txtFile << "\n";
		txtFile<< "Primitive Count ="<< i_listMeshes[i].m_primitiveCount;
		txtFile << "\n";
		txtFile<< "Vertex Count:" << i_listMeshes[i].m_indexCount;
		txtFile << "\n\n";
		
		for( unsigned int j= 0; j< m_listMeshes[i].m_indexCount; j++ )
		{
			targetFile.write( reinterpret_cast< const char*>(&i_listMeshes[i].m_vertices[j]), sizeof( s_vertex ) );
			txtFile<< "{(" <<(float) i_listMeshes[i].m_vertices[j].x;
			txtFile<<" "<<( float ) i_listMeshes[i].m_vertices[j].y;
			txtFile<<" "<< ( float ) i_listMeshes[i].m_vertices[j].z;
			txtFile<< ") &" ;
			txtFile<<(float) i_listMeshes[i].m_vertices[j].color ;
			txtFile<< " | " ;
			txtFile<<( float )  i_listMeshes[i].m_vertices[j].nx ;
			txtFile<<" "<<( float ) i_listMeshes[i].m_vertices[j].ny; 
			txtFile<<" "<<( float ) i_listMeshes[i].m_vertices[j].nz;
			txtFile<<" "<<( float ) i_listMeshes[i].m_vertices[j].tanx ;
			txtFile<<" "<<( float ) i_listMeshes[i].m_vertices[j].tany; 
			txtFile<<" "<<( float ) i_listMeshes[i].m_vertices[j].tanz;
			txtFile<<" "<<( float ) i_listMeshes[i].m_vertices[j].bix;
			txtFile<<" "<<( float ) i_listMeshes[i].m_vertices[j].biy;
			txtFile<<" "<<( float ) i_listMeshes[i].m_vertices[j].biz;
			txtFile<<" /"<<( float ) i_listMeshes[i].m_vertices[j].u;
			txtFile<<" "<<( float ) i_listMeshes[i].m_vertices[j].v << "}";
			txtFile<< "\n";
		}
		//targetFile<< "\n";
		for( unsigned int j = 0; j < i_listMeshes[i].m_primitiveCount; j+= 3 )
		{
			//targetFile.write( reinterpret_cast< const char*>(&m_listMeshes[i].m_indices[j]), sizeof( unsigned int ) );
			//targetFile.write( (char*)&m_listMeshes[i].m_indices[j+1], sizeof( unsigned int ) );
			//targetFile.write( (char*)&m_listMeshes[i].m_indices[j+2], sizeof( unsigned int ) );
			txtFile<<"["<< i_listMeshes[i].m_indices[j];
			txtFile<<","<< i_listMeshes[i].m_indices[j+1];
			txtFile<<","<< i_listMeshes[i].m_indices[j+2]<<"]";
			txtFile<<"\n";
		}	
		for( unsigned int j = 0; j < i_listMeshes[i].m_primitiveCount; j++ )
		{
			targetFile.write( reinterpret_cast< const char*>(&i_listMeshes[i].m_indices[j]), sizeof( unsigned int ) );
		}
		//targetFile<<"EOMF";
		
		txtFile << "Max :" <<( float ) i_listMeshes[i].m_max.x() << "," << ( float ) i_listMeshes[i].m_max.y() << "," << ( float ) i_listMeshes[i].m_max.z() << "\n";
		txtFile << "Min :" <<( float ) i_listMeshes[i].m_min.x() << "," << ( float ) i_listMeshes[i].m_min.y() << "," << ( float ) i_listMeshes[i].m_min.z() << "\n";
		txtFile << "ID: " << ( int ) i_listMeshes[i].m_ID << "\n";
		txtFile << "Dynamic: "<< ( bool ) i_listMeshes[i].m_dynamic << "\n";

		targetFile.write( reinterpret_cast< const char* >(&i_listMeshes[i].m_max), sizeof( Optimus::Vector3 ) );
		targetFile.write( reinterpret_cast< const char* >(&i_listMeshes[i].m_min), sizeof( Optimus::Vector3 ) );
		targetFile.write( reinterpret_cast< const char* >(&i_listMeshes[i].m_ID ), sizeof( unsigned int ) );
		targetFile.write( reinterpret_cast< const char* >(&i_listMeshes[i].m_dynamic ), sizeof( bool ) );
	}
	targetFile.close();
	txtFile.close();
}
