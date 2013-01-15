#include "Slider.h"

void Slider::Draw()
{
	// Get Graphics going
	// void FillRect(SDL_Surface *surface, int x, int y, int w, int h, Uint32 color);
	// void FillRectAlpha(SDL_Surface *surface, int x, int y, int w, int h, Uint32 color);
	
	FillRect(Screen, panel_x, panel_y, width, 10, SDL_MapRGB(Screen->format, 50,50,50));
	FillRect(Screen, adjuster_x,adjuster_y, 5, 15, SDL_MapRGB(Screen->format,0,255,0));
}

Slider::Slider(int x, int y, int width, int range_min, int range_max)
{
	this->panel_x = x;
	this->panel_y = y;

	this->width = width;

	adjuster_x = x+(width/2); // Set the adjuster in the middle of the panel
	adjuster_y = y-2;			// guess

	target_range = range_max - range_min;
	slider_range = width;

	// calculate ratio
	DoRatio();


	

}

void Slider::DoRatio()
{
	if (target_range > slider_range)
		 ratio = slider_range / target_range;
	else ratio = target_range / slider_range;
}
