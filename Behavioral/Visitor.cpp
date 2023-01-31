#include "../SmartPointer.h"
#include <iostream>
#include <vector>

/*
 * The Visitor Interface declares a set of visiting methods that correspond to
 * component classes. The signature of a visiting method allows the visitor to
 * identify the exact class of the component that it's dealing with.
 */

class ConcreteComponentA;
class ConcreteComponentB;

class Visitor {
public:
    virtual void VisitConcreteComponentA(const SmartPtr<ConcreteComponentA>& element) const = 0;
    virtual void VisitConcreteComponentB(const SmartPtr<ConcreteComponentB>& element) const = 0;
};

/*
 * The Component interface declares an `accept` method that should take the base
 * visitor interface as an argument.
 */

class Component {
public:
    virtual ~Component() = default;
    virtual void Accept(const SmartPtr<Visitor>& visitor) const = 0;
};

/*
 * Each Concrete Component must implement the `Accept` method in such a way that
 * it calls the visitor's method corresponding to the component's class.
 */

class ConcreteComponentA : public Component {
    /*
     * Note that we're calling `visitConcreteComponentA`, which matches the
     * current class name. This way we let the visitor know the class of the
     * component it works with.
     */

public:
    void Accept(const SmartPtr<Visitor>& visitor) const override {
        visitor->VisitConcreteComponentA(SmartPtr<ConcreteComponentA>(new ConcreteComponentA(*this)));
    }

    virtual std::string ExclusiveMethodOfConcreteComponentA() const {
        return "A";
    }
};

class ConcreteComponentB : public Component {
    /*
     * Same here: visitConcreteComponentB => ConcreteComponentB
     */
public:
    void Accept(const SmartPtr<Visitor>& visitor) const override {
        visitor->VisitConcreteComponentB(SmartPtr<ConcreteComponentB>(new ConcreteComponentB(*this)));
    }

    virtual std::string SpecialMethodOfConcreteComponentB() const {
        return "B";
    }
};

/*
 * Concrete Visitors implement several versions of the same algorithm, which can
 * work with all concrete component classes.
 *
 * You can experience the biggest benefit of the Visitor pattern when using it
 * with a complex object structure, such as a Composite tree. In this case, it
 * might be helpful to store some intermediate state of the algorithm while
 * executing visitor's methods over various objects of the structure.
 */

class ConcreteVisitor1 : public Visitor {
public:
    void VisitConcreteComponentA(const SmartPtr<ConcreteComponentA>& element) const override {
        std::cout << element->ExclusiveMethodOfConcreteComponentA() << " + ConcreteVisitor1\n";
    }
    void VisitConcreteComponentB(const SmartPtr<ConcreteComponentB>& element) const override {
        std::cout << element->SpecialMethodOfConcreteComponentB() << " + ConcreteVisitor1\n";
    }
};

class ConcreteVisitor2 : public Visitor {
public:
    void VisitConcreteComponentA(const SmartPtr<ConcreteComponentA>& element) const override {
        std::cout << element->ExclusiveMethodOfConcreteComponentA() << " + ConcreteVisitor2\n";
    }
    void VisitConcreteComponentB(const SmartPtr<ConcreteComponentB>& element) const override {
        std::cout << element->SpecialMethodOfConcreteComponentB() << " + ConcreteVisitor2\n";
    }
};

/*
 * The client code can run visitor operations over any set of elements without
 * figuring out their concrete classes. The accept operation directs a call to
 * the appropriate operation in the visitor object.
 */

void ClientCode(const SmartPtr<std::vector<SmartPtr<Component>>>& components, const SmartPtr<Visitor>& visitor) {
    // ...
    for (const auto& component : *components) {
        component->Accept(visitor);
    }
    // ...
}

int main() {
    SmartPtr<std::vector<SmartPtr<Component>>> components(new std::vector<SmartPtr<Component>>());
    components->push_back(SmartPtr<Component>(new ConcreteComponentA()));
    components->push_back(SmartPtr<Component>(new ConcreteComponentB()));

    std::cout << "The client code works with all visitors via the base Visitor interface:\n";
    SmartPtr<Visitor> visitor1(new ConcreteVisitor1());
    ClientCode(components, visitor1);
    std::cout << "\n";
    std::cout << "It allows the same client code to work with different types of visitors:\n";
    SmartPtr<Visitor> visitor2(new ConcreteVisitor2());
    ClientCode(components, visitor2);
}

