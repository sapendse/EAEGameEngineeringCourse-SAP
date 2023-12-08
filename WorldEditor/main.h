#ifndef _MAIN_H
#define _MAIN_H

#include <wx\wx.h>
#include <wx\event.h>
#include <wx\button.h>
#include "Optimus\_Source\Loop\Optimus.h"
#include "Optimus\_Source\PathFinding\WayPointGraph.h"
#include <fstream>


class MyApp : public wxApp
{
    virtual bool OnInit();
};


class MainFrame : public wxFrame
{
	wxPanel* panel;
	wxTextCtrl* posX;
	wxTextCtrl* posY;
	wxTextCtrl* posZ;
	wxTextCtrl* width;
	wxTextCtrl* height;
	wxTextCtrl* length;
	wxButton* okButton;
	int m_currentMeshID;
	std::vector<int> m_meshIDs, m_hardSurfaceIDs;
	std::string tempFileName;
	Optimus::Vector3 m_start, m_end;
	std::vector<Optimus::WayPointNode*> m_tempWaypoints;
	Optimus::WayPointNode* _wayPoint1;
	Optimus::WayPointNode* _wayPoint2;
	bool xDone, yDone, zDone, widthDone, heightDone, lengthDone, worldInitiated, m_connectMode, m_deleteMode, m_hardSurfaceTagMode;
	Optimus::TriggerBox* m_node1;
	Optimus::TriggerBox* m_node2;

public:
    MainFrame( const wxString& title, const wxPoint& pos, const wxSize& size );
    void OnQuit( wxCommandEvent& event );
    void OnLoad( wxCommandEvent& event );
	void OnIdle( wxIdleEvent& event );
	void OnMouseClick( wxMouseEvent& event );
	void CreatePanel( wxCommandEvent& event );
	void CreateTrigger( wxCommandEvent& event );
	void EnterPressed( wxCommandEvent& event );
	void MarkDynamic( wxCommandEvent& event );
	void DestroyMembers();
	void WriteToFile( wxCommandEvent& event );
	void CreateWayPointNode( wxMouseEvent& event );
	void ReadBackDynamicData( std::vector<Optimus::Vector3>& _pos, std::vector<Optimus::Vector3>& _size );
	void DeleteWayPoint( wxCommandEvent& event );
};

enum
{
    ID_Quit = wxID_EXIT,
	ID_Click = wxID_ANY,
	ID_Enter = wxID_ANY,
	ID_Update = wxID_ANY,
	ID_WayPointNodeCreate = wxID_ANY,
	ID_Write,
	ID_Mark,
	ID_DeleteWayPoint,
	ID_Load = wxID_NEW,
	ID_Create = wxID_OPEN
};

#endif