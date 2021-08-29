#pragma once

#include <glm/glm.hpp>
#include "Objects/AABB.hpp"

namespace Fizz {
	struct Transform {
		glm::vec2 position;
		float rotation;
		glm::vec2 scale;

		bool operator==(const Transform& rhs) const {
			return this->position == rhs.position && this->rotation == rhs.rotation &&
				   this->scale == rhs.scale;
		}

		bool operator!=(const Transform& rhs) const { return !(*this == rhs); }
	};

	class Shape {
	  public:
		virtual void Render() = 0;

		virtual glm::vec2 Support(const glm::vec2& dir) const = 0;
		virtual AABB GetAABB() const = 0;

		virtual void SetTransform(const Transform& transform) = 0;
	};
} // namespace Fizz
