#pragma once

#include "Globals.h"

/// To write Text requires the cFont class to be activated.
class TextObject : public GraphicsObject
{
public:
	string text;
	cFont *font_engine_handle;
	TTF_Font *font;

	void writeText(string text, TTF_Font *font);
	void writeText();
	void writeText(TTF_Font *font);
	void SetFontEngineHandle(cFont *handle);
};