#ifndef ARDUINO_COMPAT_H
#define ARDUINO_COMPAT_H
#include <cstddef> 



// std::Move 
template <typename T>
T&& Move(T& t) {
    return static_cast<T&&>(t);
}


// std::Forward 
template <typename T>
T&& Forward(T& t) {
    return static_cast<T&&>(t);
}

//std::Pair
template <typename T1, typename T2>
struct Pair {
    T1 first;
    T2 second;
    Pair() : first(), second() {}
    Pair(T1&& f, T2&& s) : first(Move(f)), second(Move(s)) {}
};


// std::memcpy    (raw byte copy)
template <typename T>
inline void compat_memcpy(T* dest, const T* src, std::size_t count) {
    for (std::size_t i = 0; i < count; ++i) {
        dest[i] = src[i];
    }
}





// std::unique_ptr  
// for single objects
template <typename T>
class UniquePtr {
private:
    T* ptr;
public:
    explicit UniquePtr(T* p = nullptr) : ptr(p) {}
    ~UniquePtr() { delete ptr; }

    T* get() const { return ptr; }

    T* release() {
        T* temp = ptr;
        ptr = nullptr;
        return temp;
    }

    void reset(T* p = nullptr) {
        delete ptr;
        ptr = p;
    }

    T& operator*() const { return *ptr; }
    T* operator->() const { return ptr; }

    UniquePtr(UniquePtr&& other) : ptr(other.ptr) { other.ptr = nullptr; }
    UniquePtr& operator=(UniquePtr&& other) {
        if (this != &other) {
            delete ptr;
            ptr = other.ptr;
            other.ptr = nullptr;
        }
        return *this;
    }

    UniquePtr(const UniquePtr&) = delete;
    UniquePtr& operator=(const UniquePtr&) = delete;
};

// for arrays 
template <typename T>
class UniquePtr<T[]> {
private:
    T* ptr;
public:
    explicit UniquePtr(T* p = nullptr) : ptr(p) {}
    ~UniquePtr() { delete[] ptr; }

    T* get() const { return ptr; }

    T& operator[](size_t index) { return ptr[index]; }
    const T& operator[](size_t index) const { return ptr[index]; }

    T* release() {
        T* temp = ptr;
        ptr = nullptr;
        return temp;
    }

    void reset(T* p = nullptr) {
        delete[] ptr;
        ptr = p;
    }

    UniquePtr(UniquePtr&& other) : ptr(other.ptr) { other.ptr = nullptr; }
    UniquePtr& operator=(UniquePtr&& other) {
        if (this != &other) {
            delete[] ptr;
            ptr = other.ptr;
            other.ptr = nullptr;
        }
        return *this;
    }

    UniquePtr(const UniquePtr&) = delete;
    UniquePtr& operator=(const UniquePtr&) = delete;
};


// std::make_unique 
template <typename T>
UniquePtr<T[]> make_unique(std::size_t size) {
    return UniquePtr<T[]>(new T[size]()); 
}


#endif // ARDUINO_COMPAT_H
