#ifndef INCLUDE_VECTOR_H_
#define INCLUDE_VECTOR_H_

#include <iostream>
#include <stdexcept>
#include <cassert>
#include <algorithm>

const int MAX_VECTOR_SIZE = 100000000;

template<typename T>
class TVector {
public:
    size_t _size;
    size_t _capacity;
    T* pMem;

    TVector(size_t size = 1) : _size(size), _capacity(size) {
    if (size == 0)
        throw std::length_error("Vector size should be greater than zero");
    if (size > MAX_VECTOR_SIZE)
        throw std::length_error("Vector size cannot be greater than MAX_VECTOR_SIZE");
    pMem = new T[size];
}


    TVector(const T* data, size_t size) : _size(size), _capacity(size) {
        assert(data != nullptr && "TVector constructor requires non-nullptr argument.");
        pMem = new T[size];
        std::copy(data, data + size, pMem);
    }

    TVector(const TVector& v) : _size(v._size), _capacity(v._capacity), pMem(new T[v._capacity]) {
        std::copy(v.pMem, v.pMem + v._size, pMem);
    }

    TVector(TVector&& v) noexcept : _size(0), _capacity(0), pMem(nullptr) {
        swap(*this, v);
    }

    TVector(size_t size, const T& value) : _size(size), _capacity(size) {
        if (size == 0)
            throw std::length_error("Vector size should be greater than zero");
        if (size > MAX_VECTOR_SIZE)
            throw std::length_error("Vector size cannot be greater than MAX_VECTOR_SIZE");
        pMem = new T[size];
        std::fill(pMem, pMem + size, value);
    }

    ~TVector() {
        delete[] pMem;
    }

    TVector& operator=(TVector v) {
        swap(*this, v);
        return *this;
    }

    size_t size() const noexcept {
        return _size;
    }

    size_t capacity() const {
    return _capacity;
    }

    void reallocate(size_t new_capacity) {
    T* newMem = new T[new_capacity];  // Новый массив памяти

    // Копируем старые элементы
    for (size_t i = 0; i < _size; ++i) {
        newMem[i] = pMem[i];
    }

    delete[] pMem;  // Освобождаем старую память
    pMem = newMem;
    _capacity = new_capacity;
}


    void resize(size_t new_size) {
    if (new_size > _capacity) {
        reallocate(new_size);
    }
    for (size_t i = _size; i < new_size; ++i) {
        pMem[i] = T();  // Явная инициализация
    }
    _size = new_size;
}


    void push_back(const T& value) {
    if (_size == _capacity) {
        size_t new_capacity = (_capacity == 0) ? 1 : _capacity * 2;
        reallocate(new_capacity);
    }
    pMem[_size++] = value;  // Добавляем элемент
}


    const T& operator[](size_t index) const {
    if (index >= _size) {
        throw std::out_of_range("Index out of range");
    }
    return pMem[index];
    }

    T& operator[](size_t index) {
    if (index >= _size) {
        throw std::out_of_range("Index out of range");
    }
    return pMem[index];
}

    T& at(size_t ind) {
        if (ind >= _size) {
            throw std::out_of_range("Index out of range");
        }
        return pMem[ind];
    }

    const T& at(size_t ind) const {
        if (ind >= _size) {
            throw std::out_of_range("Index out of range");
        }
        return pMem[ind];
    }

    bool operator==(const TVector& v) const noexcept {
        if (_size != v._size) {
            return false;
        }
        for (size_t i = 0; i < _size; ++i) {
            if (pMem[i] != v.pMem[i]) {
                return false;
            }
        }
        return true;
    }

    bool operator!=(const TVector& v) const noexcept {
        return !(*this == v);
    }

    TVector operator+(T val) const {
        TVector res(*this);
        for (size_t i = 0; i < _size; ++i) {
            res.pMem[i] += val;
        }
        return res;
    }

    TVector operator-(T val) const {
        TVector res(*this);
        for (size_t i = 0; i < _size; ++i) {
            res.pMem[i] -= val;
        }
        return res;
    }

    TVector operator*(T val) const {
        TVector res(*this);
        for (size_t i = 0; i < _size; ++i) {
            res.pMem[i] *= val;
        }
        return res;
    }

    T operator*(const TVector& v) const {
        if (_size != v._size) {
            throw std::length_error("Dimension mismatch");
        }
        T dotProduct = 0;
        for (size_t i = 0; i < _size; ++i) {
            dotProduct += pMem[i] * v.pMem[i];
        }
        return dotProduct;
    }

    TVector operator+(const TVector& v) const {
        if (_size != v._size) {
            throw std::length_error("Dimension mismatch");
        }
        TVector res(*this);
        for (size_t i = 0; i < _size; ++i) {
            res.pMem[i] += v.pMem[i];
        }
        return res;
    }

    TVector operator-(const TVector& v) const {
        if (_size != v._size) {
            throw std::length_error("Dimension mismatch");
        }
        TVector res(*this);
        for (size_t i = 0; i < _size; ++i) {
            res.pMem[i] -= v.pMem[i];
        }
        return res;
    }

    friend void swap(TVector& lhs, TVector& rhs) noexcept {
        std::swap(lhs._size, rhs._size);
        std::swap(lhs._capacity, rhs._capacity);
        std::swap(lhs.pMem, rhs.pMem);
    }

    friend std::istream& operator>>(std::istream& istr, TVector& v) {
        for (size_t i = 0; i < v._size; ++i) {
            istr >> v.pMem[i];
        }
        return istr;
    }

    friend std::ostream& operator<<(std::ostream& ostr, const TVector& v) {
        for (size_t i = 0; i < v._size; ++i) {
            ostr << v.pMem[i] << ' ';
        }
        return ostr;
    }

    T* begin() {
        return pMem;
    }

    T* end() {
        return pMem + _size;
    }
};

#endif  // INCLUDE_VECTOR_H_