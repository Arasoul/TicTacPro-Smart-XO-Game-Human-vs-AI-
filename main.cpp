#include <iostream>
#include <vector>

using namespace std;

enum Difficulty {
	Easy,
	Hard
};


class Board
{
private:
	vector<vector<char>> grid;
	int size;
public:
	Board(int size)
	{
		//Constructor
		vector<vector<char>> map(size, vector<char>(size, ' '));
		grid = map;
		this->size = size;
	}
	void display()
	{
		//Prints Formated board to console
		cout << "   1   2   3" << endl;
		for (int row = 0; row < size; row++)
		{
			cout << row + 1 << "  ";
			for (int col = 0; col < size; col++)
			{
				cout << grid[row][col];
				if (col < size - 1) cout << " | ";
			}
			cout << endl;
			if (row < size - 1) cout << "  ---+---+---" << endl;
		}
	}
	bool makeMove(int row, int col, char symbol)
	{
		//Boolean indecate move success
		if (isValidMove(row,col))
		{
			grid[row][col] = symbol;
			return true;
		}
		return false;
	}
	bool isValidMove(int row, int column)
	{
		//Boolean indecates move validity
		if (row >= 0 && row < size && column >= 0 && column < size && grid[row][column] == ' ')
		{
			return true;
		}
		return false;
	}
	bool checkWin(char symbol)const
	{
		//Checks all WIN conditions
		//check rows
		for (int r = 0; r < size; r++)
		{
			bool win = true;
			for (int c = 0; c < size; c++)
			{
				if (grid[r][c] != symbol)
				{
					win = false;
					break;
				}
			}
			if (win) return true;
		}
		//check column
		for (int r = 0; r < size; r++)
		{
			bool win = true;
			for (int c = 0; c < size; c++)
			{
				if (grid[c][r] != symbol)
				{
					win = false;
					break;
				}
			}
			if (win) return true;
		}
		//check diagonal 1
		
			bool win = true;
			for (int c = 0; c < size; c++)
			{
				if (grid[c][c] != symbol)
				{
					win = false;
					break;
				}
			}
			if (win) return true;
		
		//check diagonal 2
			win = true;
			for (int r = 0; r < size; r++)
			{
				if (grid[r][size - 1 - r] != symbol)
				{
					win = false;
					break;
				}
			}
			if (win) return true;

			return false;

	}
	bool isFull() const
	{
		//Checks if all cells are occupied 
		for (int row = 0; row < size; row++)
		{
			for (int col = 0; col < size; col++)
			{
				if (grid[row][col] == ' ')
				{
					return false;
				}
			}
		}
		return true;
	}
	char getCell(int row, int col)const
	{
		//Returns the symbol
		return grid[row][col];
	}
	void reset()
	{
		//Clears all cells
		for (int row = 0; row < size; row++)
		{
			for (int col = 0; col < size; col++)
			{
				grid[row][col] = ' ';
			}
		}
	}
	int getSize()const
	{
		//Return the dimension of the board
		return size;
	}
};

class Player
{
private:
	string PName;
	char PSymbol;
public:
	Player(const string& name, char symbol)
	{
		//Initialize player
		PName = name;
		PSymbol = symbol;
	}

	virtual void getMove(const Board& board, int& row, int& col) = 0;

	string getName()const
	{
		//Return Name
		return PName;
	}
	char getSymbol()const
	{
		//Return Symbol
		return PSymbol;
	}
	void setName(const string& name)
	{
		//Update Player Name
		PName = name;
	}
	virtual ~Player() {}
};

class HumanPlayer : public Player
{
public:
	HumanPlayer(const string& name, char symbol) : Player(name, symbol) {}

	void getMove(const Board& board, int& row, int& col) override {
		cout << getName() << " enter your move (row and column): ";
		cin >> row >> col;
		row--; col--;  // convert to 0-based indexing
	}
};

class AIPlayer : public Player
{
private:
	Difficulty difficulty;
public:
	AIPlayer(const string& name, char symbol, Difficulty difficulty) : Player(name, symbol), difficulty(difficulty)
	{
		//Initialize AIPlayer
	}
	void getMove(const Board& board, int& row, int& col) override {
		if (difficulty == Easy)
			getRandomMove(board, row, col);
		else
			getBestMove(board, row, col);

		cout << getName() << " chooses (" << row + 1 << ", " << col + 1 << ")" << endl;
	}
	void setDifficulty(Difficulty d)
	{
		//Change AI Difficulty
		difficulty = d;
	}
	void getRandomMove(const Board& board , int& row, int& col)const
	{
		//Select Random valid move 
		int b_size = board.getSize();
		vector<pair<int, int>> emptyboard;

		for (int i = 0; i < b_size; i++)
		{
			for (int k = 0; k < b_size; k++)
			{
				if (board.getCell(i, k) == ' ')
				{
					emptyboard.push_back({ i,k });
				}
			}
		}
		if (emptyboard.size())
		{
			int index = rand() % emptyboard.size();
			row = emptyboard[index].first;
			col = emptyboard[index].second;
		}
	}
	void getBestMove(const Board& board ,int& row, int& col)const
	{
		//Find Optimal move
		int b_size = board.getSize();
		int bestvalue = -10;
		int brow = -1;
		int bcol = -1;

		for (int i = 0; i < b_size; i++)
		{
			for (int k = 0; k < b_size; k++)
			{
				if (board.getCell(i, k) == ' ')
				{
					Board tempboard = board;
					tempboard.makeMove(i, k, getSymbol());
					int score = evaluateBoard(tempboard);

					if (score > bestvalue)
					{
						bestvalue = score;
						brow = i;
						bcol = k;
					}
				}
			}
		}
		row = brow;
		col = bcol;
	}
	int evaluateBoard(const Board& board)const
	{
		//Evaluate board state
		if (board.checkWin(getSymbol())) 
		{ 
			return +1; 
		}
		char opponent = (getSymbol() == 'X') ? 'O' : 'X';
		if (board.checkWin(opponent)) 
		{ 
			return -1; 
		}
		if (board.isFull()) 
		{ 
			return 0; 
		}
		return 0;
	}
};

class Game
{
private:
	Board board;
	Player* player1;
	Player* player2;
	Player* currentPlayer;
	Player* aiPlayer;
	bool isPVP;
	bool gameOver;
public:
	Game() : board(3)
	{
		//Initialzes game
	}
	void start()
	{
		//Main game entry point
		gameOver = false;
		while (!gameOver)
		{
			board.display();
			cout << currentPlayer->getName() << "'s turn (" << currentPlayer->getSymbol() << ")" << endl;

			int row, col;
			if (isPVP || currentPlayer == player1)
			{
				handleHumanMove(*currentPlayer);
			}
			else
			{
				handleAIMove(*aiPlayer);
			}


			if (board.checkWin(currentPlayer->getSymbol()))
			{
				board.display();
				cout << currentPlayer->getName() << " wins!" << endl;
				gameOver = true;
			}
			else if (board.isFull())
			{
				board.display();
				cout << "It's a draw!" << endl;
				gameOver = true;
			}
			else
			{
				switchPlayer();
			}
		}
	}
	int showMenu()
	{
		//Display mode selection menu
		cout << "TIC - TAC - TOE GAME" << endl;
		cout << "==========================" << endl;
		cout << "1.Player vs Player" << endl;
		cout << "2.Player vs Computer (Easy)" << endl;
		cout << "3.Player vs Computer (Hard)" << endl;
		cout << "4.Exit" << endl << endl;
		cout << "Select game mode : ";

		int mode;
		cin >> mode;
		return mode;
	}
	void setupPVP()
	{
		//Configure player vs player mode
		string name1, name2;
		cout << "Enter name for Player 1 (X): ";
		cin >> name1;
		cout << "Enter name for Player 2 (O): ";
		cin >> name2;

		player1 = new HumanPlayer(name1, 'X');
		player2 = new HumanPlayer(name2, 'O');
		currentPlayer = player1;
		isPVP = true;
	}
	void setupPVC(Difficulty difficulty)
	{
		//Configure player vs computer mode
		string name;
		cout << "Enter your name (X): ";
		cin >> name;

		player1 = new HumanPlayer(name, 'X');
		aiPlayer = new AIPlayer("Computer", 'O', difficulty);
		player2 = aiPlayer;
		currentPlayer = player1;
		isPVP = false;
	}
	void switchPlayer()
	{
		//Alternate current Player
		if (currentPlayer == player1) {
			currentPlayer = player2;
		}
		else {
			currentPlayer = player1;
		}
	}
	void handleHumanMove(Player& player)
	{
		//Processes human player input
		int row, col;
		player.getMove(board,row, col);

		while (!board.makeMove(row, col, player.getSymbol())) {
			cout << "Cell already taken! Try again: ";
			player.getMove(board,row, col);
		}
	}
	void handleAIMove(Player& aiPlayer)
	{
		//Excute AI move
		int row, col;
		aiPlayer.getMove(board, row, col);
		board.makeMove(row, col, aiPlayer.getSymbol());
		cout << aiPlayer.getName() << " plays (" << row + 1 << ", " << col + 1 << ")" << endl;
	}
	bool checkGameEnd()
	{
		//Checks win conditions
	}
	void displayResult()
	{
		//Shows game outcome message
		cout << "Game Over!" << endl;
	}
	void reset()
	{
		//Prepares game for new round 
		board.reset();
		currentPlayer = player1; // Start again with player1
		gameOver = false;
	}
};

int main()
{
	Game game;
	bool playAgain = true;
	while (playAgain) {
		int mode = game.showMenu();

		switch (mode) {
		case 1:
			game.setupPVP();
			break;
		case 2:
			game.setupPVC(Easy);
			break;
		case 3:
			game.setupPVC(Hard);
			break;
		case 4:
			cout << "Goodbye!" << endl;
			return 0;
		default:
			cout << "Invalid choice. Please try again." << endl;
			continue;
		}
		game.start();
		game.displayResult();
		string again;
		cout << " Do you want to play again ? ( yes or no )" << endl;
		cin >> again;
		if (again != "yes") {
			playAgain = false;
			game.reset();
			cout << "see you next time" << endl;
		}
		else {
			cout << "____start a new game____" << endl;
			game.reset(); // loop will show the menue again 
		}
	}

	return 0;
}