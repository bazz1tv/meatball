#include "TextObjectEngine.h"

void TextObjectEngine::setFontEngine(cFont *engine)
{
	font_engine_handle = engine;
}

TextObject* TextObjectEngine::givemeaTextObject()
{
	TextObject *tmp;
	tmp = objects.add();
	tmp->SetFontEngineHandle(font_engine_handle);
	return tmp;
}

TextObject* TextObjectEngine::givemeaTextObject(string txt)
{
	TextObject *tmp;
	tmp = objects.add();
	tmp->SetFontEngineHandle(font_engine_handle);
	tmp->text = txt;
	return tmp;
}

TextObject* TextObjectEngine::givemeaTextObject(int x, int y)
{
	TextObject *tmp;
	tmp = objects.add();
	tmp->SetFontEngineHandle(font_engine_handle);
	tmp->rect.x = x;
	tmp->rect.y = y;
	return tmp;
}

TextObject* TextObjectEngine::givemeaTextObject(int x, int y, string txt)
{
	TextObject *tmp;
	tmp = objects.add();
	tmp->SetFontEngineHandle(font_engine_handle);
	tmp->text = txt;
	tmp->rect.x = x;
	tmp->rect.y = y;
	return tmp;
}


TextObject* TextObjectEngine::givemeaTextObject(string txt, TTF_Font *font)
{
	TextObject *tmp;
	tmp = objects.add();
	tmp->SetFontEngineHandle(font_engine_handle);
	tmp->text = txt;
	tmp->font = font;

	return tmp;
}
