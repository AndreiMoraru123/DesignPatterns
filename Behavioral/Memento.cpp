#include "../SmartPointer.h"
#include <ctime>
#include <iostream>
#include <string>
#include <utility>
#include <vector>
#include <random>

/*
 * The Memento interface provides a way to retrieve the memento's metadata,
 * such as creation date or name. However, it doesn't expose the Originator's state.
 */

class Memento {
public:
    virtual ~Memento() = default;
    [[nodiscard]] virtual std::string Name() const = 0;
    [[nodiscard]] virtual std::string Date() const = 0;
    [[nodiscard]] virtual std::string State() const = 0;
};

/*
 * The Concrete Memento contains the infrastructure for storing the Originator's state.
 */

class ConcreteMemento : public Memento {
    std::string state_;
    std::string date_;

    explicit ConcreteMemento(std::string state) : state_(std::move(state)) {
        std::time_t now = std::time(nullptr);
        this->date_ = std::ctime(&now);
    }

    friend class Originator;

public:
    /*
   * The Originator uses this method when restoring its state.
   */


    [[nodiscard]] std::string Name() const override {
        return this->date_ + " / (" + this->state_.substr(0, 9) + "...)";
    }

    /*
     * The rest of the methods are used by the Caretaker to display metadata.
     */
    [[nodiscard]] std::string Date() const override {
        return this->date_;
    }

    [[nodiscard]] std::string State() const override {
        return this->state_;
    }
};


/*
 * The Originator holds some important state that may change over time. It also
 * defines a method for saving the state inside a memento and another method for
 * restoring the state from it.
 */

class Originator {
    std::string state_;
    static std::string GenerateRandomString() {
        std::random_device rd;
        std::mt19937 gen(rd());
        std::uniform_int_distribution<> dis(0, 25);
        const char alphanum[] =
            "0123456789"
            "ABCDEFGHIJKLMNOPQRSTUVWXYZ"
            "abcdefghijklmnopqrstuvwxyz";

        std::string s;
        for (int i = 0; i < 30; ++i) {
            s += alphanum[dis(gen) % (sizeof(alphanum) - 1)];
        }
        return s;
    }

    public:
        explicit Originator(std::string state) : state_(std::move(state)) {
            std::cout << "Originator: My initial state is: " << this->state_ << "\n";
        }

        /*
         * The Originator's business logic may affect its internal state.
         * Therefore, the client should back up the state before launching methods
         * of the business logic via the save() method.
         */

        void DoSomething() {
            std::cout << "Originator: I'm doing something important.\n";
            this->state_ = Originator::GenerateRandomString();
            std::cout << "Originator: and my state has changed to: " << this->state_ << "\n";
        }

        /*
         * Saves the current state inside a memento.
         */

        SmartPtr<Memento> Save() {
            return SmartPtr<Memento>(new ConcreteMemento(this->state_));
        }

        /*
         * Restores the Originator's state from a memento object.
         */

        void Restore(const SmartPtr<Memento>& memento) {
            this->state_ = memento->State();
            std::cout << "Originator: My state has changed to: " << this->state_ << "\n";
        }
};

/*
 * The Caretaker doesn't depend on the Concrete Memento class. Therefore, it
 * doesn't have access to the originator's state, stored inside the memento. It
 * works with all mementos via the base Memento interface.
 */

class Caretaker {
        /*
     * @var Memento[]
     */
    private:
        std::vector<SmartPtr<Memento>> mementos_;
        SmartPtr<Originator> originator_;

    public:
        explicit Caretaker(const SmartPtr<Originator> &originator) : originator_(originator) {
            this->originator_ = originator;
        }

        ~Caretaker() = default;

        void Backup() {
            std::cout << "\nCaretaker: Saving Originator's state...\n";
            this->mementos_.push_back(this->originator_->Save());
        }

        void Undo() {
            if (this->mementos_.empty()) {
            return;
            }
            SmartPtr<Memento> memento = this->mementos_.back();
            this->mementos_.pop_back();
            std::cout << "Caretaker: Restoring state to: " << memento->Name() << "\n";
            try {
            this->originator_->Restore(memento);
            } catch (...) {
            this->Undo();
            }
        }

        void ShowHistory() const {
            std::cout << "Caretaker: Here's the list of mementos:\n";
            for (const SmartPtr<Memento> &memento: this->mementos_) {
            std::cout << memento->Name() << "\n";
            }
        }
};

/*
 * The client code.
 */

void ClientCode() {
    SmartPtr<Originator> originator = SmartPtr<Originator>(new Originator("Super-duper-super-super."));
    SmartPtr<Caretaker> caretaker = SmartPtr<Caretaker>(new Caretaker(originator));
    caretaker->Backup();
    originator->DoSomething();

    // Caretaker sometimes may need to save extra copies
    // of the originator's state for future rollback.
    caretaker->Backup();
    originator->DoSomething();

    caretaker->Backup();
    originator->DoSomething();

    std::cout << "\n";
    caretaker->ShowHistory();
    std::cout << "\nClient: Now, let's rollback!\n\n";
    caretaker->Undo();
    std::cout << "\nClient: Once more!\n\n";
    caretaker->Undo();
}

int main() {
    ClientCode();
    return 0;
}