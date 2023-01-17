#include <algorithm>
#include <iostream>
#include <string>
#include <list>
#include <utility>
#include "../SmartPointer.h"

/*
 * The base Component class declares common operations for both simple and
 * complex objects of a composition.
 */

class Component {
    /*
     * @var Component
     */
protected:
    SmartPtr<Component> parent_;
    /*
     * Optionally, the base Component can declare an interface for setting and
     * accessing a parent of the component in a tree structure. It can also
     * provide some default implementation for these methods.
     */
public:
    virtual ~Component() = default;
    void SetParent(SmartPtr<Component> parent) {
        this->parent_ = SmartPtr<Component> (std::move(parent));
    }
    [[nodiscard]] SmartPtr<Component> GetParent() const {
        return this->parent_;
    }

    /*
     * In some cases, it would be beneficial to define the child-management
     * operations right in the base Component class. This way, you won't need to
     * expose any concrete component classes to the client code, even during the
     * object tree assembly. The downside is that these methods will be empty for
     * the leaf-level components.
     */

    virtual void Add(SmartPtr<Component> component) {}
    virtual void Remove(SmartPtr<Component> component) {}

    /*
     * You can provide a method that lets the client code figure out whether a
     * component can bear children.
     */

    [[nodiscard]] virtual bool IsComposite() const {
        return false;
    }

    /*
     * The base Component may implement some default behavior or leave it to
     * concrete classes (by declaring the method containing the behavior as
     * "abstract").
     */

    [[nodiscard]] virtual std::string Operation() const = 0;
};

/*
 * The Leaf class represents the end objects of a composition. A leaf can't have
 * any children.
 *
 * Usually, it's the Leaf objects that do the actual work, whereas Composite
 * objects only delegate to their subcomponents.
 */

class Leaf : public Component {
public:
    [[nodiscard]] std::string Operation() const override {
        return "Leaf";
    }
};


/*
 * The Composite class represents the complex components that may have children.
 * Usually, the Composite objects delegate the actual work to their children and
 * then "sum-up" the result.
 */

class Composite : public Component {
    /*
     * @var Component[]
     */
protected:
    mutable std::list<SmartPtr<Component>> children;

public:
    /*
     * A composite object can add or remove other components (both simple or
     * complex) to or from its child list.
     */
    void Add(SmartPtr<Component> component) override {
        SmartPtr<Component> thisPtr = SmartPtr<Component> (new Composite());
        children.emplace_back(component);
        component->SetParent(thisPtr);
    }

    void Remove(SmartPtr<Component> component) override {
        this->children.remove(component);
        component->SetParent(SmartPtr<Component>());
    }

    [[nodiscard]] bool IsComposite() const override {
        return true;
    }

    /*
     * The Composite executes its primary logic in a particular way. It traverses
     * recursively through all its children, collecting and summing their results.
     * Since the composite's children pass these calls to their children and so
     * forth, the whole object tree is traversed as a result.
     */

    [[nodiscard]] std::string Operation() const override {
        int i = 0;
        std::string result;
        for (const SmartPtr<Component>& component : this->children) {
            result += component->Operation();
            if (i != this->children.size() - 1) {
                result += "+";
            }
            i++;
        }
        return "Branch(" + result + ")";
    }
};

/*
 * The client code works with all the components via the base interface.
 */

void ClientCode(const SmartPtr<Component>& component) {
    // ...
    std::cout << "RESULT: " << component->Operation();
    // ...
}

/*
 * Thanks to the fact that the child-management operations are declared in the
 * base Component class, the client code can work with any component, simple or
 * complex, without depending on their concrete classes.
 */

void ClientCode2(const SmartPtr<Component>& component1, SmartPtr<Component> component2) {
    // ...
    if (component1->IsComposite()) {
        component1->Add(std::move(component2));
    }
    std::cout << "RESULT: " << component1->Operation();
    // ...
}

/*
 * This way the client code can support the simple leaf components...
 */

int main() {
    SmartPtr<Component> simple = SmartPtr<Component>(new Leaf);
    std::cout << "Client: I've got a simple component:\n";
    ClientCode(simple);
    std::cout << "\n\n";
    /*
     * ...as well as the complex composites.
     */
    SmartPtr<Component> tree = SmartPtr<Component>(new Composite);
    SmartPtr<Component> branch1 = SmartPtr<Component>(new Composite);

    SmartPtr<Component> leaf_1 = SmartPtr<Component>(new Leaf);
    SmartPtr<Component> leaf_2 = SmartPtr<Component>(new Leaf);
    SmartPtr<Component> leaf_3 = SmartPtr<Component>(new Leaf);

    branch1->Add(leaf_1);
    branch1->Add(leaf_2);

    SmartPtr<Component> branch2 = SmartPtr<Component>(new Composite);
    branch2->Add(leaf_3);

    tree->Add(branch1);
    tree->Add(branch2);
    std::cout << "Client: Now I've got a composite tree:\n";
    ClientCode(tree);
    std::cout << "\n\n";

    std::cout << "Client: I don't need to check the components classes even when managing the tree:\n";
    ClientCode2(tree, simple);
    std::cout << "\n";

    tree->Remove(branch1);
}
