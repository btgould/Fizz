#include "Quadtree.hpp"

namespace Fizz {
	const uint32_t Quadtree::MAX_LEVELS = 5;

	Quadtree::Quadtree(uint32_t level, const AABB& bounds)
		: m_Level(level), m_Bounds(bounds), m_Nodes(nullptr) {}

	Quadtree::~Quadtree() {
		if (m_Nodes) {
			m_Nodes[0].~Quadtree();
			m_Nodes[1].~Quadtree();
			m_Nodes[2].~Quadtree();
			m_Nodes[3].~Quadtree();
		}
	}

	void Quadtree::Clear() {
		m_Objects.clear();

		if (m_Nodes) {
			m_Nodes[0].~Quadtree();
			m_Nodes[1].~Quadtree();
			m_Nodes[2].~Quadtree();
			m_Nodes[3].~Quadtree();
		}
	}

	void Quadtree::Insert(Nutella::Ref<PhysicsObject>& object) {
		// calculate bounds of children
		glm::vec2 halfWidth = glm::vec2(m_Bounds.GetWidth() / 2, 0.0f);
		glm::vec2 halfHeight = glm::vec2(0.0f, m_Bounds.GetHeight() / 2);

		AABB tl = AABB(m_Bounds.min + halfHeight, m_Bounds.max - halfWidth);
		AABB tr = AABB(m_Bounds.min + halfWidth + halfHeight, m_Bounds.max);
		AABB bl = AABB(m_Bounds.min, m_Bounds.max - halfWidth - halfHeight);
		AABB br = AABB(m_Bounds.min + halfWidth, m_Bounds.max - halfHeight);
		AABB shape = object->GetShape()->GetAABB();

		// check if shape can be contained by any smaller children
		int idx = -1;
		if (tl.Contains(shape)) {
			idx = 0;
		} else if (tr.Contains(shape)) {
			idx = 1;
		} else if (bl.Contains(shape)) {
			idx = 2;
		} else if (br.Contains(shape)) {
			idx = 3;
		}

		if (idx != -1 && m_Level < MAX_LEVELS) {
			// shape can be contained in a child node -> add to child
			// create children if needed
			if (!m_Nodes) {
				m_Nodes = (Quadtree*) malloc(4 * sizeof(Quadtree));
				new (&m_Nodes[0]) Quadtree(m_Level + 1, tl);
				new (&m_Nodes[1]) Quadtree(m_Level + 1, tr);
				new (&m_Nodes[2]) Quadtree(m_Level + 1, bl);
				new (&m_Nodes[3]) Quadtree(m_Level + 1, br);
			}

			m_Nodes[idx].Insert(object);
		} else {
			m_Objects.push_back(object);
		}
	}

	CollisionList Quadtree::GetPossibleCollisions() {
		CollisionList collisionList;

		// Add all objects from children
		if (m_Nodes) {
			for (uint32_t i = 0; i < 4; i++) {
				CollisionList child = m_Nodes[i].GetPossibleCollisions();
				collisionList.insert(collisionList.end(), child.begin(), child.end());
			}
		}

		// Add objects in parent to each child list
		if (m_Objects.size() != 0) {
			if (collisionList.size() == 0) {
				// no objects in any child list, add parent list unmodified
				collisionList.push_back(m_Objects);
			} else {
				// child lists exist, add all parent objects to each
				for (auto& collisionSet : collisionList)
					collisionSet.insert(collisionSet.end(), m_Objects.begin(), m_Objects.end());
			}
		}

		return collisionList;
	}
} // namespace Fizz
