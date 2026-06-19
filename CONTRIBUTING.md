# Contributing to Summer_project

Thank you for your interest in contributing to this KMC simulation project!

## Getting Started

1. **Fork the repository** on GitHub
2. **Clone your fork** locally:
   ```bash
   git clone https://github.com/YOUR_USERNAME/Summer_project.git
   cd Summer_project
   ```
3. **Create a new branch** for your feature or bug fix:
   ```bash
   git checkout -b feature/your-feature-name
   ```

## Building the Project

### Prerequisites
- C++ compiler supporting C++17 (g++ or clang++)
- Python 3 with pip

### Compilation

Use the provided build script:
```bash
./build.sh
```

Or compile manually:
```bash
g++ -std=c++17 -O2 Main.cpp Lattice.cpp -o sei_simulation
```

### Python Setup

Install Python dependencies:
```bash
pip install -r requirements.txt
```

## Code Style Guidelines

### C++
- Use C++17 standard features
- Follow 4-space indentation
- Use meaningful variable names
- Add comments for complex algorithms

### Python
- Follow PEP 8 style guide
- Use 4-space indentation
- Add docstrings to functions
- Keep lines under 100 characters where possible

## Testing

Before submitting a pull request:

1. **Build the C++ code** without errors or warnings
2. **Run the simulation** and verify output
3. **Test Python scripts** with sample data
4. **Check Python syntax** with `python -m py_compile`

## Submitting Changes

1. **Commit your changes** with clear, descriptive messages:
   ```bash
   git commit -m "Add feature: description of changes"
   ```

2. **Push to your fork**:
   ```bash
   git push origin feature/your-feature-name
   ```

3. **Create a Pull Request** on the main repository with:
   - Clear description of changes
   - Reference to any related issues
   - Explanation of new features or bug fixes

## Areas for Contribution

We welcome contributions in the following areas:

- **Build improvements**: Enhanced build scripts, cross-platform support
- **CI/CD enhancements**: Additional testing, code coverage, automated releases
- **Documentation**: Improved README, API documentation, usage examples
- **Python packaging**: Setup.py, distribution files
- **Performance optimization**: C++ algorithmic improvements
- **Bug fixes**: Issue reports and fixes
- **Visualization**: Enhanced plotting and analysis tools

## Reporting Issues

Before opening an issue, please check if it already exists. When reporting a bug, include:

- Description of the issue
- Steps to reproduce
- Expected vs. actual behavior
- System information (OS, compiler version, Python version)

## Questions?

Feel free to open an issue with the `question` label or contact the repository owner: @Ashay-1

## Code of Conduct

Please be respectful and constructive in all interactions. We aim to maintain a welcoming and inclusive community.

---

Thank you for contributing! 🚀
