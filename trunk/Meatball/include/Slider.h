#ifndef __SLIDER_H__
#define __SLIDER_H__

#include "Globals.h"

struct GraphicsObject;

class Slider 
{
public:
	Slider(int x, int y, int width, int range_min, int range_max);
	void DoRatio();
	void Draw();

	int width;
	int panel_x,panel_y;

	int target_min, target_max;
	int slider_min, slider_max;

	int slider_range;
	int target_range;

	double ratio;

	SDL_Rect rPanel, rAdjuster;
	int adjuster_x, adjuster_y;



};


#endif