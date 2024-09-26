#include <iostream>
#include <vector>
#include <stdexcept>
#include <string>
#include <memory>
#include <algorithm>

using namespace std;

// Base class for Car,
class Car {
protected:
    string make;
    string model;
    double price;
    int quantity;

public:
    Car(string mk, string mdl, double pr, int qty) : make(mk), model(mdl), price(pr), quantity(qty) {}

    virtual void display() const {
        cout << "Make: " << make << ", Model: " << model << ", Price: $" << price << ", Quantity: " << quantity << endl;
    }

    virtual ~Car() {}

    void addQuantity(int qty) {
        quantity += qty;
    }

    void reduceQuantity(int qty) {
        if (quantity >= qty) {
            quantity -= qty;
        } else {
            throw out_of_range("Not enough cars in inventory.");
        }
    }

    int getQuantity() const {
        return quantity;
    }

    string getMake() const {
        return make;
    }

    string getModel() const {
        return model;
    }
};

// Derived class for Electric Car
class ElectricCar : public Car {
private:
    double batteryLife;

public:
    ElectricCar(string mk, string mdl, double pr, int qty, double bt) : Car(mk, mdl, pr, qty), batteryLife(bt) {}

    void display() const override {
        cout << "Make: " << make << ", Model: " << model << ", Price: $" << price << ", Quantity: " << quantity << ", Battery Life: " << batteryLife << " hours" << endl;
    }
};

// Derived class for Diesel Car
class DieselCar : public Car {
private:
    double fuelEfficiency;

public:
    DieselCar(string mk, string mdl, double pr, int qty, double fe) : Car(mk, mdl, pr, qty), fuelEfficiency(fe) {}

    void display() const override {
        cout << "Make: " << make << ", Model: " << model << ", Price: $" << price << ", Quantity: " << quantity << ", Fuel Efficiency: " << fuelEfficiency << " km/l" << endl;
    }
};

// Employee class
class Employee {
private:
    string name;
    int id;

public:
    Employee(string nm, int id) : name(nm), id(id) {}

    void display() const {
        cout << "Employee ID: " << id << ", Name: " << name << endl;
    }

    int getId() const {
        return id;
    }
};

// Dealership class
class Dealership {
private:
    string name;
    vector<unique_ptr<Car>> inventory;
    vector<unique_ptr<Employee>> employees;

public:
    Dealership(string nm) : name(nm) {}

    void addCar(unique_ptr<Car> car) {
        for (auto& c : inventory) {
            if (c->getMake() == car->getMake() && c->getModel() == car->getModel()) {
                c->addQuantity(car->getQuantity());
                return;
            }
        }
        inventory.push_back(move(car));
    }

    void removeCar(int index) {
        if (index < 0 || index >= inventory.size()) {
            throw out_of_range("Invalid car index.");
        }
        inventory.erase(inventory.begin() + index);
    }

    void showInventory() const {
        cout << "Inventory of " << name << ":" << endl;
        for (size_t i = 0; i < inventory.size(); ++i) {
            cout << i + 1 << ". ";
            inventory[i]->display();
        }
    }

    void addEmployee(unique_ptr<Employee> emp) {
        // Check if an employee with the same ID already exists
        for (const auto& e : employees) {
            if (e->getId() == emp->getId()) {
                throw invalid_argument("Employee with ID " + to_string(emp->getId()) + " is already present.");
            }
        }
        employees.push_back(move(emp));
    }

    void removeEmployee(int id) {
        auto it = remove_if(employees.begin(), employees.end(), [id](const unique_ptr<Employee>& emp) {
            return emp->getId() == id;
        });

        if (it == employees.end()) {
            throw invalid_argument("Employee not found.");
        }
        employees.erase(it, employees.end());
    }

    void showEmployees() const {
        cout << "Employees of " << name << ":" << endl;
        for (const auto& emp : employees) {
            emp->display();
        }
    }

    void buyCar(int index) {
        if (index < 0 || index >= inventory.size()) {
            throw out_of_range("Invalid car index.");
        }
        if (inventory[index]->getQuantity() == 0) {
            throw out_of_range("Car out of stock.");
        }
        cout << "Car purchased: " << inventory[index]->getMake() << endl;
        inventory[index]->reduceQuantity(1);
        if (inventory[index]->getQuantity() == 0) {
            inventory.erase(inventory.begin() + index);
        }
        printDecoration();
    }

    void printDecoration() const {
        cout << "\n**********************************************************\n";
        cout << "****************** Thank you for your purchase! **********\n";
        cout << "**********************************************************\n\n";
    }

    ~Dealership() {}
};

// Exception class for handling errors
class DealershipException : public runtime_error {
public:
    DealershipException(const string& message) : runtime_error(message) {}
};

void adminMenu(Dealership& dealership) {
    int choice;
    do {
        cout << "\nAdmin Menu:\n";
        cout << "1. Order cars\n";
        cout << "2. View employee data\n";
        cout << "3. Add employees\n";
        cout << "4. Remove employee\n";
        cout << "5. Remove old car\n";
        cout << "6. View cars\n";
        cout << "7. Exit\n";
        cout << "Enter your choice: ";
        cin >> choice;

        switch (choice) {
        case 1: {
            // Ordering cars (Adding new cars to the inventory)
            int carType;
            cout << "Enter 1 for Electric Car, 2 for Diesel Car: ";
            cin >> carType;

            string make, model;
            double price;
            int quantity;
            cout << "Enter make: ";
            cin >> make;
            cout << "Enter model: ";
            cin >> model;
            cout << "Enter price: ";
            cin >> price;
            cout << "Enter quantity: ";
            cin >> quantity;

            if (carType == 1) {
                double batteryLife;
                cout << "Enter battery life: ";
                cin >> batteryLife;
                dealership.addCar(make_unique<ElectricCar>(make, model, price, quantity, batteryLife));
            } else if (carType == 2) {
                double fuelEfficiency;
                cout << "Enter fuel efficiency: ";
                cin >> fuelEfficiency;
                dealership.addCar(make_unique<DieselCar>(make, model, price, quantity, fuelEfficiency));
            } else {
                cout << "Invalid car type.\n";
            }
            break;
        }
        case 2:
            dealership.showEmployees();
            break;
        case 3: {
            // Adding employees
            string name;
            int id;
            cout << "Enter employee name: ";
            cin >> name;
            cout << "Enter employee ID: ";
            cin >> id;
            try {
                dealership.addEmployee(make_unique<Employee>(name, id));
                cout << "Employee added.\n";
            } catch (const exception& e) {
                cout << e.what() << endl;
            }
            break;
        }
        case 4: {
            // Removing employees
            int id;
            cout << "Enter employee ID to remove: ";
            cin >> id;
            try {
                dealership.removeEmployee(id);
                cout << "Employee removed.\n";
            } catch (const exception& e) {
                cout << e.what() << endl;
            }
            break;
        }
        case 5: {
            // Removing old cars
            int index;
            dealership.showInventory();
            cout << "Enter car number to remove: ";
            cin >> index;
            try {
                dealership.removeCar(index - 1);
                cout << "Car removed.\n";
            } catch (const exception& e) {
                cout << e.what() << endl;
            }
            break;
        }
        case 6:
            dealership.showInventory();
            break;
        case 7:
            cout << "Exiting admin menu.\n";
            break;
        default:
            cout << "Invalid choice. Please try again.\n";
        }
    } while (choice != 7);
}

void customerMenu(Dealership& dealership) {
    int choice;
    dealership.showInventory();
    cout << "Enter the number of the car you want to buy: ";
    cin >> choice;
    try {
        dealership.buyCar(choice - 1);
        cout << "Car purchased successfully.\n";
    } catch (const exception& e) {
        cout << e.what() << endl;

} catch (const exception& e) {
        cout << e.what() << endl;
    }
}

int main() {
    try {
        Dealership myDealership("Auto World");

        // Adding some initial data
        myDealership.addCar(make_unique<ElectricCar>("Tesla", "Model S", 79999.99, 2, 300));
        myDealership.addCar(make_unique<DieselCar>("Ford", "F-150", 59999.99, 3, 15));
        myDealership.addCar(make_unique<ElectricCar>("Nissan", "Leaf", 31999.99, 5, 150));

        myDealership.addEmployee(make_unique<Employee>("Alice", 1));
        myDealership.addEmployee(make_unique<Employee>("Bob", 2));

        int choice;
        do {
            cout << "\nPress:\n1 for Admin\n2 for Customer\n3 to Exit\nEnter your choice: ";
            cin >> choice;

            switch (choice) {
            case 1 :

                adminMenu(myDealership);
                break;
            case 2:
                customerMenu(myDealership);
                break;
            case 3:
                cout << "Exiting program.\n";
                break;
            default:
                cout << "Invalid choice. Please try again.\n";
            }

        } while (choice != 3);

    } catch (const exception& e) {
        cerr << "Exception occurred: " << e.what() << endl;
    }

    return 0;
}
