#pragma once

namespace Fizz {
	class PhysicsObject {
	  public:
		virtual void Render() = 0;

		virtual glm::vec2 Support(const glm::vec2& dir) = 0;
		inline glm::vec2 MinkowskiDiffSupport(Nutella::Ref<PhysicsObject>& other,
											  const glm::vec2& dir) {
			NT_PROFILE_FUNC();

			return this->Support(dir) - other->Support(-dir);
		}
	};
} // namespace Fizz
