 #include "main.h"
#ifdef __WXMSW__
	#include <wx/msw/private.h>
#endif 
#include "..\Optimus\_Source\Utilities\New.h"
#include <algorithm>

IMPLEMENT_APP(MyApp)

bool MyApp::OnInit()
{
    MainFrame *frame = OPT_NEW MainFrame( _("Hello World"), wxPoint(50, 50),
                                  wxSize(1600, 900));

    frame->Connect( ID_Quit, wxEVT_COMMAND_MENU_SELECTED, (wxObjectEventFunction) &MainFrame::OnQuit );
    frame->Connect( ID_Load, wxEVT_COMMAND_MENU_SELECTED, (wxObjectEventFunction) &MainFrame::OnLoad );
	frame->Connect( ID_Write, wxEVT_COMMAND_MENU_SELECTED,(wxObjectEventFunction) &MainFrame::WriteToFile );
	frame->Connect( ID_Update,wxEVT_IDLE, wxIdleEventHandler(MainFrame::OnIdle) );
	frame->Connect( ID_Click, wxEVT_LEFT_DOWN, wxMouseEventHandler(MainFrame::OnMouseClick) );
	frame->Connect( ID_Create, wxEVT_COMMAND_MENU_SELECTED, (wxObjectEventFunction)(&MainFrame::CreatePanel) );
	frame->Connect( ID_Mark, wxEVT_COMMAND_MENU_SELECTED, (wxObjectEventFunction)&MainFrame::MarkDynamic );
	frame->Connect( ID_WayPointNodeCreate, wxEVT_RIGHT_DOWN, wxMouseEventHandler( MainFrame::CreateWayPointNode ) );
	frame->Connect( ID_DeleteWayPoint, wxEVT_COMMAND_MENU_SELECTED, (wxObjectEventFunction)&MainFrame::DeleteWayPoint );

    frame->Show(true);
    SetTopWindow(frame);
    return true;
}

MainFrame::MainFrame(const wxString& title, const wxPoint& pos, const wxSize& size)
    : wxFrame( NULL, -1, title, pos, size )
{
	panel = NULL;
	xDone = yDone = zDone = widthDone = heightDone = lengthDone = worldInitiated = m_connectMode = m_deleteMode = m_hardSurfaceTagMode = false;
	m_currentMeshID = -1;
	tempFileName = "tempFileDynamic.txt";
	_wayPoint1 = NULL;
	_wayPoint2 = NULL;
    wxMenuBar *menuBar = OPT_NEW wxMenuBar;

    wxMenu *menuFile = OPT_NEW wxMenu;

    menuFile->Append( ID_Load, _("&Load Scene") );
    menuFile->AppendSeparator();
    menuFile->Append( ID_Write, _("&Write Scene") );
	menuFile->AppendSeparator();
	menuFile->Append( ID_Create, _("&Create Trigger") );
    menuFile->AppendSeparator();
	menuFile->Append( ID_Mark, _("&Mark Dynamic") );
	menuFile->AppendSeparator();
	menuFile->Append( ID_DeleteWayPoint, _("&Delete WayPoint") );
	menuFile->AppendSeparator();
	menuFile->Append( ID_Quit, _("&Quit") );

    menuBar->Append(menuFile, _("&About Scene") );

    SetMenuBar(menuBar);

    CreateStatusBar();
        
    SetStatusText( _("Welcome to wxWidgets!") );

	//HINSTANCE hInstance = wxGetInstance();
}

void MainFrame::OnLoad(wxCommandEvent& WXUNUSED(event))
{
    /*wxMessageBox( _("wxWidgets Hello World example."), 
                  _("About Hello World"),
                  wxOK|wxICON_INFORMATION, this );*/
	HWND hWND = wxWindow::GetHWND();
	
	if( !worldInitiated )
	{
		worldInitiated = Optimus::World::getInstance()->Initialize( "MyGame", wxWindow::GetSize().GetWidth(), wxWindow::GetSize().GetHeight(), hWND, SW_SHOW, "../Assets/Scenes/MainScene.txt" );	
	}
}

void MainFrame::DeleteWayPoint( wxCommandEvent& event )
{
	if( worldInitiated )
	{
		if( m_tempWaypoints.size() == 0 && Optimus::WayPointGraph::GetInstance()->m_listNodes.size() == 0 )
		{
			SetStatusText("Nothing to delete");
		}
		int _size = m_tempWaypoints.size() - 1;
		if( m_tempWaypoints.size() != 0 )
		{
			if( m_tempWaypoints[_size] )
			{
				delete m_tempWaypoints[_size];
				m_tempWaypoints[_size] = NULL;
				m_tempWaypoints.pop_back();
				SetStatusText("Deleted from Temp");
			}
		}
		if( m_tempWaypoints.size() == 0 )
		{
			if( Optimus::WayPointGraph::GetInstance()->m_listNodes.size() != 0 )
			{
				_size = Optimus::WayPointGraph::GetInstance()->m_listNodes.size() - 1;
				if( m_tempWaypoints[_size] )
				{
					delete Optimus::WayPointGraph::GetInstance()->m_listNodes[_size];
					Optimus::WayPointGraph::GetInstance()->m_listNodes[_size] = NULL;
					Optimus::WayPointGraph::GetInstance()->m_listNodes.pop_back();
					SetStatusText("Deleted from Existing");
				}
			}
		}
	}
}

void MainFrame::OnMouseClick( wxMouseEvent& event )
{
	if( worldInitiated )
	{
		if( m_currentMeshID >= 0 )
		{
			Optimus::cSceneHandler::getInstance()->m_opaqueEnts[m_currentMeshID]->setTranslation( D3DXVECTOR3( 0, -10, 0 ) );
		}
		float x = event.GetX();
		float y = event.GetY();
		float windowX = wxWindow::GetSize().GetWidth();
		float windowY = wxWindow::GetSize().GetHeight();
		x = 2 * (x / windowX) - 1;
		y = -2 * (y / windowY) + 1;
		float z = 1.0f;
		D3DXVECTOR3 cameraPos = Optimus::cTPSCamera::getInstance()->m_position;
		float angle = Optimus::cTPSCamera::getInstance()->m_yawRotation;
		D3DXVECTOR3 nearPlaneIntersection = D3DXVECTOR3( -sin(angle) * 0.1f + cos(angle) * x * 0.2f/2 + cameraPos.x, 
													 y * 0.1125f/2 + cameraPos.y, 
													 cos(angle) * 0.1f + sin(angle) * x * 0.2f/2 + cameraPos.z);
		D3DXVECTOR3 ray_clip = nearPlaneIntersection - cameraPos;


		Optimus::Vector3 rayToWorld = Optimus::Vector3( ray_clip.x, ray_clip.y, ray_clip.z );
		Optimus::Vector3 start = Optimus::Vector3( Optimus::cTPSCamera::getInstance()->m_position.x, 
			Optimus::cTPSCamera::getInstance()->m_position.y, 
			Optimus::cTPSCamera::getInstance()->m_position.z );
		Optimus::Vector3 end = start + rayToWorld * 20000;
		m_start = start;
		m_end = end;
		float minDistSqr = FLT_MAX;
		m_currentMeshID = -1;
		std::vector<Optimus::cMesh*> meshes = Optimus::cSceneHandler::getInstance()->m_opaqueEnts;
		for( unsigned int i = 0; i < meshes.size(); i++ )
		{
			Optimus::Vector3 center = ( meshes[i]->m_minBound + meshes[i]->m_maxBound ) / 2.0f;
			Optimus::Vector3 halfDim = ( meshes[i]->m_maxBound - meshes[i]->m_minBound ) / 2.0f;
			if( Optimus::Collision::getInstance()->IntersectSegmentAABB( start, end, center, halfDim ) )
			{
				Optimus::Vector3 meshPos = ( meshes[i]->m_minBound + meshes[i]->m_maxBound ) / 2.0f;
				float distSqr = ( ( start.x() - meshPos.x() ) * ( start.x() - meshPos.x() )  +
					( start.y() - meshPos.y() ) * ( start.y() - meshPos.y() )  +
					( start.z() - meshPos.z() ) * ( start.z() - meshPos.z() ) );
				if( distSqr < minDistSqr )
				{
					minDistSqr = distSqr;
					m_currentMeshID = i;
				}
			}
		}
		if( m_currentMeshID >= 0 )
		{
			meshes[m_currentMeshID]->setTranslation( D3DXVECTOR3( 0, 10, 0 ) );
			if( m_hardSurfaceTagMode )
			{
				m_hardSurfaceIDs.push_back( m_currentMeshID );
			}
		}
	}
}

void MainFrame::OnIdle( wxIdleEvent& WXUNUSED(event) )
{
	if( worldInitiated )
	{
		Optimus::cSceneHandler::getInstance()->AddDebugLine( m_start, m_end, D3DCOLOR_XRGB( 255,0,255 ) );
		if( m_currentMeshID >= 0 )
		{
			Optimus::cSceneHandler::getInstance()->AddDebugOctreeBox( Optimus::cSceneHandler::getInstance()->m_opaqueEnts[m_currentMeshID]->m_minBound, 
				Optimus::cSceneHandler::getInstance()->m_opaqueEnts[m_currentMeshID]->m_maxBound );
		}
		Optimus::World::getInstance()->Update( Optimus::TimeSystem::getInstance()->getDeltaTime() );
		if( Optimus::UserInput::IsKeyPressed(VK_BACK ) )
		{
			m_connectMode = !m_connectMode;
			if( m_connectMode )
			{
				wxString text = "Connect Mode Is True";
				SetStatusText( text );
			}
			else
			{
				wxString text = "Connect Mode Is False";
				SetStatusText( text );
			}
		}
		if( Optimus::UserInput::IsKeyPressed( VK_DELETE ) )
		{
			m_hardSurfaceTagMode = !m_hardSurfaceTagMode;
			if( m_hardSurfaceTagMode )
			{
				wxString text = "Hard Surdace Tag Mode Is True";
				SetStatusText( text );
			}
			else
			{
				wxString text = "Hard Surdace Tag Mode Is False";
				SetStatusText( text );
			}
		}
	}
}

void MainFrame::CreatePanel( wxCommandEvent& event )
{
	if( worldInitiated )
	{
		if( panel == NULL )
		{
			panel = OPT_NEW wxPanel(this, wxID_ANY, wxPoint( 0, 0 ), wxSize( 120, 240 ));
		}
		if( !xDone )
		{
			posX = OPT_NEW wxTextCtrl(this, panel->GetId(), wxEmptyString, wxPoint( 20, 00 ), wxSize( 100, 30 ), 0L, wxDefaultValidator, "X:" );
			posX->SetFocus();
		
			Connect( ID_Enter, wxEVT_TEXT_ENTER, wxObjectEventFunction(&MainFrame::EnterPressed) );
		}
	
		else if( xDone && !yDone )
		{
			posY = OPT_NEW wxTextCtrl(this, panel->GetId(), wxEmptyString, wxPoint( 20, 40 ), wxSize( 100, 30 ), 0L, wxDefaultValidator, "Y:" );
			posY->SetFocus();
			Connect( ID_Enter, wxEVT_TEXT_ENTER, wxObjectEventFunction(&MainFrame::EnterPressed) );
		}
		else if( xDone && yDone && !zDone )
		{
			posZ = OPT_NEW wxTextCtrl(this, panel->GetId(), wxEmptyString, wxPoint( 20, 80 ), wxSize( 100, 30 ), 0L, wxDefaultValidator, "Z:" );
			posZ->SetFocus();
			Connect( ID_Enter, wxEVT_TEXT_ENTER, wxObjectEventFunction(&MainFrame::EnterPressed) );
		}
		else if( xDone && yDone && zDone && !widthDone )
		{
			width = OPT_NEW wxTextCtrl(this, panel->GetId(), wxEmptyString, wxPoint( 20, 120 ), wxSize( 100, 30 ), 0L, wxDefaultValidator, "Width:" );
			width->SetFocus();
			Connect( ID_Enter, wxEVT_TEXT_ENTER, wxObjectEventFunction(&MainFrame::EnterPressed) );
		}
		else if( xDone && yDone && zDone && widthDone && !heightDone )
		{
			height = OPT_NEW wxTextCtrl(this, panel->GetId(), wxEmptyString, wxPoint( 20, 160 ), wxSize( 100, 30 ), 0L, wxDefaultValidator, "Height:" );
			height->SetFocus();
			Connect( ID_Enter, wxEVT_TEXT_ENTER, wxObjectEventFunction(&MainFrame::EnterPressed) );
		}
		else if( xDone && yDone && zDone && widthDone && heightDone && !lengthDone )
		{
			length = OPT_NEW wxTextCtrl(this, panel->GetId(), wxEmptyString, wxPoint( 20, 200 ), wxSize( 100, 30 ), 0L, wxDefaultValidator, "Length:" );
			length->SetFocus();
			Connect( ID_Enter, wxEVT_TEXT_ENTER, wxObjectEventFunction(&MainFrame::EnterPressed) );
		}
		else if( xDone && yDone && zDone && widthDone && heightDone && lengthDone )
		{
			okButton = OPT_NEW wxButton( this, panel->GetId(), "Ok", wxPoint( 00, 240 ), wxSize( 50, 30 ) );
			Connect( wxID_ANY, wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler(MainFrame::CreateTrigger) );
		}
		Center();
	}
}

void MainFrame::CreateTrigger( wxCommandEvent& WXUNUSED(event) )
{
	float fx,fy,fz,fwidth, fheight,flength;
	fx = atof( posX->GetValue().c_str() );
	fy = atof( posY->GetValue().c_str() );
	fz = atof( posZ->GetValue().c_str() );
	fwidth = atof( width->GetValue().c_str() );
	fheight = atof( height->GetValue().c_str() );
	flength = atof( length->GetValue().c_str() );
	
	DestroyMembers();
	std::ofstream tempFile;
	tempFile.open( tempFileName, std::ios::app );
	tempFile << "Position: " << fx << "," << fy << "," << fz << "\nSize= " << flength << "," << fwidth << "," << fheight << "\n";
	tempFile.close();
	Optimus::cSceneHandler::getInstance()->AddTriggerBox( Optimus::Vector3( fx, fy, fz ), Optimus::Vector3( fwidth, fheight, flength ) );
}

void MainFrame::DestroyMembers()
{
	if( posX )
	{
		delete posX;
		posX = NULL;
	}
	if( posY )
	{
		delete posY;
		posY = NULL;
	}
	if( posZ )
	{
		delete posZ;
		posZ = NULL;
	}
	if( width )
	{
		delete width;
		width = NULL;
	}
	if( height )
	{
		delete height;
		height = NULL;
	}
	if( length )
	{
		delete length;
		length = NULL;
	}
	if( okButton )
	{
		delete okButton;
		okButton = NULL;
	}
	if( panel )
	{
		delete panel;
		panel = NULL;
	}
	xDone = yDone = zDone = widthDone = heightDone = lengthDone = false;
	/*posX->Destroy();
	posY->Destroy();
	posZ->Destroy();
	width->Destroy();
	height->Destroy();
	length->Destroy();
	panel->Destroy();
	okButton->Destroy();*/
}

void MainFrame::OnQuit(wxCommandEvent& WXUNUSED(event))
{
	if( worldInitiated )
	{
		Optimus::World::getInstance()->Shutdown();
		for( unsigned int i = 0; i < m_tempWaypoints.size(); i++ )
		{
			if( m_tempWaypoints[i] )
			{
				delete m_tempWaypoints[i];
				m_tempWaypoints[i] = NULL;
			}
		}
	}
    Close(true);
}

void MainFrame::WriteToFile( wxCommandEvent& event )
{
	if( worldInitiated )
	{
		std::ifstream tempFile;
		tempFile.open( tempFileName );
		int numberLines = 0;
		
		char c = 'a';
		float _posX, _posY, _posZ, _width, _height, _depth;
		std::vector<Optimus::Vector3> _pos, _size;
		ReadBackDynamicData( _pos, _size );
		std::ofstream targetFile ( "TriggersBin.bin", std::ios::binary | std::ios::out );
		if( !( tempFile.peek() == std::ifstream::traits_type::eof() ) )
		{
			//Add info to binary file.
			while( !tempFile.eof() )
			{
				tempFile >> c;
				if( c == ':' )
				{
					tempFile >> _posX >> c >> _posY >> c >> _posZ;
					numberLines++;
					_pos.push_back( Optimus::Vector3( _posX, _posY, _posZ ) );
				}
				else if( c == '=' )
				{
					tempFile >> _width >> c >> _height >> c >> _depth;
					numberLines++;
					_size.push_back( Optimus::Vector3( _width, _height, _depth ) );
				}
			}
		}
		tempFile.close();
		assert( _pos.size() == _size.size() );
		int size = Optimus::WayPointGraph::GetInstance()->m_listNodes.size();
		for( unsigned int i = 0; i < size; i++ )
		{
			m_tempWaypoints.push_back( Optimus::WayPointGraph::GetInstance()->m_listNodes[i] );
		}
		//Read in current Hard Surfaces ( meshes )
		for( unsigned int i = 0; i < Optimus::cSceneHandler::getInstance()->m_opaqueEnts.size(); i++ )
		{
			if( Optimus::cSceneHandler::getInstance()->m_opaqueEnts[i]->m_hardSurface )
			{
				m_hardSurfaceIDs.push_back( i );
			}
		}
		numberLines = _pos.size();
		targetFile.write( reinterpret_cast<const char*>(&numberLines), sizeof( int ) ); //Nimber of TriggerBoxes
		for( unsigned int i = 0 ; i < _pos.size(); i++ )
		{
			targetFile.write( reinterpret_cast<const char*>(&_pos[i]), sizeof( Optimus::Vector3 ) );  //Positions of TriggerBoxes
			targetFile.write( reinterpret_cast<const char*>(&_size[i]), sizeof( Optimus::Vector3 ) ); //Sizes of TriggerBoxes
		}
		numberLines = m_meshIDs.size();
		targetFile.write( reinterpret_cast<const char*>(&numberLines), sizeof( int ) ); //Number of Dynamic Meshes
		for( unsigned int i = 0; i < m_meshIDs.size(); i++ )
		{
			targetFile.write( reinterpret_cast<const char*>(&m_meshIDs[i]), sizeof( int ) ); //ID's of Dynamic Meshes
		}
		numberLines = m_tempWaypoints.size();
		unsigned int neighbourCount;
		targetFile.write( reinterpret_cast<const char*>(&numberLines), sizeof( int ) ); //Number of WayPoints
		for( unsigned int i = 0; i < m_tempWaypoints.size(); i++ )
		{
			neighbourCount = m_tempWaypoints[i]->GetNeighbourCount();
			targetFile.write( reinterpret_cast<const char*>( &m_tempWaypoints[i]->GetPosition() ), sizeof( Optimus::Vector3 ) ); //Position of WayPointNode
			targetFile.write( reinterpret_cast<const char*>( &neighbourCount ), sizeof( unsigned int ) ); //Number of Neighbours
			for( unsigned int j = 0; j < m_tempWaypoints[i]->GetNeighbourCount(); j++ )
			{
				targetFile.write( reinterpret_cast<const char*>( &m_tempWaypoints[i]->GetNeighbour( j ).m_neighbour->GetPosition() ), sizeof( Optimus::Vector3 ) ); //Position of Neighbours
			}
		}
		numberLines = m_hardSurfaceIDs.size();
		targetFile.write( reinterpret_cast<const char*>(&numberLines), sizeof( int ) );
		for( unsigned int i = 0; i < m_hardSurfaceIDs.size(); i++ )
		{
			targetFile.write( reinterpret_cast<const char*>(&m_hardSurfaceIDs[i]), sizeof( int ) ); //ID's of Hard Surfaces
		}
		targetFile.close();
		if( std::remove( tempFileName.c_str() ) != 0 )
		{
			int _error = 0;
		}
		SetStatusText("Writing Complete..");
	}
}

void MainFrame::ReadBackDynamicData( std::vector<Optimus::Vector3>& _pos, std::vector<Optimus::Vector3>& _size )
{
	int _dynamicEntityNumber, _tempID;
	Optimus::Vector3 _tempPos, _tempSize;
	std::ifstream targetFile ( "TriggersBin.bin", std::ios::binary | std::ios::in );
	if( targetFile.peek() == std::ifstream::traits_type::eof() )
	{
		return;
	}
	targetFile.read(reinterpret_cast<char*>(&_dynamicEntityNumber), sizeof( int ) );
	if( _dynamicEntityNumber < 0 )
	{
		return;
	}
	for( int i = 0; i < _dynamicEntityNumber; i++ )
	{
		targetFile.read(reinterpret_cast<char*>(&_tempPos), sizeof( Optimus::Vector3 ) );
		targetFile.read(reinterpret_cast<char*>(&_tempSize), sizeof( Optimus::Vector3 ) );
		_pos.push_back( _tempPos );
		_size.push_back( _tempSize );
	}
	targetFile.read(reinterpret_cast<char*>(&_dynamicEntityNumber), sizeof( int ) );

	for( int i = 0; i < _dynamicEntityNumber; i++ )
	{
		targetFile.read(reinterpret_cast<char*>(&_tempID), sizeof( int ) );
		m_meshIDs.push_back(_tempID);
	}
	targetFile.close();
}

void MainFrame::EnterPressed( wxCommandEvent& event )
{
	if( xDone == false )
	{
		xDone = true;
		Disconnect(wxEVT_TEXT_ENTER);
		CreatePanel(event);
	}
	else if( xDone && !yDone )
	{
		yDone = true;
		Disconnect(wxEVT_TEXT_ENTER);
		CreatePanel(event);
	}
	else if( xDone && yDone && !zDone )
	{
		zDone = true;
		CreatePanel(event);
		Disconnect(wxEVT_TEXT_ENTER);
	}
	else if( xDone && yDone && zDone && !widthDone )
	{
		widthDone = true;
		Disconnect(wxEVT_TEXT_ENTER);
		CreatePanel(event);
	}
	else if( xDone && yDone && zDone && widthDone && !heightDone )
	{
		heightDone  = true;
		Disconnect(wxEVT_TEXT_ENTER);
		CreatePanel(event);
	}
	else if( xDone && yDone && zDone && widthDone && heightDone && !lengthDone )
	{
		lengthDone = true;
		Disconnect(wxEVT_TEXT_ENTER);
		CreatePanel(event);
	}
}

void MainFrame::MarkDynamic( wxCommandEvent& WXUNUSED(event) )
{
	//Mark the mesh as dynamic
	if( worldInitiated )
	{
		if( m_currentMeshID >= 0 )
		{
			if( !( std::find(m_meshIDs.begin(), m_meshIDs.end(), m_currentMeshID) != m_meshIDs.end() ))
			{
				m_meshIDs.push_back( m_currentMeshID );
				Optimus::cSceneHandler::getInstance()->m_opaqueEnts[m_currentMeshID]->setDynamic( true );
				Optimus::cSceneHandler::getInstance()->m_opaqueEnts[m_currentMeshID]->setTranslation( D3DXVECTOR3( 0, -10, 0 ) );
			}
			m_currentMeshID = -1;
		}
	}
}

void MainFrame::CreateWayPointNode( wxMouseEvent& event )
{
	if( worldInitiated )
	{
		if( !m_connectMode )
		{
			Optimus::Vector3 _pos = Optimus::Vector3( Optimus::cTPSCamera::getInstance()->m_position.x, Optimus::cTPSCamera::getInstance()->m_position.y, Optimus::cTPSCamera::getInstance()->m_position.z );
			_pos += Optimus::Vector3( Optimus::cTPSCamera::getInstance()->m_forward.x, Optimus::cTPSCamera::getInstance()->m_forward.y, Optimus::cTPSCamera::getInstance()->m_forward.z ) * 10.0f;
			std::ofstream tempFile;
			tempFile.open( tempFileName, std::ios::app );
			tempFile << "Way Point Position- " << _pos.x() << "," << _pos.y() <<  "," << _pos.z()  << "\n";
			tempFile.close();
			Optimus::WayPointNode* _newNode = OPT_NEW Optimus::WayPointNode( _pos, 0 );
			m_tempWaypoints.push_back( _newNode );
		}
		else
		{
			float x = event.GetX();
			float y = event.GetY();
			float windowX = wxWindow::GetSize().GetWidth();
			float windowY = wxWindow::GetSize().GetHeight();
			x = 2 * (x / windowX) - 1;
			y = -2 * (y / windowY) + 1;
			float z = 1.0f;
			D3DXVECTOR3 cameraPos = Optimus::cTPSCamera::getInstance()->m_position;
			float angle = Optimus::cTPSCamera::getInstance()->m_yawRotation;
			D3DXVECTOR3 nearPlaneIntersection = D3DXVECTOR3( -sin(angle) * 0.1f + cos(angle) * x * 0.2f/2 + cameraPos.x, 
														 y * 0.1125f/2 + cameraPos.y, 
														 cos(angle) * 0.1f + sin(angle) * x * 0.2f/2 + cameraPos.z);
			D3DXVECTOR3 ray_clip = nearPlaneIntersection - cameraPos;

			Optimus::Vector3 rayToWorld = Optimus::Vector3( ray_clip.x, ray_clip.y, ray_clip.z );
			Optimus::Vector3 start = Optimus::Vector3( Optimus::cTPSCamera::getInstance()->m_position.x, 
				Optimus::cTPSCamera::getInstance()->m_position.y, 
				Optimus::cTPSCamera::getInstance()->m_position.z );
			Optimus::Vector3 end = start + rayToWorld * 20000;
			m_start = start;
			m_end = end;
			float minDistSqr = FLT_MAX;
			int _currentTriggerbox = -1;
			for( unsigned int i = 0; i < m_tempWaypoints.size(); i++ )
			{
				Optimus::Vector3 halfDim = m_tempWaypoints[i]->GetTriggerBox()->GetDimensions() / 2.0f;
				if( Optimus::Collision::getInstance()->IntersectSegmentAABB( start, end, m_tempWaypoints[i]->GetPosition(), halfDim ) )
				{
					Optimus::Vector3 meshPos = m_tempWaypoints[i]->GetPosition() - Optimus::Vector3( 10.0f );
					float distSqr = ( ( start.x() - meshPos.x() ) * ( start.x() - meshPos.x() )  +
						( start.y() - meshPos.y() ) * ( start.y() - meshPos.y() )  +
						( start.z() - meshPos.z() ) * ( start.z() - meshPos.z() ) );
					if( distSqr < minDistSqr )
					{
						minDistSqr = distSqr;
						_currentTriggerbox = i;
					}
				}
			}
			int _currentTriggerbox2 = -1;
			std::vector<Optimus::WayPointNode*> existingNodes = Optimus::WayPointGraph::GetInstance()->m_listNodes;
			//if( m_tempWaypoints.size() == 0 )
			{
				for( unsigned int i = 0; i < existingNodes.size(); i++ )
				{
					Optimus::Vector3 halfDim = existingNodes[i]->GetTriggerBox()->GetDimensions() / 2.0f;
					if( Optimus::Collision::getInstance()->IntersectSegmentAABB( start, end, existingNodes[i]->GetPosition(), halfDim ) )
					{
						Optimus::Vector3 meshPos = existingNodes[i]->GetPosition() - Optimus::Vector3( 10.0f );
						float distSqr = ( ( start.x() - meshPos.x() ) * ( start.x() - meshPos.x() )  +
							( start.y() - meshPos.y() ) * ( start.y() - meshPos.y() )  +
							( start.z() - meshPos.z() ) * ( start.z() - meshPos.z() ) );
						if( distSqr < minDistSqr )
						{
							minDistSqr = distSqr;
							_currentTriggerbox2 = i;
						}
					}
				}
			}
			Optimus::WayPointNode* _tempTrigger;
			if( _currentTriggerbox != -1 )
			{
					_tempTrigger = m_tempWaypoints[_currentTriggerbox];
					if( _wayPoint1 == NULL )					
						_wayPoint1 = m_tempWaypoints[_currentTriggerbox];					
					else if( m_tempWaypoints[_currentTriggerbox] != _wayPoint1 )
						_wayPoint2 = m_tempWaypoints[_currentTriggerbox];
					_tempTrigger->GetTriggerBox()->SetPosition( _tempTrigger->GetPosition() + Optimus::Vector3( 10.0f ) );	
			}			
			if( _currentTriggerbox2 != -1 )
			{
				_tempTrigger = existingNodes[_currentTriggerbox2];
				if( _wayPoint1 == NULL )					
					_wayPoint1 = existingNodes[_currentTriggerbox2];					
				else if( existingNodes[_currentTriggerbox2] != _wayPoint1 )
					_wayPoint2 = existingNodes[_currentTriggerbox2];
				_tempTrigger->GetTriggerBox()->SetPosition( _tempTrigger->GetPosition() + Optimus::Vector3( 10.0f ) );
			}
			std::vector<Optimus::WayPointNode*> m_wayPoints = Optimus::WayPointGraph::GetInstance()->m_listNodes;
			for( unsigned int i = 0; i < m_wayPoints.size(); i++ )
			{
				if( _currentTriggerbox != -1 && ( ( _tempTrigger->GetPosition() - Optimus::Vector3( 10.0f ) ) == m_wayPoints[i]->GetPosition() ) )
				{
					if( _wayPoint1 == NULL )					
						_wayPoint1 = m_wayPoints[i];					
					else if( m_wayPoints[i] != _wayPoint1 )
						_wayPoint2 = m_wayPoints[i];
				}
			}
			if( _wayPoint2 != NULL && _wayPoint2->GetID() != -1 )
			{
				_wayPoint1->AddNeighbour( _wayPoint2 );
				_wayPoint2->AddNeighbour( _wayPoint1 );
				_wayPoint1->GetTriggerBox()->SetPosition( _wayPoint1->GetPosition() + Optimus::Vector3( - 10.0f ) );
				_wayPoint2->GetTriggerBox()->SetPosition( _wayPoint2->GetPosition() + Optimus::Vector3( - 10.0f ) );
				_wayPoint1 = NULL;
				_wayPoint2 = NULL;
				m_connectMode = false;
				SetStatusText( "Connect Mode is False" );
			}
		}
	}
}