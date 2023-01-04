#pragma once

#include "Objects/PhysicsObject.hpp"

namespace Fizz {
	class PhysicsConstraint {
	  public:
		virtual void Resolve() const = 0;

	  private:
		virtual float Evaluate() const = 0;
		virtual float EvaluateDerivative() const = 0;
	};
} // namespace Fizz
