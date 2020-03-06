#pragma once

#include <include/glm.h>

namespace Transform2D
{
	// Translate matrix
	inline glm::mat3 Translate(float translateX, float translateY)
	{
		// TODO implement translate matrix
		return glm::mat3(
			1.f, 0.f, 0.f,
			0.f, 1.f, 0.f,
			translateX, translateY, 1
		);
	}

	// Scale matrix
	inline glm::mat3 Scale(float scaleX, float scaleY)
	{
		// TODO implement scale matrix
		return glm::mat3(
			scaleX, 0.f, 0.f,
			0.f, scaleY, 0.f,
			0.f, 0.f, 1.f
		);
	}

	// Rotate matrix
	inline glm::mat3 Rotate(float radians)
	{
		GLfloat c = cos(radians), s = sin(radians);
		// TODO implement rotate matrix
		return glm::transpose(
			glm::mat3(
			c, -s, 0.f,
			s, c, 0.f,
			0.f, 0.f, 1.f
		));
	}
}
