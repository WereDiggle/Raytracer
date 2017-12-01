#include <iostream>
#include <glm/ext.hpp>
#include <glm/gtx/component_wise.hpp>

#include "PhongMaterial.hpp"

PhongMaterial::PhongMaterial(
	const glm::vec3& kd, const glm::vec3& ks, double shininess)
	: m_kd(kd)
	, m_ks(ks)
	, m_shininess(shininess)
{}

PhongMaterial::~PhongMaterial()
{
}

glm::vec3 PhongMaterial::getColour() {
	return m_kd;
}

static glm::vec3 blend(const glm::vec3 & a, const glm::vec3 b) {
    return glm::vec3(a.r * b.r, a.g * b.g, a.b * b.b);
}

// Stands for bidirectional reflectance distribution function
glm::vec3 PhongMaterial::BRDF(const glm::vec3 & surfaceNormal, const glm::vec3 & lightDirection, const glm::vec3 & viewDirection) {

	double lightSurfaceDot = glm::dot(lightDirection, surfaceNormal);

	// If the dot product is negative, then the light does not hit the surface
	if (lightSurfaceDot <= 0) {
		return glm::vec3(0, 0, 0);
	}

	glm::vec3 reflectDirection = glm::normalize( 2*lightSurfaceDot*surfaceNormal - lightDirection );

	double specularFactor = (glm::pow(glm::dot(reflectDirection, viewDirection), m_shininess))/lightSurfaceDot;

	return specularFactor*m_ks;
}

glm::vec3 PhongMaterial::getLighting(const glm::vec3 & surfaceNormal, 
									 const glm::vec3 & lightDirection, const glm::vec3 & lightIntensity, double lightDistance, double * lightFalloff,
									 const glm::vec3 & viewDirection,
									 double textureU, double textureV, Texture * bitmap, Texture * bumpmap) {

	double lightSurfaceDot = glm::dot(lightDirection, surfaceNormal);

	if (lightSurfaceDot <= 0) {
		return glm::vec3(0, 0, 0);
	}

	glm::vec3 specularStuff = BRDF(surfaceNormal, lightDirection, viewDirection);

	glm::vec3 textureColour = bitmap->getColour(textureU, textureV);

	glm::vec3 phongStuff = blend(textureColour, m_kd) + specularStuff;

	double falloffFactor = lightSurfaceDot/(lightFalloff[0] + lightFalloff[1]*lightDistance + lightFalloff[2]*lightDistance*lightDistance);

	//std::cout << "surfaceNormal: " << glm::to_string(surfaceNormal) << std::endl;
	//std::cout << "lightDirection: " << glm::to_string(lightDirection) << std::endl;
	//std::cout << "viewDirection: " << glm::to_string(viewDirection) << std::endl; 
	//std::cout << "reflectDirection: " << glm::to_string(reflectDirection) << std::endl;
	//std::cout << "reflectViewDot: " << glm::dot(reflectDirection, viewDirection) << std::endl;
	//std::cout << "specularFactor: " << specularFactor << std::endl;
	//std::cout << "lightSurfaceDot: " << lightSurfaceDot << std::endl;
	//std::cout << "phongStuff: " << glm::to_string(phongStuff) << std::endl;
	//std::cout << "lightIntensity: " << glm::to_string(lightIntensity) << std::endl;
	//std::cout << "lightDistance: " << lightDistance << std::endl << std::endl;

	return glm::vec3(phongStuff.r * lightIntensity.r * falloffFactor, 
					 phongStuff.g * lightIntensity.g * falloffFactor,
					 phongStuff.b * lightIntensity.b * falloffFactor);
}