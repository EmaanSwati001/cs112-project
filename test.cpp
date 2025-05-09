#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <iomanip>
#include <cstdlib>
#include <stdexcept>
#include <vector>
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

class IndustrialZone : public Building {
public:
    IndustrialZone() : Building("Industrial Zone", 75) {}
    void operate() {
        cout << name << " provides industrial jobs for " << capacity << " citizens.\n";
    }
};

class LuxuryResidence : public Building {
public:
    LuxuryResidence() : Building("Luxury Residence", 25) {}
    void operate() {
        cout << name << " provides premium housing for " << capacity << " wealthy citizens.\n";
    }
};

class ShoppingMall : public Building {
public:
    ShoppingMall() : Building("Shopping Mall", 150) {}
    void operate() {
        cout << name << " provides retail space for " << capacity << " visitors.\n";
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
    cout << "?1. REAL ESTATE\n";
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
private:
    struct EnergySource {
        string name;
        int cost;
        int ecoImpact;
        int moneyGenerated;
        string description;
        int efficiency;  // New field for tracking efficiency
        int maintenanceLevel;  // New field for tracking maintenance
    };

    struct EnergyReport {
        string sourceName;
        int daysOperational;
        int totalEnergyGenerated;
        int totalIncome;
        int efficiency;
        int maintenanceLevel;
        string status;
    };

    vector<EnergyReport> energyReports;  // Track all energy installations

    EnergySource sources[3] = {
        {"Solar Panels", 200, 3, 100, "Traditional solar panel installation", 85, 100},
        {"Solar Roads", 500, 5, 200, "Roads that generate energy from sunlight and vehicle friction", 90, 100},
        {"Hydroelectric Dam", 1000, 4, 300, "Dam that generates power from water flow", 95, 100}
    };

    void displayEnergyReport() {
        if (energyReports.empty()) {
            cout << "\nNo energy infrastructure installed yet.\n";
            return;
        }

        cout << "\n=== ENERGY INFRASTRUCTURE REPORT ===\n";
        for (const auto& report : energyReports) {
            cout << "\nInstallation: " << report.sourceName << "\n";
            cout << "Days Operational: " << report.daysOperational << "\n";
            cout << "Total Energy Generated: " << report.totalEnergyGenerated << " MWh\n";
            cout << "Total Income: $" << report.totalIncome << "\n";
            cout << "Efficiency: " << report.efficiency << "%\n";
            cout << "Maintenance Level: " << report.maintenanceLevel << "%\n";
            cout << "Status: " << report.status << "\n";
            cout << "----------------------------------------\n";
        }
    }

public:
    void perform(int& eco, int& money, int& population) {
        cout << "\n--- ENERGY PRODUCTION OPTIONS ---\n";
        cout << "1. Build New Energy Source\n";
        cout << "2. View Energy Infrastructure Report\n";
        cout << "0. Back\n";
        cout << "Choose an option: ";
        
        int menuChoice;
        cin >> menuChoice;
        cin.ignore();

        if (menuChoice == 2) {
            displayEnergyReport();
            return;
        } else if (menuChoice != 1) {
            return;
        }

        cout << "\nSelect energy source to implement:\n";
        for (int i = 0; i < 3; i++) {
            cout << (i + 1) << ". " << sources[i].name 
                 << " (Cost: $" << sources[i].cost 
                 << ", Eco Impact: +" << sources[i].ecoImpact 
                 << ", Income: $" << sources[i].moneyGenerated << ")\n";
            cout << "   " << sources[i].description << "\n";
        }
        cout << "0. Cancel\n";
        cout << "Choose energy source (0-3): ";
        
        int choice;
        cin >> choice;
        cin.ignore();

        if (choice >= 1 && choice <= 3) {
            EnergySource& source = sources[choice - 1];
            
            if (money >= source.cost) {
                money -= source.cost;
                eco += source.ecoImpact;
                money += source.moneyGenerated;
                
                // Create new energy report
                EnergyReport newReport;
                newReport.sourceName = source.name;
                newReport.daysOperational = 0;
                newReport.totalEnergyGenerated = 0;
                newReport.totalIncome = source.moneyGenerated;
                newReport.efficiency = source.efficiency;
                newReport.maintenanceLevel = source.maintenanceLevel;
                newReport.status = "Operational";
                energyReports.push_back(newReport);
                
                cout << "\n=== ENERGY PRODUCTION REPORT ===\n";
                cout << "Source: " << source.name << "\n";
                cout << "Description: " << source.description << "\n";
                cout << "Initial Cost: $" << source.cost << "\n";
                cout << "Eco Impact: +" << source.ecoImpact << "\n";
                cout << "Initial Income: $" << source.moneyGenerated << "\n";
                cout << "Initial Efficiency: " << source.efficiency << "%\n";
                cout << "Maintenance Level: " << source.maintenanceLevel << "%\n";
                cout << "Status: Operational\n";

                // Special effects based on energy source
                switch (choice) {
                    case 1: // Solar Panels
                        cout << "\nSolar panels installed on buildings. Population +5\n";
                        population += 5;
                        break;
                    case 2: { // Solar Roads
                        cout << "\nSolar roads installed. Additional effects:\n";
                        int roadCondition = rand() % 3;
                        switch (roadCondition) {
                            case 0:
                                cout << "Roads in perfect condition! Extra income +$50\n";
                                money += 50;
                                newReport.totalIncome += 50;
                                break;
                            case 1:
                                cout << "Normal road conditions. No additional effects.\n";
                                break;
                            case 2:
                                cout << "Roads need maintenance. Cost -$30\n";
                                money -= 30;
                                newReport.maintenanceLevel -= 10;
                                break;
                        }
                        break;
                    }
                    case 3: { // Hydroelectric Dam
                        cout << "\nHydroelectric dam constructed. Additional effects:\n";
                        int waterLevel = rand() % 3;
                        switch (waterLevel) {
                            case 0:
                                cout << "High water levels! Maximum power generation. Income +$100\n";
                                money += 100;
                                newReport.totalIncome += 100;
                                newReport.efficiency = 100;
                                break;
                            case 1:
                                cout << "Normal water levels. Standard power generation.\n";
                                break;
                            case 2:
                                cout << "Low water levels. Reduced power generation. Income -$50\n";
                                money -= 50;
                                newReport.efficiency -= 10;
                                break;
                        }
                        cout << "Dam creates new water ecosystem. Eco +2\n";
                        eco += 2;
                        break;
                    }
                }

                // Update the report in the vector
                energyReports.back() = newReport;

                // Random maintenance event (20% chance)
                if (rand() % 5 == 0) {
                    cout << "\nMaintenance Required: ";
                    int maintenance = rand() % 3;
                    switch (maintenance) {
                        case 0:
                            cout << "Regular maintenance needed. Cost -$20\n";
                            money -= 20;
                            newReport.maintenanceLevel += 20;
                            break;
                        case 1:
                            cout << "System upgrade available. Cost -$50, Income +$30\n";
                            money -= 50;
                            money += 30;
                            newReport.efficiency += 5;
                            newReport.totalIncome += 30;
                            break;
                        case 2:
                            cout << "Emergency repair needed. Cost -$100, Eco -1\n";
                            money -= 100;
                            eco -= 1;
                            newReport.maintenanceLevel = 100;
                            newReport.efficiency -= 10;
                            break;
                    }
                    // Update the report again
                    energyReports.back() = newReport;
                }

                // Display final report
                cout << "\n=== FINAL INSTALLATION REPORT ===\n";
                cout << "Source: " << newReport.sourceName << "\n";
                cout << "Total Income Generated: $" << newReport.totalIncome << "\n";
                cout << "Current Efficiency: " << newReport.efficiency << "%\n";
                cout << "Maintenance Level: " << newReport.maintenanceLevel << "%\n";
                cout << "Status: " << newReport.status << "\n";
            } else {
                cout << "Not enough money to implement this energy source!\n";
            }
        }
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

class Deport : public CitizenAction {
private:
    struct IllegalActivity {
        string name;
        int fine;
        int populationImpact;
        int ecoImpact;
        string description;
    };

    IllegalActivity activities[5] = {
        {"Tax Evasion", 100, 2, 0, "Residents avoiding tax payments"},
        {"Environmental Violation", 150, 3, 2, "Illegal waste disposal and pollution"},
        {"Property Crime", 200, 4, -1, "Vandalism and property damage"},
        {"Business Violation", 300, 5, -2, "Operating illegal businesses"},
        {"Public Disturbance", 50, 1, 1, "Creating public nuisance"}
    };

public:
    void act(int& eco, int& money, int& population) {
        cout << "\n--- ILLEGAL ACTIVITIES DETECTED ---\n";
        cout << "Select type of violation to address:\n";
        for (int i = 0; i < 5; i++) {
            cout << (i + 1) << ". " << activities[i].name 
                 << " (Fine: $" << activities[i].fine 
                 << ", Population Impact: " << activities[i].populationImpact 
                 << ", Eco Impact: " << activities[i].ecoImpact << ")\n";
        }
        cout << "0. Cancel\n";
        cout << "Choose violation type (0-5): ";
        
        int choice;
        cin >> choice;
        cin.ignore();

        if (choice >= 1 && choice <= 5) {
            IllegalActivity& activity = activities[choice - 1];
            
            // Apply immediate effects
            population -= activity.populationImpact;
            money += activity.fine;
            eco += activity.ecoImpact;
            
            cout << "\nDeportation Report:\n";
            cout << "Violation: " << activity.name << "\n";
            cout << "Description: " << activity.description << "\n";
            cout << "Deported: " << activity.populationImpact << " residents\n";
            cout << "Fine collected: $" << activity.fine << "\n";
            cout << "Eco impact: " << (activity.ecoImpact >= 0 ? "+" : "") << activity.ecoImpact << "\n";

            // Random consequences
            int consequence = rand() % 5;
            switch (consequence) {
                case 0:
                    cout << "Consequence: Community unrest! Population -5, Eco -2\n";
                    population -= 5;
                    eco -= 2;
                    break;
                case 1:
                    cout << "Consequence: Smooth process. No additional effects.\n";
                    break;
                case 2:
                    cout << "Consequence: Community supports action! Money +$100, Eco +1\n";
                    money += 100;
                    eco += 1;
                    break;
                case 3:
                    cout << "Consequence: Media attention! Population -2, Money +$200\n";
                    population -= 2;
                    money += 200;
                    break;
                case 4:
                    cout << "Consequence: Legal challenges! Money -$50, Eco +2\n";
                    money -= 50;
                    eco += 2;
                    break;
            }

            // Additional random event
            if (rand() % 10 == 0) { // 10% chance
                cout << "\nSpecial Event: ";
                int specialEvent = rand() % 3;
                switch (specialEvent) {
                    case 0:
                        cout << "Mass protest! Population -10, Eco -3\n";
                        population -= 10;
                        eco -= 3;
                        break;
                    case 1:
                        cout << "Community support rally! Population +5, Eco +2\n";
                        population += 5;
                        eco += 2;
                        break;
                    case 2:
                        cout << "Government investigation! Money -$100, Eco +1\n";
                        money -= 100;
                        eco += 1;
                        break;
                }
            }
        }
    }
    string name() const { return "Deport"; }
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
    int resLevel = 1, comLevel = 1, indLevel = 1, luxLevel = 1, mallLevel = 1;
    int resCount = 1, comCount = 1, indCount = 0, luxCount = 0, mallCount = 0;

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
    CitizenAction* actions[4] = { new Commute(), new Protest(), new Celebrate(), new Deport() };

    int choice;
    do {
        clearScreen();
        displayMainMenu(username, "GreenHaven", eco, money / 100.0, population, 34, day);
        cin >> choice;
        cin.ignore();

        try {
        switch (choice) {
            case 1: {
                cout << "\n--- REAL ESTATE ---\n";
                cout << "Your resources: Money: $" << money << ", Eco: " << eco << ", Population: " << population << "\n";
                cout << "1. Residential Properties\n";
                cout << "2. Commercial Properties\n";
                cout << "3. Industrial Properties\n";
                cout << "4. Luxury Properties\n";
                cout << "5. View Property Portfolio\n";
                cout << "6. Property Maintenance\n";
                cout << "0. Back\n";
                cout << "Choose an option: ";
                int realEstateChoice;
                cin >> realEstateChoice;
                cin.ignore();

                switch (realEstateChoice) {
                    case 1: { // Residential Properties
                        cout << "\n--- RESIDENTIAL PROPERTIES ---\n";
                        cout << "1. Upgrade Residential Zone (Cost: $200)\n";
                        cout << "2. Build New Residential Zone (Cost: $500)\n";
                        cout << "3. Build Luxury Residence (Cost: $1000)\n";
                        cout << "0. Back\n";
                        cout << "Choose an option: ";
                        int resChoice;
                        cin >> resChoice;
                        cin.ignore();

                        if (resChoice == 1 && money >= 200) {
                            for (int i = 0; i < buildingCount; ++i)
                                if (dynamic_cast<ResidentialZone*>(buildings[i])) buildings[i]->upgrade();
                            money -= 200;
                            population += 50;
                            resLevel++;
                            cout << "Residential zones upgraded! Population +50\n";
                        } else if (resChoice == 2 && money >= 500 && buildingCount < 100) {
                            buildings[buildingCount++] = new ResidentialZone();
                            money -= 500;
                            population += 100;
                            resCount++;
                            cout << "New Residential Zone built! (+100 population)\n";
                        } else if (resChoice == 3 && money >= 1000 && buildingCount < 100) {
                            buildings[buildingCount++] = new LuxuryResidence();
                            money -= 1000;
                            population += 25;
                            money += 200; // Luxury properties generate more income
                            luxCount++;
                            cout << "New Luxury Residence built! (+25 population, +$200 income)\n";
                        }
                        break;
                    }
                    case 2: { // Commercial Properties
                        cout << "\n--- COMMERCIAL PROPERTIES ---\n";
                        cout << "1. Upgrade Commercial District (Cost: $300)\n";
                        cout << "2. Build New Commercial District (Cost: $700)\n";
                        cout << "3. Build Shopping Mall (Cost: $1500)\n";
                        cout << "0. Back\n";
                        cout << "Choose an option: ";
                        int comChoice;
                        cin >> comChoice;
                        cin.ignore();

                        if (comChoice == 1 && money >= 300) {
                            for (int i = 0; i < buildingCount; ++i)
                                if (dynamic_cast<CommercialDistrict*>(buildings[i])) buildings[i]->upgrade();
                            money -= 300;
                            eco += 2;
                            comLevel++;
                            cout << "Commercial districts upgraded! Eco +2\n";
                        } else if (comChoice == 2 && money >= 700 && buildingCount < 100) {
                            buildings[buildingCount++] = new CommercialDistrict();
                            money -= 700;
                            eco += 5;
                            comCount++;
                            cout << "New Commercial District built! (+5 eco)\n";
                        } else if (comChoice == 3 && money >= 1500 && buildingCount < 100) {
                            buildings[buildingCount++] = new ShoppingMall();
                            money -= 1500;
                            eco += 3;
                            money += 300; // Shopping malls generate more income
                            mallCount++;
                            cout << "New Shopping Mall built! (+3 eco, +$300 income)\n";
                        }
                        break;
                    }
                    case 3: { // Industrial Properties
                        cout << "\n--- INDUSTRIAL PROPERTIES ---\n";
                        cout << "1. Build Industrial Zone (Cost: $800)\n";
                        cout << "2. Upgrade Industrial Zone (Cost: $400)\n";
                        cout << "0. Back\n";
                        cout << "Choose an option: ";
                        int indChoice;
                        cin >> indChoice;
                        cin.ignore();

                        if (indChoice == 1 && money >= 800 && buildingCount < 100) {
                            buildings[buildingCount++] = new IndustrialZone();
                            money -= 800;
                            eco -= 2; // Industrial zones decrease eco
                            money += 150; // But generate income
                            indCount++;
                            cout << "New Industrial Zone built! (-2 eco, +$150 income)\n";
                        } else if (indChoice == 2 && money >= 400) {
                            for (int i = 0; i < buildingCount; ++i)
                                if (dynamic_cast<IndustrialZone*>(buildings[i])) buildings[i]->upgrade();
                            money -= 400;
                            indLevel++;
                            money += 100;
                            cout << "Industrial zones upgraded! +$100 income\n";
                        }
                        break;
                    }
                    case 4: { // Luxury Properties
                        cout << "\n--- LUXURY PROPERTIES ---\n";
                        cout << "1. Upgrade Luxury Residence (Cost: $500)\n";
                        cout << "2. Build New Luxury Residence (Cost: $1000)\n";
                        cout << "0. Back\n";
                        cout << "Choose an option: ";
                        int luxChoice;
                        cin >> luxChoice;
                        cin.ignore();

                        if (luxChoice == 1 && money >= 500) {
                            for (int i = 0; i < buildingCount; ++i)
                                if (dynamic_cast<LuxuryResidence*>(buildings[i])) buildings[i]->upgrade();
                            money -= 500;
                            luxLevel++;
                            money += 300;
                            cout << "Luxury residences upgraded! +$300 income\n";
                        } else if (luxChoice == 2 && money >= 1000 && buildingCount < 100) {
                            buildings[buildingCount++] = new LuxuryResidence();
                            money -= 1000;
                            population += 25;
                            money += 200;
                            luxCount++;
                            cout << "New Luxury Residence built! (+25 population, +$200 income)\n";
                        }
                        break;
                    }
                    case 5: { // Property Portfolio
                        cout << "\n--- PROPERTY PORTFOLIO ---\n";
                        cout << "Residential Zones: " << resCount << " (Level " << resLevel << ")\n";
                        cout << "Commercial Districts: " << comCount << " (Level " << comLevel << ")\n";
                        cout << "Industrial Zones: " << indCount << " (Level " << indLevel << ")\n";
                        cout << "Luxury Residences: " << luxCount << " (Level " << luxLevel << ")\n";
                        cout << "Shopping Malls: " << mallCount << " (Level " << mallLevel << ")\n";
                        cout << "Total Properties: " << buildingCount << "/100\n";
                        break;
                    }
                    case 6: { // Property Maintenance
                        cout << "\n--- PROPERTY MAINTENANCE ---\n";
                        int maintenanceCost = (resCount + comCount + indCount + luxCount + mallCount) * 50;
                        cout << "Maintenance cost: $" << maintenanceCost << "\n";
                        cout << "1. Perform Maintenance\n";
                        cout << "0. Back\n";
                        cout << "Choose an option: ";
                        int maintChoice;
                        cin >> maintChoice;
                        cin.ignore();

                        if (maintChoice == 1 && money >= maintenanceCost) {
                            money -= maintenanceCost;
                            eco += 1;
                            cout << "Maintenance completed! Eco +1\n";
                        } else if (maintChoice == 1) {
                            cout << "Not enough money for maintenance!\n";
                        }
                        break;
                    }
                }
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
                } else if (envChoice==4 ) {
                    eco+=2;
                    cout<<"you encourage people to turn off the light when not in use\n";
                    activityLog.add("turn off light when not in use");
                } else if (envChoice != 0) {
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
                } else if (tChoice != 0) {
                    throw ActionException("Invalid transport action!");
                }
            break;
            }
            case 4: { // Citizen Actions
                cout << "\n--- CITIZEN ACTIONS ---\n";
                cout << "1. Commute\n";
                cout << "2. Protest\n";
                cout << "3. Celebrate\n";
                cout << "4. Deport Illegal Residents\n";
                cout << "0. Back\n";
                cout << "Choose an option: ";
                int cChoice;
                cin >> cChoice;
                cin.ignore();
                if (cChoice >= 1 && cChoice <= 4) {
                    actions[cChoice - 1]->act(eco, money, population);
                    citizenLog.add(actions[cChoice - 1]->name());
                } else if (cChoice != 0) {
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







