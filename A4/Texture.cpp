#include <iostream>
#include <cmath>

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

//-----------------------------------------------------
// Bitmap Texture class 
//-----------------------------------------------------

BitmapTexture::BitmapTexture()
    : tileU(1.0), tileV(1.0), data(Image())
{
}

BitmapTexture::BitmapTexture(const std::string& filename, double tileU, double tileV)
    : tileU(tileU), tileV(tileV), data(Image(filename))
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
// TODO: Actually implement
//-----------------------------------------------------

BumpmapTexture::BumpmapTexture()
    : tileU(1.0), tileV(1.0), data(Image())
{
}

BumpmapTexture::BumpmapTexture(const std::string& filename, double tileU, double tileV, double depth)
    : tileU(tileU), tileV(tileV), data(Image(filename))
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
    if (texU + texV <= 1.0) {
        //std::cout << "lower left triangle                                   " << std::endl;
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
	glm::vec3 side = glm::normalize(glm::cross(normal, upV));
    glm::vec3 up = glm::normalize(glm::cross(side, normal));
    
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

    return glm::vec3(bumpToWorldMat * glm::vec4(bumpNormal, 0));
}
