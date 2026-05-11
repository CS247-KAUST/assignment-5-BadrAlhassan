[![Review Assignment Due Date](https://classroom.github.com/assets/deadline-readme-button-22041afd0340ce965d47ae6ef1cefeee28c7c493a6346c4f15d667ab976d596c.svg)](https://classroom.github.com/a/DDm8twLC)
# Assignment 5 Description (11% of total grade) #

The task for this assignment is the implementation of Vector field visualization.

## Reading assignments ##

* Data Visualization book, Chapter 6.5, 6.6
* Bruno Jobard and Wilfrid Lefer:
  Creating Evenly-Spaced Streamlines of Arbitrary Density,
  Eurographics Workshop on Visualization in Scientific Computing 1997
  http://citeseerx.ist.psu.edu/viewdoc/summary?doi=10.1.1.29.9498

## Basic Tasks ##

* Vector field visualization in 2D using glyphs
* Streamline integration using Euler and Runge-Kutta methods
* Pathline integration using Euler and Runge-Kutta methods
* Color-coded scalar field visualization using colormaps

## Minimum Requirements ##

+ Glyph visualization: draw glyphs ( arrows or something that indicate direction ) (15 points)
    * Adjustable length: choose between arrows with constant length or arrows with the length depending on the speed (vector magnitude) (5 points)
    * Change (sub)sampling rate for arrows or glyphs (vector grid) (5 points)

+ Streamlines using a) Euler (15 points) and b) RK2 (10 points)
    * Every time you seed a point keep the streamline of that point (don't reset streamlines) (-3 points penalty if the streamlines are removed)
    * When you change a time-slice the streamline should update correctly (recalculate) (5 points)
    * Do bilinear interpolation of vectors (don't snap to nearest vector) (5 points)
    * Stopping condition depends on the accumulated length of the streamline or if the queried vector is almost zero or if you hit a boundary (-3 points penalty if those conditions are not implemented)
    * Do backward integration with forward integration (-3 points penalty if only one integration is implemented)

+ Pathlines using a) Euler (20 points) and b) RK2 (10 points)
    * Every time you seed a point keep the pathline of that point (don't reset pathlines) (-3 points penalty if the streamlines are removed)
    * Do trilinear interpolation of vectors (don't snap to nearest vector and timeslice) (5 points)
    * Stopping condition depends on the accumulated length of the streamline or if the queried vector is almost zero or if you hit a boundary (-3 points penalty if those conditions are not implemented)
    * Do backward integration with forward integration (-3 points penalty if only one integration is implemented)

+ Color-coded scalar field visualization (15 points)
    * Implement at least two colormaps (e.g. rainbow, cool-warm) applied to the background scalar field in the fragment shader (10 points)
    * User adjustable blend factor between grayscale and color-mapped scalar field (5 points)

* Adjustable dt value by user (5 points)



## Bonus ##
* Release multiple streamline seeds in horizontal or vertical rake (+5 points)
* Switch between different background scalar field images (+3 points)
* RK4 for both streamlines (+5 points) and pathlines (+5 points)

## Notes ##
* The description and the download location of the data can be found in the file README_data.txt
* The vector data is in the vector_array, where every 3 elements in the array will give you the xyz component of a vector. After one 2D slice is finished, and if there is more than one time step, the vector data continues for the next time step.
  So the size of the array = 3 * width * height * number_of_timesteps. More information can be found in the file README_data.txt found in the source code.
* There aren't prototypes for every function you might need. Create functions as you need them.


## Screenshots for Minimum Requirements Solution ##
Glyphs Visualization

![glyphs.png](https://bitbucket.org/repo/Mq6ygx/images/425286000-glyphs.png)

Streamlines

![streamlines.png](https://bitbucket.org/repo/Mq6ygx/images/154883112-streamlines.png)

Pathlines

![pathlines.png](https://bitbucket.org/repo/Mq6ygx/images/3862458026-pathlines.png)

---

# Implementation Report

## Build Instructions

Configure and build with CMake, then run:

```powershell
cmake -B build
cmake --build build
.\build\assignment_5B.exe
```

The application loads the `block` dataset by default. Keys `1` (block), `2` (tube), and `3` (hurricane) switch datasets. Place data files in `data/block/`, `data/tube/`, `data/hurricane/`.

## Implementation Summary

### Glyph Visualization
Arrow glyphs indicate vector direction at each grid point, drawn as a line shaft with two triangle lines forming the arrowhead. The `L` key toggles between constant-length arrows and speed-proportional arrows (length scaled by the ratio of local speed to global maximum speed). The `+`/`-` keys adjust the grid sampling rate, controlling glyph density.

### Streamlines
Streamlines are computed via `integratePts()`, which supports Euler (`method == 0`) and RK2 (`method == 1`) integration. Integration proceeds both forward and backward from the seed point. Bilinear interpolation (`getVectorBilinear()`) samples the vector field at fractional positions rather than snapping to the nearest grid vertex. Stopping conditions include: accumulated path length exceeding a threshold (500 units), vector magnitude below `1e-6` (zero vector), and reaching the data boundary. Seeds are stored in a persistent `streamline_seeds` vector so previously placed streamlines remain when new ones are added. When the timestep changes, `loadNextTimestep()` recalculates all streamlines from their stored seeds while preserving each seed's original integration method.

### Pathlines
Pathlines follow the same integration logic as streamlines but use `getVectorTrilinear()` for trilinear interpolation across both space and time, allowing the seed's starting time to advance during integration. Stopping conditions are identical to streamlines (path length, zero vector, boundary) with an additional timestep boundary check. Seeds persist independently and are only removed when pressing `X`.

### Color-Coded Scalar Field
The background scalar field is loaded as a grayscale texture and colored in the fragment shader using two colormaps: a multi-stop rainbow (blue, cyan, green, yellow, red) and a blue-white-red cool-warm scheme. The `C` key cycles through off/rainbow/cool-warm modes. The `V`/`N` keys adjust a blend factor.

### Adjustable dt
The `I`/`K` keys increase/decrease the integration time step `dt` (range 0.0001–1.0). Any change to `dt` triggers an automatic recomputation of all existing streamlines and pathlines using their stored seed positions and methods.

### RK4 Integration (Bonus)
The `integratePts()` function was extended with the classical fourth-order Runge-Kutta method (`method == 2`) for both streamlines and pathlines. The `R` key now cycles through three methods: Euler (yellow), RK2 (cyan), and RK4 (magenta).

### Rake Seeding (Bonus)
Pressing `G` cycles rake mode off → horizontal → vertical. When active, a left-click seeds 10 streamlines along a rake line instead of a single point. Each seed preserves its integration method across timestep changes.

### Switch Scalar Fields (Bonus)
The `S` key cycles between available scalar fields (pressure/vorticity for block, temperature/cloud moisture for hurricane).

## Keyboard Shortcuts

| Key | Action |
|-----|--------|
| `1`/`2`/`3` | Load block/tube/hurricane dataset |
| `0` | Cycle timestep |
| `A` | Toggle glyph arrows |
| `T` | Toggle streamlines |
| `P` | Toggle pathlines |
| `+`/`-` | Increase/decrease sampling rate |
| `I`/`K` | Increase/decrease dt |
| `L` | Toggle arrow length (constant ↔ speed) |
| `C` | Cycle colormap (off → rainbow → cool-warm) |
| `V`/`N` | Increase/decrease blend factor |
| `R` | Cycle integration (Euler → RK2 → RK4 (Bonus)) |
| `G` | Cycle rake mode (off → horizontal → vertical). Bonus |
| `S` | Switch scalar field. Bonus| 
| `X` | Clear all seeds |
| `B` | Cycle background color |
| `Q`/`Esc` | Quit |
| **Left click** | Seed streamline and/or pathline at cursor; with rake mode, seeds multiple streamlines
