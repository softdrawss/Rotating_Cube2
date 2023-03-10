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
	
	void Reset();

	void ControlRotation();

	Eigen::Matrix3f FixMatrix0s(Eigen::Matrix3f mat);

	bool CompareFMatrices(Eigen::MatrixXf m1, Eigen::MatrixXf m2);

	Eigen::Matrix3f CreateRotationMatrix(Eigen::Vector4f e);

	Eigen::Vector3f RotationVectorFromAngleAndAxis(Eigen::Vector4f e);

	Eigen::Vector4f AngleAndAxisFromRotationVector(Eigen::Vector3f r);

	Eigen::Vector4f EulerAndAxisFromQuaternion(Eigen::Vector4f q);

	Eigen::Matrix3f CreateEulerAnglesRotation(float psi, float theta, float phi);

	Eigen::Vector3f EulerAnglesFromRotationMatrix(Eigen::Matrix3f r);

	Eigen::Vector4f QuaternionFromEulerAngles(float x, float y, float z);

	Eigen::Vector4f AngleAndAxisFromRotationMatrix(Eigen::Matrix3f r);

	Eigen::Vector4f AngleAndAxisFromQuaternion(Eigen::Vector4f q);

	Eigen::Vector4f QuaternionFromEulerAndAxis(Eigen::Vector4f e);

	Eigen::Vector3f QuaternionMultiplication(Eigen::Vector3f v, Eigen::Vector4f q);

	float Angle2Vectors(Eigen::Vector3f u, Eigen::Vector3f v);

	Eigen::Vector3f RotateQ(Eigen::Vector3f v, Eigen::Vector4f q);

	Eigen::Vector3f VecProduct(Eigen::Vector3f u, Eigen::Vector3f v);

	void ComputationAndPrintingOfAllRotations();

	void ChangePositionPoints(Eigen::Vector4f q);

public:

	Eigen::Vector4f qtn;
	Eigen::Vector3f director;
	Eigen::Vector3f directorx;
	Eigen::Vector3f directory;
	Eigen::Vector3f directorz;
	Eigen::Vector3f directorxy;
	Eigen::Vector3f directorxz;
	Eigen::Vector3f directoryz;
	Eigen::Vector3f directorxyz;
	Eigen::Vector3f directorref;

	Eigen::Vector3f eangles;		//XYZ Rotation
	Eigen::Matrix3f rmatrix;		//Rotation Matrix
	Eigen::Vector4f rqtn;			//Rotation Quaternion
	Eigen::Vector3f rVector;		//Rotation Vector
	Eigen::Vector4f angleAndAxis;	//Rotation Axis and Angle

	GuiButton* p[4]; 
	GuiButton* button2;
	//GuiButton* push[4]; //Not sure if I should create another button or just a bool
	InpButton* q[4];
	InpButton* e[4];
	InpButton* a[3];
	InpButton* v[3];

	bool isButtonPressed;

private:
	SDL_Texture* img;
	SDL_Texture* mouseTileTex = nullptr;
	SDL_Texture* originTex = nullptr;

	// L12: Debug pathfing
	iPoint origin;
	bool originSelected = false;

	// L15: DONE 2: Declare a GUI Button and create it using the GuiManager
	
	SDL_Texture* cubeTexture;
	SDL_Texture* UI;
	Animation down, left, right, back, front, up;
	Animation* currentAnim = nullptr;

	Eigen::Vector3f p1, p2, p3, p4, p5, p6, p7, p8;
	Eigen::Vector3f center;

};

#endif // __SCENE_H__