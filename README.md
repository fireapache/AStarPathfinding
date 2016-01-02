AStarPathfinding
================

A A* based pathfinding application to generate paths within a matrix of nodes, generally used in 2D game platforms.

Demonstration: http://www.youtube.com/watch?v=eVRv49ZibQw

================

Contents of this repository:

	* Qt GUI project:
	
		Used to build 2D maps (matrix of nodes), and to visualize the states of the A* algorithm until the final optimal 
		path has been created.
		
		In order to compile the GUI application, you'll have to, first, install the Qt Creator 2.7.1 or later (based on 
		Qt 4.8.4 32-bit framework), second, clone this entire repository or pull the updates, third, open the project 
		using the downloaded editor, and finally, compile the application within the Qt Creator environment.
		
	* A* Pathfinding source code:
	
		The developed A* algorithm is located inside the Pathfinding\ directory.
		
		astar.h
		astar.cpp
		
		The rest of the files belongs to the Qt GUI project.
		

================

Installation:

	* Install Qt Creator from Ubuntu Software Center.
	* Clone this repository.
	* Run it by: ./Build/Pathfinding
	* Or open Pathfinding.pro file using Qt Creator editor.
	* Have fun!

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
