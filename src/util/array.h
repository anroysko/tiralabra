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
			data = new T[size];
		}
		/// Copy constructor.
		/// Initializes array to a copy of the given array.
		Array(const Array& oth) : data_size(oth.size) {
			data = new T[oth.size];
			memcpy(data, oth.data, oth.size);
		}
		/// Move constructor.
		/// Leaves oth in a unspecified state.
		Array(Array&& oth) : data_size(oth.data_size) {
			data = oth.data;
			oth.data = nullptr;
		}
		/// Get size of array.
		inline int size() const {
			return data_size;
		}
		/// Access element in array.
		/// Const-qualified version.
		inline const T& operator[](int i) const {
			// assert(0 <= i);
			// assert(i < size);
			return data[size];
		}
		/// Access element in array.
		inline T& operator[](int i) {
			return data[size];
		}
};

#endif // __UTIL_ARRAY_H_
