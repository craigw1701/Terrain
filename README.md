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

Tasks ToDo, in vague order:
* Fonts
* Console
* Fog
* Sun
* Shadows
* LOD
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

* Procedrual Trees (fractal, l-systems) - https://www.youtube.com/watch?v=-wiverLQl1Q&list=PLRqwX-V7Uu6bXUJvjnMWGU5SmjhI-OXef
* Procedrual Grass

* (Volumetric) Clouds (perlin noise?)
* Waves
* Rivers
* Waterfalls

* Wind (noise based?)

* Volumentic Terrain Generation - https://developer.nvidia.com/gpugems/GPUGems3/gpugems3_ch01.html

* Nav-Mesh
* AI Navigation

* Improve day/night
* Birds - Boids?

// Other things of interest
* Flow Fields
* Neural Networks
* Genetic Algorithms
* N-Grams and Markov Chains
* Twitter bot - https://www.youtube.com/playlist?list=PLRqwX-V7Uu6atTSxoRiVnSuOn6JHnq2yV
