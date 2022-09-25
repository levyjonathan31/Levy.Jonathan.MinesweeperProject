#include "Board.h"
#include <vector>
#include <fstream>
#include <string> 
#include <iostream>
#include "Random.h"
using std::string;
Board::Board(int r, int c, int mines)
{
	this->c = c;
	this->r = r;
	this->mines = mines;
	tilesRev = 0;
	// Creates Visual Board
	LoadSprites(r, c);
	// Puts Empty Tiles in
	LoadTiles(r, c);
	// Adds bombs
	int count = 0;
	while (count < mines)
	{
		int rY = Random::Int(0, r - 1);
		int rX = Random::Int(0, c - 1);
		if (!board[rX][rY].bombed)
		{
			board[rX][rY].bombed = true;
			count++;
		}

	}
	// Points to Adjacent Tiles
	LoadNumbers(r, c);

}
void Board::LoadSprites(int r, int c)
{
	std::vector<std::vector<sf::Sprite>> tiles;
	for (int i = 0; i < c; i++)
	{
		std::vector<sf::Sprite> tileRow;
		for (int j = 0; j < r; j++)
		{
			sf::Sprite HiddenTile(TextureManager::GetTexture("tile_hidden"));
			HiddenTile.setPosition(32 * i, 32 * j);
			tileRow.push_back(HiddenTile);
		}
		tiles.push_back(tileRow);
	}

	this->sprTiles = tiles;
}
void Board::LoadTiles(int r, int c)
{
	std::vector<std::vector<Tile>> tempBoard;
	for (int i = 0; i < c; i++)
	{
		std::vector<Tile> tempVec;
		for (int j = 0; j < r; j++)
		{
			Tile newTile;
			newTile.x = j;
			newTile.y = i;
			tempVec.push_back(newTile);
		}
		tempBoard.push_back(tempVec);
	}
	this->board = tempBoard;
}
void Board::LoadNumbers(int r, int c)
{
	for (int i = 0; i < c; i++)
	{
		for (int j = 0; j < r; j++)
		{

			if (i > 0 && j < r - 1)
			{
				board[i][j].botLeft = &board[i - 1][j + 1];
			}
			if (j < r - 1)
			{
				board[i][j].botMid = &board[i][j + 1];
			}
			if (i < c - 1 && j < r - 1)
			{
				board[i][j].botRight = &board[i + 1][j + 1];
			}
			if (i > 0)
			{
				board[i][j].midLeft = &board[i - 1][j];
			}
			if (i < c - 1)
			{
				board[i][j].midRight = &board[i + 1][j];
			}
			if (i > 0 && j > 0)
			{
				board[i][j].topLeft = &board[i - 1][j - 1];
			}
			if (j > 0)
			{
				board[i][j].topMid = &board[i][j - 1];
			}
			if (i < c - 1 && j > 0)
			{
				board[i][j].topRight = &board[i + 1][j - 1];
			}
		}
	}
}

std::vector<std::vector<sf::Sprite>>& Board::getBoardTiles()
{
	return sprTiles;
}


std::vector<std::vector<Tile>> &Board::getBoardInfo()
{
	return board;
}
Tile Board::getTile(int i, int j)
{
	return board[i][j];
}

bool Board::getDebug()
{
	return debugMode;
}

int Board::getGameState()
{
	return gameState;
}

int Board::getTilesRev()
{
	return tilesRev;
}

int Board::getMineCount()
{
	return mines;
}
void Board::RevealTile(int i, int j)
{ 
	if (!board[i][j].flagged)
	{
		if (debugMode && board[i][j].bombed)
		{
			board[i][j].revealed = false;
		}

		else
		{
			if (!board[i][j].revealed)
			{
				sf::Sprite ShownTile(TextureManager::GetTexture("tile_revealed"));
				ShownTile.setPosition(sprTiles[i][j].getPosition());
				sprTiles[i][j] = ShownTile;
				board[i][j].revealed = true;
				tilesRev++;
			}
		
		}
		if (!board[i][j].numbered && !board[i][j].bombed)
		{
			if (getAdjacentNumber(i, j) == 0)
			{
				std::vector<Tile*> adjs = { board[i][j].botLeft, board[i][j].botMid, board[i][j].botRight, board[i][j].midLeft, board[i][j].midRight, board[i][j].topLeft, board[i][j].topMid, board[i][j].topRight };
				for (int k = 0; k < adjs.size(); k++)
				{
					if (adjs[k] != nullptr)
					{
						if (!adjs[k]->revealed && !adjs[k]->bombed)
						{
							RevealTile(adjs[k]->y, adjs[k]->x);
						}
					}
				}
			}
		}
		if (board[i][j].bombed && !debugMode && gameState == 0 )
		{
			gameState = 2;
			showAllMines();
		}
	}
}

void Board::AddFlag(int i, int j)
{ 
if (board[i][j].revealed == false)
{ 
	if (board[i][j].flagged)
	{
		board[i][j].flagged = false;
		flagCount--;
	}
	else
	{
		board[i][j].flagged = true;
		flagCount++;
	}
}

}

sf::Sprite Board::getAdjacentTotal(int i, int j)
{
	int count = 0;
	std::vector<Tile*> adjs = { board[i][j].botLeft, board[i][j].botMid, board[i][j].botRight, board[i][j].midLeft, board[i][j].midRight, board[i][j].topLeft, board[i][j].topMid, board[i][j].topRight };
	for (int k = 0; k < adjs.size(); k++)
	{
		if (adjs[k] != nullptr)
		{
			if (adjs[k]->bombed == true)
			{
				count++;
			}
		}
	}
	if (count > 0)
	{
		sf::Sprite NumberTile(TextureManager::GetTexture("number_" + std::to_string(count)));
		board[i][j].numbered = true;
		return NumberTile;
	}
	else
	{
		sf::Sprite BlankTile(TextureManager::GetTexture("tile_revealed"));
		board[i][j].numbered = false;
		return BlankTile;
	}

}

int Board::getAdjacentNumber(int i, int j)
{
	int count = 0;
	std::vector<Tile*> adjs = { board[i][j].botLeft, board[i][j].botMid, board[i][j].botRight, board[i][j].midLeft, board[i][j].midRight, board[i][j].topLeft, board[i][j].topMid, board[i][j].topRight };
	for (int k = 0; k < adjs.size(); k++)
	{
		if (adjs[k] != nullptr)
		{
			if (adjs[k]->bombed == true)
			{
				count++;
			}
		}
	}
	return count;

}

void Board::setDebug()
{
	if (gameState == 0)
	{
		if (debugMode)
			debugMode = false;
		else
			debugMode = true;
	}

}

void Board::setGameState(int i)
{
	gameState = i;
}

void Board::showAllMines()
{
	for (int i = 0; i < c; i++)
		{
			for (int j = 0; j < r; j++)
			{
				if (board[i][j].bombed)
				{
						RevealTile(i, j);
				}

			}
		}
}

int Board::getFlagCount()
{
	return flagCount;
}

void Board::setFlagCount(int i)
{
	flagCount = i;
}
void Board::loadFromFile(string path)
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

	std::ifstream testFile;
	testFile.open(path);
	string line;
	int rows2 = 0;
	int columns2 = 0;
	std::vector<std::vector<int>> bombChar;
	tilesRev = 0;
	gameState = 0;
	flagCount = 0;
while (getline(testFile, line))
	{
		rows2++;
		std::vector<int> subVec;
		for (int i = 0; i < line.length(); i++)
		{
			subVec.push_back((int)line[i]-48);
		}
		bombChar.push_back(subVec);
	}
	columns2 = line.length();
	
	if (columns == columns2 && rows == rows2)
	{
		std::cout << "Ready to Test!" << std::endl;
		mines = 0;
		for (int j = 0; j < columns; j++)
		{
			for (int i = 0; i < rows; i++)
			{
				board[j][i].revealed = false;
				board[j][i].numbered = false;
				board[j][i].flagged = false;
				if (bombChar[i][j] == 1)
				{
					board[j][i].bombed = true;
					mines++;
				}
				else
					board[j][i].bombed = false;
					
			}
		}
	}
	else
	{
		std::cout << "Error! Cannot test, config and test bounds do not match." << std::endl;
		std::cout << "Config: " << columns << ", " << rows << std::endl;
		std::cout << "Board: " << columns2 << ", " << rows2 << std::endl;
	}
}