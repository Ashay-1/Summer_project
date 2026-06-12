#ifndef KINETICS_HPP
#define KINETICS_HPP

#include "Lattice.hpp"
#include "Physics.hpp"
#include <cmath>
#include <vector>

class Kinetics {
public:
    // 1. Periodic Boundary Condition (PBC) Distance Calculator
    // Ensures atoms interacting across the edges of the box are calculated correctly
   // 1. Optimized PBC Distance Calculator (No Square Root)
    static double calculatePBCDistanceSquared(const LatticeSite& s1, const LatticeSite& s2, 
                                       double boxLengthX, double boxLengthY, double boxLengthZ) {
        
        double dx = s1.x - s2.x;
        double dy = s1.y - s2.y; 
        double dz = s1.z - s2.z;

        dx -= boxLengthX * std::round(dx / boxLengthX);
        dy -= boxLengthY * std::round(dy / boxLengthY);
        dz -= boxLengthZ * std::round(dz / boxLengthZ);

        // Return the squared distance to save CPU cycles
        return (dx*dx + dy*dy + dz*dz);
    }

    // 2. Dynamic Binding Energy Calculator
    // Checks a site's local coordination environment to determine its mobility barrier
    static double calculateBindingEnergy(int targetIndex, const std::vector<LatticeSite>& grid, 
                                         double boxLengthX, double boxLengthY, double boxLengthZ) {
        
        double totalBindingEnergy = 0.0;
        const LatticeSite& targetSite = grid[targetIndex];
        
        // If the site is empty, it has no binding energy
        if (targetSite.currentSpecies == Species::Vacancy) return 0.0;

        // Cutoff distance for nearest neighbors (~3.9 Angstroms covers the first coordination shell for this lattice)
        // ... inside calculateBindingEnergy ...
        const double cutoffDistanceSq = 3.9 * 3.9; // Square the cutoff

        for (size_t i = 0; i < grid.size(); ++i) {
            if (i == targetIndex) continue; 
            const LatticeSite& neighbor = grid[i];
            
            if (neighbor.currentSpecies != Species::Vacancy) {
                double distSq = calculatePBCDistanceSquared(targetSite, neighbor, boxLengthX, boxLengthY, boxLengthZ);
                
                if (distSq <= cutoffDistanceSq) {
                    totalBindingEnergy += Physics::getBondingEnergy(targetSite.currentSpecies, neighbor.currentSpecies);
                }
            }
        }
        return totalBindingEnergy;
    }

    // 3. The Arrhenius Rate Equation
    // Calculates the reaction rate (k) for an event
    static double calculateReactionRate(double attemptFrequency, double energyBarrier, 
                                        double transferCoeff, double appliedVoltage, double referenceVoltage) {
        
        // k = v * exp[-(E_a - alpha * (E - E_0)) / k_B * T]
        // Note: For pure diffusion (no electron transfer), alpha and appliedVoltage components are 0.
        
        double exponent = -(energyBarrier - transferCoeff * (appliedVoltage - referenceVoltage)) / 
                          (Physics::k_B * Physics::T);
                          
        return attemptFrequency * std::exp(exponent);
    }
};

#endif