#include <iostream>
#include <string>
#include <utility>
#include "../SmartPointer.h"

/*
 * The Mediator interface declares a method used by components to notify the
 * mediator about various events. The Mediator may react to these events and
 * pass the execution to other components.
 */

class BaseComponent;

class Mediator {
public:
    virtual ~Mediator() = default;
    virtual void Notify(SmartPtr<BaseComponent> sender, const std::string& event) const = 0;
};

/*
 * The Base Component provides the basic functionality of storing a mediator's
 * instance inside component objects.
 */

class BaseComponent {
protected:
    SmartPtr<Mediator> mediator_;
public:
    explicit BaseComponent(SmartPtr<Mediator> mediator = SmartPtr<Mediator>(nullptr) ): mediator_(std::move(mediator)) {}
    void set_mediator(SmartPtr<Mediator> mediator) {
        this->mediator_ = std::move(mediator);
    }
};

/*
 * Concrete Components implement various functionality. They don't depend on
 * other components. They also don't depend on any concrete mediator classes.
 */

class Component1 : public BaseComponent {
public:
    void DoA() {
        std::cout << "Component 1 does A.\n";
        this->mediator_->Notify(SmartPtr<BaseComponent>(new Component1(*this)), "A");
    }
    void DoB() {
        std::cout << "Component 1 does B.\n";
        this->mediator_->Notify(SmartPtr<BaseComponent>(new Component1(*this)), "B");
    }
};

class Component2 : public BaseComponent {
    public:
    void DoC() {
        std::cout << "Component 2 does C.\n";
        this->mediator_->Notify(SmartPtr<BaseComponent>(new Component2(*this)), "C");
    }
    void DoD() {
        std::cout << "Component 2 does D.\n";
        this->mediator_->Notify(SmartPtr<BaseComponent>(new Component2(*this)), "D");
    }
};

/*
 * Concrete Mediators implement cooperative behavior by coordinating several
 * components.
 */

class ConcreteMediator : public Mediator {
private:
    SmartPtr<Component1> component1_;
    SmartPtr<Component2> component2_;
public:

    ConcreteMediator(SmartPtr<Component1> c1, SmartPtr<Component2> c2) : component1_(std::move(c1)), component2_(std::move(c2)) {
        this->component1_->set_mediator(SmartPtr<Mediator>(new ConcreteMediator(*this)));
        this->component2_->set_mediator(SmartPtr<Mediator>(new ConcreteMediator(*this)));
    }

    void Notify(SmartPtr<BaseComponent> sender, const std::string& event) const override {
        if (event == "A") {
            std::cout << "Mediator reacts on A and triggers following operations:\n";
            this->component2_->DoC();
        }
        if (event == "D") {
            std::cout << "Mediator reacts on D and triggers following operations:\n";
            this->component1_->DoB();
            this->component2_->DoC();
        }
    }
};

void ClientCode() {
    SmartPtr<Component1> c1 = SmartPtr<Component1>(new Component1);
    SmartPtr<Component2> c2 = SmartPtr<Component2>(new Component2);
    SmartPtr<ConcreteMediator> mediator = SmartPtr<ConcreteMediator>(new ConcreteMediator(c1, c2));
    std::cout << "Client triggers operation A.\n";
    c1->DoA();
    std::cout << "\n";
    std::cout << "Client triggers operation D.\n";
    c2->DoD();
}

int main() {
    ClientCode();
}