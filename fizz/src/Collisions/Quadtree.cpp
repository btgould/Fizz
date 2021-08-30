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
		CollisionList collisions;
		GetPossibleCollisions(collisions);
		return collisions;
	}

	void Quadtree::GetPossibleCollisions(CollisionList& collisions) {
		// find all collisions between objects in current node
		for (uint32_t i = 0; i < m_Objects.size(); i++) {
			for (u_int32_t j = i + 1; j < m_Objects.size(); j++) {
				if (m_Objects[i]->GetShape()->GetAABB().Intersects(
						m_Objects[j]->GetShape()->GetAABB()))
					collisions.push_back({m_Objects[i], m_Objects[j]});
			}
		}

		if (m_Nodes) {
			// find all collisions between an object in this node and an object in a child node
			for (uint32_t i = 0; i < m_Objects.size(); i++) {
				for (uint32_t j = 0; j < 4; j++) {
					m_Nodes[j].GetPossibleChildCollisions(m_Objects[i], collisions);
				}
			}

			// find all collisions in child nodes
			for (uint32_t i = 0; i < 4; i++) {
				m_Nodes[i].GetPossibleCollisions(collisions);
			}
		}
	}

	void Quadtree::GetPossibleChildCollisions(const Nutella::Ref<PhysicsObject>& object,
											  CollisionList& collisions) {
		for (auto& other : m_Objects) {
			if (other->GetShape()->GetAABB().Intersects(object->GetShape()->GetAABB())) {
				collisions.push_back({object, other});
			}
		}

		if (m_Nodes) {
			for (uint32_t i = 0; i < 4; i++) {
				m_Nodes[i].GetPossibleChildCollisions(object, collisions);
			}
		}
	}

	std::vector<Nutella::Ref<PhysicsObject>> Quadtree::GetPossibleCollisions(const AABB& bounds) {
		std::vector<Nutella::Ref<PhysicsObject>> collisions;
		GetPossibleCollisions(bounds, collisions);
		return collisions;
	}

	void Quadtree::GetPossibleCollisions(const AABB& bounds,
										 std::vector<Nutella::Ref<PhysicsObject>>& collisions) {
		// check AABB against all objects in current node
		for (auto other : m_Objects) {
			if (other->GetShape()->GetAABB().Intersects(bounds)) {
				collisions.push_back(other);
			}
		}

		// check AABB against all objects in applicable child nodes
		if (m_Nodes) {
			for (uint32_t i = 0; i < 4; i++)
				if (m_Nodes[i].m_Bounds.Intersects(bounds)) {
					GetPossibleCollisions(bounds, collisions);
				}
		}
	}
} // namespace Fizz
