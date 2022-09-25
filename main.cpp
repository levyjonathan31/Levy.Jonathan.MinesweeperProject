#include <SFML/Graphics.hpp>
#include "Random.h"
#include "TextureManager.h"
#include <vector>
#include <fstream>
#include <iostream>
#include "Board.h"
void counter(int num, std::vector<sf::Sprite> &digits)
{
	if (num < 0)
	{
		num *= -1;
		digits[0].setTextureRect(sf::IntRect(210, 0, 21, 32));
	}
	else
	{
		digits[0].setTextureRect(sf::IntRect(210, 0, 0, 0));
	}

	int hunds = num / 100;
	int tens = num / 10 - hunds * 10;
	int ones = num % 10;
	digits[1].setTextureRect((sf::IntRect(21 * hunds, 0, 21, 32)));
	digits[2].setTextureRect((sf::IntRect(21 * tens, 0, 21, 32)));
	digits[3].setTextureRect((sf::IntRect(21 * ones, 0, 21, 32)));
	
}
int main()
{

	string strHeight;
	string strWidth;
	string strMines;
	std::ifstream config;
	config.open("boards/config.cfg");
	getline(config, strHeight);
	getline(config, strWidth);
	getline(config, strMines);
	
	int columns = stoi(strHeight);
	int rows = stoi(strWidth);
	int totalMines = stoi(strMines);
	sf::RenderWindow window(sf::VideoMode(columns * 32, rows * 32 + 88), "Minesweeper");
	

	std::vector<sf::Sprite> testButtons;

	for (int i = 1; i < 4; i++)
	{
		sf::Sprite test(TextureManager::GetTexture("test_" + std::to_string(i)));
		test.setPosition(((columns / 2 + 3) + 2 * i) * 32, rows * 32);
		testButtons.push_back(test);
	}
	sf::Sprite happyFace(TextureManager::GetTexture("face_happy"));
	happyFace.setPosition((columns / 2 - 1) * 32, rows * 32);

	sf::Sprite sadFace(TextureManager::GetTexture("face_lose"));
	sadFace.setPosition((columns / 2 - 1) * 32, rows * 32);

	sf::Sprite winFace(TextureManager::GetTexture("face_win"));
	winFace.setPosition((columns / 2 - 1) * 32, rows * 32);

	sf::Sprite debug(TextureManager::GetTexture("debug"));
	debug.setPosition((columns / 2 + 3) * 32, rows * 32);

	

	sf::Sprite score0(TextureManager::GetTexture("digits"));
	score0.setTextureRect(sf::IntRect((210), 0, 0, 0));
	score0.setPosition(0, rows * 32);

	sf::Sprite score1(TextureManager::GetTexture("digits"));
	score1.setTextureRect(sf::IntRect(0, 0, 21, 32));
	score1.setPosition(21, rows * 32);

	sf::Sprite score2(TextureManager::GetTexture("digits"));
	score2.setTextureRect(sf::IntRect(0, 0, 21, 32));
	score2.setPosition(42, rows * 32);

	sf::Sprite score3(TextureManager::GetTexture("digits"));
	score3.setTextureRect(sf::IntRect(0, 0, 21, 32));
	score3.setPosition(63, rows * 32);

	std::vector<sf::Sprite> digits;
	digits.push_back(score0);
	digits.push_back(score1);
	digits.push_back(score2);
	digits.push_back(score3);


	Board newBoard = Board(rows, columns, totalMines);

	while (window.isOpen())
	{
		sf::Event event;
		while (window.pollEvent(event))
		{
			if (event.type == sf::Event::Closed)
				window.close();
			else if (event.type == sf::Event::MouseButtonPressed)
			{
				sf::Vector2i pos = sf::Mouse::getPosition(window);
				int xLoc = (pos.x) / 32;
				int yLoc = (pos.y) / 32;
				if (sf::Mouse::isButtonPressed(sf::Mouse::Left))
				{
					if (xLoc >= (columns) || yLoc >= (rows))
					{
						if ((xLoc == happyFace.getPosition().x / 32 || xLoc == happyFace.getPosition().x / 32 + 1) && (yLoc == happyFace.getPosition().y / 32 || yLoc == happyFace.getPosition().y / 32 + 1))
						{
							if (newBoard.getDebug())
							{
								bool temp = newBoard.getDebug();
								newBoard = Board(rows, columns, totalMines);
								newBoard.setDebug();
								newBoard.showAllMines();
							}
							else
								newBoard = Board(rows, columns, totalMines);

						}

						else if ((xLoc == debug.getPosition().x / 32 || xLoc == debug.getPosition().x / 32 + 1) && (yLoc == debug.getPosition().y / 32 || yLoc == debug.getPosition().y / 32 + 1))
							newBoard.setDebug();
						else if ((xLoc == testButtons[0].getPosition().x / 32 || xLoc == testButtons[0].getPosition().x / 32 + 1) && (yLoc == testButtons[0].getPosition().y / 32 || yLoc == testButtons[0].getPosition().y / 32 + 1))
							newBoard.loadFromFile("boards/testboard1.brd");
						else if((xLoc == testButtons[1].getPosition().x / 32 || xLoc == testButtons[1].getPosition().x / 32 + 1) && (yLoc == testButtons[1].getPosition().y / 32 || yLoc == testButtons[1].getPosition().y / 32 + 1))
							newBoard.loadFromFile("boards/testboard2.brd");
						else if ((xLoc == testButtons[2].getPosition().x / 32 || xLoc == testButtons[2].getPosition().x / 32 + 1) && (yLoc == testButtons[2].getPosition().y / 32 || yLoc == testButtons[2].getPosition().y / 32 + 1))
							newBoard.loadFromFile("boards/testboard3.brd");

					}
					else
					{
						if (newBoard.getGameState() == 0)
						newBoard.RevealTile(xLoc, yLoc);
					}
					
				}
				else if (sf::Mouse::isButtonPressed(sf::Mouse::Right))
				{
					if (xLoc >= (columns) || yLoc >= (rows))
					{
						continue;
					}
					else
					{
						if (newBoard.getGameState() == 0)
							newBoard.AddFlag(xLoc, yLoc);
					}
				}
			}
			
		}

		window.clear(sf::Color(255, 255, 255, 255));	
		for (int i = 0; i < columns; i++)
		{
			for (int j = 0; j < rows; j++)
			{
				window.draw(newBoard.getBoardTiles()[i][j]);
				if (!newBoard.getTile(i, j).revealed)
				{
					sf::Sprite hidden(TextureManager::GetTexture("tile_hidden"));
					hidden.setPosition(newBoard.getBoardTiles()[i][j].getPosition());
					window.draw(hidden);
				}
				if (newBoard.getTile(i, j).flagged)
				{
					sf::Sprite flag(TextureManager::GetTexture("flag"));
					flag.setPosition(newBoard.getBoardTiles()[i][j].getPosition());
					window.draw(flag);
					
				}
				if (newBoard.getTile(i, j).bombed && newBoard.getTile(i, j).revealed == true)
				{
					sf::Sprite bomb(TextureManager::GetTexture("mine"));
					bomb.setPosition(newBoard.getBoardTiles()[i][j].getPosition());
					window.draw(bomb);
				}
				if (newBoard.getTile(i, j).bombed && !newBoard.getTile(i, j).revealed)
				{
					if (newBoard.getDebug())
					{
						sf::Sprite bomb(TextureManager::GetTexture("mine"));
						bomb.setPosition(newBoard.getBoardTiles()[i][j].getPosition());
						window.draw(bomb);
					}
				
				}
				if (!newBoard.getTile(i, j).bombed && newBoard.getTile(i, j).revealed == true)
				{
					sf::Sprite Number = newBoard.getAdjacentTotal(i, j);
					Number.setPosition(newBoard.getBoardTiles()[i][j].getPosition());
					window.draw(Number);
				}
				if (newBoard.getGameState() == 1)
				{
					if (!newBoard.getTile(i, j).revealed)
					{
						sf::Sprite winFlag(TextureManager::GetTexture("flag"));
						winFlag.setPosition(newBoard.getBoardTiles()[i][j].getPosition());
						window.draw(winFlag);
						newBoard.setFlagCount(newBoard.getMineCount());
					}
						
				}
				

			
			}
		}
		for (int i = 0; i < 3; i++)
		{
			window.draw(testButtons[i]);
		}
		if (newBoard.getGameState() == 0)
			window.draw(happyFace);
		if (newBoard.getGameState() == 1)
			window.draw(winFace);
		if (newBoard.getGameState() == 2)
			window.draw(sadFace);
		if (newBoard.getTilesRev() == (rows * columns) - newBoard.getMineCount() && newBoard.getGameState() == 0)
		{
			if (newBoard.getDebug())
				newBoard.setDebug();
			newBoard.setGameState(1);
		}
		counter(newBoard.getMineCount() - newBoard.getFlagCount(), digits);
		for (int i = 0; i < 4; i++)
		{
			window.draw(digits[i]);
		}
		window.draw(debug);
		window.display();

	}


	TextureManager::Clear();

	return 0;

}