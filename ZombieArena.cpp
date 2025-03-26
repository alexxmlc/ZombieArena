#include <SFML/Graphics.hpp>
#include <iostream>
#include <sstream>
#include "Player.h"
#include "ZombieArena.h"
#include "TextureHolder.h"
#include "Bullet.h"
#include "Pickup.h"

using namespace sf;

int main()
{
	//The one and only texture holder instance
	TextureHolder holder;

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
	Texture textureBackground = TextureHolder::GetTexture("graphics/background_sheet.png");
	

	//Prepare a horde of zombies
	int numZombies;
	int numZombiesAlive;
	Zombie* zombies = nullptr;

	//Create 100 bullets
	const int NB_OF_BULLETS = 100;
	Bullet bullets[NB_OF_BULLETS];
	int currentBullet = 0;
	int bulletsSpare = 24;
	int bulletsInClip = 6;
	int clipSize = 6;
	float fireRate = 1;
	//Last time fired
	Time lastPressed;

	//Give the player a crosshair, hide mouse pointer and replace with crosshair
	window.setMouseCursorVisible(false);
	Sprite spriteCrosshair;
	Texture textureCrosshair = TextureHolder::GetTexture("graphics/crosshair.png");
	spriteCrosshair.setTexture(textureCrosshair);
	spriteCrosshair.setOrigin(25, 25);

	//Create some pickups
	Pickup healthPickup(1);
	Pickup ammoPickup(2);

	//Score
	int score = 0;
	int hiScore = 0;

	// Home/Game Over screen
	Sprite spriteGameOver;
	Texture textureGameOver = TextureHolder::GetTexture("graphics/background.png");
	spriteGameOver.setTexture(textureGameOver);
	spriteGameOver.setPosition(0, 0);

	//View for the hud
	View hudView(sf::FloatRect(0, 0, resolution.x, resolution.y));

	//Ammo icon
	Sprite spriteAmmoIcon;
	Texture textureAmmoIcon = TextureHolder::GetTexture("graphics/ammo_icon.png");
	spriteAmmoIcon.setTexture(textureAmmoIcon);
	spriteAmmoIcon.setPosition(20, 980);

	//The font
	Font font;
	font.loadFromFile("font/zombiecontrol.ttf");

	//Paused Game
	Text pausedText;
	pausedText.setFont(font);
	pausedText.setCharacterSize(155);
	pausedText.setFillColor(Color::White);
	pausedText.setPosition(400, 400);
	pausedText.setString("Press Enter \nto continue");

	//Game Over
	Text gameOverText;
	gameOverText.setFont(font);
	gameOverText.setCharacterSize(125);
	gameOverText.setFillColor(Color::White);
	gameOverText.setPosition(250, 850);
	gameOverText.setString("Press Enter to play");

	//Leveling up
	Text levelUpText;
	levelUpText.setFont(font);
	levelUpText.setCharacterSize(80);
	levelUpText.setFillColor(Color::White);
	levelUpText.setPosition(150, 250);
	std::stringstream levelUpStream;
	levelUpStream << "1- Increased rate of fire" <<
		"\n2- Increased clip size" <<
		"\n3- Increased max health" <<
		"\n4- Increased run speed" <<
		"\n5- More and better health pickups" <<
		"\n6- More and better ammo pickups";
	levelUpText.setString(levelUpStream.str());

	//Ammo
	Text ammoText;
	ammoText.setFont(font);
	ammoText.setCharacterSize(55);
	ammoText.setFillColor(Color::White);
	ammoText.setPosition(200, 980);

	//Score
	Text scoreText;
	scoreText.setFont(font);
	scoreText.setCharacterSize(55);
	scoreText.setFillColor(Color::White);
	scoreText.setPosition(20, 0);

	//Hi score
	Text hiScoreText;
	hiScoreText.setFont(font);
	hiScoreText.setCharacterSize(55);
	hiScoreText.setFillColor(Color::White);
	hiScoreText.setPosition(1400, 0);
	std::stringstream s;
	s << "Hi Score:" << hiScore;
	hiScoreText.setString(s.str());

	// Zombies remaining
	Text zombiesRemainingText;
	zombiesRemainingText.setFont(font);
	zombiesRemainingText.setCharacterSize(55);
	zombiesRemainingText.setFillColor(Color::White);
	zombiesRemainingText.setPosition(1500, 980);
	zombiesRemainingText.setString("Zombies: 100");

	// Wave number
	int wave = 0;
	Text waveNumberText;
	waveNumberText.setFont(font);
	waveNumberText.setCharacterSize(55);
	waveNumberText.setFillColor(Color::White);
	waveNumberText.setPosition(1250, 980);
	waveNumberText.setString("Wave: 0");

	//Health bar
	RectangleShape healthBar;
	healthBar.setFillColor(Color::Red);
	healthBar.setPosition(450, 980);

	//Last hud update
	int framesSinceLastHUDUpdate = 0;

	//How often update the HUD
	int fpsFrameInterval = 0;

	bool zoomed = false;

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

				if (state == State::PLAYING)
				{
					//Reloading the gun
					if (event.key.code == Keyboard::R)
					{
						if (bulletsSpare >= clipSize)
						{
							//Fully reload, we have plenty of bullets
							bulletsInClip = clipSize;
							bulletsSpare -= clipSize;
						}
						else if (bulletsSpare > 0)
						{
							//Only a few bullets left
							bulletsInClip += bulletsSpare;
							bulletsSpare = 0;
						}
						else
						{

						}
					}//End reload
				}
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

		//Fire a bullet
		if (Mouse::isButtonPressed(sf::Mouse::Left))
		{
			//Don t shoot more often than the fire rate and check for bullets in clip
			if (timeGameTotal.asMilliseconds() - lastPressed.asMilliseconds() > 1000 / fireRate && bulletsInClip > 0)
			{
				//Pass the center of the player and the center of the corsshair to the shoot function
				//with other words start and finish points
				bullets[currentBullet].shoot(player.getCenter().x, player.getCenter().y, mouseWorldPosition.x, mouseWorldPosition.y);
				currentBullet++;

				if (currentBullet > 99)
				{
					currentBullet = 0;
				}
				lastPressed = timeGameTotal;
				bulletsInClip--;
			}
		}//End fire bullet

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
				arena.width = 500;
				arena.height = 500;
				arena.left = 0;
				arena.top = 0;

				//Pass the vertex array by reference to the createBackground function
				int tileSize = createBackground(background, arena);

				//Spawn the player in the minddle of the arena
				player.spawn(arena, resolution, tileSize);

				//Config the pickups
				healthPickup.setArena(arena);
				ammoPickup.setArena(arena);

				//Create a horde of zombies
				numZombies = 6;
				
				//Delete the previously allocated mem
				delete[] zombies;
				zombies = createHorde(numZombies, arena);
				numZombiesAlive = numZombies;

				//Reset the clock so there isn't a frame jump
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
			//Update delta time and total playing time
			Time dt = clock.restart();
			timeGameTotal += dt;
			float dtAsSeconds = dt.asSeconds();

			//Mouse pointer position
			mouseScreenPosition = Mouse::getPosition();

			//Convert mouse position to world coordinates of mainView
			mouseWorldPosition = window.mapPixelToCoords(Mouse::getPosition(), mainView);

			//Set the crosshair to the mouse world location
			spriteCrosshair.setPosition(mouseWorldPosition);

			//Update mainView layer
			player.update(dtAsSeconds, Mouse::getPosition());

			//Make a note of the players current position
			Vector2f playerPosition(player.getCenter());

			//Make the view center around the player
			mainView.setCenter(player.getCenter());

			//Loop through each zombie and update them
			for (int i = 0; i < numZombies; i++)
			{
				if (zombies[i].isAlive())
				{
					zombies[i].update(dt.asSeconds(), playerPosition);
				}
			}

			//Update any bullets that are in flight
			for (int i = 0; i < NB_OF_BULLETS; i++)
			{
				if (bullets[i].isInFlight())
				{
					bullets[i].update(dtAsSeconds);
				}
			}

			//Update pickup
			healthPickup.update(dtAsSeconds);
			ammoPickup.update(dtAsSeconds);

			//Have any zombies been shot
			for (int i = 0; i < NB_OF_BULLETS; i++)
			{
				for (int j = 0; j < numZombies; j++)
				{
					//Checks if the bullet has been shot and the zombie is still alive
					if (bullets[i].isInFlight() && zombies[j].isAlive())
					{
						//Checks for the collision bullet->zombie
						if (bullets[i].getPosition().intersects(zombies[j].getPosition()))
						{
							//If collides stop the bullet
							bullets[i].stop();

							//If hit increase the score and kill the zombie if the hit functions returns true
							//else it just gives damage to the zombie
							if (zombies[j].hit())
							{
								score += 10;
								if (score >= hiScore)
								{
									hiScore = score;
								}
								numZombiesAlive--;

								if (numZombiesAlive == 0)
								{
									state = State::LEVELING_UP;
								}
							}
						}
					}
				}
			}//End zombie being shot

			//Has the player been touched by a zombie
			for (int i = 0; i < numZombies; i++)
			{
				if (player.getPosition().intersects(zombies[i].getPosition()) && zombies[i].isAlive())
				{
					if (player.hit(timeGameTotal))
					{

					}

					if (player.getHealth() <= 0)
					{
						state = State::GAME_OVER;
					}
				}
			}

			//Has the player touched a pickup
			if (player.getPosition().intersects(healthPickup.getPosition()) && healthPickup.isSpawned())
			{
				player.increaseHealthLevel(healthPickup.gotIt());
			}

			if (player.getPosition().intersects(ammoPickup.getPosition()) && ammoPickup.isSpawned())
			{
				bulletsSpare += ammoPickup.gotIt();
			}

			//Size for health bar
			healthBar.setSize(Vector2f(player.getHealth() * 3, 50));
			framesSinceLastHUDUpdate++;

			if (framesSinceLastHUDUpdate > fpsFrameInterval)
			{
				std::stringstream ssAmmo;
				std::stringstream ssScore;
				std::stringstream ssHiScore;
				std::stringstream ssWave;
				std::stringstream ssZombiesAlive;

				//Update ammo text
				ssAmmo << bulletsInClip << "/" << bulletsSpare;
				ammoText.setString(ssAmmo.str());

				//Update Score
				ssScore << "Score: " << score;
				scoreText.setString(ssScore.str());

				//Update Hi Score
				ssHiScore << "Hi Score: " << hiScore;
				hiScoreText.setString(ssHiScore.str());

				//Update wave
				ssWave << "Wave: " << wave;
				waveNumberText.setString(ssWave.str());

				//Update zombie nb
				ssZombiesAlive << "Zombies: " << numZombiesAlive;
				zombiesRemainingText.setString(ssZombiesAlive.str());

				framesSinceLastHUDUpdate = 0;
			}

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

			if (!zoomed)
			{
				mainView.zoom(0.5f);
				zoomed = true;
			}
			
			//Draw the background
			window.draw(background, &textureBackground);

			//Draw the zombies
			for (int i = 0; i < numZombies; i++)
			{
				window.draw(zombies[i].getSprite());
			}

			//Draw the bullets each frame
			for (int i = 0; i < NB_OF_BULLETS; i++)
			{
				if (bullets[i].isInFlight())
				{
					window.draw(bullets[i].getShape());
				}
			}

			//Draw the player
			window.draw(player.getSprite());

			//Draw the crosshair
			window.draw(spriteCrosshair);

			//Draw the pickups
			if (healthPickup.isSpawned())
			{
				window.draw(healthPickup.getSprite());
			}
			if (ammoPickup.isSpawned())
			{
				window.draw(ammoPickup.getSprite());

			}

			//Switch to hud view
			window.setView(hudView);

			//Draw the hud
			window.draw(spriteAmmoIcon);
			window.draw(ammoText);
			window.draw(hiScoreText);
			window.draw(scoreText);
			window.draw(healthBar);
			window.draw(zombiesRemainingText);
			window.draw(waveNumberText);
		}

		if (state == State::LEVELING_UP)
		{
			window.draw(spriteGameOver);
			window.draw(levelUpText);
		}

		if (state == State::PAUSED)
		{
			window.draw(pausedText);
		}

		if (state == State::GAME_OVER)
		{
			window.draw(spriteGameOver);
			window.draw(gameOverText);
			window.draw(scoreText);
			window.draw(hiScoreText);
		}
		window.display();
	}//End of game loop
	
	delete[] zombies;
	return 0;
}