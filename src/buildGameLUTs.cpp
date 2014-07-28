#include "common.h"
#include "../gfx/titleScreen.h"
#include "../gfx/bg.h"
#include "../gfx/gfx.h"

unsigned short *image_entries[NB_IMAGES];

void buildGameLUTs()
{
	image_entries[image_LUT_player_ship_light] = image_player_ship_light;
	image_entries[image_LUT_player_ship_shadow] = image_player_ship_shadow;
	image_entries[image_LUT_player_ship_polarityswitch_0_light] = image_player_ship_polarityswitch_0_light;
	image_entries[image_LUT_player_ship_polarityswitch_0_shadow] = image_player_ship_polarityswitch_0_shadow;
	image_entries[image_LUT_player_ship_polarityswitch_1_light] = image_player_ship_polarityswitch_1_light;
	image_entries[image_LUT_player_ship_polarityswitch_1_shadow] = image_player_ship_polarityswitch_1_shadow;
	image_entries[image_LUT_player_bullet_light] = image_player_bullet_light;
	image_entries[image_LUT_player_bullet_shadow] = image_player_bullet_shadow;
	image_entries[image_LUT_enemy_bullet_0_light] = image_enemy_bullet_0_light;
	image_entries[image_LUT_enemy_bullet_0_shadow] = image_enemy_bullet_0_shadow;
	image_entries[image_LUT_enemy_bullet_1_light] = image_enemy_bullet_1_light;
	image_entries[image_LUT_enemy_bullet_1_shadow] = image_enemy_bullet_1_shadow;
	image_entries[image_LUT_enemy_ship_0_light] = image_enemy_ship_0_light;
	image_entries[image_LUT_enemy_ship_0_shadow] = image_enemy_ship_0_shadow;
	image_entries[image_LUT_enemy_ship_1_light] = image_enemy_ship_1_light;
	image_entries[image_LUT_enemy_ship_1_shadow] = image_enemy_ship_1_shadow;
	image_entries[image_LUT_enemy_ship_2_light] = image_enemy_ship_2_light;
	image_entries[image_LUT_enemy_ship_2_shadow] = image_enemy_ship_2_shadow;
	image_entries[image_LUT_enemy_ship_3_light] = image_enemy_ship_3_light;
	image_entries[image_LUT_enemy_ship_3_shadow] = image_enemy_ship_3_shadow;
	image_entries[image_LUT_chain_hit_light] = image_chain_hit_light;
	image_entries[image_LUT_chain_hit_shadow] = image_chain_hit_shadow;
	image_entries[image_LUT_explosion_light_0] = image_explosion_light_0;
	image_entries[image_LUT_explosion_light_1] = image_explosion_light_1;
	image_entries[image_LUT_explosion_light_2] = image_explosion_light_2;
	image_entries[image_LUT_explosion_light_3] = image_explosion_light_3;
	image_entries[image_LUT_explosion_light_4] = image_explosion_light_4;
	image_entries[image_LUT_explosion_light_5] = image_explosion_light_5;
	image_entries[image_LUT_explosion_shadow_0] = image_explosion_shadow_0;
	image_entries[image_LUT_explosion_shadow_1] = image_explosion_shadow_1;
	image_entries[image_LUT_explosion_shadow_2] = image_explosion_shadow_2;
	image_entries[image_LUT_explosion_shadow_3] = image_explosion_shadow_3;
	image_entries[image_LUT_explosion_shadow_4] = image_explosion_shadow_4;
	image_entries[image_LUT_explosion_shadow_5] = image_explosion_shadow_5;
	image_entries[image_LUT_background] = image_background;
	image_entries[image_LUT_titleScreen] = image_titleScreen;
}

void freeGameLUTs()
{
	
}