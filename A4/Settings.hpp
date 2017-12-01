#pragma once

// Defines the n by n super sampling rate. 1 is normal, no supersampling
#define SUPER_SAMPLING 1

// Defines how many photons to use
#define NUM_PHOTONS 0

// Defines how much to scale the power of a light for photon mapping
#define PHOTON_LIGHT_SCALE 10000

// Defines how many photons get gather around a point
#define PHOTON_GATHER_NUM 20

// Defines the max range for photon gather
#define PHOTON_GATHER_RANGE 4.0

// Max recursion depth for ray casting
#define MAX_DEPTH 10

// Stop raycast recursion if the colour contribution is less than this
#define MIN_CONTRIBUTION 0.01

// Whether to show only photons, and not direct lighting
#define PHOTONS_ONLY false
