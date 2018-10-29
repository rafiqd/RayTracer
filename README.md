# Physically Based Ray Tracer

Physically Based Renderer based on "Physically Based Rendering" and Peter Shirley's Books

I work on this in my off time on weekends mostly, the goal is to implement various rendering techniques in the 
book "Physically Based Rendering" and SIGGRAPH papers to better understand them.

## Example Output
![alt text](https://github.com/rafiqd/RayTracer/blob/master/cornell_100spp.png)

`Render Time (100SPP):` 1m 42s on 11 Cores on an i7 8700k @ 4.7Ghz

## Currently implements:
* Multithreaded Rendering
* Russian Roulette Termination
* Multiple Importance Sampling
  
## Current Implementation Goals:
* Reading PBRT scenes
* Next Event Estimation
* Bidirectional Ray Tracing
  
## Usage
`./renderer <output filepath>`

Doesn't currently read any scenes, just has a hard coded cornel box for testing.
Writes a PPM image file

