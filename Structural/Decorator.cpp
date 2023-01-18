/*
 * The Decorator pattern can be used to extend the behavior of objects at
 * runtime by placing these objects inside special wrapper objects that contain
 * the behaviors.
 */

#include <iostream>
#include <string>
#include <utility>
#include "../SmartPointer.h"

/*
 * The base Component interface defines operations that can be altered by
 * decorators.
 */

class Component {
public:
    virtual ~Component() = default;
    [[nodiscard]] virtual std::string Operation() const = 0;
};

/*
 * Concrete Components provide default implementations of the operations. There
 * might be several variations of these classes.
 */

class ConcreteComponent : public Component {
public:
    [[nodiscard]] std::string Operation() const override {
        return "ConcreteComponent";
    }
};

/*
 * The base Decorator class follows the same interface as the other components.
 * The primary purpose of this class is to define the wrapping interface for all
 * concrete decorators. The default implementation of the wrapping code might
 * include a field for storing a wrapped component and the means to initialize
 * it.
 */

class Decorator : public Component {
    /*
     * @var Component
     */
protected:
    SmartPtr<Component> component_;

public:
    explicit Decorator(SmartPtr<Component> component) : component_(std::move(component)) {}

    /*
     * The Decorator delegates all the work to the wrapped component.
     */

    [[nodiscard]] std::string Operation() const override {
        return this->component_->Operation();
    }
};

/*
 * Concrete Decorators call the wrapped object and alter its result in some way.
 */

class ConcreteDecoratorA : public Decorator {
    /*
     * Decorators may call parent implementation of the operation, instead of
     * calling the wrapped object directly. This approach simplifies extension of
     * decorator classes.
     */

public:
    explicit ConcreteDecoratorA(SmartPtr<Component> component) : Decorator(std::move(component)) {}

    [[nodiscard]] std::string Operation() const override {
        return "ConcreteDecoratorA(" + Decorator::Operation() + ")";
    }
};

class ConcreteDecoratorB : public Decorator {
public:
    explicit ConcreteDecoratorB(SmartPtr<Component> component) : Decorator(std::move(component)) {}

    [[nodiscard]] std::string Operation() const override {
        return "ConcreteDecoratorB(" + Decorator::Operation() + ")";
    }
};

/*
 * The client code works with all objects using the Component interface.
 * This way it can stay independent of the concrete classes of components
 * it works with.
 */

void ClientCode(const SmartPtr<Component>& component) {
    // ...
    std::cout << "RESULT: " << component->Operation();
    // ...
}

int main() {
    /*
     * This way the client code can support both simple components...
     */
    SmartPtr<Component> simple = SmartPtr<Component>(new ConcreteComponent);
    std::cout << "Client: I've got a simple component:\n";
    ClientCode(simple);
    std::cout << "\n\n";

    /*
     * ...as well as decorated ones.
     *
     * Note how decorators can wrap not only simple components but the other
     * decorators as well.
     */

    SmartPtr<Component> decorator1 = SmartPtr<Component>(new ConcreteDecoratorA(simple));
    SmartPtr<Component> decorator2 = SmartPtr<Component>(new ConcreteDecoratorB(decorator1));
    std::cout << "Client: Now I've got a decorated component:\n";
    ClientCode(decorator2);
    std::cout << "\n";
}