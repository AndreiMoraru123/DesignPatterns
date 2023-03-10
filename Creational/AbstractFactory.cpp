/*
 * Each distinct product of a product family should have a base interface.
 * All variants of the product must implement this interface.
 */

#include <string>
#include <iostream>
#include "../SmartPointer.h"


class AbstractProductA {
public:
    virtual ~AbstractProductA() = default;
    [[nodiscard]] virtual std::string UsefulFunctionA() const = 0;
};

/*
 * Concrete Products are created by corresponding Concrete Factories.
 */

class ConcreteProductA1 : public AbstractProductA {
    [[nodiscard]] std::string UsefulFunctionA() const override {
        return "The result of the product A1.";
    }
};

class ConcreteProductA2 : public AbstractProductA {
    [[nodiscard]] std::string UsefulFunctionA() const override {
        return "The result of the product A2.";
    }
};

/*
 * Here's the base interface of another product. All products can
 * interact with each other, but proper interaction is possible only
 * between products of the same concrete variant.
 */

class AbstractProductB {
    /*
     * Product B is able to do its own thing...
     */
public:
    virtual ~AbstractProductB() = default;
    [[nodiscard]] virtual std::string UsefulFunctionB() const = 0;

    /*
     * ...but it also can collaborate with the ProductA.
     *
     * The Abstract Factory makes sure that all products it creates are of the
     * same variant and thus, compatible.
     */
    [[nodiscard]] virtual std::string AnotherUsefulFunctionB(const AbstractProductA &collaborator) const = 0;
};

/*
 * Concrete Products are created by corresponding Concrete Factories.
 */

class ConcreteProductB1 : public AbstractProductB {
public:
    [[nodiscard]] std::string UsefulFunctionB() const override {
        return "The result of the product B1.";
    }

    /*
     *  The variant, Product B1, is only able to work correctly with the variant, Product A1.
     *  Nevertheless, it accepts any instance of AbstractProductA as an argument.
     */

    [[nodiscard]] std::string AnotherUsefulFunctionB(const AbstractProductA &collaborator) const override {
        const std::string result = collaborator.UsefulFunctionA();
        return "The result of the B1 collaborating with the (" + result + ")";
    }
};

class ConcreteProductB2 : public AbstractProductB {
public:
    [[nodiscard]] std::string UsefulFunctionB() const override {
        return "The result of the product B2.";
    }

    /*
     * The variant, Product B2, is only able to work correctly with the variant, Product A2.
     * Nevertheless, it accepts any instance of AbstractProductA as an argument.
     */

    [[nodiscard]] std::string AnotherUsefulFunctionB(const AbstractProductA &collaborator) const override {
        const std::string result = collaborator.UsefulFunctionA();
        return "The result of the B2 collaborating with the (" + result + ")";
    }
};


/*
 *  The Abstract factory interface declares a set of methods that return different
 *  abstract products. These products are called a family and are related by a
 *  high-level theme or concept. Products of one family are usually able to
 *  collaborate among themselves. A family of products may have several variants,
 *  but the products of one variant are incompatible with products of another.
 */

class AbstractFactory {
public:
    [[nodiscard]] virtual SmartPtr<AbstractProductA> CreateProductA() const = 0;
    [[nodiscard]] virtual SmartPtr<AbstractProductB> CreateProductB() const = 0;
    virtual ~AbstractFactory() = default;
};

/*
 * Concrete Factories produce a family of products that belong to a single
 * variant. The factory guarantees that resulting products are compatible.
 * Note that signatures of the Concrete Factory's methods return an abstract
 * product, while inside the method a concrete product is instantiated.
 */

class ConcreteFactory1 : public AbstractFactory {
public:
    [[nodiscard]] SmartPtr<AbstractProductA> CreateProductA() const override {
        return SmartPtr<AbstractProductA>(new ConcreteProductA1());
    }
    [[nodiscard]] SmartPtr<AbstractProductB> CreateProductB() const override {
        return SmartPtr<AbstractProductB>(new ConcreteProductB1());
    }
    ~ConcreteFactory1() override = default;
};

/*
 * Each Concrete Factory has a corresponding product variant.
 */

class ConcreteFactory2 : public AbstractFactory {
public:
    [[nodiscard]] SmartPtr<AbstractProductA> CreateProductA() const override {
        return SmartPtr<AbstractProductA>(new ConcreteProductA2());
    }
    [[nodiscard]] SmartPtr<AbstractProductB> CreateProductB() const override {
        return SmartPtr<AbstractProductB>(new ConcreteProductB2());
    }
    ~ConcreteFactory2() override = default;
};

/*
 * The client code works with factories and products only through abstract
 * types: AbstractFactory and AbstractProduct. This lets you pass any factory
 * or product subclass to the client code without breaking it.
 */

void ClientCode(const AbstractFactory &factory) {
    const SmartPtr<AbstractProductA> product_a = factory.CreateProductA();
    const SmartPtr<AbstractProductB> product_b = factory.CreateProductB();
    std::cout << product_b->UsefulFunctionB() << std::endl;
    std::cout << product_b->AnotherUsefulFunctionB(*product_a) << std::endl;
}


int main() {
    std::cout << "Client: Testing client code with the first factory type:\n";
    SmartPtr<AbstractFactory> f1(new ConcreteFactory1());
    ClientCode(*f1);
    std::cout << std::endl;
    std::cout << "Client: Testing the same client code with the second factory type:\n";
    SmartPtr<AbstractFactory> f2(new ConcreteFactory2());
    ClientCode(*f2);
}