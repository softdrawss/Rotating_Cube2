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
#include <Eigen/Core>
#include <Eigen/Dense>
#include <math.h>
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
	//for (pugi::xml_node itemNode = config.child("item"); itemNode; itemNode = itemNode.next_sibling("item"))
	//{
	//	Item* item = (Item*)app->entityManager->CreateEntity(EntityType::ITEM);
	//	item->parameters = itemNode;
	//}
	//
	////L02: DONE 3: Instantiate the player using the entity manager
	//if (config.child("player")) {
	//	player = (Player*)app->entityManager->CreateEntity(EntityType::PLAYER);
	//	player->parameters = config.child("player");
	//}
	
	return ret;
}

// Called before the first frame
bool Scene::Start()
{
	
	// L15: DONE 2: Declare a GUI Button and create it using the GuiManager
	uint w, h;
	app->win->GetWindowSize(w, h);
	button1 = (GuiButton*)app->guiManager->CreateGuiControl(GuiControlType::BUTTON, 1, "BUENAS", { (int)w / 2,(int)h / 2 - 30,100,20 }, this);
	button2 = (GuiButton*)app->guiManager->CreateGuiControl(GuiControlType::BUTTON, 2, "R", { (int)w / 2 - 50,(int)h / 2,200,20 }, this);



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
	app->render->DrawLine(50, 50, 50, 300, 250, 250, 250, 250);
	// L03: DONE 3: Request App to Load / Save when pressing the keys F5 (save) / F6 (load)
	//if (app->input->GetKey(SDL_SCANCODE_F5) == KEY_DOWN)
	//	app->SaveGameRequest();
	//
	//if (app->input->GetKey(SDL_SCANCODE_F6) == KEY_DOWN)
	//	app->LoadGameRequest();
	//
	//// L14: DONE 4: Make the camera movement independent of framerate
	//float speed = 0.2 * dt;
	//if (app->input->GetKey(SDL_SCANCODE_UP) == KEY_REPEAT)
	//	app->render->camera.y += ceil(speed);
	//
	//if (app->input->GetKey(SDL_SCANCODE_DOWN) == KEY_REPEAT)
	//	app->render->camera.y -= ceil(speed);
	//
	//if (app->input->GetKey(SDL_SCANCODE_LEFT) == KEY_REPEAT)
	//	app->render->camera.x += ceil(speed);
	//
	//if (app->input->GetKey(SDL_SCANCODE_RIGHT) == KEY_REPEAT)
	//	app->render->camera.x -= ceil(speed);

	// Draw map
	//app->map->Draw();

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

	// L12: Get the latest calculated path and draw
	const DynArray<iPoint>* path = app->pathfinding->GetLastPath();
	for (uint i = 0; i < path->Count(); ++i)
	{
		iPoint pos = app->map->MapToWorld(path->At(i)->x, path->At(i)->y);
		app->render->DrawTexture(mouseTileTex, pos.x, pos.y);
	}

	// L12: Debug pathfinding
	iPoint originScreen = app->map->MapToWorld(origin.x, origin.y);
	app->render->DrawTexture(originTex, originScreen.x, originScreen.y);
	*/

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
