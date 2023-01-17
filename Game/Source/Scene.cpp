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
	
	return ret;
}

// Called before the first frame
bool Scene::Start()
{
	//UI
	cubeTexture = app->tex->Load("Assets/cube.png");
	UI = app->tex->Load("Assets/UI.png");

	// Buttons
	// Reset
	button2 = (GuiButton*)app->guiManager->CreateGuiControl(GuiControlType::BUTTON, 2, "RESET", { 450,624,120,40 }, this);
	button2->button = GuiButtontype::RESET;

	// Push
	p[0] = (GuiButton*)app->guiManager->CreateGuiControl(GuiControlType::BUTTON, 2, "PUSH", { 1130,51,100,40 }, this);
	p[1] = (GuiButton*)app->guiManager->CreateGuiControl(GuiControlType::BUTTON, 2, "PUSH", { 1130,173,100,40 }, this);
	p[2] = (GuiButton*)app->guiManager->CreateGuiControl(GuiControlType::BUTTON, 2, "PUSH", { 1130,295,100,40 }, this);
	p[3] = (GuiButton*)app->guiManager->CreateGuiControl(GuiControlType::BUTTON, 2, "PUSH", { 1130,417,100,40 }, this);

	p[0]->button = GuiButtontype::PUSH;
	p[1]->button = GuiButtontype::PUSH;
	p[2]->button = GuiButtontype::PUSH;
	p[3]->button = GuiButtontype::PUSH;

	// Quaternion Input
	q[0] = (InpButton*)app->guiManager->CreateGuiControl(GuiControlType::INPUTBOX, 2, " ", { 707 ,100,90,30 }, this);
	q[1] = (InpButton*)app->guiManager->CreateGuiControl(GuiControlType::INPUTBOX, 2, " ", { 815 ,100,90,30 }, this);
	q[2] = (InpButton*)app->guiManager->CreateGuiControl(GuiControlType::INPUTBOX, 2, " ", { 923 ,100,90,30 }, this);
	q[3] = (InpButton*)app->guiManager->CreateGuiControl(GuiControlType::INPUTBOX, 2, " ", { 1031 ,100,90,30 }, this);

	e[0] = (InpButton*)app->guiManager->CreateGuiControl(GuiControlType::INPUTBOX, 2, " ", { 743 ,223,90,30 }, this);
	e[1] = (InpButton*)app->guiManager->CreateGuiControl(GuiControlType::INPUTBOX, 2, " ", { 851,223,90,30 }, this);
	e[2] = (InpButton*)app->guiManager->CreateGuiControl(GuiControlType::INPUTBOX, 2, " ", { 959,223,90,30 }, this);
	e[3] = (InpButton*)app->guiManager->CreateGuiControl(GuiControlType::INPUTBOX, 2, " ", { 980 ,180,90,30 }, this);

	a[0] = (InpButton*)app->guiManager->CreateGuiControl(GuiControlType::INPUTBOX, 2, " ", { 670 ,350,90,30 }, this);
	a[1] = (InpButton*)app->guiManager->CreateGuiControl(GuiControlType::INPUTBOX, 2, " ", { 812 ,350,90,30 }, this);
	a[2] = (InpButton*)app->guiManager->CreateGuiControl(GuiControlType::INPUTBOX, 2, " ", { 956 ,350,90,30 }, this);

	v[0] = (InpButton*)app->guiManager->CreateGuiControl(GuiControlType::INPUTBOX, 2, " ", { 707 ,468,90,30 }, this);
	v[1] = (InpButton*)app->guiManager->CreateGuiControl(GuiControlType::INPUTBOX, 2, " ", { 815 ,468,90,30 }, this);
	v[2] = (InpButton*)app->guiManager->CreateGuiControl(GuiControlType::INPUTBOX, 2, " ", { 923 ,468,90,30 }, this);

	//Animation Camera views
	down.PushBack({ 0, 0, 100, 90 });
	left.PushBack({ 100, 0, 100, 90 });
	right.PushBack({ 100 *2, 0, 100, 90 });
	back.PushBack({ 100 *3, 0, 100, 90 });
	front.PushBack({ 100 *4, 0, 100, 90 });
	up.PushBack({ 100 *5, 0, 100, 90 });
	currentAnim = &front;

	Reset();

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
	//Camera rotation
#pragma region CAMERA_ROTATION
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
		roll = -0.02;
	}
	if (app->input->GetKey(SDL_SCANCODE_Q) == KEY_REPEAT) {
		yaw = 0.02;
	}
	if (app->input->GetKey(SDL_SCANCODE_E) == KEY_REPEAT) {
		roll = -0.02;
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
#pragma	endregion CAMERA_ROTATION
	
#pragma region UI
	//Draw UI
	app->render->DrawTexture(UI, 0, 0);
	//SDL_Rect rect2 = currentAnim->GetCurrentFrame();
	//app->render->DrawTexture(cubeTexture, 50, 50, &rect2);
	//currentAnim->Update();
	app->guiManager->Draw();

	//Draw coordenades of each point
	std::string string;
	string = "p1 = " + std::to_string(p1(0)) + ", " + std::to_string(p1(1)) + ", " + std::to_string(p1(2));
	app->render->DrawText(string.c_str(), 50, 305 + 200, 390, 20, { 204, 204, 204 });
	string = "p2 = " + std::to_string(p2(0)) + ", " + std::to_string(p2(1)) + ", " + std::to_string(p2(2));
	app->render->DrawText(string.c_str(), 50, 305 + 220, 390, 20, { 204, 204, 204 });
	string = "p3 = " + std::to_string(p3(0)) + ", " + std::to_string(p3(1)) + ", " + std::to_string(p3(2));
	app->render->DrawText(string.c_str(), 50, 305 + 240, 390, 20, { 204, 204, 204 });
	string = "p4 = " + std::to_string(p4(0)) + ", " + std::to_string(p4(1)) + ", " + std::to_string(p4(2));
	app->render->DrawText(string.c_str(), 50, 305 + 260, 390, 20, { 204, 204, 204 });
	string = "p5 = " + std::to_string(p5(0)) + ", " + std::to_string(p5(1)) + ", " + std::to_string(p5(2));
	app->render->DrawText(string.c_str(), 50, 305 + 280, 390, 20, { 204, 204, 204 });
	string = "p6 = " + std::to_string(p6(0)) + ", " + std::to_string(p6(1)) + ", " + std::to_string(p6(2));
	app->render->DrawText(string.c_str(), 50, 305 + 300, 390, 20, { 204, 204, 204 });
	string = "p7 = " + std::to_string(p7(0)) + ", " + std::to_string(p7(1)) + ", " + std::to_string(p7(2));
	app->render->DrawText(string.c_str(), 50, 305 + 320, 390, 20, { 204, 204, 204 });
	string = "p8 = " + std::to_string(p8(0)) + ", " + std::to_string(p8(1)) + ", " + std::to_string(p8(2));
	app->render->DrawText(string.c_str(), 50, 305 + 340, 390, 20, { 204, 204, 204 });

	//Draw matrix
	string = std::to_string(p1(2));
	app->render->DrawText(string.c_str(), 730, 570, 120, 25, { 204, 204, 204 });
	string = std::to_string(p2(2));
	app->render->DrawText(string.c_str(), 730, 600, 120, 25, { 204, 204, 204 });
	string = std::to_string(p3(2));
	app->render->DrawText(string.c_str(), 730, 630, 120, 25, { 204, 204, 204 });
	string = std::to_string(p4(2));
	app->render->DrawText(string.c_str(), 870, 570, 120, 25, { 204, 204, 204 });
	string = std::to_string(p5(2));
	app->render->DrawText(string.c_str(), 870, 600, 120, 25, { 204, 204, 204 });
	string = std::to_string(p6(2));
	app->render->DrawText(string.c_str(), 870, 630, 120, 25, { 204, 204, 204 });
	string = std::to_string(p7(2));
	app->render->DrawText(string.c_str(), 1010, 570, 120, 25, { 204, 204, 204 });
	string = std::to_string(p8(2));
	app->render->DrawText(string.c_str(), 1010, 600, 120, 25, { 204, 204, 204 });
	string = std::to_string(p1(2));
	app->render->DrawText(string.c_str(), 1010, 630, 120, 25, { 204, 204, 204 });

#pragma endregion UI

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


void Scene::Reset() {
	p1 << 100, 100, 100;
	p2 << 300, 100, 100;
	p3 << 300, 300, 100;
	p4 << 100, 300, 100;
	p5 << 100, 100, 300;
	p6 << 300, 100, 300;
	p7 << 300, 300, 300;
	p8 << 100, 300, 300;


}