/* $Id: mouse_action.hpp 47808 2010-12-05 18:08:14Z mordante $ */
/*
   Copyright (C) 2008 - 2010 by Tomasz Sniatowski <kailoran@gmail.com>
   Part of the Battle for Wesnoth Project http://www.wesnoth.org/

   This program is free software; you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation; either version 2 of the License, or
   (at your option) any later version.
   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY.

   See the COPYING file for more details.
*/

#ifndef EDITOR_MOUSE_ACTION_HPP
#define EDITOR_MOUSE_ACTION_HPP

#include "editor/action_base.hpp"
#include "editor/editor_map.hpp"

class CKey;

namespace editor {

/**
 * A mouse action receives events from the controller, and responds to them by creating
 * an appropriate editor_action object. Mouse actions may store some temporary data
 * such as the last clicked hex for better handling of click-drag. They should *not* modify
 * the map or trigger refreshes, but may set brush locations and similar overlays that
 * should be visible around the mouse cursor, hence the display references are not const.
 */
class mouse_action
{
public:
	mouse_action(const CKey& key)
		: previous_move_hex_()
		, key_(key)
	{
	}

	virtual ~mouse_action() {}

	virtual bool has_context_menu() const;

	/**
	 * Mouse move (not a drag). Never changes anything (other than temporary highlihts and similar)
	 */
	void move(editor_display& disp, const map_location& hex);

	/**
	 * Unconditionally update the brush highlights for the current tool when hex is the center location
	 */
	void update_brush_highlights(editor_display& disp, const map_location& hex);

	/**
	 * Locations that would be affected by a click, used by move to update highlights. Defauts to higlight the mouseover hex.
	 * Maybe also used for actually performing the action in click() or drag().
	 */
	virtual std::set<map_location> affected_hexes(editor_display& disp, const map_location& hex);

	/**
	 * A click, possibly the beginning of a drag. Must be overridden.
	 */
	virtual editor_action* click_left(editor_display& disp, int x, int y) = 0;

	/**
	 * A click, possibly the beginning of a drag. Must be overridden.
	 */
	virtual editor_action* click_right(editor_display& disp, int x, int y) = 0;

	/**
	 * Drag operation. A click should have occurred earlier. Defaults to no action.
	 */
	virtual editor_action* drag_left(editor_display& disp, int x, int y, bool& partial, editor_action* last_undo);

	/**
	 * Drag operation. A click should have occurred earlier. Defaults to no action.
	 */
	virtual editor_action* drag_right(editor_display& disp, int x, int y, bool& partial, editor_action* last_undo);

	/**
	 * The end of dragging. Defaults to no action.
	 */
	virtual editor_action* drag_end(editor_display& disp, int x, int y);


	virtual editor_action* up_left(editor_display& disp, int x, int y);

	virtual editor_action* up_right(editor_display& disp, int x, int y);

	/**
	 * Function called by the controller on a key event for the current mouse action.
	 * Defaults to starting position processing.
	 */
	virtual editor_action* key_event(editor_display& disp, const SDL_Event& e);

	/**
	 * Set the mouse overlay for this action. Defaults to an empty overlay.
	 */
	virtual void set_mouse_overlay(editor_display& disp);

protected:
	bool has_alt_modifier() const;
	bool has_shift_modifier() const;
	bool has_ctrl_modifier() const;

	/**
	 * Helper function for derived classes that need a active-terrain mouse overlay
	 */
	void set_terrain_mouse_overlay(editor_display& disp, t_translation::t_terrain fg,
		t_translation::t_terrain bg);

	/**
	 * The hex previously used in move operations
	 */
	map_location previous_move_hex_;

	/**
	 * Key presses, used for modifiers (alt, shift) in some operations
	 */
	const CKey& key_;

};

/**
 * A brush-drag mouse action base class which adds brush and drag processing to a basic mouse action
 */
class brush_drag_mouse_action : public mouse_action
{
public:
	brush_drag_mouse_action(const brush* const * const brush, const CKey& key)
		: mouse_action(key)
		, previous_drag_hex_()
		, brush_(brush)
	{
	}

	/**
	 * The affected hexes of a brush action are the result of projecting the current brush on the mouseover hex
	 */
	std::set<map_location> affected_hexes(editor_display& disp, const map_location& hex);

	/**
	 * The actual action function which is called by click() and drag(). Derived classes override this instead of click() and drag().
	 */
	virtual editor_action* click_perform_left(editor_display& disp, const std::set<map_location>& hexes) = 0;

	/**
	 * The actual action function which is called by click() and drag(). Derived classes override this instead of click() and drag().
	 */
	virtual editor_action* click_perform_right(editor_display& disp, const std::set<map_location>& hexes) = 0;

	/**
	 * Calls click_perform_left()
	 */
	editor_action* click_left(editor_display& disp, int x, int y);

	/**
	 * Calls click_perform_right()
	 */
	editor_action* click_right(editor_display& disp, int x, int y);

	/**
	 * Calls click_perform() for every new hex the mouse is dragged into.
	 * @todo partial actions support and merging of many drag actions into one
	 */
	editor_action* drag_left(editor_display& disp, int x, int y, bool& partial, editor_action* last_undo);

	/**
	 * Calls click_perform for every new hex the mouse is dragged into.
	 * @todo partial actions support and merging of many drag actions into one
	 */
	editor_action* drag_right(editor_display& disp, int x, int y, bool& partial, editor_action* last_undo);

	/**
	 * End of dragging.
	 * @todo partial actions (the entire drag should end up as one action)
	 */
	editor_action* drag_end(editor_display& disp, int x, int y);

protected:
	/** Brush accessor */
	const brush& get_brush();

	/**
	 * The previous hex dragged into.
	 * @todo keep a set of all "visited" locations to reduce action count in long drags that hit the same hexes multiple times?
	 */
	map_location previous_drag_hex_;

private:
	/**
	 * Template helper gathering actions common for both drag_right and drag_left.
	 * The drags differ only in the worker function called, which should be
	 * passed as the template parameter. This exists only to avoid copy-pasting code.
	 */
	template <editor_action* (brush_drag_mouse_action::*perform_func)(editor_display&, const std::set<map_location>&)>
	editor_action* drag_generic(editor_display& disp, int x, int y, bool& partial, editor_action* last_undo);

	/**
	 * Current brush handle. Currently a pointer-to-pointer with full constness.
	 * The mouse action does not modify the brush, does not modify the pointer
	 * to the current brush, and we allow setting this pointr only once, hence
	 * the three "consts".
	 */
	const brush* const * const brush_;
};

/**
 * Brush paint mouse action. Uses keyboard modifiers for one-layer painting.
 */
class mouse_action_paint : public brush_drag_mouse_action
{
public:
	mouse_action_paint(t_translation::t_terrain& terrain_left,
		t_translation::t_terrain& terrain_right,
		const brush* const * const brush, const CKey& key)
	: brush_drag_mouse_action(brush, key)
	, terrain_left_(terrain_left)
	, terrain_right_(terrain_right)
	{
	}

	/**
	 * Handle terrain sampling before calling generic handler
	 */
	editor_action* click_left(editor_display& disp, int x, int y);

	/**
	 * Handle terrain sampling before calling generic handler
	 */
	editor_action* click_right(editor_display& disp, int x, int y);

	/**
	 * Create an appropriate editor_action and return it
	 */
	editor_action* click_perform_left(editor_display& disp, const std::set<map_location>& hexes);

	/**
	 * Create an appropriate editor_action and return it
	 */
	editor_action* click_perform_right(editor_display& disp, const std::set<map_location>& hexes);

	void set_mouse_overlay(editor_display& disp);

protected:
	t_translation::t_terrain& terrain_left_;
	t_translation::t_terrain& terrain_right_;
};

/**
 * Select (and deselect) action, by brush or "magic wand" (via keyboard modifier)
 */
class mouse_action_select : public brush_drag_mouse_action
{
public:
	mouse_action_select(const brush* const * const brush, const CKey& key)
	: brush_drag_mouse_action(brush, key)
	{
	}

	/**
	 * Overridden to allow special behaviour based on modifier keys
	 */
	std::set<map_location> affected_hexes(editor_display& disp, const map_location& hex);

	/**
	 * Force a fake "move" event to update brush overlay on key event
	 */
	editor_action* key_event(editor_display& disp, const SDL_Event& e);

	/**
	 * Left click/drag selects
	 */
	editor_action* click_perform_left(editor_display& disp, const std::set<map_location>& hexes);

	/**
	 * Right click/drag deselects
	 */
	editor_action* click_perform_right(editor_display& disp, const std::set<map_location>& hexes);

	virtual void set_mouse_overlay(editor_display& disp);
};

/**
 * Paste action. No dragging capabilities.
 */
class mouse_action_paste : public mouse_action
{
public:
	mouse_action_paste(const map_fragment& paste, const CKey& key)
	: mouse_action(key), paste_(paste)
	{
	}

	bool has_context_menu() const;

	/**
	 * Show an outline of where the paste will go
	 */
	std::set<map_location> affected_hexes(editor_display& disp, const map_location& hex);

	/**
	 * Return a paste with offset action
	 */
	editor_action* click_left(editor_display& disp, int x, int y);

	/**
	 * Right click does nothing for now
	 */
	editor_action* click_right(editor_display& disp, int x, int y);

	virtual void set_mouse_overlay(editor_display& disp);

protected:
	/**
	 * Reference to the buffer used for pasting (e.g. the clipboard)
	 */
	const map_fragment& paste_;
};

/**
 * Fill action. No dragging capabilities. Uses keyboard modifiers for one-layer painting.
 */
class mouse_action_fill : public mouse_action
{
public:
	mouse_action_fill(t_translation::t_terrain& terrain_left,
		t_translation::t_terrain& terrain_right, const CKey& key)
	: mouse_action(key)
	, terrain_left_(terrain_left)
	, terrain_right_(terrain_right)
	{
	}

	/**
	 * Tiles that will be painted to, possibly use modifier keys here
	 */
	std::set<map_location> affected_hexes(editor_display& disp, const map_location& hex);

	/**
	 * Left / right click fills with the respective terrain
	 */
	editor_action* click_left(editor_display& disp, int x, int y);

	/**
	 * Left / right click fills with the respective terrain
	 */
	editor_action* click_right(editor_display& disp, int x, int y);

	virtual void set_mouse_overlay(editor_display& disp);

protected:
	t_translation::t_terrain& terrain_left_;
	t_translation::t_terrain& terrain_right_;
};

/**
 * Set starting position action.
 */
class mouse_action_starting_position : public mouse_action
{
public:
	mouse_action_starting_position(const CKey& key)
	: mouse_action(key), click_(false)
	{
	}

	/**
	 * Left click displays a player-number-selector dialog and then creates an action
	 * or returns NULL if cancel was pressed or there would be no change.
	 * Do this on mouse up to avoid drag issue.
	 */
	editor_action* up_left(editor_display& disp, int x, int y);

	editor_action* click_left(editor_display& disp, int x, int y);
	/**
	 * Right click only erases the starting position if there is one.
	 * Do this on mouse up to avoid drag issue,
	 */
	editor_action* up_right(editor_display& disp, int x, int y);

	editor_action* click_right(editor_display& disp, int x, int y);

	virtual void set_mouse_overlay(editor_display& disp);

private:
	bool click_;
};



} //end namespace editor

#endif
