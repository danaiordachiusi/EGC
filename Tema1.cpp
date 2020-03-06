#include "Tema1.h"

#include <vector>
#include <iostream>
#include <cstdlib>

#include <Core/Engine.h>
#include "Transform2D.h"
#include "Object2D.h"
#include <time.h>


using namespace std;

Tema1::Tema1()
{
}

Tema1::~Tema1()
{
}

void Tema1::Init()
{
	resolution = window->GetResolution();
	auto camera = GetSceneCamera();
	camera->SetOrthographic(0, (float)resolution.x, 0, (float)resolution.y, 0.01f, 400);
	camera->SetPosition(glm::vec3(0, 0, 50));
	camera->SetRotation(glm::vec3(0, 0, 0));
	camera->Update();
	GetCameraInput()->SetActive(false);

	// compute coordinates of square center
	float cx = corner.x + squareSide / 2;
	float cy = corner.y + squareSide / 2;
	
	// initialize tx and ty (the translation steps)
	translateX = 0;
	translateY = 0;
	translateXX = 0;
	translateYY = 0;

	stop = false;
	afis = true;

	// initialize sx and sy (the scale factors)
	scaleX = 1;
	scaleY = 1;
	
	// initialize angularStep
	angle= 0;
	Mesh* triangle = Object2D::CreateTriangle("triangle", corner, squareSide, glm::vec3(0.1, 0.01, 0), true);
	AddMeshToList(triangle);
	
	Mesh* circle = Object2D::CreateCircle("circle", initialC, radius, glm::vec3(1, 0, 0), true);
	AddMeshToList(circle);

	Mesh* rectangle = Object2D::CreateRectangle("rectangle", corner, squareSide, glm::vec3(0, 0.5, 0.5), true);
	AddMeshToList(rectangle);

	Mesh* obstacle = Object2D::CreateRectangle("obstacle", corner, squareSide, glm::vec3(0, 0.5, 0.5), true);
	AddMeshToList(obstacle);

	Mesh* circle_imag = Object2D::CreateCircle("circle_imag", initialC, radius2, glm::vec3(1, 0, 0), false);
	AddMeshToList(circle_imag);

	float f = (double)rand() / RAND_MAX;
	var.push_back(std::make_pair(float(resolution.x) - squareSide, 0.2 + f * 1.2));
	f = (double)rand() / RAND_MAX;
	var.push_back(std::make_pair(float(resolution.x) - squareSide + 360, 0.2 + f * 1.2));
	f = (double)rand() / RAND_MAX;
	var.push_back(std::make_pair(float(resolution.x) - squareSide + 360 * 2, 0.2 + f * 1.2));
	f = (double)rand() / RAND_MAX;
	var.push_back(std::make_pair(float(resolution.x) - squareSide + 360 * 3, 0.2 + f * 1.2));

}

void Tema1::FrameStart()
{
	// clears the color buffer (using the previously set color) and depth buffer
	glClearColor(0, 1, 1, 1);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glm::ivec2 resolution = window->GetResolution();
	// sets the screen area where to draw
	glViewport(0, 0, resolution.x, resolution.y);
}

void Tema1::Update(float deltaTimeSeconds)
{	


	if (!stop && afis == true) {
		if (jump == true) {
			translateY += deltaTimeSeconds * 150;
			Center.y = posY + translateY;
			sec -= deltaTimeSeconds;
			angle -= deltaTimeSeconds * 10;
			if (angle < -0.3) {
				angle = -0.3;
			}
			if (sec < 0) {
				jump = false;
			}
		}
		else {
			//conditia pentru cadere
			if (Center.y > -75) {
				translateY -= deltaTimeSeconds * 150;
				Center.y = posY + translateY;
				angle += deltaTimeSeconds * 5;
				if (angle > 0.7) {
					angle = 0.7;
				}
			}
			else {
				stop = true;
				printf("GAME OVER\n");
				printf("Scorul final este: 0");
				afis = false;
			}
		}
	}

	//Corpul
	bodyMatrix = glm::mat3(1);
	bodyMatrix *= Transform2D::Translate(translateX, translateY);
	bodyMatrix *= Transform2D::Translate(posX, posY);
	bodyMatrix *= Transform2D::Translate(initialC.x, initialC.y) * Transform2D::Rotate(angle)* Transform2D::Translate(-initialC.x,-initialC.y);
	bodyMatrix *= Transform2D::Translate(initialC.x, initialC.y) * Transform2D::Scale(0.4, 0.3) * Transform2D::Translate(-initialC.x, -initialC.y);

	//Aripa
	wingMatrix = glm::mat3(1);
	wingMatrix *= bodyMatrix;
	wingMatrix *= Transform2D::Translate(-60, 15);
	wingMatrix *= Transform2D::Rotate(1);
	wingMatrix *= Transform2D::Scale(0.2, 0.7);

	//Pupila
	pupilaMatrix = glm::mat3(1);
	pupilaMatrix *= bodyMatrix;
	pupilaMatrix *= Transform2D::Translate(160, 145);
	pupilaMatrix *= Transform2D::Scale(0.1, 0.1);

	//Ochi
	eyeMatrix = glm::mat3(1);
	eyeMatrix *= bodyMatrix;
	eyeMatrix *= Transform2D::Translate(175, 100);
	eyeMatrix *= Transform2D::Rotate(-0.6);
	eyeMatrix *= Transform2D::Scale(0.2, 0.45);

	//Cioc
	beakMatrix = glm::mat3(1);
	beakMatrix *= bodyMatrix;
	beakMatrix *= Transform2D::Translate(175, 140);
	beakMatrix *= Transform2D::Rotate(1.1);
	beakMatrix *= Transform2D::Scale(0.4, 0.4);

	RenderMesh2D(meshes["circle"], pupilaMatrix, glm::vec3(0,0,0));
	RenderMesh2D(meshes["circle"], shaders["Simple"], eyeMatrix);
	RenderMesh2D(meshes["circle"], wingMatrix, glm::vec3(0.1, 0.1, 0.1));
	RenderMesh2D(meshes["triangle"], shaders["VertexColor"], beakMatrix);
	RenderMesh2D(meshes["circle"],bodyMatrix, glm::vec3(0.5, 0, 0));

	//desenarea obstacolelor
	Blocuri(deltaTimeSeconds);

	
	circleMatrix = glm::mat3(1);
	circleMatrix *= Transform2D::Translate(translateX, translateY);
	circleMatrix *= Transform2D::Translate(posX, posY);
	circleMatrix *= Transform2D::Translate(initialC.x, initialC.y) * Transform2D::Scale(0.3, 0.3) * Transform2D::Translate(-initialC.x, -initialC.y);

	//pozitii bloc jos
	glm::vec2 Pos[4];
	glm::vec2 Pos2[4];
	for (int i = 0; i < 4; i++) {
		Pos[i].x = var[i].first + (squareSide / 1.5) / 2;
		Pos[i].y = ((squareSide * 3) / 2) * var[i].second;
		Pos2[i].x = var[i].first + (squareSide / 1.5) / 2;
		Pos2[i].y = (float)resolution.y + ((var.at(i).second - 1.5f) * squareSide * 3) / 2 ;
		
	}

	for (int i = 0; i < 4; i++) {
		//se verifica coliziunea cu obstacolele de jos
		if (CheckCollision(glm::vec3(translateX + posX + initialC.x, translateY + posY + initialC.y, 0), radius3, squareSide / 1.5, squareSide * 3 * var.at(i).second, Pos[i]) == true) {
			stop = true;
			Scor(scor);
		}
		//se verifica coliziunea cu obstacolele de sus
		if (CheckCollision(glm::vec3(translateX + posX + initialC.x, translateY + posY + initialC.y, 0), radius3, squareSide / 1.5, -squareSide * 3 * (var.at(i).second - 1.5f), Pos2[i]) == true) {
			stop = true;
			Scor(scor);
		}
	}
}	

void Tema1::Blocuri(float deltaTimeSeconds) {


	float c1 = 0.1, c2 = 0.3, c3 = 0;
	for (int i = 0; i < 4; i++) {
		//bloc jos
		modelMatrix = glm::mat3(1);
		modelMatrix *= Transform2D::Translate(var.at(i).first, 0);
		modelMatrix *= Transform2D::Scale(1, var.at(i).second);
		RenderMesh2D(meshes["rectangle"], modelMatrix, glm::vec3(c1,c2,c3));
		//dreptunghi deasupra bloc jos
		modelMatrix = glm::mat3(1);
		modelMatrix *= Transform2D::Translate(var.at(i).first - 0.13 * squareSide, var.at(i).second * squareSide * 3 - squareSide * 0.15);
		modelMatrix *= Transform2D::Scale(1.4, 0.1);
		RenderMesh2D(meshes["obstacle"], modelMatrix, glm::vec3(c1, c2, c3));
		//bloc sus
		modelMatrix = glm::mat3(1);
		modelMatrix *= Transform2D::Translate(var.at(i).first,(float)resolution.y);
		modelMatrix *= Transform2D::Scale(1, var.at(i).second - 1.5f);
		RenderMesh2D(meshes["rectangle"], modelMatrix, glm::vec3(c1, c2, c3));
		//dreptunghi sub bloc sus
		modelMatrix = glm::mat3(1);
		modelMatrix *= Transform2D::Translate(var.at(i).first - 0.13 * squareSide,(float)resolution.y + (var.at(i).second - 1.55f) * squareSide * 3);
		modelMatrix *= Transform2D::Scale(1.4, 0.1);
		RenderMesh2D(meshes["obstacle"], modelMatrix, glm::vec3(c1, c2, c3));
		if (c1 < 1.1) {
			c1 += 0.2;
			c2 += 0.35;
			c3 += 0.1;
		}
		else if (c1 > 1.1) {
			c1 -= 0.2;
			c2 -= 0.1;
			c3 -= 0.2;
		}
		if (var.at(i).first + squareSide / 1.5 < resolution.x / 4 && scor == 0) {
			scor = 1;
			printf("Scorul este: %d\n", scor);

		}
		if (!stop && afis == true) {
			var.at(i).first -= deltaTimeSeconds * 210;	//viteza cu care se misca obstacolele
		}
		//afisarea continua a blocurilor
		if (var.at(i).first + squareSide / 1.5 < 0) {
			var.at(i).first = (float)resolution.x + 100;
			scor++;
			printf("Scorul este: %d\n", scor);
		}

	}

}
float Tema1::clamp(float value, float min, float max) {
	return std::max(min, std::min(max, value));
}
GLboolean Tema1::CheckCollision(glm::vec3 Center, float radius, float width,float height,glm::vec2 obstacle_center) {
	// Get center point circle first
	glm::vec2 center(Center.x, Center.y);
	// Calculate obstacle info (center, half-extents)
	glm::vec2 aabb_half_extents(width / 2, height / 2);
	glm::vec2 aabb_center = obstacle_center;
	// Get difference vector between both centers
	glm::vec2 difference = center - aabb_center;
	glm::vec2 clamped = glm::clamp(difference, -aabb_half_extents, aabb_half_extents);
	// Add clamped value to AABB_center and we get the value of box closest to circle
	glm::vec2 closest = aabb_center + clamped;
	// Retrieve vector between center circle and closest point AABB and check if length <= radius
	difference = closest - center;
	return glm::length(difference) < radius;
}


void Tema1::FrameEnd()
{

}

void Tema1::Scor(int scor)
{
	if (afis == true) {
		printf("GAME OVER\n");
		printf("Scorul final este: %d\n", scor);
		afis = false;
	}
}

void Tema1::OnInputUpdate(float deltaTime, int mods)
{
	
}

void Tema1::OnKeyPress(int key, int mods)
{
	// add key press event 
	glm::ivec2 resolution = window->GetResolution();
	if (key == GLFW_KEY_SPACE) {
		//conditia pt a nu putea sa zboare in afara ecranului
		if (Center.y < resolution.y) {
			jump = true;
			sec = 0.3;	//timpul pentru rotatia pasarii
		}
	}
}

void Tema1::OnKeyRelease(int key, int mods)
{
	// add key release event
}

void Tema1::OnMouseMove(int mouseX, int mouseY, int deltaX, int deltaY)
{
	// add mouse move event
}

void Tema1::OnMouseBtnPress(int mouseX, int mouseY, int button, int mods)
{
	// add mouse button press event
}

void Tema1::OnMouseBtnRelease(int mouseX, int mouseY, int button, int mods)
{
	// add mouse button release event
}

void Tema1::OnMouseScroll(int mouseX, int mouseY, int offsetX, int offsetY)
{
}

void Tema1::OnWindowResize(int width, int height)
{
}
