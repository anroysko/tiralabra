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
	/// Minimum capacity a vector can have.
	/// Capacity is always at least this.
	const static int MIN_SIZE = 10;
	/// How much larger the vector gets when it requires more space.
	const static int GROWTH_FACTOR = 2;

	private:
		T* data; /// Raw data.
		int el_count; /// Amount of elements.
		int capacity; /// How large is data?
	public:
		/// Regular constructor.
		Vector() {
			el_count = 0;
			capacity = MIN_SIZE;
			data = new T[MIN_SIZE];
		}
		/// Construct with size "ini_size".
		/// Initial state of elements is undefined.
		Vector(int size) {
			el_count = size;
			capacity = (size < MIN_SIZE ? MIN_SIZE : size);
			data = new T[capacity];
		}
		/// Construct with size "size" and all elements being "val".
		Vector(int size, T val) {
			el_count = size;
			capacity = (size < MIN_SIZE ? MIN_SIZE : size);
			data = new T[capacity];
			// TODO: speed this up
			for (int i = 0; i < size; ++i) data[i] = val;
		}
		/// Copy from other Vector.
		Vector(const Vector& oth) {
			el_count = oth.el_count;
			capacity = (oth.el_count < MIN_SIZE ? MIN_SIZE : oth.el_count);
			data = new T[capacity];
			memcpy(data, oth.data, sizeof(T) * oth.el_count);
			// for (int i = 0; i < capacity; ++i) data[i] = oth.data[i];
		}
		/// Move from other Vector.
		/// Does not guarantee anything about the state of oth.
		Vector(Vector&& oth) {
			el_count = oth.el_count;
			capacity = oth.capacity;
			data = oth.data;
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
			T* new_data = new T[space];
			memcpy(new_data, data, sizeof(T) * el_count);
			// for (int i = 0; i < el_count; ++i) new_data[i] = data[i];
			if (data != nullptr) delete[] data;	
			capacity = space;
			data = new_data;
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
				reserve(GROWTH_FACTOR * (capacity + 1));
			}
			data[el_count] = val;
			++el_count;
		}
		/// Pop from the back of the vector.
		/// You can free memory by calling reserve on smaller one.
		inline void pop() {
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
		Vector& operator=(const Vector& oth) {
			el_count = oth.el_count;
			capacity = (oth.el_count < MIN_SIZE ? MIN_SIZE : oth.el_count);
			delete[] data;
			data = new T[capacity];
			memcpy(data, oth.data, sizeof(T) * capacity);
			// for (int i = 0; i < capacity; ++i) data[i] = oth.data[i];
		}

		/// Move assignment.
		/// Sets "oth" to an unspecified state. 
		Vector& operator=(Vector&& oth) {
			el_count = oth.el_count;
			capacity = oth.capacity;
			delete[] data;
			data = oth.data;
			
			oth.data = nullptr;
		}
};

#endif //__UTIL_VECTOR_H_
