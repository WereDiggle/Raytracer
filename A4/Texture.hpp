#pragma once

#include <string>
#include <libpng>

// Class for handling UV mapping
class Texture {

    // How often the texture should repeat along the U and V axis
    double tileU;
    double tileV;

public:

    // construct the Texture from a png file
    Texture( const std::string& fname, double tileU, double tileV);


}