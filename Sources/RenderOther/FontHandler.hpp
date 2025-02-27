#ifndef _FONT_HANDLER_
#define _FONT_HANDLER_

/*
 *
 *  Aleph Bet is copyright ©1994-2024 Bungie Inc., the Aleph One developers,
 *  and the Aleph Bet developers.
 *
 *  Aleph Bet is free software: you can redistribute it and/or modify it
 *  under the terms of the GNU General Public License as published by the
 *  Free Software Foundation, either version 3 of the License, or (at your
 *  option) any later version.
 *
 *  Aleph Bet is distributed in the hope that it will be useful, but WITHOUT
 *  ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
 *  FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for
 *  more details.
 *
 *  You should have received a copy of the GNU General Public License along
 *  with this program. If not, see <https://www.gnu.org/licenses/>.
 *
 *  This license notice applies only to the Aleph Bet engine itself, and
 *  does not apply to Marathon, Marathon 2, or Marathon Infinity scenarios
 *  and assets, nor to elements of any third-party scenarios.
 *
 */

/*	
 *  This is for specifying and working with text fonts; specifying them,
 *  changing them with XML, and creating a font brush for OpenGL rendering
 */


#include "cseries.hpp"
#include "sdl_fonts.hpp"
#include "OGL_Headers.hpp"

#include <set>

struct screen_rectangle;

class FontSpecifier;

class FontSpecifier
{
public:
	// Parameters:
	std::string NameSet; // unused

	short Size;
	short Style;
	short AdjustLineHeight;
	
	// For SDL font support: a font filename
	std::string File;
	
	// Derived quantities: sync with parameters by calling Update()
	short Height;			// How tall is it?
	short LineSpacing;		// From same positions in each line
	short Ascent, Descent, Leading;
	short Widths[256];
	
	font_info *Info;
	
	// Initialize: call this before calling anything else;
	// this is from not having a proper constructor for this object.
	void Init();
	
	// Do the updating: must be called before using the font; however, it is called by Init(),
	// and it will be called by the XML parser if it updates the parameters
	void Update();
	
	// Get text width for text that must be centered (map title)
	int TextWidth(const char *Text);

	// Get width of one character
	int CharWidth(char c) const { return Widths[static_cast<int>(c)]; }
	
#ifdef HAVE_OPENGL	
	// Reset the OpenGL fonts; its arg indicates whether this is for starting an OpenGL session
	// (this is to avoid texture and display-list memory leaks and other such things)
	void OGL_Reset(bool IsStarting);
	
	// Renders a C-style string in OpenGL.
	// assumes screen coordinates and that the left baseline point is at (0,0).
	// Alters the modelview matrix so that the next characters will be drawn at the proper place.
	// One can surround it with glPushMatrix() and glPopMatrix() to remember the original.
	void OGL_Render(const char *Text);

	// Renders text a la _draw_screen_text() (see screen_drawing.h), with
	// alignment and wrapping. Modelview matrix is unaffected.
	void OGL_DrawText(const char *Text, const screen_rectangle &r, short flags);
	
	// Calls OGL_Reset() on all fonts. This is used when the OpenGL context
	// is changing, so that textures and display lists are cleaned up.
	static void OGL_ResetFonts(bool IsStarting);	

	// Add or remove an instance from the registry of in-use OpenGL fonts.
	// To recycle OpenGL assets properly on context switches, the set
	// m_font_registry tracks all active fonts.
	static void OGL_Register(FontSpecifier *F);
	static void OGL_Deregister(FontSpecifier *F);
	
#endif

	// Draw text without worrying about OpenGL vs. SDL mode.
	int DrawText(SDL_Surface *s, const char *text, int x, int y, uint32 pixel, bool utf8 = false);

	~FontSpecifier();

	// Equality and assignment operators
	bool operator==(FontSpecifier& F);
	bool operator!=(FontSpecifier& F)
		{return (!((*this) == F));}
	FontSpecifier& operator=(FontSpecifier& F);
	
	// Not sure what kind of explicit constructor would be consistent with the way
	// that fonts' initial values are specified, as {nameset-string, size, style}.
	// Also, private members are inconsistent with that sort of initialization.

#ifdef HAVE_OPENGL
	// Stuff for OpenGL font rendering: the font texture and a display list for font rendering;
	// if OGL_Texture is NULL, then there is no OpenGL font texture to render.
	uint8 *OGL_Texture;
	short TxtrWidth, TxtrHeight;
	int GetTxtrSize() {return int(TxtrWidth)*int(TxtrHeight);}
	GLuint TxtrID;
	GLuint NearFilter = GL_LINEAR;
	uint32 DispList;
	static std::set<FontSpecifier*> *m_font_registry;
#endif
};


#endif
