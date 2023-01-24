#include "../SmartPointer.h"
#include <iostream>
#include <string>
#include <vector>

/*
 * The Handler interface declares a method for building the chain of handlers.
 * It also declares a method for executing a request.
 */

class Handler {
public:
    virtual SmartPtr<Handler> SetNext(SmartPtr<Handler> handler) = 0;
    virtual ~Handler() = default;
    [[nodiscard]] virtual std::string Handle(std::string request) const = 0;
};

/*
 * The default chaining behavior can be implemented inside a base handler class.
 */

class AbstractHandler : public Handler {
    /*
     * @var Handler
     */
private:
    SmartPtr<Handler> next_handler_;

public:
    AbstractHandler(): next_handler_(nullptr) {}
    SmartPtr<Handler> SetNext(SmartPtr<Handler> handler) override {
        this->next_handler_ = handler;
        // Returning a handler from here will let us link handlers in a
        // convenient way like this:
        // $monkey->setNext($squirrel)->setNext($dog);
        return handler;
    }

    [[nodiscard]] std::string Handle(std::string request) const override {
        if (this->next_handler_.get()) {
            return this->next_handler_->Handle(request);
        }
        return {};
    }
};

/*
 * All Concrete Handlers either handle a request or pass it to the next handler
 * in the chain.
 */

class MonkeyHandler : public AbstractHandler {
public:
    [[nodiscard]] std::string Handle(std::string request) const override {
        if (request == "Banana") {
            return "Monkey: I'll eat the " + request + ".\n";
        }
        return AbstractHandler::Handle(request);
    }
};

class SquirrelHandler : public AbstractHandler {
public:
    [[nodiscard]] std::string Handle(std::string request) const override {
        if (request == "Nut") {
            return "Squirrel: I'll eat the " + request + ".\n";
        }
        return AbstractHandler::Handle(request);
    }
};

class DogHandler : public AbstractHandler {
public:
    [[nodiscard]] std::string Handle(std::string request) const override {
        if (request == "MeatBall") {
            return "Dog: I'll eat the " + request + ".\n";
        }
        return AbstractHandler::Handle(request);
    }
};

/*
 * The client code is usually suited to work with a single handler. In most
 * cases, it is not even aware that the handler is part of a chain.
 */

void ClientCode(const Handler &handler) {
    std::vector<std::string> food = {"Nut", "Banana", "Cup of coffee"};
    for (const std::string &f : food) {
        std::cout << "Client: Who wants a " << f << "?\n";
        const std::string result = handler.Handle(f);
        if (!result.empty()) {
            std::cout << "  " << result;
        } else {
            std::cout << "  " << f << " was left untouched.\n";
        }
    }
}

/*
 * The other part of the client code constructs the actual chain.
 */

int main() {
    SmartPtr<Handler> monkey = SmartPtr<Handler>(new MonkeyHandler);
    SmartPtr<Handler> squirrel = SmartPtr<Handler>(new SquirrelHandler);
    SmartPtr<Handler> dog = SmartPtr<Handler>(new DogHandler);
    monkey->SetNext(squirrel)->SetNext(dog);

    /*
     * The client should be able to send a request to any handler, not just the
     * first one in the chain.
     */

    std::cout << "Chain: Monkey > Squirrel > Dog\n\n";
    ClientCode(*monkey);
    std::cout << std::endl;
    std::cout << "Sub chain: Squirrel > Dog\n\n";
    ClientCode(*squirrel);
}