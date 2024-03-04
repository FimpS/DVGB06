#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include "objects.h"
#include "global.h"

void gfx_init(SDL_Texture **textures, SDL_Surface *sur, SDL_Renderer *renderer)
{
	for(int i = 0; i < 2; i++)
	{
		sur = IMG_Load("assets/Untitled.png");
		textures[i] = SDL_CreateTextureFromSurface(renderer, sur);
	}
}

void render_pObject_deathrattle(SDL_Renderer *renderer, SDL_Texture* tex, SDL_Rect R, SDL_Rect r)
{
	SDL_RenderCopy(renderer, tex, &R, &r);
}

void process_symmetric_animation(SDL_Renderer *renderer, SDL_Texture *tex, SDL_Rect sR, SDL_Rect dR, struct pObject *pObject)
{
	const double conv = 57.29577;
	
	if(pObject->anim_timer < 4)
	{
		SDL_RenderCopyEx(renderer, tex, &sR, &dR, pObject->theta*conv, NULL, 0);
	}
	else if(pObject->anim_timer >= 4 && pObject->anim_timer < 8)
	{
		sR.x += 16;
		SDL_RenderCopyEx(renderer, tex, &sR, &dR, pObject->theta*conv, NULL, 0);
	}
	else if(pObject->anim_timer >= 8 && pObject->anim_timer < 12)
	{
		sR.x += 32;
		SDL_RenderCopyEx(renderer, tex, &sR, &dR, pObject->theta*conv, NULL, 0);
	}
	else if(pObject->anim_timer >= 12 && pObject->anim_timer < 16)
	{
		sR.x += 48;
		SDL_RenderCopyEx(renderer, tex, &sR, &dR, pObject->theta*conv, NULL, 0);
	}
	else if(pObject->anim_timer >= 16)
	{
		SDL_RenderCopyEx(renderer, tex, &sR, &dR, pObject->theta*conv, NULL, 0);
		pObject->anim_timer = -1;	
	}
	pObject->anim_timer ++;
}

void render_hpbar(SDL_Renderer *renderer, struct player* player, struct cam *cam, double *reduce)
{
	//TODO good enough for now but should be altererd prob
	//Prob health should be integer so +1/-1 will fix everything
	for(int i = 0; i < 6; i++)
	{
		if((*reduce <= 320 * player->health / player->maxhealth))
		{
			(*reduce) += 1.0;
		}
	}
	for(int i = 0; i < 6; i++)
	{
		if((*reduce > 320 * player->health / player->maxhealth))
			(*reduce) -= 1.0;
	}
	

	SDL_Rect curr_health = {(0.5) * TILE_LENGTH, (0.5) * TILE_LENGTH, *reduce, TILE_LENGTH / 2};
	SDL_Rect full_health = {0.5 * TILE_LENGTH, 0.5 * TILE_LENGTH, 8 * TILE_LENGTH, TILE_LENGTH / 2};
	SDL_Rect surround = {0.25 * TILE_LENGTH, 0.35 * TILE_LENGTH, 10.75 * 32, 32};

	SDL_SetRenderDrawColor(renderer, 0x60, 0x60, 0x60, 0xFF);
	SDL_RenderFillRect(renderer, &surround);
	SDL_SetRenderDrawColor(renderer, 0x00, 0x00, 0x00, 0xFF);
	SDL_RenderFillRect(renderer, &full_health);
	SDL_SetRenderDrawColor(renderer, 0xFF, 0x00, 0x00, 0xFF);
	SDL_RenderFillRect(renderer, &curr_health);
}

void load_textures()
{
	return;
}
