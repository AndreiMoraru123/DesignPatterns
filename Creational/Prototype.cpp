#include "../SmartPointer.h"
#include <iostream>
#include <string>
#include <unordered_map>
#include <utility>
#include <functional>

/*
 * Prototype Design Pattern
 *
 * Intent: Lets you copy existing objects without making your code dependent on
 */

enum Type {
    PROTOTYPE_1 = 0,
    PROTOTYPE_2
};

/*
 * The example class that has cloning ability. We'll see how the values of field
 * with different types will be cloned.
 */

class Prototype {
protected:
    std::string prototype_name;
    float prototype_field{};

public:
    Prototype() = default;
    explicit Prototype(std::string prototype_name) : prototype_name(std::move(prototype_name)) {};
    virtual ~Prototype() = default;
    [[nodiscard]] virtual SmartPtr<Prototype> Clone() const = 0;
    virtual void Method(float prototypeField) {
        this->prototype_field = prototypeField;
        std::cout << "Call Method from " << prototype_name << " with field : " << prototypeField << std::endl;
    }
};

/*
 * ConcretePrototype1 is a Sub-Class of Prototype and implements the Clone method
 * in order to return a copy of itself. In this example all the data members of
 * Prototype Class are on the stack. So, the shallow copy of the object is
 * sufficient. If there are any data members on the heap, then the deep copy of
 * the object is required. In that case, the Clone method should be overridden to
 * perform the deep copy.
 */

class ConcretePrototype1 : public Prototype {
private:
    float concrete_prototype_field1;
public:
    ConcretePrototype1(std::string prototype_name, float concrete_prototype_field) : Prototype(std::move(prototype_name)), concrete_prototype_field1(concrete_prototype_field) {}

    [[nodiscard]] SmartPtr<Prototype> Clone() const override {
        return static_cast<SmartPtr<Prototype>>(new ConcretePrototype1(*this));
    }
};

class ConcretePrototype2 : public Prototype {
private:
    float concrete_prototype_field2;
public:
    ConcretePrototype2(std::string prototype_name, float concrete_prototype_field) : Prototype(std::move(prototype_name)), concrete_prototype_field2(concrete_prototype_field) {}

    [[nodiscard]] SmartPtr<Prototype> Clone() const override {
        return static_cast<SmartPtr<Prototype>>(new ConcretePrototype2(*this));
    }
};

/*
 * In PrototypeFactory, we have a map of prototypes. The key of the map is the
 * type of the prototype. The value of the map is the prototype itself. The
 * PrototypeFactory has a method to add a new prototype to the map. The
 * PrototypeFactory has a method to return a clone of the prototype based on the
 * type of the prototype.
*/

class PrototypeFactory {
    std::unordered_map<Type, std::function<SmartPtr<Prototype>()>, std::hash<int>> prototypes;

public:
    PrototypeFactory() {
        prototypes[Type::PROTOTYPE_1] = []() { return static_cast<SmartPtr<Prototype>>(new ConcretePrototype1("PROTOTYPE_1", 50.f)); };
        prototypes[Type::PROTOTYPE_2] = []() { return static_cast<SmartPtr<Prototype>>(new ConcretePrototype2("PROTOTYPE_2", 60.f)); };
    }
    ~PrototypeFactory() = default;
    [[nodiscard]] SmartPtr<Prototype> CreatePrototype(Type type) {
        return prototypes[type]()->Clone();
    }
};

void Client(PrototypeFactory &prototype_factory) {
    std::cout << "Let's create a Prototype 1" << std::endl;
    SmartPtr<Prototype> prototype = prototype_factory.CreatePrototype(Type::PROTOTYPE_1);
    prototype->Method(90);

    std::cout << std::endl;

    std::cout << "Let's create a Prototype 2" << std::endl;
    prototype = prototype_factory.CreatePrototype(Type::PROTOTYPE_2);
    prototype->Method(10);
}

int main() {
    SmartPtr<PrototypeFactory> prototype_factory = static_cast<SmartPtr<PrototypeFactory>>(new PrototypeFactory());
    Client(*prototype_factory);
}
