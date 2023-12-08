#include "FBXImporter.h"
#include "OctreeBuilder.h"
#include <iostream>


FBXImporter::FBXImporter* FBXImporter::FBXImporter::m_instance = NULL;
OctreeBuilder::OctreeCreator* OctreeBuilder::OctreeCreator::m_instance = NULL;
int main()
{
	//FBXImporter::FBXImporter::getInstance()->InitiateFBX("Collision"); 
	OctreeBuilder::OctreeCreator::getInstance()->CreateOctree();
	std::cout<<"Input a number: ";
	int i;
	std::cin>>i;
	return 0;
}