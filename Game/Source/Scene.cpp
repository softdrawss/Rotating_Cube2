#include "App.h"
#include "Input.h"
#include "Textures.h"
#include "Audio.h"
#include "Render.h"
#include "Window.h"
#include "Scene.h"
#include "GuiManager.h"
#include <eigen/Eigen/Core>
#include <eigen/Eigen/Dense>
#include <math.h>
#include "Defs.h"
#include "Log.h"
#include <string>

Scene::Scene() : Module()
{
	name.Create("scene");
}

// Destructor
Scene::~Scene()
{}

// Called before render is available
bool Scene::Awake(pugi::xml_node& config)
{
	LOG("Loading Scene");
	bool ret = true;

	// iterate all objects in the scene
	// Check https://pugixml.org/docs/quickstart.html#access
	
	return ret;
}

// Called before the first frame
bool Scene::Start()
{
	//UI
	cubeTexture = app->tex->Load("Assets/cube.png");
	UI = app->tex->Load("Assets/UI.png");

	// Buttons
	uint w, h;
	app->win->GetWindowSize(w, h);
	//button1 = (GuiButton*)app->guiManager->CreateGuiControl(GuiControlType::BUTTON, 1, "BUENAS", { (int)w / 2,(int)h / 2 - 30,100,20 }, this);
	button2 = (GuiButton*)app->guiManager->CreateGuiControl(GuiControlType::BUTTON, 2, "RESET", { 450 ,624,120,40 }, this);
	button2->button = GuiButtontype::RESET;

	//Animation Camera views
	down.PushBack({ 0, 0, 100, 90 });
	left.PushBack({ 100, 0, 100, 90 });
	right.PushBack({ 100 *2, 0, 100, 90 });
	back.PushBack({ 100 *3, 0, 100, 90 });
	front.PushBack({ 100 *4, 0, 100, 90 });
	up.PushBack({ 100 *5, 0, 100, 90 });
	currentAnim = &front;

	p1 << 100, 100, 100;
	p2 << 200, 100, 100;
	p3 << 200, 200, 100;
	p4 << 100, 200, 100;

	p5 << 100, 100, 200;
	p6 << 200, 100, 200;
	p7 << 200, 200, 200;
	p8 << 100, 200, 200;


	return true;
}

// Called each loop iteration
bool Scene::PreUpdate()
{
	return true;
}

// Called each loop iteration
bool Scene::Update(float dt)
{

#pragma region CUBE
	roll = 0;
	pitch = 0;
	yaw = 0;

	center << p1(0) + p2(0) + p3(0) + p4(0) + p5(0) + p6(0) + p7(0) + p8(0),
		p1(1) + p2(1) + p3(1) + p4(1) + p5(1) + p6(1) + p7(1) + p8(1),
		p1(2) + p2(2) + p3(2) + p4(2) + p5(2) + p6(2) + p7(2) + p8(2);

	center(0) = center(0) / 8;
	center(1) = center(1) / 8;
	center(2) = center(2) / 8;


	if (app->input->GetKey(SDL_SCANCODE_W) == KEY_REPEAT) {
		roll = -0.02;
	}
	if (app->input->GetKey(SDL_SCANCODE_S) == KEY_REPEAT) {
		roll = 0.02;
	}
	if (app->input->GetKey(SDL_SCANCODE_A) == KEY_REPEAT) {
		pitch = 0.02;
	}
	if (app->input->GetKey(SDL_SCANCODE_D) == KEY_REPEAT) {
		pitch = -0.02;
	}
	if (app->input->GetKey(SDL_SCANCODE_Q) == KEY_REPEAT) {
		yaw = 0.02;
	}
	if (app->input->GetKey(SDL_SCANCODE_E) == KEY_REPEAT) {
		yaw = -0.02;
	}

	if (app->input->GetKey(SDL_SCANCODE_R) == KEY_DOWN) {
		p1 << 100, 100, 100;
		p2 << 200, 100, 100;
		p3 << 200, 200, 100;
		p4 << 100, 200, 100;
		p5 << 100, 100, 200;
		p6 << 200, 100, 200;
		p7 << 200, 200, 200;
		p8 << 100, 200, 200;
		LOG("Rotation reset");
	}

	p1 -= center;
	p1 = Rotate(p1, roll, pitch, yaw);
	p1 += center;

	p2 -= center;
	p2 = Rotate(p2, roll, pitch, yaw);
	p2 += center;

	p3 -= center;
	p3 = Rotate(p3, roll, pitch, yaw);
	p3 += center;

	p4 -= center;
	p4 = Rotate(p4, roll, pitch, yaw);
	p4 += center;

	p5 -= center;
	p5 = Rotate(p5, roll, pitch, yaw);
	p5 += center;

	p6 -= center;
	p6 = Rotate(p6, roll, pitch, yaw);
	p6 += center;

	p7 -= center;
	p7 = Rotate(p7, roll, pitch, yaw);
	p7 += center;

	p8 -= center;
	p8 = Rotate(p8, roll, pitch, yaw);
	p8 += center;

	//SDL_RenderDrawPoint(app->render->renderer, p1(0), p1(1));
	//SDL_RenderDrawPoint(app->render->renderer, p2(0), p2(1));
	//SDL_RenderDrawPoint(app->render->renderer, p3(0), p3(1));
	//SDL_RenderDrawPoint(app->render->renderer, p4(0), p4(1));
	//SDL_RenderDrawPoint(app->render->renderer, p5(0), p5(1));
	//SDL_RenderDrawPoint(app->render->renderer, p6(0), p6(1));
	//SDL_RenderDrawPoint(app->render->renderer, p7(0), p7(1));
	//SDL_RenderDrawPoint(app->render->renderer, p8(0), p8(1));

	app->render->DrawLine(p1(0), p1(1), p2(0), p2(1), 250, 250, 250, 250);
	app->render->DrawLine(p1(0), p1(1), p4(0), p4(1), 0, 250, 0, 250);
	app->render->DrawLine(p1(0), p1(1), p5(0), p5(1), 250, 250, 250, 250);
	app->render->DrawLine(p2(0), p2(1), p3(0), p3(1), 250, 250, 250, 250);
	app->render->DrawLine(p2(0), p2(1), p6(0), p6(1), 250, 250, 250, 250);
	app->render->DrawLine(p3(0), p3(1), p7(0), p7(1), 250, 250, 250, 250);
	app->render->DrawLine(p3(0), p3(1), p4(0), p4(1), 0, 0, 250, 250);
	app->render->DrawLine(p4(0), p4(1), p8(0), p8(1), 250, 0, 0, 250);
	app->render->DrawLine(p5(0), p5(1), p6(0), p6(1), 250, 250, 250, 250);
	app->render->DrawLine(p5(0), p5(1), p8(0), p8(1), 250, 250, 250, 250);
	app->render->DrawLine(p6(0), p6(1), p7(0), p7(1), 250, 250, 250, 250);
	app->render->DrawLine(p7(0), p7(1), p8(0), p8(1), 250, 250, 250, 250);
#pragma	endregion
	
	//Controls camera View
	if (app->input->GetKey(SDL_SCANCODE_W) == KEY_UP)
		currentAnim = &up;
	
	if (app->input->GetKey(SDL_SCANCODE_S) == KEY_REPEAT)
		currentAnim = &down;
	
	if (app->input->GetKey(SDL_SCANCODE_A) == KEY_REPEAT)
		currentAnim = &left;
	
	if (app->input->GetKey(SDL_SCANCODE_D) == KEY_REPEAT)
		currentAnim = &right;

	if (app->input->GetKey(SDL_SCANCODE_F) == KEY_REPEAT)
		currentAnim = &front;

	if (app->input->GetKey(SDL_SCANCODE_R) == KEY_REPEAT)
		currentAnim = &back;


	// L08: DONE 3: Test World to map method

	/*
	int mouseX, mouseY;
	app->input->GetMousePosition(mouseX, mouseY);

	iPoint mouseTile = iPoint(0, 0); 

	if (app->map->mapData.type == MapTypes::MAPTYPE_ISOMETRIC) {
		mouseTile = app->map->WorldToMap(mouseX - app->render->camera.x - app->map->mapData.tileWidth / 2,
												mouseY - app->render->camera.y - app->map->mapData.tileHeight / 2);
	}
	if (app->map->mapData.type == MapTypes::MAPTYPE_ORTHOGONAL) {
		mouseTile = app->map->WorldToMap(mouseX - app->render->camera.x,
												mouseY - app->render->camera.y);
	}

	//Convert again the tile coordinates to world coordinates to render the texture of the tile
	iPoint highlightedTileWorld = app->map->MapToWorld(mouseTile.x, mouseTile.y);
	app->render->DrawTexture(mouseTileTex, highlightedTileWorld.x, highlightedTileWorld.y);

	//Test compute path function
	if (app->input->GetMouseButtonDown(SDL_BUTTON_LEFT) == KEY_DOWN)
	{
		if (originSelected == true)
		{
			app->pathfinding->CreatePath(origin, mouseTile);
			originSelected = false;
		}
		else
		{
			origin = mouseTile;
			originSelected = true;
			app->pathfinding->ClearLastPath();
		}
	}
	*/

	app->render->DrawTexture(UI, 0, 0);
	SDL_Rect rect2 = currentAnim->GetCurrentFrame();
	app->render->DrawTexture(cubeTexture, 50, 50, &rect2);
	currentAnim->Update();
	app->guiManager->Draw();

	//Draw coordenades of each point
	std::string string;
	string = "p1 = " + std::to_string(p1(0)) + ", " + std::to_string(p1(1)) + ", " + std::to_string(p1(2));
	app->render->DrawText(string.c_str(), 50, 200, 420, 20, { 250, 250, 250 });
	string = "p2 = " + std::to_string(p2(0)) + ", " + std::to_string(p2(1)) + ", " + std::to_string(p2(2));
	app->render->DrawText(string.c_str(), 50, 240, 420, 20, { 250, 250, 250 });
	string = "p3 = " + std::to_string(p3(0)) + ", " + std::to_string(p3(1)) + ", " + std::to_string(p3(2));
	app->render->DrawText(string.c_str(), 50, 280, 420, 20, { 250, 250, 250 });
	string = "p4 = " + std::to_string(p4(0)) + ", " + std::to_string(p4(1)) + ", " + std::to_string(p4(2));
	app->render->DrawText(string.c_str(), 50, 320, 420, 20, { 250, 250, 250 });
	string = "p5 = " + std::to_string(p5(0)) + ", " + std::to_string(p5(1)) + ", " + std::to_string(p5(2));
	app->render->DrawText(string.c_str(), 50, 360, 420, 20, { 250, 250, 250 });
	string = "p6 = " + std::to_string(p6(0)) + ", " + std::to_string(p6(1)) + ", " + std::to_string(p6(2));
	app->render->DrawText(string.c_str(), 50, 400, 420, 20, { 250, 250, 250 });
	string = "p7 = " + std::to_string(p7(0)) + ", " + std::to_string(p7(1)) + ", " + std::to_string(p7(2));
	app->render->DrawText(string.c_str(), 50, 440, 420, 20, { 250, 250, 250 });
	string = "p8 = " + std::to_string(p8(0)) + ", " + std::to_string(p8(1)) + ", " + std::to_string(p8(2));
	app->render->DrawText(string.c_str(), 50, 480, 420, 20, { 250, 250, 250 });

	return true;
}

// Called each loop iteration
bool Scene::PostUpdate()
{
	bool ret = true;

	if(app->input->GetKey(SDL_SCANCODE_ESCAPE) == KEY_DOWN)
		ret = false;

	return ret;
}

bool Scene::OnGuiMouseClickEvent(GuiControl* control)
{
	// L15: DONE 5: Implement the OnGuiMouseClickEvent method
	LOG("Event by %d ",control->id);

	switch (control->id)
	{
	case 1:
		LOG("Button 1 click");
		break;
	case 2:
		LOG("Button 2 click");
		break;
	}

	return true;

}

Eigen::Vector3f Scene::Rotate(Eigen::Vector3f point, float x, float y, float z) {
	float rad = 0;

	rad = x;
	point(1) = cos(rad) * point(1) - sin(rad) * point(2);
	point(2) = sin(rad) * point(1) + cos(rad) * point(2);

	rad = y;
	point(0) = cos(rad) * point(0) + sin(rad) * point(2);
	point(2) = -sin(rad) * point(0) + cos(rad) * point(2);

	rad = z;
	point(0) = cos(rad) * point(0) - sin(rad) * point(1);
	point(1) = sin(rad) * point(0) + cos(rad) * point(1);

	return point;

}

// Called before quitting
bool Scene::CleanUp()
{
	LOG("Freeing scene");
	

	return true;
}


void Scene::Reset() {

}