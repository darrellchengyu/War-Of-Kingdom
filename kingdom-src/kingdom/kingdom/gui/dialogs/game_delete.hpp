/* $Id: game_delete.hpp 48871 2011-03-13 07:49:19Z mordante $ */
/*
   Copyright (C) 2008 - 2011 by Jörg Hinrichs <joerg.hinrichs@alice-dsl.de>
   Part of the Battle for Wesnoth Project http://www.wesnoth.org/

   This program is free software; you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation; either version 2 of the License, or
   (at your option) any later version.
   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY.

   See the COPYING file for more details.
*/

#ifndef GUI_DIALOGS_DELETE_GAME_HPP_INCLUDED
#define GUI_DIALOGS_DELETE_GAME_HPP_INCLUDED

#include "gui/dialogs/dialog.hpp"

namespace gui2 {

class tgame_delete
	: public tdialog
{
public:

	tgame_delete();

	/** The excute function see @ref tdialog for more information. */
	static bool execute(CVideo& video)
	{
		return tgame_delete().show(video);
	}

private:

	/** Inherited from tdialog, implemented by REGISTER_DIALOG. */
	virtual const std::string& window_id() const;

	/** Inherited from tdialog. */
	void pre_show(CVideo& video, twindow& window);
};

}

#endif

