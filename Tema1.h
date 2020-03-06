#pragma once

#include <Component/SimpleScene.h>
#include <string>
#include <Core/Engine.h>
using namespace std;


class Tema1 : public SimpleScene
{
	public:
		Tema1();
		~Tema1();

		void Init() override;

	private:
		void Blocuri(float deltaTimeSeconds);
		void FrameStart() override;
		void Update(float deltaTimeSeconds) override;
		void FrameEnd() override;
		void Scor(int scor);




		void OnInputUpdate(float deltaTime, int mods) override;
		void OnKeyPress(int key, int mods) override;
		void OnKeyRelease(int key, int mods) override;
		void OnMouseMove(int mouseX, int mouseY, int deltaX, int deltaY) override;
		void OnMouseBtnPress(int mouseX, int mouseY, int button, int mods) override;
		void OnMouseBtnRelease(int mouseX, int mouseY, int button, int mods) override;
		void OnMouseScroll(int mouseX, int mouseY, int offsetX, int offsetY) override;
		void OnWindowResize(int width, int height) override;

		float clamp(float value, float min, float max);
		GLboolean CheckCollision(glm::vec3 Center, float radius, float width, float height, glm::vec2 obstacle_center);
	protected:
		glm::vec3 Center = glm::vec3(100, 100, 0);
		glm::vec3 initialC = glm::vec3(100, 100, 0);
		float radius = 100.0f; //raza pasare
		float radius2 = 120.0f; //raza cerc imag
		float radius3 = 48;	//raza pt coliziune
		glm::vec3 corner = glm::vec3(0, 0, 0);
		float squareSide = 100;
		glm::mat3 modelMatrix, bodyMatrix, wingMatrix, pupilaMatrix,eyeMatrix,beakMatrix, circleMatrix;
		float translateX, translateY, translateXX, translateYY;
		float scaleX, scaleY;
		float angle;
		std::vector<std::pair<float, float>> var;
		float r, factor, contor;
		glm::ivec2 resolution = window->GetResolution();
		bool jump, wing, stop, afis;
		float sec;
		int scor = 0, cron = 0;
		float posX = (float)resolution.x / 4;
		float posY = (float)resolution.y / 2;
};
