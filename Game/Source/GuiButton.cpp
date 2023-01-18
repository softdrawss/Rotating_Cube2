#include "GuiButton.h"
#include "Render.h"
#include "App.h"
#include "Audio.h"
#include "Log.h"
#include "Scene.h"

GuiButton::GuiButton(uint32 id, SDL_Rect bounds, const char* text) : GuiControl(GuiControlType::BUTTON, id)
{
	this->bounds = bounds;
	this->text = text;

	canClick = true;
	drawBasic = false;

	//audioFxId = app->audio->LoadFx("Assets/Audio/Fx/retro-video-game-coin-pickup-38299.ogg");
}

GuiButton::~GuiButton()
{

}

bool GuiButton::Update(float dt)
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
				LOG("Change state from %d to %d",previousState,state);
				//app->audio->PlayFx(audioFxId);
			}

			if (app->input->GetMouseButtonDown(SDL_BUTTON_LEFT) == KeyState::KEY_REPEAT) {
				state = GuiControlState::PRESSED;
				if (button == GuiButtontype::PUSH_Q) {
					//Quaternion values in std::string to float
					std::stof(app->scene->q[0]->input);
					std::stof(app->scene->q[1]->input);
					std::stof(app->scene->q[2]->input);
					std::stof(app->scene->q[3]->input);
				}
				if (button == GuiButtontype::PUSH_E) {
					//Euler axis and angle values in std::string to float
					std::stof(app->scene->e[0]->input);
					std::stof(app->scene->e[1]->input);
					std::stof(app->scene->e[2]->input);
					std::stof(app->scene->e[3]->input);
				}
				if (button == GuiButtontype::PUSH_A) {
					//Euler angles values in std::string to float
					std::stof(app->scene->a[0]->input);
					std::stof(app->scene->a[1]->input);
					std::stof(app->scene->a[2]->input);
					std::stof(app->scene->a[3]->input);
				}
				if (button == GuiButtontype::PUSH_V) {
					//Rotation vector in std::string to float
					std::stof(app->scene->v[0]->input);
					std::stof(app->scene->v[1]->input);
					std::stof(app->scene->v[2]->input);
					std::stof(app->scene->v[3]->input);
				}
				if (button == GuiButtontype::RESET) {
					app->scene->Reset();
				}
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




bool GuiButton::Draw(Render* render)
{
	//L15: DONE 4: Draw the button according the GuiControl State

	switch (state)
	{
	case GuiControlState::DISABLED:
		render->DrawRectangle(bounds, 200, 200, 200, 200, true, false);
		break;
	case GuiControlState::NORMAL:
		render->DrawRectangle(bounds, 0, 0, 255, 200, true, false);
		break;
	case GuiControlState::FOCUSED:
		render->DrawRectangle(bounds, 153, 153, 255, 200, true, false);
		break;
	case GuiControlState::PRESSED:
		render->DrawRectangle(bounds, 0, 255, 0, 200, true, false);
		break;
	}

	app->render->DrawText(text.GetString(), bounds.x+10, bounds.y+5, bounds.w-20, bounds.h-10, {255,255,255});

	return false;
}