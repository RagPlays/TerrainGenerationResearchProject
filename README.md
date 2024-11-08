#  Research - TerrainGeneration - Maurice Vandenheede - 2DAEGD18

## Overview
In the beginning, I had many doubts about what kind of project I was going to choose. I came across a video by Sebastian Lague about terrain generation and found it very interesting.
So now my research project is focused on generating a realistic 2D top-down map. Faking 3D with racasting shadows. (If I had experience with a 3D engine, I would have considered using it.)

My first thought was to use the kevGen -> Win32 API engine made by Kevin Hoefman. This would have given me good performance with only a little memory usage.
But for my project, I needed a framework where manipulating every single pixel separately would be possible, so I changed my framework to the framework of my "graphic programming" lessons.
The framework itself is very simple, but I build onto it to create my research project.

## Features
### Perlin Noise Generator
The foundation of terrain generation is a Perlin noise generator. Perlin noise is used to create gradient noise, which is used to generate the initial terrain heightmap.
For creating a noise map, you need a lot of variables that can change the output of the noise map.

The most important variables are:
- Seed

  Is the same thing as Minecraft seeds. The "randomness" of the map is based on the seed, meaning that with every new seed, you get a completely different map. But with the same one, you get the same map.
  
- Zoom value

  Determines how much the map is zoomed in or out. -> mostly stored as frequency = 1.f / zoom value
  
- Number of octaves
  
  The number of levels of detail you want your perlin noise to have. (This determines a lot of how fast the map is created because it goes into a for loop.)
  
- Lacunarity
  
  Number that determines how much detail is added or removed at each octave (adjusts zoom value or frequency)

<p align="center">
  <img width="542" alt="noiseMap" src="https://github.com/Howest-DAE-GD/gpp-researchtopic-RagPlays/assets/106875652/146239cc-839f-4af3-a540-b7e31d59f3fa">
</p>

### Layer Manager
I made a manager than I can use to create all terrain layers. Each layer corresponds to a height. There is a function "GetColorByValue" with as a parameter the noisemap (height) value. This function will return the right layer color.
The height of the map is the maximum height the layer is going. So the height needs to be under that value.

Programm basic layers with color:
- Water, height = 0.4f, Color = 30, 176, 251
- Sand, height = 0.5f, Color = 255, 246, 193
- Grass, height = 0.7f, Color = 118, 239, 124
- MoutainGrass, height = 1.1f, Color = 22, 181, 141

<p align="center">
  <img width="542" alt="terrainMap" src="https://github.com/Howest-DAE-GD/gpp-researchtopic-RagPlays/assets/106875652/d1b00131-b86b-485a-bed1-6579f5d191aa">
</p>

### Raycasting Shadows
To add depth and realism to the 2D maps, I used raycasting to create shadows. I cast rays from every point on the map to a light source (mouse position) and check if this ray is inside a mountain or not.
I do this every frame for every pixel (mapPosistion), you can see that this is a heavy operation. I will say that I tried to use multithreading, but it only made my programm slower. (Meaning I did it wrong.)

<p align="center">
  <img width="542" alt="terrainMapShadows" src="https://github.com/Howest-DAE-GD/gpp-researchtopic-RagPlays/assets/106875652/a0f00abf-c2b6-4082-a26c-2a6881170e30">
</p>

### Multi-Map Rendering
For more experimentations I added the capability to render more than one map on the screen at once. This allows for side-by-side comparison of different terrains or viewing multiple aspects of the same terrain simultaneously.
From this, I learned that playing with the pixel buffer is painfully hard but also cool if it works.

<p align="center">
  <img width="542" alt="multipleDisplay" src="https://github.com/Howest-DAE-GD/gpp-researchtopic-RagPlays/assets/106875652/d643665c-e16e-4667-b5c3-717cd4f0fdbf">
</p>

## Preformance diagnostics
At first, I thought that rendering four maps at a time would make it harder for the processor. 
What I found out is that it doesn't really matter how many maps I load in and render, it's more about how big these maps are.

In the LayerShadowMap render mode it depends where the sun (mousePos) is located. When it is in the middle of the screen, the frames are on its lowest.

The window resolution is 720 x 480. Thats 345 600 pixels.

Here are some average FPS benchmarks on my computer, ofcourse this is different for every computer.

| RenderMode          | Debug              | Release            | Average Drop In %             |
|  :------------:     | :-------------     | :------------      | :----------------:            |
| NoiseMap            | FPS: 137           | FPS: 550           | ...                           |
| LayerMap            | FPS: 33            | FPS: 330           | Debug: -75% Release: -40%     |
| LayerShadowMap      | FPS: 5-6           | FPS: 54-68         | Debug: -83% Release: -81%     |

## What could I do more or better?
I have put a lot of work into this project, mostly because I enjoyed working on it. But there are some things that I left behind.
For example, I wanted to create more of a stranded on an island feel. To get that effect, I needed to create a circular gradient overlapping with the map.
More of the map in the middle of the circle and fading out further from the middle. Then you can create the effect of land in the middle and only ocean(water) on the sides.

My programm processes each pixel at a time, and that takes a lot of processing power for one core/thread. Trying to multithread this was a mistake because I only made it slower. I would say my programm runs well, but implementing it with multithreading or running it on the GPU would help a lot.

## How do I use this programm?
For clarity, these are all inputs from an AZERTY keyboard.

--------- General ---------
* Escape -> Exits the programm
* X -> Safe pixelBuffer to a .bmp file. (taking a screenshot)
* F -> Toggle show FPS

---------- Maps -----------
* N -> Creating new map(s)
* W -> Toggle between one/multiple map(s)
* R -> Toggle between render mode
* A -> add new mapLayer type in "name height r g b"(with spaces!) -> ex. rocks 0.2 96 96 96
* S -> Change seed of a map, type in "mapNumber newSeed" -> ex. 1 232
* Mouse ScrollWheel -> zoom in/out on map(s)
* Left Mouse Click/Hold -> Set sun position (only works in render mode with shadows)



