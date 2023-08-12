//   main.cpp
//   created by danihek
//   source: https://github.com/danihek07/SnakeNN
//	 
//   Training neural network to solve the best way to play Snake
//   using Genetic Algorithm. Program creates X (population) NN's and testing their performance
//   in game using fitness function. NN with the best fitness value is mixed with second
//   best NN and mutated X times to fill out population vector.
//   The best progressing NN's are saved to file named with their fitness and generation.
//   Neural network is designed like this:
//   
//   16 - Input Layer ( Every update Snake (his head) is sending rays in 8 directions (like in compass: N, NE, E, SE, S.. etc.)
//	 8 for walls/borders and 8 for food if ray is coliding with object like Food-Apple or border then it takes that distance to neuron with 1/distance.
//   If ray sent for Food is not pointing for it Neuron Value is set to 0
// 
//   {  - Hidden Layer
//	    8 - first with 8 neurons
//      6 - second with 6 neurons
//   }
// 
//   4 - Output Layer (4 output neurons: left, right, up, down)
//

#include <SFML/Graphics.hpp>
#include <sstream>
#include<iostream>
#include <ctime>
#include <Windows.h>
#include <cstdlib>
#include <WinUser.h>
#include <ctime>
#include <string>
#include <algorithm>
#include <stdio.h>
#include <vector>
#include "Board.h"

int SCREEN_WIDTH = 1600;
int SCREEN_HEIGHT = 1200;
int blockSize = 30;
int POPULATION = 300;

bool isAiplaying = true;
bool loadModel = false;
bool evolveOrShow = false;

std::string modelFilename="5720-362989568";

int main()
{
	int cols=30;
	int rows=30;
	std::cout << "Population size(100-500 recommended): \n"; std::cin >> POPULATION;
	std::cout << "screen width(1200): \n"; std::cin >> SCREEN_WIDTH;
	std::cout << "screen height(1200): \n"; std::cin >> SCREEN_HEIGHT;
	std::cout << "screen blockSize(px): \n"; std::cin >> blockSize;
	
	std::cout << "screen rows(30): \n"; std::cin >> rows;
	std::cout << "screen cols(30): \n"; std::cin >> cols;
	
	std::cout << "player or ai? (0-1): \n"; std::cin >> isAiplaying;
	if (isAiplaying)
	{
		std::cout << "load already trained model? yes-no (0-1): \n"; std::cin >> loadModel;

		if (loadModel)
		{
			std::cout << "model filename: \n"; std::cin >> modelFilename;
		}
	}

	sf::Event event;
	sf::Clock clock;
	bool stateOfnumberthingstodraw = false;
	bool drawing = true;
	
	int timeStep = 100;
	
	float lasttime = clock.getElapsedTime().asMilliseconds();
	float scale = 1;


	Board board(rows, cols, SCREEN_WIDTH, SCREEN_HEIGHT, blockSize, POPULATION, isAiplaying, loadModel);
	
	
	if (loadModel)
	{
		board.setModelFilename(modelFilename, evolveOrShow);
		board.SetUpModels();
	}
	sf::RenderWindow window(sf::VideoMode(SCREEN_WIDTH, SCREEN_HEIGHT), "Snake by danihek");
	
	bool slowUpdate = true;

	while (window.isOpen())
	{
		while (window.pollEvent(event))
		{
			if (event.type == sf::Event::Closed)
				window.close();

			if (event.type == sf::Event::KeyPressed)
			{
				if (event.key.code == sf::Keyboard::Left)
					board.MoveSnake(true, -1, 0);
					//board.turnSnake(true);// , -1, 0);
			else
				if (event.key.code == sf::Keyboard::Right)
					board.MoveSnake(true, 1, 0);
					//board.turnSnake(false);// , 1, 0);
			else
				if (event.key.code == sf::Keyboard::Up)
					board.MoveSnake(false, -1, 0);
			else
				if (event.key.code == sf::Keyboard::Down)
					board.MoveSnake(false, 1, 0);

				if (event.key.code == sf::Keyboard::Space)
					if (slowUpdate)
					{
						slowUpdate = false;
					}
					else
						slowUpdate = true;
				if (event.key.code == sf::Keyboard::Num0)
				{
					timeStep += 50;
				}
				else
					if (event.key.code == sf::Keyboard::Num9)
					{
						timeStep -= 50;
					}
				if (event.key.code == sf::Keyboard::R)
				{
					if (stateOfnumberthingstodraw)
						stateOfnumberthingstodraw = false;
					else
						stateOfnumberthingstodraw = true;
				}
				if (event.key.code == sf::Keyboard::Q)
				{
					if (drawing)
						drawing = false;
					else
						drawing = true;
				}
				if (event.key.code == sf::Keyboard::T)
				{
					board.turnRaycasts();
				}
				if (event.key.code == sf::Keyboard::U)
				{
					board.showInput();
				}
				if (event.key.code == sf::Keyboard::Y)
				{
					board.showBrain();
				}
				if (event.key.code == sf::Keyboard::Z)
				{
					scale -= 0.02;
				}
				if (event.key.code == sf::Keyboard::X)
				{
					scale += 0.02;
				}
			}
		}


		if (slowUpdate)
		{
			if (clock.getElapsedTime().asMilliseconds() - lasttime > timeStep)
			{
				lasttime = clock.getElapsedTime().asMilliseconds();

				board.Update(slowUpdate, drawing);

				window.clear();
				if (drawing)
					board.draw(&window, stateOfnumberthingstodraw,scale);
				window.display();
			}
		}
		else
		{
			if (isAiplaying)
			{
				board.Update(slowUpdate, drawing);

				window.clear();
				if (drawing)
					board.draw(&window, stateOfnumberthingstodraw,scale);
				window.display();
			}
		}
	}

	return 0;
}
