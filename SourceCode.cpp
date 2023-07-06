#include <SFML/Graphics.hpp>
#include <iostream>
#include <cmath>
#include <pthread.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <ctime>
#include <vector>
#include <atomic>
#include <string>
#include <stdlib.h>
#include <cstring>
#include <unistd.h>

using namespace std;

// Global variables

// Player 1 variables and checks
const float P1_Speed = 700.f;
bool goUp = true;
bool goDown = true;
bool goLeft = true;
bool goRight = true;
int P1_Score = 0;

// Player 2 variables and checks
const float P2_Speed = 700.f;
bool goW = true;
bool goS = true;
bool goA = true;
bool goD = true;
int P2_Score = 0;

//Grid
const int GridWidth = 500;
const int GridHeight = 500;

// Array to store number of random Sprites On Board
sf::Sprite spritesOnBoard[15];

// Struct for player 1 data and time
class P1_Data
{
public:
	sf::Sprite * P1;
	sf::Time * P1_Time;
	
	int play1_x = 0; 
	int play1_y = 0;
	
	int getPlay1_X()
	{
		return play1_x;
	}
	int getPlay1_Y()
	{
		return play1_y;
	}
	void setPlay1_X (int x)
	{
		play1_x = x;
	}
	void setPlay1_Y( int y)
	{
		play1_y = y;
	}
	
	
};

// Struct for player 2 data and time
class P2_Data
{
public:
	sf::Sprite * P2;
	sf::Time * P2_Time;
	int play2_x = 0; 
	int play2_y = 0;
	
	int getPlay2_X()
	{
		return play2_x;
	}
	int getPlay2_Y()
	{
		return play2_y;
	}
	void setPlay2_X (int x)
	{
		play2_x = x;
	}
	void setPlay2_Y( int y)
	{
		play2_y = y;
	}
	
};

// Function for the movement of Player1
void *P1_Movement(void *data)
{
	P1_Data *T_Data = static_cast<P1_Data*> (data);
	sf::Time *P1_Time = T_Data->P1_Time;
	sf::Sprite *P1 = T_Data->P1;
	

	while (!false)
	{
		// Change in direction when ARROW KEYS are pressed
		float x_axis = 0.f;
		float y_axis = 0.f;
		
		if ( goLeft == true ) 
			x_axis = x_axis - 1.25f;
		if (goRight == true) 
			x_axis = x_axis + 1.25f;
		
		if (goUp == true) 
			y_axis = y_axis - 1.25f;
		if (goDown == true) 
			y_axis =  y_axis + 1.25f;

		// Calculating new position of player 1 when key is prsessed
		sf::Vector2f toNewPosition = P1->getPosition() + sf::Vector2f(x_axis, y_axis) *P1_Speed *P1_Time->asSeconds();

		// Check for the limits of grid boundry 
		if (500 - P1->getGlobalBounds().width < toNewPosition.x )
		{
			toNewPosition.x = 500 - P1->getGlobalBounds().width;
		}
		else if (0 > toNewPosition.x)
		{
			toNewPosition.x = 0;
		}

		if ( ( 500 - P1->getGlobalBounds().height) < toNewPosition.y )		{
			toNewPosition.y = 500 - P1->getGlobalBounds().height;
		}
		else if (0 > toNewPosition.y)
		{
			toNewPosition.y = 0;
		}

		// Finally move the sprite to the new position when boundries are checked
		P1->setPosition(toNewPosition);

		// Checking if the player is collecting sprite i.e collision wih sprite
		for (int i = 0; i < 10; i++)
		{
			sf::Sprite &spritesBoard = spritesOnBoard[i];
			if (P1->getGlobalBounds().intersects(spritesBoard.getGlobalBounds()))
			{
		// Disappearing of sprite once collected
				spritesOnBoard[i].setPosition(sf::Vector2f(-2000, -2000));
				P1_Score = P1_Score + 6;
			}
		}

		// Sleep to reduce CPU usage
		sf::sleep(sf::milliseconds(15));
	}
}

// Function for the movement of Player2
void *P2_Movement(void *data1)
{
	P2_Data *T_Data_ = static_cast<P2_Data*> (data1);
	sf::Time *P2_Time = T_Data_->P2_Time;
	sf::Sprite *P2 = T_Data_->P2;
	
	while (!false)
	{
		// Change in direction when A, S, D, W KEYS are pressed
		float x_axis_ = 0.f;
		float y_axis_ = 0.f;
		if (goW == true)
			y_axis_ = y_axis_ - 1.f;
		if (goS == true)
			y_axis_ = y_axis_ + 1.f;
		if (goA == true)
			x_axis_ = x_axis_ - 1.f;
		if (goD == true)
			x_axis_ = x_axis_ + 1.f;

		// Calculating new position of player 2 when key is prsessed
		sf::Vector2f Moving(x_axis_, y_axis_);
		float Move_L1 = (Moving.x *Moving.x + Moving.y *Moving.y);
		if (0.f < Move_L1)
		{
			Moving = Moving / Move_L1;
			Moving = Moving * (P2_Speed *P2_Time->asSeconds());
			sf::Vector2f toNewPosition = P2->getPosition() + Moving;

		// Check for the limits of grid boundry 
			if (500.f < toNewPosition.x )
				Moving.x = 500.f - P2->getPosition().x;
			else if ( 0.f > toNewPosition.x )
				Moving.x = -P2->getPosition().x;
			if ( 500.f < toNewPosition.y )
				Moving.y = 500.f - P2->getPosition().y;
			else if (0.f > toNewPosition.y )
				Moving.y = -P2->getPosition().y;

			// Check if Player 2 collects the sprite
			int i =0;
			while (i < 10)
			{
				sf::FloatRect player_Limits = P2->getGlobalBounds();
				sf::FloatRect sprite_Limits = spritesOnBoard[i].getGlobalBounds();
				if (player_Limits.intersects(sprite_Limits))
				{
			// Disapperaing the sprite once collected
					spritesOnBoard[i].setPosition(sf::Vector2f(-2000.f, -2000.f));
					P2_Score = P2_Score + 6;
				}
				i++;
			}

			// Finally move the sprite to the new position when boundries are checked
			P2->move(Moving);
		}

		// Sleep to reduce CPU usage
		sf::sleep(sf::milliseconds(15));
	}
}

int main()
{
	// Creating windown and player 1
	sf::RenderWindow window(sf::VideoMode(500, 500), "Multiplyer Board Game");

	// Grid variables
	const int SIZE_OF_GRID = 10; const int IBRAHIM_CELL = window.getSize().x / SIZE_OF_GRID;

	//Player 1 Set-up
	sf::Texture P_Texture; P_Texture.loadFromFile("player1.png"); sf::Sprite P1(P_Texture); 	P1.setOrigin(P_Texture.getSize().x / 2.f, P_Texture.getSize().y / 2.f); P1.setScale(0.05f, 0.05f); P1.setPosition(IBRAHIM_CELL *5 + IBRAHIM_CELL / 2.f, IBRAHIM_CELL + IBRAHIM_CELL / 2.f);
	
	// Delta time for smooth movement
	sf::Clock clock_time1; 	sf::Time P1_Time = sf::Time::Zero;
	
	//Player 2 set-up
	sf::Texture P2_Texture; P2_Texture.loadFromFile("player2.png"); P2_Texture.setSmooth(true); sf::Sprite P2(P2_Texture); P2.setOrigin(P2_Texture.getSize().x / 2.f, P2_Texture.getSize().y / 2.f); P2.setScale(0.05f, 0.05f); P2.setPosition(IBRAHIM_CELL *5 + IBRAHIM_CELL / 2.f, IBRAHIM_CELL *(SIZE_OF_GRID - 1) + IBRAHIM_CELL / 2.f);
	
	sf::Clock clock_time2; sf::Time P2_Time = sf::Time::Zero;

	
	P2_Data T_Data_ = { &P2, &P2_Time
	};

	// Thread 2 for player 2
	pthread_t T_id2; 
	pthread_create(&T_id2, NULL, P2_Movement, &T_Data_); 
	P1_Data T_Data = { &P1, &P1_Time };
	
	// Thread 1 or player 1
	pthread_t T_id1;
	pthread_create(&T_id1, NULL, P1_Movement, &T_Data);

	// Cell
	sf::RectangleShape cell(sf::Vector2f(IBRAHIM_CELL, IBRAHIM_CELL)); cell.setFillColor(sf::Color::White); cell.setOutlineThickness(3); cell.setOutlineColor(sf::Color::Red);

	

	sf::Texture Sp_Texture; 	Sp_Texture.loadFromFile("starr.png");
	

	
	srand(time(NULL));

	// Generating random location of sprite
	
	int i = 0;
	while(i<15)
	{
		int x, y;
		bool position_bool;
		do { 	
		// Generate random position
			x = rand() % SIZE_OF_GRID;
			y = rand() % SIZE_OF_GRID;

			// If cell already occupied?
			position_bool = false;
			int j = 0;
			while ( j < i)
			{
				if ( sf::Vector2f(x *IBRAHIM_CELL + IBRAHIM_CELL / 2.f, y *IBRAHIM_CELL + IBRAHIM_CELL / 2.f)== spritesOnBoard[j].getPosition())
				{
					position_bool = true;
					break;
				}
				j++;
			}
			
		} while (position_bool == true ) ;

		// Sprite set-up
		spritesOnBoard[i].setTexture(Sp_Texture); spritesOnBoard[i].setScale(0.20f, 0.20f); 		spritesOnBoard[i].setOrigin(Sp_Texture.getSize().x / 2.f, Sp_Texture.getSize().y / 2.f); spritesOnBoard[i].setPosition(x *IBRAHIM_CELL + IBRAHIM_CELL / 2.f, y *IBRAHIM_CELL + IBRAHIM_CELL / 2.f);
		
		i++;
	}

	while (window.isOpen() == true)
	{
		// Player movement handling
		sf::Event new_event;
		while (window.pollEvent(new_event) == true)
		{
			if (new_event.type == sf::Event::KeyPressed)
			{
				if (sf::Keyboard::S == new_event.key.code)
					goW = false;
				
				else if (sf::Keyboard::D == new_event.key.code)
					goA = false;
				else if (sf::Keyboard::A == new_event.key.code)
					goD = false;
				else if (sf::Keyboard::W == new_event.key.code)
					goS = false;
			}
			else if (sf::Event::KeyReleased == new_event.type)
			{
				if (sf::Keyboard::S == new_event.key.code)
					goW = true;
				
				else if (sf::Keyboard::D == new_event.key.code) 
					goA = true;
				else if (sf::Keyboard::W == new_event.key.code)
					goS = true;
				else if (sf::Keyboard::A == new_event.key.code)
					goD = true;
			}
			if (sf::Event::KeyPressed == new_event.type)
			{
				if (sf::Keyboard::Down == new_event.key.code)
					goUp = false;
				else if (sf::Keyboard::Left == new_event.key.code)
					goRight = false;
				else if (sf::Keyboard::Up == new_event.key.code )
					goDown = false;
				else if (sf::Keyboard::Right == new_event.key.code)
					goLeft = false;
				
			}
			else if (sf::Event::KeyReleased == new_event.type)
			{
				if (sf::Keyboard::Down == new_event.key.code)
					goUp = true;
				
				else if (sf::Keyboard::Right == new_event.key.code)
					goLeft = true;
				else if (sf::Keyboard::Left == new_event.key.code)
					goRight = true;
				else if (sf::Keyboard::Up == new_event.key.code)
					goDown = true;	
			}

			if (new_event.type == sf::Event::Closed)
			{
				window.close();
			}
		}

		 
		P1_Time = clock_time1.restart();
		P2_Time = clock_time2.restart();

		window.clear();

		int x = 0;
		while ( x < SIZE_OF_GRID)
		{
			int y = 0;
			while ( y < SIZE_OF_GRID)
			{
			 	// cell position
				cell.setPosition(x *IBRAHIM_CELL, y *IBRAHIM_CELL);
				
				window.draw(cell);
				y++;
			}
			
			x++;
		}

		// Draw sprites
		for (int i = 0; i < 10; i++)
		{
			window.draw(spritesOnBoard[i]);
		}

		window.draw(P1);
		window.draw(P2);

		if (25 < P1_Score || 25 < P2_Score)
		{
			if (P1_Score > P2_Score)
			{
				cout << "Player 1 Wins :)" << endl;
				exit(0);
			}
			else if (P1_Score == P2_Score)
			{
				cout << "Game Over " <<endl;
				exit(0);
			}
			else
			{
				cout << "Player 2 Wins :)" << endl;
				exit(0);
			}
		}

		window.display();
	}

	// Wait func calls for each thread
	pthread_join(T_id1, NULL);
	pthread_join(T_id2, NULL);

	return 0;

}
