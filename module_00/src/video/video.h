///////////////////////////////////////////////////////////////////////////////
//            Copyright (C) 2005 by The Allacrost Project
//                       All Rights Reserved
//
// This code is licensed under the GNU GPL. It is free software and you may
// modify it and/or redistribute it under the terms of this license.
// See http://www.gnu.org/copyleft/gpl.html for details.
/////////////////////////////////////////////////////////////////////////////// 

/*!****************************************************************************
 * \file    video.h
 * \author  Raj Sharma, rajx30@gmail.com
 * \date    Last Updated: August 23rd, 2005
 * \brief   Header file for video engine interface.
 *
 * This code provides an easy-to-use API for managing all video and GUI stuff.
 *
 * \note This code uses the SDL_ttf 2.0 extension library. The documentation
 *       for it may be found here: http://jcatki.no-ip.org/SDL_ttf
 *
 * \note This code uses the DevIL library, v.1.6.7. This library can be found
 *       at http://openil.sourceforge.net
 *
 * \note Full documentation for video engine can be found at:
 *       http://www.allacrost.org/staff/user/roos/video.html
 *****************************************************************************/ 

#ifndef _VIDEO_HEADER_
#define _VIDEO_HEADER_

#include "defs.h"
#include "utils.h"

// SDL includes
#include <SDL/SDL_ttf.h>

// OpenGL includes
#include <GL/gl.h>
#include <GL/glu.h>

// DevIL includes
#define ILUT_USE_OPENGL
#include <IL/il.h>
#include <IL/ilu.h>
#include <IL/ilut.h>

 
//! All calls to the video engine are wrapped in this namespace.
namespace hoa_video 
{

//! The singleton pointer responsible for all video operations.
extern GameVideo *VideoManager;
//! Determines whether the code in the hoa_video namespace should print
extern bool VIDEO_DEBUG;

/*!
 *  \brief Linearly interpolates between initial and final value.
 *
 *  \param alpha   From 0.0 to 1.0, how much to interpolate between initial and final
 *  \param initial Initial value
 *  \param final   Final value
 */
 
float _Lerp(float alpha, float initial, float final);


/*!
 *  \brief Creates a random float between a and b.
 */

float RandomFloat(float a, float b);

//! \name Constants
//@{
//! \brief PI and multiples of PI. Used in various math calculations such as interpolations
const float VIDEO_QUARTER_PI = 0.785398163f;
const float VIDEO_HALF_PI    = 1.570796326f;
const float VIDEO_PI         = 3.141592653f;
const float VIDEO_2PI        = 6.283185307f;
//@}

//! animation frame period: how many milliseconds 1 frame of animation lasts for
const int32 VIDEO_ANIMATION_FRAME_PERIOD = 10;

//! Draw flags to control x,y alignment, flipping, and blending
enum
{
	VIDEO_X_LEFT = 1,
	VIDEO_X_CENTER,
	VIDEO_X_RIGHT,
	VIDEO_Y_TOP,
	VIDEO_Y_CENTER,
	VIDEO_Y_BOTTOM,
	VIDEO_X_FLIP,
	VIDEO_X_NOFLIP,
	VIDEO_Y_FLIP,
	VIDEO_Y_NOFLIP,
	VIDEO_NO_BLEND,
	VIDEO_BLEND,
	VIDEO_BLEND_ADD
};


/*!***************************************************************************
 *  \brief An enumeration of shake falloff modes.which controls how quickly the
 *         shaking dies down when you do a screen shake.
 *****************************************************************************/

enum ShakeFalloff
{
	VIDEO_FALLOFF_INVALID = -1,
	
	VIDEO_FALLOFF_NONE,     //! shake remains at constant force
	VIDEO_FALLOFF_EASE,     //! shake starts out small, builds up, then dies down
	VIDEO_FALLOFF_LINEAR,   //! shake strength decreases linear til the end
	VIDEO_FALLOFF_GRADUAL,  //! shake decreases slowly and drops off at the end
	VIDEO_FALLOFF_SUDDEN,   //! shake suddenly falls off, for "impacts" like meteors
	
	VIDEO_FALLOFF_TOTAL
};

/*!***************************************************************************
 *  \brief Interpolation metods are various ways to create smoothed values
 *         between two numbers, e.g. linear interpolation
 *****************************************************************************/

enum InterpolationMethod
{
	VIDEO_INTERPOLATE_INVALID = -1,
	
	VIDEO_INTERPOLATE_EASE,   //! rise from A to B and then down to A again
	VIDEO_INTERPOLATE_SRCA,   //! constant value of A
	VIDEO_INTERPOLATE_SRCB,   //! constant value of B
	VIDEO_INTERPOLATE_FAST,   //! rises quickly at the beginning and levels out
	VIDEO_INTERPOLATE_SLOW,   //! rises slowly at the beginning then shoots up
	VIDEO_INTERPOLATE_LINEAR, //! simple linear interpolation between A and B
	
	VIDEO_INTERPOLATE_TOTAL
};


/*!***************************************************************************
 *  \brief Styles for text shadows
 *****************************************************************************/

enum TextShadowStyle
{
	VIDEO_TEXT_SHADOW_INVALID = -1,
	
	VIDEO_TEXT_SHADOW_NONE,       //! no text shadow, even if shadows are enabled
	VIDEO_TEXT_SHADOW_DARK,       //! shadowed area is darkened (default)
	VIDEO_TEXT_SHADOW_LIGHT,      //! shadowed area is lightened
	VIDEO_TEXT_SHADOW_BLACK,      //! shadowed area is completely black
	VIDEO_TEXT_SHADOW_COLOR,      //! shadowed area is the color of the text, but less alpha
	VIDEO_TEXT_SHADOW_INVCOLOR,   //! shadowed area is the inverse of the text's color (e.g. white text, black shadow)
	
	VIDEO_TEXT_SHADOW_TOTAL
	
};




// forward declarations
class StaticImage;
class GameVideo;
class Color;


/*!***************************************************************************
 *  \brief class for representing colors, with common operations like
 *         multiplying, adding, etc.
 *****************************************************************************/

class Color
{
public:

	// default colors
	
	static Color clear;
	static Color white;
	static Color gray;
	static Color black;
	static Color red;
	static Color orange;
	static Color yellow;
	static Color green;
	static Color aqua;
	static Color blue;
	static Color violet;
	static Color brown;

	float color[4];
	
	bool operator == (const Color &c) const
	{
		return color[0] == c.color[0] &&
		       color[1] == c.color[1] &&
		       color[2] == c.color[2] &&
		       color[3] == c.color[3];
	}

	Color operator + (const Color &c) const
	{
		Color color = Color(color[0] + c.color[0],
		                    color[1] + c.color[1],
		                    color[2] + c.color[2],
		                    color[3] + c.color[3]);
		                    
		if(color[0] > 1.0f) color[0] = 0.0f;
		if(color[1] > 1.0f) color[1] = 0.0f;
		if(color[2] > 1.0f) color[2] = 0.0f;
		if(color[3] > 1.0f) color[3] = 0.0f;
		return color;
	}

	Color operator *= (const Color &c)
	{
		return Color(color[0] * c.color[0],
		             color[1] * c.color[1],
		             color[2] * c.color[2],
		             color[3] * c.color[3]);
	}


	Color operator * (const Color &c) const
	{
		return Color(color[0] * c.color[0],
		             color[1] * c.color[1],
		             color[2] * c.color[2],
		             color[3] * c.color[3]);
	}
	
	Color operator * (float f) const
	{
		return Color(color[0] * f,
		             color[1] * f,
		             color[2] * f,
		             color[3]);
	}

	Color()
	{
		color[0]=0.0f;
		color[1]=0.0f;
		color[2]=0.0f;
		color[3]=1.0f;		
	}
	
	Color(float r, float g, float b, float a)
	{
		color[0]=r;
		color[1]=g;
		color[2]=b;
		color[3]=a;
	}

	float &operator[](int32 i)
	{
		// no bounds check for efficiency!
		return color[i];
	}

	const float &operator[](int32 i) const
	{
		// no bounds check for efficiency!
		return color[i];
	}
};


/*!***************************************************************************
 *  \brief the CoordSys structure holds a "coordinate system", which is
 *         defined by rectangle (left, right, bottom, and top) which determines
 *         how coordinates are mapped to the screen. The default coordinate
 *         system is (0,1024,0,768). As another example, if you wanted to make
 *         it so that the screen coordinates ranged from 0 to 1, you could
 *         set the coordinate system to (0,1,0,1)
 *****************************************************************************/

class CoordSys
{
public:

	CoordSys() {}

	CoordSys(float left, float right, float bottom, float top)
	{
		_left   = left;
		_right  = right;
		_bottom = bottom;
		_top    = top;
		
		if(_right > _left)
			_rightDir = 1.0f;
		else
			_rightDir = -1.0f;
			
		if(_top > _bottom)
			_upDir = 1.0f;
		else
			_upDir = -1.0f;
	}
	
private:

	float _upDir;     //! this is 1.0f if increasing y coordinates are up, otherwise it's -1.0f.
	float _rightDir;  //! this is 1.0f if increasing x coordinates are right, otherwise -1.0f. Pretty much any SANE coordinate system has a rightVec of 1.0f

	float _left;
	float _right;
	float _bottom;
	float _top;

	friend class GUIElement;
	friend class TextBox;
	friend class OptionBox;
	friend class GameVideo;
};


/*!***************************************************************************
 *  \brief this structure holds properties about fonts
 *****************************************************************************/

class FontProperties
{
public:
	int32 height;   //! maximum height of all glyphs
	int32 lineskip; //! SDL_ttf's recommended amount of spacing between lines
	int32 ascent;   //! height above baseline of font
	int32 descent;  //! height below baseline of font
	int32 shadowX;  //! x offset of text shadow
	int32 shadowY;  //! y offset of text shadow
	
	TextShadowStyle shadowStyle;  //! style of text shadow
};


/*!***************************************************************************
 *  \brief a rectangle structure, used in storing the current scissoring
 *         or viewport rectangles, etc. It is based on standard screen coordinates,
 *         where (0,0) is the top-left and the unit is 1 pixel (hence int32 coordinates)
 *****************************************************************************/

class ScreenRect
{
public:	
	ScreenRect() {}
	ScreenRect(int32 l, int32 t, int32 w, int32 h)
	: left(l), top(t), width(w), height(h)
	{
	}
	

	/*!
	 *  \brief intersects this rectangle with the one passed in, and modifies this
	 *         rect to be the intersection of the two. (The intersection of two
	 *         rectangles is of course, another smaller rectangle). Note that
	 *         if the two rectangles don't intersect at all, then a "zero rectangle"
	 *         results where left, top, width, and height are all zero.
	 */	
	
	void Intersect(const ScreenRect &rect);	
	
	int32 left, top, width, height;
};


//! private_video namespace, hides things which are used internally
namespace private_video
{

class GUI;
class TexSheet;
class TexMemMgr;
class FixedTexMemMgr;
class VariableTexMemMgr;

class Image;

/*!***************************************************************************
 *  \brief Represents the different image sizes that a texture sheet can hold
 *****************************************************************************/

enum TexSheetType
{
	VIDEO_TEXSHEET_INVALID = -1,
	
	VIDEO_TEXSHEET_32x32,
	VIDEO_TEXSHEET_32x64,
	VIDEO_TEXSHEET_64x64,
	VIDEO_TEXSHEET_ANY,
	
	VIDEO_TEXSHEET_TOTAL
};


/*!***************************************************************************
 *  \brief Represents a single image within an image descriptor. Compound
 *         images are formed of multiple ImageElements.
 *****************************************************************************/

class ImageElement
{
public:
	ImageElement
	(
		Image *image_, 
		float xOffset_, 
		float yOffset_, 
		float width_, 
		float height_,
		Color color_[4]
	)
	{
		image    = image_;
		xOffset  = xOffset_;
		yOffset  = yOffset_;
		width    = width_;
		height   = height_;
		
		color[0] = color_[0];
		
		if(color_[1] == color[0] &&
		   color_[2] == color[0] &&
		   color_[3] == color[0])
		{
			// if all colors are the same then mark it so we don't have to process all vertex colors
			oneColor = true;

			if(color[0] == Color::white)
			{			
				// if all vertex colors are white, just set a flag so they don't have to
				// be processed
				white = true;
				blend = false;
			}
			else
			{
				// blend if alpha < 1
				blend = (color[0][3] < 1.0f);
			}
		}
		else
		{	
			color[0] = color_[0];
			color[1] = color_[1];
			color[2] = color_[2];
			color[3] = color_[3];
			
			blend = (color[0][3] < 1.0f || color[1][3] < 1.0f || color[2][3] < 1.0f || color[3][3] < 1.0f);			
		}
	}
	
	
	ImageElement
	(
		Image *image_, 
		float xOffset_, 
		float yOffset_, 
		float width_, 
		float height_
	)
	{
		image    = image_;
		xOffset  = xOffset_;
		yOffset  = yOffset_;
		width    = width_;
		height   = height_;
		white    = true;
		oneColor = true;
		blend    = false;
		color[0] = Color::white;
	}
	
	bool blend;
	bool oneColor;
	bool white;	
	Image * image;

	float xOffset;
	float yOffset;
	
	float width;
	float height;
	
	Color color[4];
	
	friend class AnimatedImage;
};


/*!***************************************************************************
 *  \brief represents a single image. Internally it's a reference to a
 *         sub-rect within a texture sheet.
 *****************************************************************************/

class Image
{
public:
	Image
	(
		const std::string &fname,
		int32 w, 
		int32 h
	) 
	: filename(fname)
	{ 
		width    = w;
		height   = h;
		refCount = 0;
	}

	Image
	(
		TexSheet *sheet,
		const std::string &fname,
		int32 x_,
		int32 y_,
		int32 w, 
		int32 h,
		float u1_,
		float v1_,
		float u2_,
		float v2_
	) 
	: filename(fname)
	{
		texSheet = sheet;
		x = x_;
		y = y_;
		width = w;
		height = h;
		u1 = u1_;
		u2 = u2_;
		v1 = v1_;
		v2 = v2_;
		refCount = 0;
	}



	TexSheet *texSheet;     //! texture sheet using this image
	std::string filename;   //! stored for every image in case it needs to be reloaded
	
	int32 x, y;             //! location of image within the sheet
	int32 width, height;    //! width and height, in pixels

	float u1, v1;           //! also store the actual uv coords. This is a bit
	float u2, v2;           //! redundant, but saves floating point calculations
	
	int32 refCount;         //! keep track of when this image can be deleted
};


/*!***************************************************************************
 *  \brief An actual OpenGL texture which can be used for storing multiple
 *         smaller images in it, to save on texture switches.
 *
 *  \note  This is called TexSheet instead of Texture, so it's clear that
 *         this doesn't represent an image that you would draw on the screen,
 *         but simply a "container" for smaller images.
 *****************************************************************************/

class TexSheet
{
public:

	TexSheet(int32 w, int32 h, GLuint texID_, TexSheetType type_, bool isStatic_);
	~TexSheet();

	bool AddImage                   //! adds new image to the tex sheet
	(
		Image *img
	);
	
	bool SaveImage(Image *img);
	
	bool CopyRect(int32 x, int32 y, int32 w, int32 h);
	
	bool RemoveImage (Image *img);  //! removes an image completely
	bool FreeImage   (Image *img);  //! marks the image as free
	bool RestoreImage (Image *img); //! marks a previously freed image as "used"
	
	bool Unload();  //! unloads texture memory used by this sheet
	bool Reload();  //! reloads all the images into the sheet

	int32 width;
	int32 height;

	bool isStatic;       //! if true, images in this sheet that are unlikely to change
	TexSheetType type;   //! does it hold 32x32, 32x64, 64x64, or any kind

	TexMemMgr *texMemManager;  //! manages which areas of the texture are free

	GLuint texID;     //! number OpenGL uses to refer to this texture
	bool loaded;		
};


/*!***************************************************************************
 *  \brief base class for texture memory managers. It is used by TextureSheet
 *         to manage which areas of the texture are available and which are used.
 *****************************************************************************/

class TexMemMgr
{
public:
	
	virtual ~TexMemMgr() {}

	virtual bool Insert  (Image *img)=0;
	virtual bool Remove  (Image *img)=0;
	virtual bool Free    (Image *img)=0;
	virtual bool Restore (Image *img)=0;
	
};


/*!***************************************************************************
 *  \brief used by the fixed-size texture manager to keep track of which blocks
 *         are owned by which images.
 *
 *  \note  The list is doubly linked to allow for O(1) removal
 *****************************************************************************/

class FixedImageNode
{
public:
	Image          *image;
	FixedImageNode *next;
	FixedImageNode *prev;
	
	int32 blockIndex;
};


/*!***************************************************************************
 *  \brief used to manage textures which are designated for fixed image sizes.
 *         For example, a 512x512 sheet that only holds 32x32 tiles.
 *
 *  \note  The texture sheet's size must be divisible by the size of the images
 *         that it holds. For example, you can't create a 256x256 sheet which
 *         holds tiles which are 17x93.
 *****************************************************************************/

class FixedTexMemMgr : public TexMemMgr
{
public:
	FixedTexMemMgr(TexSheet *texSheet, int32 imgW, int32 imgH);
	~FixedTexMemMgr();
	
	bool Insert  (Image *img);
	bool Remove  (Image *img);
	bool Free    (Image *img);
	bool Restore (Image *img);

private:

	int32 CalculateBlockIndex(Image *img);
	void DeleteNode(int32 blockIndex);
	
	//! store dimensions of both the texture sheet, and the images that
	//! it contains
	
	//! NOTE: the sheet dimensions are not in pixels, but images.
	//!       So, a 512x512 sheet holding 32x32 images would be 16x16
	
	int32 _sheetWidth;
	int32 _sheetHeight;
	int32 _imageWidth;
	int32 _imageHeight;
	
	TexSheet *_texSheet;
	
	//! The open list keeps track of which blocks of memory are
	//! open. Note that we track blocks with BOTH an array and a list.
	//! Although it takes up more memory, this makes ALL operations
	//! dealing with the blocklist O(1) so performance is awesome.
	//! Memory isn't too bad either, since blocklist is fairly small.
	//! The tail pointer is also kept so that we can add newly
	//! freed blocks to the end of the list- that way, essentially
	//! blocks that are freed are given a little bit of time from the
	//! time they're freed to the time they're removed, in case they
	//! are loaded again in the near future
	
	FixedImageNode *_openListHead;
	FixedImageNode *_openListTail;
	
	//! this is our actual array of blocks which is indexed like a 2D
	//! array. For example, blocks[x+y*width]->image would tell us
	//! which image is currently allocated at spot (x,y)
	
	FixedImageNode *_blocks;
};


/*!***************************************************************************
 *  \brief this is how we keep track of which images are used/freed in the
 *         variable texture mem manager
 *****************************************************************************/

class VariableImageNode
{
public:
	VariableImageNode()
	{
		image = NULL;
		free  = true;
	}

	Image *image;
	bool   free;
};


/*!***************************************************************************
 *  \brief class that lets you set up various kinds of interpolations.
 *         The basic way to use it is to set the interpolation method using
 *         SetMethod(), then call Start() with the values you want to
 *         interpolate between and the time to do it in.
 *****************************************************************************/
 
class Interpolator
{
public:

	Interpolator();

	//! call to begin an interpolation
	bool Start(float a, float b, int32 milliseconds);

	//! set the method of the interpolator. If you don't call it, the default
	//! is VIDEO_INTERPOLATION_LINEAR
	bool  SetMethod(InterpolationMethod method);
	
	float GetValue();              //! get the current value
	bool  Update(int32 frameTime);   //! call this every frame
	bool  IsFinished();            //! returns true if interpolation is done

private:

	float FastTransform(float t);
	float SlowTransform(float t);
	float EaseTransform(float t);

	bool ValidMethod();
	
	InterpolationMethod _method;
	
	float _a, _b;
	int32   _currentTime;
	int32   _endTime;
	bool  _finished;
	float _currentValue;
};


/*!***************************************************************************
 *  \brief every time ShakeScreen() is called, a new ShakeForce is created
 *         to represent the force of that particular shake
 *****************************************************************************/

class ShakeForce
{
public:
	float initialForce;  //! initial force of the shake
	
	
	Interpolator interpolator;
	int32   currentTime;   //! milliseconds that passed since this shake started
	int32   endTime;       //! milliseconds that this shake was set to last for
};


/*!***************************************************************************
 *  \brief class for fading the screen. Keeps track of current color and
 *         figures out whether we should implement the fade using modulation
 *         or an overlay.
 *****************************************************************************/

class ScreenFader
{
public:
	
	ScreenFader()
	: _currentColor(0.0f, 0.0f, 0.0f, 0.0f),
	  _isFading(false)
	{
		_currentTime = _endTime = 0;
		_fadeModulation = 1.0f;
		_useFadeOverlay = false;
	}
	
	bool FadeTo(const Color &final, float numSeconds);
	bool Update(int32 t);

	//! fades are either implemented with overlays or with modulation, depending
	//! on whether it's a simple fade to black or a fade to a different color.
	//! Based on that, these functions tell what overlay and modulation factors
	//! to use.
	bool  ShouldUseFadeOverlay() { return _useFadeOverlay;   }
	Color GetFadeOverlayColor()  { return _fadeOverlayColor; }
	float GetFadeModulation()    { return _fadeModulation;   }

	bool  IsFading() { return _isFading; }

private:
	
	
	Color _currentColor;  //! color the screen is currently faded to	
	Color _initialColor;  //! color we started from
	Color _finalColor;    //! color we are fading to
	int32   _currentTime;   //! milliseconds that passed since this fade started
	int32   _endTime;       //! milliseconds that this fade was set to last for
	bool  _isFading;      //! true if we're in the middle of a fade
	
	bool  _useFadeOverlay;
	Color _fadeOverlayColor;
	float _fadeModulation;
};


/*!***************************************************************************
 *  \brief used to manage a texture sheet where the size of the images it
 *         will contain are unknown
 *
 *  \note  For the sake of reducing the time it takes to allocate an image,
 *         this class rounds image dimensions up to powers of 16. So although
 *         it's fine to add any-sized images to a variable texture sheet,
 *         some space will be wasted due to rounding.
 *****************************************************************************/

class VariableTexMemMgr : public TexMemMgr
{
public:
	
	VariableTexMemMgr(TexSheet *sheet);
	~VariableTexMemMgr();

	bool Insert  (Image *img);
	bool Remove  (Image *img);
	bool Free    (Image *img);
	bool Restore (Image *img);

private:

	bool SetBlockProperties
	(
		Image *img, 
		bool changeFree, 
		bool changeImage, 
		bool free, 
		Image *newImage
	);

	TexSheet *_texSheet;
	VariableImageNode *_blocks;
	
	//! Sheet's dimensions
	//! NOTE: these aren't in pixels, but in "blocks" of 16x16. So,
	//!       a 512x512 sheet would be 32x32 in blocks
	
	int32 _sheetWidth;
	int32 _sheetHeight;
};


/*!***************************************************************************
 *  \brief Represents the graphics "context", i.e. draw flags, transformation
 *         and coord sys. Must be pushed and popped by any GameVideo functions
 *         which modify any of the context
 *
 *         Note: transformation is actually not a part of this struct since
 *               it is handled separately by the OpenGL transformation stack
 *****************************************************************************/

class Context
{
public:
	char blend, xalign, yalign, xflip, yflip;
	CoordSys coordSys;
	std::string currentFont;
	Color currentTextColor;
	ScreenRect viewport;
	ScreenRect scissorRect;
	bool scissorEnabled;
};


} // namespace private_video


/*!***************************************************************************
 *  \brief base class for StaticImage and AnimatedImage
 *****************************************************************************/

class ImageDescriptor
{
public:

	virtual ~ImageDescriptor() {}

	void SetStatic(bool isStatic);

	virtual float GetWidth() const = 0;
	virtual float GetHeight() const = 0;

protected:

	ImageDescriptor() {}  // private constructor, this class is only meant to be used as a base
	bool _animated; 
	
	friend class GameVideo;
};


/*!***************************************************************************
 *  \brief this represents an image, used internally ONLY
 *
 *  \note  StaticImages may be simple images or compound images. Compound
 *         images are when you stitch together multiple small images to create
 *         a large image, e.g. with TilesToObject(). Externally though,
 *         it's fine to think of compound images as just a single image.
 *****************************************************************************/

class StaticImage : public ImageDescriptor
{
public:

	StaticImage() 
	{
		Clear();
		_animated = false;
	}
	
	//! AddImage allows you to create compound images. You start with a 
	//! newly created StaticImage, then call AddImage(), passing in
	//! all the images you want to add, along with the x, y offsets they
	//! should be positioned at.
	
	bool AddImage(const StaticImage &id, float xOffset, float yOffset);
	
	void Clear()
	{
		_filename.clear();
		_isStatic = false;
		_width = _height = 0.0f;
		_color[0] = _color[1] = _color[2] = _color[3] = Color::white;
		_elements.clear();
	}

	void SetFilename     (const std::string &filename) { _filename = filename; }

	void SetColor        (const Color &color)
	{
		_color[0] = _color[1] = _color[2] = _color[3] = color;
	}
	
	void SetVertexColors (const Color &tl, const Color &tr, const Color &bl, const Color &br)
	{
		_color[0] = tl;
		_color[1] = tr;
		_color[2] = bl;
		_color[3] = br;
	}
	
	void SetDimensions   (float width, float height) {	_width  = width;  _height = height; }
	
	void SetWidth        (float width)    { _width = width; }	
	void SetHeight       (float height)   {	_height = height; }
	
	void SetStatic       (bool isStatic)  { _isStatic = isStatic; }
		
	float GetWidth() const  { return _width; }
	float GetHeight() const { return _height; }
	void  GetVertexColor(Color &c, int colorIndex) { c = _color[colorIndex]; }
			
private:

	std::string _filename;  //! used only as a parameter to LoadImage.

	//! an image descriptor represents a compound image, which is made
	//! up of multiple elements
	std::vector <private_video::ImageElement> _elements;


	Color _color[4];      //! used only as a parameter to LoadImage. Holds the color of the upper left, upper right, lower left, and lower right vertices respectively

	bool  _isStatic;      //! used only as a parameter to LoadImage. This tells
	                      //! whether the image being loaded is to be loaded
	                      //! into a non-volatile area of texture memory
	                      
	float _width, _height;  //! width and height of image, in pixels.
	                         //! If the StaticImage is a compound, i.e. it
	                         //! contains multiple images, then the width and height
	                         //! refer to the entire compound           

	friend class GameVideo;
	friend class AnimatedImage;
};	



/*!***************************************************************************
 *  \brief this represents an animated image.
 *****************************************************************************/

class AnimationFrame
{
public:
	
	int32     _frame_time;  // how long to display this frame (relative to VIDEO_ANIMTION_FRAME_PERIOD)
	StaticImage _image;
};

class AnimatedImage : public ImageDescriptor
{
public:
	AnimatedImage() { _animated = true; _frame_index = _frame_counter = 0; }
	

	/*!
	 *  \brief call every frame to update the animation's current frame. This will automatically
	 *         synchronize the animation to VIDEO_ANIMATION_FRAME_PERIOD, i.e. 30 frames per second.
	 *         If you want to update the frames yourself using some custom method, then use
	 *         SetFrame()
	 */
	void Update();
	

	/*!
	 *  \brief call to set the current frame index of the animation. This is useful for character
	 *         animation for example, where the current frame depends on how far the character is
	 *         from one tile to the next tile.
	 *
	 *  \param frame_index an index from 0 to numFrames - 1
	 */	
	void SetFrameIndex(int32 frame_index);
	
	
	/*!
	 *  \brief returns the number of frames in this animation
	 */
	int32 GetNumFrames() const { return static_cast<int32>(_frames.size()); }


	/*!
	 *  \brief returns an index to the current animation
	 */
	int32 GetCurFrameIndex() const { return _frame_index; }

	
	/*!
	 *  \brief adds a frame using the filename of the image. This is the most convenient
	 *         way to add frames, BUT this makes it impossible to control the image properties,
	 *         such as vertex colors, and size (width and height). If you do it this way,
	 *         the width and height will be the pixel width/height of the image itself. This isn't
	 *         always what you want- for example if your coordinate system is in terms of tiles,
	 *         then a 32x32 tile would have a width and height of 1, not 32.
	 *
	 *  \param frame filename of the frame to add
	 *
	 *  \param frame_time  how many animation periods this frame lasts for. For example, if
	 *                     VIDEO_ANIMATION_FRAME_PERIOD is 30, and frame_count is 3, then this
	 *                     frame will last for 90 milliseconds.
	 */	
	bool AddFrame(const std::string &frame, int frame_time);


	/*!
	 *  \brief adds a frame by passing in a static image.
	 *
	 *  \param frame a static image to use as the frame
	 *
	 *  \param frame_time  how many animation periods this frame lasts for. For example, if
	 *                     VIDEO_ANIMATION_FRAME_PERIOD is 30, and frame_count is 3, then this
	 *                     frame will last for 90 milliseconds.
	 */	
	bool AddFrame(const StaticImage &frame, int frame_time);


	/*!
	 *  \brief returns the width of the 1st frame of animation. You should try to make all your
	 *         frames the same size, otherwise the "width" of an animation doesn't really have any
	 *         meaning.
	 */
	float GetWidth() const;


	/*!
	 *  \brief returns the height of the 1st frame of animation. You should try to make all your
	 *         frames the same size, otherwise the "height" of an animation doesn't really have any
	 *         meaning.
	 */
	float GetHeight() const;

	
	/*!
	 *  \brief returns a pointer to the indexth frame. For the most part, this is a function you
	 *         should never be messing with, but this function is available in case you need it.
	 */
	StaticImage *GetFrame(int32 index) const;
	

private:

	int32 _frame_index;      // index of which animation frame to show
	int32 _frame_counter;    // count how long each frame has been shown for
	
	std::vector<AnimationFrame> _frames;

	friend class GameVideo;
};



/*!****************************************************************************
 *  \brief Manages all the video audio and serves as the API to the video engine.
 *
 * \note Full documentation for video engine can be found at:
 *       http://www.allacrost.org/staff/user/roos/video.html
 *****************************************************************************/

class GameVideo
{
public:
	
	SINGLETON_METHODS(GameVideo);
	

	//-- General --------------------------------------------------------------

	/*!
	 *  \brief call at beginning of every frame
	 */
	bool Clear();
		
	/*!
	 *  \brief call at end of every frame
	 *
	 *  \param frameTime   milliseconds since the last frame
	 */
	bool Display(int32 frameTime);
	

	//-- Video settings -------------------------------------------------------
	
	//! NOTE: when you modify video setting, you must call ApplySettings()
	//!       to actually apply them

	/*!
	 *  \brief sets the current resolution to the given width and height
	 *
	 *  \param width new screen width
	 *  \param height new screen height
	 *  \note  you must call ApplySettings() to actually apply the change
	 */	
	bool SetResolution(int32 width, int32 height);	

	/*!
	 *  \brief returns true if game is in fullscreen mode
	 */	
	bool IsFullscreen();
	
	/*!
	 *  \brief sets the game to fullscreen or windowed depending on whether
	 *         true or false is passed
	 *
	 *  \param fullscreen set to true if you want fullscreen, false for windowed.
	 *  \note  you must call ApplySettings() to actually apply the change
	 */	
	bool SetFullscreen(bool fullscreen);
	
	/*!
	 *  \brief toggles fullscreen on and off
	 *
	 *  \note  you must call ApplySettings() to actually apply the change
	 */	
	bool ToggleFullscreen();
	
	/*!
	 *  \brief applies any changes to video settings like resolution and
	 *         fullscreen. If the changes fail, then this function returns
	 *         false, and the video settings are reset to whatever the last
	 *         working setting was.
	 */	
	bool ApplySettings();


	//-- Coordinate system / viewport  ------------------------------------------
	
	/*!
	 *  \brief sets the viewport, i.e. the area of the screen that gets drawn
	 *         to. The default is (0, 100, 0, 100).
	 *
	 *  \param left   left border of screen
	 *  \param right  right border of screen
	 *  \param top    top border of screen
	 *  \param bottom bottom border of screen
	 *  \note  The arguments are percentages (0-100)
	 */
	void SetViewport
	(
		float left,
		float right,
		float bottom,
		float top
	);

	/*!
	 *  \brief sets the coordinate system. Default is (0,1024,0,768)
	 *  \param left   left border of screen
	 *  \param right  right border of screen
	 *  \param top    top border of screen
	 *  \param bottom bottom border of screen
	 */   	                  
	void SetCoordSys
	(
		float left,
		float right,
		float bottom,
		float top
	);


	/*!
	 *  \brief sets the coordinate system. Default is (0,1024,0,768)
	 *  \param coordSys the coordinate system you want to set to
	 */   	                  
	void SetCoordSys
	(
		const CoordSys &coordSys
	);


	/*!
	 *  \brief enables scissoring, where you can specify a rectangle of the screen
	 *         which is affected by rendering operations. MAKE SURE to disable
	 *         scissoring as soon as you're done using the effect, or all subsequent
	 *         draw calls will get messed up
	 *
	 *  \param enable pass true to turn on scissoring, false to disable
	 */   	                  
	void EnableScissoring(bool enable);
	

	/*!
	 *  \brief returns true if scissoring's enabled
	 */   	                  
	
	bool IsScissoringEnabled() { return _scissorEnabled; }

	/*!
	 *  \brief sets the rectangle to use for scissorring, where you can specify an
	 *         area of the screen for draw operations to affect. Note, the coordinates
	 *         you pass in are based on the current coordinate system, not screen coords
	 */   	                  
	void SetScissorRect
	(
		float left,
		float right,
		float bottom,
		float top
	);


	/*!
	 *  \brief sets the rectangle to use for scissorring. This version of the function
	 *         expects a ScreenRect, in other words the coordinates have already been
	 *         transformed to integer values (pixel unit) with (0,0) as the upper left
	 *         and (w-1, h-1) as the lower right, where w and h are the current screen
	 *         dimensions
	 */   	                  
	void SetScissorRect
	(
		const ScreenRect &rect
	);


	/*!
	 *  \brief returns scissor rect
	 */   	                  
	ScreenRect GetScissorRect() { return _scissorRect; }


	/*!
	 *  \brief converts coordinates given relative to the current coord sys into
	 *         "screen coordinates", which are in pixel units with (0,0) as the
	 *         top left and (w-1, h-1) as the lower-right, where w and h are the
	 *         dimensions of the screen
	 */   	                  

	ScreenRect CalculateScreenRect(float left, float right, float bottom, float top);

	//-- Transformations ------------------------------------------------------

	/*!
	 *  \brief saves entire state of the video engine on to the stack (all draw
	 *         flags, coordinate system, scissor rect, viewport, etc.)
	 *         This is useful for safety purposes between two major parts of code
	 *         to ensure that one part doesn't inadvertently affect the other.
	 *         However, it's a very expensive function call. If you only need to
	 *         push the current transformation, you should use PushMatrix() and
	 *         PopMatrix()
	 */
	void PushState();
	
	/*!
	 *  \brief pops the most recently pushed video engine state from the stack
	 *         and restores all of the old settings.
	 */
	void PopState ();
	

	/*!
	 *  \brief saves current modelview transformation on to the stack. In English,
	 *         that means the combined result of calls to Move/MoveRelative/Scale/Rotate
	 */
	void PushMatrix();
	
	/*!
	 *  \brief pops the modelview transformation from the stack.
	 */
	void PopMatrix();

	/*!
	 *  \brief sets draw position to (x,y)
	 *  \param x  x coordinate to move to
	 *  \param y  y coordinate to move to
	 */
	void Move   (float x, float y);
	
	/*!
	 *  \brief moves draw position (dx, dy) units
	 *  \param dx how many units to move in x direction
	 *  \param dy how many units to move in y direction
	 */
	void MoveRelative(float dx, float dy);
	
	/*!
	 *  \brief rotates images counterclockwise by 'angle' radians
	 *  \param angle how many radians to rotate by
	 *  \note This function should NOT be used unless you understand how transformation
	 *        matrices work in OpenGL.
	 */
	void Rotate (float angle);

	
	/*!
	 *  \brief after you call this, subsequent calls to DrawImage() result in
	 *         a scaled image
	 *
	 *  \param xScale pass 1.0 for normal horizontal scaling, 2.0 for double
	 *                  scaling, etc.
	 *  \param yScale same, except vertical scaling
	 *
	 *  \note This function should NOT be used unless you understand how transformation
	 *        matrices work in OpenGL.
	 */

	void Scale(float xScale, float yScale);
	

	/*!
	 *  \brief sets OpenGL transform to contents of 4x4 matrix (16 values)
	 *  \param array of 16 float values forming a 4x4 transformation matrix
	 */
	void SetTransform(float m[16]);


	//-- Text -----------------------------------------------------------------

	/*!
	 *  \brief loads a font from a TTF file
	 *  \param TTF_filename  the filename of the TTF file to load
	 *  \param name          name of the font (e.g. "courier24")
	 *  \param size          point size of the font
	 */
	bool LoadFont
	(
		const std::string &TTF_filename, 
		const std::string &name, 
		int32 size
	);
	

	/*!
	 *  \brief returns true if the font with the given name has been successfully loaded
	 *  \param name   name of the font (e.g. "courier24")
	 */
	bool IsValidFont(const std::string &name);


	/*!
	 *  \brief given the name of a font, fill in a font properties structure with info
	 *         like the height of the font, etc.
	 *
	 *  \param fontName  name of the font, e.g. "courier24"
	 *  \param fp        reference to font properties structure to return information into
	 */
	bool GetFontProperties(const std::string &fontName, FontProperties &fp);


	/*!
	 *  \brief calculates the width of the given text if it were rendered with the given font
	 *         If an invalid font name is passed, returns -1
	 *
	 *  \param fontName  the font to use
	 *  \param text      the text string in unicode
	 */

	int32 CalculateTextWidth(const std::string &fontName, const hoa_utils::ustring &text);


	/*!
	 *  \brief calculates the width of the given text if it were rendered with the given font
	 *         If an invalid font name is passed, returns -1
	 *
	 *  \param fontName  the font to use
	 *  \param text      the text string in multi-byte character format
	 */

	int32 CalculateTextWidth(const std::string &fontName, const std::string  &text);


	/*!
	 *  \brief sets current font
	 *  \param name  name of the font to set to
	 */
	bool SetFont(const std::string &name);


	/*!
	 *  \brief enables/disables text shadowing
	 *  \param enable  pass true to enable, false to disable
	 */
	void EnableTextShadow(bool enable);

	
	/*!
	 *  \brief sets current text color
	 *  \param color  color to set to
	 */
	bool SetTextColor(const Color &color);
		

	/*!
	 *  \brief sets the shadow offset to use for a given font. By default, all font shadows
	 *         are slightly to the right and to the bottom of the text, by an offset of
	 *         fontHeight / 8. That doesn't always look good though, so use this function
	 *         to adjust it if you want.
	 *
	 *  \param fontName  label of the font you want to set the shadow offset for
	 *  \param x         x offset in pixels (based on 1024x768)
	 */
	bool SetFontShadowXOffset(const std::string &fontName, int32 x);


	/*!
	 *  \brief sets the shadow offset to use for a given font. By default, all font shadows
	 *         are slightly to the right and to the bottom of the text, by an offset of
	 *         fontHeight / 8. That doesn't always look good though, so use this function
	 *         to adjust it if you want.
	 *
	 *  \param fontName  label of the font you want to set the shadow offset for
	 *  \param y         y offset in pixels (based on 1024x768)
	 */
	bool SetFontShadowYOffset(const std::string &fontName, int32 y);


	/*!
	 *  \brief sets the shadow style to use for the given font
	 *
	 *  \param fontName  label of the font you want to set the shadow style for
	 *  \param style     the shadow style you want (e.g. VIDEO_TEXT_SHADOW_BLACK)
	 */
	bool SetFontShadowStyle(const std::string &fontName, TextShadowStyle style);


	/*!
	 *  \brief get name of current font
	 */
	std::string GetFont      () const;
	
	/*!
	 *  \brief get current text color
	 */
	Color       GetTextColor () const;
	
	/*!
	 *  \brief non-unicode version of DrawText(). Only use this for debug
	 *         output or other things which don't have to be localized
	 *
	 *  \param text   text string to draw
	 */
	bool DrawText(const std::string &text);
	
	/*!
	 *  \brief unicode version of DrawText(). This should be used for
	 *         anything in the game which might need to be localized
	 *         (game dialogue, interface text, etc.)
	 *
	 *  \param uText  unicode text string to draw
	 */
	bool DrawText(const hoa_utils::ustring &uText);


	//-- Images / Animation ---------------------------------------------------------
	

	/*!
	 *  \brief loads an image (static or animated image). Assumes that you have already called
	 *         all the appropriate functions to initialize the image. In the case of a static image,
	 *         this means setting its filename, and possibly other properties like width, height, and
	 *         color. In the case of an animated image, it means calling AddFrame().
	 *
	 *  \param id  image descriptor to load- either a StaticImage or AnimatedImage.
	 */
	bool LoadImage(ImageDescriptor &id);


	/*!
	 *  \brief captures the contents of the screen and saves it to an image
	 *         descriptor
	 *
	 *  \param id  image descriptor to capture to
	 */
	bool CaptureScreen(StaticImage &id);

	
	/*!
	 *  \brief decreases ref count of an image (static or animated)
	 *
	 *  \param id  image descriptor to decrease the reference count of
	 */
	bool DeleteImage(ImageDescriptor &id);
	

	/*!
	 *  \brief marks the start of an image loading batch. Use for loading
	 *         map tiles for instance, where many images are being loaded at
	 *         once
	 */
	bool BeginImageLoadBatch();
	
	/*!
	 *  \brief marks the end of image loading batch.
	 */
	bool EndImageLoadBatch();

	/*!
	 *  \brief unloads all texture sheets from memory when we lose the GL
	 *         context, so textures can be properly reloaded
	 */
	bool UnloadTextures();
	
	/*!
	 *  \brief reloads textures that have been unloaded after a video
	 *         settings change
	 */
	bool ReloadTextures();

	
	/*!
	 *  \brief sets draw flags (flip, align, blending, etc). Simply pass
	 *         in as many parameters as you want, as long as the last
	 *         parameter is a zero.
	 *         e.g. SetDrawFlags(VIDEO_X_LEFT, VIDEO_Y_CENTER, 0);
	 */
	void SetDrawFlags(int32 firstflag, ...);


	/*!
	 *  \brief draws an image which is modulated by the scene's light color
	 *
	 *  \param id  image descriptor to draw (either StaticImage or AnimatedImage)
	 */	
	bool DrawImage(const ImageDescriptor &id);


	/*!
	 *  \brief draws an image which is modulated by a custom color
	 *
	 *  \param id  image descriptor to draw (either StaticImage or AnimatedImage)
	 */	
	bool DrawImage(const ImageDescriptor &id, const Color &color);
	

	/*!
	 *  \brief converts a 2D array of tiles into one big image
	 *
	 *  \param tiles   a vector of image descriptors (the tiles)
	 *  \param indices a 2D vector in row-column order (e.g. indices[y][x])
	 *         which forms a rectangular array of tiles
	 */
	StaticImage TilesToObject
	( 
		std::vector<StaticImage> &tiles, 
		std::vector< std::vector<uint32> > indices 
	);


	/*!
	 *  \brief returns the amount of animation frames that have passed since the last
	 *         call to GameVideo::Display(). This number is based on VIDEO_ANIMATION_FRAME_PERIOD,
	 *         and is used so that AnimatedImages know how many frames to increment themselves by.
	 */
	int32 GetFrameChange() { return _current_frame_diff; }

	void DEBUG_NextTexSheet();    // These cycle through the currently loaded
	void DEBUG_PrevTexSheet();    // texture sheets so they can be viewed on screen
	
	
	//-- Menus -----------------------------------------------------------------

	/*!
	 *  \brief sets the current menu skin (borders+fill color). Assumes all four
	 *         vertices of menu interior are same color
	 *
	 *  \param imgBaseName  name of images which form this skin.
	 *                      For example if you pass in "/img/menus/chrome", then it will load:
	 *                          /img/menus/chrome_tl.png
	 *                          /img/menus/chrome_t.png
	 *                          /img/menus/chrome_tr.png
	 *                          /img/menus/chrome_l.png
	 *                          /img/menus/chrome_r.png
	 *                          /img/menus/chrome_bl.png
	 *                          /img/menus/chrome_b.png
	 *                          /img/menus/chrome_br.png
	 *                          /img/menus/chrome_tri_t.png
	 *                          /img/menus/chrome_tri_l.png
	 *                          /img/menus/chrome_tri_r.png
	 *                          /img/menus/chrome_tri_b.png
	 *                          /img/menus/chrome_quad.png
	 *	 
	 *  \param fillColor    color for inner area of menu. can be transparent
	 */
	bool SetMenuSkin
	(
		const std::string &imgBaseName,
		const Color  &fillColor
	);

	
	/*!
	 *  \brief sets the current menu skin (borders+fill color). This version of
	 *         SetMenuSkin() allows the 4 vertices of the interior of the menu
	 *         to have different colors so you can have gradients.
	 *
	 *  \param imgBaseName  name of images which form this skin.
	 *                      For example if you pass in "/img/menus/chrome", then it will load:
	 *                          /img/menus/chrome_tl.png
	 *                          /img/menus/chrome_t.png
	 *                          /img/menus/chrome_tr.png
	 *                          /img/menus/chrome_l.png
	 *                          /img/menus/chrome_r.png
	 *                          /img/menus/chrome_bl.png
	 *                          /img/menus/chrome_b.png
	 *                          /img/menus/chrome_br.png
	 *                          /img/menus/chrome_tri_t.png
	 *                          /img/menus/chrome_tri_l.png
	 *                          /img/menus/chrome_tri_r.png
	 *                          /img/menus/chrome_tri_b.png
	 *                          /img/menus/chrome_quad.png
	 *	 
	 *  \param fillColor_TL  color for upper left  vertex of interior
	 *  \param fillColor_TR  color for upper right vertex of interior
	 *  \param fillColor_BL  color for lower left  vertex of interior
	 *  \param fillColor_BR  color for lower right vertex of interior
	 */
	bool SetMenuSkin
	(
		const std::string &imgBaseName,
		const Color  &fillColor_TL,
		const Color  &fillColor_TR,
		const Color  &fillColor_BL,
		const Color  &fillColor_BR
	);


	//-- Lighting and fog -----------------------------------------------------
	
	/*!
	 *  \brief sets the light color for the scene
	 */
	bool SetLighting (const Color &color);

	/*!
	 *  \brief sets fog parameters
	 *
	 *  \param color  Color of the fog (alpha should be 1.0)
	 *  \param intensity  Intensity of fog from 0.0f to 1.0f
	 */	
	bool SetFog      (const Color &color, float intensity);
	
	/*!
	 *  \brief draws a halo at the given spot
	 *
	 *  \param id    image descriptor for the halo image
	 *  \param x     x coordinate of halo
	 *  \param y     y coordinate of halo
	 *  \param color color of halo
	 */
	bool DrawHalo
	(
		const StaticImage &id, 
		float x, 
		float y, 
		const Color &color = Color(1.0f, 1.0f, 1.0f, 1.0f)
	);

	/*!
	 *  \brief draws a real light at the given spot
	 *
	 *  \param id    image descriptor for the light mask
	 *  \param x     x coordinate of light
	 *  \param y     y coordinate of light
	 *  \param color color of light
	 */	
	bool DrawLight
	(
		const StaticImage &id, 
		float x, 
		float y, 
		const Color &color = Color(1.0f, 1.0f, 1.0f, 1.0f)
	);

	/*!
	 *  \brief call with true if this map uses real lights, otherwise call with
	 *         false
	 */	
	bool EnableRealLights(bool enable);
	

	/*!
	 *  \brief call after rendering all real lights
	 */	
	bool AccumulateLights();

	/*!
	 *  \brief call after all map images are drawn to apply lighting. All
	 *         menu and text rendering should occur AFTER this call, so that
	 *         they are not affected by lighting.
	 */	
	bool ApplyLightingOverlay();


	//-- Overlays / lightning -------------------------------------------------------


	/*!
	 *  \brief draws a full screen overlay of the given color
	 *  \note  This is very slow, so use sparingly!
	 */	
	bool DrawFullscreenOverlay(const Color &color);


	/*!
	 *  \brief call to create lightning effect
	 *  \param litFile a .lit file which contains lightning intensities stored
	 *                 as bytes (0-255).
	 */	
	bool MakeLightning(const std::string &litFile);


	/*!
	 *  \brief call this every frame to draw any lightning effects. You should make
	 *         sure to place this call in an appropriate spot. In particular, you should
	 *         draw the lightning before drawing the GUI.
	 */	
	bool DrawLightning();

	
	//-- Fading ---------------------------------------------------------------

	/*!
	 *  \brief fade screen to color in fadeTime number of seconds
	 */
	bool FadeScreen(const Color &color, float fadeTime);
	
	/*!
	 *  \brief returns true if screen fade is currently in progress
	 */
	bool IsFading();
	

	//-- Screen shaking -------------------------------------------------------
	
	/*!
	 *  \brief shakes the screen
	 *
	 *  \param force        Initial force of the shake
	 *  \param falloffTime  How long the shake should last for. Pass 0.0f for
	 *                      a shake that doesn't end until you stop it manually
	 *  \param falloff      Specifies the method of falloff. The default is
	 *                      VIDEO_FALLOFF_NONE.
	 */
	bool ShakeScreen
	(
		float force, 
		float falloffTime, 
		ShakeFalloff falloff = VIDEO_FALLOFF_NONE
	);	

	/*!
	 *  \brief stops all shake effects
	 */
	bool StopShaking();
	
	/*!
	 *  \brief returns true if screen is shaking
	 */
	bool IsShaking();
	

	//-- Miscellaneous --------------------------------------------------------

	/*!
	 *  \brief updates the FPS counter with the given frame time and draws the
	 *         current FPS on the screen.
	 */
	bool DrawFPS(int32 frameTime);
	

	/*!
	 *  \brief toggles the FPS display (on by default)
	 */
	void ToggleFPS();
	

	/*!
	 *  \brief makes a screenshot, saves it as screenshot.jpg in the directory
	 *         of the game
	 */
	bool MakeScreenshot();
	
	/*!
	 *  \brief toggles advanced information display for video engine, shows
	 *         things like number of texture switches per frame, etc.
	 */
	bool ToggleAdvancedDisplay();

	bool SetDefaultCursor(const std::string &cursorImageFilename);
	StaticImage *GetDefaultCursor();

private:
	SINGLETON_DECLARE(GameVideo);
	
	//-- Private variables ----------------------------------------------------

	
	// for now the game gui class is a member of video so that
	// externally people only have to deal with GameVideo.

	private_video::GUI *_gui;    //! pointer to GUI class which implements all GUI functionality

	// draw flags	
	char _blend;        //! blend flag which specifies normal alpha blending
	char _xalign;       //! x align flag which tells if images should be left, center, or right aligned
	char _yalign;       //! y align flag which tells if images should be top, center, or bottom aligned
	char _xflip;        //! x flip flag. true if images should be flipped horizontally
	char _yflip;        //! y flip flag. true if images should be flipped vertically

	char _nextTempFile[9];    //! eight character name for temp files that increments every time you create a new one so they are always unique

	CoordSys    _coordSys;    //! current coordinate system
	
	ScreenRect _viewport;     //! current viewport
	ScreenRect _scissorRect;  //! current scissor rectangle
	
	bool _scissorEnabled;   //! is scissoring enabled or not
	
	private_video::ScreenFader _fader;  //! fader class which implements screen fading
	
	bool   _advancedDisplay;       //! advanced display flag. If true, info about the video engine is shown on screen
	bool   _fpsDisplay;            //! fps display flag. If true, FPS is displayed
	
	int32  _currentDebugTexSheet;  //! current debug texture sheet
	int32  _numTexSwitches;        //! keep track of number of texture switches per frame
	int32  _numDrawCalls;          //! keep track of number of draw calls per frame
	bool   _batchLoading;          //! if true, we are in batch mode
	
	bool   _usesLights;      //! true if real lights are enabled
	GLuint _lightOverlay;    //! lighting overlay texture

	float  _shakeX, _shakeY; //! offsets to shake the screen by (if any)
	
	std::list<private_video::ShakeForce> _shakeForces;  //! current shake forces affecting screen
		
	bool _fullscreen;     //! true if game is currently running fullscreen
	int32  _width;        //! current screen width
	int32  _height;       //! current screen height
	
	// changing the video settings does not actually do anything until
	// you call ApplySettings(). Up til that point, store them in temp
	// variables so if the new settings are invalid, we can roll back.
	
	bool   _temp_fullscreen;   //! holds the desired fullscreen status (true=fullscreen, false=windowed). Not actually applied until ApplySettings() is called
	int32  _temp_width;        //! holds the desired screen width. Not actually applied until ApplySettings() is called
	int32  _temp_height;       //! holds the desired screen height. Not actually applied until ApplySettings() is called
		
	GLuint _lastTexID;    //! ID of the last texture that was bound. Used to eliminate redundant binding of textures

	std::string _currentFont;          //! current font name
	Color       _currentTextColor;     //! current text color

	StaticImage _defaultMenuCursor;  //! image which is to be used as the cursor
	
	bool _textShadow;   //! if true, text shadow effect is enabled

	Color _fogColor;        //! current fog color (set by SetFog())
	float _fogIntensity;    //! current fog intensity (set by SetFog())
	
	Color _lightColor;      //! current scene lighting color (essentially just modulates vertex colors of all the images)

	bool  _lightningActive;  //! true if a lightning effect is active
	int32 _lightningCurTime; //! current time of lightning effect (time since it started)
	int32 _lightningEndTime; //! how many milliseconds to do the lightning effect for
	
	std::vector <float> _lightningData;  //! intensity data for lightning effect

	int32 _animation_counter;   //! counter to keep track of milliseconds since game started for animations
	int32 _current_frame_diff;  //! keeps track of the number of frames animations should increment by for the current frame

	std::vector <StaticImage *>    _batchLoadImages;    //! vector of images in a batch which are to be loaded
	
	std::map    <std::string, private_video::Image*>   _images;      //! STL map containing all the images currently being managed by the video engine	
	std::vector <private_video::TexSheet *>     _texSheets;          //! vector containing all texture sheets currently being managed by the video engine
	std::map    <std::string, TTF_Font *>       _fontMap;            //! STL map containing all the fonts currently being managed by the video engine
	std::map    <std::string, FontProperties *> _fontProperties;     //! STL map containing properties for each font
	
	std::stack  <private_video::Context>      _contextStack;         //! stack containing context, i.e. draw flags plus coord sys. Context is pushed and popped by any GameVideo functions that clobber these settings


	//-- Private methods ------------------------------------------------------


	/*!
	 *  \brief wraps a call to glBindTexture(), except it adds checking to eliminate redundant texture binding. Redundancy checks are already implemented by most drivers, but this is a double check "just in case"
	 *
	 *  \param texID   integer handle to the OpenGL texture
	 */	

	bool _BindTexture(GLuint texID);

	int32 _ConvertXAlign(int32 xalign);
	int32 _ConvertYAlign(int32 yalign);

	/*!
	 *  \brief creates a blank texture of the given width and height and returns integer used by OpenGL to refer to this texture. Returns 0xffffffff on failure.
	 *
	 *  \param width   desired width of the texture
	 *  \param height  desired height of the texture
	 */	

	GLuint _CreateBlankGLTexture(int32 width, int32 height);


	/*!
	 *  \brief creates an StaticImage of a menu which is the given size
	 *
	 *  \param menu   Reference to menu to create
	 *
	 *  \param width  Width of menu, based on pixels in 1024x768 resolution
	 *  \param height Height of menu, based on pixels in 1024x768 resolution.
	 *  \param edgeVisibleFlags bit flags to tell which edges are visible
	 *  \param edgeSharedFlags  bit flags to tell which edges are shared with other menus
	 *
	 *  \note  this is only meant to be used by the Menu class, not by users of
	 *         the video engine.
	 */
	bool _CreateMenu
	(
		StaticImage &menu, 
		float width, 
		float height, 
		int32 edgeVisibleFlags, 
		int32 edgeSharedFlags
	);


	/*!
	 *  \brief returns a filename like TEMP_abcd1234.ext, and each time you call it, it increments the
	 *         alphanumeric part of the filename. This way, during any particular run
	 *         of the game, each temp filename is guaranteed to be unique.
	 *         Assuming you create a new temp file every second, it would take 100,000 years to get
	 *         from TEMP_00000000 to TEMP_zzzzzzzz
	 *
	 *  \param extension   The extension for the temp file. Although we could just save temp files
	 *                     without an extension, that might cause stupid bugs like DevIL refusing
	 *                     to load an image because it doesn't end with .png.
	 */	

	std::string _CreateTempFilename(const std::string &extension);


	/*!
	 *  \brief creates a texture sheet
	 *
	 *  \param width    width of the sheet
	 *  \param height   height of the sheet
	 *  \param type     specifies what type of images this texture sheet manages (e.g. 32x32 images, 64x64 images, any type, etc)
	 *  \param isStatic if true, this texture sheet is meant to manage images which are not expected to be loaded and unloaded very often
	 */	

	private_video::TexSheet *_CreateTexSheet
	(
		int32 width,
		int32 height,
		private_video::TexSheetType type,
		bool isStatic
	);


	/*!
	 *  \brief wraps a call to glDeleteTextures(), except it adds some checking related to eliminating redundant texture binding.
	 *
	 *  \param texID   integer handle to the OpenGL texture
	 */	
	bool _DeleteTexture(GLuint texID);


	/*!
	 *  \brief decreases the reference count of an image
	 *
	 *  \param image  pointer to image
	 */	
	bool _DeleteImage    (private_video::Image *const image);
	

	/*!
	 *  \brief decreases ref count of an image
	 *
	 *  \param id  image descriptor to decrease the reference count of
	 */
	bool _DeleteImage(StaticImage &id);

	
	/*!
	 *  \brief decreases ref count of an animated image
	 *
	 *  \param id  image descriptor to decrease the reference count of
	 */
	bool _DeleteImage(AnimatedImage &id);

	
	/*!
	 *  \brief deletes the temporary textures from the "temp" folder that were saved
	 *         by _SaveTempTextures()
	 */	
	
	bool _DeleteTempTextures();

	
	/*!
	 *  \brief draws an image element, i.e. one image within an image descriptor which may contain multiple images
	 *
	 *  \param element        pointer to the image element to draw
	 *  \param modulateColor  combination of color for this image, and our current fade color
	 */		 
	bool _DrawElement
	(
		const private_video::ImageElement &element, 
		const Color &modulateColor
	);


	/*!
	 *  \brief draws an image element, i.e. one image within an image descriptor which may contain multiple images
	 *
	 *  \note  this version of the function accepts no color, so for cases where no fade is going on
	 *         and we don't want to modulate the image's color (which is the case indeed 99% of the time),
	 *         we can skip all the nasty modulation math for a bit of extra efficiency
	 *
	 *  \param element        pointer to the image element to draw
	 */		 
	bool _DrawElement
	(
		const private_video::ImageElement &element
	);


	/*!
	 *  \brief helper function to DrawImage() to do the actual work of doing an image
	 *
	 *  \param img static image to draw
	 */	
	bool _DrawStaticImage(const StaticImage &img);


	/*!
	 *  \brief helper function to DrawImage() to do the actual work of drawing an image
	 *
	 *  \param img static image to draw
	 *  \param color color to modulate image by
	 */	
	bool _DrawStaticImage(const StaticImage &img, const Color &color);  


	/*!
	 *  \brief does the actual work of drawing text
	 *
	 *  \param uText  Pointer to a unicode string holding the text to draw
	 */	

	bool _DrawTextHelper
	(
		const uint16 *const uText
	);


	/*!
	 *  \brief inserts an image into a texture sheet
	 *
	 *  \param image       pointer to the image to insert
	 *  \param w           width of image (in pixels)
	 *  \param h           height of image (in pixels) 
	 */	

	private_video::TexSheet *_InsertImageInTexSheet
	(
		private_video::Image *image,
		int32 w,
		int32 h,
		bool isStatic
	);


	/*!
	 *  \brief linearly interpolation, returns a value which is (alpha*100) percent between initial and final
	 *
	 *  \param alpha    controls the linear interpolation
	 *  \param initial  initial value
	 *  \param final    final value
	 */	

	float _Lerp(float alpha, float initial, float final);


	/*!
	 *  \brief loads an image
	 *
	 *  \param id  image descriptor to load. Can specify filename, color, width, height, and static as its parameters
	 */
	bool _LoadImage(StaticImage &id);


	/*!
	 *  \brief loads an animated image. Assumes that you have called AddFrame for all the frames.
	 *
	 *  \param id  image descriptor to load
	 */
	bool _LoadImage(AnimatedImage &id);
	

	/*!
	 *  \brief does the actual work of loading an image
	 *
	 *  \param id  StaticImage of the image to load. May specify a filename, color, width, height, and static
	 */	

	bool _LoadImageHelper(StaticImage &id);


	/*!
	 *  \brief use DevIL to load an image and return raw pixel data
	 *
	 *  \param filename   Filename of image to load
	 *  \param pixelData  The loaded image's integer handle gets returned into this variable
	 *  \param width      The loaded image's width gets returned into this variable
	 *  \param width      The loaded image's height gets returned into this variable
	 */	

	bool _LoadRawPixelData(const std::string &filename, ILuint &pixelData, uint32 &width, uint32 &height);


	/*!
	 *  \brief loop through all currently loaded images and if they belong to the given tex sheet, reload them into it
	 *
	 *  \param texSheet   pointer to the tex sheet whose images we want to load
	 */	

	bool _ReloadImagesToSheet(private_video::TexSheet *texSheet);


	/*!
	 *  \brief removes the image from the STL map with the same pointer as the one passed in. Returns false on failure
	 *
	 *  \param imageToRemove   pointer to the image we want to remove
	 */	

	bool _RemoveImage(private_video::Image *imageToRemove);
	

	/*!
	 *  \brief removes a texture sheet from our vector of sheets and deletes it
	 *
	 *  \param sheetToRemove  pointer to the sheet we want to remove
	 */	
	bool _RemoveSheet(private_video::TexSheet *sheetToRemove);


	/*!
	 *  \brief rounds a force value to the nearest integer. Rounding is based on probability. For example the number 2.85 has an 85% chance of rounding to 3 and a 15% chance of rounding to 2
	 *
	 *  \param force  The force to round
	 */	
	float _RoundForce(float force);   // rounds a force value


	/*!
	 *  \brief restores coord system, draw flags, and transformations
	 */	

	void _PopContext();


	/*!
	 *  \brief saves coord system, draw flags, and transformations
	 */	

	void _PushContext();
	
	/*!
	 *  \brief saves temporary textures to disk, in other words, textures which were not
	 *         loaded to a file. This is used when the GL context is being destroyed,
	 *         perhaps because we are switching from windowed to fullscreen. So, we need
	 *         to save all textures to disk so we can reload them later.
	 */	
	
	bool _SaveTempTextures();


	int32 _ScreenCoordX(float x);
	int32 _ScreenCoordY(float y);

	/*!
	 *  \brief updates the shaking effect
	 *
	 *  \param frameTime  elapsed time for the current rendering frame
	 */	
	void  _UpdateShake(int32 frameTime);


	/*!
	 *  \brief function solely for debugging, which shows number of texture switches made during a frame
	 */	
	bool _DEBUG_ShowTexSwitches();


	/*!
	 *  \brief function solely for debugging, which displays the currently selected texture sheet. By using DEBUG_NextTexSheet() and DEBUG_PrevTexSheet(), you can change the current texture sheet so the sheet shown by this function cycles through all currently loaded sheets.
	 */	
	bool _DEBUG_ShowTexSheet();
	
	friend class TextBox;
	friend class OptionBox;
	friend class GUIElement;
	friend class MenuWindow;
	friend class private_video::GUI;	
	friend class private_video::FixedTexMemMgr;
	friend class private_video::VariableTexMemMgr;
	friend class private_video::TexSheet;
};

}   // namespace hoa_video

#endif // !_VIDEO_HEADER_
