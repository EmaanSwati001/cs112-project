#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <cstdlib>
#include <ctime>
#include <exception>

using namespace std;

// === Utility Functions ===
void clearScreen() { cout << "\033[2J\033[1;1H"; }
void drawBorder() { cout << "==========================================\n"; }

// === User Authentication System ===
class User {
public:
    string username;
    string password;
    
    bool login() {
        ifstream file("users.txt");
        string line, user, pass;
        while (getline(file, line)) {
            stringstream ss(line);
            ss >> user >> pass;
            if (user == username && pass == password) return true;
        }
        return false;
    }
    
    bool registerUser() {
        // Check if username exists
        ifstream checkFile("users.txt");
        string line, user;
        while (getline(checkFile, line)) {
            stringstream ss(line);
            ss >> user;
            if (user == username) return false;
        }
        
        // Create new user
        ofstream file("users.txt", ios::app);
        file << username << " " << password << endl;
        return true;
    }
    
    bool saveCityData(int money, int eco, int population, int day, int resLevel, int comLevel,
                     int resCount, int comCount, int parkCount, int forestCount, int powerPlantCount,
                     int numBikes, int numBuses, int numEVs, int pollution) {
        ofstream file(username + "_city.txt");
        if (!file) return false;
        
        file << money << " " << eco << " " << population << " " << day << " "
             << resLevel << " " << comLevel << " "
             << resCount << " " << comCount << " " << parkCount << " " << forestCount << " " << powerPlantCount << " "
             << numBikes << " " << numBuses << " " << numEVs << " " << pollution;
        
        return true;
    }
    
    bool loadCityData(int& money, int& eco, int& population, int& day, int& resLevel, int& comLevel,
                     int& resCount, int& comCount, int& parkCount, int& forestCount, int& powerPlantCount,
                     int& numBikes, int& numBuses, int& numEVs, int& pollution) {
        ifstream file(username + "_city.txt");
        if (!file) return false;
        
        file >> money >> eco >> population >> day
             >> resLevel >> comLevel
             >> resCount >> comCount >> parkCount >> forestCount >> powerPlantCount
             >> numBikes >> numBuses >> numEVs >> pollution;
        
        return true;
    }
    
    bool deleteCity() {
        return remove((username + "_city.txt").c_str()) == 0;
    }
};

// === City Activity Template & Classes ===
class ActionException : public exception {
private:
    string m_message;
public:
    ActionException(string message) : m_message(message) {}
    virtual const char* what() const throw() { return m_message.c_str(); }
};

class Activity {
public:
    virtual void perform(int& eco, int& money, int& population, int& pollution) = 0;
    virtual ~Activity() {}
};

class Recycling : public Activity {
public:
    void perform(int& eco, int& money, int& population, int& pollution) override {
        if (money < 200) {
            throw ActionException("Not enough money for recycling program!");
        }
        
        money -= 200;
        eco += 5;
        pollution -= 10;
        
        cout << "♻️ Recycling program implemented! Eco +5, Pollution -10\n";
    }
};

class TreePlanting : public Activity {
public:
    void perform(int& eco, int& money, int& population, int& pollution) override {
        if (money < 100) {
            throw ActionException("Not enough money for tree planting!");
        }
        
        money -= 100;
        eco += 3;
        pollution -= 5;
        
        cout << "🌱 Tree planting campaign successful! Eco +3, Pollution -5\n";
    }
};

class EnergyProduction : public Activity {
public:
    void perform(int& eco, int& money, int& population, int& pollution) override {
        if (money < 300) {
            throw ActionException("Not enough money for energy production upgrade!");
        }
        
        money -= 300;
        eco += 7;
        pollution -= 3;
        
        cout << "⚡ Energy production optimized! Eco +7, Pollution -3\n";
    }
};

class GreenProgram : public Activity {
public:
    void perform(int& eco, int& money, int& population, int& pollution) override {
        if (money < 300) {
            throw ActionException("Not enough money for green program!");
        }
        
        money -= 300;
        eco += 8;
        pollution -= 12;
        population += 30;
        
        cout << "🌿 Green program launched! Eco +8, Pollution -12, Population +30\n";
    }
};

class DisasterManagement : public Activity {
public:
    void perform(int& eco, int& money, int& population, int& pollution) override {
        // Handle natural disaster
        cout << "🌪️ Managing natural disaster response!\n";
        pollution += 30;
        money -= 500;
        eco -= 5;
        
        cout << "Disaster contained! Pollution +30, Money -500, Eco -5\n";
        cout << "But it could have been much worse without your intervention!\n";
    }
};

// === Transport Options ===
class Transport {
public:
    virtual void build(int& eco, int& money, int& population, int& pollution) = 0;
    virtual ~Transport() {}
};

class Bike : public Transport {
public:
    void build(int& eco, int& money, int& population, int& pollution) override {
        if (money < 200) {
            throw ActionException("Not enough money for bike lanes!");
        }
        
        money -= 200;
        eco += 3;
        pollution -= 5;
        
        cout << "🚲 Bike lanes built! Eco +3, Pollution -5\n";
    }
};

class Bus : public Transport {
public:
    void build(int& eco, int& money, int& population, int& pollution) override {
        if (money < 350) {
            throw ActionException("Not enough money for bus system!");
        }
        
        money -= 350;
        eco += 5;
        pollution -= 8;
        
        cout << "🚌 EV Bus network expanded! Eco +5, Pollution -8\n";
    }
};

class EV : public Transport {
public:
    void build(int& eco, int& money, int& population, int& pollution) override {
        if (money < 400) {
            throw ActionException("Not enough money for EV infrastructure!");
        }
        
        money -= 400;
        eco += 7;
        pollution -= 10;
        
        cout << "🔋 EV infrastructure improved! Eco +7, Pollution -10\n";
    }
};

class Subway : public Transport {
public:
    void build(int& eco, int& money, int& population, int& pollution) override {
        if (money < 1000) {
            throw ActionException("Not enough money for subway system!");
        }
        
        money -= 1000;
        eco += 10;
        pollution -= 15;
        population += 50;
        
        cout << "🚇 Subway system expanded! Eco +10, Pollution -15, Population +50\n";
    }
};

// === Citizen Actions ===
class CitizenAction {
public:
    virtual void execute(int& eco, int& money, int& population, int& pollution) = 0;
    virtual ~CitizenAction() {}
};

class Commute : public CitizenAction {
public:
    void execute(int& eco, int& money, int& population, int& pollution) override {
        cout << "👨‍💼 Citizens are using your transportation systems!\n";
        money += 50;
        cout << "Revenue from fares: +$50\n";
    }
};

class Protest : public CitizenAction {
public:
    void execute(int& eco, int& money, int& population, int& pollution) override {
        cout << "⚠️ Citizens are protesting against pollution!\n";
        if (pollution > 50) {
            population -= 20;
            cout << "20 citizens have left the city.\n";
        } else {
            cout << "But your good eco policies pacified them!\n";
        }
    }
};

class Celebrate : public CitizenAction {
public:
    void execute(int& eco, int& money, int& population, int& pollution) override {
        cout << "🎉 Citizens are celebrating your leadership!\n";
        population += 25;
        cout << "25 new people have moved to your city!\n";
    }
};

class BuildHospital : public CitizenAction {
public:
    void execute(int& eco, int& money, int& population, int& pollution) override {
        if (money < 800) {
            throw ActionException("Not enough money to build a hospital!");
        }
        
        money -= 800;
        population += 100;
        eco += 5;
        
        cout << "🏥 New hospital built! Population +100, Eco +5\n";
    }
};

class OpenSchool : public CitizenAction {
public:
    void execute(int& eco, int& money, int& population, int& pollution) override {
        if (money < 600) {
            throw ActionException("Not enough money to open a school!");
        }
        
        money -= 600;
        population += 80;
        eco += 10;
        
        cout << "🏫 New school opened! Population +80, Eco +10\n";
    }
};

class CreateJobs : public CitizenAction {
public:
    void execute(int& eco, int& money, int& population, int& pollution) override {
        if (money < 500) {
            throw ActionException("Not enough money to create jobs!");
        }
        
        money -= 500;
        population += 50;
        money += 200; // Return on investment
        
        cout << "💼 New jobs created! Population +50, Money +200 (ROI)\n";
    }
};

// === City Infrastructure ===
class CityObject {
public:
    virtual void upgrade() = 0;
    virtual ~CityObject() {}
};

class ResidentialZone : public CityObject {
private:
    int level = 1;
    
public:
    void upgrade() override {
        level++;
        cout << "Residential zone upgraded to level " << level << "!\n";
    }
};

class CommercialDistrict : public CityObject {
private:
    int level = 1;
    
public:
    void upgrade() override {
        level++;
        cout << "Commercial district upgraded to level " << level << "!\n";
    }
};

class Park : public CityObject {
public:
    void upgrade() override {
        cout << "Park beautified!\n";
    }
};

class Forest : public CityObject {
public:
    void upgrade() override {
        cout << "Forest expanded!\n";
    }
};

class PowerPlant : public CityObject {
private:
    bool isGreen;
    
public:
    PowerPlant(bool green) : isGreen(green) {}
    
    void upgrade() override {
        if (isGreen) {
            cout << "Green power plant capacity increased!\n";
        } else {
            cout << "Coal power plant efficiency improved!\n";
        }
    }
};

// === Activity Log System ===
template<typename T>
class Log {
private:
    T entries[50];
    int count = 0;
    
public:
    void add(T entry) {
        if (count < 50) {
            entries[count++] = entry;
        } else {
            // Shift all entries one position back
            for (int i = 0; i < 49; i++) {
                entries[i] = entries[i+1];
            }
            entries[49] = entry;
        }
    }
    
    void display() {
        cout << "--- Recent Activity Log ---\n";
        for (int i = max(0, count-10); i < count; i++) {
            cout << "- " << entries[i] << "\n";
        }
    }
};

// === UI Functions ===
void displayWelcomeScreen() {
    clearScreen();
    drawBorder();
    cout << "          SMART ECO CITY SIMULATION -           \n";
    cout << "                   WELCOME                      \n";
    drawBorder();
    cout << "0. Login 🔑 Existing City\n";
    cout << "1. Register 📝 New Mayor Account\n";
    cout << "2. Quick Play 🎮 Demo Mode (No Save)\n";
    cout << "3. Exit Game\n";
    drawBorder();
    cout << "Enter choice (0-3): ";
}

void displayMainMenu(const string& username, const string& cityName, int eco, int money, int population, int pollution, int day) {
    clearScreen();
    drawBorder();
    cout << "SMART ECO CITY - Mayor: " << username << " | Day: " << day << "\n";
    cout << "City: " << cityName << "\n";
    drawBorder();
    
    cout << "ECO RATING: " << eco << "% | ";
    cout << "TREASURY: $" << money << " | ";
    cout << "POPULATION: " << population << " | ";
    cout << "POLLUTION: " << pollution << "\n";
    
    drawBorder();
    cout << "1. 🏗️ BUILD & UPGRADE\n";
    cout << "2. 🌱 MANAGE ENVIRONMENT\n";
    cout << "3. 🚌 CONTROL TRANSPORT\n";
    cout << "4. 👥 CITIZEN ACTIONS\n";
    cout << "5. 🏢 MAYOR'S OFFICE\n";
    cout << "6. 🏆 VIEW ACHIEVEMENTS\n";
    cout << "7. 🤝 SOCIAL FEATURES\n";
    cout << "0. 🔒 LOGOUT\n";
    drawBorder();
    cout << "Choose an option: ";
}

// === Social Features Class ===
class SocialFeatures {
public:
    static void displayGlobalRankings() {
        clearScreen();
        drawBorder();
        cout << "           GLOBAL CITY RANKINGS             \n";
        drawBorder();
        
        cout << "🌎 Top Eco-Friendly Cities:\n\n";
        
        // Simulated global rankings
        cout << "1. 🥇 Solaria (Eco: 95%, Pop: 7.2M)\n";
        cout << "2. 🥈 New Arboria (Eco: 92%, Pop: 5.1M)\n";
        cout << "3. 🥉 Greenfield (Eco: 89%, Pop: 3.8M)\n";
        cout << "4. ⭐ Eco Haven (Eco: 87%, Pop: 2.5M)\n";
        cout << "5. ⭐ Sustainia (Eco: 85%, Pop: 4.2M)\n";
        cout << "\n📊 Your city is not yet ranked. Reach 80% Eco rating to qualify.\n";
        
        drawBorder();
        cout << "Press Enter to return to main menu...\n";
        cin.ignore();
        cin.get();
    }
    
    static void hostClimateSummit(int& eco, int& money, int& population) {
        clearScreen();
        drawBorder();
        cout << "              CLIMATE SUMMIT                \n";
        drawBorder();
        
        if (money < 5000) {
            cout << "❌ Not enough funds to host a Climate Summit.\n";
            cout << "   Required: $5,000    Available: $" << money << "\n";
        } else if (eco < 40) {
            cout << "❌ Your city's eco rating is too low to host a Climate Summit.\n";
            cout << "   Required: 40% Eco    Current: " << eco << "%\n";
        } else {
            money -= 5000;
            eco += 15;
            population += 100;
            
            cout << "🌡️ Global Climate Summit successfully hosted!\n\n";
            cout << "Outcomes:\n";
            cout << "- International recognition for your city\n";
            cout << "- Eco rating increased by 15%\n";
            cout << "- Population increased by 100 new eco-conscious citizens\n";
            cout << "- Cost: $5,000\n";
        }
        
        drawBorder();
        cout << "Press Enter to return to main menu...\n";
        cin.ignore();
        cin.get();
    }
    
    static void displayEcoTips() {
        clearScreen();
        drawBorder();
        cout << "               ECO TIPS LIBRARY             \n";
        drawBorder();
        
        // Display eco tips
        cout << "💡 Balance residential and commercial zones for optimal growth\n\n";
        cout << "💡 Bike lanes are the most cost-effective transport solution\n\n";
        cout << "💡 Plant forests early to combat pollution growth\n\n";
        cout << "💡 Green power plants cost more but pay off in the long run\n\n";
        cout << "💡 Schools improve eco rating and attract new citizens\n\n";
        cout << "💡 Invest in recycling centers after reaching 500 population\n\n";
        cout << "💡 Host climate summits to gain international prestige\n\n";
        cout << "💡 Disaster readiness saves money in the long term\n\n";
        cout << "💡 Parks provide happiness and increase eco rating\n\n";
        cout << "💡 Balance short-term costs with long-term sustainability\n\n";
        
        drawBorder();
        cout << "Press Enter to return to main menu...\n";
        cin.ignore();
        cin.get();
    }
};

// === Achievements System ===
class AchievementSystem {
public:
    static void displayAchievements(int eco, int money, int population, int day) {
        clearScreen();
        drawBorder();
        cout << "              ACHIEVEMENTS                  \n";
        drawBorder();
        
        // Check achievements one by one
        int total = 6;
        int earned = 0;
        
        // Green Pioneer achievement
        bool greenPioneer = eco > 30;
        cout << (greenPioneer ? "✅ " : "❌ ") << "🏆 Green Pioneer (Eco rating > 30)" << "\n";
        if (greenPioneer) earned++;
        
        // Eco Visionary achievement
        bool ecoVisionary = eco > 60;
        cout << (ecoVisionary ? "✅ " : "❌ ") << "🏆 Eco Visionary (Eco rating > 60)" << "\n";
        if (ecoVisionary) earned++;
        
        // Millionaire Mayor achievement
        bool millionaireMayor = money > 1000000;
        cout << (millionaireMayor ? "✅ " : "❌ ") << "🏆 Millionaire Mayor (Money > $1,000,000)" << "\n";
        if (millionaireMayor) earned++;
        
        // Thriving Metropolis achievement
        bool thrivingMetropolis = population > 1000;
        cout << (thrivingMetropolis ? "✅ " : "❌ ") << "🏆 Thriving Metropolis (Population > 1,000)" << "\n";
        if (thrivingMetropolis) earned++;
        
        // Century City achievement
        bool centuryCity = day > 100;
        cout << (centuryCity ? "✅ " : "❌ ") << "🏆 Century City (Run for 100+ days)" << "\n";
        if (centuryCity) earned++;
        
        // Sustainable Utopia achievement
        bool sustainableUtopia = eco > 80 && population > 5000;
        cout << (sustainableUtopia ? "✅ " : "❌ ") << "🏆 Sustainable Utopia (Eco > 80, Pop > 5,000)" << "\n";
        if (sustainableUtopia) earned++;
        
        cout << "\nYou've earned " << earned << " out of " << total << " achievements!\n";
        
        drawBorder();
        cout << "Press Enter to return to main menu...\n";
        cin.ignore();
        cin.get();
    }
};

// === Mayor's Office Functions ===
bool saveGame(User& user, int money, int eco, int population, int day, int resLevel, int comLevel,
              int resCount, int comCount, int parkCount, int forestCount, int powerPlantCount,
              int numBikes, int numBuses, int numEVs, int pollution) {
    
                return user.saveCityData(money, eco, population, day, resLevel, comLevel, resCount, comCount,
                     parkCount, forestCount, powerPlantCount, numBikes, numBuses, numEVs, pollution);
}

bool loadGame(User& user, int& money, int& eco, int& population, int& day, int& resLevel, int& comLevel,
             int& resCount, int& comCount, int& parkCount, int& forestCount, int& powerPlantCount,
             int& numBikes, int& numBuses, int& numEVs, int& pollution) {
    
    return user.loadCityData(money, eco, population, day, resLevel, comLevel, resCount, comCount,
                           parkCount, forestCount, powerPlantCount, numBikes, numBuses, numEVs, pollution);
}

void deleteGame(User& user) {
    if (user.deleteCity()) {
        cout << "City deleted successfully!\n";
    } else {
        cout << "Failed to delete city.\n";
    }
}

// === Simulation Core (runSimulation) ===
void runSimulation(string username) {
    // Initialize data structures to hold buildings and infrastructure
    CityObject* buildings[100]; // Array of buildings instead of vector
    int buildingCount = 0;
    
    // Initialize city statistics
    int money = 1000;
    int eco = 10;
    int population = 100;
    int day = 1;
    int resLevel = 1, comLevel = 1, resCount = 1, comCount = 1;
    int parkCount = 0, forestCount = 0, powerPlantCount = 0;
    int numBikes = 0, numBuses = 0, numEVs = 0;
    int pollution = 10;
    
    // User object for data persistence
    User user;
    user.username = username;
    
    // Try to load city data
    if (loadGame(user, money, eco, population, day, resLevel, comLevel, resCount, comCount,
                parkCount, forestCount, powerPlantCount, numBikes, numBuses, numEVs, pollution)) {
        
        // Load buildings based on saved data
        int currentBuilding = 0;
        
        for (int i = 0; i < resCount; ++i) {
            ResidentialZone* rz = new ResidentialZone();
            for (int l = 1; l < resLevel; ++l) rz->upgrade();
            buildings[currentBuilding++] = rz;
        }
        
        for (int i = 0; i < comCount; ++i) {
            CommercialDistrict* cd = new CommercialDistrict();
            for (int l = 1; l < comLevel; ++l) cd->upgrade();
            buildings[currentBuilding++] = cd;
        }
        
        for (int i = 0; i < parkCount; ++i) {
            Park* pk = new Park();
            buildings[currentBuilding++] = pk;
        }
        
        for (int i = 0; i < forestCount; ++i) {
            Forest* fr = new Forest();
            buildings[currentBuilding++] = fr;
        }
        
        for (int i = 0; i < powerPlantCount; ++i) {
            PowerPlant* pp = new PowerPlant(true); // Assume all saved power plants are green
            buildings[currentBuilding++] = pp;
        }
        
        buildingCount = currentBuilding;
        
        cout << "City data loaded successfully!\n";
    } else {
        // Default city
        buildings[0] = new ResidentialZone();
        buildings[1] = new CommercialDistrict();
        buildingCount = 2;
        cout << "Starting a new city...\n";
    }
    
    // Initialize activities, transport options, and citizen actions
    Activity* activities[5];
    activities[0] = new Recycling();
    activities[1] = new TreePlanting();
    activities[2] = new EnergyProduction();
    activities[3] = new GreenProgram();
    activities[4] = new DisasterManagement();
    
    Transport* transports[4];
    transports[0] = new Bike();
    transports[1] = new Bus();
    transports[2] = new EV();
    transports[3] = new Subway();
    
    CitizenAction* citizenActions[6];
    citizenActions[0] = new Commute();
    citizenActions[1] = new Protest();
    citizenActions[2] = new Celebrate();
    citizenActions[3] = new BuildHospital();
    citizenActions[4] = new OpenSchool();
    citizenActions[5] = new CreateJobs();
    
    // Activity logs
    Log<string> activityLog, transportLog, citizenLog;
    
    int choice;
    bool continueSimulation = true;
    
    while (continueSimulation) {
        displayMainMenu(username, "GreenHaven", eco, money, population, pollution, day);
        cin >> choice;
        cin.ignore();
        
        try {
            switch (choice) {
                case 0: // Logout
                    continueSimulation = false;
                    cout << "Logging out...\n";
                    break;
                    
                case 1: { // BUILD & UPGRADE
                    clearScreen();
                    drawBorder();
                    cout << "--- BUILD & UPGRADE ---\n";
                    drawBorder();
                    
                    cout << "Your resources: Money: $" << money << ", Eco: " << eco << "%, Population: " << population << "\n\n";
                    cout << "1. 🏢 Upgrade Residential Zone (Cost: $200)\n";
                    cout << "2. 🏬 Upgrade Commercial District (Cost: $300)\n";
                    cout << "3. 🏢 Build New Residential Zone (Cost: $500)\n";
                    cout << "4. 🏬 Build New Commercial District (Cost: $700)\n";
                    cout << "5. 🌳 Build New Park (Cost: $400)\n";
                    cout << "6. 🌲 Plant Forest (Cost: $600)\n";
                    cout << "7. ⚡ Build Power Plant (Cost: $1000)\n";
                    cout << "0. Back\n";
                    drawBorder();
                    cout << "Choose an option: ";
                    
                    int buildChoice;
                    cin >> buildChoice;
                    cin.ignore();
                    
                    if (buildChoice == 1 && money >= 200) {
                        // Upgrade all residential zones
                        bool upgraded = false;
                        for (int i = 0; i < buildingCount; ++i) {
                            if (dynamic_cast<ResidentialZone*>(buildings[i])) {
                                buildings[i]->upgrade();
                                upgraded = true;
                            }
                        }
                        
                        if (upgraded) {
                            money -= 200;
                            population += 50;
                            resLevel++;
                            cout << "Population increased! (+50)\n";
                            activityLog.add("Upgraded residential zones to level " + to_string(resLevel));
                        } else {
                            cout << "No residential zones to upgrade!\n";
                        }
                    } 
                    else if (buildChoice == 2 && money >= 300) {
                        // Upgrade all commercial districts
                        bool upgraded = false;
                        for (int i = 0; i < buildingCount; ++i) {
                            if (dynamic_cast<CommercialDistrict*>(buildings[i])) {
                                buildings[i]->upgrade();
                                upgraded = true;
                            }
                        }
                        
                        if (upgraded) {
                            money -= 300;
                            eco += 2;
                            comLevel++;
                            cout << "Eco improved! (+2)\n";
                            activityLog.add("Upgraded commercial districts to level " + to_string(comLevel));
                        } else {
                            cout << "No commercial districts to upgrade!\n";
                        }
                    } 
                    else if (buildChoice == 3 && money >= 500 && buildingCount < 100) {
                        buildings[buildingCount++] = new ResidentialZone();
                        money -= 500;
                        population += 100;
                        resCount++;
                        cout << "New Residential Zone built! (+100 population)\n";
                        activityLog.add("Built new residential zone");
                    } 
                    else if (buildChoice == 4 && money >= 700 && buildingCount < 100) {
                        buildings[buildingCount++] = new CommercialDistrict();
                        money -= 700;
                        eco += 5;
                        pollution += 5;
                        comCount++;
                        cout << "New Commercial District built! (+5 eco, +5 pollution)\n";
                        activityLog.add("Built new commercial district");
                    }
                    else if (buildChoice == 5 && money >= 400 && buildingCount < 100) {
                        buildings[buildingCount++] = new Park();
                        money -= 400;
                        eco += 8;
                        pollution -= 3;
                        parkCount++;
                        cout << "New Park built! (+8 eco, -3 pollution)\n";
                        activityLog.add("Built new park");
                    }
                    else if (buildChoice == 6 && money >= 600 && buildingCount < 100) {
                        buildings[buildingCount++] = new Forest();
                        money -= 600;
                        eco += 12;
                        pollution -= 10;
                        forestCount++;
                        cout << "New Forest planted! (+12 eco, -10 pollution)\n";
                        activityLog.add("Planted new forest");
                    }
                    else if (buildChoice == 7 && money >= 1000) {
                        clearScreen();
                        drawBorder();
                        cout << "--- POWER PLANT OPTIONS ---\n";
                        drawBorder();
                        cout << "1. 🏭 Coal Power Plant (Cost: $1000, +$300/day, -5 eco/day)\n";
                        cout << "2. ⚡ Green Power Plant (Cost: $1500, +$150/day, +3 eco/day)\n";
                        cout << "0. Back\n";
                        drawBorder();
                        cout << "Choose an option: ";
                        
                        int powerChoice;
                        cin >> powerChoice;
                        cin.ignore();
                        
                        if (powerChoice == 1 && money >= 1000 && buildingCount < 100) {
                            buildings[buildingCount++] = new PowerPlant(false);
                            money -= 1000;
                            powerPlantCount++;
                            pollution += 20;
                            cout << "Coal Power Plant built! (+20 pollution)\n";
                            activityLog.add("Built coal power plant");
                        }
                        else if (powerChoice == 2 && money >= 1500 && buildingCount < 100) {
                            buildings[buildingCount++] = new PowerPlant(true);
                            money -= 1500;
                            powerPlantCount++;
                            eco += 5;
                            cout << "Green Power Plant built! (+5 eco)\n";
                            activityLog.add("Built green power plant");
                        }
                        else if (powerChoice != 0) {
                            cout << "Not enough money or invalid choice!\n";
                        }
                    }
                    else if (buildChoice == 0) {
                        break;
                    } 
                    else {
                        cout << "Not enough money or invalid choice!\n";
                    }
                    
                    // Random event
                    int event = rand() % 5;
                    if (event == 0) {
                        cout << "📊 Bonus! You received a government grant of $200!\n";
                        money += 200;
                    } else if (event == 1) {
                        cout << "⚠️ Power outage! Eco drops by 2.\n";
                        eco -= 2;
                    } else if (event == 2) {
                        cout << "👥 Citizens are happy! Population increases by 20.\n";
                        population += 20;
                    }
                    
                    day++;
                    
                    cout << "Press Enter to continue...";
                    cin.get();
                    break;
                }
                
                case 2: { // MANAGE ENVIRONMENT
                    clearScreen();
                    drawBorder();
                    cout << "--- MANAGE ENVIRONMENT ---\n";
                    drawBorder();
                    
                    cout << "Your resources: Money: $" << money << ", Eco: " << eco << "%, Population: " << population << "\n\n";
                    cout << "1. 🌱 Launch Green Program (Cost: $300)\n";
                    cout << "2. ♻️ Build Recycling Center (Cost: $400)\n";
                    cout << "3. 🌲 Plant Trees Campaign (Cost: $100)\n";
                    cout << "4. ⚡ Promote Clean Energy (Cost: $200)\n";
                    cout << "5. 🌪️ Handle Natural Disaster (if any)\n";
                    cout << "0. Back\n";
                    drawBorder();
                    cout << "Choose an option: ";
                    
                    int envChoice;
                    cin >> envChoice;
                    cin.ignore();
                    
                    // Check if a disaster is happening
                    bool disasterHappening = (rand() % 20 == 0); // 5% chance of disaster
                    
                    if (envChoice == 1 && money >= 300) {
                        // Launch green program
                        GreenProgram program;
                        program.perform(eco, money, population, pollution);
                        activityLog.add("Launched green program");
                    }
                    else if (envChoice == 2 && money >= 400) {
                        // Build recycling center
                        money -= 400;
                        eco += 10;
                        pollution -= 15;
                        cout << "♻️ Recycling center built! Eco +10, Pollution -15\n";
                        activityLog.add("Built recycling center");
                    }
                    else if (envChoice == 3 && money >= 100) {
                        // Plant trees
                        TreePlanting treeCampaign;
                        treeCampaign.perform(eco, money, population, pollution);
                        activityLog.add("Organized tree planting campaign");
                    }
                    else if (envChoice == 4 && money >= 200) {
                        // Promote clean energy
                        money -= 200;
                        eco += 5;
                        pollution -= 8;
                        cout << "⚡ Clean energy promotion campaign successful! Eco +5, Pollution -8\n";
                        activityLog.add("Promoted clean energy");
                    }
                    else if (envChoice == 5) {
                        if (disasterHappening) {
                            DisasterManagement disaster;
                            disaster.perform(eco, money, population, pollution);
                            activityLog.add("Responded to natural disaster");
                        } else {
                            cout << "No disasters currently happening. Stay prepared!\n";
                        }
                    }
                    else if (envChoice == 0) {
                        break;
                    }
                    else {
                        cout << "Not enough money or invalid choice!\n";
                    }
                    
                    // Check if disaster happens and player didn't address it
                    if (disasterHappening && envChoice != 5) {
                        cout << "🌪️ A natural disaster struck and you weren't prepared!\n";
                        eco -= 10;
                        money -= 800;
                        population -= 30;
                        pollution += 50;
                        cout << "Eco -10, Money -800, Population -30, Pollution +50\n";
                        activityLog.add("Failed to respond to natural disaster");
                    }
                    
                    day++;
                    
                    cout << "Press Enter to continue...";
                    cin.get();
                    break;
                }
                
                case 3: { // CONTROL TRANSPORT
                    clearScreen();
                    drawBorder();
                    cout << "--- CONTROL TRANSPORT ---\n";
                    drawBorder();
                    
                    cout << "Your resources: Money: $" << money << ", Eco: " << eco << "%, Population: " << population << "\n\n";
                    cout << "1. 🚲 Build/Upgrade Bike Lanes (Cost: $200)\n";
                    cout << "2. 🚌 Expand EV Bus Network (Cost: $350)\n";
                    cout << "3. 🚇 Develop Subway System (Cost: $1000)\n";
                    cout << "4. 🔋 Install EV Charging Stations (Cost: $400)\n";
                    cout << "5. 🚦 Improve Traffic Management (Cost: $150)\n";
                    cout << "0. Back\n";
                    drawBorder();
                    cout << "Choose an option: ";
                    
                    int transChoice;
                    cin >> transChoice;
                    cin.ignore();
                    
                    if (transChoice == 1) {
                        // Build/Upgrade bike lanes
                        if (numBikes == 0 && money >= 200) {
                            money -= 200;
                            numBikes = 1;
                            pollution -= 5;
                            eco += 3;
                            cout << "🚲 Bike lanes built! Pollution -5, Eco +3\n";
                            activityLog.add("Built bike lanes");
                        } else if (numBikes > 0 && money >= 200) {
                            money -= 200;
                            numBikes++;
                            pollution -= 3;
                            eco += 2;
                            cout << "🚲 Bike lanes expanded! Pollution -3, Eco +2\n";
                            activityLog.add("Expanded bike lanes");
                        } else {
                            cout << "Not enough money!\n";
                        }
                    }
                    else if (transChoice == 2) {
                        // Expand EV bus network
                        if (numBuses == 0 && money >= 350) {
                            money -= 350;
                            numBuses = 1;
                            pollution -= 8;
                            eco += 5;
                            cout << "🚌 EV Bus network established! Pollution -8, Eco +5\n";
                            activityLog.add("Established EV bus network");
                        } else if (numBuses > 0 && money >= 350) {
                            money -= 350;
                            numBuses++;
                            pollution -= 5;
                            eco += 3;
                            cout << "🚌 EV Bus network expanded! Pollution -5, Eco +3\n";
                            activityLog.add("Expanded EV bus network");
                        } else {
                            cout << "Not enough money!\n";
                        }
                    }
                    else if (transChoice == 3) {
                        // Develop subway system
                        if (money >= 1000) {
                            money -= 1000;
                            pollution -= 15;
                            eco += 10;
                            population += 50;
                            cout << "🚇 Subway system developed! Pollution -15, Eco +10, Population +50\n";
                            activityLog.add("Developed subway system");
                        } else {
                            cout << "Not enough money!\n";
                        }
                    }
                    else if (transChoice == 4) {
                        // Install EV charging stations
                        if (numEVs == 0 && money >= 400) {
                            money -= 400;
                            numEVs = 1;
                            pollution -= 10;
                            eco += 7;
                            cout << "🔋 EV charging stations installed! Pollution -10, Eco +7\n";
                            activityLog.add("Installed EV charging stations");
                        } else if (numEVs > 0 && money >= 400) {
                            money -= 400;
                            numEVs++;
                            pollution -= 7;
                            eco += 5;
                            cout << "🔋 Additional EV charging stations installed! Pollution -7, Eco +5\n";
                            activityLog.add("Expanded EV charging network");
                        } else {
                            cout << "Not enough money!\n";
                        }
                    }
                    else if (transChoice == 5 && money >= 150) {
                        // Improve traffic management
                        money -= 150;
                        pollution -= 5;
                        eco += 2;
                        cout << "🚦 Traffic management improved! Pollution -5, Eco +2\n";
                        activityLog.add("Improved traffic management");
                    }
                    else if (transChoice == 0) {
                        break;
                    }
                    else {
                        cout << "Not enough money or invalid choice!\n";
                    }
                    
                    day++;
                    
                    cout << "Press Enter to continue...";
                    cin.get();
                    break;
                }
                
                case 4: { // CITIZEN ACTIONS
                    clearScreen();
                    drawBorder();
                    cout << "--- CITIZEN ACTIONS ---\n";
                    drawBorder();
                    
                    cout << "Your resources: Money: $" << money << ", Eco: " << eco << "%, Population: " << population << "\n\n";
                    cout << "1. 🏥 Build Hospital (Cost: $800)\n";
                    cout << "2. 🏫 Open School (Cost: $600)\n";
                    cout << "3. 💼 Create Jobs (Cost: $500)\n";
                    cout << "4. 🎭 Host Public Event (Cost: $300)\n";
                    cout << "5. 📣 Address Citizens\n";
                    cout << "0. Back\n";
                    drawBorder();
                    cout << "Choose an option: ";
                    
                    int citizenChoice;
                    cin >> citizenChoice;
                    cin.ignore();
                    
                    if (citizenChoice == 1 && money >= 800) {
                        // Build hospital
                        citizenActions[3]->execute(eco, money, population, pollution);
                        citizenLog.add("Built new hospital");
                    }
                    else if (citizenChoice == 2 && money >= 600) {
                        // Open school
                        citizenActions[4]->execute(eco, money, population, pollution);
                        citizenLog.add("Opened new school");
                    }
                    else if (citizenChoice == 3 && money >= 500) {
                        // Create jobs
                        citizenActions[5]->execute(eco, money, population, pollution);
                        citizenLog.add("Created new jobs");
                    }
                    else if (citizenChoice == 4 && money >= 300) {
                        // Host public event
                        money -= 300;
                        population += 30;
                        eco += 3;
                        cout << "🎭 Public event hosted! Population +30, Eco +3\n";
                        citizenLog.add("Hosted public event");
                    }
                    else if (citizenChoice == 5) {
                        // Address citizens
                        clearScreen();
                        drawBorder();
                        cout << "--- ADDRESS CITIZENS ---\n";
                        drawBorder();
                        
                        cout << "1. 📈 Focus on Economic Growth\n";
                        cout << "2. 🌿 Focus on Environmental Protection\n";
                        cout << "3. 👪 Focus on Population Growth\n";
                        cout << "0. Back\n";
                        drawBorder();
                        cout << "Choose your address focus: ";
                        
                        int addressChoice;
                        cin >> addressChoice;
                        cin.ignore();
                        
                        if (addressChoice == 1) {
                            money += 100;
                            cout << "Citizens are motivated to work harder! Money +100\n";
                            citizenLog.add("Addressed citizens about economic growth");
                        }
                        else if (addressChoice == 2) {
                            eco += 5;
                            pollution -= 5;
                            cout << "Citizens are inspired to protect the environment! Eco +5, Pollution -5\n";
                            citizenLog.add("Addressed citizens about environmental protection");
                        }
                        else if (addressChoice == 3) {
                            population += 20;
                            cout << "Citizens are encouraged to grow families! Population +20\n";
                            citizenLog.add("Addressed citizens about population growth");
                        }
                    }
                    else if (citizenChoice == 0) {
                        break;
                    }
                    else {
                        cout << "Not enough money or invalid choice!\n";
                    }
                    
                    // Random citizen action
                    int randomAction = rand() % 3;
                    cout << "\nCitizen action occurred:\n";
                    citizenActions[randomAction]->execute(eco, money, population, pollution);
                    
                    day++;
                    
                    cout << "Press Enter to continue...";
                    cin.get();
                    break;
                }
                
                case 5: { // MAYOR'S OFFICE
                    clearScreen();
                    drawBorder();
                    cout << "--- MAYOR'S OFFICE ---\n";
                    drawBorder();
                    
                    cout << "1. 📋 View Activity Log\n";
                    cout << "2. 💾 Save/Load City\n";
                    cout << "3. ⚙️ Account Settings\n";
                    cout << "4. 🗑️ Delete City\n";
                    cout << "0. Back\n";
                    drawBorder();
                    cout << "Choose an option: ";
                    
                    int mayorChoice;
                    cin >> mayorChoice;
                    cin.ignore();
                    
                    if (mayorChoice == 1) {
                        // View activity log
                        clearScreen();
                        drawBorder();
                        cout << "--- ACTIVITY LOG ---\n";
                        drawBorder();
                        
                        activityLog.display();
                        
                        drawBorder();
                        cout << "--- TRANSPORT LOG ---\n";
                        drawBorder();
                        
                        transportLog.display();
                        
                        drawBorder();
                        cout << "--- CITIZEN LOG ---\n";
                        drawBorder();
                        
                        citizenLog.display();
                    }
                    else if (mayorChoice == 2) {
                        // Save/Load City
                        clearScreen();
                        drawBorder();
                        cout << "--- SAVE/LOAD CITY ---\n";
                        drawBorder();
                        
                        cout << "1. Save current city\n";
                        cout << "2. Load saved city\n";
                        cout << "0. Back\n";
                        drawBorder();
                        cout << "Choose an option: ";
                        
                        int saveLoadChoice;
                        cin >> saveLoadChoice;
                        cin.ignore();
                        
                        if (saveLoadChoice == 1) {
                            if (saveGame(user, money, eco, population, day, resLevel, comLevel,
                                        resCount, comCount, parkCount, forestCount, powerPlantCount,
                                        numBikes, numBuses, numEVs, pollution)) {
                                cout << "City saved successfully!\n";
                            } else {
                                cout << "Failed to save city.\n";
                            }
                        }
                        else if (saveLoadChoice == 2) {
                            cout << "Loading will overwrite current city. Continue? (y/n): ";
                            char confirm;
                            cin >> confirm;
                            cin.ignore();
                            
                            if (confirm == 'y' || confirm == 'Y') {
                                if (loadGame(user, money, eco, population, day, resLevel, comLevel, resCount, comCount,
                                            parkCount, forestCount, powerPlantCount, numBikes, numBuses, numEVs, pollution)) {
                                    
                                    // Clean up existing buildings
                                    for (int i = 0; i < buildingCount; ++i) {
                                        delete buildings[i];
                                        buildings[i] = nullptr;
                                    }
                                    buildingCount = 0;
                                    
                                    // Recreate buildings based on loaded data
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
                                    
                                    for (int i = 0; i < parkCount; ++i) {
                                        buildings[buildingCount++] = new Park();
                                    }
                                    
                                    for (int i = 0; i < forestCount; ++i) {
                                        buildings[buildingCount++] = new Forest();
                                    }
                                    
                                    for (int i = 0; i < powerPlantCount; ++i) {
                                        buildings[buildingCount++] = new PowerPlant(true);
                                    }
                                    
                                    cout << "City loaded successfully!\n";
                                } else {
                                    cout << "No saved city found!\n";
                                }
                            }
                        }
                    }
                    else if (mayorChoice == 3) {
                        // Account Settings
                        clearScreen();
                        drawBorder();
                        cout << "--- ACCOUNT SETTINGS ---\n";
                        drawBorder();
                        
                        cout << "Current username: " << username << "\n\n";
                        cout << "1. Change password\n";
                        cout << "0. Back\n";
                        drawBorder();
                        cout << "Choose an option: ";
                        
                        int accountChoice;
                        cin >> accountChoice;
                        cin.ignore();
                        
                        if (accountChoice == 1) {
                            string oldPass, newPass;
                            cout << "Enter current password: ";
                            getline(cin, oldPass);
                            
                            user.password = oldPass;
                            if (user.login()) {
                                cout << "Enter new password: ";
                                getline(cin, newPass);
                                
                                user.password = newPass;
                                if (user.registerUser()) {
                                    cout << "Password changed successfully!\n";
                                } else {
                                    cout << "Failed to change password.\n";
                                }
                            } else {
                                cout << "Incorrect password!\n";
                            }
                        }
                    }
                    else if (mayorChoice == 4) {
                        // Delete City
                        cout << "Are you sure you want to delete your city? All progress will be lost. (y/n): ";
                        char confirm;
                        cin >> confirm;
                        cin.ignore();
                        
                        if (confirm == 'y' || confirm == 'Y') {
                            deleteGame(user);
                            
                            // Reset to default values
                            money = 1000;
                            eco = 10;
                            population = 100;
                            day = 1;
                            resLevel = comLevel = 1;
                            resCount = comCount = 1;
                            parkCount = forestCount = powerPlantCount = 0;
                            numBikes = numBuses = numEVs = 0;
                            pollution = 10;
                            
                            // Clean up buildings
                            for (int i = 0; i < buildingCount; ++i) {
                                delete buildings[i];
                                buildings[i] = nullptr;
                            }
                            buildingCount = 0;
                            
                            // Create default buildings
                            buildings[0] = new ResidentialZone();
                            buildings[1] = new CommercialDistrict();
                            buildingCount = 2;
                            
                            cout << "City has been reset to default.\n";
                        }
                    }
                    
                    cout << "Press Enter to continue...";
                    cin.get();
                    break;
                }
                
                case 6: { // VIEW ACHIEVEMENTS
                    AchievementSystem::displayAchievements(eco, money, population, day);
                    break;
                }
                
                case 7: { // SOCIAL FEATURES
                    clearScreen();
                    drawBorder();
                    cout << "--- SOCIAL FEATURES ---\n";
                    drawBorder();
                    
                    cout << "1. 🌎 Global City Rankings\n";
                    cout << "2. 🌡️ Host Climate Summit\n";
                    cout << "3. 💡 View Eco Tips Library\n";
                    cout << "0. Back\n";
                    drawBorder();
                    cout << "Choose an option: ";
                    
                    int socialChoice;
                    cin >> socialChoice;
                    cin.ignore();
                    
                    if (socialChoice == 1) {
                        SocialFeatures::displayGlobalRankings();
                    }
                    else if (socialChoice == 2) {
                        SocialFeatures::hostClimateSummit(eco, money, population);
                    }
                    else if (socialChoice == 3) {
                        SocialFeatures::displayEcoTips();
                    }
                    break;
                }
                
                default:
                    cout << "Invalid option! Please try again.\n";
                    break;
            }
        } catch (const ActionException& e) {
            cout << "Error: " << e.what() << endl;
            cout << "Press Enter to continue...";
            cin.ignore();
            cin.get();
        } catch (const exception& e) {
            cout << "An error occurred: " << e.what() << endl;
            cout << "Press Enter to continue...";
            cin.ignore();
            cin.get();
        }
        
        // Calculate daily changes
        population += (resCount * resLevel * 2);
        money += (comCount * comLevel * 20);
        pollution += (comCount * 2) - (forestCount * 3) - (parkCount * 1);
        
        // Basic balance adjustments
        if (pollution < 0) pollution = 0;
        if (pollution > 1000) pollution = 1000;
        if (eco < 0) eco = 0;
        if (eco > 100) eco = 100;
    }
    
    // Clean up memory before exiting
    for (int i = 0; i < buildingCount; ++i) {
        delete buildings[i];
    }
    
    for (int i = 0; i < 5; ++i) {
        delete activities[i];
    }
    
    for (int i = 0; i < 4; ++i) {
        delete transports[i];
    }
    
    for (int i = 0; i < 6; ++i) {
        delete citizenActions[i];
    }
}

int main() {
    // Seed the random number generator
    srand(static_cast<unsigned int>(time(nullptr)));
    
    // Variables for login/registration
    string username, password;
    User user;
    bool loggedIn = false;
    int choice;
    
    // Main game loop
    bool running = true;
    while (running) {
        if (!loggedIn) {
            displayWelcomeScreen();
            cin >> choice;
            cin.ignore();
            
            switch (choice) {
                case 0: { // Login
                    cout << "Enter username: ";
                    getline(cin, username);
                    cout << "Enter password: ";
                    getline(cin, password);
                    
                    user.username = username;
                    user.password = password;
                    
                    if (user.login()) {
                        cout << "Login successful!\n";
                        loggedIn = true;
                        runSimulation(username);
                        loggedIn = false; // After returning from simulation
                    } else {
                        cout << "Login failed. Incorrect username or password.\n";
                        cout << "Press Enter to continue...";
                        cin.get();
                    }
                    break;
                }
                case 1: { // Register
                    cout << "Create a new mayor account\n";
                    cout << "Enter username: ";
                    getline(cin, username);
                    cout << "Enter password: ";
                    getline(cin, password);
                    
                    user.username = username;
                    user.password = password;
                    
                    if (user.registerUser()) {
                        cout << "Registration successful! You can now login.\n";
                    } else {
                        cout << "Registration failed. Username may already exist.\n";
                    }
                    cout << "Press Enter to continue...";
                    cin.get();
                    break;
                }
                case 2: { // Quick Play (Demo Mode)
                    cout << "Starting demo mode as Mayor 'Demo'...\n";
                    runSimulation("Demo");
                    break;
                }
                case 3: { // Exit Game
                    cout << "Exiting game. Thank you for playing!\n";
                    running = false;
                    break;
                }
                default:
                    cout << "Invalid option. Please try again.\n";
                    cout << "Press Enter to continue...";
                    cin.get();
                    break;
            }
        }
    }
    
    return 0;
}