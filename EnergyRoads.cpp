#include <iostream>
#include <vector>
#include <string>
#include <iomanip>
#include <cstdlib>
#include <ctime>

using namespace std;

// ---------------------- Road Class ----------------------
class Road {
private:
    string name, method;   // "solar" or "friction"
    double lengthKm;
    int trafficPerHour;
    double totalEnergy;

public:
    Road(string roadName, string genMethod, double length, int traffic)
        : name(roadName), method(genMethod), lengthKm(length), trafficPerHour(traffic), totalEnergy(0) {}

    void generateEnergy(double sunlight = 1.0) {
        double energy = 0;
        if (method == "solar") {
            energy = 100 * lengthKm * sunlight;
        } else if (method == "friction") {
            energy = trafficPerHour * 0.005 * lengthKm;
        }
        totalEnergy += energy;
    }

    void report() const {
        cout << fixed << setprecision(2);
        cout << name << " [" << method << "] - " << totalEnergy << " kWh" << endl;
    }

    double getTotalEnergy() const {
        return totalEnergy;
    }
};

// ---------------------- City Class ----------------------
class City {
private:
    string name;
    vector<Road> roads;

public:
    City(string cityName) : name(cityName) {}

    void addRoad(const Road& road) {
        roads.push_back(road);
    }

    void simulateHours(int hours) {
        for (int i = 0; i < hours; ++i) {
            double sunlight = 0.6 + static_cast<double>(rand()) / RAND_MAX * 0.4;
            for (auto& road : roads) {
                road.generateEnergy(sunlight);
            }
        }
    }

    void showReport() const {
        double total = 0;
        cout << "\n--- " << name << " Energy Report ---\n";
        for (const auto& road : roads) {
            road.report();
            total += road.getTotalEnergy();
        }
        cout << "Total Energy: " << fixed << setprecision(2) << total << " kWh\n";
    }
};

// ---------------------- Simulation Function ----------------------
void runSimulation() {
    srand(time(0)); // Seed random

    City city("EcoVille");

    city.addRoad(Road("Main Street", "solar", 5.0, 0));
    city.addRoad(Road("Downtown Ave", "friction", 3.0, 1200));
    city.addRoad(Road("Green Road", "solar", 2.0, 0));
    city.addRoad(Road("Industrial Way", "friction", 4.0, 800));

    city.simulateHours(5);
    city.showReport();
}

// ---------------------- Main Function (2 lines only) ----------------------
int main() {
    runSimulation();
    return 0;
}
