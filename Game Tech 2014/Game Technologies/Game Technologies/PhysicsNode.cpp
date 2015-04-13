#include "PhysicsNode.h"

const Vector3 PhysicsNode::gravity = Vector3(0,-0.001,0);

PhysicsNode::PhysicsNode(void): vol(NULL) {
	target = NULL;
	useGravity = true;
}

PhysicsNode::PhysicsNode(Quaternion orientation, Vector3 position): vol(NULL) {
	m_orientation	= orientation;
	m_position		= position;
	useGravity = true;
}

PhysicsNode::~PhysicsNode(void)	{

}

void	PhysicsNode::Update(float msec) {

	Vector3 acc = m_force*m_invMass + (useGravity? gravity: Vector3(0,0,0));
	m_linearVelocity = m_linearVelocity + acc*msec;

	//m_linearVelocity = m_linearVelocity*LINEAR_VELOCITY_DAMP;
	//if (Vector3::Dot(m_linearVelocity, m_linearVelocity) < LINEAR_VELOCITY_MIN) {
	//	m_linearVelocity = Vector3(0,0,0);
	//}

	m_position = m_position + m_linearVelocity*msec;

	Vector3 angAcc = m_invInertia*m_torque;
	m_angularVelocity = m_angularVelocity + angAcc*msec;
	m_angularVelocity = m_angularVelocity*ANGULAR_VELOCITY_DAMP;
	m_orientation = m_orientation + m_orientation*(m_angularVelocity*(msec/2.0f));
	m_orientation.Normalise();

	m_force = Vector3(0,0,0);
	m_torque = Vector3(0,0,0);

	if(target) {
		target->SetTransform(BuildTransform());
	}
}

void PhysicsNode::AddForce(Vector3 point, Vector3 force) {
	m_force = m_force + force;
	m_torque = m_torque + Vector3::Cross(point - m_position, force);
}

Matrix4		PhysicsNode::BuildTransform() {
	Matrix4 m = m_orientation.ToMatrix();

	m.SetPositionVector(m_position);

	return m;
}