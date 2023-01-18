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
#include "GuiButton.h"
#include "SString.h"

#define PI 3.14159265358979323846264338327950288

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

	isButtonPressed = false;

	// Buttons
	// Reset
	button2 = (GuiButton*)app->guiManager->CreateGuiControl(GuiControlType::BUTTON, 2, "RESET", { 450,624,120,40 }, this);
	button2->button = GuiButtontype::RESET;

	// Push
	p[0] = (GuiButton*)app->guiManager->CreateGuiControl(GuiControlType::BUTTON, 2, "PUSH", { 1130,51,100,40 }, this);
	p[1] = (GuiButton*)app->guiManager->CreateGuiControl(GuiControlType::BUTTON, 2, "PUSH", { 1130,173,100,40 }, this);
	p[2] = (GuiButton*)app->guiManager->CreateGuiControl(GuiControlType::BUTTON, 2, "PUSH", { 1130,295,100,40 }, this);
	p[3] = (GuiButton*)app->guiManager->CreateGuiControl(GuiControlType::BUTTON, 2, "PUSH", { 1130,417,100,40 }, this);

	p[0]->button = GuiButtontype::PUSH_Q;
	p[1]->button = GuiButtontype::PUSH_E;
	p[2]->button = GuiButtontype::PUSH_A;
	p[3]->button = GuiButtontype::PUSH_V;

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

	rqtn << 1, 0, 0, 0;
	rmatrix << 1, 0, 0,
			   0, 1, 0,
			   0, 0, 1;
	angleAndAxis << 0, 0, 0, 0;
	rVector << 0, 0, 0;

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

	if (!isButtonPressed) {

		qtn << 1, 0, 0, 0;

		center << p1(0) + p2(0) + p3(0) + p4(0) + p5(0) + p6(0) + p7(0) + p8(0),
			p1(1) + p2(1) + p3(1) + p4(1) + p5(1) + p6(1) + p7(1) + p8(1),
			p1(2) + p2(2) + p3(2) + p4(2) + p5(2) + p6(2) + p7(2) + p8(2);

		center(0) = center(0) / 8;
		center(1) = center(1) / 8;
		center(2) = center(2) / 8;

		float aux = 0.99979997999;
		float aux2 = 0.99959991996;
		float aux3 = 0.99939981989;

		if (app->input->GetKey(SDL_SCANCODE_W) == KEY_REPEAT) {
			qtn(1) = -0.02;
			qtn(0) = aux;
			if (app->input->GetKey(SDL_SCANCODE_A) == KEY_REPEAT || app->input->GetKey(SDL_SCANCODE_D) == KEY_REPEAT
				|| app->input->GetKey(SDL_SCANCODE_E) == KEY_REPEAT || app->input->GetKey(SDL_SCANCODE_Q) == KEY_REPEAT) {
				qtn(0) = aux2;
			}
			if ((app->input->GetKey(SDL_SCANCODE_A) == KEY_REPEAT || app->input->GetKey(SDL_SCANCODE_D) == KEY_REPEAT)
				&& (app->input->GetKey(SDL_SCANCODE_E) == KEY_REPEAT || app->input->GetKey(SDL_SCANCODE_Q) == KEY_REPEAT)) {
				qtn(0) = aux3;
			}

		}
		if (app->input->GetKey(SDL_SCANCODE_S) == KEY_REPEAT) {
			qtn(1) = 0.02;
			qtn(0) = aux;
			if (app->input->GetKey(SDL_SCANCODE_A) == KEY_REPEAT || app->input->GetKey(SDL_SCANCODE_D) == KEY_REPEAT
				|| app->input->GetKey(SDL_SCANCODE_E) == KEY_REPEAT || app->input->GetKey(SDL_SCANCODE_Q) == KEY_REPEAT) {
				qtn(0) = aux2;
			}
			if ((app->input->GetKey(SDL_SCANCODE_A) == KEY_REPEAT || app->input->GetKey(SDL_SCANCODE_D) == KEY_REPEAT)
				&& (app->input->GetKey(SDL_SCANCODE_E) == KEY_REPEAT || app->input->GetKey(SDL_SCANCODE_Q) == KEY_REPEAT)) {
				qtn(0) = aux3;
			}
		}
		if (app->input->GetKey(SDL_SCANCODE_A) == KEY_REPEAT) {
			qtn(2) = 0.02;
			qtn(0) = aux;
			if (app->input->GetKey(SDL_SCANCODE_W) == KEY_REPEAT || app->input->GetKey(SDL_SCANCODE_S) == KEY_REPEAT
				|| app->input->GetKey(SDL_SCANCODE_E) == KEY_REPEAT || app->input->GetKey(SDL_SCANCODE_Q) == KEY_REPEAT) {
				qtn(0) = aux2;
			}
			if ((app->input->GetKey(SDL_SCANCODE_W) == KEY_REPEAT || app->input->GetKey(SDL_SCANCODE_S) == KEY_REPEAT)
				&& (app->input->GetKey(SDL_SCANCODE_E) == KEY_REPEAT || app->input->GetKey(SDL_SCANCODE_Q) == KEY_REPEAT)) {
				qtn(0) = aux3;
			}
		}
		if (app->input->GetKey(SDL_SCANCODE_D) == KEY_REPEAT) {
			qtn(2) = -0.02;
			qtn(0) = aux;
			if (app->input->GetKey(SDL_SCANCODE_W) == KEY_REPEAT || app->input->GetKey(SDL_SCANCODE_S) == KEY_REPEAT
				|| app->input->GetKey(SDL_SCANCODE_E) == KEY_REPEAT || app->input->GetKey(SDL_SCANCODE_Q) == KEY_REPEAT) {
				qtn(0) = aux2;
			}
			if ((app->input->GetKey(SDL_SCANCODE_W) == KEY_REPEAT || app->input->GetKey(SDL_SCANCODE_S) == KEY_REPEAT)
				&& (app->input->GetKey(SDL_SCANCODE_E) == KEY_REPEAT || app->input->GetKey(SDL_SCANCODE_Q) == KEY_REPEAT)) {
				qtn(0) = aux3;
			}
		}
		if (app->input->GetKey(SDL_SCANCODE_Q) == KEY_REPEAT) {
			qtn(3) = 0.02;
			qtn(0) = aux;
			if (app->input->GetKey(SDL_SCANCODE_A) == KEY_REPEAT || app->input->GetKey(SDL_SCANCODE_D) == KEY_REPEAT
				|| app->input->GetKey(SDL_SCANCODE_W) == KEY_REPEAT || app->input->GetKey(SDL_SCANCODE_S) == KEY_REPEAT) {
				qtn(0) = aux2;
			}
			if ((app->input->GetKey(SDL_SCANCODE_A) == KEY_REPEAT || app->input->GetKey(SDL_SCANCODE_D) == KEY_REPEAT)
				&& (app->input->GetKey(SDL_SCANCODE_W) == KEY_REPEAT || app->input->GetKey(SDL_SCANCODE_S) == KEY_REPEAT)) {
				qtn(0) = aux3;
			}
		}
		if (app->input->GetKey(SDL_SCANCODE_E) == KEY_REPEAT) {
			qtn(3) = -0.02;
			qtn(0) = aux;
			if (app->input->GetKey(SDL_SCANCODE_A) == KEY_REPEAT || app->input->GetKey(SDL_SCANCODE_D) == KEY_REPEAT
				|| app->input->GetKey(SDL_SCANCODE_W) == KEY_REPEAT || app->input->GetKey(SDL_SCANCODE_S) == KEY_REPEAT) {
				qtn(0) = aux2;
			}
			if ((app->input->GetKey(SDL_SCANCODE_A) == KEY_REPEAT || app->input->GetKey(SDL_SCANCODE_D) == KEY_REPEAT)
				&& (app->input->GetKey(SDL_SCANCODE_W) == KEY_REPEAT || app->input->GetKey(SDL_SCANCODE_S) == KEY_REPEAT)) {
				qtn(0) = aux3;
			}
		}

		//Calculating Axis and Angle
		Eigen::Vector<float, 3>dircopia = director.normalized();

		angleAndAxis(0) = dircopia(0);
		angleAndAxis(1) = dircopia(1);
		angleAndAxis(2) = dircopia(2);
		angleAndAxis(3) = Angle2Vectors(director, directorref);

		//Calculating everything else with the axis and angle
		rmatrix = CreateRotationMatrix(angleAndAxis(3), { angleAndAxis(0), angleAndAxis(1), angleAndAxis(2) });
		rVector = RotationVectorFromAngleAndAxis(angleAndAxis(3), { angleAndAxis(0), angleAndAxis(1), angleAndAxis(2) });
		rqtn = QuaternionFromEulerAndAxis(angleAndAxis(3), { angleAndAxis(0), angleAndAxis(1), angleAndAxis(2) });
		eangles = EulerAnglesFromRotationMatrix(rmatrix);

		//Angle and Axis print
		for (int i = 0; i < 4; i++) {
			e[i]->input = std::to_string(angleAndAxis(i));
		}

		//Rotation Vector print
		for (int i = 0; i < 3; i++) {
			v[i]->input = std::to_string(rVector(i));
		}

		//Quaternion print
		for (int i = 0; i < 4; i++) {
			q[i]->input = std::to_string(rqtn(i));
		}

		//Euler angles print
		for (int i = 0; i < 3; i++) {
			a[i]->input = std::to_string(eangles(i));
		}

		p1 -= center;
		p1 = RotateQ(p1, qtn);
		p1 += center;

		p2 -= center;
		p2 = RotateQ(p2, qtn);
		p2 += center;

		p3 -= center;
		p3 = RotateQ(p3, qtn);
		p3 += center;

		p4 -= center;
		p4 = RotateQ(p4, qtn);
		p4 += center;

		p5 -= center;
		p5 = RotateQ(p5, qtn);
		p5 += center;

		p6 -= center;
		p6 = RotateQ(p6, qtn);
		p6 += center;

		p7 -= center;
		p7 = RotateQ(p7, qtn);
		p7 += center;

		p8 -= center;
		p8 = RotateQ(p8, qtn);
		p8 += center;

		director = RotateQ(director, qtn);

	}

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
	string = "ct = " + std::to_string(director(0)) + ", " + std::to_string(director(1)) + ", " + std::to_string(director(2));
	app->render->DrawText(string.c_str(), 50, 305 + 180, 390, 20, { 204, 204, 204 });
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
	string = std::to_string(rmatrix(0));
	app->render->DrawText(string.c_str(), 730, 570, 120, 25, { 204, 204, 204 });
	string = std::to_string(rmatrix(1));
	app->render->DrawText(string.c_str(), 730, 600, 120, 25, { 204, 204, 204 });
	string = std::to_string(rmatrix(2));
	app->render->DrawText(string.c_str(), 730, 630, 120, 25, { 204, 204, 204 });
	string = std::to_string(rmatrix(3));
	app->render->DrawText(string.c_str(), 870, 570, 120, 25, { 204, 204, 204 });
	string = std::to_string(rmatrix(4));
	app->render->DrawText(string.c_str(), 870, 600, 120, 25, { 204, 204, 204 });
	string = std::to_string(rmatrix(5));
	app->render->DrawText(string.c_str(), 870, 630, 120, 25, { 204, 204, 204 });
	string = std::to_string(rmatrix(6));
	app->render->DrawText(string.c_str(), 1010, 570, 120, 25, { 204, 204, 204 });
	string = std::to_string(rmatrix(7));
	app->render->DrawText(string.c_str(), 1010, 600, 120, 25, { 204, 204, 204 });
	string = std::to_string(rmatrix(8));
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

Eigen::Matrix3f Scene::FixMatrix0s(Eigen::Matrix3f mat) {
	for (int i = 0; i < 9; i++) {
		if (abs(mat(i)) < 0.00001) {
			mat(i) = 0;
		}
	}
	return mat;
}

bool Scene::CompareFMatrices(Eigen::MatrixXf m1, Eigen::MatrixXf m2) {

	for (int i = 0; i < 9; i++) {
		if (!(abs(m1(i)) <= abs(m2(i)) + 0.0025f
			&& abs(m1(i)) >= abs(m2(i)) - 0.0025f
			&& !(m1(i) > 0 && m2(i) < 0) || (m1(i) < 0 && m2(i) > 0))) {
			return false;
		}
	}
	return true;
}

Eigen::Matrix3f Scene::CreateRotationMatrix(float angle, Eigen::Vector3f u) {

	Eigen::Matrix <float, 3, 3> identity;
	identity << 1, 0, 0, 0, 1, 0, 0, 0, 1;

	Eigen::Matrix <float, 3, 3> ux;
	ux << 0, -u(2), u(1), u(2), 0, -u(0), -u(1), u(0), 0;

	Eigen::Matrix <float, 3, 3> result;
	result = identity * cos(angle) + (1 - cos(angle)) * (u * u.transpose()) + ux * sin(angle);

	result = FixMatrix0s(result);

	return result;
}

Eigen::Vector3f Scene::RotationVectorFromAngleAndAxis(float angle, Eigen::Vector3f u) {
	Eigen::Vector<float, 3> r;
	r << angle * u(0), angle* u(1), angle* u(2);
	return r;
}

Eigen::Vector4f Scene::AngleAndAxisFromRotationVector(Eigen::Vector3f r) {
	Eigen::Vector<float, 4> result;
	result << r.norm(), r(0) / r.norm(), r(1) / r.norm(), r(2) / r.norm();
	return result;
}

Eigen::Vector4f Scene::EulerAndAxisFromQuaternion(Eigen::Vector4f q) {

	Eigen::Vector<float, 4>result;
	result(0) = 2 * (acos(q(0)));
	result(1) = q(1) / (sin(result(0) / 2));
	result(2) = q(2) / (sin(result(0) / 2));
	result(3) = q(3) / (sin(result(0) / 2));

	return result;
}

Eigen::Matrix3f Scene::CreateEulerAnglesRotation(float x, float y, float z) {

	Eigen::Matrix<float, 3, 3> result;
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

Eigen::Vector3f Scene::EulerAnglesFromRotationMatrix(Eigen::Matrix3f m) {

	Eigen::Vector <float, 3> angles;
		float roll, pitch, yaw;
		pitch = asin(-m(2, 0));
		if (abs(atan(m(2, 1) / m(2, 2))) < 1E-8 || abs(atan(m(1, 0) / m(0, 0))) < 1E-8) {
			roll = acos(m(2, 2) / cos(pitch));
			yaw = acos(m(0, 0) / cos(pitch));
		}
		else {
			roll = atan(m(2, 1) / m(2, 2));
			yaw = atan(m(1, 0) / m(0, 0));
		}
		angles << roll, pitch, yaw;

	return angles;
}

Eigen::Vector4f Scene::QuaternionFromEulerAngles(float roll, float pitch, float yaw) {

	Eigen::Vector<float, 4> q;
	
	q(0) = sin(roll / 2) * cos(pitch / 2) * cos(yaw / 2) - (cos(roll / 2) * sin(pitch / 2) * sin(yaw / 2));
	q(1) = cos(roll / 2) * sin(pitch / 2) * cos(yaw / 2) + (sin(roll / 2) * cos(pitch / 2) * sin(yaw / 2));
	q(2) = cos(roll / 2) * cos(pitch / 2) * sin(yaw / 2) - (sin(roll / 2) * sin(pitch / 2) * cos(yaw / 2));
	q(3) = cos(roll / 2) * cos(pitch / 2) * cos(yaw / 2) + (sin(roll / 2) * sin(pitch / 2) * sin(yaw / 2));

	return q;

}

Eigen::Vector4f Scene::QuaternionFromEulerAndAxis(float angle, Eigen::Vector3f u) {

	Eigen::Vector<float, 4> q;
	q << cos(angle / 2), u(0)* sin(angle / 2), u(1)* sin(angle / 2), u(2)* sin(angle / 2);

	return q;
}

Eigen::Vector4f Scene::AngleAndAxisFromRotationMatrix(Eigen::Matrix3f r) {

	Eigen::Vector<float, 4>result;
	Eigen::Vector<float, 3>u;
	Eigen::Matrix<float, 3, 3>ux;

	result(0) = acos((r.trace() - 1) / 2);
	ux = (r - r.transpose()) / (2 * sin(result(0)));

	result(1) = ux(2, 1);
	result(2) = ux(0, 2);
	result(3) = ux(1, 0);

	return result;
}

Eigen::Vector4f Scene::AngleAndAxisFromQuaternion(Eigen::Vector4f q) {

	Eigen::Vector<float, 4> zero = { 0, 0, 0, 0 };
	if (q(0) == 1) {
		return zero;
	}

	Eigen::Vector<float, 4> u;
	u(0) = q(1) / (sqrt(1 - (q(0) * q(0))));
	u(1) = q(2) / (sqrt(1 - (q(0) * q(0))));
	u(2) = q(3) / (sqrt(1 - (q(0) * q(0))));
	u(3) = 2 * acos(q(0));

	return u;
}

Eigen::Vector3f Scene::QuaternionMultiplication(Eigen::Vector3f v, Eigen::Vector4f q) {

	Eigen::Matrix <float, 3, 3> identity;
	identity << 1, 0, 0, 0, 1, 0, 0, 0, 1;

	Eigen::Matrix <float, 3, 3> qx;
	qx << 0, -q(3), q(2), q(3), 0, -q(1), -q(2), q(1), 0;

	Eigen::Vector<float, 3>qv;
	qv << q(1), q(2), q(3);

	Eigen::Matrix<float, 3, 3>rotationQ;
	rotationQ = (q(0) * q(0) - qv.transpose() * qv) * identity + 2 * qv * qv.transpose() + 2 * q(0) * qx;

	return rotationQ * v;

}

float Scene::Angle2Vectors(Eigen::Vector3f u, Eigen::Vector3f v) {
	return acos((u(0) * (v(0)) + u(1) * (v(1)) + u(2) * (v(2))) / (u.norm() * v.norm()));
}

Eigen::Vector3f Scene::RotateQ(Eigen::Vector3f v, Eigen::Vector4f q) {
	v = QuaternionMultiplication(v, q);
	return v;
}

// Called before quitting
bool Scene::CleanUp()
{
	LOG("Freeing scene");
	

	return true;
}


void Scene::Reset() {
	p1 << 200, 200, 200;
	p2 << 400, 200, 200;
	p3 << 400, 400, 200;
	p4 << 200, 400, 200;
	p5 << 200, 200, 400;
	p6 << 400, 200, 400;
	p7 << 400, 400, 400;
	p8 << 200, 400, 400;

	rqtn << 1, 0, 0, 0;

	director << 0, 0, 1;
	directorref << 0, 0, 1;
	
}