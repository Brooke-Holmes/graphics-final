#pragma once
#include "../ew/ewMath/ewMath.h"
#include <random>

//https://www.youtube.com/watch?v=4066MndcyCk
//Although it was for a javascipt implementation, this video was helpful in understanding worley noise

static std::random_device rd;
static std::mt19937 rng(rd());
namespace anm {
	class Noise
	{
	public:
		Noise() {}
		Noise(int numPoints, float width, float height) //for voronoi
		{
			createPoints(numPoints, -(width / 2.0f), (width / 2.0f), -(height / 2.0f), (height / 2.0f));
		}
		Noise(float width, int segments)//for worley
		{
			createGridPoints( -(width / 2.0f), (width / 2.0f), segments);
		}
		std::vector<ew::Vec2> getPoints() { return points; }

	private:
		void createPoints(const int numPoints, int xMin, int xMax, int yMin, int yMax) //generates points randomly in an area
		{
			for (int i = 0; i < numPoints; i++)
			{
				std::uniform_int_distribution<> distribX(xMin, xMax);
				int xVal = distribX(rng);
				std::uniform_int_distribution<> distribY(yMin, yMax);
				int yVal = distribY(rng);
				points.push_back(ew::Vec2(xVal, yVal));
			}
		}

		void createGridPoints(int min, int max, int segments) //assumes the plane is square, generates one point per grid square
		{
			int box = (max - min) / segments; //width of each square
			//box = -box / 2 + box;
			//build a grid of random points (one point per square)
			for (int x = 0; x < segments; x++)
			{
				float xcoord = min + x * box;
				std::uniform_int_distribution<> distribX( xcoord, xcoord + box);
				int xVal = distribX(rng);
				for (int y = 0; y < segments; y++) {
					//-width / 2 + width * v.uv.x
					float ycoord = min + y * box;
					std::uniform_int_distribution<> distribY(ycoord, ycoord + box);
					int yVal = distribY(rng);
					points.push_back(ew::Vec2(xVal, yVal));
				}
			}
		}

		std::vector<ew::Vec2> points;
	};

}