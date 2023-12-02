#include <iostream>
#include <stdexcept>

template <size_t Size, typename T, bool Force = true>
class LimitedQueue {
    T data[Size];
    size_t count = 0;
    
public:
    LimitedQueue() = default;

    LimitedQueue(std::initializer_list<T> init) {
        count = 0;
        for (const auto& item : init) {
            if (count < Size) {
                data[count++] = item;
            }
        }
    }

    void push_back(const T& value) {
        if (count < Size) {
            data[count++] = value;
        } else if (Force) {
            for (size_t i = 1; i < count; ++i) {
                data[i - 1] = data[i];
            }
            data[Size - 1] = value;
        } else {
            throw std::runtime_error("Queue is full");
        }
    }

    T pop_front() {
        if (count == 0) {
            throw std::runtime_error("Queue is empty");
        }
        T value = data[0];
        for (size_t i = 1; i < count; ++i) {
            data[i - 1] = data[i];
        }
        count--;
        return value;
    }

    T front() const {
        if (count == 0) {
            throw std::runtime_error("Queue is empty");
        }
        return data[0];
    }

    bool empty() const {
        return count == 0;
    }

    bool full() const {
        return count == Size;
    }

    void clear() {
        count = 0;
    }

    size_t size() const {
        return count;
    }
};

int main() {
    LimitedQueue<3, int, true> queue = {1, 2};
    try {
        queue.push_back(3);
        queue.push_back(4);
    } catch (const std::exception& e) {
        std::cout << "Exception: " << e.what() << std::endl;
    }
    while (!queue.empty()) {
        std::cout << queue.pop_front() << std::endl;
    }
    return 0;
}