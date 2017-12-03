#pragma once

// Defines the n by n super sampling rate. 1 is normal, no supersampling
#define SUPER_SAMPLING 1

// Defines how many photons to use
#define NUM_PHOTONS 1000000

// Defines how much to scale the power of a light for photon mapping
#define PHOTON_LIGHT_SCALE 80000

// Defines how many photons get gathered around a point
#define PHOTON_GATHER_NUM 60

// Defines the max range for photon gather
#define PHOTON_GATHER_RANGE 5.0

// Defines the minimum number of times a photon will bounce around
#define MIN_PHOTON_BOUNCE 3

// How sharp caustics are
#define CAUSTIC_SHARPNESS 4

// More photons than this are required for caustic calculations, otherwise, we ignore
// This is to avoid random bright spots not even close to the main body of caustics
#define MIN_CAUSTIC_PHOTONS 2

// How much direct light affects the scene
#define DIRECT_LIGHT_SCALE 0.5

// Max recursion depth for ray casting
#define MAX_DEPTH 20

// Stop raycast recursion if the colour contribution is less than this
#define MIN_CONTRIBUTION 0.01

// Whether to use photon mapping global lighting
#define PHOTONS_GLOBAL true 

// Whether to use photon mapping caustic lighting
#define PHOTONS_CAUSTIC true 

// Whether to use direct lighting
#define DIRECT_LIGHTING true 
