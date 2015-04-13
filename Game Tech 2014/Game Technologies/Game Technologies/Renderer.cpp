#include "Renderer.h"

Renderer* Renderer::instance = NULL;

Renderer::Renderer(Window &parent) : OGLRenderer(parent)	{
	camera = NULL;

	root = new SceneNode();
	quad = Mesh::GenerateQuad();
	simpleShader = new Shader(SHADERDIR"TechVertex.glsl", SHADERDIR"TechFragment.glsl");
	skyboxShader = new Shader(SHADERDIR"skyboxVertex.glsl", SHADERDIR"skyboxFragment.glsl");

	if (!simpleShader->LinkProgram()||!skyboxShader->LinkProgram()){
		return;
	}

	cubeMap = SOIL_load_OGL_cubemap(
		"../../Textures/rusted_west.jpg", "../../Textures/rusted_east.jpg",
		"../../Textures/rusted_up.jpg", "../../Textures/rusted_down.jpg",
		"../../Textures/rusted_south.jpg", "../../Textures/rusted_north.jpg",
		SOIL_LOAD_RGB,
		SOIL_CREATE_NEW_ID, 0
		);
	if (!cubeMap)
	{ 
		return; 
	}
	instance = this;

	init = true;

	glEnable(GL_TEXTURE_CUBE_MAP_SEAMLESS);
}

Renderer::~Renderer(void)	{
	delete root;
	delete simpleShader;

	currentShader = NULL;
}

void Renderer::UpdateScene(float msec)	{
	if (camera) {
		camera->UpdateCamera(msec);
	}
	root->Update(msec);
}

void Renderer::RenderScene()	{
	glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
	DrawSkybox();
	if (camera) {
		SetCurrentShader(simpleShader);

		textureMatrix.ToIdentity();
		modelMatrix.ToIdentity();
		viewMatrix = camera->BuildViewMatrix();
		projMatrix = Matrix4::Perspective(1.0f, 10000.0f, (float)width / (float)height, 45.0f);
		frameFrustum.FromMatrix(projMatrix * viewMatrix);
		UpdateShaderMatrices();

		//Return to default 'usable' state every frame!
		glEnable(GL_DEPTH_TEST);
		//glEnable(GL_CULL_FACE);
		glDisable(GL_STENCIL_TEST);
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

		BuildNodeLists(root);
		SortNodeLists();
		DrawNodes();
		ClearNodeLists();	
	}

	glUseProgram(0);
	SwapBuffers();
}

void	Renderer::DrawNode(SceneNode*n)	{
	if (n->GetMesh()) {
		glUniformMatrix4fv(glGetUniformLocation(currentShader->GetProgram(), "modelMatrix"), 1, false, (float*)&(n->GetModelMatrix()));
		glUniformMatrix4fv(glGetUniformLocation(currentShader->GetProgram(), "textureMatrix"), 1, false, (float*)&(n->textureMatrix));
		glUniform4fv(glGetUniformLocation(currentShader->GetProgram(), "nodeColour"), 1, (float*)&n->GetColour());
		glUniform1i(glGetUniformLocation(currentShader->GetProgram(), "useTexture"), int(n->GetMesh()->GetTexture()));
		n->Draw(*this);
	}
}

void	Renderer::BuildNodeLists(SceneNode* from)	{
	Vector3 direction = from->GetWorldTransform().GetPositionVector() - camera->GetPosition();
	from->SetCameraDistance(Vector3::Dot(direction, direction));

	if (frameFrustum.InsideFrustum(*from)) {
		if (from->GetColour().w < 1.0f) {
			transparentNodeList.push_back(from);
		}
		else{
			nodeList.push_back(from);
		}
	}

	for (vector<SceneNode*>::const_iterator i = from->GetChildIteratorStart(); i != from->GetChildIteratorEnd(); ++i) {
		BuildNodeLists((*i));
	}
}

void	Renderer::DrawNodes()	 {
	for (vector<SceneNode*>::const_iterator i = nodeList.begin(); i != nodeList.end(); ++i) {
		DrawNode((*i));
	}

	for (vector<SceneNode*>::const_reverse_iterator i = transparentNodeList.rbegin(); i != transparentNodeList.rend(); ++i) {
		DrawNode((*i));
	}
}

void	Renderer::SortNodeLists()	{
	std::sort(transparentNodeList.begin(), transparentNodeList.end(), SceneNode::CompareByCameraDistance);
	std::sort(nodeList.begin(), nodeList.end(), SceneNode::CompareByCameraDistance);
}

void	Renderer::ClearNodeLists()	{
	transparentNodeList.clear();
	nodeList.clear();
}

void	Renderer::SetCamera(Camera*c) {
	camera = c;
}

void	Renderer::AddNode(SceneNode* n) {
	root->AddChild(n);
}

void	Renderer::RemoveNode(SceneNode* n) {
	root->RemoveChild(n);
}

void	Renderer::DrawSkybox() {
	glDepthMask(GL_FALSE);
	SetCurrentShader(skyboxShader);

	modelMatrix.ToIdentity();
	UpdateShaderMatrices();
	quad->Draw();

	glUseProgram(0);
	glDepthMask(GL_TRUE);
}