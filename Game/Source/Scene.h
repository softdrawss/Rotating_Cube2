#ifndef __SCENE_H__
#define __SCENE_H__

#include "Module.h"
#include "GuiButton.h"
#include "InpButton.h"
#include "SDL/include/SDL.h"
#include "Animation.h"
#include <vector>
#include <eigen/Eigen/Core>
#include <eigen/Eigen/Dense>
#include <math.h>


struct SDL_Texture;

class Scene : public Module
{
public:

	Scene();

	// Destructor
	virtual ~Scene();

	// Called before render is available
	bool Awake(pugi::xml_node& config);

	// Called before the first frame
	bool Start();

	// Called before all Updates
	bool PreUpdate();

	// Called each loop iteration
	bool Update(float dt);

	// Called before all Updates
	bool PostUpdate();

	// Called before quitting
	bool CleanUp();

	// Define multiple Gui Event methods
	bool OnGuiMouseClickEvent(GuiControl* control);

	Eigen::Vector3f Scene::Rotate(Eigen::Vector3f point, float x, float y, float z);
	void Reset();

public:

	//L02: DONE 3: Declare a Player attribute 
	//Player* player;

	float roll;   //X Rotation
	float pitch;  //Y Rotation
	float yaw;    //Z Rotation

private:
	SDL_Texture* img;
	SDL_Texture* mouseTileTex = nullptr;
	SDL_Texture* originTex = nullptr;

	// L12: Debug pathfing
	iPoint origin;
	bool originSelected = false;

	// L15: DONE 2: Declare a GUI Button and create it using the GuiManager
	GuiButton* button1; 
	GuiButton* button2;
	//GuiButton* push[4]; //Not sure if I should create another button or just a bool
	InpButton* q[4];
	InpButton* euler[4];
	InpButton* angles[3];
	InpButton* vector[3];

	SDL_Texture* cubeTexture;
	SDL_Texture* UI;
	Animation down, left, right, back, front, up;
	Animation* currentAnim = nullptr;

	Eigen::Vector3f p1, p2, p3, p4, p5, p6, p7, p8;
	Eigen::Vector3f center;

};

#endif // __SCENE_H__