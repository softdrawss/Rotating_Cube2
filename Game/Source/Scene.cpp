#include "App.h"
#include "Input.h"
#include "Textures.h"
#include "Audio.h"
#include "Render.h"
#include "Window.h"
#include "Scene.h"
//#include "EntityManager.h"
#include "Map.h"
#include "PathFinding.h"
#include "GuiManager.h"

#include "Defs.h"
#include "Log.h"

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
	button1 = (GuiButton*)app->guiManager->CreateGuiControl(GuiControlType::BUTTON, 1, "BUENAS", { (int)w / 2,(int)h / 2 - 30,100,20 }, this);
	button2 = (GuiButton*)app->guiManager->CreateGuiControl(GuiControlType::BUTTON, 2, "R", { (int)w / 2 - 50,(int)h / 2,200,20 }, this);
	

	//Animation Camera views
	down.PushBack({ 0, 0, 100, 90 });
	left.PushBack({ 100, 0, 100, 90 });
	right.PushBack({ 100 *2, 0, 100, 90 });
	back.PushBack({ 100 *3, 0, 100, 90 });
	front.PushBack({ 100 *4, 0, 100, 90 });
	up.PushBack({ 100 *5, 0, 100, 90 });
	currentAnim = &front;

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
	
	//app->render->DrawLine(50, 50, 50, 300, 250, 250, 250, 250);
	// L03: DONE 3: Request App to Load / Save when pressing the keys F5 (save) / F6 (load)
	//if (app->input->GetKey(SDL_SCANCODE_F5) == KEY_DOWN)
	//	app->SaveGameRequest();
	//
	//if (app->input->GetKey(SDL_SCANCODE_F6) == KEY_DOWN)
	//	app->LoadGameRequest();
	//
	
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

	//L15: Draw GUI
	app->guiManager->Draw();

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

// Called before quitting
bool Scene::CleanUp()
{
	LOG("Freeing scene");

	return true;
}
