#include "Simplex.hpp"

namespace Fizz {
	Simplex::Iterator::Iterator(glm::vec2* ptr) : m_Ptr(ptr) {}

	glm::vec2& Simplex::Simplex::Iterator::operator*() { return *m_Ptr; }

	Simplex::Iterator& Simplex::Simplex::Iterator::operator++() {
		m_Ptr++;
		return *this;
	}
	Simplex::Iterator Simplex::Simplex::Iterator::operator++(int) {
		Simplex::Iterator temp = *this;
		++(*this);
		return temp;
	}
	Simplex::Iterator& Simplex::Simplex::Iterator::operator--() {
		m_Ptr--;
		return *this;
	}
	Simplex::Iterator Simplex::Simplex::Iterator::operator--(int) {
		Simplex::Iterator temp = *this;
		++(*this);
		return temp;
	}

	bool Simplex::Simplex::Iterator::operator==(const Simplex::Iterator& other) {
		return m_Ptr == other.m_Ptr;
	}

	Simplex::Simplex() {}

	Simplex::Simplex(std::vector<glm::vec2> points) : m_Points(points) {}

	Simplex::~Simplex() {}

	void Simplex::Add(const glm::vec2& point) { m_Points.emplace_back(point); }

	void Simplex::Remove(const glm::vec2& point) {
		for (int i = 0; i < m_Points.size(); i++) {
			if (m_Points[i] == point) {
				m_Points.erase(m_Points.begin() + i);
				return;
			}
		}
	}

	uint32_t Simplex::Size() { return m_Points.size(); }

	glm::vec2& Simplex::operator[](uint32_t index) { return m_Points[index]; }

	Simplex::Iterator Simplex::begin() { return Iterator(&m_Points[0]); }

	Simplex::Iterator Simplex::end() { return Iterator(&m_Points[0] + m_Points.size()); }
} // namespace Fizz
