#ifndef __UTIL_TWO_DIM_ARRAY_H_
#define __UTIL_TWO_DIM_ARRAY_H_

#include "vector.h" // Vector
#include "string.h" // memcpy

/// Struct representing a two-dimensional array
/// Has the data better aligned than two Vectors inside each other,
/// so it is faster. This approach doesn't allow resizing though.
/// Does lots of copying, so should not be used with large structs.
/// primitives like ints/pointers are good though.
template<class T>
class TwoDimArray {
	private:
		/// Raw data
		T* data;
		/// Where does each array start?
		/// offset[i] is where array "i" starts.
		int* offset;
		/// How much raw data is there in total
		int total;
		/// How many arrays do we have.
		int size;
	
		/// Allocate space for the data.
		/// Make sure to clear first.
		void reserve(int total, int size) {
			this->total = total;
			this->size = size;
			if (size == 0) {
				data = nullptr;
				offset = nullptr;
			} else {
				this->data = new T[total];
				this->offset = new int[size];
			}
		}

		/// Clears all data.
		/// After this the object is in the same state as a default-constructed equivalent.
		/// Does the same thing as the destructor.
		void clear() {
			if (data != nullptr) delete[] data;
			if (offset != nullptr) delete[] offset;
			data = nullptr;
			offset = nullptr;
			total = 0;
			size = 0;
		}
	public:
		/// Initializes the struct into a empty state.
		/// Only useful if you wish to assign to it later.
		TwoDimArray() {
			reserve(0, 0);
		}
		/// Copy constructor.
		TwoDimArray(const TwoDimArray& oth) {
			reserve(oth.total, oth.size);
			memcpy(offset, oth.offset, sizeof(int) * size);
			memcpy(data, oth.data, sizeof(T) * total);
			// for (int i = 0; i < size; ++i) offset[i] = oth.offset[i];
			// for (int i = 0; i < total; ++i) data[i] = oth.data[i];
		}
		/// Move constructor.
		/// Clears oth.
		TwoDimArray(TwoDimArray&& oth) {
			data = oth.data;
			offset = oth.offset;
			total = oth.total;
			size = oth.size;
		
			oth.clear();
		}
		/// Initialize with sizes of the arrays.
		/// Total is the total size of all the arrays.
		TwoDimArray(int total, const Vector<int>& sizes) {
			reserve(total, sizes.size());
			for (int i = 0, sum = 0; i < size; sum += sizes[i], ++i) offset[i] = sum;
		}
		/// Initialize with offsets
		/// Total is the total size of all the arrays.
		/// Size is the array count.
		TwoDimArray(int total, int size, const int* offset) {
			reserve(total, size);
			memcpy(this->offset, offset, sizeof(int) * size);
			// for (int i = 0; i < size; ++i) this->offset[i] = offset[i];
		}
		/// Frees data.
		~TwoDimArray() {
			if (data != nullptr) delete[] data;
			if (offset != nullptr) delete[] offset;
			data = nullptr;
			offset = nullptr;
		}
		/// Array count.
		inline int getSize() const {
			return size;
		}
		/// Size of array i.
		inline int getArraySize(int x) const {
			// assert(0 <= x);
			// assert(x < size);
			if (x != size - 1) return offset[x + 1] - offset[x];
			else return total - offset[size - 1];
		}
		/// Returns the array of offsets.
		/// So that it can be used to initialize other TwoDimArray s.
		const int* getOffset() const {
			return offset;
		}
		/// Get array "x".
		inline T* operator[] (int x) {
			// assert(0 <= x);
			// assert(x < size);
			return &data[offset[x]];
		}
		/// Get array "x".
		/// The const version. Doesn't allow modifying the elements.
		inline const T* operator[] (int x) const {
			// assert(0 <= x);
			// assert(x < size);
			return &data[offset[x]];
		}
		/// Copy assignment.
		TwoDimArray& operator=(const TwoDimArray& oth) {
			clear();
			reserve(oth.total, oth.size);
			memcpy(data, oth.data, sizeof(T) * total);
			memcpy(offset, oth.offset, sizeof(int) * size);
			// for (int i = 0; i < total; ++i) data[i] = oth.data[i];
			// for (int i = 0; i < size; ++i) offset[i] = oth.offset[i];
		}
		/// Move assignment.
		/// Clears oth.
		TwoDimArray& operator=(TwoDimArray&& oth) {
			clear();

			size = oth.size;
			total = oth.total;
			data = oth.data;
			offset = oth.offset;

			// Do not call clear() here, that deallocates the moved data!
			oth.size = 0;
			oth.total = 0;
			oth.data = nullptr;
			oth.offset = nullptr;
		}
};

#endif // __UTIL_TWO_DIM_ARRAY_H_
