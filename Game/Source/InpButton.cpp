#include "InpButton.h"
#include "Render.h"
#include "App.h"
#include "Audio.h"
#include "Log.h"
#include "SString.h"

InpButton::InpButton(uint32 id, SDL_Rect bounds, const char* text) : GuiControl(GuiControlType::INPUTBOX, id)
{
	this->bounds = bounds;
	this->text = text;

	canClick = true;
	drawBasic = false;

	audioFxId = app->audio->LoadFx("Assets/Audio/Fx/retro-video-game-coin-pickup-38299.ogg");
}

InpButton::~InpButton()
{

}

bool InpButton::Update(float dt)
{
	if (state != GuiControlState::DISABLED)
	{
		// L15: DONE 3: Update the state of the GUiButton according to the mouse position
		app->input->GetMousePosition(mouseX, mouseY);

		GuiControlState previousState = state;

		// I'm inside the limitis of the button
		if (mouseX >= bounds.x && mouseX <= bounds.x + bounds.w &&
			mouseY >= bounds.y && mouseY <= bounds.y + bounds.h) {

			state = GuiControlState::FOCUSED;
			if (previousState != state) {
				LOG("Change state from %d to %d", previousState, state);
				//app->audio->PlayFx(audioFxId);
			}

			if (app->input->GetMouseButtonDown(SDL_BUTTON_LEFT) == KeyState::KEY_REPEAT) {
				state = GuiControlState::PRESSED;
			}

			//
			if (app->input->GetMouseButtonDown(SDL_BUTTON_LEFT) == KeyState::KEY_UP) {
				NotifyObserver();
			}
		}
		else {
			state = GuiControlState::NORMAL;
		}
	}

	return false;
}




bool InpButton::Draw(Render* render)
{
	//L15: DONE 4: Draw the button according the GuiControl State

	switch (state)
	{
	case GuiControlState::DISABLED:
		render->DrawRectangle(bounds, 200, 200, 200, 255, true, false);
		break;
	case GuiControlState::NORMAL:
		render->DrawRectangle(bounds, 0, 0, 255, 255, true, false);
		break;
	case GuiControlState::FOCUSED:
		render->DrawRectangle(bounds, 0, 0, 20, 255, true, false);
		break;
	case GuiControlState::PRESSED:
		render->DrawRectangle(bounds, 0, 255, 0, 255, true, false);
		break;
	}

	app->render->DrawText(text.GetString(), bounds.x, bounds.y, bounds.w, bounds.h, { 255,255,255 });

	return false;
}

/*
* Constructor with text, font, font color, and renderer.
*
* @param t: The text in the text box.
* @param font: The font we are using.
* @param color: The color of the text.
* @param renderer: The renderer.
* @param maxLength: The maximum width of the text box before it wraps around.
*/

//TextBox(string t, TTF_Font* font, SDL_Color color, SDL_Renderer* renderer, int maxLength)
//{
//	char CharArray[2000]; // Create a char array
//	strcpy_s(CharArray, t.c_str()); // Convert the string into a char array for the surface function.
//
//	Surface = TTF_RenderText_Blended_Wrapped(font, CharArray, color, maxLength); // Make into a surface.
//	Texture = SDL_CreateTextureFromSurface(renderer, Surface); // Turn the surface into a texture.
//	TTF_SizeText(font, CharArray, &w, &h); // Size the texture so it renders the text correctly.
//}