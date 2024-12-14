# WFC
Uses Wave Function Collapse to generate a random map.  
This repository is a GBA implementation of [a previous demo](https://github.com/DgZiga/MapGenerator).

## WIP
Although most of the work is done, this repository is still a work in progress. 
See [Wfc module todos](#wfc-module-todos) and [implementation todos](#implementation-todos) for more information.

## Internal workings
The implementation currently hooks at address `0x08058A02`, inside the function that loads map data from a given header. From there, it creates a new map with hard-coded values.

### Wfc module
The Wfc module, included with the file `wfc/wfc.h`, contains the necessary code to run the [Wave Function Collapse (WFC)](https://github.com/mxgmn/WaveFunctionCollapse) algorithm.  

The internal workings of this codebase are mostly the same as the [previous demo ones](https://github.com/DgZiga/MapGenerator?tab=readme-ov-file#internal-workings). The main difference is that the propagation of information has been converted from a recursive function to a stack-in-wram-based one, as the call stack of the GBA isn't big enough to support anything more than a small map.

The main issue currently is that the propagation step of WFC takes far too long for larger maps:
| time taken to complete (s) | map size |
|----------------------------|----------|
| ~300                       | 40x40    |
| 15                         | 20x20    |
| 5                          | 15x15    |
| 1                          | 10x10    |

#### Wfc module todos
- optimize for speed
- map borders contraints: for example pallet town borders impose that the map's topmost row of tiles must be mostly filled with middle sections of trees
- find out how to properly calc stack size in recalc_prob_iterative
- tileset is hardcoded
- walkable/non-walkable must be done by hand

### Rasterization module
The internal workings of this codebase are mostly the same as the [previous demo ones](https://github.com/DgZiga/MapGenerator?tab=readme-ov-file#internal-workings). The main difference is that the Ellipse rasterization algorithm has been complitely rewritten with a custom implementation to get around the use of floating numbers: more info can be found directly in the [source code comments](https://github.com/DgZiga/FR-WFC/blob/93c7806233a9942088e32ac2560c714993f2c528/src/include/rasterization/ellipse.c#L25).

### Implementation
Currently the implementations creates an hard-coded amount of ellipses with random center coordinates, and links them up with vectors.

#### Implementation todos:
- create various algorithms for different map types (city, forest, cave, mazes all have different layouts).
- find out how to create your own tileset (current implemnetation only works in cities like pallet town)
- fix borders

## How to insert
This tool requires:
- python (tested with 3.8.3)
- [devkitARM](https://devkitpro.org/wiki/devkitARM)

Once you have all the necessary software installed and ready to go, just navigate to the root of this project and copy/paste your ROM there. Rename the file to `BPRE0.gba`.  
Open the terminal, navigate to the root of the project, then run `python scripts/build.py 0xFFFFFFFF`, where `0xFFFFFFFF` is the free space in your ROM, and the script should insert everything, creating two new files:
- `multi.gba`: the updated ROM
- `symbols.txt`: a text file containing all the addresses known to the compiler. Conserve this file to know where everything in your ROM was inserted
