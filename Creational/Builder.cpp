/*
 * It makes sense to use the Builder pattern only when your products are quite
 * complex and require extensive configuration.
 *
 * Unlike in other creational patterns, different concrete builders can produce
 * unrelated products. In other words, results of various builders may not
 * always follow the same interface.
 */

#include "../SmartPointer.h"
#include <iostream>
#include <string>
#include <vector>


class Product1{
public:
    std::vector<std::string> parts;
    void ListParts() const {
        std::cout << " Product parts: ";
        for (const auto& part : parts) {
            if (part == parts.back()){
                std::cout << part;
            } else {
                std::cout << part << ", ";
            }
        }
        std::cout << " " << std::endl;
    }
};


/*
 * The Builder interface specifies methods for creating the different parts of
 * the Product objects.
 */


class Builder{
public:
    virtual ~Builder() = default;
    virtual void ProducePartA() const = 0;
    virtual void ProducePartB() const = 0;
    virtual void ProducePartC() const = 0;
    virtual SmartPtr<Product1> GetProduct() = 0;
};

/*
 * The Concrete Builder classes follow the Builder interface and provide
 * specific implementations of the building steps. Your program may have
 * several variations of Builders, implemented differently.
 */

class ConcreteBuilder1 : public Builder {
private:
    SmartPtr<Product1> product;
public:
    ConcreteBuilder1() {
        this->Reset();
    }
    ~ConcreteBuilder1() override = default;

    void Reset() {
        this->product = static_cast<SmartPtr<Product1>>(new Product1());
    }

    /*
     * All production steps work with the same product instance.
     */

    void ProducePartA() const override {
        this->product->parts.emplace_back("PartA1");
    }

    void ProducePartB() const override {
        this->product->parts.emplace_back("PartB1");
    }

    void ProducePartC() const override {
        this->product->parts.emplace_back("PartC1");
    }

    /*
     * Concrete Builders are supposed to provide their own methods for
     * retrieving results. That's because various types of builders may create
     * entirely different products that don't follow the same interface.
     * Therefore, such methods cannot be declared in the base Builder interface
     * (at least in a statically typed programming language).
     *
     * Usually, after returning the end result to the client, a builder
     * instance is expected to be ready to start producing another product.
     * That's why it's a usual practice to call the reset method at the end of
     * the `GetProduct` method body. However, this behavior is not mandatory,
     * and you can make your builders wait for an explicit reset call from the
     * client code before disposing of the previous result.
     */

    SmartPtr<Product1> GetProduct() override{
        SmartPtr<Product1> result = this->product;
        this->Reset();
        return result;
    }
};

/*
 * The Director is only responsible for executing the building steps in a
 * particular sequence. It is helpful when producing products according to a
 * specific order or configuration. Strictly speaking, the Director class is
 * optional, since the client can control builders directly.
 */

class Director{
private:
    SmartPtr<Builder> builder;
    /*
     * The Director works with any builder instance that the client code passes
     * to it. This way, the client code may alter the final type of the newly
     * assembled product.
     */
public:
    void set_builder(SmartPtr<Builder> builder_) {
        this->builder = std::move(builder_);
    }
    /*
     * The Director can construct several product variations using the same
     * building steps.
     */

    void BuildMinimalViableProduct() {
        this->builder->ProducePartA();
    }

    void BuildFullFeaturedProduct() {
        this->builder->ProducePartA();
        this->builder->ProducePartB();
        this->builder->ProducePartC();
    }
};

/*
 * The client code creates a builder object, passes it to the director and then
 * initiates the construction process. The end result is retrieved from the
 * builder object.
 */

void ClientCode(Director &director) {
    SmartPtr<Builder> builder = SmartPtr<Builder>(new ConcreteBuilder1());
    director.set_builder(builder);
    std::cout << "Standard basic product:\n";

    director.BuildMinimalViableProduct();
    auto product = builder->GetProduct();
    product->ListParts();

    std::cout << "Standard full featured product:\n";
    director.BuildFullFeaturedProduct();
    product = builder->GetProduct();
    product->ListParts();

    // Remember, the Builder pattern can be used without a Director class.
    std::cout << "Custom product:\n";
    builder->ProducePartA();
    builder->ProducePartC();
    product = builder->GetProduct();
    product->ListParts();
}

int main() {
    SmartPtr<Director> director = static_cast<SmartPtr<Director>>(new Director());
    ClientCode(*director);
    return 0;
}