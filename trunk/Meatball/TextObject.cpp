#include "TextObject.h"


/// Update's the surface and internal font and text members
void TextObject::writeText(string text, TTF_Font *font)
{
	this->font = font;
	this->text = text;
	surface = font_engine_handle->CreateText(text.c_str(), font);
}

/// Set the active font engine handle
void TextObject::SetFontEngineHandle(cFont *handle)
{
	this->font_engine_handle = handle;
}

/// Update the surface with the object's internal text and font
void TextObject::writeText()
{
	surface = font_engine_handle->CreateText(text.c_str(),font);
}

/// Write text with a different font
/// This won't change the text object's default font
void TextObject::writeText(TTF_Font *different_font)
{
	surface = font_engine_handle->CreateText(text.c_str(), different_font);
}