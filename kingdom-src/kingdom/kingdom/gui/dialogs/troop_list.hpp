/* $Id: hero_list.hpp 49605 2011-05-22 17:56:24Z mordante $ */
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

#ifndef GUI_DIALOGS_TROOP_LIST_HPP_INCLUDED
#define GUI_DIALOGS_TROOP_LIST_HPP_INCLUDED

#include "gui/dialogs/dialog.hpp"

#include "config.hpp"

class game_display;
class team;
class unit_map;
class unit;
class hero;
class hero_map;
class game_state;

namespace gui2 {

class tbutton;
class tgrid;
class tlistbox;
class tstacked_widget;
class twindow;

class ttroop_list : public tdialog
{
public:
	explicit ttroop_list(game_display& gui, std::vector<team>& teams, unit_map& units, hero_map& heros, game_state& gamestate, int side_num);

	bool compare_row(twidget& row1, twidget& row2);
protected:
	/** Inherited from tdialog. */
	void pre_show(CVideo& video, twindow& window);

	/** Inherited from tdialog. */
	void post_show(twindow& window);

private:
	/** Inherited from tdialog, implemented by REGISTER_DIALOG. */
	virtual const std::string& window_id() const;

	enum {OWNERSHIP_PAGE, STATUS_PAGE, MERIT_PAGE};
	void catalog_page(twindow& window, int catalog, bool swap);	

	void city_changed(twindow& window, tlistbox& list, const int type = twidget::drag_none);
	void fill_table(tlistbox& list, int catalog);
	void fill_table_row(team& t, int catalog);

	void sort_column(tbutton& widget, int catalog);
private:
	game_display& gui_;
	std::vector<team>& teams_;
	unit_map& units_;
	hero_map& heros_;
	game_state& gamestate_;

	int side_;
	std::vector<const unit*> candidate_troops_;

	bool ascend_;
	int current_page_;
	std::map<int, std::vector<tbutton*> > sorting_widgets_;
	tbutton* sorting_widget_;

	tstacked_widget* page_panel_;
	std::vector<tlistbox*> lists_;
	twindow* window_;
};

}

#endif

