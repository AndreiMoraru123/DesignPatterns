/*
 * Flyweight Design Pattern
 *
 * Intent: Lets you fit more objects into the available amount of RAM by sharing
 * common parts of state between multiple objects instead of keeping all the
 * data in each object.
 */

#include "../SmartPointer.h"
#include <iostream>
#include <map>
#include <string>
#include <unordered_map>
#include <utility>
#include <vector>


struct SharedState {
    std::string brand_;
    std::string model_;
    std::string color_;

    SharedState(std::string brand, std::string model, std::string color)
        : brand_(std::move(brand)), model_(std::move(model)), color_(std::move(color)) {}

    friend std::ostream &operator << (std::ostream &os, const SharedState &ss)
    {
        return os << "[ " << ss.brand_ << " , " << ss.model_ << " , " << ss.color_ << " ]";
    }
};

struct UniqueState {
    std::string owner_;
    std::string plates_;

    friend std::ostream &operator << (std::ostream &os, const UniqueState &us)
    {
        return os << "[ " << us.owner_ << " , " << us.plates_ << " ]";
    }
};

/*
 * The Flyweight stores a common portion of the state (also called intrinsic
 * state) that belongs to multiple real business entities. The Flyweight
 * accepts the rest of the state (extrinsic state, unique for each entity) via
 * its method parameters.
 */

class Flyweight {
private:
    SmartPtr<SharedState> shared_state_;
public:
    explicit Flyweight(const SmartPtr<SharedState> &sharedstate) : shared_state_(sharedstate) {}

    Flyweight(const Flyweight &other) : shared_state_(SmartPtr<SharedState>(new SharedState(*other.shared_state_))) {}

    ~Flyweight() = default;

    void Operation(const UniqueState &unique_state) const {
        std::cout << "Flyweight: Displaying shared (" << *this->shared_state_ << ") and unique (" << unique_state << ") state.\n";
    }
};

/*
 * The Flyweight Factory creates and manages the Flyweight objects. It ensures
 * that flyweights are shared correctly. When the client requests a flyweight,
 * the factory either returns an existing instance or creates a new one, if it
 * doesn't exist yet.
 */

class FlyweightFactory {
    /*
     * @var Flyweight[]
     */

private:
    std::unordered_map<std::string, Flyweight> flyweights_;
    /*
     * Returns a Flyweight's string hash for a given state.
     */
    [[nodiscard]] static std::string GetKey(const SharedState &ss) {
        return ss.brand_ + "_" + ss.model_ + "_" + ss.color_;
    }
public:
    FlyweightFactory(std::initializer_list<SharedState> share_states) {
        for (const SharedState &ss : share_states) {
            this->flyweights_.insert(std::make_pair<std::string, Flyweight>(GetKey(ss), Flyweight(SmartPtr<SharedState>(new SharedState(ss)))));
        }
    }

    /*
     * Returns an existing Flyweight with a given state or creates a new one.
     */

    Flyweight GetFlyweight(const SharedState &shared_state) {
        std::string key = GetKey(shared_state);
        if (this->flyweights_.find(key) == this->flyweights_.end()) {
            std::cout << "FlyweightFactory: Can't find a flyweight, creating new one.\n";
            this->flyweights_.insert(std::make_pair(key, Flyweight(SmartPtr<SharedState>(new SharedState(shared_state)))));
        } else {
            std::cout << "FlyweightFactory: Reusing existing flyweight.\n";
        }
        return this->flyweights_.at(key);
    }

    void ListFlyweights() const {
        size_t count = this->flyweights_.size();
        std::cout << "\nFlyweightFactory: I have " << count << " flyweights:\n";
        for (const auto &pair : this->flyweights_) {
            std::cout << pair.first << "\n";
        }
    }
};

void AddCarToPoliceDatabase(FlyweightFactory &ff, const std::string &plates, const std::string &owner,
                            const std::string &brand, const std::string &model, const std::string &color)
{
    std::cout << "\nClient: Adding a car to database.\n";
    const Flyweight &flyweight = ff.GetFlyweight({brand, model, color});
    // The client code either stores or calculates extrinsic state and passes it
    // to the flyweight's methods.
    flyweight.Operation({owner, plates});
}

/*
 * The client code usually creates a bunch of pre-populated flyweights in the
 * initialization stage of the application.
 */

int main() {
    SmartPtr<FlyweightFactory> factory = SmartPtr<FlyweightFactory> (new FlyweightFactory({{"Chevrolet", "Camaro2018", "pink"},
                                                                                           {"Mercedes Benz", "C300", "black"},
                                                                                           {"Mercedes Benz", "C500", "red"},
                                                                                           {"BMW", "M5", "red"},
                                                                                           {"BMW", "X6", "white"}}));

    factory->ListFlyweights();

    AddCarToPoliceDatabase(*factory, "CL234IR", "James Doe", "BMW", "M5", "red");
    AddCarToPoliceDatabase(*factory, "CL234IR", "James Doe", "BMW", "X1", "red");

    factory->ListFlyweights();
}