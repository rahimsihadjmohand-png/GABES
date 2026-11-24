Copyright © 2026 Rahim Si Hadj Mohand.

**GABES:** is a scientific software application developed as part of Rahim Si Hadj Mohand's PhD research.

**Laboratory:** Laboratory of Green and Mechanical Development (LGMD)

**Institution:** National Polytechnic School (ENP)

**Address:** 10, Rue des Frères Oudek, Hassen Badi, El Harrach, 16200, Algiers, Algeria.

# GABES – Graphical \& Accelerated Boundary Element Solver
GABES (Graphical \& Accelerated Boundary Element Solver) is an open-source software application for the analysis of three-dimensional linear elastostatic problems in homogeneous, isotropic media using the collocation Boundary Element Method (BEM). The software employs hierarchical matrices (H-Matrices) to reduce the computational and memory requirements associated with the dense system matrices arising in conventional BEM formulations. In particular, GABES implements two dedicated on-request evaluation algorithms that avoid the explicit construction and storage of dense system matrices, supporting the H-Matrix approach and its underlying Adaptive Cross Approximation (ACA) low-rank construction. The software also incorporates specialized numerical procedures for the accurate evaluation of regular, nearly singular, and singular boundary integrals, including an adaptive integration scheme specifically developed for GABES.

GABES runs exclusively on 64-bit Windows platforms and provides a graphical user interface with an integrated interactive 3D environment. Users can import three-dimensional surface meshes, define material properties and numerical parameters, assign boundary conditions interactively, and configure and execute simulations within a unified workflow. The software exclusively uses triangular surface meshes, allowing complex three-dimensional geometries to be represented through boundary discretization.

GABES also provides integrated post-processing tools for visualizing and analyzing computed results. Displacements, stresses, and strains can be evaluated at boundary and interior points and visualized using color maps and deformed configurations. Numerical results can also be extracted as tabulated data and exported in CSV format for further analysis and reporting.

## RELEASE NOTES – GABES v2.0.0
This release introduces a major update to GABES, featuring substantial improvements to the Boundary Element Method (BEM) solver and its underlying H-Matrix framework.

The main changes include:

a\. Replacement of the previous AHMED-based H-Matrix implementation with H2Lib;

b\. Improved memory efficiency through compressed H-Matrix representations and on-request evaluation of BEM matrix entries, avoiding the assembly and storage of dense system matrices;

c\. Improvements to ACA-based H-Matrix compression;

d\. Support for H-Matrix-based direct and iterative solvers, including H-LU and GMRES;

e\. Integration of H-LU-based preconditioning to improve iterative solver performance;

f\. Improved configuration options for numerical integration, H-Matrix construction, and solver parameters;

g\. Graphical user interface improvements and refinements;

h\. Bug fixes and corrections to numerical and computational issues.

This version represents a substantial architectural and performance evolution of GABES while preserving its graphical workflow for three-dimensional elastostatic analysis.

## 1. DOWNLOAD AND INSTALLATION
GABES is available as an open-source software project hosted on GitHub. The main repository can be accessed through the following link:
https://github.com/rahimsihadjmohand-png/GABES
The repository provides the latest source code, together with the latest binary release installer for users who prefer to run the software without compiling it.

### 1.1. Downloading the Latest Official Release
To facilitate immediate testing and practical use, official software releases are published with binary installers, allowing users to download, install, and run GABES without manually compiling the source code.

To download the latest release, navigate to the GABES main repository page and click on Releases. This opens the releases page, where the available versions are listed. Select the latest release (e.g., GABES v2.0.1 – Setup) to access its dedicated release page.

Alternatively, the latest release can be accessed directly from the main repository page. Under the Assets section, click on the installer (e.g., GABES v2.0.1 – Setup.exe) to download it.

Once the download is complete, run the installer and follow the instructions provided by the Windows installation wizard to complete the installation.

The release page also provides the source code corresponding to that particular version. However, the repository may occasionally be updated without a new official release being published. Therefore, users wishing to obtain the most recent source code should download it directly from the repository's main page. 

### 1.2. Downloading the Source Code
To download the complete source code package, navigate to the Code tab on the GABES GitHub repository page. Click the green Code button to open the download menu, then select Download ZIP. The archive will be downloaded to the browser's default download directory.

Alternatively, users with Git installed on Windows can clone the repository from the command line by opening a command prompt or terminal in the desired directory and executing the following command: git clone https://github.com/rahimsihadjmohand-png/GABES.git

For most Windows users, downloading the ZIP archive is sufficient and more straightforward.

After extracting the downloaded archive, the resulting directory contains the following main components:
a\. Examples\\ directory: Contains two sample models ready for execution, together with their associated three-dimensional mesh files.

b\. GABES\\ directory: Contains the Visual Studio project files and the C++ source code of GABES.

c\. H2Lib\\ directory: Contains the Visual Studio project files and the imported source code of the H2Lib library.

d\. doc\\ directory: Contains the software User Guide.

e\. .gitattributes: Contains Git configuration instructions used by the repository.

f\. GABES.sln: The main Visual Studio solution file.

g\. LICENSE: Specifies the license terms governing the use, modification, and distribution of GABES.

h\. README.md: Provides introductory information and setup instructions for the project.

Further instructions regarding the configuration and compilation of the source code are provided in the software User's Guide.

## 2. FURTHER INFORMATION
For detailed information concerning:
1\. Software installation and configuration;

2\. Theoretical background and numerical formulations;

3\. Software architecture and organization;

4\. Detailed descriptions of the graphical interface and its functionalities;

5\. Step-by-step tutorials and demonstration examples;

please refer to the GABES User Guide provided in the installation directory and included in the source code repository.

## 3. LICENSE
GABES is distributed under the GNU General Public License, version 3 (GPL-3.0).

The complete license text is provided in the License file of the installation.

## 4. ACKNOWLEDGEMENTS
The support provided by the Directorate-General for Scientific Research and Technological Development (DG-RSDT) of the Algerian government through a research grant is gratefully acknowledged. The Laboratory of Green and Mechanical Development (LGMD) at the National Polytechnic School (ENP) is also gratefully acknowledged for providing the resources and research environment that contributed to the development of GABES.

The guidance and support provided by Pr. Yacine Belkacemi and Pr. Said Rechak (PhD supervisors) are gratefully acknowledged. Their scientific guidance, technical advice, constructive discussions, and valuable recommendations throughout this research are sincerely appreciated.

## 5. FEEDBACK AND CONTRIBUTIONS 
Feedback from users and researchers is highly appreciated. Comments, constructive feedback, and critical suggestions regarding the software, its numerical methods, performance, and usability are warmly welcomed. Such contributions are valuable for identifying limitations, improving existing features, and guiding the future development of GABES.

Readers are encouraged to share their observations, report issues, and propose improvements through the project's GitHub repository or by contacting the author directly at the following email address: rahim.si_hadj_mohand@g.enp.edu.dz.

