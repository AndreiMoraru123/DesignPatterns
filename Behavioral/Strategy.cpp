#include "../SmartPointer.h"
#include <algorithm>
#include <iostream>
#include <string>
#include <vector>

/*
 * The Strategy interface declares operations common to all supported versions
 * of some algorithm.
 *
 * The Context uses this interface to call the algorithm defined by Concrete
 * Strategies.
 */



class Strategy {
public:
    virtual ~Strategy() = default;
    virtual std::string DoAlgorithm(const std::vector<std::string> &data) const = 0;
};

/*
 * The Context defines the interface of interest to clients.
 */

class Context {
    /*
     * @var Strategy The Context maintains a reference to one of the Strategy
     * objects. The Context does not know the concrete class of a strategy. It
     * should work with all strategies via the Strategy interface.
     */
private:
    SmartPtr<Strategy> strategy_;
    /*
     * Usually, the Context accepts a strategy through the constructor, but also
     * provides a setter to change it at runtime.
     */
public:
    explicit Context(SmartPtr<Strategy> strategy) : strategy_(std::move(strategy)) {}
    ~Context() = default;
    /*
     * Usually, the Context allows replacing a Strategy object at runtime.
     */
    void set_strategy(SmartPtr<Strategy> strategy) {
        this->strategy_ = std::move(strategy);
    }
    /*
     * The Context delegates some work to the Strategy object instead of
     * implementing multiple versions of the algorithm on its own.
     */
    void DoSomeBusinessLogic() const {
       if (strategy_.get()) {
           std::cout << "Context: Sorting data using the strategy (not sure how it'll do it)\n";
           std::string result = this->strategy_->DoAlgorithm(std::vector<std::string>{"a", "e", "c", "b", "d"});
           std::cout << result << "\n";
       }
       else {
           std::cout << "Context: Strategy is not set.\n";
       }
    }
};

/*
 * Concrete Strategies implement the algorithm while following the base Strategy
 * interface. The interface makes them interchangeable in the Context.
 */

class ConcreteStrategyA : public Strategy {
public:
    ~ConcreteStrategyA() override = default;
    std::string DoAlgorithm(const std::vector<std::string> &data) const override {
        std::string result;
        for (const std::string &word : data) {
            result += word;
        }
        std::sort(result.begin(), result.end());
        return result;
    }
};

class ConcreteStrategyB : public Strategy {
public:
    ~ConcreteStrategyB() override = default;
    std::string DoAlgorithm(const std::vector<std::string> &data) const override {
        std::string result;
        for (const std::string &word : data) {
            result += word;
        }
        std::sort(result.begin(), result.end(), std::greater<char>());
        return result;
    }
};

/*
 * The client code picks a concrete strategy and passes it to the context. The
 * client should be aware of the differences between strategies in order to make
 * the right choice.
 */

void clientCode(){
    Context context(SmartPtr<Strategy>(new ConcreteStrategyA));
    std::cout << "Client: Strategy is set to normal sorting.\n";
    context.DoSomeBusinessLogic();
    std::cout << "\n";
    std::cout << "Client: Strategy is set to reverse sorting.\n";
    context.set_strategy(SmartPtr<Strategy>(new ConcreteStrategyB));
    context.DoSomeBusinessLogic();
}

int main() {
    clientCode();
    return 0;
}