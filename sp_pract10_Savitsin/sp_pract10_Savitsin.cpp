#include <iostream>
#include <conio.h>
#include <windows.h>

struct SegmentSnake
{
	int x, y;
};

struct Node
{
	Node* next;
	Node* prev;

	Node(SegmentSnake segment) : next(nullptr) {}
};

class Snake
{
public:
	Node* head;	

	int length;
	int score;

	Snake()
	{
		head = nullptr;		
	}

	Node* push_front(SegmentSnake segment) //Добавление в начало
	{
		Node* ptr = new Node(segment);

		ptr->next = head;
		if (head != nullptr)
		{
			head->prev = ptr;
		}
		head = ptr;

		return ptr;
	}
};

void PrintMenu()
{
	std::cout << "1. Новая игра\n";
	std::cout << "2. Загрузить игру\n";
	std::cout << "3. Таблица рекордов\n";
	std::cout << "4. Выход\n";
}

void CreateMap(std::string map[10][20])
{	
	for (int i = 0; i < 10; i++)
	{
		map[i][0] = "#";
		map[i][19] = "#";
	}

	for (int j = 0; j < 20; j++)
	{
		map[0][j] = "#";
		map[9][j] = "#";
	}

	for (int i = 1; i < 9; i++)
	{
		for (int j = 1; j < 19; j++)
		{
			map[i][j] = " ";
		}
	}
}

void ShowMap(std::string map[10][20])
{
	for (int i = 0; i < 10; i++)
	{
		for (int j = 0; j < 20; j++)
		{
			std::cout << map[i][j];
		}
		std::cout << std::endl;
	}
}

void SpawnSnake(std::string map[10][20])
{
	map[5][7] = "$";
	map[5][8] = "$";
	map[5][9] = "$";
	map[5][10] = "$";
	map[5][11] = "$";	
}

void SpawnApple(std::string map[10][20])
{	
	int i = 1 + rand() % 9;
	int j = 1 + rand() % 19;

	while (map[i][j] != " ")
	{
		i = 1 + rand() % 9;
		j = 1 + rand() % 19;
	}

	map[i][j] = "*";
}

void GameProcess(std::string map[10][20])
{
	for (int i = 1; i < 9; i++)
	{
		for (int j = 1; j < 19; j++)
		{

		}
	}
}

int main()
{
	setlocale(LC_ALL, "Russian");
	srand(time(NULL));

	Snake snake;

	std::string map[10][20];

	bool exit = false;	

	while (exit == false)
	{
		PrintMenu();

		std::cout << "Выберите пункт меню: ";
		int choice;
		std::cin >> choice;

		switch (choice)
		{
		case 1: //Новая игра
			CreateMap(map);
			SpawnSnake(map);
			SpawnApple(map);
			ShowMap(map);
			break;
		case 2: //Загрузить игру

			break;
		case 3: //Таблица рекордов

			break;
		case 4: //Выход
			exit = true;
			break;
		default:
			std::cout << "Такого пункта меню нет\n";
			break;
		}
	}
}
