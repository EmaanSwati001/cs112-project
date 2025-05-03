#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <iomanip>
#include <cstdlib>
#include <stdexcept>
using namespace std;

// === Clear screen (Optional) ===
void clearScreen() {
#ifdef _WIN32
    system("cls");
#else
    system("clear");
#endif
}

// === Abstract Base Class ===
class CityObject {
protected:
    string name;
    int level;
public:
    CityObject(string name) : name(name), level(1) {}
    virtual void operate() = 0;
    virtual void upgrade() {
        level++;
        cout << name << " upgraded to level " << level << "!\n";
    }
    virtual ~CityObject() {}
};

// === Derived Classes ===
class Building : public CityObject {
protected:
    int capacity;
public:
    Building(string name, int cap) : CityObject(name), capacity(cap) {}
    virtual void operate() {
        cout << name << " is operating with capacity " << capacity << ".\n";
    }
};

class ResidentialZone : public Building {
public:
    ResidentialZone() : Building("Residential Zone", 100) {}
    void operate() {
        cout << name << " provides housing for " << capacity << " citizens.\n";
    }
};

class CommercialDistrict : public Building {
public:
    CommercialDistrict() : Building("Commercial District", 50) {}
    void operate() {
        cout << name << " provides jobs for " << capacity << " citizens.\n";
    }
};

// === User System ===
class User {
public:
    string username, password;

    // Each user has their own credential file: username_user.txt
    bool login() {
        ifstream file((username + "_user.txt").c_str());
        if (!file.is_open()) {
            cout << "User not found.\n";
            return false;
        }
        string storedPassword;
        getline(file, storedPassword);
        file.close();
        return (storedPassword == password);
    }

    bool registerUser() {
        // Check if user already exists
        ifstream file((username + "_user.txt").c_str());
        if (file.is_open()) {
            file.close();
            return false; // User already exists
        }
        ofstream out((username + "_user.txt").c_str());
        if (!out.is_open()) {
            cout << "Could not create user file.\n";
            return false;
        }
        out << password << endl;
        out.close();
        return true;
    }

    // Each user has their own city data file: username_city.txt
    void saveCityData(int money, int eco, int population, int day, int resLevel, int comLevel, int resCount, int comCount) {
        ofstream out((username + "_city.txt").c_str());
        out << money << " " << eco << " " << population << " " << day << " "
            << resLevel << " " << comLevel << " " << resCount << " " << comCount << endl;
        out.close();
    }

    bool loadCityData(int& money, int& eco, int& population, int& day, int& resLevel, int& comLevel, int& resCount, int& comCount) {
        ifstream in((username + "_city.txt").c_str());
        if (!in.is_open()) return false;
        in >> money >> eco >> population >> day >> resLevel >> comLevel >> resCount >> comCount;
        in.close();
        return true;
    }
};

// === Menu Display ===
void displayMainMenu(string mayor, string city = "GreenHaven", float eco = 12.0, float money = 1.2, int people = 4200, int co2 = 34, int day = 27) {
    cout << "==========================================\n";
    cout << "   SMART ECO CITY SIMULATION - MAIN MENU  \n";
    cout << "==========================================\n";
    cout << "[Mayor: " << mayor << "] [City: " << city << "] [?" << eco << "% Eco]\n";
    cout << "[??" << fixed << setprecision(1) << money << "M] [??" << people << "] [??" << co2 << "ppm] [Day: " << day << "]\n\n";
    cout << "?1. BUILD & UPGRADE\n";
    cout << "?2. MANAGE ENVIRONMENT\n";
    cout << "?3. CONTROL TRANSPORT\n";
    cout << "?4. CITIZEN ACTIONS\n";
    cout << "?5. CITY STATS\n";
    cout << "?6. SOCIAL FEATURES\n";
    cout << "?7. MAYOR'S OFFICE\n";
    cout << "0. ?? LOGOUT\n";
    cout << "==========================================\n";
    cout << "Enter action (0-7): ";
}

// === Environment Activities (Polymorphism & Virtual Functions) ===
class Activity {
public:
    virtual void perform(int& eco, int& money, int& population) = 0;
    virtual string name() const = 0;
    virtual ~Activity() {}
};

class Recycling : public Activity {
public:
    void perform(int& eco, int& money, int& population) {
        eco += 3;
        money -= 50;
        cout << "Recycling drive successful! Eco +3, Money -50\n";
    }
    string name() const { return "Recycling"; }
};

class TreePlanting : public Activity {
public:
    void perform(int& eco, int& money, int& population) {
        eco += 5;
        money -= 100;
        cout << "Tree planting event! Eco +5, Money -100\n";
    }
    string name() const { return "Tree Planting"; }
};

class EnergyProduction : public Activity {
public:
    void perform(int& eco, int& money, int& population) {
        eco += 2;
        money += 150;
        cout << "Solar energy produced! Eco +2, Money +150\n";
    }
    string name() const { return "Energy Production"; }
};

// === Transportation (Polymorphism) ===
class Transport {
public:
    virtual void operate(int& eco, int& money) = 0;
    virtual string name() const = 0;
    virtual ~Transport() {}
};

class Bike : public Transport {
public:
    void operate(int& eco, int& money) {
        eco += 2;
        cout << "Bike lanes expanded! Eco +2\n";
    }
    string name() const { return "Bike"; }
};

class Bus : public Transport {
public:
    void operate(int& eco, int& money) {
        eco += 1;
        money -= 100;
        cout << "New buses added! Eco +1, Money -100\n";
    }
    string name() const { return "Bus"; }
};

class EV : public Transport {
public:
    void operate(int& eco, int& money) {
        eco += 3;
        money -= 200;
        cout << "EV charging stations built! Eco +3, Money -200\n";
    }
    string name() const { return "EV"; }
};

// === Citizen Actions (Virtual Functions) ===
class CitizenAction {
public:
    virtual void act(int& eco, int& money, int& population) = 0;
    virtual string name() const = 0;
    virtual ~CitizenAction() {}
};

class Commute : public CitizenAction {
public:
    void act(int& eco, int& money, int& population) {
        eco -= 1;
        cout << "Citizens commute to work. Eco -1\n";
    }
    string name() const { return "Commute"; }
};

class Protest : public CitizenAction {
public:
    void act(int& eco, int& money, int& population) {
        money -= 50;
        cout << "Citizens protest for more parks! Money -50\n";
    }
    string name() const { return "Protest"; }
};

class Celebrate : public CitizenAction {
public:
    void act(int& eco, int& money, int& population) {
        population += 10;
        cout << "City festival! Population +10\n";
    }
    string name() const { return "Celebrate"; }
};

// === Template for Logs/History ===
template<typename T>
class Log {
    T entries[100];
    int count;
public:
    Log() : count(0) {}
    void add(const T& entry) {
        if (count < 100) entries[count++] = entry;
    }
    void show() const {
        for (int i = 0; i < count; ++i) {
            cout << (i + 1) << ". " << entries[i] << endl;
        }
    }
};

// === Exception for Invalid Actions ===
class ActionException : public std::exception {
    string msg;
public:
    ActionException(const string& m) : msg(m) {}
    virtual ~ActionException() throw() {}
    const char* what() const throw() { return msg.c_str(); }
};

// === Simulation Core (runSimulation) ===
void runSimulation(string username) {
    CityObject* buildings[100];
    int buildingCount = 0;

    int money = 1000;
    int eco = 10;
    int population = 100;
    int day = 1;
    int resLevel = 1, comLevel = 1, resCount = 1, comCount = 1;
    int numBikes = 0, numBuses = 0, numEVs = 0;
    User user;
    user.username = username;

    // Try to load city data
    if (user.loadCityData(money, eco, population, day, resLevel, comLevel, resCount, comCount)) {
        // Load buildings based on saved data
        for (int i = 0; i < resCount; ++i) {
            ResidentialZone* rz = new ResidentialZone();
            for (int l = 1; l < resLevel; ++l) rz->upgrade();
            buildings[buildingCount++] = rz;
        }
        for (int i = 0; i < comCount; ++i) {
            CommercialDistrict* cd = new CommercialDistrict();
            for (int l = 1; l < comLevel; ++l) cd->upgrade();
            buildings[buildingCount++] = cd;
        }
    } else {
        // Default city
        buildings[buildingCount++] = new ResidentialZone();
        buildings[buildingCount++] = new CommercialDistrict();
    }

    Log<string> activityLog, transportLog, citizenLog;
    Activity* activities[3] = { new Recycling(), new TreePlanting(), new EnergyProduction() };
    Transport* transports[3] = { new Bike(), new Bus(), new EV() };
    CitizenAction* actions[3] = { new Commute(), new Protest(), new Celebrate() };

    int choice;
    do {
        clearScreen();
        displayMainMenu(username, "GreenHaven", eco, money / 100.0, population, 34, day);
        cin >> choice;
        cin.ignore();

        try {
        switch (choice) {
            case 1: {
            cout << "\n--- BUILD & UPGRADE ---\n";
                cout << "Your resources: Money: $" << money << ", Eco: " << eco << ", Population: " << population << "\n";
                cout << "1. Upgrade Residential Zone (Cost: $200)\n";
                cout << "2. Upgrade Commercial District (Cost: $300)\n";
                cout << "3. Build New Residential Zone (Cost: $500)\n";
                cout << "4. Build New Commercial District (Cost: $700)\n";
                cout << "0. Back\n";
                cout << "Choose an option: ";
                int buildChoice;
                cin >> buildChoice;
                cin.ignore();

                if (buildChoice == 1 && money >= 200) {
                    // Upgrade all residential zones
                    for (int i = 0; i < buildingCount; ++i)
                        if (dynamic_cast<ResidentialZone*>(buildings[i])) buildings[i]->upgrade();
                    money -= 200;
                    population += 50;
                    resLevel++;
                    cout << "Population increased! (+50)\n";
                } else if (buildChoice == 2 && money >= 300) {
                    // Upgrade all commercial districts
                    for (int i = 0; i < buildingCount; ++i)
                        if (dynamic_cast<CommercialDistrict*>(buildings[i])) buildings[i]->upgrade();
                    money -= 300;
                    eco += 2;
                    comLevel++;
                    cout << "Eco improved! (+2)\n";
                } else if (buildChoice == 3 && money >= 500 && buildingCount < 100) {
                    buildings[buildingCount++] = new ResidentialZone();
                    money -= 500;
                    population += 100;
                    resCount++;
                    cout << "New Residential Zone built! (+100 population)\n";
                } else if (buildChoice == 4 && money >= 700 && buildingCount < 100) {
                    buildings[buildingCount++] = new CommercialDistrict();
                    money -= 700;
                    eco += 5;
                    comCount++;
                    cout << "New Commercial District built! (+5 eco)\n";
                } else if (buildChoice == 0) {
                    break;
                } else {
                    cout << "Not enough money or invalid choice!\n";
                }

                // Random event
                int event = rand() % 5;
                if (event == 0) {
                    cout << "?? Bonus! You received a government grant of $200!\n";
                    money += 200;
                } else if (event == 1) {
                    cout << "? Power outage! Eco drops by 2.\n";
                    eco -= 2;
                } else if (event == 2) {
                    cout << "?? Citizens are happy! Population increases by 20.\n";
                    population += 20;
                }

                day++;
                break;
            }
            case 2: { // Manage Environment
                cout << "\n--- MANAGE ENVIRONMENT ---\n";
                cout << "1. Recycling\n2. Tree Planting\n3. Energy Production\n4. Turn of light when not in use\n0. Back\nChoose: ";
                int envChoice;
                cin >> envChoice;
                cin.ignore();
                if (envChoice >= 1 && envChoice <= 3) {
                    activities[envChoice - 1]->perform(eco, money, population);
                    activityLog.add(activities[envChoice - 1]->name());
                }else if (envChoice==4 ) {
                        eco+=2;
                        cout<<"you encourage people to turn off the light when not in use\nEco improved +2\n";
                        activityLog.add("turn off light when not in use");
                    }
             else if (envChoice != 0) {
                    throw ActionException("Invalid environment action!");
            }
            break;
            }
            case 3: { // Control Transport
                cout << "\n--- CONTROL TRANSPORT ---\n";
                cout << "1. Bike\n2. Bus\n3. EV\n0. Back\nChoose: ";
                int tChoice;
                cin >> tChoice;
                cin.ignore();
                if (tChoice >= 1 && tChoice <= 3) {
                    transports[tChoice - 1]->operate(eco, money);
                    transportLog.add(transports[tChoice - 1]->name());
                    if (tChoice == 1) numBikes++;
                    else if (tChoice == 2) numBuses++;
                    else if (tChoice == 3) numEVs++;
                    if (tChoice == 1)
        cout << "Bike lanes expanded! Total bike lanes: " << numBikes << ". Eco +2\n";
    else if (tChoice == 2)
        cout << "New buses added! Total buses: " << numBuses << ". Eco +1, Money -100\n";
    else if (tChoice == 3)
        cout << "EV charging stations built! Total EV stations: " << numEVs << ". Eco +3, Money -200\n";
        
                } else if (tChoice != 0) {
                    throw ActionException("Invalid transport action!");
                }
            break;
            }
            case 4: { // Citizen Actions
                cout << "\n--- CITIZEN ACTIONS ---\n";
                cout << "1. Commute\n2. Protest\n3. Celebrate\n0. Back\nChoose: ";
                int cChoice;
                cin >> cChoice;
                cin.ignore();
                if (cChoice >= 1 && cChoice <= 3) {
                    actions[cChoice - 1]->act(eco, money, population);
                    citizenLog.add(actions[cChoice - 1]->name());
                } 
                else if (cChoice != 0) {
                    throw ActionException("Invalid citizen action!");
                }
            break;
            }
            case 5: { // City Stats
                cout << "\n--- CITY STATS ---\n";
                cout << "Eco: " << eco << "\nMoney: $" << money << "\nPopulation: " << population << "\n";
                cout << "Recent Activities:\n"; activityLog.show();
                cout << "Recent Transport Actions:\n"; transportLog.show();
                cout << "Recent Citizen Actions:\n"; citizenLog.show();
                cout << "Bike Lanes: " << numBikes << ", Buses: " << numBuses << ", EV Stations: " << numEVs << endl;
            break;
            }
            case 6: { // Social Features
                cout << "\n--- SOCIAL FEATURES ---\n";
                cout << "Leaderboard (Demo):\n";
                cout << "1. " << username << " - Eco: " << eco << ", Population: " << population << "\n";
                cout << "Citizen Feedback: \"We love the green spaces!\"\n";
            break;
            }
            case 7: { // Mayor's Office
                cout << "\n--- MAYOR'S OFFICE ---\n";
                if (eco > 20) cout << "Achievement: Eco Champion!\n";
                if (population > 500) cout << "Achievement: Growing City!\n";
                if (money < 0) cout << "Alert: City is in debt!\n";
            break;
            }
        case 0:
            cout << "Logging out...\n";
            break;
        default:
                throw ActionException("Invalid main menu choice!");
            }
        } catch (const ActionException& e) {
            cout << "Error: " << e.what() << endl;
        }

        // Save city data after each action
        user.saveCityData(money, eco, population, day, resLevel, comLevel, resCount, comCount);
        ofstream rankingOut("city_ranking.txt", ios::app);
        rankingOut<<user.username<<" "<<eco<<" "<<population<<endl;
        rankingOut.close();
        if (choice != 0) {
            cout << "\nPress Enter to continue...";
            cin.get();
        }
        day++;
    } while (choice != 0);

    for (int i = 0; i < buildingCount; ++i)
        delete buildings[i];
    for (int i = 0; i < 3; ++i) {
        delete activities[i];
        delete transports[i];
        delete actions[i];
    }
}

// === Main ===
int main() {
    int option;
    string username, password;

    while (true) {
        clearScreen();
        cout << "==========================================\n";
        cout << "   SMART ECO CITY SIMULATION - WELCOME\n";
        cout << "==========================================\n";
        cout << "1. Login ? Existing City\n";
        cout << "2. Register ? New Mayor Account\n";
        cout << "3. Quick Play ? Demo Mode (No Save)\n";
        cout << "0. Exit\n";
        cout << "==========================================\n";
        cout << "Enter choice (0-3): ";
        cin >> option;

        if (cin.fail()) {
            cin.clear();
            cin.ignore(10000, '\n');
            cout << "Invalid input. Please enter a number.\n";
            cout << "\nPress Enter to continue...";
            cin.get();
            cin.get();
            continue;
        }
        cin.ignore();

        User user;

        if (option == 1) {
            cout << "Enter Username: ";
            getline(cin, user.username);
            cout << "Enter Password: ";
            getline(cin, user.password);

            if (user.login()) {
                cout << "Login successful!\n";
                runSimulation(user.username);
            } else {
                cout << "Invalid credentials.\n";
            }
        } else if (option == 2) {
            cout << "Choose Username: ";
            getline(cin, user.username);
            cout << "Choose Password: ";
            getline(cin, user.password);

            if (user.registerUser()) {
                cout << "Registration successful!\n";
            } else {
                cout << "Username already exists.\n";
            }
        } else if (option == 3) {
            runSimulation("Guest");
        } else if (option == 0) {
            break;
        } else {
            cout << "Invalid choice.\n";
        }

        cout << "\nPress Enter to continue...";
        cin.get();
    }

    return 0;
}







