#pragma once
#include "../ew/ewMath/ewMath.h"
#include <random>

//function for making worley noise if I can figure it out
//place in a bunch of points
//for every point see how close the pixel is to it and calculate the distance between them (Euclidean distance)
//I need to figure out how to either put it into a texture or add it to the shader 
//what if I figure out how to do it on line segments to get that oval/long shape that they have

namespace anm {
	static std::random_device rd;
	static std::mt19937 rng(rd());
	ew::Vec2* createPoints(const int numPoints, int xMin, int xMax, int yMin, int yMax) 
	{
		ew::Vec2* points = new ew::Vec2[numPoints];
		for (int i = 0; i < numPoints; i++)  
		{
			std::uniform_int_distribution<> distribX(xMin, xMax);
			int xVal = distribX(rng);
			std::uniform_int_distribution<> distribY(yMin, yMax);
			int yVal = distribY(rng);
			points[i] = ew::Vec2(xVal, yVal);
		}
		return points;
	}

	std::vector<float> calcDistance(std::vector<ew::Vec3> vertices, ew::Vec2* points)
	{
		std::vector<float> distance;
		for (int i = 0; i < vertices.size(); i++) 
		{
			float xDist = vertices[i].x - points[0].x;
			float yDist = vertices[i].y - points[0].y;
			float dist = sqrt((pow(xDist,2.0f) + pow(yDist, 2.0f)));
			for (int j = 1; j < sizeof(points); j++)
			{
				xDist = vertices[i].x - points[j].x;
				yDist = vertices[i].y - points[j].y;
				float newDist = sqrt((pow(xDist, 2.0f) + pow(yDist, 2.0f)));
				if (newDist < dist) { dist = newDist; }
			}
			distance.push_back(dist);
		}
		return distance;
	}

	/*void skewPoints() 
	{
		
	}*/
}

//https://catlikecoding.com/unity/tutorials/pseudorandom-noise/simplex-noise/
//https://muugumuugu.github.io/bOOkshelF/generative%20art/simplexnoise.pdf
