#include "CollisionDetection.hpp"

#include "Simplex.hpp"

namespace Fizz {
	/** Checks if simplex contains origin, returning true if it does. Otherwise, it removes any
	 *  redundant points on the simplex.
	 */
	bool UpdateSimplex(Simplex<Support>& s);
	/** Calculates the normal of the simplex pointed towards the origin */
	glm::vec2 NextDir(const Simplex<Support>& s);

	bool GJKColliding(Nutella::Ref<PhysicsObject>& p1, Nutella::Ref<PhysicsObject>& p2) {
		NT_PROFILE_FUNC();

		glm::vec2 nextDir = p2->GetPos() - p1->GetPos();
		if (nextDir == glm::vec2(0.0f, 0.0f)) {
			nextDir = glm::vec2(1.0f, 0.0f);
		}

		// add first point to simplex
		Support supportPoint(MinkowskiDiffSupport(p1, p2, nextDir));
		Simplex<Support> s({supportPoint});
		nextDir = -supportPoint.mkSupport;

		// add second point to simplex
		supportPoint = MinkowskiDiffSupport(p1, p2, nextDir);
		if (glm::dot(nextDir, supportPoint.mkSupport) < 0) {
			// simplex cannot possibly contain origin
			return false;
		}
		s.Add(supportPoint);
		nextDir = NextDir(s);

		while (true) {
			// calculate + add next point
			supportPoint = MinkowskiDiffSupport(p1, p2, nextDir);
			if (glm::dot(nextDir, supportPoint.mkSupport) < 0) {
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

	bool UpdateSimplex(Simplex<Support>& s) {
		NT_ASSERT(s.Size() == 3, "Invalid Simplex during collision detection!");

		// calculate where origin is
		glm::vec3 side1 = glm::vec3(s[1].mkSupport.x - s[2].mkSupport.x,
									s[1].mkSupport.y - s[2].mkSupport.y, 0.0f);
		glm::vec3 side2 = glm::vec3(s[0].mkSupport.x - s[2].mkSupport.x,
									s[0].mkSupport.y - s[2].mkSupport.y, 0.0f);
		glm::vec3 toOrigin = glm::vec3(-s[2].mkSupport.x, -s[2].mkSupport.y, 0.0f);
		glm::vec3 norm1 = glm::cross(glm::cross(side2, side1), side1);
		glm::vec3 norm2 = glm::cross(glm::cross(side1, side2), side2);

		if (glm::dot(norm1, toOrigin) > 0) {
			// origin is in region 1
			s.Remove(0);
		} else if (glm::dot(norm2, toOrigin) > 0) {
			// origin is in region 2
			s.Remove(1);
		} else {
			// origin is in simplex -> collision
			return true;
		}

		return false;
	}

	glm::vec2 NextDir(const Simplex<Support>& s) {
		glm::vec3 segment = glm::vec3(s[0].mkSupport.x - s[1].mkSupport.x,
									  s[0].mkSupport.y - s[1].mkSupport.y, 0.0f);
		glm::vec3 toOrigin = glm::vec3(-s[1].mkSupport.x, -s[1].mkSupport.y, 0.0f);

		glm::vec2 dir = glm::cross(glm::cross(segment, toOrigin), segment);

		if (dir == glm::vec2(0.0f, 0.0f)) {
			// origin is on line segment -> search along normal
			dir = glm::vec2(-segment.y, segment.x);
		}

		return dir;
	}

	/** Finds the closest point to the origin on the line segment defined by two points */
	glm::vec2 Line(const Simplex<Support>& s) {
		// TEMP: consider a way of doing this that is less senstive to fp errors
		NT_ASSERT(s.Size() == 2, "Invalid Simplex during collision detection!");

		if (s[0].mkSupport == s[1].mkSupport)
			return s[0].mkSupport;

		glm::vec2 AB = s[1].mkSupport - s[0].mkSupport;
		glm::vec2 AO = -s[0].mkSupport;

		// AB * AO =  length of projection of AO onto AB times length of AB
		// AB * AB = length of AB^2
		// therefore, t = length of AB component of AO as a fraction of length of AB
		float t = glm::dot(AB, AO) / glm::dot(AB, AB);
		t = glm::clamp(t, 0.0f, 1.0f); // we can only go so far in each direction

		// P = A + t * AB gives all of AB component of AO
		// therefore, PO is perpendicular to AB, meaning it is the closest to the origin
		return s[0].mkSupport + t * AB;
	}

	/** Finds the closest point to the origin on the triangle defined by three points. Also removes
	 *  the redundant point from the simplex
	 */
	glm::vec2 Triangle(Simplex<Support>& s) {
		NT_ASSERT(s.Size() == 3, "Invalid Simplex during collision detection!");

		// calculate where origin is
		glm::vec3 side1 = glm::vec3(s[1].mkSupport.x - s[2].mkSupport.x,
									s[1].mkSupport.y - s[2].mkSupport.y, 0.0f);
		glm::vec3 side2 = glm::vec3(s[0].mkSupport.x - s[2].mkSupport.x,
									s[0].mkSupport.y - s[2].mkSupport.y, 0.0f);
		glm::vec3 toOrigin = glm::vec3(-s[2].mkSupport.x, -s[2].mkSupport.y, 0.0f);
		glm::vec3 norm1 = glm::cross(glm::cross(side2, side1), side1);
		glm::vec3 norm2 = glm::cross(glm::cross(side1, side2), side2);

		float proj1 = glm::dot(norm1, toOrigin);
		float proj2 = glm::dot(norm2, toOrigin);

		if (proj1 > 0 && proj2 > 0) {
			// either line from s[2] could be closest, we must check
			glm::vec2 p1 = Line(Simplex<Support>({s[0], s[2]}));
			glm::vec2 p2 = Line(Simplex<Support>({s[1], s[2]}));

			if (glm::dot(p1, p1) > glm::dot(p2, p2)) {
				// line using s[1] is closer
				s.Remove(0);
				return p2;
			} else {
				// line using s[0] is closer
				s.Remove(1);
				return p1;
			}
		} else if (proj1 > 0) {
			// origin is in region 2
			s.Remove(0);
			return Line(s);
		} else if (proj2 > 0) {
			// origin is in region 3
			s.Remove(1);
			return Line(s);
		} else if (glm::dot(norm1, norm1) < glm::pow(10, -8)) {
			// triangle is really a line, can only happen if we have already reached the termination
			// condition (i.e. support has moved perpendicularly to direction of origin, therefore,
			// no progress was made)
			s.Remove(2);
			return Line(Simplex<Support>({s[0], s[1]}));
		}

		// Otherwise, origin is in simplex -> collision
		return glm::vec2(0.0f, 0.0f);
	}

	/** Finds the distance between the two objects. Returns the direction and magnitude of the
	 *  shortest vector from any point on p1 to any point on p2.
	 */
	Collision GJKDistance(Nutella::Ref<PhysicsObject>& p1, Nutella::Ref<PhysicsObject>& p2,
						  Simplex<Support>& s, float tolerance);

	/** Calculates a point on p1 and p2 such that the distance between these points is the shortest
	 * distance from any point on p1 to any point on p2. Uses the final simplex created by
	 * GJKDistance as an input to do this.
	 */
	std::pair<glm::vec2, glm::vec2> ComputeWitnessPoints(Nutella::Ref<PhysicsObject>& p1,
														 Nutella::Ref<PhysicsObject>& p2,
														 Simplex<Support>& s);

	/** Finds the closest point on the edge of the Minkowski difference to the origin, and
	 * returns the collision this point describes.
	 */
	Collision EPA(Nutella::Ref<PhysicsObject>& p1, Nutella::Ref<PhysicsObject>& p2,
				  Simplex<Support>& s, float tolerance);

	Collision GJKGetCollision(Nutella::Ref<PhysicsObject>& p1, Nutella::Ref<PhysicsObject>& p2,
							  float tolerance /* = glm::pow(10, -5)*/) {
		NT_PROFILE_FUNC();

		glm::vec2 nextDir = p2->GetPos() - p1->GetPos();
		if (nextDir == glm::vec2(0.0f, 0.0f)) {
			nextDir = glm::vec2(1.0f, 0.0f);
		}

		// add first point to simplex
		Support supportPoint(MinkowskiDiffSupport(p1, p2, nextDir));
		Simplex<Support> s({supportPoint});
		nextDir = -supportPoint.mkSupport;

		// add second point to simplex
		supportPoint = MinkowskiDiffSupport(p1, p2, nextDir);
		s.Add(supportPoint);
		if (glm::dot(nextDir, supportPoint.mkSupport) < 0) {
			// simplex cannot possibly contain origin
			return GJKDistance(p1, p2, s, tolerance);
		}
		nextDir = NextDir(s);

		while (true) {
			// calculate + add next point
			supportPoint = MinkowskiDiffSupport(p1, p2, nextDir);
			s.Add(supportPoint);
			if (glm::dot(nextDir, supportPoint.mkSupport) < 0) {
				// simplex cannot possibly contain origin
				Triangle(s);
				return GJKDistance(p1, p2, s, tolerance);
			}

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

	Collision GJKDistance(Nutella::Ref<PhysicsObject>& p1, Nutella::Ref<PhysicsObject>& p2,
						  Simplex<Support>& s, float tolerance) {
		NT_PROFILE_FUNC();

		glm::vec2 nextDir = -Line(s);
		// check went here

		while (true) {
			s.Add(MinkowskiDiffSupport(p1, p2, nextDir));

			// check if we are no longer making significant progress
			float newSupportProj = glm::dot(nextDir, s[2].mkSupport);
			float oldSupportProj = glm::dot(nextDir, s[1].mkSupport);

			if (newSupportProj - oldSupportProj < tolerance) {
				// error is less than tolerance -> return current distance
				float separationDist = glm::length(nextDir);
				glm::vec2 closestDir = nextDir / separationDist;

				Triangle(s);
				auto [w1, w2] = ComputeWitnessPoints(p1, p2, s);
				// glm::vec2 w1, w2;

				return {p1, p2, false, separationDist, closestDir, w1, w2};
			}

			// find next direction to search in, update simplex
			nextDir = -Triangle(s);
		}
	}

	std::pair<glm::vec2, glm::vec2> ComputeWitnessPoints(Nutella::Ref<PhysicsObject>& p1,
														 Nutella::Ref<PhysicsObject>& p2,
														 Simplex<Support>& s) {
		NT_ASSERT(s.Size() == 2, "Invalid Simplex during collision detection!");

		glm::vec2 p1a = s[0].p1Support;
		glm::vec2 p1b = s[1].p1Support;
		glm::vec2 p2a = s[0].p2Support;
		glm::vec2 p2b = s[1].p2Support;

		// closest point is vertex of simplex -> witness points are whatever produce this vertex
		if (s[0].mkSupport == s[1].mkSupport)
			return {p1a, p2a};

		glm::vec2 segment = s[1].mkSupport - s[0].mkSupport;
		float lambdaB = -glm::dot(segment, s[0].mkSupport) / glm::dot(segment, segment);
		lambdaB = glm::clamp(lambdaB, 0.0f, 1.0f);
		float lambdaA = 1 - lambdaB;

		glm::vec2 closest1 = lambdaA * p1a + lambdaB * p1b;
		glm::vec2 closest2 = lambdaA * p2a + lambdaB * p2b;

		return {closest1, closest2};
	}

	Collision EPA(Nutella::Ref<PhysicsObject>& p1, Nutella::Ref<PhysicsObject>& p2,
				  Simplex<Support>& s, float tolerance) {
		while (true) {
			float closestDist = glm::dot(s[0].mkSupport, s[0].mkSupport);
			glm::vec2 closestDir(s[0].mkSupport);
			uint32_t closestIdx = 1;

			// calculate closest point on current simplex
			for (uint32_t i = 0; i < s.Size(); i++) {
				uint32_t j = i + 1 == s.Size() ? 0 : i + 1;

				glm::vec2 p = Line(Simplex<Support>({s[i], s[j]}));
				float newDist = glm::dot(p, p);

				if (newDist < closestDist) {
					closestDist = newDist;
					closestDir = p;
					closestIdx = i + 1;
				}
			}

			// calculate next point to add to simplex
			if (closestDist < glm::pow(10, -15)) {
				// origin is on edge of simplex -> need to search along edge normals
				glm::vec2 edge = s[closestIdx % s.Size()].mkSupport - s[closestIdx - 1].mkSupport;
				glm::vec2 norm(-edge.y, edge.x);

				// make sure we are searching away from rest of simplex
				if (glm::dot(norm, s[(closestIdx + 1) % s.Size()].mkSupport) > 0) {
					norm = -norm;
				}

				closestDir = norm;
			}

			Support nextPoint = MinkowskiDiffSupport(p1, p2, closestDir);

			// check if we are still making significant progress
			float oldDist = closestDist;
			float newDist = glm::dot(nextPoint.mkSupport, closestDir);

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
