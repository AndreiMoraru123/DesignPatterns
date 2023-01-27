/**
* Observer Design Pattern
*
* Intent: Lets you define a subscription mechanism to notify multiple objects
* about any events that happen to the object they're observing.
*
* Note that there's a lot of different terms with similar meaning associated
* with this pattern. Just remember that the Subject is also called the
* Publisher and the Observer is often called the Subscriber and vice versa.
* Also the verbs "observe", "listen" or "track" usually mean the same thing.
*/

#include <iostream>
#include <list>
#include <string>
#include <utility>
#include "../SmartPointer.h"


class IObserver {
public:
    virtual ~IObserver() = default;
    virtual void Update(const std::string& message_from_subject) = 0;
};

class ISubject {
public:
    virtual ~ISubject() = default;
    virtual void Attach(SmartPtr<IObserver> observer) = 0;
    virtual void Detach(SmartPtr<IObserver> observer) = 0;
    virtual void Notify() = 0;
};

/**
 * The Subject owns some important state and notifies observers
 * when the state changes.
 */

class Subject : public ISubject {
    std::list<SmartPtr<IObserver>> list_observer_;
    std::string message_;
public:
    ~Subject() override {
        std::cout << "Goodbye, I was the Subject.\n";
    }

    /**
     * The subscription management methods.
     */

    void Attach(SmartPtr<IObserver> observer) override {
        list_observer_.push_back(observer);
    }
    void Detach(SmartPtr<IObserver> observer) override {
        list_observer_.remove(observer);
    }
    void Notify() override {
        auto iterator = list_observer_.begin();
        HowManyObserver();
        while (iterator != list_observer_.end()) {
            (*iterator)->Update(message_);
            ++iterator;
        }
    }

    void CreateMessage(std::string message = "Empty") {
        this->message_ = std::move(message);
        Notify();
    }

    void HowManyObserver() {
        std::cout << "There are " << list_observer_.size() << " observers in the list.\n";
    }

    /**
     * Usually, the subscription logic is only a fraction of what a Subject can
     * really do. Subjects commonly hold some important business logic, that
     * triggers a notification method whenever something important is about to
     * happen (or after it).
     */

    void SomeBusinessLogic() {
        this->message_ = "change message message";
        Notify();
        std::cout << "I'm about to do some thing important\n";
    }
};

/**
 * Concrete Observers react to the updates issued by the Subject they had been
 * attached to.
 */

class Observer : public IObserver {
    std::string message_from_subject_;
//    Subject& subject_;
    SmartPtr<Subject> subject_;
    static int static_number_;
    int number_;
public:
    explicit Observer(SmartPtr<Subject>& subject) : subject_(subject) {
        this->subject_->Attach(SmartPtr<IObserver>(new Observer(*this)));
        std::cout << "Hi, I'm the Observer \"" << ++Observer::static_number_ << "\".\n";
        number_ = Observer::static_number_;
    }
    ~Observer() override {
        std::cout << "Goodbye, I was the Observer \"" << this->number_ << "\".\n";
    }

    void Update(const std::string& message_from_subject) override {
        message_from_subject_ = message_from_subject;
        PrintInfo();
    }
    void RemoveMeFromTheList() {
        subject_->Detach(SmartPtr<IObserver>(new Observer(*this)));
        std::cout << "Observer \"" << this->number_ << "\" removed from the list.\n";
    }
    void PrintInfo() {
        std::cout << "Observer \"" << this->number_ << "\": a new message is available --> " << this->message_from_subject_ << "\n";
    }
};

int Observer::static_number_ = 0;

void ClientCode() {
    SmartPtr<Subject> subject = SmartPtr<Subject>(new Subject);
    SmartPtr<Observer> observer1 = SmartPtr<Observer>(new Observer(subject));
    SmartPtr<Observer> observer2 = SmartPtr<Observer>(new Observer(subject));
    SmartPtr<Observer> observer3 = SmartPtr<Observer>(new Observer(subject));
    SmartPtr<Observer> observer4;
    SmartPtr<Observer> observer5;

    subject->CreateMessage("Hello World! :D");
    observer3->RemoveMeFromTheList();

    subject->CreateMessage("The weather is hot today! :p");
    observer4 = SmartPtr<Observer>(new Observer(subject));

    observer2->RemoveMeFromTheList();
    observer5 = SmartPtr<Observer>(new Observer(subject));

    subject->CreateMessage("My new car is great! ;)");
    observer5->RemoveMeFromTheList();

    observer4->RemoveMeFromTheList();
    observer1->RemoveMeFromTheList();
}

int main() {
    ClientCode();
    return 0;
}