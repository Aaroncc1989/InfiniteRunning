#pragma once
#include "GameEntity.h"
#include "GameClass.h"
#include "../nclgl/Camera.h"
#include "../nclgl/CubeRobot.h"

class MyGame : public GameClass	{
public:
	MyGame();
	~MyGame(void);

	virtual void UpdateGame(float msec);

protected:
	GameEntity* BuildRobotEntity();

	GameEntity* BuildCubeEntity(float size);

	GameEntity* BuildSphereEntity(float radius, Vector3 pos, Vector3 vel);

	GameEntity* BuildQuadEntity(float size);

	void SetTextureRepeating(GLuint target, bool repeating);

	Mesh* cube;
	Mesh* quad;
	Mesh* sphere;
	GameEntity* robot;
};

