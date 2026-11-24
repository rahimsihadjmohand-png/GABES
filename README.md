Copyright (C) 2026, Rahim Si Hadj Mohand



# GABES – Graphical \& Accelerated Boundary Element Solver



GABES (Graphical \& Accelerated Boundary Element Solver) is an open-source software application for the analysis of three-dimensional linear elastostatic problems in homogeneous, isotropic media using the collocation Boundary Element Method (BEM). The software employs hierarchical matrices (H-Matrices) to reduce the computational and memory requirements associated with the dense system matrices arising in conventional BEM formulations. In particular, GABES implements two dedicated on-request evaluation algorithms that avoid the explicit construction and storage of dense system matrices, supporting the H-Matrix approach and its underlying Adaptive Cross Approximation (ACA) low-rank construction. The software also incorporates specialized numerical procedures for the accurate evaluation of regular, nearly singular, and singular boundary integrals, including an adaptive integration scheme specifically developed for GABES.



GABES runs exclusively on 64-bit Windows platforms and provides a graphical user interface with an integrated interactive 3D environment. Users can import three-dimensional surface meshes, define material properties and numerical parameters, assign boundary conditions interactively, and configure and execute simulations within a unified workflow. The software exclusively uses triangular surface meshes, allowing complex three-dimensional geometries to be represented through boundary discretization.



GABES also provides integrated post-processing tools for visualizing and analyzing computed results. Displacements, stresses, and strains can be evaluated at boundary and interior points and visualized using color maps and deformed configurations. Numerical results can also be extracted as tabulated data and exported in CSV format for further analysis and reporting.



================================================================================

RELEASE NOTES – GABES v2.0.0

================================================================================



This release introduces a major update to GABES, featuring substantial improvements to the Boundary Element Method (BEM) solver and its underlying H-Matrix framework.



The main changes include:



1\. Replacement of the previous AHMED-based H-Matrix implementation with H2Lib;

2\. Improved memory efficiency through compressed H-Matrix representations and on-request evaluation of BEM matrix entries, avoiding the assembly and storage of dense system matrices;

3\. Improvements to ACA-based H-Matrix compression;

4\. Support for H-Matrix-based direct and iterative solvers, including H-LU and GMRES;

5\. Integration of H-LU-based preconditioning to improve iterative solver performance;

6\. Improved configuration options for numerical integration, H-Matrix construction, and solver parameters;

7\. Graphical user interface improvements and refinements;

8\. Bug fixes and corrections to numerical and computational issues.



This version represents a substantial architectural and performance evolution of GABES while preserving its graphical workflow for three-dimensional elastostatic analysis.



================================================================================

1\. DOWNLOAD AND INSTALLATION

================================================================================



GABES is available as an open-source software project hosted on GitHub. The main repository can be accessed through the following link:



https://github.com/rahimsihadjmohand-png/GABES



The repository provides the latest source code, together with the latest binary release installer for users who prefer to run the software without compiling it.



1.1. Downloading the Latest Official Release

\--------------------------------------------



To facilitate immediate testing and practical use, official software releases are published with binary installers, allowing users to download, install, and run GABES without manually compiling the source code.



To download the latest release, navigate to the GABES main repository page and click on Releases. This opens the releases page, where the available versions are listed. Select the latest release (e.g., GABES v2.0.1 – Setup) to access its dedicated release page.



Alternatively, the latest release can be accessed directly from the main repository page. Under the Assets section, click on the installer (e.g., GABES v2.0.1 – Setup.exe) to download it.



Once the download is complete, run the installer and follow the instructions provided by the Windows installation wizard to complete the installation.



The release page also provides the source code corresponding to that particular version. However, the repository may occasionally be updated without a new official release being published. Therefore, users wishing to obtain the most recent source code should download it directly from the repository's main page. 



1.2. Downloading the Source Code

\--------------------------------



To download the complete source code package, navigate to the Code tab on the GABES GitHub repository page. Click the green Code button to open the download menu, then select Download ZIP. The archive will be downloaded to the browser's default download directory.



Alternatively, users with Git installed on Windows can clone the repository from the command line by opening a command prompt or terminal in the desired directory and executing the following command:



git clone https://github.com/rahimsihadjmohand-png/GABES.git



For most Windows users, downloading the ZIP archive is sufficient and more straightforward.



After extracting the downloaded archive, the resulting directory contains the following main components:



1\. Examples\\ directory: Contains two sample models ready for execution, together with their associated three-dimensional mesh files.

2\. GABES\\ directory: Contains the Visual Studio project files and the C++ source code of GABES.

3\. H2Lib\\ directory: Contains the Visual Studio project files and the imported source code of the H2Lib library.

4\. doc\\ directory: Contains the software User Guide.

5\. .gitattributes: Contains Git configuration instructions used by the repository.

6\. GABES.sln: The main Visual Studio solution file.

7\. LICENSE: Specifies the license terms governing the use, modification, and distribution of GABES.

8\. README.md: Provides introductory information and setup instructions for the project.



Further instructions regarding the configuration and compilation of the source code are provided in the software User's Guide.



================================================================================

2\. FURTHER INFORMATION

================================================================================



For detailed information concerning:



1\. Software installation and configuration;

2\. Theoretical background and numerical formulations;

3\. Software architecture and organization;

4\. Detailed descriptions of the graphical interface and its functionalities;

5\. Step-by-step tutorials and demonstration examples;



please refer to the GABES User Guide provided in the installation directory and included in the source code repository.



================================================================================

3\. LICENSE

================================================================================



GABES is distributed under the GNU General Public License, version 3 (GPL-3.0).



The complete license text is provided in the License directory of the installation.



