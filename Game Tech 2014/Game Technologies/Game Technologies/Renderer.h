#pragma once

#include "../../nclgl/OGLRenderer.h"
#include "../../nclgl/SceneNode.h"
#include "../../nclgl/Camera.h"
#include "../../nclgl/Frustum.h"
#include <algorithm>

class Renderer : public OGLRenderer	{
public:
	virtual void RenderScene();
	virtual void UpdateScene(float msec);

	void	SetCamera(Camera*c);

	void	AddNode(SceneNode* n);

	void	RemoveNode(SceneNode* n);

	//Statics
	static bool Initialise() {
		instance = new Renderer(Window::GetWindow());
		return instance->HasInitialised();
	}

	static void Destroy() {
		delete instance;
	}
	
	static Renderer&GetRenderer() { return *instance;}

protected:	
	Renderer(Window &parent);
	virtual ~Renderer(void);

	void	BuildNodeLists(SceneNode* from);
	void	SortNodeLists();
	void	ClearNodeLists();
	void    DrawSkybox();
	void	DrawNodes();
	void	DrawNode(SceneNode*n);

	SceneNode*	root;
	Camera*		camera;
	Shader*		simpleShader;
	Shader*     skyboxShader;
	Mesh*       quad;
	GLuint		cubeMap;

	Frustum		frameFrustum;

	vector<SceneNode*> transparentNodeList;
	vector<SceneNode*> nodeList;

	static Renderer*	instance;
};

