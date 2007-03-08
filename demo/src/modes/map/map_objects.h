///////////////////////////////////////////////////////////////////////////////
//            Copyright (C) 2004-2006 by The Allacrost Project
//                         All Rights Reserved
//
// This code is licensed under the GNU GPL version 2. It is free software
// and you may modify it and/or redistribute it under the terms of this license.
// See http://www.gnu.org/copyleft/gpl.html for details.
///////////////////////////////////////////////////////////////////////////////

/** ****************************************************************************
*** \file    map_objects.h
*** \author  Tyler Olsen, roots@allacrost.org
*** \brief   Header file for map mode objects.
*** *****************************************************************************/

#ifndef __MAP_OBJECTS_HEADER__
#define __MAP_OBJECTS_HEADER__

#include "utils.h"
#include "defs.h"
#include "video.h"
#include "map_actions.h"
#include "map_dialogue.h"
#include "map_zones.h"

namespace hoa_map {

namespace private_map {

// *********************** SPRITE CONSTANTS **************************

/** \name Map Sprite Speeds
*** \brief Common speeds for sprite movement.
*** These values are the time (in milliseconds) that it takes a sprite to walk
*** the distance of one map grid (16 pixels).
**/
//@{
const float VERY_SLOW_SPEED = 300.0f;
const float SLOW_SPEED      = 250.0f;
const float NORMAL_SPEED    = 200.0f;
const float FAST_SPEED      = 150.0f;
const float VERY_FAST_SPEED = 100.0f;
//@}

/** \name Sprite Direction Constants
*** \brief Constants used for setting and determining sprite directions
*** Sprites are allowed to travel in eight different directions, however the sprite itself
*** can only be facing one of four ways: north, south, east, or west. Because of this, it
*** is possible to travel, for instance, northwest facing north <i>or</i> northwest facing west.
*** The "NW_NORTH" constant means that the sprite is traveling to the northwest and is
*** facing towards the north.
***
*** \note The set of "FACING_DIRECTION" and "MOVING_DIRECTION" constants are only meant to be
*** used as shorthands. You shouldn't assign the MapSprite#direction member to any of these values.
**/
//@{
const uint16 NORTH     = 0x0001;
const uint16 SOUTH     = 0x0002;
const uint16 WEST      = 0x0004;
const uint16 EAST      = 0x0008;
const uint16 NW_NORTH  = 0x0010;
const uint16 NW_WEST   = 0x0020;
const uint16 NE_NORTH  = 0x0040;
const uint16 NE_EAST   = 0x0080;
const uint16 SW_SOUTH  = 0x0100;
const uint16 SW_WEST   = 0x0200;
const uint16 SE_SOUTH  = 0x0400;
const uint16 SE_EAST   = 0x0800;

const uint16 NORTHWEST = NW_NORTH | NW_WEST;
const uint16 NORTHEAST = NE_NORTH | NE_EAST;
const uint16 SOUTHWEST = SW_SOUTH | SW_WEST;
const uint16 SOUTHEAST = SE_SOUTH | SE_EAST;

const uint16 FACING_NORTH = NORTH | NW_NORTH | NE_NORTH;
const uint16 FACING_SOUTH = SOUTH | SW_SOUTH | SE_SOUTH;
const uint16 FACING_WEST = WEST | NW_WEST | SW_WEST;
const uint16 FACING_EAST = EAST | NE_EAST | SE_EAST;
//@}

/** \name Map Sprite Animation Constants
*** These constants are used to index the MapSprite#animations vector to display the correct
*** animation. The first 8 entries in this vector always represent the same sets of animations
*** for each map sprite.
**/
//@{
const uint32 ANIM_STANDING_SOUTH = 0;
const uint32 ANIM_STANDING_NORTH = 1;
const uint32 ANIM_STANDING_WEST  = 2;
const uint32 ANIM_STANDING_EAST  = 3;
const uint32 ANIM_WALKING_SOUTH  = 4;
const uint32 ANIM_WALKING_NORTH  = 5;
const uint32 ANIM_WALKING_WEST   = 6;
const uint32 ANIM_WALKING_EAST   = 7;
//@}

/** \name Map Sprite Type Constants
*** These constants are used to identify the type a map sprite
**/
//@{
const uint8 PHYSICAL_TYPE = 0;
const uint8 VIRTUAL_TYPE = 1;
const uint8 SPRITE_TYPE = 2;
const uint8 MONSTER_TYPE = 3;
//@}

/** ****************************************************************************
*** \brief Abstract class that represents objects on a map
***
*** A map object can be anything from a sprite to a tree to a house. To state
*** it simply, a map object is a map image that is not tiled and may not be fixed
*** in place. Map objects are drawn in one of three layers: ground, pass, and sky
*** object layers. Every map object has a collision rectangle associated with it.
*** The collision rectangle indicates what parts of the object may not overlap with
*** other collision rectangles.
***
*** \note It is advised not to attempt to make map objects with dynamic sizes (i.e.
*** the various image frames that compose the object are all the same size). In
*** theory, dynamically sized objects are feasible to implement in maps, but they
*** are much more vulnerable to bugs
*** ***************************************************************************/
class MapObject {
public:
	/** \brief An identification number for the object as it is represented in the map file.
	*** Player sprites are assigned object ids from 5000 and above. Technically this means that
	*** a map can have no more than 5000 objects that are not player sprites, but no map should
	*** need to contain that many objects in the first place. Objects with an ID less than zero
	*** are invalid.
	**/
	int16 object_id;

	/** \brief The map context that the object currently resides in.
	*** Context helps to determine where an object "resides". For example, inside of a house or
	*** outside of a house. The context member determines if the object should be drawn or not,
	*** since objects are only drawn if they are in the same context as the map's camera.
	*** Objects can only interact with one another if they both reside in the same context.
	***
	*** \note The default value for this member is -1. A negative context indicates that the
	*** object is invalid and it does not exist anywhere. Objects with a negative context are never
	*** drawn to the screen. A value equal to zero indicates that the object is "always in
	*** context", meaning that the object will be drawn regardless of the current context. An
	*** example of where this is useful is a bridge, which shouldn't simply disappear because the
	*** player walks inside a nearby home.
	**/
	uint32 context;

	/** \brief Coordinates for the object's origin/position.
	*** The origin of every map object is the bottom center point of the object. These
	*** origin coordinates are used to determine where the object is on the map as well
	*** as where the objects collision rectangle lies.
	***
	*** The position coordinates are described by an integer (position) and a float (offset).
	*** The position coordinates point to the map grid tile that the object currently occupies
	*** and may range from 0 to the number of columns or rows of grid tiles on the map. The
	*** offset member will always range from 0.0f and 1.0f to indicate the exact position of
	*** the object within that tile.
	**/
	//@{
	uint16 x_position, y_position;
	float x_offset, y_offset;
	//@}

	/** \brief The half-width and height of the image, in map grid coordinates.
	*** The half_width member is indeed just that: half the width of the object's image. We keep
	*** the half width rather than the full width because the origin of the object is its bottom
	*** center, and it is more convenient to store only half the sprite's width as a result.
	***
	*** \note These members assume that the object retains the same width and height regardless
	*** of the current animation or image being drawn. If the object's image changes size, the
	*** API user must remember to change these values accordingly.
	**/
	float img_half_width, img_height;

	/** \brief Determines the collision rectangle for the object.
	*** The collision area determines what portion of the map object may not be overlapped
	*** by other objects or unwalkable regions of the map. The x and y coordinates are
	*** relative to the origin, so an x value of 0.5f means that the collision rectangle
	*** extends the length of 1/2 of a tile from the origin on both sides, and a y value
	*** of 1.0f means that the collision area exists from the origin to 1 tile's length
	*** above.
	***
	*** \note These members should always be positive. Setting these members to zero does *not*
	*** eliminate collision detection for the object, and therefore they should usually never
	*** be zero.
	**/
	float coll_half_width, coll_height;

	//! \brief When set to false, the Update() function will do nothing (default = true).
	bool updatable;

	//! \brief When set to false, the Draw() function will do nothing (default = true).
	bool visible;

	/** \brief When set to true, the object will not be examined for collision detection (default = false).
	*** Setting this member to true really has two effects. First, the object may exist anywhere on
	*** the map, including where the collision rectangles of other objects are located. Second, the
	*** object is ignored when other objects are performing their collision detection. This property
	*** is useful for virtual objects or objects with an image but no "physical form" (i.e. ghosts
	*** that other sprites may walk through). Note that while this member is set to true, the object's
	*** collision rectangle members are ignored.
	**/
	bool no_collision;

	/** \brief When set to true, objects in the ground object layer will be drawn after the pass objects
	*** \note This member is only checked for objects that exist in the ground layer. It has no meaning
	*** for objects in the pass or sky layers.
	**/
	bool draw_on_second_pass;

	std::string filename;

	// ---------- Methods

	MapObject();

	virtual ~MapObject()
		{}

	/** \brief Loads any data that the object requires.
	*** \return False if there was a failure in loading any of the data.
	**/
	virtual bool Load() = 0;

	/** \brief Updates the state of an object.
	*** Many map objects may not actually have a use for this function. For example, animated objects like a
	*** tree automatically have their frames updated by the video engine, so there is no need to
	*** call this function for it. The function is only called for objects which have the UPDATEABLE bit in
	*** the MapObject#_status member set.
	**/
	virtual void Update() = 0;

	/** \brief Draws the object to the frame buffer.
	*** Objects are drawn differently depending on what type of object they are and what their current
	*** state is. This function is only called for objects that will be visible on the screen when drawn
	*** and have their VISIBLE bit in the MapObject#_status member set.
	**/
	virtual void Draw() = 0;

	/** \brief Assists with the drawing of map objects
	*** \return True if the object should be drawn, or false if it is not visible on the screen.
	***
	*** This method performs the common drawing operations of identifying whether or not the object
	*** is visible on the screen and moving the drawing cursor to its location. The children classes
	*** of this class may choose to make use of it (or not). All that needs to be done after this
	*** method returns true is to draw the object's image on the screen.
	**/
	bool DrawHelper();

	/** \brief Computes the full floating-point location coordinates of the object
	*** \return The full x or y coordinate location of the object
	***
	*** Since an object's position is stored as an integer component and an offset component, this
	*** method simply returns a single floating point value representing the full x and y positions
	*** of the object in a single variable.
	**/
	//@{
	float ComputeXLocation() const
		{ return (static_cast<float>(x_position) + x_offset); }
	float ComputeYLocation() const
		{ return (static_cast<float>(y_position) + y_offset); }
	//@}

	/** \name Lua Access Functions
	*** These functions are specifically written for Lua binding, to enable Lua to access the
	*** members of this class.
	**/
	//@{
	void SetObjectID(int16 id = 0)
		{ object_id = id; }

	void SetContext(uint32 ctxt)
		{ context = ctxt; }

	void SetXPosition(uint16 x, float offset)
		{ x_position = x; x_offset = offset; }

	void SetYPosition(uint16 y, float offset)
		{ y_position = y; y_offset = offset; }

	void SetImgHalfWidth(float width)
		{ img_half_width = width; }

	void SetImgHeight(float height)
		{ img_height = height; }

	void SetCollHalfWidth(float collision)
		{ coll_half_width = collision; }

	void SetCollHeight(float collision)
		{ coll_height = collision; }

	void SetUpdatable(bool update)
		{ updatable = update; }

	void SetVisible(bool vis)
		{ visible = vis; }

	void SetNoCollision(bool coll)
		{ no_collision = coll; }

	void SetDrawOnSecondPass(bool pass)
		{ draw_on_second_pass = pass; }

	int16 GetObjectID() const
		{ return object_id; }

	uint32 GetContext() const
		{ return context; }

	void GetXPosition(uint16 &x, float &offset) const
		{ x = x_position; offset = x_offset; }

	void GetYPosition(uint16 &y, float &offset) const
		{ y = y_position; offset = y_offset; }

	float GetImgHalfWidth() const
		{ return img_half_width; }

	float GetImgHeight() const
		{ return img_height; }

	float GetCollHalfWidth() const
		{ return coll_half_width; }

	float GetCollHeight() const
		{ return coll_height; }

	bool IsUpdatable() const
		{ return updatable; }

	bool IsVisible() const
		{ return visible; }

	bool IsNoCollision() const
		{ return no_collision; }

	bool IsDrawOnSecondPass() const
		{ return draw_on_second_pass; }

	uint8 GetType() const
	{ return _object_type; }
	//@}



protected:
	//! \brief This holds the the type of sprite this is.
	uint8 _object_type;

}; // class MapObject

/** \brief This is a predicate used to sort MapObjects in correct draw order
*** \note A simple < operator cannot be used with the sorting algorithm because it is sorting pointers.
*** \return True if the MapObject pointed by a should be drawn behind MapObject pointed by b
**/
struct MapObject_Ptr_Less
{
	const bool operator()( const MapObject * a, const MapObject * b )
	{
		return ( a->y_position + a->y_offset ) < ( b->y_position + b->y_offset );
	}
};


/** ****************************************************************************
*** \brief Represents visible objects on the map that have no motion.
***
*** This class represents both still image and animated objects. These objects
*** are fixed in place and can not move. The object must have at least one
*** entry in its image vector, otherwise a segmentation fault will occur if the
*** Update or Draw functions are called.
***
*** \note If the object does not have any animated images, set the updatable
*** member of this class to false. Forgetting to do this will do no harm, but
*** it will do an extra function call that it shouldn't need to do.
*** ***************************************************************************/
class PhysicalObject : public MapObject {
public:
	//! \brief The index to the animations vector that contains the current image to display
	uint8 current_animation;

	/** \brief A vector containing all the object's animations.
	*** Note that these need not be actual animations. An AnimatedImage object may consist
	*** of only a single frame. Usually an object will only need a single image or animation,
	*** but a vector is used here in case others are needed.
	**/
	std::vector<hoa_video::AnimatedImage> animations;

	PhysicalObject();

	~PhysicalObject();

	/** \brief Loads the objects animation images
	*** \return False if the images failed to load.
	**/
	bool Load()
		{ return true; }

	//! \brief Updates the object's animation frames if it is animated.
	void Update();

	//! \brief Draws the object to the screen, if it is visible.
	void Draw();

	/** \name Lua Access Functions
	*** These functions are specifically written for Lua binding, to enable Lua to access the
	*** members of this class.
	**/
	//@{
	void AddAnimation(hoa_video::AnimatedImage new_img)
		{ animations.push_back(new_img); }

	void SetCurrentAnimation(uint8 current)
		{ animations[current_animation].SetTimeProgress(0); current_animation = current; }

	void SetAnimationProgress(uint32 progress)
		{ animations[current_animation].SetTimeProgress(progress); }

	uint8 GetCurrentAnimation() const
		{ return current_animation; }
	//@}
}; // class PhysicalObject : public MapObject


/** ****************************************************************************
*** \brief An invisible and possible mobile sprite on a map
***
*** The VirtualSprite is a special type of MapObject because it has no physical
*** form (no image). Virtual sprites may be manipulated to move around on the screen,
*** or they may remain stationary. VirtualSprites do take collision detection into account
*** by default, unless the no_collision member is set to true. Here are some examples of
*** where virtual sprites may be of use:
***
*** - As a mobile focusing point for the map camera
*** - As an impassible map location for ground objects in a specific context only
*** - To set impassible locations for objects in the sky layer
*** ***************************************************************************/
class VirtualSprite : public MapObject {
public:
	/** \brief A bit-mask for the sprite's draw orientation and direction vector.
	*** This member determines both where to move the sprite (8 directions) and
	*** which way the sprite is facing (4 directions). See the Sprite Directions
	*** series of constants for the values that this member may be set to.
	**/
	uint16 direction;

	//! \brief The speed at which the sprite moves around the map.
	float movement_speed;

	/** \brief Set to true when the sprite is currently moving.
	*** \note This does not necessarily mean that the sprite actually is moving, but rather that
	*** the sprite is <i>trying</i> to move in a certain direction.
	**/
	bool moving;

	/** \brief When set to true, indicates that the object exists on the sky object layer (default = false).
	*** This member is necessary for collision detection purposes. When a sprite needs to detect
	*** if it has encountered a collision, that collision must be examined with other objects on
	*** the appropriate layer (the ground or sky layer).
	**/
	bool sky_object;

	//! \brief The name of the sprite, as seen by the player in the game.
	hoa_utils::ustring name;

	//! \brief A pointer to the face portrait of the sprite, as seen in dialogues and menus.
	hoa_video::StillImage* face_portrait;

	/** \brief An index to the actions vector, representing the current sprite action being performed.
	*** A negative value indicates that the sprite is taking no action. If the sprite has no entries
	*** in its actions vector, this member should remain negative, otherwise a segmentation fault
	*** will occur.
	**/
	int8 current_action;

	//TODO: change how forced action work
	int8 forced_action;

	//! \brief A container for all of the actions this sprite performs.
	std::vector<SpriteAction*> actions;

	/** \name Saved state attributes
	*** These attributes are used to save and load the state of a VirtualSprite
	**/
	//@{
	//! \brief This indicates if a state was saved or not.
	bool _saved;
	uint16 _saved_direction;
	float _saved_movement_speed;
	bool _saved_moving;
	hoa_utils::ustring _saved_name;
	int8 _saved_current_action;
	//@}

	//! \brief This vector contains all the dialogues of the sprite
	std::vector< MapDialogue* > dialogues;

	/** \brief An index to the dialogues vector, representing the current sprite dialogue to
	*** display when talked to by the player. A negative value indicates that the sprite has no dialogue.
	*** \note If the sprite has no entries in its dialogues vector, this member should remain negative,
	*** otherwise a segmentation fault will occur.
	**/
	int16 current_dialogue;

	// -------------------- Publice methods
	VirtualSprite();

	~VirtualSprite();

	/** \brief Loads the sprite's face portrait image, if it has one
	*** \return False if the image failed to load.
	**/
	virtual bool Load()
		{ return true; }

	//! \brief Updates the virtual object's position if it is moving, otherwise does nothing.
	virtual void Update();

	//! \brief Does nothing since a virtual object has nothing to draw.
	virtual void Draw()
		{ return; }

	/** \name Lua Access Functions
	*** These functions are specifically written for Lua binding, to enable Lua to access the
	*** members of this class.
	**/
	//@{
	/** \note This method takes into account the current direction when setting the new direction
	*** in the case of diagonal movement. For example, if the sprite is currently facing north
	*** and this function indicates that the sprite should move northwest, it will face north
	*** during the northwest movement.
	**/
	void SetDirection(uint16 dir);

	void SetMovementSpeed(float speed)
		{ movement_speed = speed; }

	uint16 GetDirection() const
		{ return direction; }

	float GetMovementSpeed() const
		{ return movement_speed; }

	void SetFacePortrait(std::string pn);

	//@}

	/** \brief This method will save the state of a sprite.
	*** Attributes saved: direction, speed, moving state, name, current action.
	**/
	virtual void SaveState();

	/** \brief This method will load the saved state of a sprite.
	*** Attributes loaded: direction, speed, moving state, name, current action.
	*** \return false if there was no saved state, true otherwise.
	**/
	virtual bool LoadState();


	/** \name Dialogue control methods
	*** These methods are used to add and control which dialogue should the sprite speak.
	**/
	//@{
	void AddAction(SpriteAction* act)
		{ act->SetSprite(this); actions.push_back(act); }

	void AddDialogue(MapDialogue* md)
		{ dialogues.push_back(md); }

	bool HasDialogue() const
		{ return dialogues.size() > 0; }

	MapDialogue* GetCurrentDialogue() const
		{ return dialogues[current_dialogue]; }

	void SetDialogue(const int16 dialogue)
		{ current_dialogue = dialogue; }

	int16 GetNumDialogues() const
		{ return dialogues.size(); }
	//@}

	/** \brief This static class function returns the opposite direction of the
	*** direction given in parameter.
	*** \note This is mostly used as an helper function to make sprites face each other.
	**/
	static uint16 CalculateOppositeDirection( const uint16 direction );
}; // class VirtualMapObject : public MapObject


/** ****************************************************************************
*** \brief A mobile map object with which the player can interact with.
***
*** Map sprites are animate, mobile, living map objects. Although there is
*** but this single class to represent all the map sprites in the game, they can
*** divided into types such as NPCs, friendly creatures, and enemies. The fact
*** that there is only one class for representing several sprite types is the
*** reason why many of the class members are pointers. For example, we don't
*** need dialogue for a dog sprite.
*** ***************************************************************************/
class MapSprite : public VirtualSprite {
public:

	//! \brief Holds the previous value of VirtualSprite#moving from the last call to MapSprite#Update().
	bool was_moving;

	/** \brief The sound that will play when the sprite walks.
	*** This member references the MapMode#_map_sounds vector as the sound to play. If this member
	*** is less than zero, no sound is played when the object is walking.
	**/
	int8 walk_sound;

	//! \brief The index to the animations vector containing the current sprite image to display
	uint8 current_animation;

	/** \brief A vector containing all the sprite's various animations.
	*** The first four entries in this vector are the walking animation frames.
	*** They are ordered from index 0 to 3 as: down, up, left, right. Additional
	*** animations may follow.
	**/
	std::vector<hoa_video::AnimatedImage> animations;

	/** \name Saved state attributes
	*** These attributes are used to save and load the state of a VirtualSprite
	**/
	//@{
	bool _saved_was_moving;
	int8 _saved_walk_sound;
	uint8 _saved_current_animation;
	//@}


	// -------------------------------- Methods --------------------------------

	MapSprite();

	~MapSprite();

	/** \brief Fills up the animations vector and loads the sprite image frames.
	*** \return False if there was a problem loading the sprite.
	**/
	virtual bool Load();

	//! \brief Updates the sprite's position and state.
	virtual void Update();

	//! \brief Draws the sprite frame in the appropriate position on the screen, if it is visible.
	virtual void Draw();

	/** \name Lua Access Functions
	*** These functions are specifically written for Lua binding, to enable Lua to access the
	*** members of this class.
	**/
	//@{
	void SetName(std::string na)
		{ name = hoa_utils::MakeUnicodeString(na); }

	void SetWalkSound(int8 sound)
		{ walk_sound = sound; }

	void SetCurrentAnimation(uint8 anim)
		{ current_animation = anim; }

	int8 GetWalkSound() const
		{ return walk_sound; }

	uint8 GetCurrentAnimation() const
		{ return current_animation; }
	//@}

	/** \brief This method will save the state of a sprite.
	*** Attributes saved: direction, speed, moving state, name, current action,
	*** current animation, current walk sound.
	**/
	virtual void SaveState();

	/** \brief This method will load the saved state of a sprite.
	*** Attributes loaded: direction, speed, moving state, name, current action,
	*** current animation, current walk sound.
	*** \return false if there was no saved state, true otherwise.
	**/
	virtual bool LoadState();

}; // class MapSprite : public VirtualSprite


/** ****************************************************************************
*** \brief A mobile map object with which the player can get in a fight.
***
*** Monster sprites are attached to a MonsterZone, where they will respawn when
*** dead. A monster sprite can be in one of 3 states: SPAWNING, HOSTILE or DEAD.
*** In spawning state, the monster becomes gradually visible, is immobile and
*** cannot be attacked. In hostile state, the monsters roams the map and will
*** attack if touched by the player. In dead state, the monsters is invisible 
*** and waits for the MonsterZone to reset it in an other position, back in 
*** spawning state.
*** ***************************************************************************/
class MonsterSprite : public MapSprite
{
private:
	enum State
	{
		SPAWNING,
		HOSTILE,
		DEAD
	};
public:
	MonsterSprite( std::string file )
		: _color( 1.0f,1.0f,1.0f,0.0f ),
		 _aggro_range(8.0f),
		 _time_dir_change(2500),
		 _time_to_spawn(3500),
		 _zone(0)
	{
		filename = file;
		MapObject::_object_type = MONSTER_TYPE;
		moving = true;
		Reset();
	}

	virtual bool Load();

	//! \brief Updates the sprite's position and state.
	virtual void Update();

	//! \brief Draws the sprite frame in the appropriate position on the screen, if it is visible.
	virtual void Draw();

	void SetZone( MonsterZone* zone )
		{ _zone = zone; }

	void Reset() {
		updatable = false; 
		no_collision = true;
		_state = DEAD; 
		_time_elapsed = 0; 
		_color.SetAlpha( 0.0f );
	}

	void SetAggroRange( float range )
		{ _aggro_range = range; }

	float GetAggroRange()
		{ return _aggro_range; }

	void SetTimeToChange( uint32 time )
		{ _time_dir_change = time; }

	uint32 GetTimeToChange()
		{ return _time_dir_change; }

	void SetTimeToSpawn( uint32 time )
		{ _time_to_spawn = time; }

	uint32 GetTimeToSpawn()
		{ return _time_to_spawn; }

	void ChangeStateDead()
		{ Reset(); _zone->MonsterDead(); }
	
	void ChangeStateSpawning()
		{ updatable = true; _state = SPAWNING; no_collision = false; } 
	
	void ChangeStateHostile()
		{ updatable = true; _state = HOSTILE;}

	bool IsDead()
		{ return _state == DEAD; }
	
	bool IsSpawning()
		{ return _state == SPAWNING; }
	
	bool IsHostile()
		{ return _state == HOSTILE; }

private:
	private_map::MonsterZone* _zone;
	hoa_video::Color _color;
	uint32 _time_elapsed; 
	State _state; 
	
	float _aggro_range;
	uint32 _time_dir_change;
	uint32 _time_to_spawn;
};

} // namespace private_map

} // namespace hoa_map

#endif
