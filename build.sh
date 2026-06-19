#!/bin/bash
# Build script for Summer_project KMC simulation

# Color output
RED='\033[0;31m'
GREEN='\033[0;32m'
YELLOW='\033[1;33m'
NC='\033[0m' # No Color

echo -e "${YELLOW}Building Summer_project...${NC}"

# Check if C++ compiler is available
if ! command -v g++ &> /dev/null; then
    echo -e "${RED}Error: g++ not found. Please install a C++ compiler.${NC}"
    exit 1
fi

# Compile the project
echo -e "${YELLOW}Compiling C++ sources...${NC}"
g++ -std=c++17 -O2 Main.cpp Lattice.cpp -o sei_simulation

if [ $? -eq 0 ]; then
    echo -e "${GREEN}Build successful! Executable: sei_simulation${NC}"
    chmod +x sei_simulation
    echo -e "${YELLOW}To run the simulation:${NC}"
    echo "./sei_simulation"
else
    echo -e "${RED}Build failed!${NC}"
    exit 1
fi
