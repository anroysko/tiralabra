#ifndef __UTIL_TWO_DIM_ARRAY_H_
#define __UTIL_TWO_DIM_ARRAY_H_

#include <vector>
#include <iostream>

// Struct representing a two-dimensional array
// Template and inlines so defined in header
template<class T>
struct TwoDimArray {
	T* data;
	int* offset;
	int total_size;
	int size;

	TwoDimArray() {
		data = nullptr;
		offset = nullptr;
		total_size = 0;
		size = 0;
	}
	TwoDimArray(int total_size, const std::vector<int>& sizes) {
		std::cout << "init twodimarray: " << total_size << "\n";
		for (auto it : sizes) std::cout << it << ' '; std::cout << '\n';
		this->total_size = total_size;
		size = sizes.size();
		data = new T[total_size];
		offset = new int[size];
		for (int i = 0, sum = 0; i < size; ++i, sum += sizes[i]) offset[i] = sum;
		print();
	}
	TwoDimArray(int total_size, int size, const int* sizes) {
		this->total_size = total_size;
		this->size = size;
		data = new T[total_size];
		offset = new int[size];
		for (int i = 0, sum = 0; i < size; ++i, sum += sizes[i]) offset[i] = sum;
	}
	~TwoDimArray() {
		if (data != nullptr) delete[] data;
		if (offset != nullptr) delete[] offset;
		data = nullptr;
		offset = nullptr;
	}
	inline T* operator[] (int x) {
		return &data[offset[x]];
	}
	inline int getSize() const {
		return size;
	}
	inline int getArraySize(int x) const {
		if (x != size - 1) return offset[x + 1] - offset[x];
		else return total_size - offset[size - 1];
	}
	void print() {
		std::cout << "Twodimarray " << total_size << " " << size << "\n";
		for (int i = 0; i < size; ++i) std::cout << offset[i] << " "; std::cout << "\n";
		for (int i = 0; i < total_size; ++i) std::cout << data[i] << " "; std::cout << "\n";
	}
};

#endif // __UTIL_TWO_DIM_ARRAY_H_
