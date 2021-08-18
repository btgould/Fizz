#include "PhysicsObject.hpp"

#include "Simplex.hpp"

namespace Fizz {
	// checks is simplex contains origin, returning true if it does. p2wise, it removes any
	// redundant points on the simplex
	bool UpdateSimplex(Simplex& s);
	// calculates the normal of the simplex pointed towards the origin
	glm::vec2 NextDir(const Simplex& s);

	bool GJKColliding(Nutella::Ref<PhysicsObject>& p1, Nutella::Ref<PhysicsObject>& p2) {
		NT_PROFILE_FUNC();

		glm::vec2 nextDir = p2->GetPos() - p1->GetPos();

		// add first point to simplex
		glm::vec2 supportPoint(p1->MinkowskiDiffSupport(p2, nextDir));
		Simplex s({supportPoint});
		nextDir = -supportPoint;

		// add second point to simplex
		supportPoint = p1->MinkowskiDiffSupport(p2, nextDir);
		if (glm::dot(nextDir, supportPoint) < 0) {
			// simplex cannot possibly contain origin
			return false;
		}
		s.Add(supportPoint);
		nextDir = NextDir(s);

		while (true) {
			// calculate + add next point
			supportPoint = p1->MinkowskiDiffSupport(p2, nextDir);
			if (glm::dot(nextDir, supportPoint) < 0) {
				// simplex cannot possibly contain origin
				return false;
			}
			s.Add(supportPoint);

			// check if simplex contains origin + remove redundant points
			if (UpdateSimplex(s)) {
				// Simplex contains origin -> collision
				return true;
			}

			// update seach direction
			nextDir = NextDir(s);
		}
	}

	bool UpdateSimplex(Simplex& s) {
		// calculate where origin is
		glm::vec3 side1 = glm::vec3(s[1].x - s[2].x, s[1].y - s[2].y, 0.0f);
		glm::vec3 side2 = glm::vec3(s[0].x - s[2].x, s[0].y - s[2].y, 0.0f);
		glm::vec3 toOrigin = glm::vec3(-s[2].x, -s[2].y, 0.0f);
		glm::vec3 norm1 = glm::cross(glm::cross(side2, side1), side1);
		glm::vec3 norm2 = glm::cross(glm::cross(side1, side2), side2);

		if (glm::dot(norm1, toOrigin) > 0) {
			// origin is in region 1
			s.Remove(s[0]);
		} else if (glm::dot(norm2, toOrigin) > 0) {
			// origin is in region 2
			s.Remove(s[1]);
		} else {
			// origin is in simplex -> collision
			return true;
		}

		return false;
	}

	glm::vec2 NextDir(const Simplex& s) {
		glm::vec3 segment = glm::vec3(s[0].x - s[1].x, s[0].y - s[1].y, 0.0f);
		glm::vec3 toOrigin = glm::vec3(-s[1].x, -s[1].y, 0.0f);

		glm::vec2 dir = glm::cross(glm::cross(segment, toOrigin), segment);

		if (dir == glm::vec2(0.0f, 0.0f)) {
			// origin is on line segment -> search along normal
			dir = glm::vec2(-segment.y, segment.x);
		}

		return dir;
	}

	// Finds the closest point to the origin on the line segment defined by two points
	glm::vec2 Line(const Simplex& s);
	// Finds the closest point to the origin on the triangle defined by three points. Also removes
	// the redundant point from the simplex
	glm::vec2 Triangle(Simplex& s);

	glm::vec2 GJKDistance(Nutella::Ref<PhysicsObject>& p1, Nutella::Ref<PhysicsObject>& p2,
						  float tolerance /* = glm::pow(10, -8)*/) {
		NT_PROFILE_FUNC();

		glm::vec2 nextDir = p2->GetPos() - p1->GetPos();

		Simplex s({p1->MinkowskiDiffSupport(p2, nextDir)});
		s.Add(p1->MinkowskiDiffSupport(p2, -nextDir));

		nextDir = -Line(s);

		while (true) {
			s.Add(p1->MinkowskiDiffSupport(p2, nextDir));

			// check if we are no longer making significant progress
			float newSupportProj = glm::dot(nextDir, s[2]);
			float oldSupportProj = glm::dot(nextDir, s[0]);

			if (newSupportProj - oldSupportProj < tolerance)
				return nextDir;

			// find next direction to search in, update simplex
			nextDir = -Triangle(s);
		}
	}

	glm::vec2 Line(const Simplex& s) {
		NT_ASSERT(s.Size() == 2, "Invalid Simplex during collision detection!");

		if (s[0] == s[1])
			return s[0];

		glm::vec2 AB = s[1] - s[0];
		glm::vec2 AO = -s[0];

		// AB * AO =  length of projection of AO onto AB times length of AB
		// AB * AB = length of AB^2
		// therefore, t = length of AB component of AO as a fraction of length of AB
		float t = glm::dot(AB, AO) / glm::dot(AB, AB);
		t = glm::clamp(t, 0.0f, 1.0f); // we can only go so far in each direction

		// P = A + t * AB gives all of AB component of AO
		// therefore, PO is perpendicular to AB, meaning it is the closest to the origin
		return s[0] + t * AB;
	}

	glm::vec2 Triangle(Simplex& s) {
		NT_ASSERT(s.Size() == 3, "Invalid Simplex during collision detection!");

		// calculate where origin is
		glm::vec3 side1 = glm::vec3(s[1].x - s[2].x, s[1].y - s[2].y, 0.0f);
		glm::vec3 side2 = glm::vec3(s[0].x - s[2].x, s[0].y - s[2].y, 0.0f);
		glm::vec3 toOrigin = glm::vec3(-s[2].x, -s[2].y, 0.0f);
		glm::vec3 norm1 = glm::cross(glm::cross(side2, side1), side1);
		glm::vec3 norm2 = glm::cross(glm::cross(side1, side2), side2);

		float proj1 = glm::dot(norm1, toOrigin);
		float proj2 = glm::dot(norm2, toOrigin);

		if (proj1 > 0 && proj2 > 0) {
			// either line from s[2] could be closest, we must check
			glm::vec2 p1 = Line(std::vector<glm::vec2>({s[0], s[2]}));
			glm::vec2 p2 = Line(std::vector<glm::vec2>({s[1], s[2]}));

			if (glm::dot(p1, p1) > glm::dot(p2, p2)) {
				// line using s[1] is closer
				s.Remove(s[0]);
				return p2;
			} else {
				// line using s[0] is closer
				s.Remove(s[1]);
				return p1;
			}
		} else if (proj1 > 0) {
			// origin is in region 2
			s.Remove(s[0]);
			return Line(s);
		} else if (proj2 > 0) {
			// origin is in region 3
			s.Remove(s[1]);
			return Line(s);
		}

		// p2wise, origin is in simplex -> collision
		return glm::vec2(0.0f, 0.0f);
	}

	// Finds the closest point on the edge of the Minkowski difference to the origin, and returns
	// the collision this point describes.
	Collision EPA(Nutella::Ref<PhysicsObject>& p1, Nutella::Ref<PhysicsObject>& p2, Simplex& s,
				  float tolerance);

	Collision GJKGetCollision(Nutella::Ref<PhysicsObject>& p1, Nutella::Ref<PhysicsObject>& p2,
							  float tolerance /* = glm::pow(10, -4)*/) {
		NT_PROFILE_FUNC();

		glm::vec2 nextDir = p2->GetPos() - p1->GetPos();
		if (nextDir == glm::vec2(0.0f, 0.0f)) {
			nextDir = glm::vec2(1.0f, 0.0f);
		}

		// add first point to simplex
		glm::vec2 supportPoint(p1->MinkowskiDiffSupport(p2, nextDir));
		Simplex s({supportPoint});
		nextDir = -supportPoint;

		// add second point to simplex
		supportPoint = p1->MinkowskiDiffSupport(p2, nextDir);
		if (glm::dot(nextDir, supportPoint) < 0) {
			// simplex cannot possibly contain origin
			return Collision::None(p1, p2);
		}
		s.Add(supportPoint);
		nextDir = NextDir(s);

		while (true) {
			// calculate + add next point
			supportPoint = p1->MinkowskiDiffSupport(p2, nextDir);
			if (glm::dot(nextDir, supportPoint) < 0) {
				// simplex cannot possibly contain origin
				return Collision::None(p1, p2);
			}
			s.Add(supportPoint);

			// check if simplex contains origin + remove redundant points
			if (UpdateSimplex(s)) {
				// Simplex contains origin -> collision
				break;
			}

			// update seach direction
			nextDir = NextDir(s);
		}

		// compute + return collision
		return EPA(p1, p2, s, tolerance);
	}

	Collision EPA(Nutella::Ref<PhysicsObject>& p1, Nutella::Ref<PhysicsObject>& p2, Simplex& s,
				  float tolerance) {
		while (true) {
			float closestDist = glm::dot(s[0], s[0]);
			glm::vec2 closestDir(s[0]);
			uint32_t closestIdx = 1;

			// calculate closest point on current simplex
			for (uint32_t i = 0; i < s.Size(); i++) {
				uint32_t j = i + 1 == s.Size() ? 0 : i + 1;

				glm::vec2 p = Line(Simplex({s[i], s[j]}));
				float newDist = glm::dot(p, p);

				if (newDist < closestDist) {
					closestDist = newDist;
					closestDir = p;
					closestIdx = i + 1;
				}
			}

			// calculate next point to add to simplex
			if (closestDist < glm::pow(10, -8)) {
				// origin is on edge of simplex -> need to search along edge normals
				glm::vec2 edge = s[closestIdx % s.Size()] - s[closestIdx - 1];
				glm::vec2 norm(-edge.y, edge.x);

				// make sure we are searching away from rest of simplex
				if (glm::dot(norm, s[(closestIdx + 1) % s.Size()]) > 0) {
					norm = -norm;
				}

				closestDir = norm;
			}

			glm::vec2 nextPoint = p1->MinkowskiDiffSupport(p2, closestDir);

			// check if we are still making significant progress
			float oldDist = closestDist;
			float newDist = glm::dot(nextPoint, closestDir);

			if (newDist - oldDist < tolerance) {
				float penetrationDepth;
				glm::vec2 MTV;

				if (closestDist < glm::pow(10, -8)) {
					// origin is on edge of simplex
					penetrationDepth = 0.0f;
					MTV = glm::normalize(closestDir);
				} else {
					// orgin is properly inside simplex
					penetrationDepth = glm::length(closestDir);
					MTV = closestDir / penetrationDepth;
				}

				return {p1, p2, true, penetrationDepth, MTV};
			} else {
				s.Add(nextPoint, closestIdx);
			}
		}
	}
} // namespace Fizz
