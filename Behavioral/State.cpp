#include <iostream>
#include <typeinfo>
#include "../SmartPointer.h"

/*
 * The base State class declares methods that all Concrete State should
 * implement and also provides a backreference to the Context object,
 * associated with the State. This backreference can be used by States to
 * transition the Context to another State.
 */

class Context;

class State {
    /*
     * @var Context
     */

protected:
    SmartPtr<Context> context;
public:
    virtual ~State() = default;
    void set_context(SmartPtr<Context> context) {
        this->context = std::move(context);
    }
    virtual void Handle1() = 0;
    virtual void Handle2() = 0;
};

/*
 * The Context defines the interface of interest to clients. It also maintains a
 * reference to an instance of a State subclass, which represents the current
 * state of the Context.
 */

class Context {
    /*
     * @var State A reference to the current state of the Context.
     */
private:
    SmartPtr<State> state_;
public:
    explicit Context(SmartPtr<State> state) : state_(nullptr) {
        this->TransitionTo(std::move(state));
    }
    ~Context() = default;
    /*
     * The Context allows changing the State object at runtime.
     */
    void TransitionTo(SmartPtr<State> state) {
        std::cout << "Context: Transition to " << typeid(*state).name() << ".\n";
        this->state_ = std::move(state);
        this->state_->set_context(SmartPtr<Context>(new Context(*this)));
    }
    /*
     * The Context delegates part of its behavior to the current State object.
     */
    void Request1() {
        this->state_->Handle1();
    }
    void Request2() {
        this->state_->Handle2();
    }
};


/*
 * Concrete States implement various behaviors,
 * associated with a state of the Context.
 */


class ConcreteStateA : public State {
public:
    ~ConcreteStateA() override = default;

    void Handle1() override;

    void Handle2() override {
        std::cout << "ConcreteStateA handles request2.\n";
    }
};

class ConcreteStateB : public State {
public:
    ~ConcreteStateB() override = default;

    void Handle1() override {
        std::cout << "ConcreteStateB handles request1.\n";
    }

    void Handle2() override {
        std::cout << "ConcreteStateB handles request2.\n";
        std::cout << "ConcreteStateB wants to change the state of the context.\n";
        this->context->TransitionTo(SmartPtr<State>(new ConcreteStateA));
    }
};

void ConcreteStateA::Handle1() {
    {
        std::cout << "ConcreteStateA handles request1.\n";
        std::cout << "ConcreteStateA wants to change the state of the context.\n";
        this->context->TransitionTo(SmartPtr<State>(new ConcreteStateB));
    }
}

void ClientCode() {
    SmartPtr<Context> context(new Context(SmartPtr<State>(new ConcreteStateA)));
    context->Request1();
    context->Request2();
}

int main() {
    ClientCode();
    return 0;
}