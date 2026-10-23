#include <iostream>
#include <string>
#include <utility>
#include <vector>

class AClass {
public:
    AClass(int id, std::string name)
        : id_(id), name_(std::move(name)) {
        std::cout << "ctor " << id_ << '\n';
    }

    AClass(const AClass& other)
        : id_(other.id_), name_(other.name_) {
        std::cout << "copy " << id_ << '\n';
    }

    AClass(AClass&& other) noexcept
        : id_(other.id_), name_(std::move(other.name_)) {
        std::cout << "move " << id_ << '\n';
    }

private:
    int id_;
    std::string name_;
};

int main() {
    {
        std::cout << "[1] push_back temporary\n";
        std::vector<AClass> values;
        values.reserve(1);
        values.push_back(AClass(1, "a"));
    }
    {
        std::cout << "[2] emplace_back arguments\n";
        std::vector<AClass> values;
        values.reserve(1);
        values.emplace_back(1, "a");
    }
    {
        std::cout << "[3] push_back lvalue\n";
        std::vector<AClass> values;
        values.reserve(1);
        AClass value(1, "a");
        values.push_back(value);
    }
    {
        std::cout << "[4] emplace_back temporary\n";
        std::vector<AClass> values;
        values.reserve(1);
        values.emplace_back(AClass(1, "a"));
    }
    {
        std::cout << "[5] fill capacity\n";
        std::vector<AClass> values;
        values.reserve(1);
        while (values.size() < values.capacity()) {
            values.emplace_back(1, "a");
        }
        std::cout << "[5] append beyond capacity\n";
        values.emplace_back(2, "b");
    }
    return 0;
}
