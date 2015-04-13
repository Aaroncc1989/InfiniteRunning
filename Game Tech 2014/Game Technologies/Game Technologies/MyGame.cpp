#include "MyGame.h"
#include "InertialMatrixHelper.h"
#include "Spring.h"
#include "SpringDemo.h"


MyGame::MyGame()	{
	gameCamera = new Camera(-30.0f,180.0f,Vector3(0,450,-850));

	Renderer::GetRenderer().SetCamera(gameCamera);

	CubeRobot::CreateCube();

	cube	= new OBJMesh(MESHDIR"cube1.obj");
	quad	= Mesh::GenerateQuad();
	sphere	= new OBJMesh(MESHDIR"ico.obj");

	cube->SetTexture(SOIL_load_OGL_texture("../../Textures/ground.jpg",
		SOIL_LOAD_AUTO, SOIL_CREATE_NEW_ID, SOIL_FLAG_MIPMAPS));
	SetTextureRepeating(cube->GetTexture(), true);

	GameEntity* quadEntity = BuildQuadEntity(5000.0f);
	allEntities.push_back(quadEntity);
	robot = BuildRobotEntity();
	allEntities.push_back(robot);	
}

MyGame::~MyGame(void)	{
	delete cube;
	delete quad;
	delete sphere;

	CubeRobot::DeleteCube();
}


void MyGame::UpdateGame(float msec) {
	if (gameCamera) {
		gameCamera->UpdateCamera(msec);
	}

	if (Window::GetKeyboard()->KeyTriggered(KEYBOARD_J))
	{
		Vector3 tmpv = robot->GetPhysicsNode().GetLinearVelocity() + Vector3(0, 0.6, 0);
		robot->GetPhysicsNode().SetLinearVelocity(tmpv);
	}

	for (vector<GameEntity*>::iterator i = allEntities.begin(); i != allEntities.end(); ++i) {
		(*i)->Update(msec);
	}

}
GameEntity* MyGame::BuildRobotEntity() {
	SceneNode* s = new CubeRobot();
	PhysicsNode* p = new PhysicsNode();
	p->SetUseGravity(true);
	p->SetPosition(Vector3(0,200,0));
	p->SetLinearVelocity(Vector3(0, 0, 1));
	p->SetInverseMass(1.f);
	p->SetCollisionVolume(new CollisionAABB(Vector3(2,2,2)));
	GameEntity* g = new GameEntity(s,p);
	g->ConnectToSystems();
	return g;
}

GameEntity* MyGame::BuildCubeEntity(float size) {
	GameEntity*g = new GameEntity(new SceneNode(cube), new PhysicsNode());
	g->ConnectToSystems();

	SceneNode &test = g->GetRenderNode();

	test.SetModelScale(Vector3(size,size,size));
	test.SetBoundingRadius(size);

	return g;
}

GameEntity* MyGame::BuildSphereEntity(float radius, Vector3 pos, Vector3 vel) {
	SceneNode* s = new SceneNode(sphere);

	s->SetModelScale(Vector3(radius,radius,radius));
	s->SetBoundingRadius(radius);
	s->SetColour(Vector4(0.2,0.2,0.5,1));
	PhysicsNode*p = new PhysicsNode();
	p->SetPosition(pos);
	p->SetLinearVelocity(vel);
	p->SetAngularVelocity(Vector3(0,0,0.01f));
	p->SetInverseInertia(InertialMatrixHelper::createSphereInvInertial(1.0f, radius));
	p->SetInverseMass(1.0f);
	p->SetCollisionVolume(new CollisionSphere(radius));
	GameEntity*g = new GameEntity(s, p);
	g->ConnectToSystems();
	return g;
}


GameEntity* MyGame::BuildQuadEntity(float size) {
	SceneNode* s = new SceneNode(cube);
	s->SetModelScale(Vector3(500, 100, size));
	s->SetBoundingRadius(size*5);
	//s->SetColour(Vector4(0,0,0,1));
	s->textureMatrix.SetScalingVector(Vector3(0.5, size/1000, 10));

	PhysicsNode*p = new PhysicsNode(Quaternion::AxisAngleToQuaterion(Vector3(1, 0, 0), 0), Vector3());
	p->SetUseGravity(false);
	p->SetInverseMass(0.0f);
	p->SetPosition(Vector3(0,0,size));
	p->SetInverseInertia(InertialMatrixHelper::createImmovableInvInertial());
	p->SetCollisionVolume(new CollisionAABB(s->GetModelScale()));
	GameEntity*g = new GameEntity(s, p);
	g->ConnectToSystems();
	return g;
}

void MyGame::SetTextureRepeating(GLuint target, bool repeating)	{
	glBindTexture(GL_TEXTURE_2D, target);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, repeating ? GL_REPEAT : GL_CLAMP);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, repeating ? GL_REPEAT : GL_CLAMP);
	glBindTexture(GL_TEXTURE_2D, 0);
}