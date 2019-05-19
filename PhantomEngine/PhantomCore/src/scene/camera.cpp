#include <iostream>
#include "camera.h"
#include "AngleUtils.h"
#include <math.h>


using namespace Phantom::maths;
const Phantom::maths::vec3 Phantom::CameraNode::DEFAULT_POS (0,500,500);
void Phantom::CameraNode::initViewMatrix()
{
	Yaw += 0.1f;//todo  有个bug，启动第一帧天空盒不渲
	updateCameraVectors();
	/*GfxConfiguration& conf = g_pApp->GetConfiguration();
	float screenAspect = (float)conf.screenWidth / (float)conf.screenHeight;*/
	if (true||m_Transforms.size() == 0) {
		//m_projectionMatrix.orthographic(-5.0f, 5.0f, -5.0f, 5.0f, 0.01f, 1000.0f);
		//ÉãÏñ»úÎ»ÖÃ£»  LookAtÎ»ÖÃ£¬ Up·½Ïò  
		m_viewMatrix.LookAtMatrixBuild(Position, Position + Front, UP);

	}
	else {
		mat4x4 cMat = (*(m_Transforms.begin()))->GetMatrix();
		cMat.InverseMatrix4X4f();
		m_viewMatrix = cMat;
	}
	m_projectionMatrix.perspective(Zoom, 16.0f / 9.0f, 0.01f, 10000.0f);

}
void Phantom::CameraNode::CalculateVPMatrix()
{
	if (true||m_Transforms.size() == 0) {
		//m_projectionMatrix.orthographic(-5.0f, 5.0f, -5.0f, 5.0f, 0.01f, 1000.0f);
		//ÉãÏñ»úÎ»ÖÃ£»  LookAtÎ»ÖÃ£¬ Up·½Ïò  
		m_viewMatrix.LookAtMatrixBuild(Position, Position + Front,  UP);
	}
	else {
		mat4x4 cMat = (*(m_Transforms.begin()))->GetMatrix();//todo Ä¬ÈÏÎÞÏà»úÃ»ÓÐ¿¼ÂÇ
		cMat.InverseMatrix4X4f();
		m_viewMatrix = cMat;
	}
	m_projectionMatrix.perspective(Zoom ,16.0f / 9.0f, 0.01f, 10000.0f);
	//m_projectionMatrix.orthographic(-5.0f, 5.0f, -5.0f, 5.0f, 0.01f, 1000.0f);
	/*glm::mat4 Projection = glm::perspective(glm::pi<float>() * 0.25f, 16.0f / 9.0f, 0.01f, 10000.0f);

	m_projectionMatrix = &Projection[0][0];*/
}

void Phantom::CameraNode::ProcessKeyboard(CameraDirection direction, float deltaTime)
{
	float velocity =  deltaTime*3;
	vec3 movation;
	if (direction == FORWARD) {
		movation = Front * velocity;
		Position += movation;
	}
	if (direction == BACKWARD) {
		movation = Front * velocity;
        Position -= movation;
	}
	if (direction == LEFT) {
		movation = Right * velocity;
		Position -= movation;
	}
	if (direction == RIGHT) {
		movation = Right * velocity;
		Position += movation;
	}
          
}

void Phantom::CameraNode::ProcessMouseMovement(float xoffset, float yoffset)
{
	xoffset*=0.1f;
	yoffset*=0.1f;
	Yaw += xoffset;
	Pitch -= yoffset;
#ifdef _DEBUG
	std::cerr << "[camera] camera angle::! ("
		<< xoffset << ","
		<< yoffset << ")"
		<< std::endl;
#endif

	/*if (Pitch > 89.0f)
		Pitch = 89.0f;
	if (Pitch < -89.0f)
		Pitch = -89.0f;*/
	// Update Front, Right and Up Vectors using the updated Euler angles
	updateCameraVectors();
}

void Phantom::CameraNode::updateCameraVectors()
{
	vec3 front;
	front.x = cos(toRadians(Yaw)) * cos(toRadians(Pitch));
	front.y = sin(toRadians(Pitch));
	front.z = sin(toRadians(Yaw))* cos(toRadians(Pitch));
	Front = front.normalize();
	Right = Front.crossProduct(WorldUp).normalize();  //ÎªÁËËãUP£¬Up¿ÉÄÜÊÇ´¹Ö±ÓÚÓÒ·½ÏòµÄÈÎÒâ·½Ïò¡£
	UP = Right.crossProduct(Front).normalize();
}
