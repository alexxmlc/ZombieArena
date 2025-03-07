#include "ZombieArena.h"
#include <SFML/Graphics.hpp>

int createBackground(VertexArray& rVA, IntRect arena)
{
	//Anything that is modified to rVA will be modified in the main function

	const int TILE_SIZE = 50;  //pixels
	const int TILE_TYPES = 3;  //3 dif tiles on sprite sheet
	const int VERTS_IN_QUAD = 4;  //4 vertices in a quad

	//Count the width and height of the world in nb of tiles
	int worldWidth = arena.width / TILE_SIZE;
	int worldHeight = arena.height / TILE_SIZE;

	//Set the type of primitive for VertexArray
	rVA.setPrimitiveType(Quads);

	//Set the size of the vertex array
	rVA.resize(worldHeight * worldWidth * VERTS_IN_QUAD);

	//The begginign of the array
	int currentVertex = 0;

	for (int w = 0; w < worldWidth; w++)
	{
		for (int h = 0; h < worldHeight; h++)
		{
			//Position each vertex in the current quad
			rVA[currentVertex + 0].position = Vector2f(w * TILE_SIZE, h * TILE_SIZE);
			rVA[currentVertex + 1].position = Vector2f((w * TILE_SIZE) + TILE_SIZE, h * TILE_SIZE);
			rVA[currentVertex + 2].position = Vector2f((w * TILE_SIZE) + TILE_SIZE, (h * TILE_SIZE) + TILE_SIZE);
			rVA[currentVertex + 3].position = Vector2f(w * TILE_SIZE, (h * TILE_SIZE) + TILE_SIZE);

			//Each vertex has it s coordinates set to one of the 4 corners of the wall texture in the sprite sheet
			if (h == 0 || h == worldHeight - 1 || w == 0 || w == worldWidth - 1)
			{
				rVA[currentVertex + 0].texCoords = Vector2f(0, 0 + TILE_SIZE * TILE_TYPES);
				rVA[currentVertex + 1].texCoords = Vector2f(TILE_SIZE, 0 + TILE_SIZE * TILE_TYPES);
				rVA[currentVertex + 2].texCoords = Vector2f(TILE_SIZE, TILE_SIZE + TILE_SIZE * TILE_TYPES);
				rVA[currentVertex + 3].texCoords = Vector2f(0, TILE_SIZE + TILE_SIZE * TILE_TYPES);
			}
			else
			{
				//Use a random floor texture (first 3 from the sprite sheet)
				srand((int)time(0) + h * w - h);				//seeding the random nb generator
				int mudOrGrass = (rand() % TILE_TYPES);			//initialized with a nb between 0 and TYLE_TYPES
				int verticalOffset = mudOrGrass * TILE_SIZE;	//vertical reference point to the starting height (the bottom) of the randomly chosen texture from the sprite sheet

				//We set the coordinates for each one of the corners of the quad texture sheet starting from the left-bottom one = (0, verticalOffset)
				rVA[currentVertex + 0].texCoords = Vector2f(0, 0 + verticalOffset);
				rVA[currentVertex + 1].texCoords = Vector2f(TILE_SIZE, 0 + verticalOffset);
				rVA[currentVertex + 2].texCoords = Vector2f(TILE_SIZE, verticalOffset + TILE_SIZE);
				rVA[currentVertex + 3].texCoords = Vector2f(0, verticalOffset + TILE_SIZE);
			}

			currentVertex += VERTS_IN_QUAD;
		}
	}
	return TILE_SIZE;
}