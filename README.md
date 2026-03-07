*This project has been created as part of the 42 curriculum by vbleskin.*

# FDF

## Description

**FDF** is a 42 school graphic project focused on creating a 3D wireframe renderer. The goal is to read a landscape map from a .fdf file, where each point's value represents its altitude, and represent it as a 3D perspective using an isometric projection.

The project involves managing a window, handling keyboard/mouse inputs, and implementing efficient line-drawing algorithms using the MiniLibX graphical library.

✨ **Standard Features & Bonuses**

In addition to the core requirements, several visual and interactive enhancements were implemented to provide a fluid and comprehensive user experience when exploring the standard .fdf heightmaps:

    Dynamic Camera: Full control over the viewpoint, allowing the user to translate (pan) across the map, zoom in and out smoothly, and freely rotate the model in 3D space using mouse drag and keyboard inputs.

    Multiple Projections: Ability to switch seamlessly between a mathematically accurate Isometric projection and various Orthographic/Parallel views (front, top, side, etc.) to analyze the topology from different angles.

    Color Rendering: Support for parsing and rendering specific hex colors directly from the map files, bringing the wireframes to life and making the altitude variations easier to read.

    Z-Scaling: Real-time manipulation of the Z-axis (altitude) to dynamically exaggerate or flatten the terrain's relief.

🚀 **Extended Features (Custom 3D Engine Bonus)**

Going far beyond the standard curriculum requirements, this project has been upgraded into a custom-built 3D engine. It is capable of parsing and rendering complex 3D models and skeletal animations entirely from scratch, using only the basic MiniLibX graphical library and raw mathematics.

    .OBJ Support: Parses standard 3D Wavefront files, extracting vertices and faces to render complex polygonal structures.

    .FBX Support: Parses ASCII FBX files, navigating through complex nested nodes to extract geometries, models, skeletal structures (bones/deformers), and animation stacks.

    Skeletal Animation & Skinning: Computes complex 4x4 matrix transformations (Translation, Rotation, Scale) and applies vertex weights to deform meshes in real-time based on animation keyframes.

    Multithreading: Utilizes pthread to parallelize heavy matrix calculations and the rasterization process (Bresenham line drawing), ensuring smooth framerates even with high-poly animated models.

    Interactive UI: Features a custom on-screen progress bar and timecodes to track animation playback seamlessly.

## Instructions

### 🌧 Compilation

The project uses a `Makefile` to compile the source files.

* **Compile the project:**
    `make`
    -> This generates the `fdf` executable.

* **Debug the code:**
    `make debug`
    -> This compiles with flag -g3.

* **Clean object files:**
    `make clean`

* **Full clean (objects + executables):**
    `make fclean`

* **Clean and compile :**
    `make re`

### 🌟 Execution

To run the program:
`./fdf [file]`
    -> Example: ./fdf maps/42.fdf

### 🎮 Controls

	- ESC			: Close the window and exit the program.
	- Arrows		: Translate camera position.
	- Scroll		: Zoom in/out.
	- Clic+drag		: Rotate around.
	- R				: Rotate 1.
	- Tab			: Rotate 2.
	- Num 0			: Isometric view.
	- Num 1			: Parallel view 1.
	- Num 2			: Parallel view 2.
	- Num 3			: Parallel view 3.
	- Num 4			: Parallel view 4.
	- Num 5			: Parallel view 5.
	- Num 6			: Parallel view 6.
	- Num 7			: Parallel view 7.
	- -/+			: Change Z axis length.
	- N				: Next animation (for .fbx files).
	- P				: Previous animation (for .fbx files).
	- Space			: Play / pause animation (for .fbx files).

## Resources & AI Usage

* **💻 VIDEOS**

	- PROJETER un POINT 3D sur un Plan 2D [Tout ce qu'il faut savoir] - Quantale :
	https://www.youtube.com/watch?v=SGLK2p3PIqE&list=LL&index=20&t=2s

	- Comment comprendre FACILEMENT les matrices - ParaMaths :
	https://www.youtube.com/watch?v=Dr9J1BIcVBw&list=WL

	- Essence of linear algebra - 3Blue1Brown :
	https://youtube.com/playlist?list=PLZHQObOWTQDPD3MizzM2xVFitgF8hE_ab&si=6JXNZDeUJRwmWLm7

	- How to make a 3D Renderer [Explained Simply] - GraverDev :
	https://www.youtube.com/watch?v=nvWDgBGcAIM&t=130s

	- The Hairy Ball Theorem - 3Blue1Brown :
	https://youtu.be/BHdbsHFs2P0?is=3d5N5q7XCrxXZI42

* **📚 ARTICLES**

	- The Bresenham Line-Drawing Algorithm :
	https://www.cs.helsinki.fi/group/goa/mallinnus/lines/bresenh.html

	- Introduction to multithreading in C/C++ :
	https://hoangvankhoa.medium.com/introduction-to-multithreading-in-c-c-adf7ffbe045d

	- Les angles d’Euler par Ilian Bonev :
	https://mecademic.com/fr/connaissance/tutoriels-academiques/comment-orientation-espace-est-elle-representee-par-angles-euler/

* **🤖 AI USAGE**

	Throughout the development of this project, Artificial Intelligence (LLMs) was utilized as a supportive tool to enhance productivity and deepen my understanding of the engine's underlying mechanics. Specifically, AI was leveraged to:

	Automate Repetitive Tasks: Speeding up the writing of boilerplate code and repetitive structures, saving valuable development time.

	Norm Compliance: Assisting in the refactoring and formatting of the codebase to ensure strict adherence to the rigorous 42 school coding standard ("la Norme").

	Mathematical Comprehension: Acting as an interactive tutor to break down, visualize, and explain complex linear algebra concepts (such as matrix inversions, Euler angles, and skeletal animation algorithms).

	Research & Troubleshooting: Serving as a powerful research assistant to quickly navigate technical documentation, find optimal algorithmic approaches, and debug obscure C programming errors.
