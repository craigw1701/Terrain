# Terrain

A simple terrain program in OpenGL.

The plan is to spend 4-8 hours on each area beofre moving on, allowing me to get basic versions of everything working and making it easy to change how things work as I learn more.

* Terrain
* Water
* Sky
* Grass
* Trees
* Shadows
* Post Processing effects
* Roads
* Cities/Houses
* Rain/Snow
* Day/Night
* Waterfalls
* Aurora


Looking at tutorials from:

https://www.youtube.com/user/ThinMatrix

http://www.opengl-tutorial.org/

http://nehe.gamedev.net/

Done:
* Regenerate with new seeds at runtime
 + Press "U" to switch beween threaded and single-threaded generation
 + Press "Y" to toggle caching using a std::Map (off in debug as it's slower)
 + Press "-" to generate using a new seeds
 + Dual core time for 16 tiles goes from 1.4seconds to 0.9seconds
* Fonts - 14/07/2017
 + Font Loading an Rendering working
 + Basic word wrapping
* Console
 + Basic Auto-Complete
 + Basic History
 + Able to add any existing variable to be changable in the Debug Console
 + Able to add any "Command" that calls a function from the Debug Console
* Fog
 + Add basic fog for Terrain, water and the skybox
* Sun
 + Added a sun texture that follows the sun light source across the sky, changing colour when it becomes dusk/dawn
* Shader Hot Reloading
 + Added basic Hot Realoding of Shaders, just saving a shader will have it reload. Also useful to recover from errors.
 
Tasks ToDo, in vague order:
* Shadows
* LOD - Geomipmapping
* Gamestates (Load all systems first, then load the "level")
* Mouse picking
* Level "editor" (placing objects)
* Saving/Loading
* Better Model/Texture/Material loading
* Actual GUI (buttons, sliders)
* Particle Effect system
* Snow/Rain/changing weather
* God Rays
* Bloom/HDR
* Global Ilumination
* Geometry Shaders
* Lens Flare
* Resource Manager (fonts, objects, etc - make assets?)
* Debug Screens (for showing real-time information, i.e. rendering information)

* Procedrual Trees (fractal, l-systems) - https://www.youtube.com/watch?v=-wiverLQl1Q&list=PLRqwX-V7Uu6bXUJvjnMWGU5SmjhI-OXef
* Procedrual Grass

* (Volumetric) Clouds (perlin noise?)
* Waves
* Rivers
* Waterfalls

* Shader Base Classes or Components to stop so much duplication?

* Wind (noise based?)

* Volumentic Terrain Generation - https://developer.nvidia.com/gpugems/GPUGems3/gpugems3_ch01.html

* Nav-Mesh
* AI Navigation

* First Person Camera

* Fonts/Text
	+ Better word formatting, wrapping
* Console
	+ Better console variable work

* Improve day/night
* Birds - Boids?

// Other things of interest
* Flow Fields
* Neural Networks
* Genetic Algorithms
* N-Grams and Markov Chains
* Twitter bot - https://www.youtube.com/playlist?list=PLRqwX-V7Uu6atTSxoRiVnSuOn6JHnq2yV


Temp Cam posiion
-1790.22 169.96 -2122.62
2.9 502 0

-1705.92 385.123
-2342.868

18 150 0
