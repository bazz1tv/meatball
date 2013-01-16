#pragma once

#include "Globals.h"

class TextObjectEngine
{
public:
	TextObjectEngine(cFont *active_handle)
	{
		font_engine_handle = active_handle;
	}


	TextObject* givemeaTextObject();
	TextObject* givemeaTextObject(string txt, TTF_Font *font);
	TextObject* givemeaTextObject(TTF_Font *font);
	TextObject* givemeaTextObject(string txt);
	TextObject* givemeaTextObject(int x, int y);
	TextObject* givemeaTextObject(int x, int y, string txt);

	void setFontEngine(cFont *engine);


	cFont *font_engine_handle;
	ObjectManager<TextObject> objects;
};

