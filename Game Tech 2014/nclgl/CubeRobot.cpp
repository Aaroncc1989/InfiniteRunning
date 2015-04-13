#include "CubeRobot.h"

Mesh* CubeRobot::cube = NULL;
Mesh* CubeRobot::sphere = NULL;

CubeRobot::CubeRobot(void)	{	
	if(!cube) {
		CreateCube();
	}
	if (!sphere)
	{
		CreateSphere();
	}
	SetMesh(cube);

	body = new SceneNode(cube, Vector4(1, 0, 0, 1));
	body->SetModelScale(Vector3(8, -15, 5));
	body->SetTransform(Matrix4::Translation(Vector3(0, 75, 0)));
	AddChild(body);

	head = new SceneNode(sphere, Vector4(0, 1, 0, 1));
	head->SetModelScale(Vector3(6,8,6));
	head->SetTransform(Matrix4::Translation(Vector3(0,9,0)));
	body->AddChild(head);

	leftArm = new SceneNode(cube,Vector4(0,0,1,1));
	leftArm->SetModelScale(Vector3(3,-7,3));
	leftArm->SetTransform(Matrix4::Translation(Vector3(-11, 0, -1)));
	leftArm->SetRotation(Matrix4::Rotation(-5, Vector3(0, 0, 1)));
	body->AddChild(leftArm);

	leftArmLower = new SceneNode(cube, Vector4(1, 0, 1, 1));
	leftArmLower->SetModelScale(Vector3(2.9, -8, 3));
	leftArmLower->SetTransform(Matrix4::Translation(Vector3(0, -14, 0)));
	leftArm->AddChild(leftArmLower);

	rightArm = new SceneNode(cube,Vector4(0,0,1,1));
	rightArm->SetModelScale(Vector3(3, -7, 3));
	rightArm->SetTransform(Matrix4::Translation(Vector3(11, 0, -1)));
	rightArm->SetRotation(Matrix4::Rotation(5,Vector3(0,0,1)));
	body->AddChild(rightArm);

	rightArmLower = new SceneNode(cube, Vector4(1, 0, 1, 1));
	rightArmLower->SetModelScale(Vector3(2.9, -8, 3));
	rightArmLower->SetTransform(Matrix4::Translation(Vector3(0, -14, 0)));
	rightArm->AddChild(rightArmLower);

	leftLeg = new SceneNode(cube,Vector4(0,0,1,1));
	leftLeg->SetModelScale(Vector3(3,-10,3));
	leftLeg->SetTransform(Matrix4::Translation(Vector3(-5,-30,0)));
	body->AddChild(leftLeg);

	leftLegLower = new SceneNode(cube, Vector4(1, 0, 1, 1));
	leftLegLower->SetModelScale(Vector3(3, -12, 3));
	leftLegLower->SetTransform(Matrix4::Translation(Vector3(0, -20, 0)));
	leftLeg->AddChild(leftLegLower);

	rightLeg = new SceneNode(cube,Vector4(0,0,1,1));
	rightLeg->SetModelScale(Vector3(3,-10,3));
	rightLeg->SetTransform(Matrix4::Translation(Vector3(5, -30, 0)));
	body->AddChild(rightLeg);

	rightLegLower = new SceneNode(cube, Vector4(1, 0, 1, 1));
	rightLegLower->SetModelScale(Vector3(3, -12, 3));
	rightLegLower->SetTransform(Matrix4::Translation(Vector3(0, -20, 0)));
	rightLeg->AddChild(rightLegLower);

	body->SetBoundingRadius(15.0f);
	head->SetBoundingRadius(5.0f);

	leftArm->SetBoundingRadius(18.0f);
	rightArm->SetBoundingRadius(18.0f);

	leftLeg->SetBoundingRadius(18.0f);
	rightLeg->SetBoundingRadius(18.0f);
}

void CubeRobot::Update(float msec) {
	RunAnimate(msec);
	SceneNode::Update(msec);
}

void    CubeRobot::RunAnimate(float msec)
{
	msec *= 1.5;
	body->SetRotation(Matrix4::Rotation(10,Vector3(1,0,0)));
	CircleRotate(msec, 30, -30, leftArm);
	leftArmLower->SetRotation(Matrix4::Rotation(-60, Vector3(1, 0, 0)));
	CircleRotate(msec, -30, 30, rightArm);
	rightArmLower->SetRotation(Matrix4::Rotation(-60, Vector3(1, 0, 0)));

	CircleRotate(msec , 30, -30, rightLeg);
	if (rightLeg->rotateDirect)
	{
		AimRotate(msec, 0, rightLegLower);
	}
	else
	{
		AimRotate(msec, 30, rightLegLower);
	}

	CircleRotate(msec , -30, 30, leftLeg);
	if (!rightLeg->rotateDirect)
	{
		AimRotate(msec, 0, leftLegLower);
	}
	else
	{
		AimRotate(msec, 30, leftLegLower);
	}
}

bool    CubeRobot::AimRotate(float msec, float aimDegree, SceneNode* s)
{
	float r = s->GetRotateAccum();
	if (r < aimDegree - 3)
	{
		s->SetRotateAccum(r += (msec / 10.f));
		s->SetTransform(s->GetTransform()* Matrix4::Rotation(msec / 10.0f, Vector3(1, 0, 0)));
		return false;
	}
	else if (r > aimDegree + 3)
	{
		s->SetRotateAccum(r -= (msec / 10.f));
		s->SetTransform(s->GetTransform()* Matrix4::Rotation(-msec / 10.0f, Vector3(1, 0, 0)));
		return false;
	}
	else
	{
		return true;
	}
}

void    CubeRobot::CircleRotate(float msec, float range1, float range2, SceneNode* s)
{
	bool d = s->rotateDirect;
	if (d)
	{
		if (AimRotate(msec, range1, s))
		{
			s->rotateDirect = false;
		}
	}
	else
	{
		if (AimRotate(msec, range2, s))
		{
			s->rotateDirect = true;
		}
	}
}
