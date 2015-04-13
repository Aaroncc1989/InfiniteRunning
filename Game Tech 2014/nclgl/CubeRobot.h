#pragma once
#include "SceneNode.h"
#include "OBJMesh.h"
#include "common.h"

class CubeRobot : public SceneNode	{
public:
	CubeRobot(void);
	virtual ~CubeRobot(void){};

	//Overloaded from SceneNode, makes the CubeRobot dance!
	virtual void	Update(float msec);

	virtual bool    AimRotate(float msec,float aimDegree,SceneNode* s);
	virtual void    CircleRotate(float msec,float min,float max,SceneNode* s);
	void			RunAnimate(float msec);
	//CubeRobot relies on their being a cube Mesh - this creates that Mesh.
	static void	CreateCube() {
		OBJMesh*m = new OBJMesh();
		m->LoadOBJMesh(MESHDIR"cube.obj");
		cube = m; 
	}

	static void CreateSphere()
	{
		OBJMesh*m = new OBJMesh();
		m->LoadOBJMesh(MESHDIR"ico.obj");
		sphere = m;
	}

	//Deletes the CubeRobot cube Mesh.
	static void DeleteCube(){
		delete cube;
		delete sphere;
	}

protected:
	static Mesh* cube;
	static Mesh* sphere;

	SceneNode*body;
	SceneNode*head;
	SceneNode*leftJoint;
	SceneNode*leftArm;
	SceneNode*rightArm;
	SceneNode*leftLeg;
	SceneNode*rightLeg;

	SceneNode*leftArmLower;
	SceneNode*rightArmLower;
	SceneNode*leftLegLower;
	SceneNode*rightLegLower;

};

