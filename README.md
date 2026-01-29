# GABES: Graphical & Accelerated Boundary Element Solver

**Author:** Rahim SI HADJ MOHAND  
**Institution:** Department of Mechanical Engineering, National Polytechnic School, Algiers  
**Thesis Supervisors:** Yacine BELKACEMI, Said RECHAK  
**Laboratory:** Laboratory of Green Mechanical Development (LGMD)

---

## Overview

GABES is an open-source application implementing the **Boundary Element Method (BEM)** for solving three-dimensional linear elastostatic problems in isotropic and homogeneous media.  

It provides a **graphical user interface (GUI)** for:  
- Importing and visualizing 3D meshed models  
- Configuring numerical and physical parameters  
- Applying various boundary conditions  
- Launching simulations  
- Post-processing results interactively  

This project is part of the author's PhD thesis work at the National Polytechnic School of Algiers.

---

## Dependencies

To build and run GABES, the following dependencies are required:

1. **AHMED library (M. Bebendorf, 2012)**  
   - Non-commercial, academic-only library for hierarchical matrices  
   - GABES uses a slightly modified version of AHMED for GUI integration (progress bars).  

2. **DirectX 9.0 SDK - June 2010**  
   - Required for 3D visualization.  

3. **Microsoft Visual Studio (required)**  
   - Project files are provided for MSVC.  

> **Note:** GABES is academic-only due to AHMED’s non-commercial restriction. Commercial use requires prior written permission from the authors of both GABES and AHMED.

---

## Building the Project

1. Install **AHMED** and **DirectX SDK** on your machine.  
2. Configure the **include paths and library paths** in the Visual Studio project to point to AHMED and DirectX directories.  
3. Open `GABES.sln` in Visual Studio.  
4. Build the solution in either `Debug` or `Release` mode.  

> Precompiled binaries are provided as separate releases in this GitHub repository.  
> Available releases:  
> - Version 0.9.0-beta: Not fully implemented (95%), but functional and testable  

---

## License

GABES is distributed under the **GABES Academic-Only License** (see `LICENSE`).  
- Academic and educational use only  
- Redistribution allowed for academic users  
- Commercial use strictly prohibited if AHMED is included (see `LICENSE.AHMED`)  
- Modified AHMED routines for GUI progress bars are in `Ahmed_Overrides.h` and `Ahmed_Overrides.cpp`  

---

## Acknowledgments

- M. Bebendorf for **AHMED**  
- Thesis supervisors: **Yacine BELKACEMI** and **Said RECHAK**  
- Laboratory of Green Mechanical Development (**LGMD**) at National Polytechnic School of Algiers  

---

## Contact

- **GABES Author:** Rahim SI HADJ MOHAND  
  Email: rahim.si_hadj_mohand@g.enp.edu.dz


