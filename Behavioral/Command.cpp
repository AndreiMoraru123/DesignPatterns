#include <iostream>
#include <string>
#include "../SmartPointer.h"

/*
 * The Command interface declares a method for executing a command.
 */

class Command {
public:
    virtual ~Command() = default;
    virtual void Execute() const = 0;
};

/*
 * Some commands can implement simple operations on their own.
 */

class SimpleCommand : public Command {
private:
    std::string payload_;

public:
    explicit SimpleCommand(std::string payload) : payload_(std::move(payload)) {}

    void Execute() const override {
        std::cout << "SimpleCommand: See, I can do simple things like printing (" << this->payload_ << ")\n";
    }
};

/*
 * The Receiver classes contain some important business logic. They know how to
 * perform all kinds of operations, associated with carrying out a request. In
 * fact, any class may serve as a Receiver.
 */

class Receiver {
public:
    static void DoSomething(const std::string& a) {
        std::cout << "Receiver: Working on (" << a << ".)\n";
    }

    static void DoSomethingElse(const std::string& b) {
        std::cout << "Receiver: Also working on (" << b << ".)\n";
    }
};

/*
 * However, some commands can delegate more complex operations to other objects,
 * called "receivers."
 */

class ComplexCommand : public Command {
    /*
     * @var Receiver
     */
private:
    SmartPtr<Receiver> receiver_;
    std::string a_;
    std::string b_;

    /*
     * Context data, required for launching the receiver's methods.
     */

public:
    ComplexCommand(SmartPtr<Receiver> receiver, std::string a, std::string b) : receiver_(std::move(receiver)), a_(std::move(a)), b_(std::move(b)) {}

    /*
     * Commands can delegate to any methods of a receiver.
     */

    void Execute() const override {
        std::cout << "ComplexCommand: Complex stuff should be done by a receiver object.\n";
        this->receiver_->DoSomething(this->a_);
        this->receiver_->DoSomethingElse(this->b_);
    }
};

/*
 * The Invoker is associated with one or several commands. It sends a request to
 * the command.
 */

class Invoker {
    /*
     * @var Command
     */
private:
    SmartPtr<Command> onStart_;
    SmartPtr<Command> onFinish_;

    /*
     * Initialize commands.
     */

public:
    ~Invoker() = default;
    void SetOnStart(SmartPtr<Command> command) {
        this->onStart_ = SmartPtr<Command> (std::move(command));
    }

    void SetOnFinish(SmartPtr<Command> command) {
        this->onFinish_ = SmartPtr<Command> (std::move(command));
    }

    /*
     * The Invoker does not depend on concrete command or receiver classes. The
     * Invoker passes a request to a receiver indirectly, by executing a command.
     */

    void DoSomethingImportant() {
        std::cout << "Invoker: Does anybody want something done before I begin?\n";
        if (this->onStart_.get()) {
            this->onStart_->Execute();
        }
        std::cout << "Invoker: ...doing something really important...\n";
        std::cout << "Invoker: Does anybody want something done after I finish?\n";
        if (this->onFinish_.get()) {
            this->onFinish_->Execute();
        }
    }
};

/*
 * The client code can parameterize an invoker with any commands.
 */

int main() {
    SmartPtr<Invoker> invoker = SmartPtr<Invoker>(new Invoker);
    invoker->SetOnStart(SmartPtr<Command>(new SimpleCommand("Say Hi!")));
    SmartPtr<Receiver> receiver = SmartPtr<Receiver>(new Receiver);
    invoker->SetOnFinish(SmartPtr<Command>(new ComplexCommand(receiver, "Send email", "Save report")));

    invoker->DoSomethingImportant();
}