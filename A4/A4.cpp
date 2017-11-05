#include <glm/ext.hpp>

#include "Ray.hpp"

#include "A4.hpp"

void A4_Render(
		// What to render
		SceneNode * root,

		// Image to write to, set to a given width and height
		Image & image,

		// Viewing parameters
		const glm::vec3 & eye,
		const glm::vec3 & view,
		const glm::vec3 & up,
		double fovy,

		// Lighting parameters
		const glm::vec3 & ambient,
		const std::list<Light *> & lights
) {

  // Fill in raytracing code here...

  std::cout << "Calling A4_Render(\n" <<
		  "\t" << *root <<
          "\t" << "Image(width:" << image.width() << ", height:" << image.height() << ")\n"
          "\t" << "eye:  " << glm::to_string(eye) << std::endl <<
		  "\t" << "view: " << glm::to_string(view) << std::endl <<
		  "\t" << "up:   " << glm::to_string(up) << std::endl <<
		  "\t" << "fovy: " << fovy << std::endl <<
          "\t" << "ambient: " << glm::to_string(ambient) << std::endl <<
		  "\t" << "lights{" << std::endl;

	for(const Light * light : lights) {
		std::cout << "\t\t" <<  *light << std::endl;
	}
	std::cout << "\t}" << std::endl;
	std:: cout <<")" << std::endl;

	size_t h = image.height();
	size_t w = image.width();

	// The size of the image should not be more than 1600x900 or whatever the largest screen resolution is. Definitely not even close to the INT_MAX
	int imageHeight = h;
	int imageWidth = w;

	std::cout << imageHeight << " " << imageWidth << std::endl;

	// TODO: Assume camera pointing directly down z axis for now
	// TODO: get left vec by cross product of up vector and direction of view vector
	glm::vec3 leftVec = glm::vec3(-3,0,0);
	glm::vec3 upVec = glm::vec3(0,3,0);

	for (int y = 0; y < imageHeight; ++y) {
		for (int x = 0; x < imageWidth; ++x) {

			// If y == h/2 and x == w/2, then the pixelLoc is just the view vector
			// Construct the ray for the current pixel
			glm::vec3 pixelLoc = view + (x - imageWidth/2)*leftVec + (y - imageHeight/2)*upVec;
			Ray primRay = Ray(eye, pixelLoc);
			//std::cout << "x - w/2 " << (w/2) << std::endl;
			//std::cout << "y: " << y << ", x: " << x << " " << primRay.direction.x << " " << primRay.direction.y << " " << primRay.direction.z << std::endl;

			// Check for intersection for primary ray			

			image(x, y, 0) = 0;
			image(x, y, 1) = 0;
			image(x, y, 2) = 0;
			Intersect primRayIntersect = Intersect();
			for (SceneNode * childNode : root->children) {
				Intersect curRayIntersect = childNode->checkIntersection(primRay);
				if (curRayIntersect.isHit) {
					if (!primRayIntersect.isHit || curRayIntersect.distanceHit < primRayIntersect.distanceHit) {
						primRayIntersect = curRayIntersect;
					}
				}
			}
			image(x, y, 0) = primRayIntersect.colour.r;
			image(x, y, 1) = primRayIntersect.colour.g;
			image(x, y, 2) = primRayIntersect.colour.b;

			/*
			// Red: increasing from top to bottom
			image(x, y, 0) = (double)y / h;
			// Green: increasing from left to right
			image(x, y, 1) = (double)x / w;
			// Blue: in lower-left and upper-right corners
			image(x, y, 2) = ((y < h/2 && x < w/2)
						  || (y >= h/2 && x >= w/2)) ? 1.0 : 0.0;
			*/
		}
	}

}
