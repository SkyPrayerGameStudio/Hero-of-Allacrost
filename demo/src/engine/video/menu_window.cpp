///////////////////////////////////////////////////////////////////////////////
//            Copyright (C) 2004-2007 by The Allacrost Project
//                         All Rights Reserved
//
// This code is licensed under the GNU GPL version 2. It is free software
// and you may modify it and/or redistribute it under the terms of this license.
// See http://www.gnu.org/copyleft/gpl.html for details.
///////////////////////////////////////////////////////////////////////////////

#include <sstream>

#include "utils.h"
#include "video.h"
#include "gui.h"
#include "menu_window.h"

using namespace std;
using namespace hoa_utils;
using namespace hoa_video;
using namespace hoa_video::private_video;


namespace hoa_video {

MenuWindow::MenuWindow() :
	_width(0),
	_height(0),
	_skin(NULL),
	_window_state(VIDEO_MENU_STATE_HIDDEN),
	_display_timer(0),
	_display_mode(VIDEO_MENU_INSTANT),
	_is_scissored(false)
{
	_id = GUIManager->GetNextMenuWindowID();
	_initialized = IsInitialized(_initialization_errors);
}



bool MenuWindow::Create(string skin_name, float w, float h, int32 visible_flags, int32 shared_flags) {
	_skin = GUIManager->GetMenuSkin(skin_name);
	if (_skin == NULL) {
		if (VIDEO_DEBUG)
			cerr << "VIDEO WARNING: MenuWindow::Create() failed because the requested menu skin was not found: "
				<< skin_name << endl;
		return false;
	}

	return Create(w, h, visible_flags, shared_flags);
}



bool MenuWindow::Create(float w, float h, int32 visible_flags, int32 shared_flags) {
	if (w <= 0 || h <= 0) {
		if (VIDEO_DEBUG)
			cerr << "VIDEO WARNING: MenuWindow::Create() failed because an invalid width or height was passed in: "
				<< "(width = " << w << ", height = " << h << ")" << endl;
		return false;
	}

	_width = w;
	_height = h;
	_edge_visible_flags = visible_flags;
	_edge_shared_flags = shared_flags;

	if (_skin == NULL) {
		_skin = GUIManager->GetDefaultMenuSkin();
	}
	if (_skin == NULL) {
		if (VIDEO_DEBUG)
			cerr << "VIDEO WARNING: MenuWindow::Create() failed because a default menu skin was unavailable "
				<< "(no skins were loaded)" << endl;
			return false;
	}

	if (_RecreateImage() == false) {
		if (VIDEO_DEBUG)
			cerr << "VIDEO WARNING: MenuWindow::Create() failed because the window could not be recreated" << endl;
		return false;
	}

	// Add the new menu window to the menu map
	GUIManager->AddMenuWindow(this);
	_initialized = IsInitialized(_initialization_errors);
	return true;
}



void MenuWindow::Destroy() {
	_skin = NULL;
	GUIManager->RemoveMenuWindow(this);
	VideoManager->DeleteImage(_menu_image);
}



void MenuWindow::Update(uint32 frame_time) {
	_display_timer += frame_time;

	if (_display_timer >= VIDEO_MENU_SCROLL_TIME) {
		if (_window_state == VIDEO_MENU_STATE_SHOWING)
			_window_state = VIDEO_MENU_STATE_SHOWN;
		else if (_window_state == VIDEO_MENU_STATE_HIDING)
			_window_state = VIDEO_MENU_STATE_HIDDEN;
	}

	if (_window_state == VIDEO_MENU_STATE_HIDDEN || _window_state == VIDEO_MENU_STATE_SHOWN) {
//		if (_is_scissored == true) {
			float x_buffer = (_width - _inner_width) / 2;
			float y_buffer = (_height - _inner_height) / 2;

			float left, right, bottom, top;
			left = 0.0f;
			right = _width;
			bottom = 0.0f;
			top = _height;

			VideoManager->PushState();
			VideoManager->SetDrawFlags(_xalign, _yalign, 0);
			CalculateAlignedRect(left, right, bottom, top);
			VideoManager->PopState();

			_scissor_rect = VideoManager->CalculateScreenRect(left, right, bottom, top);

			_scissor_rect.left   += static_cast<int32>(x_buffer);
			_scissor_rect.width  -= static_cast<int32>(x_buffer * 2);
			_scissor_rect.top    += static_cast<int32>(y_buffer);
			_scissor_rect.height -= static_cast<int32>(y_buffer * 2);
// 		}

		_is_scissored = false;
		return;
	}

	_is_scissored = true;

	// Holds the amount of the window that should be drawn (1.0 == 100%)
	float draw_percent = 1.0f;

	if (_display_mode != VIDEO_MENU_INSTANT && _window_state != VIDEO_MENU_STATE_SHOWN) {
		float time = static_cast<float>(_display_timer) / static_cast<float>(VIDEO_MENU_SCROLL_TIME);
		if (time > 1.0f)
			time = 1.0f;

		if (_window_state == VIDEO_MENU_STATE_HIDING)
			time = 1.0f - time;

		draw_percent = time;
	}

	if (draw_percent != 1.0f) {
		if (_display_mode == VIDEO_MENU_EXPAND_FROM_CENTER) {
			float left, right, bottom, top;
			left = 0.0f;
			right = _width;
			bottom = 0.0f;
			top = _height;

			VideoManager->PushState();
			VideoManager->SetDrawFlags(_xalign, _yalign, 0);
			CalculateAlignedRect(left, right, bottom, top);
			VideoManager->PopState();

			float center = (top + bottom) * 0.5f;

			bottom = center * (1.0f - draw_percent) + bottom * draw_percent;
			top    = center * (1.0f - draw_percent) + top * draw_percent;

			_scissor_rect = VideoManager->CalculateScreenRect(left, right, bottom, top);
		}
	}
} // void MenuWindow::Update(uint32 frame_time)



void MenuWindow::Draw() {
	if (_initialized == false) {
		if (VIDEO_DEBUG)
			cerr << "VIDEO WARNING: MenuWindow::Draw() failed because the menu window was not initialized:\n" << _initialization_errors << endl;
		return;
	}

	if (_window_state == VIDEO_MENU_STATE_HIDDEN)
		return;

	VideoManager->_PushContext();
	VideoManager->SetDrawFlags(_xalign, _yalign, VIDEO_BLEND, 0);

	if (_is_scissored) {
		ScreenRect rect = _scissor_rect;
		if (VideoManager->IsScissoringEnabled()) {
			rect.Intersect(VideoManager->GetScissorRect());
		}
		else {
			VideoManager->EnableScissoring(true);
		}
		VideoManager->SetScissorRect(rect);
	}

	VideoManager->Move(_x_position, _y_position);
	VideoManager->DrawImage(_menu_image, Color::white);

	VideoManager->_PopContext();
	return;
} // void MenuWindow::Draw()



void MenuWindow::Show() {
	if (_initialized == false) {
		if (VIDEO_DEBUG)
			cerr << "VIDEO WARNING: MenuWindow::Show() failed because the menu window was not initialized:\n" << _initialization_errors << endl;
		return;
	}

	if (_window_state == VIDEO_MENU_STATE_SHOWING || _window_state == VIDEO_MENU_STATE_SHOWN) {
		return;
	}

	_display_timer = 0;

	if (_display_mode == VIDEO_MENU_INSTANT)
		_window_state = VIDEO_MENU_STATE_SHOWN;
	else
		_window_state = VIDEO_MENU_STATE_SHOWING;
} // void MenuWindow::Show()



void MenuWindow::Hide() {
	if (_initialized == false) {
		if (VIDEO_DEBUG)
			cerr << "VIDEO WARNING: MenuWindow::Hide() failed because the menu window was not initialized:\n" << _initialization_errors << endl;
		return;
	}

	if (_window_state == VIDEO_MENU_STATE_HIDING || _window_state == VIDEO_MENU_STATE_HIDDEN) {
		return;
	}

	_display_timer = 0;

	if (_display_mode == VIDEO_MENU_INSTANT)
		_window_state = VIDEO_MENU_STATE_HIDDEN;
	else
		_window_state = VIDEO_MENU_STATE_HIDING;
} // void MenuWindow::Hide()



bool MenuWindow::IsInitialized(string& errors) {
	errors.clear();
	ostringstream stream;

	// Check width
	if (_width <= 0.0f || _width > 1024.0f)
		stream << "* Invalid width (" << _width << ")" << endl;

	// Check height
	if (_height <= 0.0f || _height > 768.0f)
		stream << "* Invalid height (" << _height << ")" << endl;

	// Check display mode
	if (_display_mode <= VIDEO_MENU_INVALID || _display_mode >= VIDEO_MENU_TOTAL)
		stream << "* Invalid display mode (" << _display_mode << ")" << endl;

	// Check state
	if (_window_state <= VIDEO_MENU_STATE_INVALID || _window_state >= VIDEO_MENU_STATE_TOTAL)
		stream << "* Invalid state (" << _window_state << ")" << endl;

	// Check to see a valid image is loaded
	if (_menu_image.GetWidth() == 0)
		stream << "* Menu image is not loaded" << endl;

	// Check to see that a valid menu skin is being used
	if (_skin == NULL)
		stream << "* No menu skin is assigned" << endl;

	errors = stream.str();

	if (errors.empty()) {
		_initialized = true;
	}
	else {
		_initialized = false;
	}

	return _initialized;
} // bool MenuWindow::IsInitialized(string& errors)



void MenuWindow::ChangeEdgeVisibleFlags(int32 flags) {
	_edge_visible_flags = flags;
	_RecreateImage();
}



void MenuWindow::ChangeEdgeSharedFlags(int32 flags) {
	_edge_shared_flags = flags;
	_RecreateImage();
}



void MenuWindow::ChangeMenuSkin(string& skin_name) {
	MenuSkin* new_skin = GUIManager->GetMenuSkin(skin_name);
	if (new_skin == NULL) {
		if (VIDEO_DEBUG)
			cerr << "VIDEO WARNING: MenuWindow::ChangeMenuSkin() failed because the skin_name \""
				<< skin_name << "\" was invalid" << endl;
		return;
	}
	_skin = new_skin;
	_RecreateImage();
}



void MenuWindow::SetDisplayMode(VIDEO_MENU_DISPLAY_MODE mode) {
	if (mode <= VIDEO_MENU_INVALID || mode >= VIDEO_MENU_TOTAL) {
		if (VIDEO_DEBUG)
			cerr << "VIDEO WARNING: MenuWindow::SetDisplayMode() failed because an invalid argument was given: " << mode << endl;
		return;
	}

	_display_mode = mode;
	_initialized = IsInitialized(_initialization_errors);
}



bool MenuWindow::_RecreateImage() {
	if (_skin == NULL) {
		if (VIDEO_DEBUG) {
			cerr << "VIDEO ERROR: In MenuWindow::_RecreateImage(), there was no menu skin set" << endl;
		}
		return false;
	}

	VideoManager->DeleteImage(_menu_image);
	_menu_image.Clear();

	// Get information about the border sizes
	float left_border_size   = _skin->borders[1][0].GetWidth();
	float right_border_size  = _skin->borders[1][2].GetWidth();
	float top_border_size    = _skin->borders[2][1].GetHeight();
	float bottom_border_size = _skin->borders[0][1].GetHeight();

	float horizontal_border_size = left_border_size + right_border_size;
	float vertical_border_size   = top_border_size  + bottom_border_size;

	float top_width   = _skin->borders[2][1].GetWidth();
	float left_height = _skin->borders[1][0].GetHeight();

	// Calculate how many times the top/bottom images have to be tiled in order to make up the width of the window
	_inner_width = _width - horizontal_border_size;
	_inner_height = _height - vertical_border_size;

	if (_inner_width < 0.0f) {
		if (VIDEO_DEBUG) {
			cerr << "VIDEO ERROR: In MenuWindow::_RecreateImage(), _inner_width was negative" << endl;
		}
		return false;
	}

	if (_inner_height < 0.0f) {
		if (VIDEO_DEBUG) {
			cerr << "VIDEO ERROR: In MenuWindow::_RecreateImage(), _inner_height was negative" << endl;
		}
		return false;
	}

	// Will be true if there is a background image for the menu skin being used
	bool background_loaded = _skin->background.GetWidth();

	// Find how many times we have to tile the border images to fit the dimensions given
	float num_x_tiles = _inner_width  / top_width;
	float num_y_tiles = _inner_height / left_height;

	int32 inum_x_tiles = static_cast<int32>(num_x_tiles);
	int32 inum_y_tiles = static_cast<int32>(num_y_tiles);

	// Ideally, the border sizes should evenly divide into the window sizes but the person who
	// created the window might have passed in a width and height that is not a multiple of
	// the border sizes. If this is the case, we have to extend the dimensions a little bit.
	float dnum_x_tiles = num_x_tiles - inum_x_tiles;
	float dnum_y_tiles = num_y_tiles - inum_y_tiles;

	if (dnum_x_tiles > 0.001f) {
		float width_adjust = (1.0f - dnum_x_tiles) * top_width;
		_width += width_adjust;
		_inner_width += width_adjust;
		++inum_x_tiles;
	}

	if (dnum_y_tiles > 0.001f) {
		float height_adjust = (1.0f - dnum_y_tiles) * top_width;
		_height += height_adjust;
		_inner_height += height_adjust;
		++inum_y_tiles;
	}

	// Now we have all the information we need to create the window.
	// Begin by re-creating the overlay at the correct width and height
	Color c[4];
	_skin->borders[1][1].GetVertexColor(c[0], 0);
	_skin->borders[1][1].GetVertexColor(c[1], 1);
	_skin->borders[1][1].GetVertexColor(c[2], 2);
	_skin->borders[1][1].GetVertexColor(c[3], 3);

	VideoManager->DeleteImage(_skin->borders[1][1]);
	_skin->borders[1][1].SetDimensions(left_border_size, top_border_size);
	_skin->borders[1][1].SetVertexColors(c[0], c[1], c[2], c[3]);
	VideoManager->LoadImage(_skin->borders[1][1]);


	// If a valid background image is loaded, then tile the interior of the window with it
	if (background_loaded) {
		_skin->background.SetVertexColors(c[0], c[1], c[2], c[3]);

		float width = _skin->background.GetWidth();
		float height = _skin->background.GetHeight();

		float min_x = 0;
		float min_y = 0;

		float max_x = _inner_width + horizontal_border_size;
		float max_y = _inner_height + vertical_border_size;

		if (_edge_visible_flags & VIDEO_MENU_EDGE_TOP)
			max_y -= (top_border_size / 2);
		if (_edge_visible_flags & VIDEO_MENU_EDGE_BOTTOM)
			min_y += (bottom_border_size / 2);
		if (_edge_visible_flags & VIDEO_MENU_EDGE_LEFT)
			min_x += (left_border_size / 2);
		if (_edge_visible_flags & VIDEO_MENU_EDGE_RIGHT)
			max_x -= (right_border_size / 2);

		for (float y = min_y ; y < max_y; y += height) {
			for (float x = min_x; x < max_x; x += width) {
				float u = 1.0, v = 1.0;

				if (x + width > max_x)
					u = (max_x - x) / width;
				if (y + height > max_y)
					v = (max_y - y) / height;

				_menu_image.AddImage(_skin->background, x, y, 0, 0, u, v);
			}
		}
	}
	else {
		// Otherwise re-create the overlay at the correct width and height
		VideoManager->DeleteImage(_skin->borders[1][1]);
		_skin->borders[1][1].SetDimensions(_inner_width, _inner_height);
		_skin->borders[1][1].SetVertexColors(c[0], c[1], c[2], c[3]);
		VideoManager->LoadImage(_skin->borders[1][1]);

		_menu_image.AddImage(_skin->borders[1][1], left_border_size, bottom_border_size);
	}

	// First create the corners of the image
	float max_x = left_border_size + inum_x_tiles * top_width;
	float max_y = bottom_border_size + inum_y_tiles * left_height;
	float min_x = 0.0f;
	float min_y = 0.0f;

	// Bottom left
	if (_edge_visible_flags & VIDEO_MENU_EDGE_LEFT && _edge_visible_flags & VIDEO_MENU_EDGE_BOTTOM) {
		if (_edge_shared_flags & VIDEO_MENU_EDGE_LEFT && _edge_shared_flags & VIDEO_MENU_EDGE_BOTTOM)
			_menu_image.AddImage(_skin->connectors[4], min_x, min_y);
		else if (_edge_shared_flags & VIDEO_MENU_EDGE_LEFT)
			_menu_image.AddImage(_skin->connectors[1], min_x, min_y);
		else if (_edge_shared_flags & VIDEO_MENU_EDGE_BOTTOM)
			_menu_image.AddImage(_skin->connectors[2], min_x, min_y);
		else
			_menu_image.AddImage(_skin->borders[2][0], min_x, min_y);
	}
	else if (_edge_visible_flags & VIDEO_MENU_EDGE_LEFT)
		_menu_image.AddImage(_skin->borders[1][0], min_x, min_y);
	else if (_edge_visible_flags & VIDEO_MENU_EDGE_BOTTOM)
		_menu_image.AddImage(_skin->borders[0][1], min_x, min_y);
	else if (!background_loaded)
		_menu_image.AddImage(_skin->borders[1][1], min_x, min_y);

	// Bottom right
	if (_edge_visible_flags & VIDEO_MENU_EDGE_RIGHT && _edge_visible_flags & VIDEO_MENU_EDGE_BOTTOM) {
		if (_edge_shared_flags & VIDEO_MENU_EDGE_RIGHT && _edge_shared_flags & VIDEO_MENU_EDGE_BOTTOM)
			_menu_image.AddImage(_skin->connectors[4], max_x, min_y);
		else if (_edge_shared_flags & VIDEO_MENU_EDGE_RIGHT)
			_menu_image.AddImage(_skin->connectors[1], max_x, min_y);
		else if (_edge_shared_flags & VIDEO_MENU_EDGE_BOTTOM)
			_menu_image.AddImage(_skin->connectors[3], max_x, min_y);
		else
			_menu_image.AddImage(_skin->borders[2][2], max_x, min_y);
	}
	else if (_edge_visible_flags & VIDEO_MENU_EDGE_RIGHT)
		_menu_image.AddImage(_skin->borders[1][2], max_x, min_y);
	else if (_edge_visible_flags & VIDEO_MENU_EDGE_BOTTOM)
		_menu_image.AddImage(_skin->borders[2][1], max_x, min_y);
	else if (!background_loaded)
		_menu_image.AddImage(_skin->borders[1][1], max_x, min_y);

	// Top left
	if (_edge_visible_flags & VIDEO_MENU_EDGE_LEFT && _edge_visible_flags & VIDEO_MENU_EDGE_TOP) {
		if (_edge_shared_flags & VIDEO_MENU_EDGE_LEFT && _edge_shared_flags & VIDEO_MENU_EDGE_TOP)
			_menu_image.AddImage(_skin->connectors[4], min_x, max_y);
		else if (_edge_shared_flags & VIDEO_MENU_EDGE_LEFT)
			_menu_image.AddImage(_skin->connectors[0], min_x, max_y);
		else if (_edge_shared_flags & VIDEO_MENU_EDGE_TOP)
			_menu_image.AddImage(_skin->connectors[2], min_x, max_y);
		else
			_menu_image.AddImage(_skin->borders[0][0], min_x, max_y);
	}
	else if (_edge_visible_flags & VIDEO_MENU_EDGE_LEFT)
		_menu_image.AddImage(_skin->borders[1][0], min_x, max_y);
	else if (_edge_visible_flags & VIDEO_MENU_EDGE_TOP)
		_menu_image.AddImage(_skin->borders[0][1], min_x, max_y);
	else if (!background_loaded)
		_menu_image.AddImage(_skin->borders[1][1], min_x, max_y);

	// Top right
	if (_edge_visible_flags & VIDEO_MENU_EDGE_TOP && _edge_visible_flags & VIDEO_MENU_EDGE_RIGHT) {
		if (_edge_shared_flags & VIDEO_MENU_EDGE_RIGHT && _edge_shared_flags & VIDEO_MENU_EDGE_TOP)
			_menu_image.AddImage(_skin->connectors[4], max_x, max_y);
		else if (_edge_shared_flags & VIDEO_MENU_EDGE_RIGHT)
			_menu_image.AddImage(_skin->connectors[0], max_x, max_y);
		else if (_edge_shared_flags & VIDEO_MENU_EDGE_TOP)
			_menu_image.AddImage(_skin->connectors[3], max_x, max_y);
		else
			_menu_image.AddImage(_skin->borders[0][2], max_x, max_y);
	}
	else if (_edge_visible_flags & VIDEO_MENU_EDGE_TOP)
		_menu_image.AddImage(_skin->borders[0][1], max_x, max_y);
	else if (_edge_visible_flags & VIDEO_MENU_EDGE_RIGHT)
		_menu_image.AddImage(_skin->borders[1][2], max_x, max_y);
	else if (!background_loaded)
		_menu_image.AddImage(_skin->borders[1][1], max_x, max_y);

	// Iterate from left to right and fill in the horizontal borders
	for (int32 tile_x = 0; tile_x < inum_x_tiles; ++tile_x) {
		if (_edge_visible_flags & VIDEO_MENU_EDGE_TOP)
			_menu_image.AddImage(_skin->borders[0][1], left_border_size + top_width * tile_x, max_y);
		else if (!background_loaded)
			_menu_image.AddImage(_skin->borders[1][1], left_border_size + top_width * tile_x, max_y);

		if (_edge_visible_flags & VIDEO_MENU_EDGE_BOTTOM)
			_menu_image.AddImage(_skin->borders[2][1], left_border_size + top_width * tile_x, 0.0f);
		else if (!background_loaded)
			_menu_image.AddImage(_skin->borders[1][1], left_border_size + top_width * tile_x, 0.0f);
	}

	// Iterate from bottom to top and fill in the vertical borders
	for (int32 tile_y = 0; tile_y < inum_y_tiles; ++tile_y) {
		if (_edge_visible_flags & VIDEO_MENU_EDGE_LEFT)
			_menu_image.AddImage(_skin->borders[1][0], 0.0f, bottom_border_size + left_height * tile_y);
		else if (!background_loaded)
			_menu_image.AddImage(_skin->borders[1][1], 0.0f, bottom_border_size + left_height * tile_y);

		if (_edge_visible_flags & VIDEO_MENU_EDGE_RIGHT)
			_menu_image.AddImage(_skin->borders[1][2], max_x, bottom_border_size + left_height * tile_y);
		else if (!background_loaded)
			_menu_image.AddImage(_skin->borders[1][1], max_x, bottom_border_size + left_height * tile_y);
	}

	return true;
}

}  // namespace hoa_video
