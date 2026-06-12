#ifndef PHYSICS_HPP
#define PHYSICS_HPP

#include <map>
#include <utility>
#include "Lattice.hpp"

// Define a physical constants namespace
namespace Physics {
    
    // Boltzmann constant in eV/K
    constexpr double k_B = 8.617333262145e-5; 
    // Simulation Temperature (e.g., 300K for standard battery operation)
    constexpr double T = 300.0; 

    // E_ICOHP bonding energies in eV (Extracted directly from Table 1 of the paper)
    // Using a map to easily look up the energy between two specific species
    const std::map<std::pair<Species, Species>, double> E_ICOHP = {
        {{Species::Li_Ion, Species::Li_Ion}, 0.1037},
        {{Species::Li_Ion, Species::F_Ion},  0.7146}, // Strong ionic bond in LiF
        {{Species::F_Ion,  Species::F_Ion},  0.0336},
        {{Species::Li_Ion, Species::O_Ion},  0.8027}, // Strong ionic bond in Li2O
        {{Species::O_Ion,  Species::O_Ion},  0.0615},
        // Note: For metallic lithium in the slab, the paper implies a weaker bond
        // compared to the inorganic phases. We can use the Li-Li value here.
        {{Species::Li_Metal, Species::Li_Metal}, 0.1037} 
    };

    // Helper function to safely get bonding energy
    inline double getBondingEnergy(Species s1, Species s2) {
        // Ensure we check both orderings (s1-s2 and s2-s1)
        auto pair1 = std::make_pair(s1, s2);
        auto pair2 = std::make_pair(s2, s1);
        
        if (E_ICOHP.count(pair1)) return E_ICOHP.at(pair1);
        if (E_ICOHP.count(pair2)) return E_ICOHP.at(pair2);
        
        // If no explicit interaction exists, return 0.0 (no bonding energy)
        return 0.0; 
    }
}

#endif