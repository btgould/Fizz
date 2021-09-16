#pragma once

#include <glm/glm.hpp>
#include <vector>

namespace Fizz {

	/* Represents a set of points in 2 dimensional space. Technically, simplices are always affinely
	   independent, meaning they can never have more than 3 points (in 2D space). However, this
	   class has a somewhat looser definition, and is used more as an ordered collection of points.
	 */
	template <typename T> class Simplex {
	  private:
		class Iterator {

		  public:
			Iterator(T* ptr) : m_Ptr(ptr) {}

			inline T& operator*() { return *m_Ptr; }

			inline Iterator& operator++() {
				m_Ptr++;
				return *this;
			}

			inline Iterator operator++(int) {
				Simplex::Iterator temp = *this;
				++(*this);
				return temp;
			}

			inline Iterator& operator--() {
				m_Ptr--;
				return *this;
			}

			inline Iterator operator--(int) {
				Simplex::Iterator temp = *this;
				--(*this);
				return temp;
			}

			inline bool operator==(const Iterator& rhs) { return m_Ptr == rhs.m_Ptr; }
			inline bool operator!=(const Iterator& rhs) { return !(*this == rhs); }

		  private:
			T* m_Ptr;
		};

	  public:
		Simplex() {}
		Simplex(std::vector<T> points) : m_Points(points) {}
		~Simplex() {}

		/* Adds the given point to the end of the simplex.

		   @param point: The point to add
		 */
		inline void Add(const T& point) { m_Points.push_back(point); }

		/* Adds the given point simplex at the specified index. All points after the index are moved
		   back to make room for the new point, which will occupy the given index after insertion.

		   @param point: The point to add
		   @param idx: The index to insert the point at
		 */
		inline void Add(const T& point, uint32_t idx) {
			m_Points.insert(m_Points.begin() + idx, point);
		}

		/** Removes the given point from the simplex.
		 *
		 * @param point: The point to remove
		 */
		inline void Remove(const T& point) {
			for (int i = 0; i < m_Points.size(); i++) {
				if (m_Points[i] == point) {
					m_Points.erase(m_Points.begin() + i);
					return;
				}
			}
		}

		/** Removes the point at the given index from the simplex.
		 *
		 * @param idx: The index of the point to remove
		 */
		inline void Remove(const uint32_t idx) { m_Points.erase(m_Points.begin() + idx); }

		/* Gets the number of points in the simplex */
		inline uint32_t Size() const { return m_Points.size(); }

		inline T& operator[](uint32_t index) { return m_Points[index]; }
		inline const T& operator[](uint32_t index) const { return m_Points[index]; }

		inline Iterator begin() { return Iterator(&m_Points[0]); }
		inline Iterator end() { return Iterator(&m_Points[0] + m_Points.size()); }

	  private:
		std::vector<T> m_Points;
	};
} // namespace Fizz
