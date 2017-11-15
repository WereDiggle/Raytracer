#include <iostream>
#include <glm/ext.hpp>

#include "PhongMaterial.hpp"

PhongMaterial::PhongMaterial(
	const glm::vec3& kd, const glm::vec3& ks, double shininess )
	: m_kd(kd)
	, m_ks(ks)
	, m_shininess(shininess)
	, testTexture(Image("nonhier.png"))
{}

PhongMaterial::~PhongMaterial()
{}

glm::vec3 PhongMaterial::getColour() {
	// Just the diffuse for now
	return m_kd;
}

glm::vec3 PhongMaterial::getLighting(const glm::vec3 & surfaceNormal, 
									 const glm::vec3 & lightDirection, const glm::vec3 & lightIntensity, double lightDistance, double * lightFalloff,
									 const glm::vec3 & viewDirection,
									 double textureU, double textureV) {

	// TODO: generalize, we know that the dimensions of the image are 512 by 512 in this case

	int u = 256 * textureU;
	int v = 256 * (1.0 - textureV);

	glm::vec3 textureColour = glm::vec3(testTexture(u, v, 0), testTexture(u, v, 1), testTexture(u, v, 2));

	double lightSurfaceDot = glm::dot(lightDirection, surfaceNormal);

	// If the dot product is negative, then the light does not hit the surface
	if (lightSurfaceDot <= 0) {
		return glm::vec3(0, 0, 0);
	}
	
	glm::vec3 reflectDirection = glm::normalize( 2*lightSurfaceDot*surfaceNormal - lightDirection );

	double specularFactor = (glm::pow(glm::dot(reflectDirection, viewDirection), m_shininess))/lightSurfaceDot;

	/*
	glm::vec3 phongStuff = glm::vec3(m_kd.r + m_ks.r * specularFactor, 
									 m_kd.g + m_ks.g * specularFactor, 
									 m_kd.b + m_ks.b * specularFactor);
									 */
	glm::vec3 phongStuff = glm::vec3(textureColour.r + m_ks.r * specularFactor, 
									 textureColour.g + m_ks.g * specularFactor, 
									 textureColour.b + m_ks.b * specularFactor);

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

	/*
	return glm::vec3(phongStuff.r * lightIntensity.r * falloffFactor, 
					 phongStuff.g * lightIntensity.g * falloffFactor,
					 phongStuff.b * lightIntensity.b * falloffFactor);
					 */
	return textureColour;
}