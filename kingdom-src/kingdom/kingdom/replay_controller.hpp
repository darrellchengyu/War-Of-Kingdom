/* $Id: replay_controller.hpp 46186 2010-09-01 21:12:38Z silene $ */
/*
   Copyright (C) 2006 - 2010 by Joerg Hinrichs <joerg.hinrichs@alice-dsl.de>
   wesnoth playlevel Copyright (C) 2003 by David White <dave@whitevine.net>
   Part of the Battle for Wesnoth Project http://www.wesnoth.org/

   This program is free software; you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation; either version 2 of the License, or
   (at your option) any later version.
   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY.

   See the COPYING file for more details.
*/

#ifndef REPLAY_CONTROLLER_H_INCLUDED
#define REPLAY_CONTROLLER_H_INCLUDED

#include "game_end_exceptions.hpp"
#include "gamestatus.hpp"
#include "play_controller.hpp"

#include <vector>

class video;

class replay_controller : public play_controller
{
public:
	replay_controller(const config& level, game_state& state_of_game, hero_map& heros, hero_map& heros_start,
		card_map& cards,
		const int ticks, const int num_turns, const config& game_config, CVideo& video);
	virtual ~replay_controller();

	//event handlers
	virtual void preferences();
	void play_replay();
	void stop_replay();
	void replay_next_turn();
	void replay_next_side();
	void process_oos(const std::string& msg) const;
	void replay_show_everything();
	void replay_show_each();
	void replay_show_team1();
	void replay_skip_animation();

	virtual void force_end_turn() {}
	virtual void force_end_level(LEVEL_RESULT) {}
	virtual void check_end_level() {}

	bool is_playing() const { return is_playing_; }
	void play_replay2();

	std::vector<team> teams_start_;

protected:
	virtual void init_gui();
	void linger();

private:
	/** command_executor override */
	void execute_command2(int command, const std::string& sparam);

	void init();
	virtual void play_turn();
	virtual void play_side();
	void update_teams();
	void update_gui();

	game_state gamestate_start_;
	tod_manager tod_manager_start_;

	unsigned int current_turn_;
	int delay_;
	bool is_playing_;

	bool show_everything_;
	unsigned int show_team_;
};


LEVEL_RESULT play_replay_level(const config& terrain_config,
		const config* level, CVideo& video,
		game_state& state_of_game, hero_map& heros, hero_map& heros_start,
		card_map& cards);

#endif
