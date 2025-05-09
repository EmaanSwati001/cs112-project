//class Activity
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
        
        cout << " Green program launched! Eco +8, Pollution -12, Population +30\n";
    }
};


class DisasterManagement : public Activity {
public:
    void perform(int& eco, int& money, int& population, int& pollution) override {
        // Handle natural disaster
        cout << "Managing natural disaster response!\n";
        pollution += 30;
        money -= 500;
        eco -= 5;
        
        cout << "Disaster contained! Pollution +30, Money -500, Eco -5\n";
        cout << "But it could have been much worse without your intervention!\n";
    }
};

//transport
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
        
        cout << "Subway system expanded! Eco +10, Pollution -15, Population +50\n";
    }
};

//citizen action
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
        
        cout << "New school opened! Population +80, Eco +10\n";
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
        
        cout << "New jobs created! Population +50, Money +200 (ROI)\n";
    }
};

//city infrastructure(derived class acc to final code)
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