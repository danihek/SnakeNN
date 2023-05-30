#include <iostream>
#include <SFML/Graphics.hpp>
#include <ctime>
#include <random>
#include "Snake.h"
//#include "Apple.h"
//#include "NeuralNetwork.h"

int SNAKESFACTOR = 2;

void bubbleSort(float* arr[], int n)
{
	bool swapped;
	for (int i = 0; i < n - 1; i++)
	{
		swapped = false;
		for (int j = 0; j < n - i - 1; j++)
		{
			if (arr[j] > arr[j + 1])
			{
				float temp = *arr[j];
				arr[j] = arr[j + 1];
				*arr[j + 1] = temp;
				swapped = true;
			}
		}
		if (swapped == false)
			break;
	}
}

int randomnumber(int mini, int maxi)
{
	return rand() % (maxi - mini + 1) + mini;
}

class Board
{
public:

	Board(int _rows, int _cols, int _SCREEN_WIDTH, int _SCREEN_HEIGHT, int _blockSize, int _population, bool _isAi, bool _preTrainedModels)
	{
		rows = _rows;
		cols = _cols;
		SCREEN_WIDTH = _SCREEN_WIDTH;
		SCREEN_HEIGHT = _SCREEN_HEIGHT;
		blockSize = _blockSize;
		isAi = _isAi;
		srand(time(NULL));

		SNAKESFACTOR = _population;

		preTrainedModels = _preTrainedModels;

		if (!font.loadFromFile("arial.ttf"))
		{
			std::cerr << "cannot load font!" << std::endl;
		}

		if (isAi)
		{
			for (int i = 0; i < SNAKESFACTOR; i++)
			{
				if (!_preTrainedModels)
				{
					SpawnSnake();
				}
				else
				{
					//SpawnSnake();
				}
				
				//snakes.at(i).MutateCreature();
			}
		}
		else
		{
			SpawnSnake();
			//SpawnApple();
		}

		//net.at(0).printWeights();
		//net.at(0).printBiases();
		//system("pause");
	}

	~Board()
	{

	}

	void SetUpModels()
	{
		std::vector<unsigned> topology;
		topology.push_back(16); // working = {16, 8, 6 ,4}
		topology.push_back(8);
		topology.push_back(6);
		topology.push_back(4);
		
		Net __net = readNet(currentModelFilename, topology);
		
		loadedModel.push_back(__net);

		loadedModel.at(0).draw();
		//__net.saveNet("CEI2");
		 //system("pause");

		 //loadedModel.at(0).saveNet("CEI3");

		for (int i = 0; i < SNAKESFACTOR; i++)
		{
			SpawnSpecificSnake(loadedModel.at(0));
		}
	}

	void setModelFilename(std::string ___filename, bool _evolveOrShow)
	{
		evolveOrShowModel = _evolveOrShow;
		currentModelFilename = ___filename;
	}

	void MoveSnake(bool ver, int dir, int arr)
	{
		if (snakes.size() >= 1)
		{
			if (ver)
			{
				//if (snakes.at(arr).canChangeVelX())
					snakes.at(arr).mVeritcal(dir);
			}
			else
			{
				//if (snakes.at(arr).canChangeVelY())
					snakes.at(arr).mHorizontal(dir);
			}
		}
	}

	int nbOfSnakesAlive()
	{
		int alv = 0;
		for (int snak = 0; snak < snakes.size(); snak++)
		{
			if (snakes.at(snak).isAlive_())
			{
				alv++;
			}
		}

		return alv;
	}

	void turnRaycasts()
	{
		if (raycastState)
			raycastState = false;
		else
			raycastState = true;
	}

	void SaveBestModel()
	{
		if (bestNetwork.size() > 0)
		{
			
			//SaveNet(&bestNetwork.at(0), "models/model-" + std::to_string(generation) + "-" + std::to_string(int(lastBestFitness)));
		}
	}

	void loadSpecificModel(std::string filename)
	{
		std::vector<unsigned> topology;

		topology.push_back(16); // working = {16, 8, 6 ,4}
		topology.push_back(8);
		topology.push_back(6);
		//male.push_back(topology);
		//readNet(&male.at(0),filename);
	}

	void Update(bool slowUpdate)
	{
		if (nbOfSnakesAlive() > 0)
		{
			int tempIndexforInput = 0;
			for (int snak = 0; snak < snakes.size(); snak++)
			{
				if (snakes.at(snak).isAlive_())
				{
					snakes.at(snak).turnInputOutputShowUp(false);
					if (tempIndexforInput < snakes.at(snak).segmentSize())
					{
						tempIndexforInput = snak;/////////////////////////////
					}
				}
			}
			if (!___stateOfNumbers)
				snakes.at(tempIndexforInput).turnInputOutputShowUp(showInputVar);

			for (int snak = 0; snak < snakes.size(); snak++)
			{
				//if (snakes.at(snak).Mutated())
				//{
					//snakes.at(snak).MutateCreature();
				//}

				if (snakes.at(snak).getFitness() > FitnessPerGen)
					FitnessPerGen = snakes.at(snak).getFitness();

				if (snakes.at(snak).isAlive_())
				{
					bool canGoFurther = true;
					//snakes.at(snak).Update(apples.at(0).getPos().x, apples.at(0).getPos().y);
					snakes.at(snak).Update();

					if (snakes.at(snak).getPos(0).x < 0 || snakes.at(snak).getPos(0).x >= rows || snakes.at(snak).getPos(0).y < 0 || snakes.at(snak).getPos(0).y >= cols)
					{
						snakes.at(snak).setIsAlive(false);
						//eraseSnake(snak);
						//applesErase(snak);
						//return;
					}
					else
					{
						for (int i = 1; i < snakes.at(snak).segmentSize(); i++)
						{
							if (snakes.at(snak).getPos(0) == snakes.at(snak).getPos(i))
							{
								snakes.at(snak).setIsAlive(false); //importante
								//eraseSnake(snak);
								//applesErase(snak); --- error fucking destroyer
								canGoFurther = false;
							}
						}
					}

					if (canGoFurther)
					{
						if (snakes.at(snak).getPos(0) == snakes.at(snak).applePos()) /// remeber!!!
						{
							//applesErase(snak);

							snakes.at(snak).SpawnApple();

							if (isAi)
							{
								//SpawnAppleInElement(snak);
								snakes.at(snak).addScore(10);
								//SpawnSpecificSnake(snakes.at(snak).getNN());

							}
							else
							{
								//SpawnApple();
							}
							snakes.at(snak).AddSegment();

						}
					}


				}
			}
		}
		else
		{
			if (isAi)
			{
				std::vector<Net> male;
				int bestIndex=-1;
				for (int snak = 0; snak < snakes.size(); snak++)
				{
					snakes.at(snak).turnInputOutputShowUp(false);

					if (snakes.at(snak).getFitness() > lastBestFitness)
					{
						//bestIndex = snak;
						//lastBestFitness = snakes.at(snak).getFitness();
						if (male.size() <= 0)
						{
							male.push_back(snakes.at(snak).getNN());
						}
						else
						{
							female.clear();
							female.push_back(male.at(0));

							male.clear();
							male.push_back(snakes.at(snak).getNN());
						}
						lastBestFitness = snakes.at(snak).getFitness();
					}
				}

				if (male.size() > 0 && lastBestFitness > FitnessPerGen)
				{
					bestNetwork.clear();
					bestNetwork.push_back(male.at(0));
				}

				snakes.clear();
				
				if (preTrainedModels)
				{
					for (int snak = 0; snak < SNAKESFACTOR; snak++)
					{
						if (loadedModel.size() > 0)
						{
							if (evolveOrShowModel)
							{
								LoadSpawnSpecificSnake(loadedModel.at(0));
							}
							else
							{
								if (snak % 2 == 0)
								{
									if (male.size() > 0)
									{
										SpawnSpecificSnake(male.at(0));
									}
									else
									{
										//std::cout << "XD?!" << std::endl;
										SpawnSnake();
									}
								}
								else
								{
									if (bestNetwork.size()>0)
										SpawnSpecificSnake(bestNetwork.at(0));
									SpawnSnake();
								}
							}
						}
					}
				}
				else
				{
					for (int snak = 0; snak < SNAKESFACTOR; snak++)
					{
						if (snak % 2 == 0)
						{
							if (male.size() > 0)
							{
								if (female.size() > 0)
								{
									SpawnSpecificSnake(male.at(0)); // - working
									//SpawnSpecificSnake(male.at(0).MixNetworks(female.at(0)));
								}
								else
								{
									SpawnSpecificSnake(male.at(0));
								}
							}
							else
							{
								//std::cout << "XD?!" << std::endl;
								SpawnSnake();
							}
						}
						else
						{
							//SpawnSpecificSnake(female.at(0));
							if (bestNetwork.size() > 0)
								SpawnSpecificSnake(bestNetwork.at(0));
							SpawnSnake();
							//if (male.size() > 0)//male.size() > 0)
							//{
							//	if (female.size() > 0)
							//	{
							//		SpawnSpecificSnake(female.at(0).MixNetworks(male.at(0)));
							//	}
							//	else
							//	{
							//		SpawnSpecificSnake(male.at(0));
							//	}
							//}
							//else
							//{
							//	//std::cout << "XD?!" << std::endl;
							//	SpawnSnake();
							//}
						}
					}
					//if (snak % 2 == 0)
					//{
					//	if (male.size() > 0)
					//		SpawnSpecificSnake(male.at(0));
					//	else
					//		SpawnSnake();
					//}
					//else
					//{
					//	//SpawnSnake();
					//	if (female.size() > 0)
					//		SpawnSpecificSnake(female.at(0));
					//	else if (male.size() > 0)
					//		SpawnSpecificSnake(male.at(0));
					//	else
					//		SpawnSnake();
					//}
					for (int snak = 0; snak < 10; snak++)
					{
						SpawnSnake();
						if (bestNetwork.size()>0)
							SpawnSpecificSnake(bestNetwork.at(0));
					}
				
				}

				if (lastBestFitness > FitnessPerGen)
					FitnessPerGen = lastBestFitness;

				if (counterOfGenerations >= 20)
				{
					counterOfGenerations = 0;
					if (bestNetwork.size() > 0)
					bestNetwork.at(0).saveNet(std::to_string(generation)
						+ "-"
						+ std::to_string(int(FitnessPerGen)));
				}
				
				
				/*if (male.size() > 0)
					SaveNet(&male.at(0), "male-"+std::to_string(generation)+ std::to_string(int(lastBestFitness)));
				else if (female.size() > 0)
					SaveNet(&female.at(0), "female-" + std::to_string(generation) + std::to_string(int(lastBestFitness)));
					*/


				generation++;
				std::cout << "Generation: " << generation << " | Current Best Score: " << lastBestFitness << 
					" | Highest Score: " << FitnessPerGen <<std::endl;
				counterOfGenerations++;
				lastBestFitness = 0.0f;
				lastHighestIndex = 0;

				//system("pause");

			}
			else
			{
				snakes.clear();
				SpawnSnake();
			}
		}

	}
	void SpawnApple()
	{
		/*if(apples.size()>0)
			apples.erase(apples.begin());*/

		int tX = randomnumber(1, rows - 1);
		int tY = randomnumber(1, cols - 1);

		apples.push_back(Apple(tX, tY));
	}

	void SpawnAppleInElement(int id)
	{
		int tX = randomnumber(1, rows - 1);
		int tY = randomnumber(1, cols - 1);

		if (apples.size() > id)
			apples.insert(apples.begin() + id, Apple(tX, tY));
		else
			SpawnApple();
	}

	void showInput()
	{
		if (showInputVar)
			showInputVar = false;
		else
			showInputVar = true;
	}

	void eraseSnake(int er)
	{
		if (snakes.size() + er < snakes.size())
			snakes.erase(snakes.begin() + er);
	}

	void applesErase(int er)
	{
		if (apples.size() + er < apples.size())
			apples.erase(apples.begin() + er);
	}

	void SpawnSnake()
	{
		//int ar[] = { 4, 2 ,4 };
		//int arSize = sizeof(ar) / sizeof(ar[0]);
		std::vector<unsigned> topology;

		topology.push_back(16); // working = {16, 8, 6 ,4}
		topology.push_back(8);
		topology.push_back(6);
		topology.push_back(4);
		int tX = randomnumber(2, rows - 2);
		int tY = randomnumber(2, cols - 2);
		snakes.push_back(Snake(tX, tY, rows, cols, topology, isAi));
		snakes.back().MutateCreature();
	}

	void SpawnSpecificSnake(Net _NN)
	{
		int tX = randomnumber(2, rows - 2);
		int tY = randomnumber(2, cols - 2);
		snakes.push_back(Snake(tX, tY, rows, cols, _NN, isAi));
		snakes.back().MutateCreature();
	}

	void LoadSpawnSpecificSnake(Net _NN)
	{
		int tX = randomnumber(2, rows - 2);
		int tY = randomnumber(2, cols - 2);
		snakes.push_back(Snake(tX, tY, rows, cols, _NN, isAi));
		//snakes.back().MutateCreature();
	}

	void draw(sf::RenderWindow* window, bool stateofNumbers)
	{
		___stateOfNumbers = stateofNumbers;
		int gridX = SCREEN_WIDTH / 2 - (rows * blockSize) / 2;
		int gridY = SCREEN_HEIGHT / 2 - (cols * blockSize) / 2;

		int tempIndex = 0;
		for (int snak = 0; snak < snakes.size(); snak++)
		{
			if (snakes.at(snak).isAlive_())
			{
				if (tempIndex < snakes.at(snak).segmentSize())
				{
					tempIndex = snak;/////////////////////////////
				}
			}
		}

		if (tempIndex > lastHighestIndex)
		{
			lastHighestIndex = tempIndex;
		}
		if (!snakes.at(lastHighestIndex).isAlive_())
		{
			lastHighestIndex = tempIndex;
		}

		tempIndex = lastHighestIndex;

		// background
		sf::RectangleShape background(sf::Vector2f(SCREEN_WIDTH, SCREEN_HEIGHT));
		background.setFillColor(sf::Color(30, 30, 30));
		background.setPosition(0, 0);
		window->draw(background);

		//map

		sf::RectangleShape tCell(sf::Vector2f(blockSize, blockSize));
		for (int r = 0; r < rows; r++)
		{
			for (int c = 0; c < cols; c++)
			{
				tCell.setPosition(gridX + r * blockSize, gridY + c * blockSize);
				tCell.setFillColor(sf::Color(166, 166, 166));
				tCell.setOutlineThickness(2);//blockSize / 50);
				tCell.setOutlineColor(sf::Color::White);
				window->draw(tCell);
			}
		}

		//text
		if (snakes.size() >= 1)
		{
			if (!stateofNumbers)
			{
				sf::Text numberOfPoints("Generation: " 
					+ std::to_string(generation) 
					+ " | Alive: " + std::to_string(nbOfSnakesAlive()) 
					+ " | Population: " + std::to_string(snakes.size()) + " | BestScore: " 
					+ std::to_string(int(FitnessPerGen)) + " | CurrentSnake ID: " + std::to_string(tempIndex) 
					+ "| Size: " + std::to_string(snakes.at(tempIndex).segmentSize() - 1) 
					+ "| Fitness: " + std::to_string(int(snakes.at(tempIndex).getFitness()))
					+ "| LeftMoves: " + std::to_string(snakes.at(tempIndex).leftMovesNumber()) + "-" + std::to_string(snakes.at(tempIndex).MaxleftMovesNumber()),
					font, 20);
				numberOfPoints.setCharacterSize(15);
				numberOfPoints.setStyle(sf::Text::Bold);
				numberOfPoints.setPosition(10, 10);
				numberOfPoints.setFillColor(sf::Color::White);
				window->draw(numberOfPoints);
			}
			else
			{
				sf::Text numberOfPoints("Generation: "
					+ std::to_string(generation)
					+ " | Alive: " + std::to_string(nbOfSnakesAlive())
					+ " | Population: " + std::to_string(snakes.size()) + " | BestScore: "
					+ std::to_string(int(FitnessPerGen)), font, 20);
				numberOfPoints.setCharacterSize(15);
				numberOfPoints.setStyle(sf::Text::Bold);
				numberOfPoints.setPosition(10, 10);
				numberOfPoints.setFillColor(sf::Color::White);
				window->draw(numberOfPoints);
			}
		}
		else
		{
			sf::Text numberOfPoints("Points: 0 | Generation: " + std::to_string(generation) + " | Alive: " + std::to_string(snakes.size()), font, 20);
			numberOfPoints.setCharacterSize(15);
			numberOfPoints.setStyle(sf::Text::Bold);
			numberOfPoints.setPosition(10, 10);
			numberOfPoints.setFillColor(sf::Color::White);
			window->draw(numberOfPoints);
		}

		//snake
		sf::RectangleShape snakeShape(sf::Vector2f(blockSize, blockSize));

		if (stateofNumbers)
		{
			for (int snak = 0; snak < snakes.size(); snak++)
			{
				if (snakes.at(snak).isAlive_())
				{
					for (int i = 0; i < snakes.at(snak).segmentSize(); i++)
					{
						snakeShape.setPosition(gridX + snakes.at(snak).getPos(i).x * blockSize, gridY + snakes.at(snak).getPos(i).y * blockSize);
						//snakeShape.setFillColor(sf::Color::Black);
						//if (snak != 0)
						//{
							snakeShape.setFillColor(snakes.at(snak).getColor());
					//	}
						snakeShape.setOutlineThickness(2);//blockSize / 50);
						snakeShape.setOutlineColor(sf::Color::Green);
						window->draw(snakeShape);


						if (raycastState)//raycasts
						{
							std::vector<sf::Vector2f> v1 = snakes.at(snak).linePos();
							//std::cout << v1.size() << std::endl;

							for (int y = 0; y < v1.size(); y += 2)
							{
								//std::cout << v1.at(0).x << " " << v1.at(0).y << " | " << v1.at(1).x << " " << v1.at(1).y << std::endl;

								v1.at(y).x = gridX + (v1.at(y).x * blockSize);
								v1.at(y).y = gridY + (v1.at(y).y * blockSize);

								v1.at(y + 1).x = gridX + (v1.at(y + 1).x * blockSize);
								v1.at(y + 1).y = gridY + (v1.at(y + 1).y * blockSize);

								sf::Vertex v[] =
								{
									sf::Vertex(v1.at(y),sf::Color::Red),
									sf::Vertex(v1.at(y + 1),sf::Color::Red)
								};
								window->draw(v, 2, sf::Lines);
							}
						}
					}
				}
			}
		}
		else
		{
			if (snakes.at(tempIndex).isAlive_())
			{
				snakes.at(tempIndex).turnInputOutputShowUp(showInputVar);
				for (int i = 0; i < snakes.at(tempIndex).segmentSize(); i++)
				{
					snakeShape.setPosition(gridX + snakes.at(tempIndex).getPos(i).x * blockSize, gridY + snakes.at(tempIndex).getPos(i).y * blockSize);
					//snakeShape.setFillColor(sf::Color::Black);
					//if (snak != 0)
					//{

						snakeShape.setFillColor(snakes.at(tempIndex).getColor());
					//}
					snakeShape.setOutlineThickness(2);//blockSize / 50);
					snakeShape.setOutlineColor(sf::Color::Green);
					window->draw(snakeShape);
					if (raycastState)//raycasts
					{
						std::vector<sf::Vector2f> v1 = snakes.at(tempIndex).linePos();
						//std::cout << v1.size() << std::endl;

						for (int y = 0; y < v1.size(); y += 2)
						{
							//std::cout << v1.at(0).x << " " << v1.at(0).y << " | " << v1.at(1).x << " " << v1.at(1).y << std::endl;

							v1.at(y).x = gridX + (v1.at(y).x * blockSize);
							v1.at(y).y = gridY + (v1.at(y).y * blockSize);

							v1.at(y + 1).x = gridX + (v1.at(y + 1).x * blockSize);
							v1.at(y + 1).y = gridY + (v1.at(y + 1).y * blockSize);

							sf::Vertex v[] =
							{
								sf::Vertex(v1.at(y),sf::Color::Red),
								sf::Vertex(v1.at(y + 1),sf::Color::Red)
							};
							window->draw(v, 2, sf::Lines);
						}
					}
				}
			}
		}

		//apple
		sf::RectangleShape appleShape(sf::Vector2f(blockSize, blockSize));
		appleShape.setOutlineThickness(2);//blockSize / 50);
		appleShape.setOutlineColor(sf::Color(255, 28, 28));

		if (stateofNumbers)
		{
			for (int snak = 0; snak < snakes.size(); snak++)
			{
				if (snakes.at(snak).isAlive_())
				{
					appleShape.setPosition(gridX + snakes.at(snak).applePos().x * blockSize, gridY + snakes.at(snak).applePos().y * blockSize);
					appleShape.setFillColor(sf::Color(238, 114, 114));
					window->draw(appleShape);
				}
			}
		}
		else
		{
			appleShape.setPosition(gridX + snakes.at(tempIndex).applePos().x * blockSize, gridY + snakes.at(tempIndex).applePos().y * blockSize);
			appleShape.setFillColor(sf::Color(238, 114, 114));
			window->draw(appleShape);
		}
	}

	bool checkFitness(double f)
	{
		if (f > lastBestFitness)
		{
			lastBestFitness = f;
		}
		if (f > FitnessPerGen)
		{
			return true;
		}
		return false;
	}

private:
	double FitnessPerGen = 0;
	double lastBestFitness = 0;
	std::vector<Snake> bestPerGen;
	//std::vector<Net> male;
	std::vector<Net> female;
	std::vector<Net> bestNetwork;
	std::vector<Net> loadedModel;
	int lastHighestIndex = 0;

	std::string currentModelFilename="";

	sf::Font font;
	bool isAi;
	bool preTrainedModels;
	bool evolveOrShowModel;
	bool raycastState=false;
	bool showInputVar = false;
	bool ___stateOfNumbers = false;
	int generation = 0;
	int counterOfGenerations = 0;
	int rows;
	int cols;
	int SCREEN_WIDTH;
	int SCREEN_HEIGHT;
	int blockSize;
	std::vector<Snake> snakes;
	std::vector<Apple> apples;
	//std::vector<NeuralNetwork> net;
};