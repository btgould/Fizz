#pragma once

#include <glm/glm.hpp>
#include <vector>

namespace Fizz {
	/* Represents a set of points in 2 dimensional space. Technically, simplices are always affinely
	   independent, meaning they can never have more than 3 points (in 2D space). However, this
	   class has a somewhat looser definition, and is used more as an ordered collection of points.
	 */
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

		/* Adds the given point to the end of the simplex.

		   @param point: The point to add
		 */
		void Add(const glm::vec2& point);

		/* Adds the given point simplex at the specified index. All points after the index are moved
		   back to make room for the new point, which will occupy the given index after insertion.

		   @param point: The point to add
		   @param idx: The index to insert the point at
		 */
		void Add(const glm::vec2& point, uint32_t idx);

		/** Removes the given point from the simplex.
		 *
		 * @param point: The point to remove
		 */
		void Remove(const glm::vec2& point);

		/** Removes the point at the given index from the simplex.
		 *
		 * @param idx: The index of the point to remove
		 */
		void Remove(const uint32_t idx);

		/* Gets the number of points in the simplex */
		uint32_t Size() const;

		glm::vec2& operator[](uint32_t index);
		const glm::vec2& operator[](uint32_t index) const;

		Iterator begin();
		Iterator end();

	  private:
		std::vector<glm::vec2> m_Points;
	};
} // namespace Fizz
