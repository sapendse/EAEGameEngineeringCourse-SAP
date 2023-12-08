#ifndef _CEFFECTS_H
#define _CEFFECTS_H

#include <string>

namespace Optimus
{
	class cEffects
	{
		std::string m_fileName;
		std::string m_technique;
		std::string m_vertexExpected;
		std::string m_fragmentExpected;
		std::string m_renderState;
		std::string m_textureExpected;

	public:
		//Constructor and Destructor
		cEffects( std::string i_fileName );
		~cEffects();

		//Getters
		std::string getTechnique();
		std::string getVertexShader();
		std::string getFragmentShader();
		std::string getRenderState();
		std::string getExpectedTexture();

		//Read file
		void readThisFile();
	};
}

#endif