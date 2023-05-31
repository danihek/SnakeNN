#include <iostream>
#include <SFML/Graphics.hpp>
#include <SFML/Graphics.hpp>
#include "Board.h"

int SCREEN_WIDTH = 1600;
int SCREEN_HEIGHT = 1000;

int blockSize = 30;

int POPULATION = 250;
bool isAiplaying = true;
bool loadModel = false;
bool evolveOrShow=false;
std::string modelFilename="";

int main()
{
	int cols=30;
	int rows=30;
	//std::cout << "screen width: \n"; std::cin >> SCREEN_WIDTH;
	//std::cout << "screen height: \n"; std::cin >> SCREEN_HEIGHT;
	//std::cout << "screen blockSize(px): \n"; std::cin >> blockSize;
	//
	//std::cout << "screen rows: \n"; std::cin >> rows;
	//std::cout << "screen cols: \n"; std::cin >> cols;

	std::cout << "player or ai? (0-1): \n"; std::cin >> isAiplaying;
	if (isAiplaying)
	{
		std::cout << "Population size(100-500 recommended): \n"; std::cin >> POPULATION;
		std::cout << "load already trained model? yes-no (0-1): \n"; std::cin >> loadModel;

		if (loadModel)
		{
			std::cout << "model filename: \n"; std::cin >> modelFilename;
			//std::cout << "evolve | show ? (0-1): \n"; std::cin >> evolveOrShow;
		}
	}

	sf::Event event;
	sf::Clock clock;
	float lasttime = clock.getElapsedTime().asMilliseconds();
	bool stateOfnumberthingstodraw = false;
	bool drawing = true;
	int timeStep = 100;

	Board board(rows, cols, SCREEN_WIDTH, SCREEN_HEIGHT, blockSize, POPULATION, isAiplaying, loadModel);
	
	/*while (true)
	{
		board.Update(false);
	}*/
	
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
				else
					if (event.key.code == sf::Keyboard::Right)
						board.MoveSnake(true, 1, 0);
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
					board.draw(&window, stateOfnumberthingstodraw);
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
					board.draw(&window, stateOfnumberthingstodraw);
				window.display();
			}
		}
	}

	return 0;
}
