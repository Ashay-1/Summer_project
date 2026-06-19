# Summer_project

A lattice-based kinetic Monte Carlo (KMC) simulation with Python visualization tools for analyzing materials science phenomena.

## Overview

This repository contains:
- **C++ implementation** of a KMC simulation engine for lattice-based modeling
- **Python analysis tools** for data visualization and result processing
- **Complete documentation** and build infrastructure

## Quick Start

### Prerequisites
- C++17 compatible compiler (g++ or clang++)
- Python 3.6+
- pip or conda for Python package management

### Build & Run (Linux/macOS)

**Option 1: Using the build script**
```bash
chmod +x build.sh
./build.sh
./sei_simulation
```

**Option 2: Manual compilation**
```bash
g++ -std=c++17 -O2 Main.cpp Lattice.cpp -o sei_simulation
chmod +x sei_simulation
./sei_simulation
```

### Python Visualization

Install Python dependencies:
```bash
pip install -r requirements.txt
```

Run visualization scripts:
```bash
python plot_metrics.py
python plot_efficiency.py
python visualize_sei.py
```

## Project Structure

```
Summer_project/
├── Main.cpp                    # Program entry point
├── Lattice.cpp                 # Lattice implementation
├── Lattice.hpp                 # Lattice header
├── KMCEngine.hpp               # KMC engine interface
├── Kinetics.hpp                # Kinetics calculations
├── Physics.hpp                 # Physics models
├── sei_simulation              # Compiled executable (optional)
├── plot_metrics.py             # Metrics visualization
├── plot_efficiency.py          # Efficiency analysis
├── visualize_sei.py            # SEI visualization
├── metrics.csv                 # Sample output data
├── trajectory.csv              # Sample trajectory data
├── build.sh                    # Build script
├── requirements.txt            # Python dependencies
├── LICENSE                     # MIT License
├── README.md                   # This file
├── CONTRIBUTING.md             # Contribution guidelines
└── .github/workflows/
    └── ci.yml                  # GitHub Actions CI/CD
```

## Features

- **Efficient KMC simulation** on lattice structures
- **Flexible physics engine** for materials science applications
- **CSV data export** for analysis
- **Python-based visualization** with matplotlib
- **Cross-platform support** (Linux, macOS, Windows with WSL)
- **Automated CI/CD** with GitHub Actions

## Language Composition

- **C++** (79.7%) - Core simulation engine
- **Python** (20.3%) - Analysis and visualization tools

## Documentation

- [CONTRIBUTING.md](CONTRIBUTING.md) - Contribution guidelines and code style
- [Build Script](build.sh) - Automated compilation
- [CI/CD Pipeline](.github/workflows/ci.yml) - Continuous integration setup

## Common Issues & Solutions

### Build fails with "g++ not found"
Install a C++ compiler:
- **Ubuntu/Debian**: `sudo apt-get install build-essential`
- **macOS**: `brew install gcc`
- **Windows**: Use WSL or install MinGW

### Python scripts can't find CSV files
Ensure you've run the simulation first:
```bash
./sei_simulation
```
This generates `metrics.csv` and `trajectory.csv` in the current directory.

### Import errors in Python scripts
Install missing packages:
```bash
pip install -r requirements.txt
```

## Contributing

We welcome contributions! Please see [CONTRIBUTING.md](CONTRIBUTING.md) for guidelines on:
- Setting up your development environment
- Code style and standards
- Testing requirements
- Submitting pull requests

## License

This project is licensed under the **MIT License** - see [LICENSE](LICENSE) for details.

## Contact

**Repository Owner**: @Ashay-1

For questions, issues, or suggestions, please open a GitHub issue or contact the repository owner.

---

**Last Updated**: 2026-06-19
