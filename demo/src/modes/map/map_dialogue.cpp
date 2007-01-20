///////////////////////////////////////////////////////////////////////////////
//            Copyright (C) 2004-2006 by The Allacrost Project
//                         All Rights Reserved
//
// This code is licensed under the GNU GPL version 2. It is free software
// and you may modify it and/or redistribute it under the terms of this license.
// See http://www.gnu.org/copyleft/gpl.html for details.
///////////////////////////////////////////////////////////////////////////////

/** ****************************************************************************
*** \file    map_dialogue.cpp
*** \author  Tyler Olsen, roots@allacrost.org
*** \brief   Source file for map mode dialogue.
*** ***************************************************************************/

#include "utils.h"
#include "map.h"
#include "map_objects.h"
#include "map_dialogue.h"
#include "audio.h"
#include "video.h"
#include "global.h"
#include "script.h"
#include "battle.h"
#include "menu.h"

using namespace std;
using namespace hoa_utils;
using namespace hoa_audio;
using namespace hoa_video;
using namespace hoa_global;
using namespace hoa_script;
using namespace hoa_battle;
using namespace hoa_menu;
using namespace luabind;

namespace hoa_map {

namespace private_map {

// ****************************************************************************
// *********************** MapDialogue Class Functions ************************
// ****************************************************************************

MapDialogue::MapDialogue() {
	if (MAP_DEBUG)
		cout << "MAP: MapDialogue constructor invoked" << endl;
	_current_line = 0;
	_seen = false;
}



MapDialogue::~MapDialogue() {
	if (MAP_DEBUG)
		cout << "MAP: MapDialogue destructor invoked" << endl;
}



const bool MapDialogue::ReadNextLine() {
	if ( ++_current_line >= _text.size() ) {
		_current_line = 0;
		_seen = true;
		return false;
	}
	return true;
}



void MapDialogue::AddText( const uint32 speaker_id, const hoa_utils::ustring text, SpriteAction* action )
{
	_speakers.push_back( speaker_id );
	_text.push_back( text );
	_actions.push_back( action );
}

} // namespace private_map

} // namespace hoa_map
