/* $Id: image.hpp 52533 2012-01-07 02:35:17Z shadowmaster $ */
/*
   Copyright (C) 2008 - 2012 by Mark de Wever <koraq@xs4all.nl>
   Part of the Battle for Wesnoth Project http://www.wesnoth.org/

   This program is free software; you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation; either version 2 of the License, or
   (at your option) any later version.
   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY.

   See the COPYING file for more details.
*/

#ifndef GUI_WIDGETS_IMAGE_HPP_INCLUDED
#define GUI_WIDGETS_IMAGE_HPP_INCLUDED

#include "gui/widgets/control.hpp"

namespace gui2 {

/** An image. */
class timage : public tcontrol
{
public:

	timage()
		: tcontrol(COUNT)
		, best_size_(0, 0)
	{}

public:
	/**
	 * Wrapper for set_label.
	 *
	 * Some people considered this function missing and confusing so added
	 * this forward version.
	 *
	 * @param label               The filename image to show.
	 */
	void set_image(const std::string& label)
	{
		set_label(label);
	}

	/**
	 * Wrapper for label.
	 *
	 * Some people considered this function missing and confusing so added
	 * this forward version.
	 *
	 * @returns                   The filename of the image shown.
	 */
	std::string get_image() const
	{
		return label();
	}

	/***** ***** ***** ***** layout functions ***** ***** ***** *****/

private:
	/** Inherited from tcontrol. */
	tpoint calculate_best_size() const;
public:

	/***** ***** ***** ***** Inherited ***** ***** ***** *****/

	/** Inherited from tcontrol. */
	void set_active(const bool /*active*/) {}

	/** Inherited from tcontrol. */
	bool get_active() const { return true; }

	/** Inherited from tcontrol. */
	unsigned get_state() const { return ENABLED; }

	/** Inherited from tcontrol. */
	bool disable_click_dismiss() const { return false; }

	/***** ***** ***** setters / getters for members ***** ****** *****/
	void set_best_size(const tpoint& best_size) { best_size_ = best_size; }

private:

	/** When we're used as a fixed size item, this holds the best size. */
	tpoint best_size_;

	/**
	 * Possible states of the widget.
	 *
	 * Note the order of the states must be the same as defined in settings.hpp.
	 */
	enum tstate { ENABLED, COUNT };

	/** Inherited from tcontrol. */
	const std::string& get_control_type() const;
};

} // namespace gui2

#endif

