#include "cEffectsParser.h"
#include <fstream>

Optimus::cEffects::cEffects( std::string i_fileName )
{
	m_fileName = i_fileName;
}

Optimus::cEffects::~cEffects()
{
	//delete m_technique, m_vertexExpected,m_fragmentExpected, m_renderState, m_textureExpected;
}

std::string Optimus::cEffects::getExpectedTexture()
{
	return m_textureExpected;
}

std::string Optimus::cEffects::getFragmentShader()
{
	return m_fragmentExpected;
}

std::string Optimus::cEffects::getRenderState()
{
	return m_renderState;
}

std::string Optimus::cEffects::getTechnique()
{
	return m_technique;
}

std::string Optimus::cEffects::getVertexShader()
{
	return m_vertexExpected;
}

void Optimus::cEffects::readThisFile()
{
	char c= 'a';
	std::ifstream effectfile ( m_fileName );
	if( effectfile.is_open() )
	{
		while( !effectfile.eof() )
		{
			effectfile >> c;
			if( c == ':' )
			{
				effectfile>>m_technique>>m_vertexExpected>>m_fragmentExpected>>m_renderState;
			}
			else if( c== '-' )
			{
				effectfile>>m_textureExpected;
			}
		}
	}
}