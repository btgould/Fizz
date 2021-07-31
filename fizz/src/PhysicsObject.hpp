#pragma once

namespace Fizz {
	class PhysicsObject {
	  public:
		virtual void Render() = 0;

		virtual glm::vec2 Support(const glm::vec2& dir) const = 0;
		inline glm::vec2 MinkowskiDiffSupport(Nutella::Ref<PhysicsObject>& other,
											  const glm::vec2& dir) const {
			NT_PROFILE_FUNC();

			return this->Support(dir) - other->Support(-dir);
		}

		virtual glm::vec2 GetPos() const = 0;
		virtual void SetPos(const glm::vec2& pos) = 0;
		virtual float GetRot() const = 0;
		virtual void SetRot(const float rot) = 0;
		virtual glm::vec2 GetScale() const = 0;
		virtual void SetScale(const glm::vec2& scale) = 0;
	};
} // namespace Fizz
