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

	double aspectRatio = (double) imageWidth / imageHeight;

	std::cout << imageHeight << " " << imageWidth << " " << aspectRatio << std::endl;

	// TODO: set max and min for fov
	double fovFactor = glm::tan(glm::radians(fovy)/2.0);

	std::cout << fovFactor << std::endl;

	double xFactor = aspectRatio * fovFactor;

	// TODO: Assume camera pointing directly down z axis for now
	// TODO: get side vec by cross product of up vector and direction of view vector
	glm::vec3 viewDirection = view - eye;
	glm::vec3 side = glm::cross(viewDirection, up);
	glm::vec3 newUp = glm::cross(side, viewDirection);

	std::cout << "side: " << glm::to_string(side) << std::endl;
	std::cout << "newUp: " << glm::to_string(newUp) << std::endl;
	std::cout << "viewDirection: " << glm::to_string(viewDirection) << std::endl;

	viewDirection = glm::normalize(viewDirection);
	side = glm::normalize(side);
	newUp = glm::normalize(newUp);

	std::cout << "side: " << glm::to_string(side) << std::endl;
	std::cout << "newUp: " << glm::to_string(newUp) << std::endl;
	std::cout << "viewDirection: " << glm::to_string(viewDirection) << std::endl;

	// TODO: construct a camera-to-world transformation matrix, so that the camera can look from any angle
	glm::mat4 cameraToWorldMat = glm::mat4(1);
	// Remeber, it's [col][row]
	// Set translation 
	cameraToWorldMat[3][0] = eye.x;
	cameraToWorldMat[3][1] = eye.y;
	cameraToWorldMat[3][2] = eye.z;

	// Set rotation
	// side
	cameraToWorldMat[0][0] = side.x;
	cameraToWorldMat[0][1] = side.y;
	cameraToWorldMat[0][2] = side.z;

	// up
	cameraToWorldMat[1][0] = newUp.x;
	cameraToWorldMat[1][1] = newUp.y;
	cameraToWorldMat[1][2] = newUp.z;

	// view direction
	cameraToWorldMat[2][0] = viewDirection.x;
	cameraToWorldMat[2][1] = viewDirection.y;
	cameraToWorldMat[2][2] = viewDirection.z;

	std::cout << "cameraToWorld: " << glm::to_string(cameraToWorldMat) << std::endl;


	for (int y = 0; y < imageHeight; ++y) {
		for (int x = 0; x < imageWidth; ++x) {

			// Transform the pixel in raster space to a pixel in screen space
			//glm::vec3 pixelLoc = view + (x - imageWidth/2)*side + (y - imageHeight/2)*up;
			// TODO: convert raster calculations into a raster-to-camera matrix
			glm::vec3 pixelLocation = glm::vec3( (2.0*((x+0.5)/imageWidth)-1.0) * xFactor , (1.0-2.0*(y+0.5)/imageHeight) * fovFactor, 1/*TODO: generalize for not looking down z axis*/ );


			pixelLocation = glm::vec3(cameraToWorldMat * glm::vec4(pixelLocation, 1));
			if (y == imageHeight/2 && x == imageWidth/2) {
				std::cout << glm::to_string(pixelLocation) << std::endl;
			}
			Ray primRay = Ray(eye, pixelLocation);
			//std::cout << "x - w/2 " << (w/2) << std::endl;
			//std::cout << "y: " << y << ", x: " << x << " " << primRay.direction.x << " " << primRay.direction.y << " " << primRay.direction.z << std::endl;

			// Check for intersection for primary ray			
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

			// TODO: remove. make a white border and cross hairs
			if (y == 0 || y == imageHeight/2 || y == imageHeight-1 || x == 0 || x == imageWidth/2 || x == imageWidth-1) {
				image(x, y, 0) = 1;
				image(x, y, 1) = 1;
				image(x, y, 2) = 1;
			}

			/*
			// Red: increasing from top to bottom
			image(x, y, 0) = (double)y / h;
			// Green: increasing from side to right
			image(x, y, 1) = (double)x / w;
			// Blue: in lower-side and upper-right corners
			image(x, y, 2) = ((y < h/2 && x < w/2)
						  || (y >= h/2 && x >= w/2)) ? 1.0 : 0.0;
			*/
		}
	}

}
