///////////////////////////////////////////////////////////////////////////////
//            Copyright (C) 2004, 2005 by The Allacrost Project
//                       All Rights Reserved
//
// This code is licensed under the GNU GPL. It is free software and you may
// modify it and/or redistribute it under the terms of this license.
// See http://www.gnu.org/copyleft/gpl.html for details.
///////////////////////////////////////////////////////////////////////////////

/*!****************************************************************************
 * \file    menu.cpp
 * \author  Tyler Olsen, roots@allacrost.org
 * \date    Last Updated: August 12th, 2005
 * \brief   Source file for menu mode interface.
 *****************************************************************************/

#include "utils.h"
#include <iostream>
#include "menu.h"
#include "audio.h"
#include "video.h"
#include "engine.h"
#include "global.h"
#include "data.h"

using namespace std;
using namespace hoa_menu::private_menu;
using namespace hoa_utils;
using namespace hoa_audio;
using namespace hoa_video;
using namespace hoa_engine;
using namespace hoa_global;
using namespace hoa_data;



namespace hoa_menu {

bool MENU_DEBUG = false;


MenuMode::MenuMode() {
	if (MENU_DEBUG) cout << "MENU: MenuMode constructor invoked." << endl;

	// Save the currently drawn screen
	if (!VideoManager->CaptureScreen(_saved_screen)) {
		cerr << "MENU: ERROR: Couldn't save the screen!" << endl;
	}
	
}


MenuMode::~MenuMode() {
	if (MENU_DEBUG) cout << "MENU: MenuMode destructor invoked." << endl;
	
	// Remove saved images
	VideoManager->DeleteImage(_saved_screen);
	
	for (uint32 i = 0; i < _menu_images.size(); i++) {
		VideoManager->DeleteImage(_menu_images[i]);
	}
	
	// Destroy menu windows
	_character_window0.Destroy();
	_character_window1.Destroy();
	_character_window2.Destroy();
	_character_window3.Destroy();
	_bottom_window.Destroy();
}


// Resets appropriate class members
void MenuMode::Reset() {
	VideoManager->SetCoordSys(0, 1024, 768, 0); // Top left corner coordinates are (0,0)
	if(!VideoManager->SetFont("default")) 
		cerr << "MAP: ERROR > Couldn't set menu font!" << endl;
	
		// Setup the menu windows
	_character_window0.Create(256, 576);
	_character_window0.SetPosition(0, 0);
	_character_window0.Show();
	_character_window1.Create(256, 576);
	_character_window1.SetPosition(256, 0);
	_character_window1.Show();
	_character_window2.Create(256, 576);
	_character_window2.SetPosition(512, 0);
	_character_window2.Show();
	_character_window3.Create(256, 576);
	_character_window3.SetPosition(768, 0);
	_character_window3.Show();
	_bottom_window.Create(1024, 192);
	_bottom_window.SetPosition(0, 576);
	_bottom_window.Show();
}


void MenuMode::Update(uint32 time_elapsed) {

	if (InputManager->CancelPress()) {
		ModeManager->Pop();
	}
}


void MenuMode::Draw() {
	VideoManager->SetDrawFlags(VIDEO_X_LEFT, VIDEO_Y_TOP, VIDEO_BLEND, 0);
	// Move to the top left corner
	VideoManager->Move(0,0);
	
	// Draw the saved screen as the menu background
	VideoManager->DrawImage(_saved_screen); 
	
	// Draw the four character menus
	_character_window0.Draw();
	_character_window1.Draw();
	_character_window2.Draw();
	_character_window3.Draw();
	_bottom_window.Draw();
	
	VideoManager->SetDrawFlags(VIDEO_X_LEFT, VIDEO_Y_BOTTOM, 0);
	
	// Draw 1st character menu text
	VideoManager->Move(32, 40);
	if (!VideoManager->DrawText("Claudius"))
		cerr << "MENU: ERROR > Couldn't draw text!" << endl;
	
	VideoManager->MoveRelative(0, 468);
	if (!VideoManager->DrawText("Health: 68"))
		cerr << "MENU: ERROR > Couldn't draw text!" << endl;
		
	VideoManager->MoveRelative(0, -35);
	if (!VideoManager->DrawText("Skill: 23"))
		cerr << "MENU: ERROR > Couldn't draw text!" << endl;
		
	VideoManager->MoveRelative(0, 85);
	if (!VideoManager->DrawText("XP to level: 498"))
		cerr << "MENU: ERROR > Couldn't draw text!" << endl;
		
	// Draw selection menu text
	VideoManager->MoveRelative(0, 74);
	if (!VideoManager->DrawText("Inventory     Skills     Equipment     Status     Options     Save"))
		cerr << "MENU: ERROR > Couldn't draw text!" << endl;
	
	// Draw 2nd menu text
	VideoManager->MoveRelative(0, 80);
	if (!VideoManager->DrawText("Time: 00:24:35"))
		cerr << "MENU: ERROR > Couldn't draw text!" << endl;
		
	VideoManager->MoveRelative(0, 24);
	if (!VideoManager->DrawText("Bling: 4,201B"))
		cerr << "MENU: ERROR > Couldn't draw text!" << endl;
	
}

} // namespace hoa_menu
