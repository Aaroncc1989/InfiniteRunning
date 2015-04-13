#pragma once
#include "Matrix4.h"
#include "Vector3.h"
#include "Vector4.h"
#include "Mesh.h"
#include <vector>

class SceneNode	{
public:
	 SceneNode(Mesh*m = NULL, Vector4 colour = Vector4(1,1,1,1));
	~SceneNode(void);

	void			SetTransform(const Matrix4 &matrix) { transform = matrix;}
	const Matrix4&	GetTransform() const				{ return transform;}
	Matrix4			GetWorldTransform() const			{ return worldTransform;}
	Matrix4         GetModelMatrix() const				{ return modelMatrix; }

	virtual void	Update(float msec);
	virtual void	Draw(const OGLRenderer & r);

	Vector4			GetColour()		const			{return colour;}
	void			SetColour(const Vector4 &c)		{colour = c;}

	Vector3			GetModelScale()		const			{return modelScale;}
	void			SetModelScale(const Vector3 &s)		{modelScale = s;}

	Matrix4         GetRotation()		const	{ return rotation; }
	void			SetRotation(const Matrix4 &s)		{ rotation = s; }

	float           GetRotateAccum()    { return rotateAccum; }
	void            SetRotateAccum(float r){ rotateAccum = r; }

	void			AddChild(SceneNode* s);
	bool			RemoveChild(SceneNode* s,bool recursive = true);

	float			GetBoundingRadius() const	{return boundingRadius;}
	void			SetBoundingRadius(float f)	{boundingRadius = f;}

	float			GetCameraDistance() const	{return distanceFromCamera;}
	void			SetCameraDistance(float f)	{distanceFromCamera = f;}

	void			SetMesh(Mesh*m)				{mesh = m;}
	Mesh*			GetMesh()					{return mesh;}

	bool	IsAwake()	{return awake;}
	void	Wake()		{awake = true;}
	void	Sleep()		{awake = false;} 

	std::vector<SceneNode*>::const_iterator GetChildIteratorStart()	{return children.begin();}
	std::vector<SceneNode*>::const_iterator GetChildIteratorEnd()	{return children.end();}

	static bool		CompareByCameraDistance(SceneNode*a,SceneNode*b) ;
	static bool		CompareByZ(SceneNode*a,SceneNode*b) ;

	bool rotateDirect;
	Matrix4     textureMatrix;
protected:
	Matrix4     modelMatrix;
	Matrix4		worldTransform;
	Matrix4		transform;
	Matrix4     rotation;
	float       rotateAccum;

	SceneNode*	parent;
	float		distanceFromCamera;
	float		boundingRadius;
	Vector4		colour;
	Vector3		modelScale;
	Mesh*		mesh;
	bool		awake;
	std::vector<SceneNode*>		children;
};

