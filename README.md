# Spiri uC software

This repository holds source for computation module software used in Spiri car (models CP104 and newer), as well vehicles retrofitted with Spiri ride-sharing cluster technology.

# Toolchain

* QtCreator is used as IDE. HalCoGen is used for generating HAL and FreeRTOS sources
* GCC toolchain is used for compiling and cross-compiling 
* Clang is used for static analysis, Valgrind is used for dynamic analysis
* QTestLib is used as unit testing framework
* [Qt Coding Style](https://wiki.qt.io/Qt_Coding_Style) and [Qt Coding Conventions](https://wiki.qt.io/Coding_Conventions) are used for all Qt code

# Qt methodologies

* IPC mechanism - D-BUS through Qt Signal/Slots
* Use Signal/Slots whenever possible
* Divide tasks into different processes if one or more parts are likely to be updated more frequently than the others
* Keep disk flushes to minimum
* Save debug data on uSD card, if present
* Use threading when necessary
* If we need functionality not supported by Qt we will discuss it in the team at that time

# Repository organization 

* Releases will have separate branches, first release has codename Anvil
* Each developer will have at least one main development branch and a review branch
* Features are developed in their own branches and after code review merged into the release branches
* Releases will be merged into the master branch after release approval
* Releases will be tagged across repositories, with common release names
* Code shared between repositories will be kept in subtree repositories

