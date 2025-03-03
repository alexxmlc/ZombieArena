#include <SFML/Graphics.hpp>
#include <iostream>
#include "Player.h"
#include "ZombieArena.h"

using namespace sf;

int main()
{
	
	//The game will be in one of these 4 states
	enum class State { PAUSED, LEVELING_UP, GAME_OVER, PLAYING };

	//Start with the game over state
	State state = State::GAME_OVER;

	//Get the screen resolution and  create an SFML window
	Vector2f resolution;
	resolution.x = VideoMode::getDesktopMode().width;
	resolution.y = VideoMode::getDesktopMode().height;

	RenderWindow window(VideoMode(resolution.x, resolution.y), "Zombie Arena", Style::Fullscreen);

	//Create an smfl view for the main action
	View mainView(sf::FloatRect(0, 0, resolution.x, resolution.y));

	//For measuring time
	Clock clock;

	//The time that the game has been in PLAYING state
	Time timeGameTotal;

	//Mouse position in relation to world coordinates
	Vector2f mouseWorldPosition;

	//Mouse position in relation to screen coordinates
	Vector2i mouseScreenPosition;

	//Create the player (instance of player class)
	Player player;

	//Arena boundaries
	IntRect arena;

	//Create the background and load the texture for it
	VertexArray background;
	Texture textureBackgrund;
	if (!textureBackgrund.loadFromFile("graphics/background_sheet.png")) 
	{
		std::cerr << "Error: Could not load background texture!" << std::endl;
		return 1;
	}



	//main game loop
	while (window.isOpen())
	{
		/*
		*******************
		*   Handle Input  *
		*******************
		*/

		//event used for system events like keys pressed
		Event event;
		while (window.pollEvent(event))
		{
			if (event.type == Event::KeyPressed) {
				//Pausing the game
				if (event.key.code == Keyboard::Return && state == State::PLAYING)
				{
					state = State::PAUSED;
				}
				//Restart while paused
				else if (event.key.code == Keyboard::Return && state == State::PAUSED)
				{
					state = State::PLAYING;
					clock.restart();
				}
				//Starte another game while in GAME_OVER state
				else if (event.key.code == Keyboard::Return && state == State::GAME_OVER)
				{
					state = State::LEVELING_UP;
				}

				if (state == State::PLAYING) {}
			}

		}//end pool event while

		//Exit game with esc
		if (Keyboard::isKeyPressed(Keyboard::Escape))
		{
			window.close();
		}

		//Player movement
		if (state == State::PLAYING)
		{
			if (Keyboard::isKeyPressed(Keyboard::W))
			{
				player.moveUp();
			}
			else
			{
				player.stopUp();
			}

			if (Keyboard::isKeyPressed(Keyboard::S))
			{
				player.moveDown();
			}
			else
			{
				player.stopDown();
			}

			if (Keyboard::isKeyPressed(Keyboard::A))
			{
				player.moveLeft();
			}
			else
			{
				player.stopLeft();
			}

			if (Keyboard::isKeyPressed(Keyboard::D))
			{
				player.moveRight();
			}
			else
			{
				player.stopRight();
			}
		}//end player movement

		//Handle the leveling up state
		if (state == State::LEVELING_UP)
		{
			//Handle the leveling up
			if (event.key.code == Keyboard::Num1)
			{
				state = State::PLAYING;
			}

			if (event.key.code == Keyboard::Num2)
			{
				state = State::PLAYING;
			}

			if (event.key.code == Keyboard::Num3)
			{
				state = State::PLAYING;
			}

			if (event.key.code == Keyboard::Num4)
			{
				state = State::PLAYING;
			}

			if (event.key.code == Keyboard::Num5)
			{
				state = State::PLAYING;
			}

			if (event.key.code == Keyboard::Num6)
			{
				state = State::PLAYING;
			}

			if (state == State::PLAYING)
			{
				//Prepare the level
				arena.width = 1000;
				arena.height = 1000;
				arena.left = 0;
				arena.top = 0;

				//Pass the vertex array by reference to the createBackground function
				int tileSize = createBackground(background, arena);

				//Spawn the player in the minddle of the arena
				player.spawn(arena, resolution, tileSize);

				//Reset the clock so there isn't a frame jumb
				clock.restart();
			}
		}//End leveling up

		/*
		******************
		*Update the frame*
		******************
		*/
		if (state == State::PLAYING)
		{
			//update delta time and total playing time
			Time dt = clock.restart();
			timeGameTotal += dt;
			float dtAsSeconds = dt.asSeconds();

			//Mouse pointer position
			mouseScreenPosition = Mouse::getPosition();

			//Convert mouse position to world coordinates of mainView
			mouseWorldPosition = window.mapPixelToCoords(Mouse::getPosition(), mainView);

			//Update mainView layer
			player.update(dtAsSeconds, Mouse::getPosition());

			//Make a note	of the players current position
			Vector2f playerPosition(player.getCenter());

			//Make the view center around the player
			mainView.setCenter(player.getCenter());
		}//End updating the scene

		/*
		******************
		* Draw the scene *
		******************
		*/
		if (state == State::PLAYING)
		{
			window.clear();

			//Set the mainView to window and draw everything to it
			window.setView(mainView);

			//Draw the background
			window.draw(background, &textureBackgrund);

			//Draw the player
			window.draw(player.getSprite());
		}

		if (state == State::LEVELING_UP)
		{

		}

		if (state == State::PAUSED)
		{

		}

		if (state == State::GAME_OVER)
		{

		}
		window.display();
	}
}