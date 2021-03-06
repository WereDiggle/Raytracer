#include <iostream>
#include <algorithm>
#include <cmath>
#include <cstdlib>

#include "Texture.hpp"

//-----------------------------------------------------
// Texture base class
//-----------------------------------------------------
Texture* Texture::defaultTexture = new Texture(); 

Texture::Texture() {}

Texture::~Texture() {}

// just return white, should be multiplied with actual colour to get the same colour
glm::vec3 Texture::getColour(double u, double v) 
{
    return glm::vec3(1.0);
}

// Normal doesn't change
glm::vec3 Texture::getNormal(const glm::vec3 & normal, const glm::vec3 & upV, double u, double v)
{
    return normal;
}

glm::mat4 Texture::getBumpToWorldMat(const glm::vec3 & normal, const glm::vec3 & upV)
{

	glm::vec3 side = glm::normalize(glm::cross(upV, normal));
    glm::vec3 up = glm::normalize(glm::cross(normal, side));
 
    glm::mat4 bumpToWorldMat = glm::mat4(0);

    // Remeber, it's [col][row]
    bumpToWorldMat[3][3] = 1.0f;
	// Set rotation
	// side
	bumpToWorldMat[0][0] = side.x;
	bumpToWorldMat[0][1] = side.y;
	bumpToWorldMat[0][2] = side.z;

	// up
	bumpToWorldMat[1][0] = up.x;
	bumpToWorldMat[1][1] = up.y;
	bumpToWorldMat[1][2] = up.z;

	// view direction
	bumpToWorldMat[2][0] = normal.x;
	bumpToWorldMat[2][1] = normal.y;
    bumpToWorldMat[2][2] = normal.z;

    return bumpToWorldMat;
}

//-----------------------------------------------------
// Bitmap Texture class 
//-----------------------------------------------------

BitmapTexture::BitmapTexture()
    : tileU(1.0), tileV(1.0), data(Image())
{
}

BitmapTexture::BitmapTexture(const std::string& filename, double tileU, double tileV)
    : tileU(tileU), tileV(tileV), data(Image("Assets/"+filename))
{
}

BitmapTexture::~BitmapTexture()
{
}

glm::vec3 BitmapTexture::getColour(double u, double v)
{
    // Remember that v is upside down
    double intPart;
    double texU = glm::clamp(u, 0.0, 1.0) * tileU;
    double texV = (1.0 - glm::clamp(v, 0.0, 1.0)) * tileV;
    texU = std::modf(texU, &intPart);
    texV = std::modf(texV, &intPart);

    int indexU = data.width() * texU; 
    int indexV = data.height() * texV; 

    return glm::vec3(data(indexU,indexV,0), data(indexU,indexV,1), data(indexU, indexV, 2));
}

//-----------------------------------------------------
// Bumpmap Texture class 
//-----------------------------------------------------

BumpmapTexture::BumpmapTexture()
    : tileU(1.0), tileV(1.0), data(Image())
{
}

BumpmapTexture::BumpmapTexture(const std::string& filename, double tileU, double tileV, double depth)
    : tileU(tileU), tileV(tileV), depth(depth), data(Image("Assets/"+filename))
{
}

BumpmapTexture::~BumpmapTexture()
{
}

double BumpmapTexture::getBumpHeight(int u, int v) 
{
    // any height beyond the 0 or width/height just gets the boundary height
    int indexU = glm::clamp(u, 0, (int) data.width());
    int indexV = glm::clamp(v, 0, (int) data.height());
    double height = (data(indexU, indexV, 0) + data(indexU, indexV, 1) + data(indexU, indexV, 2))/3.0 * depth;
}

// TODO: make this return the changed normal
glm::vec3 BumpmapTexture::getNormal(const glm::vec3 & normal, const glm::vec3 & upV, double u, double v)
{
    // Remember that v is upside down
    double intPart;
    double texU = glm::clamp(u, 0.0, 1.0) * tileU;
    double texV = (1.0 - glm::clamp(v, 0.0, 1.0)) * tileV;
    texU = std::modf(texU, &intPart) * data.width();
    texV = std::modf(texV, &intPart) * data.height();

    int indexU = glm::floor(texU); 
    int indexV = glm::floor(texV); 

    texU = std::modf(texU, &intPart);
    texV = std::modf(texV, &intPart);

    double heightA, heightB, heightC;
    glm::vec3 AB, AC, bumpNormal;
    // Determine which triangle to use
    // Calculate the normal from a triangle formed by three points
    if (texU + texV <= 1.0) { //std::cout << "lower left triangle                                   " << std::endl;
        heightA = getBumpHeight(indexU, indexV);
        heightB = getBumpHeight(indexU, indexV-1);
        heightC = getBumpHeight(indexU+1, indexV);
        //std::cout << "A: " << heightA << ", B: " << heightB << ", C: " << heightC << std::endl;
        //std::cout << "texU: " << texU << ", texV: " << texV << std::endl;
        AB = glm::vec3(0, 1, heightB) - glm::vec3(0, 0, heightA);
        AC = glm::vec3(1, 0, heightC) - glm::vec3(0, 0, heightA);
    }
    else {
        //std::cout << "upper right triangle                                  " << std::endl;
        heightA = getBumpHeight(indexU+1, indexV-1);
        heightB = getBumpHeight(indexU+1, indexV);
        heightC = getBumpHeight(indexU, indexV-1);
        //std::cout << "A: " << heightA << ", B: " << heightB << ", C: " << heightC << std::endl;
        //std::cout << "texU: " << texU << ", texV: " << texV << std::endl;
        AB = glm::vec3(1, 0, heightB) - glm::vec3(1, 1, heightA);
        AC = glm::vec3(0, 1, heightC) - glm::vec3(1, 1, heightA);
    }

    // TODO: really test the order of these cross products
    bumpNormal = glm::normalize(glm::cross(AC, AB));

    // Combine the bumpNormal with the given normal
	glm::vec3 side = glm::normalize(glm::cross(upV, normal));
    glm::vec3 up = glm::normalize(glm::cross(normal, side));
 
    //std::cout << "side: " << glm::to_string(side) << ", up: " << glm::to_string(up) << std::endl;
   
	glm::mat4 bumpToWorldMat = glm::mat4(0);
    // Remeber, it's [col][row]
    bumpToWorldMat[3][3] = 1.0f;
	// Set rotation
	// side
	bumpToWorldMat[0][0] = side.x;
	bumpToWorldMat[0][1] = side.y;
	bumpToWorldMat[0][2] = side.z;

	// up
	bumpToWorldMat[1][0] = up.x;
	bumpToWorldMat[1][1] = up.y;
	bumpToWorldMat[1][2] = up.z;

	// view direction
	bumpToWorldMat[2][0] = normal.x;
	bumpToWorldMat[2][1] = normal.y;
    bumpToWorldMat[2][2] = normal.z;

    return glm::normalize(glm::vec3(bumpToWorldMat * glm::vec4(bumpNormal, 0)));
}

//-----------------------------------------------------
// Noise Texture class 
//-----------------------------------------------------

NoiseTexture::NoiseTexture() 
{
    NoiseTexture(256,256);
}

NoiseTexture::NoiseTexture(int width, int height)
    : latticeWidth(width), latticeHeight(height)
{
    srand(420);

    // TODO: implement 2d, and then 3d, but for now, just 1d
    for (int i = 0; i<256; ++i) {
        float r = static_cast <float> (rand()) / static_cast <float> (RAND_MAX);
        float g = static_cast <float> (rand()) / static_cast <float> (RAND_MAX);
        float b = static_cast <float> (rand()) / static_cast <float> (RAND_MAX);
        randomColour.push_back(glm::vec3(r,g,b));
        permutationTable.push_back(i);
    }

    // Shuffle our permutation table
    std::vector<int> secondPermutationTable = permutationTable; 

    std::random_shuffle(permutationTable.begin(), permutationTable.end());
    std::random_shuffle(secondPermutationTable.begin(), secondPermutationTable.end());
    permutationTable.insert(permutationTable.end(), secondPermutationTable.begin(), secondPermutationTable.end());

}

NoiseTexture::~NoiseTexture()
{
}

glm::vec3 NoiseTexture::getFractalSumColour(double u, double v, int layers)
{
    glm::vec3 noiseTotal = glm::vec3(0);
    double noiseMax = 0;
    for (int i=0; i<layers; ++i) {
        double amplitude = glm::exp2((double)-i);
        noiseTotal += amplitude * getRandomColour(u, v, glm::exp2((double)i));
        noiseMax += amplitude;
    }

    return noiseTotal / noiseMax;
}

glm::vec3 NoiseTexture::getRandomColour(double u, double v, int frequency)
{
    double texU = latticeWidth * frequency * glm::clamp(u, 0.0, 1.0);
    double texV = latticeHeight * frequency * glm::clamp(v, 0.0, 1.0);

    double floorU, floorV;
    double tU = glm::smoothstep(0.0, 1.0, std::modf(texU, &floorU));
    double tV = glm::smoothstep(0.0, 1.0, std::modf(texV, &floorV));

    int indexU = (int) floorU;
    int indexV = (int) floorV;

    glm::vec3 topLeft = getRandomColour(indexU, indexV+1);
    glm::vec3 topRight = getRandomColour(indexU+1, indexV+1);
    glm::vec3 bottomLeft = getRandomColour(indexU, indexV);
    glm::vec3 bottomRight = getRandomColour(indexU+1, indexV);

    return mix2D(topLeft, topRight, bottomLeft, bottomRight, tU, tV);
}

// u and v will wrap instead of clamp to the right range
glm::vec3 NoiseTexture::getRandomColour(int u, int v, int frequency)
{
    int indexU = (u % (latticeWidth * frequency)) % 256;
    int indexV = (v % (latticeHeight * frequency)) % 256;
    return randomColour[permutationTable[permutationTable[indexU] + indexV]];
}

// Linearly interpolates the colour value between 4 points in a square
glm::vec3 NoiseTexture::mix2D(const glm::vec3 & topLeft, 
                              const glm::vec3 & topRight, 
                              const glm::vec3 & bottomLeft, 
                              const glm::vec3 & bottomRight, 
                              double tx, double ty)
{
    glm::vec3 top = glm::mix(topLeft, topRight, tx);
    glm::vec3 bottom = glm::mix(bottomLeft, bottomRight, tx);

    glm::vec3 center = glm::mix(bottom, top, ty);

    // TODO: just use the r value for now, It's a huge waste 2f space, but whatever.
    return glm::vec3(center.r);
}

glm::vec3 NoiseTexture::getColour(double u, double v)
{
    double texU = latticeWidth * glm::clamp(u, 0.0, 1.0);
    double texV = latticeHeight * glm::clamp(v, 0.0, 1.0);

    double floorU, floorV;
    double tU = glm::smoothstep(0.0, 1.0, std::modf(texU, &floorU));
    double tV = glm::smoothstep(0.0, 1.0, std::modf(texV, &floorV));

    int indexU = (int) floorU;
    int indexV = (int) floorV;

    glm::vec3 topLeft = getRandomColour(indexU, indexV+1);
    glm::vec3 topRight = getRandomColour(indexU+1, indexV+1);
    glm::vec3 bottomLeft = getRandomColour(indexU, indexV);
    glm::vec3 bottomRight = getRandomColour(indexU+1, indexV);

    // 1d: return glm::mix(randomColour[indexU], randomColour[(indexU+1)%latticeWidth], glm::smoothstep(0.0, 1.0, tU));
    // 2d: return mix2D(topLeft, topRight, bottomLeft, bottomRight, tU, tV);
    return glm::vec3(getFractalSumColour(u, v, 7).r);
}

//-----------------------------------------------------
// Ripple Texture class 
// TODO: try this maybe z=(cos( 0.5sqrt(x^2+y^2)-6n)/(0.5(x^2+y^2)+1+2n), n={0...10} 
//      Courtesy of youtube video
// TODO: try this maybe (a/(1+r)) * cos((b/log(r+2))*r), where a and b are constants, and r is the distance from the center of the ripple
//      Thanks reddit
//-----------------------------------------------------

RippleTexture::RippleTexture()
    : centerU(0.5), centerV(0.5), waveHeight(1) 
{
}

RippleTexture::RippleTexture(double centerU, double centerV, double scaleU, double scaleV, double waveHeight)
    : centerU(glm::clamp(centerU, 0.0, 1.0)), centerV(glm::clamp(centerV, 0.0, 1.0)), scaleU(scaleU), scaleV(scaleV), waveHeight(waveHeight) 
{
}

RippleTexture::~RippleTexture() {}

double RippleTexture::getDerivative(double t)
{
    // TODO: Right now just use cos(t), whose derivative is -sin(t)
    return waveHeight * -glm::sin(t);
}

glm::vec3 RippleTexture::getNormal(const glm::vec3 & normal, const glm::vec3 & upV, double u, double v)
{
    // Getting the derivative might be harder with more complex functions
    // TODO: remember how to do derivatives
    glm::dvec3 center = glm::dvec3(scaleU*centerU, scaleV*centerV, 0.0);
    glm::dvec3 uv = glm::dvec3(scaleU*u, scaleV*v, 0.0) - center;
    glm::dvec3 uvDirection = glm::normalize(uv);

    double t = glm::length(uv); 
    double dw_dt = getDerivative(t);

    // Get 3d direction of derivative at u and v
    glm::dvec3 dw_dtDirection = glm::normalize(glm::dvec3(uvDirection.x, uvDirection.y, dw_dt));

    // Cross with direction of (u,v,0) to get a side vector
    // This side vector needs always be the same sign
    glm::dvec3 uvSide = glm::cross(uvDirection, dw_dtDirection);

    // Cross side vector with derivative to get normal, ie perpendicular, on the same plane as the direction of (u,v)
    glm::dvec3 waveNormal = glm::normalize(glm::cross(uvSide, dw_dtDirection));

    // Surface should not be perturbed inside out
    if (glm::dot(waveNormal, glm::dvec3(0,0,1.0)) < 0) {
        //std::cout << "wave normal is pointing downwards" << std::endl;
        waveNormal = -waveNormal;
    }

    return glm::normalize(glm::vec3(getBumpToWorldMat(normal, upV) * glm::vec4(waveNormal, 0)));
}