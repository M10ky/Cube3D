*This project has been created as part of the 42 curriculum by jonandri, miokrako.*

# cub3D

# Description:

### Overview :
cub3D is a 3D graphics project from the 42 School curriculum. It consists of creating a real-time 3D maze exploration engine using raycasting technology, inspired by the iconic game Wolfenstein 3D. The project is written entirely in C and uses the MiniLibX graphics library to handle window management, image rendering, and user input.
### Goal :
The main goal of cub3D is to build a functional first-person 3D renderer from a 2D map. The player must be able to navigate through a maze, look around, and experience a convincing 3D perspective. This project aims to teach fundamental concepts such as:
- Raycasting algorithms (DDA – Digital Differential Analyzer)
- Real-time graphics rendering
- Basic game loop mechanics
- Event handling (keyboard and mouse)

##### cub3D is both a technical and creative challenge. It bridges the gap between mathematics, graphics programming, and game design. By completing this project, developers gain a deep understanding of how early 3D game engines worked

# Instructions:

### Installation
**1 - Clone the repository**  

	git clone <-repository-url-> [name]

	./[name]

**2 - Ensure `MiniLibX` is installed:**

- Download the file `minilibx-linux.tgz` attached to the project’s page and extract it into **[name]** folder.

### Compilation

To compile the project, run :

	make

**This will generate the executable file named `cub3D`**

#### Available Make commands:
- **make or make all** : Compiles the project
- **make clean** : Removes object files
- **make fclean** : Removes object files and the executable
- **make re** : Recompiles the project from scratch (fclean + all)

### Execution

The repository contains the following directories:

```
cub3D/
├── includes_parsing
|
├── includes_raycasting
|
├── Libft/
|
├── maps/
|
├── src_parsing/
|
├── src_raycasting/
│
├── textures/
├── main.c
├── Makefile
└── README.md

```

**To run the program :**
- **Provide a valid .cub map and put it to ./maps folder**
#### Map File Format (.cub) :
The map file must follow a specific format:

	NO ./textures/north.xpm
	SO ./textures/south.xpm
	WE ./textures/west.xpm
	EA ./textures/east.xpm
	F 220,100,0
	C 0,100,200

	111111
	100001
	101001
	10N001
	111111

- NO, SO, WE, EA: Paths to wall textures (XPM format)

- F: Floor color (RGB values from 0 to 255)

- C: Ceiling color (RGB values from 0 to 255)

- Map: Must be enclosed by walls (1), with 0 representing empty space and N/S/E/W indicating the player's starting position and orientation

**File it as an argument :**

		./cub3D ./maps/example.cub

# Resources :

### Project Documentation :

https://lodev.org/cgtutor/raycasting.html   
https://valgrind.org/docs/   
https://en.wikipedia.org/wiki/RGB_color_model   
https://en.wikipedia.org/wiki/X_PixMap   
https://www.gnu.org/software/gdb/documentation/   

### Use of Artificial Intelligence :

**Debugging and Error Resolution**
- Identifying segmentation faults (segfaults)
- Finding memory leaks before running Valgrind
- Understanding obscure compiler errors
- Detecting logical errors in mathematical calculations
