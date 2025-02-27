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

#ifndef PREFERENCE_DIALOGS_H
#define PREFERENCE_DIALOGS_H

#include "shared_widgets.hpp"
#include "OGL_Setup.hpp"

class OpenGLDialog
{
public:
	// Abstract factory; concrete type determined at link-time
	static std::unique_ptr<OpenGLDialog> Create(int theSelectedRenderer);

	void OpenGLPrefsByRunning ();

	virtual ~OpenGLDialog ();
protected:
	OpenGLDialog ();
	
	virtual bool Run () = 0;
	virtual void Stop (bool result) = 0;
	
	ButtonWidget*		m_cancelWidget;
	ButtonWidget*		m_okWidget;
	
	ToggleWidget*		m_zBufferWidget;
	ToggleWidget*		m_fogWidget;
	ToggleWidget*		m_colourEffectsWidget;
	ToggleWidget*		m_transparentLiquidsWidget;
	ToggleWidget*		m_3DmodelsWidget;
	ToggleWidget*		m_blurWidget;
	ToggleWidget*		m_bumpWidget;
	ToggleWidget*		m_perspectiveWidget;
	
	ToggleWidget*		m_colourTheVoidWidget;
	ColourPickerWidget*	m_voidColourWidget;

	SelectorWidget* m_ephemeraQualityWidget;
	
	SelectorWidget*		m_fsaaWidget;
	
	SelectorWidget*		m_anisotropicWidget;

	ToggleWidget*           m_sRGBWidget;
	
	SelectorWidget* m_textureQualityWidget [OGL_NUMBER_OF_TEXTURE_TYPES];
	SelectorWidget* m_modelQualityWidget;
	
	ToggleWidget*		m_useNPOTWidget;
	ToggleWidget* m_vsyncWidget;
	SelectSelectorWidget*		m_wallsFilterWidget;
	SelectSelectorWidget*		m_spritesFilterWidget;

	SelectSelectorWidget* m_nearFiltersWidget[OGL_NUMBER_OF_TEXTURE_TYPES];
};

#endif
