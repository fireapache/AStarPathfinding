AStarPathfinding
================

A A* based pathfinding application to generate paths within a matrix of nodes, generally used in 2D game platforms.

Demonstration: http://www.youtube.com/watch?v=eVRv49ZibQw

================

Technology Stack:

	* GLFW 3 - Cross-platform windowing and input
	* Dear ImGui - Immediate mode GUI
	* OpenGL 2.1 - Graphics rendering
	* stb_image - Image loading

================

Contents of this repository:

	* GUI Application:

		Used to build 2D maps (matrix of nodes), and to visualize the states of the A* algorithm until the final optimal
		path has been created. The application uses GLFW for window management, Dear ImGui for the user interface,
		and OpenGL for rendering the grid.

	* A* Pathfinding source code:

		The developed A* algorithm is located in the src/ directory:

		src/astar.h
		src/astar.cpp

		The core A* algorithm is completely independent of the GUI framework.


================

Building from Source:

Requirements:
	* CMake 3.14 or later
	* C++11 compatible compiler (GCC, Clang, MSVC)
	* GLFW 3
	* OpenGL 2.1 or later
	* Git (for downloading dependencies)

Note: CMake will automatically download Dear ImGui and stb_image during the build process.

On Ubuntu/Debian:

	# Install dependencies (only GLFW and OpenGL needed - ImGui downloads automatically)
	sudo apt-get update
	sudo apt-get install build-essential cmake git libglfw3-dev libgl1-mesa-dev libglu1-mesa-dev

	# Clone and build
	git clone https://github.com/fireapache/AStarPathfinding.git
	cd AStarPathfinding
	mkdir build && cd build
	cmake ..
	make -j4

	# Run the application
	./AStarPathfinding

On other platforms:

	1. Install CMake and a C++ compiler
	2. Install GLFW3 development libraries
	3. Install OpenGL development libraries
	4. Follow the same build steps as above

================

Usage:

	1. Set the map width and height
	2. Click "Build Map" to create a new grid
	3. Use the tool buttons to:
	   - Set Start position [S]
	   - Set End position [E]
	   - Draw walls [#] (impassable obstacles)
	   - Draw walkable paths [W]
	   - Draw dirt [D] (higher cost terrain)
	4. Choose heuristic: Euclidean or Manhattan
	5. Click "Show Path" to compute and display the optimal path
	6. Click "Show Step" or scroll mouse wheel to step through the algorithm visualization
	7. Click and drag to paint multiple tiles at once

================

	Developer contact:
	
	vinicius-rs@hotmail.com.br
	or
	vrdsantos@inf.ufpel.edu.br
	
	*
	* Vinícius Rodrigues dos Santos
	*
	* Student of Computer Science at Federal University of Pelotas (UFPEL), Rio Grande do Sul, Brazil.
	* Researcher at LUPS (Laboratory of Ubiquitous and Parallel Systems).
	*
