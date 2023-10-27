#include <iostream>
#include <memory>
#include <cassert>


template <typename T>
class Grid final {
public:
    using value_type = T;
    using size_type = unsigned;

    Grid() : data(nullptr), y_size(0), x_size(0) {}

    explicit Grid(T const& t) : data(new T[1]{ t }), y_size(1), x_size(1) {}

    explicit Grid(size_type y_size, size_type x_size) :
        data(new T[y_size * x_size]()), y_size(y_size), x_size(x_size) {}

    Grid(size_type y_size, size_type x_size, T const& t) :
        data(new T[y_size * x_size]{ t }), y_size(y_size), x_size(x_size) {}

    Grid(Grid<T> const&) = delete;
    Grid(Grid<T>&&) = delete;

    Grid<T>& operator=(Grid<T> const& other) {
        if (this != &other) {
            delete[] data;
            data = new T[other.y_size * other.x_size];
            std::copy(other.data, other.data + other.y_size * other.x_size, data);
            y_size = other.y_size;
            x_size = other.x_size;
        }
        return *this;
    }

    Grid<T>& operator=(Grid<T>&& other) {
        if (this != &other) {
            delete[] data;
            data = other.data;
            y_size = other.y_size;
            x_size = other.x_size;
            other.data = nullptr;
            other.y_size = 0;
            other.x_size = 0;
        }
        return *this;
    }

    ~Grid() {
        delete[] data;
    }

    T operator()(size_type y_idx, size_type x_idx) const {
        return data[y_idx * x_size + x_idx];
    }

    T& operator()(size_type y_idx, size_type x_idx) {
        return data[y_idx * x_size + x_idx];
    }

    Grid<T>& operator=(T const& t) {
        for (auto it = data, end = data + x_size * y_size; it != end; ++it) {
            *it = t;
        }
        return *this;
    }

    size_type get_y_size() const { return y_size; }
    size_type get_x_size() const { return x_size; }

    // Оператор индексирования []
    T* operator[](size_type y_idx) {
        return &data[y_idx * x_size];
    }

    // Константный оператор индексирования []
    const T* operator[](size_type y_idx) const {
        return &data[y_idx * x_size];
    }

private:
    T* data;
    size_type y_size;
    size_type x_size;
};

int main() {
    Grid<float> g(3, 2, 0.0f);
    assert(3 == g.get_y_size());
    assert(2 == g.get_x_size());
    //using Grid<int>::size_type = gsize_t;
    for (size_t y_idx = 0; y_idx != g.get_y_size(); ++y_idx) {
        for (size_t x_idx = 0; x_idx != g.get_x_size(); ++x_idx) {
            assert(0.0f == g[y_idx][x_idx]);
        }
    }
    for (size_t y_idx = 0; y_idx != g.get_y_size(); ++y_idx) {
        for (size_t x_idx = 0; x_idx != g.get_x_size(); ++x_idx) {
            g[y_idx][x_idx] = 1.0f;
        }
    }
    for (size_t y_idx = 0; y_idx != g.get_y_size(); ++y_idx) {
        for (size_t x_idx = 0; x_idx != g.get_x_size(); ++x_idx) {
            assert(1.0f == g(y_idx, x_idx));
        }
    }
}
