#ifndef ENTRY_POINT_H
#define ENTRY_POINT_H

int		cInitialise( int i_width, int i_height, HINSTANCE i_thisInstanceOfTheApplication, int i_initialWindowDisplayState, std::string i_sceneName );
int		cInitialise( unsigned int i_width, unsigned int i_height, HWND i_window, int i_initialWindowDisplayState, std::string i_sceneName );

int		cUpdate();

void	cShutDown();

#endif