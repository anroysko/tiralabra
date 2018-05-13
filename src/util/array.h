#ifndef __UTIL_ARRAY_H_
#define __UTIL_ARRAY_H_

/// Array with constant size
template<class T>
class Array {
	private:
		/// Raw data.
		T* data;
		/// Size of raw data array.
		const int data_size;
	public:
		/// Initializes array with size "sz".
		Array(int size) : data_size(size) {
			char* ptr = new char[sizeof(T) * size];
			data = new (ptr) T;
		}
		/// Initializes array with all elements equal to "val"
		Array(int size, T val) : data_size(size) {
			char* ptr = new char[sizeof(T) * size];
			data = new (ptr) T;
			for (int i = 0; i < size; ++i) data[i] = val;
		}
		/// Copy constructor.
		/// Initializes array to a copy of the given array.
		Array(const Array& oth) : data_size(oth.data_size) {
			char* ptr = new char[sizeof(T) * oth.data_size];
			data = new (ptr) T;
			memcpy(data, oth.data, sizeof(T) * oth.data_size);
		}
		/// Move constructor.
		/// Leaves oth in a unspecified state.
		Array(Array&& oth) : data_size(oth.data_size) {
			data = oth.data;
			oth.data = nullptr;
		}
		/// Freees data.
		~Array() {
			if (data != nullptr) {
				delete[] data;
				data = nullptr;
			}
		}
		/// Get size of array.
		inline int size() const {
			return data_size;
		}
		/// Access element in array.
		/// Const-qualified version.
		inline const T& operator[](int i) const {
			// assert(0 <= i);
			// assert(i < data_size);
			return data[i];
		}
		/// Access element in array.
		inline T& operator[](int i) {
			// assert(0 <= i);
			// assert(i < data_size);
			return data[i];
		}
};

#endif // __UTIL_ARRAY_H_
