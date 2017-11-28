#include <stdlib.h>
#include <iomanip>
#include <ctime>

#include <glm/ext.hpp>

#include "Ray.hpp"
#include "PhotonMap.hpp"

#include "A4.hpp"

#define SUPER_SAMPLING 1

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
		const std::list<Light *> & lights,

		// debug parameters
		int debugX,
		int debugY
) {

  // Fill in raytracing code here...
  int start_time = clock();

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

	if (debugX >= 0 && debugY >= 0) {
		std::cout << "\t" << "debugX: " << debugX << std::endl;
		std::cout << "\t" << "debugY: " << debugY << std::endl;
	}

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

	glm::vec3 viewDirection = view - eye;
	glm::vec3 side = glm::cross(viewDirection, up);
	glm::vec3 newUp = glm::cross(side, viewDirection);

	viewDirection = glm::normalize(viewDirection);
	side = glm::normalize(side);
	newUp = glm::normalize(newUp);

	std::cout << "side: " << glm::to_string(side) << std::endl;
	std::cout << "newUp: " << glm::to_string(newUp) << std::endl;
	std::cout << "viewDirection: " << glm::to_string(viewDirection) << std::endl;

	glm::mat4 cameraToWorldMat = glm::mat4(0);
	// Remeber, it's [col][row]
	// Set translation 
	cameraToWorldMat[3][0] = eye.x;
	cameraToWorldMat[3][1] = eye.y;
	cameraToWorldMat[3][2] = eye.z;
	cameraToWorldMat[3][3] = 1.0f;

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

	srand(451);

	// Set up debug variables
	int xStart = 0;
	int yStart = 0;

	int xEnd = imageWidth;
	int yEnd = imageHeight;

	if (debugX >=0 && debugY >= 0) {
		xStart = debugX;
		yStart = debugY;
		xEnd = xStart+1;
		yEnd = yStart+1;
	}


	// Generate the background
	for (int y = yStart; y < yEnd; ++y) {
		for (int x = xStart; x < xEnd; ++x) {

			int prob = rand() % 100;
			int r = rand() % 100;
			int g = rand() % 100;
			int b = rand() % 100;

			if (prob < 2) {
				image(x, y, 0) = r/100.0f;
				image(x, y, 1) = g/100.0f;
				image(x, y, 2) = b/100.0f;
			}
			else {
				image(x, y, 0) = 0;
				image(x, y, 1) = 0;
				image(x, y, 2) = 0;
			}
		}
	}

	// TODO: Photon mapping
	// PhotonMap testPhotonMap = PhotonMap(100);
	// testPhotonMap.emitLight(root);
	// return;


	std::cout << "RAY TRACER START" << std::endl << std::endl;

	for (int y = yStart; y < yEnd; ++y) {
		for (int x = xStart; x < xEnd; ++x) {

			// Sampling. Not actually n^2 since SUPER_SAMPLING should be a small constant number
			// Will make it run a lot slower though :(
			glm::vec3 sampledPixels[SUPER_SAMPLING*SUPER_SAMPLING];

			bool useBackground = true;

			//std::cout << std::endl;

			for (int ys = 0; ys < SUPER_SAMPLING; ++ys) {
				for (int xs = 0; xs < SUPER_SAMPLING; ++xs) {

					// Transform the pixel in raster space to a pixel in screen space
					//glm::vec3 pixelLoc = view + (x - imageWidth/2)*side + (y - imageHeight/2)*up;
					// TODO: convert raster calculations into a raster-to-camera matrix
					glm::vec3 pixelLocation = glm::vec3( (2.0*((SUPER_SAMPLING*x+xs+0.5)/(SUPER_SAMPLING*imageWidth))-1.0) * xFactor , (1.0-2.0*(SUPER_SAMPLING*y+ys+0.5)/(SUPER_SAMPLING*imageHeight)) * fovFactor, 1);

					//std::cout << "pixelLocation: " << glm::to_string(pixelLocation) << std::endl;

					pixelLocation = glm::vec3(cameraToWorldMat * glm::vec4(pixelLocation, 1));
					Ray primRay = Ray(eye, pixelLocation);

					// Check for intersection for primary ray			
					Intersect primRayIntersect = root->castRay(primRay);

					// Lighting Calculations
					if (primRayIntersect.isHit && primRayIntersect.material != nullptr) {

						// Ambient light
						useBackground = false;

						glm::vec3 totalLighting = primRayIntersect.getLighting(ambient, lights, root);

						// Store the sampled pixels
						sampledPixels[ys*SUPER_SAMPLING+xs] = totalLighting;
					}
				}
			}

			// Average sampled pixels
			if (!useBackground) {
				glm::vec3 averageColour = glm::vec3(0);
				for (int pix = 0; pix<SUPER_SAMPLING*SUPER_SAMPLING; ++pix) {
					averageColour += sampledPixels[pix];
				}
				averageColour = averageColour/(SUPER_SAMPLING*SUPER_SAMPLING);

				image(x, y, 0) = averageColour.r;
				image(x, y, 1) = averageColour.g;
				image(x, y, 2) = averageColour.b;
			}

			// Progress calculations
			int intermediate_time = clock();
			std::cout << std::setfill('0') << std::setw(5) << std::fixed << std::setprecision(2);
			std::cout << "Progress: " << (100.0*(y*imageWidth + x))/(imageHeight*imageWidth) << "%";
			std::cout << " [Elapsed Time: " << (intermediate_time - start_time)/double(CLOCKS_PER_SEC) << " seconds] \t" << '\r';
			std::cout.flush();

		}
	}
	std::cout << std::endl;
	int stop_time = clock();
	std::cout << "ray tracing took: " << (stop_time - start_time)/double(CLOCKS_PER_SEC) << " seconds" << std::endl;

}
