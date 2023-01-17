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
#include <eigen/Eigen/Core>
#include <eigen/Eigen/Dense>
#include <math.h>
#include "Defs.h"
#include "Log.h"

#define PI 3.141592654

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
	roll = 0;
	pitch = 0;
	yaw = 0;

	center << p1(0) + p2(0) + p3(0) + p4(0) + p5(0) + p6(0) + p7(0) + p8(0),
		p1(1) + p2(1) + p3(1) + p4(1) + p5(1) + p6(1) + p7(1) + p8(1),
		p1(2) + p2(2) + p3(2) + p4(2) + p5(2) + p6(2) + p7(2) + p8(2);

	center(0) = center(0) / 8;
	center(1) = center(1) / 8;
	center(2) = center(2) / 8;


	if (app->input->GetKey(SDL_SCANCODE_W) == KEY_DOWN) {
		roll = -PI/2;
	}
	if (app->input->GetKey(SDL_SCANCODE_S) == KEY_REPEAT) {
		roll = 0.02;
	}
	if (app->input->GetKey(SDL_SCANCODE_A) == KEY_REPEAT) {
		pitch = 0.02;
	}
	if (app->input->GetKey(SDL_SCANCODE_D) == KEY_DOWN) {
		pitch = -PI/2;
	}
	if (app->input->GetKey(SDL_SCANCODE_Q) == KEY_REPEAT) {
		yaw = 0.02;
	}
	if (app->input->GetKey(SDL_SCANCODE_E) == KEY_DOWN) {
		yaw = -PI/2;
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

Eigen::Matrix3d Scene::FixMatrix0s(Eigen::Matrix3d mat) {
	for (int i = 0; i < 9; i++) {
		if (abs(mat(i)) < 0.00001) {
			mat(i) = 0;
		}
	}
	return mat;
}

Eigen::Vector3d Scene::RotationVectorFromAngleAndAxis(double angle, Eigen::Vector3d u) {
	Eigen::Vector<double, 3> r;
	r << angle * u(0), angle* u(1), angle* u(2);
	return r;
}

Eigen::Vector4d Scene::AngleAndAxisFromRotationVector(Eigen::Vector3d r) {
	Eigen::Vector<double, 4> result;
	result << r.norm(), r(0) / r.norm(), r(1) / r.norm(), r(2) / r.norm();
	return result;
}

Eigen::Vector4d Scene::EulerAndAxisFromQuaternion(Eigen::Vector4d q) {

	Eigen::Vector<double, 4>result;
	result(0) = 2 * (acos(q(0)));
	result(1) = q(1) / (sin(result(0) / 2));
	result(2) = q(2) / (sin(result(0) / 2));
	result(3) = q(3) / (sin(result(0) / 2));

	return result;
}

Eigen::Matrix3d Scene::CreateEulerAnglesRotation(double x, double y, double z) {

	Eigen::Matrix<double, 3, 3> result;
	result(0, 0) = cos(y) * cos(z);
	result(0, 1) = cos(z) * sin(y) * sin(x) - (cos(x) * sin(z));
	result(0, 2) = cos(x) * cos(z) * sin(y) + (sin(z) * sin(x));
	result(1, 0) = cos(y) * sin(z);
	result(1, 1) = sin(z) * sin(y) * sin(x) + (cos(x) * cos(z));
	result(1, 2) = sin(y) * sin(z) * cos(x) - (cos(z) * sin(x));
	result(2, 0) = -sin(y);
	result(2, 1) = cos(y) * sin(x);
	result(2, 2) = cos(y) * cos(x);

	result = FixMatrix0s(result);

	return result;
}

Eigen::Vector4d Scene::QuaternionFromEulerAndAxis(double angle, Eigen::Vector3d u) {

	Eigen::Vector<double, 4> q;
	q << cos(angle / 2), u(0)* sin(angle / 2), u(1)* sin(angle / 2), u(2)* sin(angle / 2);

	return q;
}

Eigen::Vector3d Scene::QuaternionMultiplication(Eigen::Vector3d v, Eigen::Vector4d q) {

	Eigen::Matrix <double, 3, 3> identity;
	identity << 1, 0, 0, 0, 1, 0, 0, 0, 1;

	Eigen::Matrix <double, 3, 3> qx;
	qx << 0, -q(3), q(2), q(3), 0, -q(1), -q(2), q(1), 0;

	Eigen::Vector<double, 3>qv;
	qv << q(1), q(2), q(3);

	Eigen::Matrix<double, 3, 3>rotationQ;
	rotationQ = (q(0) * q(0) - qv.transpose() * qv) * identity + 2 * qv * qv.transpose() + 2 * q(0) * qx;

	return rotationQ * v;

}

// Called before quitting
bool Scene::CleanUp()
{
	LOG("Freeing scene");

	return true;
}
