#include <iostream>
#include <conio.h>
#include <windows.h>

struct SegmentSnake
{
	int x, y;
};

struct Node
{
	SegmentSnake segment;
	Node* next;	

	Node(SegmentSnake segment) : segment(segment), next(nullptr) {}
};

class Snake
{
public:
	Node* head;	

	int length = 5;
	int score = 0;

	Snake()
	{
		head = nullptr;		
	}
public:
	Node* push_front(SegmentSnake segment) //Добавление в начало
	{
		Node* ptr = new Node(segment);

		if (head != nullptr)
		{
			ptr->next = head;
		}

		head = ptr;

		return ptr;
	}
	void GameProcess(std::string map[10][20])
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

		SpawnSnake();
		SpawnApple(map);
		ShowMap(map);

		if (_kbhit)
		{
			switch (_getch())
			{
			case 'w':
				for (Node* ptr = this->head; ptr != nullptr; ptr = ptr->next)
				{
					ptr->segment.y++;
				}
				break;
			case 'a':
				for (Node* ptr = this->head; ptr != nullptr; ptr = ptr->next)
				{
					ptr->segment.x--;
				}
				break;
			case 's':
				for (Node* ptr = this->head; ptr != nullptr; ptr = ptr->next)
				{
					ptr->segment.y--;
				}
				break;
			case 'd':
				for (Node* ptr = this->head; ptr != nullptr; ptr = ptr->next)
				{
					ptr->segment.x++;
				}
				break;
			default:
				break;
			}
		}

		system("cls");		

		for (int i = 1; i < 9; i++)
		{
			int x = i;
			for (int j = 1; j < 19; j++)
			{			
				int y = j;
				for (Node* ptr = this->head; ptr != nullptr; ptr = ptr->next)
				{
					if (x == ptr->segment.x && y == ptr->segment.y)
					{
						map[i][j] = "$";
					}
					else
					{
						map[i][j] = " ";
					}
				}
				if (map[i][j] != " " || map[i][j] != "*")
				{ 
					map[i][j] = " ";
				}
			}
		}

		ShowMap(map);
	}

	void SpawnSnake()
	{
		int y = 7;
		Node* ptr = this->head;
		for (int i = 0; i < this->length; i++)
		{
			ptr->segment.x = 5;
			ptr->segment.y = y;			
			push_front(ptr->segment);
			ptr = ptr->next;
			y++;
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
};

void PrintMenu()
{
	std::cout << "1. Новая игра\n";
	std::cout << "2. Загрузить игру\n";
	std::cout << "3. Таблица рекордов\n";
	std::cout << "4. Выход\n";
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
			snake.GameProcess(map);						
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
