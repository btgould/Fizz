#pragma once

#include <glm/glm.hpp>
#include <vector>

namespace Fizz {
	class Simplex {
	  private:
		class Iterator {
		  public:
			Iterator(glm::vec2* ptr);

			glm::vec2& operator*();

			Iterator& operator++();
			Iterator operator++(int);
			Iterator& operator--();
			Iterator operator--(int);

			bool operator==(const Iterator& rhs);
			inline bool operator!=(const Iterator& rhs) { return !(*this == rhs); }

		  private:
			glm::vec2* m_Ptr;
		};

	  public:
		Simplex();
		Simplex(std::vector<glm::vec2> points);
		~Simplex();

		void Add(const glm::vec2& point);
		void Remove(const glm::vec2& point);

		uint32_t Size();

		glm::vec2& operator[](uint32_t index);

		Iterator begin();
		Iterator end();

	  private:
		std::vector<glm::vec2> m_Points;
	};
} // namespace Fizz
