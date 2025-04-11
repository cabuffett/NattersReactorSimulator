#include <iostream>
#include <cmath>

class ReactorCore {
public:
    double neutronFlux;
    double reactorTemperature;
    double controlRodInsertion; // 0.0 (out) to 1.0 (in)
    double coolantInletTemperature;
    double coolantOutletTemperature;
    double heatCapacity; // J/K
    double heatTransferCoefficient; // W/(m^2.K)
    double surfaceArea; // m^2
    double powerLevel; // MW
    bool scramReq = false

    ReactorCore(double initialFlux = 1.0, double initialTemp = 300.0, double rodPos = 0.5, double coolantIn = 290.0, double hc = 1e6, double htc = 1e4, double sa = 100.0)
        : neutronFlux(initialFlux), reactorTemperature(initialTemp), controlRodInsertion(rodPos), coolantInletTemperature(coolantIn), coolantOutletTemperature(coolantIn),
          heatCapacity(hc), heatTransferCoefficient(htc), surfaceArea(sa), powerLevel(0.0) {}

    void update(double deltaTime, bool loopStarted) {
        // check if startup has not commenced:
        if (loopStarted == false) {
            
        } else {
             
        // Simplified reactivity model:
        // Reactivity increases with flux, decreases with temperature, decreases with control rod insertion
        double reactivity = 0.01 * neutronFlux - 0.001 * (reactorTemperature - 300.0) - 0.05 * controlRodInsertion;

        // Simplified neutron flux model (exponential growth/decay based on reactivity)
        neutronFlux = neutronFlux * exp(reactivity * deltaTime);
        if (neutronFlux < 0.1) neutronFlux = 0.1; // Prevent flux from going to zero

        // Power generation (proportional to neutron flux)
        powerLevel = 0.001 * neutronFlux; // Scale factor for power in MW

        // Heat generation
        double heatGenerated = powerLevel * 1e6 * deltaTime; // Convert MW to J/s, then to J over deltaTime

        // Heat removal by coolant
        double heatRemoved = heatTransferCoefficient * surfaceArea * (reactorTemperature - (coolantInletTemperature + coolantOutletTemperature) / 2.0) * deltaTime;

        // Update reactor temperature
        reactorTemperature += (heatGenerated - heatRemoved) / heatCapacity;

        // Simplified coolant outlet temperature (linear relationship with power and flow rate - not implemented here for simplicity)
        coolantOutletTemperature = coolantInletTemperature + 0.0001 * powerLevel; // Very basic approximation

        // SCRAM check (Checks if the temperature is above 1000)
        if (reactorTemperature >= 1000) {
            scramReq = true
        }

        }
       
    }

    void setControlRodInsertion(double insertion) {
        controlRodInsertion = std::max(0.0, std::min(1.0, insertion));
    }

    void setCoolantInletTemperature(double temp) {
        coolantInletTemperature = temp;
    }
};