# GABES: Graphical & Accelerated Boundary Element Solver

**Author:** Rahim SI HADJ MOHAND  
**Institution:** Department of Mechanical Engineering, National Polytechnic School, Algiers  
**Thesis Supervisors:** Yacine BELKACEMI, Said RECHAK  
**Laboratory:** Laboratory of Green Mechanical Development (LGMD)

---

## Overview

GABES is an academic software application implementing the **Boundary Element Method (BEM)**  
for solving three-dimensional linear elastostatic problems in isotropic and homogeneous media.

It includes a **graphical user interface (GUI)** for:  
- Importing and visualizing 3D meshed models  
- Configuring numerical and physical parameters  
- Applying boundary conditions  
- Launching simulations  
- Post-processing results interactively  

This project is part of the author's PhD thesis work at the National Polytechnic School of Algiers.

---

## Dependencies

To build and run GABES, the following dependencies are required:

1. **AHMED library (M. Bebendorf, 2012)**  
   - Non-commercial, academic-only library for hierarchical matrices  
   - GABES uses a slightly modified version for GUI integration (progress bars)  

2. **DirectX 9.0 SDK (June 2010)**  
   - Required for 3D visualization  

3. **Microsoft Visual Studio**  
   - Project files provided for MSVC  

---

## Building the Project

1. Install **AHMED** and **DirectX SDK** on your machine  
2. Configure **include and library paths** in the Visual Studio project to point to AHMED and DirectX  
3. Open `GABES.sln` in Visual Studio  
4. Build the solution in either `Debug` or `Release` mode  

> Precompiled binaries are provided in this GitHub repository.  

> Pre-releases: Functional and testable but not fully optimized and partially implemented (~95%).  
> - Version 0.9.0-beta  
> - Version 0.9.1-beta  
> - Version 0.9.2-beta  

> Latest release: Functional and fully implemented (100%)  
> - Version 1.0.0  

> More exact details for setting up the source code, compiling, and tutorials on how to  
> use GABES are provided in the **GABES User's Guide** located in the `\doc` directory.

---

## License

GABES is distributed under the **GABES Academic-Only License** (`LICENSE`).  

Key points:  
- You may use and modify GABES for academic and educational purposes only  
- Redistribution is allowed under the academic-only terms, provided all license and copyright notices are preserved  
- Commercial use is strictly prohibited without prior written permission  
- Modified AHMED routines for GUI progress bars are in `Ahmed_Overrides.h` and `Ahmed_Overrides.cpp`  
- Any distribution using AHMED must include a verbatim copy of the AHMED license (`AHMED License.pdf`)

---

## Acknowledgments

The support from the **Directorate-General for Scientific Research and Technological Development (DG-RSDT)**  
of the Algerian government in the form of a research grant is gratefully acknowledged. The **Laboratory of  
Green and Mechanical Development (LGMD)** of the National Polytechnic School (ENP) is also gratefully  
acknowledged for the resources and support. The author gratefully acknowledges the guidance and support of  
his thesis supervisors: **Dr. Yacine BELKACEMI** and **Pr. Said RECHAK**.

---

## Contact

**Author:** Rahim SI HADJ MOHAND  
  - Email: rahim.si_hadj_mohand@g.enp.edu.dz
  - Research Gate Account: https://www.researchgate.net/profile/Rahim-Si-Hadj-Mohand
  
