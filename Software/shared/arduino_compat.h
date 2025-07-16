#ifndef ARDUINO_COMPAT_H
#define ARDUINO_COMPAT_H
#include <stddef.h>       
                        

template<typename T> struct _rr          { using type = T; };
template<typename T> struct _rr<T&>      { using type = T; };
template<typename T> struct _rr<T&&>     { using type = T; };

template<typename T>
using remove_reference_t = typename _rr<T>::type;

template<typename T>
constexpr remove_reference_t<T>&& Move(T&& t) noexcept {
    return static_cast<remove_reference_t<T>&&>(t);
}

template<typename T>
constexpr T Min(T a, T b) { return a < b ? a : b; }

template<typename T>
constexpr remove_reference_t<T>&& Forward(remove_reference_t<T>& t) noexcept {
    return static_cast<remove_reference_t<T>&&>(t);
}


//std::Pair
template <typename T1, typename T2>
struct Pair {
    T1 first;
    T2 second;
    Pair() : first(), second() {}
    Pair(T1&& f, T2&& s) : first(Move(f)), second(Move(s)) {}
};


template <typename T>
inline void compat_memcpy(T* dest, const T* src, size_t count) {
    for (size_t i = 0; i < count; ++i) {
        dest[i] = src[i];
    }
}





template <typename T>
class UniquePtr {
private:
    T* ptr;
public:
    explicit UniquePtr(T* p = nullptr) : ptr(p) {}
    ~UniquePtr() { delete ptr; }

    T* get() const { return ptr; }

    T& operator*() { return *ptr; }
    T* operator->() { return ptr; }

    T* release() {
        T* temp = ptr;
        ptr = nullptr;
        return temp;
    }

    void reset(T* p = nullptr) {
        delete ptr;
        ptr = p;
    }

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



template <typename T>
UniquePtr<T[]> make_unique(size_t size) {
    return UniquePtr<T[]>(new T[size]());
}


#endif
