/*! \file menu.c
 *  \brief Main Menu view.
 */
/*
 * Copyright (c) Sebastian Krzyszkowiak <dos@dosowisko.net>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA 02110-1301, USA.
 */
#include <stdio.h>
#include <math.h>
#include <allegro5/allegro_ttf.h>
#include <allegro5/allegro_primitives.h>
#include "../config.h"
#include "../utils.h"
#include "../timeline.h"
#include "menu.h"

#define SOLO_MIN 20

int Gamestate_ProgressCount = 6;

void DrawMenuState(struct Game *game, struct MenuResources *data) {
	ALLEGRO_FONT *font = data->font;
	char* text = malloc(255*sizeof(char));
	struct ALLEGRO_COLOR color;
	switch (data->menustate) {
		case MENUSTATE_MAIN:
			DrawTextWithShadow(font, data->selected==0 ? al_map_rgb(255,255,128) : al_map_rgb(255,255,255), game->viewport.width*0.5, game->viewport.height*0.5, ALLEGRO_ALIGN_CENTRE, "Start game");
			DrawTextWithShadow(font, data->selected==1 ? al_map_rgb(255,255,128) : al_map_rgb(255,255,255), game->viewport.width*0.5, game->viewport.height*0.6, ALLEGRO_ALIGN_CENTRE, "Options");
			DrawTextWithShadow(font, data->selected==2 ? al_map_rgb(255,255,128) : al_map_rgb(255,255,255), game->viewport.width*0.5, game->viewport.height*0.7, ALLEGRO_ALIGN_CENTRE, "About");
			DrawTextWithShadow(font, data->selected==3 ? al_map_rgb(255,255,128) : al_map_rgb(255,255,255), game->viewport.width*0.5, game->viewport.height*0.8, ALLEGRO_ALIGN_CENTRE, "Exit");
			break;
		case MENUSTATE_OPTIONS:
			DrawTextWithShadow(font, data->selected==0 ? al_map_rgb(255,255,128) : al_map_rgb(255,255,255), game->viewport.width*0.5, game->viewport.height*0.5, ALLEGRO_ALIGN_CENTRE, "Video settings");
			DrawTextWithShadow(font, data->selected==1 ? al_map_rgb(255,255,128) : al_map_rgb(255,255,255), game->viewport.width*0.5, game->viewport.height*0.6, ALLEGRO_ALIGN_CENTRE, "Audio settings");
			DrawTextWithShadow(font, data->selected==3 ? al_map_rgb(255,255,128) : al_map_rgb(255,255,255), game->viewport.width*0.5, game->viewport.height*0.8, ALLEGRO_ALIGN_CENTRE, "Back");
			break;
		case MENUSTATE_AUDIO:
			if (game->config.music) snprintf(text, 255, "Music volume: %d0%%", game->config.music);
			else sprintf(text, "Music disabled");
			DrawTextWithShadow(font, data->selected==0 ? al_map_rgb(255,255,128) : al_map_rgb(255,255,255), game->viewport.width*0.5, game->viewport.height*0.5, ALLEGRO_ALIGN_CENTRE, text);
			if (game->config.fx) snprintf(text, 255, "Effects volume: %d0%%", game->config.fx);
			else sprintf(text, "Effects disabled");
			DrawTextWithShadow(font, data->selected==1 ? al_map_rgb(255,255,128) : al_map_rgb(255,255,255), game->viewport.width*0.5, game->viewport.height*0.6, ALLEGRO_ALIGN_CENTRE, text);
			DrawTextWithShadow(font, data->selected==3 ? al_map_rgb(255,255,128) : al_map_rgb(255,255,255), game->viewport.width*0.5, game->viewport.height*0.8, ALLEGRO_ALIGN_CENTRE, "Back");
			break;
		case MENUSTATE_ABOUT:
			DrawTextWithShadow(font, data->selected==0 ? al_map_rgb(255,255,128) : al_map_rgb(255,255,255), game->viewport.width*0.5, game->viewport.height*0.8, ALLEGRO_ALIGN_CENTRE, "Back");
			break;
		case MENUSTATE_VIDEO:
			if (data->options.fullscreen) {
				sprintf(text, "Fullscreen: yes");
				color = al_map_rgba(0,0,0,128);
			}
			else {
				sprintf(text, "Fullscreen: no");
				color = data->selected==1 ? al_map_rgb(255,255,128) : al_map_rgb(255,255,255);
			}
			DrawTextWithShadow(font, data->selected==0 ? al_map_rgb(255,255,128) : al_map_rgb(255,255,255), game->viewport.width*0.5, game->viewport.height*0.5, ALLEGRO_ALIGN_CENTRE, text);
			sprintf(text, "Resolution: %dx", data->options.resolution);
			DrawTextWithShadow(font, color, game->viewport.width*0.5, game->viewport.height*0.6, ALLEGRO_ALIGN_CENTRE, text);
			DrawTextWithShadow(font, data->selected==3 ? al_map_rgb(255,255,128) : al_map_rgb(255,255,255), game->viewport.width*0.5, game->viewport.height*0.8, ALLEGRO_ALIGN_CENTRE, "Back");
			break;
		case MENUSTATE_HIDDEN:
			break;
		case MENUSTATE_LOST:
			DrawTextWithShadow(font, al_map_rgb(255,255,255), game->viewport.width*0.5, game->viewport.height*0.5, ALLEGRO_ALIGN_CENTRE, "You lost!");
			sprintf(text, "Score: %d", data->score);
			DrawTextWithShadow(font, al_map_rgb(255,255,255), game->viewport.width*0.5, game->viewport.height*0.6, ALLEGRO_ALIGN_CENTRE, text);
			DrawTextWithShadow(font, al_map_rgb(255,255,128), game->viewport.width*0.5, game->viewport.height*0.8, ALLEGRO_ALIGN_CENTRE, "Back to menu");
			break;
		default:
			data->selected=0;
			DrawTextWithShadow(font, data->selected==0 ? al_map_rgb(255,255,128) : al_map_rgb(255,255,255), game->viewport.width*0.5, game->viewport.height*0.5, ALLEGRO_ALIGN_CENTRE, "Not implemented yet");
			break;
	}
	free(text);
}

void AnimateBadguys(struct Game *game, struct MenuResources *data, int i) {
	struct Badguy *tmp = data->badguys[i];
	while (tmp) {
		AnimateCharacter(game, tmp->character, tmp->melting ? 1 : tmp->speed * data->badguySpeed);
		tmp=tmp->next;
	}
}

void Die(struct Game *game, struct MenuResources *data) {
	data->dead = true;
	data->deathflash = 7;
	ChangeMenuState(game,data,MENUSTATE_LOST);
	al_set_sample_instance_position(data->end, 0);
	al_play_sample_instance(data->end);
	al_stop_sample_instance(data->motherlode);
	al_set_sample_instance_gain(data->music, 1.25);
}

bool CheckCollision(struct Game *game, struct MenuResources *data, struct Character *character, struct Badguy *badguy) {
	int egowidth = al_get_bitmap_width(character->bitmap);
	int egoheight = al_get_bitmap_height(character->bitmap);
	int badguywidth = al_get_bitmap_width(badguy->character->bitmap);
	int badguyheight = al_get_bitmap_height(badguy->character->bitmap);

	if (character->x < badguy->character->x + badguywidth &&
		 character->x + egowidth > badguy->character->x &&
		 character->y < badguy->character->y + badguyheight &&
		 egoheight + character->y > badguy->character->y) {
			// collision detected!
		PrintConsole(game, "COLLISION");
		return true;
	}

	return false;
}

void MoveBadguys(struct Game *game, struct MenuResources *data, int i, float dx) {
	struct Badguy *tmp = data->badguys[i];
	while (tmp) {

		if (!tmp->character->spritesheet->kill) {
			MoveCharacter(game, tmp->character, dx * tmp->speed * data->badguySpeed, 0, 0);
		}

		bool colliding = false;
		if (i == 0) {
			// check collision with ego
			if (!(data->dead)) {
				colliding = CheckCollision(game, data, data->ego, tmp);
				if (colliding) Die(game, data);
			}

			//check collision with bullets
			struct Badguy *bullets = data->badguys[3];
			while (bullets) {
				if (CheckCollision(game, data, bullets->character, tmp)) {
					colliding = true;
					data->score += 100 * tmp->speed;
					if (bullets->prev) {
						bullets->prev->next = bullets->next;
						if (bullets->next) bullets->next->prev = bullets->prev;
					} else {
						data->badguys[3] = bullets->next;
						if (bullets->next) bullets->next->prev = NULL;
					}
					struct Badguy *old = bullets;
					bullets = bullets->next;
					old->character->dead = true;
					old->prev = NULL;
					old->next = data->destroyQueue;
					if (data->destroyQueue) data->destroyQueue->prev = old;
					data->destroyQueue = old;
				} else {
					bullets = bullets->next;
				}
			}
		}


		if ((tmp->character->dead) || (tmp->character->x < -30) || (tmp->character->x > 500) || (colliding)) {
			if (tmp->prev) {
				tmp->prev->next = tmp->next;
				if (tmp->next) tmp->next->prev = tmp->prev;
			} else {
				data->badguys[i] = tmp->next;
				if (tmp->next) tmp->next->prev = NULL;
			}
			struct Badguy *old = tmp;
			tmp = tmp->next;
			old->character->dead = true;
			old->prev = NULL;
			old->next = data->destroyQueue;
			if (data->destroyQueue) data->destroyQueue->prev = old;
			data->destroyQueue = old;
		} else {
			tmp = tmp->next;
		}

	}
}

void ChangeMenuState(struct Game *game, struct MenuResources* data, enum menustate_enum state) {
	data->menustate=state;
	data->selected=0;
	PrintConsole(game, "menu state changed %d", state);
}

void CheckForEnd(struct Game *game, struct MenuResources *data) {
	int i;
	bool lost = false;
	for (i=0; i<4; i++) {
		struct Badguy *tmp = data->badguys[i];
		while (tmp) {
			if (tmp->character->x <= (139-(i*10))-10) {
				lost = true;
				break;
			}
			tmp=tmp->next;
		}
		if (lost) break;
	}

	if (lost) {

		al_stop_sample_instance(data->motherlode);
		al_play_sample_instance(data->end);
		ChangeMenuState(game, data, MENUSTATE_LOST);
	}
}

void DrawBadguys(struct Game *game, struct MenuResources *data, int i) {
	struct Badguy *tmp = data->badguys[i];
	while (tmp) {
		DrawCharacter(game, tmp->character, al_map_rgb(255,255,255), 0);
		tmp=tmp->next;
	}
}

void Gamestate_Draw(struct Game *game, struct MenuResources* data) {

	al_set_target_bitmap(al_get_backbuffer(game->display));

	al_clear_to_color(al_map_rgb(3, 213, 255));

	al_draw_bitmap(data->bg,0, 0,0);

	DrawBadguys(game, data, 1);
	DrawBadguys(game, data, 2);

	if (!(data->dead)) {
		DrawCharacter(game, data->ego, al_map_rgb(255,255,255), 0);
	}

	DrawBadguys(game, data, 0);
	DrawBadguys(game, data, 3);

	if (data->menustate != MENUSTATE_HIDDEN) {
		DrawTextWithShadow(data->font_title, al_map_rgb(255,255,255), game->viewport.width*0.5, game->viewport.height*0.15, ALLEGRO_ALIGN_CENTRE, data->menustate == MENUSTATE_LOST ? "Radio Edited!" : "Controls Control");
		DrawMenuState(game, data);
	} else {
		char score[255];
		snprintf(score, 255, "Score: %d", data->score);
		DrawTextWithShadow(data->font, al_map_rgb(255,255,255), 2, game->viewport.height - 10, ALLEGRO_ALIGN_LEFT, score);

	}

	if (data->deathflash) {
		al_draw_filled_rectangle(0, 0, 320, 180, al_map_rgb(255,255,255));
	}
}

void AddBadguy(struct Game *game, struct MenuResources* data, int i) {
	struct Badguy *n = malloc(sizeof(struct Badguy));
	n->next = NULL;
	n->prev = NULL;
	n->speed = (rand() % 3) * 0.25 + 1;
	n->melting = false;
	n->character = CreateCharacter(game, "badguy");
	n->character->spritesheets = data->badguy->spritesheets;
	n->character->shared = true;
	SetCharacterPosition(game, n->character, 320, rand() % 190 - 10, 0);
	if (i == 0) {
		SelectSpritesheet(game, n->character, "walk");
		if (n->character->y < 0) n->character->y = 0;
		if (n->character->y > 180 - 9) n->character->y = 180 - 9;
	} else if (i == 1) {
		SelectSpritesheet(game, n->character, "bigstar");
	} else if (i == 2) {
		SelectSpritesheet(game, n->character, "smallstar");
	} else {
		SelectSpritesheet(game, n->character, "bullet");
		SetCharacterPosition(game, n->character, data->ego->x + 36, data->ego->y + 8, 0);
	}

	if (data->badguys[i]) {
		struct Badguy *tmp = data->badguys[i];
		while (tmp->next) {
			tmp=tmp->next;
		}
		tmp->next = n;
		n->prev = tmp;
	} else {
		data->badguys[i] = n;
	}
}

void Fire(struct Game *game, struct MenuResources *data) {

	data->usage=10;

	al_set_sample_instance_position(data->shoot, 0);
	al_play_sample_instance(data->shoot);

	AddBadguy(game, data, 3);
}

void Gamestate_Logic(struct Game *game, struct MenuResources* data) {


	data->cloud_position-=0.1;
	if (data->cloud_position<-40) { data->cloud_position=100; PrintConsole(game, "cloud_position"); }
	AnimateCharacter(game, data->ego, 1);

	if (data->menustate == MENUSTATE_HIDDEN) {


		if (data->keys.up) {
			MoveCharacter(game, data->ego, 0, -1, 0);
		}

		if (data->keys.down) {
			MoveCharacter(game, data->ego, 0, 1, 0);
		}

		if (data->keys.left) {
			MoveCharacter(game, data->ego, -1, 0, 0);
		}

		if (data->keys.right) {
			MoveCharacter(game, data->ego, 1, 0, 0);
		}

		if (data->ego->x < 0) data->ego->x = 0;
		if (data->ego->y < 0) data->ego->y = 0;
		if (data->ego->x > 320 - 37) data->ego->x = 320 - 37;
		if (data->ego->y > 180 - 16) data->ego->y = 180 - 16;


		if ((data->keys.space) && (data->usage==0)) {
			Fire(game, data);
		}

		}

	AnimateBadguys(game, data, 0);

	MoveBadguys(game, data, 0, -0.17);
	AnimateBadguys(game, data, 1);
	AnimateBadguys(game, data, 2);
	MoveBadguys(game, data, 1, -1);
	MoveBadguys(game, data, 2, -0.5);
	MoveBadguys(game, data, 3, 1);

				data->timeTillNextBadguy--;
				if (data->timeTillNextBadguy <= 0) {
					data->timeTillNextBadguy = data->badguyRate;

		if ((data->menustate == MENUSTATE_HIDDEN) || (data->menustate == MENUSTATE_LOST)) {

				data->badguyRate -= data->badguyRate * 0.0025;
				if (data->badguyRate < 20) {
					data->badguyRate = 20;
				}

				data->badguySpeed+= 0.001;
				AddBadguy(game, data, 0);


		}

			AddBadguy(game, data, 1);
			AddBadguy(game, data, 2);
			AddBadguy(game, data, 1);
			AddBadguy(game, data, 2);



		//CheckForEnd(game, data);
	}

	if (data->usage) { data->usage--; }
	if (data->deathflash) { data->deathflash--; }

	TM_Process(data->timeline);
}

void* Gamestate_Load(struct Game *game, void (*progress)(struct Game*)) {

	struct MenuResources *data = malloc(sizeof(struct MenuResources));

	data->timeline = TM_Init(game, "main");
	(*progress)(game);

	data->options.fullscreen = game->config.fullscreen;
	data->options.fps = game->config.fps;
	data->options.width = game->config.width;
	data->options.height = game->config.height;
	data->options.resolution = game->config.width / 320;
	if (game->config.height / 180 < data->options.resolution) data->options.resolution = game->config.height / 180;

	data->bg = al_load_bitmap( GetDataFilePath(game, "bg.png") );

	data->sample = al_load_sample( GetDataFilePath(game, "bg.ogg") );
	(*progress)(game);
	data->click_sample = al_load_sample( GetDataFilePath(game, "click.flac") );
	data->end_sample = al_load_sample( GetDataFilePath(game, "end.flac") );
	data->shoot_sample = al_load_sample( GetDataFilePath(game, "shoot.flac") );
	(*progress)(game);
	data->motherlode_sample = al_load_sample( GetDataFilePath(game, "motherlode.ogg") );
	(*progress)(game);

	data->music = al_create_sample_instance(data->sample);
	al_attach_sample_instance_to_mixer(data->music, game->audio.music);
	al_set_sample_instance_playmode(data->music, ALLEGRO_PLAYMODE_LOOP);

	data->motherlode = al_create_sample_instance(data->motherlode_sample);
	al_attach_sample_instance_to_mixer(data->motherlode, game->audio.music);
	al_set_sample_instance_playmode(data->motherlode, ALLEGRO_PLAYMODE_LOOP);

	data->click = al_create_sample_instance(data->click_sample);
	al_attach_sample_instance_to_mixer(data->click, game->audio.fx);
	al_set_sample_instance_playmode(data->click, ALLEGRO_PLAYMODE_ONCE);

	data->shoot = al_create_sample_instance(data->shoot_sample);
	al_attach_sample_instance_to_mixer(data->shoot, game->audio.fx);
	al_set_sample_instance_playmode(data->shoot, ALLEGRO_PLAYMODE_ONCE);

	data->end = al_create_sample_instance(data->end_sample);
	al_attach_sample_instance_to_mixer(data->end, game->audio.fx);
	al_set_sample_instance_playmode(data->end, ALLEGRO_PLAYMODE_ONCE);

	if (!data->click_sample){
		fprintf(stderr, "Audio clip sample not loaded!\n" );
		exit(-1);
	}

	data->font_title = al_load_ttf_font(GetDataFilePath(game, "fonts/MonkeyIsland.ttf"),game->viewport.height*0.16,0 );
	data->font = al_load_ttf_font(GetDataFilePath(game, "fonts/MonkeyIsland.ttf"),game->viewport.height*0.05,0 );
	(*progress)(game);

	data->ego = CreateCharacter(game, "ego");
	RegisterSpritesheet(game, data->ego, "stand");
	LoadSpritesheets(game, data->ego);

	data->badguy = CreateCharacter(game, "badguy");
	RegisterSpritesheet(game, data->badguy, "walk");
	RegisterSpritesheet(game, data->badguy, "bigstar");
	RegisterSpritesheet(game, data->badguy, "smallstar");
	RegisterSpritesheet(game, data->badguy, "bullet");
	LoadSpritesheets(game, data->badguy);
	(*progress)(game);

	al_set_target_backbuffer(game->display);
	return data;
}

void DestroyBadguys(struct Game *game, struct MenuResources* data, int i) {
	struct Badguy *tmp = data->badguys[i];
	if (!tmp) {
		tmp = data->destroyQueue;
		data->destroyQueue = NULL;
	}
	while (tmp) {
		DestroyCharacter(game, tmp->character);
		struct Badguy *old = tmp;
		tmp = tmp->next;
		free(old);
		if ((!tmp) && (data->destroyQueue)) {
			tmp = data->destroyQueue;
			data->destroyQueue = NULL;
		}
	}
	data->badguys[i] = NULL;
}

void Gamestate_Stop(struct Game *game, struct MenuResources* data) {
	al_stop_sample_instance(data->music);

	int i;
	for (i=0; i<4; i++) {
		DestroyBadguys(game, data, i);
	}
}

void Gamestate_Unload(struct Game *game, struct MenuResources* data) {
	al_destroy_bitmap(data->bg);
	al_destroy_font(data->font_title);
	al_destroy_font(data->font);
	al_destroy_sample_instance(data->music);
	al_destroy_sample_instance(data->click);
	al_destroy_sample_instance(data->end);
	al_destroy_sample_instance(data->motherlode);
	al_destroy_sample(data->sample);
	al_destroy_sample(data->motherlode_sample);
	al_destroy_sample(data->click_sample);
	al_destroy_sample(data->end_sample);
	DestroyCharacter(game, data->ego);
	DestroyCharacter(game, data->badguy);
	TM_Destroy(data->timeline);
}


void StartGame(struct Game *game, struct MenuResources *data) {
	TM_CleanQueue(data->timeline);
	TM_CleanBackgroundQueue(data->timeline);
	ChangeMenuState(game,data,MENUSTATE_HIDDEN);
	al_play_sample_instance(data->motherlode);
	al_set_sample_instance_gain(data->music, 1);
	data->dead = false;
}



void Gamestate_Start(struct Game *game, struct MenuResources* data) {
	data->deathflash = 0;
	data->cloud_position = 100;
	SetCharacterPosition(game, data->ego, 22, 107, 0);

	data->dead = false;
	data->score = 0;

	data->keys.left = false;
	data->keys.right = false;
	data->keys.up = false;
	data->keys.down = false;
	data->keys.space = false;

	data->badguySpeed = 1.2;

	data->usage = 0;

	SelectSpritesheet(game, data->ego, "stand");
	ChangeMenuState(game,data,MENUSTATE_MAIN);
	al_play_sample_instance(data->music);
	al_set_sample_instance_gain(data->music, 1.25);

	data->badguys[0] = NULL;
	data->badguys[1] = NULL;
	data->badguys[2] = NULL;
	data->badguys[3] = NULL;
	data->destroyQueue = NULL;

	data->badguyRate = 100;
	data->timeTillNextBadguy = 0;

	int i;
	for (i=0; i<300; i++) {
		// fill it with stars
		Gamestate_Logic(game, data);
	}
}

void Gamestate_ProcessEvent(struct Game *game, struct MenuResources* data, ALLEGRO_EVENT *ev) {
	TM_HandleEvent(data->timeline, ev);

	if ((data->menustate == MENUSTATE_ABOUT) && (ev->type == ALLEGRO_EVENT_KEY_DOWN)) {
		ChangeMenuState(game, data, MENUSTATE_MAIN);
		return;
	}

	if (data->menustate == MENUSTATE_HIDDEN) {

		if (ev->type == ALLEGRO_EVENT_KEY_DOWN) {

			switch (ev->keyboard.keycode) {
				case ALLEGRO_KEY_UP:
					data->keys.up = true;
					break;
				case ALLEGRO_KEY_DOWN:
					data->keys.down = true;
					break;
				case ALLEGRO_KEY_LEFT:
					data->keys.left = true;
					break;
				case ALLEGRO_KEY_RIGHT:
					data->keys.right = true;
					break;
				case ALLEGRO_KEY_SPACE:
					data->keys.space = true;
					break;
				case ALLEGRO_KEY_ESCAPE:
					Die(game, data);
					break;
			}
		} else if (ev->type == ALLEGRO_EVENT_KEY_UP) {
			switch (ev->keyboard.keycode) {
				case ALLEGRO_KEY_UP:
					data->keys.up = false;
					break;
				case ALLEGRO_KEY_DOWN:
					data->keys.down = false;
					break;
				case ALLEGRO_KEY_LEFT:
					data->keys.left = false;
					break;
				case ALLEGRO_KEY_RIGHT:
					data->keys.right = false;
					break;
				case ALLEGRO_KEY_SPACE:
					data->keys.space = false;
					break;
			}
		}

		return;
	}

	if (ev->type != ALLEGRO_EVENT_KEY_DOWN) return;

	if (ev->keyboard.keycode==ALLEGRO_KEY_UP) {
		data->selected--;
		if ((data->selected == 2) && ((data->menustate==MENUSTATE_VIDEO) || (data->menustate==MENUSTATE_OPTIONS) || (data->menustate==MENUSTATE_AUDIO))) {
			data->selected --;
		}
		if ((data->menustate==MENUSTATE_VIDEO) && (data->selected==1) && (data->options.fullscreen)) data->selected--;
		al_play_sample_instance(data->click);
	} else if (ev->keyboard.keycode==ALLEGRO_KEY_DOWN) {
		data->selected++;
		if ((data->menustate==MENUSTATE_VIDEO) && (data->selected==1) && (data->options.fullscreen)) data->selected++;
		if ((data->selected == 2) && ((data->menustate==MENUSTATE_VIDEO) || (data->menustate==MENUSTATE_OPTIONS) || (data->menustate==MENUSTATE_AUDIO))) {
			data->selected ++;
		}


		al_play_sample_instance(data->click);
	}
	int pos;
	if (ev->keyboard.keycode==ALLEGRO_KEY_ENTER) {
		char *text;
		al_play_sample_instance(data->click);
		switch (data->menustate) {
			case MENUSTATE_MAIN:
				switch (data->selected) {
					case 0:

						StartGame(game, data);
						break;
					case 1:
						ChangeMenuState(game,data,MENUSTATE_OPTIONS);
						break;
					case 2:
						ChangeMenuState(game,data,MENUSTATE_ABOUT);
						break;
					case 3:
						UnloadGamestate(game, "menu");
						break;
				}
				break;
			case MENUSTATE_AUDIO:
				text = malloc(255*sizeof(char));
				switch (data->selected) {
					case 0:
						game->config.music--;
						if (game->config.music<0) game->config.music=10;
						snprintf(text, 255, "%d", game->config.music);
						SetConfigOption(game, "SuperDerpy", "music", text);
						al_set_mixer_gain(game->audio.music, game->config.music/10.0);
						break;
					case 1:
						game->config.fx--;
						if (game->config.fx<0) game->config.fx=10;
						snprintf(text, 255, "%d", game->config.fx);
						SetConfigOption(game, "SuperDerpy", "fx", text);
						al_set_mixer_gain(game->audio.fx, game->config.fx/10.0);
						break;
					case 2:
						game->config.voice--;
						if (game->config.voice<0) game->config.voice=10;
						snprintf(text, 255, "%d", game->config.voice);
						SetConfigOption(game, "SuperDerpy", "voice", text);
						al_set_mixer_gain(game->audio.voice, game->config.voice/10.0);
						break;
					case 3:
						ChangeMenuState(game,data,MENUSTATE_OPTIONS);
						break;
				}
				free(text);
				break;
			case MENUSTATE_OPTIONS:
				switch (data->selected) {
					case 0:
						ChangeMenuState(game,data,MENUSTATE_VIDEO);
						break;
					case 1:
						ChangeMenuState(game,data,MENUSTATE_AUDIO);
						break;
					case 3:
						ChangeMenuState(game,data,MENUSTATE_MAIN);
						break;
					default:
						break;
				}
				break;
			case MENUSTATE_VIDEO:
				switch (data->selected) {
					case 0:
						data->options.fullscreen = !data->options.fullscreen;
						if (data->options.fullscreen)
							SetConfigOption(game, "SuperDerpy", "fullscreen", "1");
						else
							SetConfigOption(game, "SuperDerpy", "fullscreen", "0");
						al_set_display_flag(game->display, ALLEGRO_FULLSCREEN_WINDOW, data->options.fullscreen);
						SetupViewport(game);
						PrintConsole(game, "Fullscreen toggled");
						break;
					case 1:
						data->options.resolution++;

						int max = 0, i = 0;

						for (i=0; i < al_get_num_video_adapters(); i++) {
							ALLEGRO_MONITOR_INFO aminfo;
							al_get_monitor_info(i , &aminfo);
							int desktop_width = aminfo.x2 - aminfo.x1 + 1;
							int desktop_height = aminfo.y2 - aminfo.y1 + 1;
							int localmax = desktop_width / 320;
							if (desktop_height / 180 < localmax) localmax = desktop_height / 180;
							if (localmax > max) max = localmax;
						}


						if (data->options.resolution > max) data->options.resolution = 1;
						text = malloc(255*sizeof(char));
						snprintf(text, 255, "%d", data->options.resolution * 320);
						SetConfigOption(game, "SuperDerpy", "width", text);
						snprintf(text, 255, "%d", data->options.resolution * 180);
						SetConfigOption(game, "SuperDerpy", "height", text);
						free(text);
						al_resize_display(game->display, data->options.resolution * 320, data->options.resolution * 180);

						if ((al_get_display_width(game->display) < (data->options.resolution * 320)) || (al_get_display_height(game->display) < (data->options.resolution * 180))) {
							SetConfigOption(game, "SuperDerpy", "width", "320");
							SetConfigOption(game, "SuperDerpy", "height", "180");
							data->options.resolution = 1;
							al_resize_display(game->display, 320, 180);
						}

						SetupViewport(game);
						PrintConsole(game, "Resolution changed");
						break;
					case 3:
						ChangeMenuState(game,data,MENUSTATE_OPTIONS);
						break;
					default:
						break;
				}
				break;
			case MENUSTATE_ABOUT:
				break;
			case MENUSTATE_LOST:
				pos = al_get_sample_instance_position(data->music);
				Gamestate_Stop(game,data);
				Gamestate_Start(game,data);
				al_set_sample_instance_position(data->music, pos);
				break;
			default:
				UnloadGamestate(game, "menu");
				return;
				break;
		}
	} else if (ev->keyboard.keycode==ALLEGRO_KEY_ESCAPE) {
		switch (data->menustate) {
			case MENUSTATE_OPTIONS:
				ChangeMenuState(game,data,MENUSTATE_MAIN);
				break;
			case MENUSTATE_ABOUT:
				ChangeMenuState(game,data,MENUSTATE_MAIN);
				break;
			case MENUSTATE_HIDDEN:
				Die(game, data);
				break;
			case MENUSTATE_VIDEO:
				ChangeMenuState(game,data,MENUSTATE_OPTIONS);
				break;
			case MENUSTATE_AUDIO:
				ChangeMenuState(game,data,MENUSTATE_OPTIONS);
				break;
			case MENUSTATE_LOST:
				pos = al_get_sample_instance_position(data->music);
				Gamestate_Stop(game,data);
				Gamestate_Start(game,data);
				al_set_sample_instance_position(data->music, pos);
				break;
			default:
				UnloadGamestate(game, "menu");
				return;
		}
	}

	if (data->selected==-1) data->selected=3;
	if (data->selected==4) data->selected=0;
	return;
}

void Gamestate_Pause(struct Game *game, struct MenuResources* data) {}
void Gamestate_Resume(struct Game *game, struct MenuResources* data) {}
void Gamestate_Reload(struct Game *game, struct MenuResources* data) {}
