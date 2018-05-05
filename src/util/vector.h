#ifndef __UTIL_VECTOR_H_
#define __UTIL_VECTOR_H_

#include <assert.h> // assert
#include <string.h> // memcpy

/// Dynamic array.
/// Does a lot of copying. Should not be used with large structs.
/// ints/pointers are ideal.
/// Also has no minimum size, so it is slow when pushing/poppin alternatively at zero or one elements.
template<class T>
class Vector {
	private:
		T* data; /// Raw data.
		int el_count; /// Amount of elements.
		int capacity; /// How large is data?
	public:
		/// Regular constructor.
		Vector() {
			this->el_count = 0;
			this->capacity = 0;
			this->data = nullptr;
		}
		/// Construct with size "size".
		/// Initial state of elements is undefined.
		Vector(int size) {
			this->el_count = size;
			this->capacity = size;
			this->data = new T[capacity];
		}
		/// Construct with size "size" and all elements being "val".
		Vector(int size, T val) {
			this->el_count = size;
			this->capacity = size;
			this->data = new T[capacity];
			// TODO: speed this up
			for (int i = 0; i < size; ++i) data[i] = val;
		}
		/// Copy from other Vector.
		Vector(const Vector& oth) {
			el_count = oth.el_count;
			capacity = oth.el_count;
			data = new T[capacity];
			memcpy(data, oth.data, sizeof(T) * capacity);
			// for (int i = 0; i < capacity; ++i) data[i] = oth.data[i];
		}
		/// Move from other Vector.
		/// Sets "oth" to clear() ed state.
		Vector(Vector&& oth) {
			el_count = oth.el_count;
			capacity = oth.capacity;
			data = oth.data;
			
			oth.el_count = 0;
			oth.capacity = 0;
			oth.data = nullptr;
		}

		/// Destructor.
		/// Simply frees data.
		~Vector() {
			if (data != nullptr) {
				delete[] data;
				data = nullptr;
			}
		}

		/// Clears the vector of elements.
		/// Does the exact same thing as the destructor.
		void clear() {
			if (data != nullptr) {
				delete[] data;
				data = nullptr;
			}
			el_count = 0;
			capacity = 0;
		}
		/// Reserve space.
		/// Use this so that the vector doesn't need to realloc, If you know how large it will get.
		/// Also used internally.
		void reserve(int space) {
			// assert(space >= el_count);
			if (space == 0) {
				clear();
			} else {
				T* new_data = new T[space];
				memcpy(new_data, data, sizeof(T) * el_count);
				// for (int i = 0; i < el_count; ++i) new_data[i] = data[i];
				if (data != nullptr) delete[] data;
				
				capacity = space;
				data = new_data;
			}
		}
		// Inline small functions
		/// Shrinks the vector to have capacity equal to its size.
		inline void shrink() {
			reserve(el_count);
		}
		/// Resizes the vector.
		/// Sets amount of elements to "size".
		/// Elements in newly created indexes are in a unspecified state.
		inline void resize(int size) {
			if (capacity < size) reserve(size);
			el_count = size;
		}
		/// Push element to the back of the vector.
		inline void push(T val) {
			// 2 * (capacity + 1) so that after pushing back capacity is 2 * el_count.
			if (el_count == capacity) {
				reserve(2 * (capacity + 1));
			}
			data[el_count] = val;
			++el_count;
		}
		/// Pop from the back of the vector.
		/// To free memory, you need to call shrink()
		inline void pop() {
			// assert(el_count > 0);
			--el_count;
		}
		/// Get back element in the vector.
		inline const T& back() const {
			// assert(el_count > 0);
			return data[el_count - 1];
		}
		/// Get the amount of elements in the vector.
		inline int size() const {
			return el_count;
		}
		/// Get element at index.
		inline const T& operator[] (int i) const {
			// assert(0 <= i < el_count);
			return data[i];
		}
		/// Get element at index.
		inline T& operator[] (int i) {
			// assert(0 <= i);
			// assert(i < el_count);
			return data[i];
		}

		/// Copy assignment.
		/// Same as copy constructor, expect that it clears first.
		Vector& operator=(const Vector& oth) {
			clear();
			el_count = oth.el_count;
			capacity = oth.el_count;
			data = new T[capacity];
			memcpy(data, oth.data, sizeof(T) * capacity);
			// for (int i = 0; i < capacity; ++i) data[i] = oth.data[i];
		}

		/// Move assignment.
		/// Same as move constructor, expect that it clears first.
		/// Sets "oth" to clear() ed state.
		Vector& operator=(Vector&& oth) {
			clear();
			el_count = oth.el_count;
			capacity = oth.capacity;
			data = oth.data;
			
			oth.el_count = 0;
			oth.capacity = 0;
			oth.data = nullptr;
		}
};

#endif //__UTIL_VECTOR_H_
