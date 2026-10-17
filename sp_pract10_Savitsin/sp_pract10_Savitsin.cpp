#include <iostream>
#include <conio.h>
#include <windows.h>
#include <fstream>

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

struct Record
{
	char name[32];
	int score;
};

class Snake
{
public:
	Node* head;

	int length = 5;
	int score = 0;

	int appleX = -1;
	int appleY = -1;

	char lastKey = 'w';

	Snake()
	{
		head = nullptr;
	}

	Node* push_front(SegmentSnake segment)
	{
		Node* ptr = new Node(segment);

		if (head != nullptr)
		{
			ptr->next = head;
		}

		head = ptr;

		return ptr;
	}

	Node* push_back(SegmentSnake segment)
	{
		Node* ptr = new Node(segment);

		if (head == nullptr)
		{
			head = ptr;
			return ptr;
		}

		Node* last = head;
		while (last->next != nullptr)
		{
			last = last->next;
		}
		last->next = ptr;
		return ptr;
	}

	void SaveGame()
	{
		std::ofstream file("save.bin", std::ios::binary);

		if (!file.is_open())
		{
			return;
		}

		file.write(reinterpret_cast<const char*>(&score), sizeof(score));
		file.write(reinterpret_cast<const char*>(&lastKey), sizeof(lastKey));
		file.write(reinterpret_cast<const char*>(&appleX), sizeof(appleX));
		file.write(reinterpret_cast<const char*>(&appleY), sizeof(appleY));

		int count = 0;
		for (Node* ptr = head; ptr != nullptr; ptr = ptr->next)
		{
			count++;
		}

		file.write(reinterpret_cast<const char*>(&count), sizeof(count));

		for (Node* ptr = head; ptr != nullptr; ptr = ptr->next)
		{
			file.write(reinterpret_cast<const char*>(&ptr->segment.x), sizeof(ptr->segment.x));
			file.write(reinterpret_cast<const char*>(&ptr->segment.y), sizeof(ptr->segment.y));
		}

		file.close();
	}

	bool LoadGame()
	{
		std::ifstream file("save.bin", std::ios::binary);

		if (!file.is_open())
		{
			return false;
		}

		Clear();

		file.read(reinterpret_cast<char*>(&score), sizeof(score));
		file.read(reinterpret_cast<char*>(&lastKey), sizeof(lastKey));
		file.read(reinterpret_cast<char*>(&appleX), sizeof(appleX));
		file.read(reinterpret_cast<char*>(&appleY), sizeof(appleY));

		int count = 0;
		file.read(reinterpret_cast<char*>(&count), sizeof(count));

		for (int i = 0; i < count; i++)
		{
			int x, y;
			file.read(reinterpret_cast<char*>(&x), sizeof(x));
			file.read(reinterpret_cast<char*>(&y), sizeof(y));

			SegmentSnake segment;
			segment.x = x;
			segment.y = y;
			push_back(segment);
		}

		file.close();
		return true;
	}

	void GameProcess(std::string map[10][20], bool loaded)
	{
		for (int i = 0; i < 10; i++)
		{
			for (int j = 0; j < 20; j++)
			{
				map[i][j] = " ";
			}
		}

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

		if (loaded == false)
		{
			Clear();

			score = 0;
			appleX = -1;
			appleY = -1;
			lastKey = 'w';

			SpawnSnake(map);
			SpawnApple(map);
		}

		ShowMap(map);

		int tick = 0;

		bool end = false;
		while (end == false)
		{
			if (_kbhit())
			{
				switch (_getch())
				{
				case 'w':
					lastKey = 'w';
					break;
				case 's':
					lastKey = 's';
					break;
				case 'a':
					lastKey = 'a';
					break;
				case 'd':
					lastKey = 'd';
					break;
				case 'k':
					SaveGame();
					break;
				default:
					break;
				}
			}

			tick++;
			if (tick >= 5)
			{
				tick = 0;

				if (lastKey == 'w')
				{
					int newX = head->segment.x - 1;
					int newY = head->segment.y;

					if (map[newX][newY] == "#")
					{
						end = true;
					}
					else
					{
						bool hitSelf = false;
						for (Node* ptr = head; ptr != nullptr; ptr = ptr->next)
						{
							if (ptr->segment.x == newX && ptr->segment.y == newY)
							{
								hitSelf = true;
								break;
							}
						}

						if (hitSelf)
						{
							end = true;
						}
						else
						{
							bool ateApple = false;
							if (newX == appleX && newY == appleY)
							{
								ateApple = true;
							}

							Node* last = head;
							while (last->next != nullptr)
							{
								last = last->next;
							}
							SegmentSnake oldTail = last->segment;

							int prevX = head->segment.x;
							int prevY = head->segment.y;
							head->segment.x = newX;
							head->segment.y = newY;

							for (Node* ptr = head->next; ptr != nullptr; ptr = ptr->next)
							{
								int tmpX = ptr->segment.x;
								int tmpY = ptr->segment.y;
								ptr->segment.x = prevX;
								ptr->segment.y = prevY;
								prevX = tmpX;
								prevY = tmpY;
							}

							if (ateApple)
							{
								push_back(oldTail);
								score += 10;
								appleX = -1;
								appleY = -1;
							}
						}
					}
				}
				else if (lastKey == 's')
				{
					int newX = head->segment.x + 1;
					int newY = head->segment.y;

					if (map[newX][newY] == "#")
					{
						end = true;
					}
					else
					{
						bool hitSelf = false;
						for (Node* ptr = head; ptr != nullptr; ptr = ptr->next)
						{
							if (ptr->segment.x == newX && ptr->segment.y == newY)
							{
								hitSelf = true;
								break;
							}
						}

						if (hitSelf)
						{
							end = true;
						}
						else
						{
							bool ateApple = false;
							if (newX == appleX && newY == appleY)
							{
								ateApple = true;
							}

							Node* last = head;
							while (last->next != nullptr)
							{
								last = last->next;
							}
							SegmentSnake oldTail = last->segment;

							int prevX = head->segment.x;
							int prevY = head->segment.y;
							head->segment.x = newX;
							head->segment.y = newY;

							for (Node* ptr = head->next; ptr != nullptr; ptr = ptr->next)
							{
								int tmpX = ptr->segment.x;
								int tmpY = ptr->segment.y;
								ptr->segment.x = prevX;
								ptr->segment.y = prevY;
								prevX = tmpX;
								prevY = tmpY;
							}

							if (ateApple)
							{
								push_back(oldTail);
								score += 10;
								appleX = -1;
								appleY = -1;
							}
						}
					}
				}
				else if (lastKey == 'a')
				{
					int newX = head->segment.x;
					int newY = head->segment.y - 1;

					if (map[newX][newY] == "#")
					{
						end = true;
					}
					else
					{
						bool hitSelf = false;
						for (Node* ptr = head; ptr != nullptr; ptr = ptr->next)
						{
							if (ptr->segment.x == newX && ptr->segment.y == newY)
							{
								hitSelf = true;
								break;
							}
						}

						if (hitSelf)
						{
							end = true;
						}
						else
						{
							bool ateApple = false;
							if (newX == appleX && newY == appleY)
							{
								ateApple = true;
							}

							Node* last = head;
							while (last->next != nullptr)
							{
								last = last->next;
							}
							SegmentSnake oldTail = last->segment;

							int prevX = head->segment.x;
							int prevY = head->segment.y;
							head->segment.x = newX;
							head->segment.y = newY;

							for (Node* ptr = head->next; ptr != nullptr; ptr = ptr->next)
							{
								int tmpX = ptr->segment.x;
								int tmpY = ptr->segment.y;
								ptr->segment.x = prevX;
								ptr->segment.y = prevY;
								prevX = tmpX;
								prevY = tmpY;
							}

							if (ateApple)
							{
								push_back(oldTail);
								score += 10;
								appleX = -1;
								appleY = -1;
							}
						}
					}
				}
				else if (lastKey == 'd')
				{
					int newX = head->segment.x;
					int newY = head->segment.y + 1;

					if (map[newX][newY] == "#")
					{
						end = true;
					}
					else
					{
						bool hitSelf = false;
						for (Node* ptr = head; ptr != nullptr; ptr = ptr->next)
						{
							if (ptr->segment.x == newX && ptr->segment.y == newY)
							{
								hitSelf = true;
								break;
							}
						}

						if (hitSelf)
						{
							end = true;
						}
						else
						{
							bool ateApple = false;
							if (newX == appleX && newY == appleY)
							{
								ateApple = true;
							}

							Node* last = head;
							while (last->next != nullptr)
							{
								last = last->next;
							}
							SegmentSnake oldTail = last->segment;

							int prevX = head->segment.x;
							int prevY = head->segment.y;
							head->segment.x = newX;
							head->segment.y = newY;

							for (Node* ptr = head->next; ptr != nullptr; ptr = ptr->next)
							{
								int tmpX = ptr->segment.x;
								int tmpY = ptr->segment.y;
								ptr->segment.x = prevX;
								ptr->segment.y = prevY;
								prevX = tmpX;
								prevY = tmpY;
							}

							if (ateApple)
							{
								push_back(oldTail);
								score += 10;
								appleX = -1;
								appleY = -1;
							}
						}
					}
				}
			}

			system("cls");

			for (int i = 1; i < 9; i++)
			{
				for (int j = 1; j < 19; j++)
				{
					map[i][j] = " ";
				}
			}

			for (Node* ptr = head; ptr != nullptr; ptr = ptr->next)
			{
				map[ptr->segment.x][ptr->segment.y] = "$";
			}

			if (appleX >= 0 && appleY >= 0)
			{
				map[appleX][appleY] = "*";
			}

			ShowMap(map);

			if (appleX == -1)
			{
				SpawnApple(map);
			}

			Sleep(20);
		}
	}

	void Clear()
	{
		while (head != nullptr)
		{
			Node* ptr = head;
			head = head->next;
			delete ptr;
		}
	}

	void SpawnSnake(std::string map[10][20])
	{
		int y = 7;
		for (int i = 0; i < this->length; i++)
		{
			SegmentSnake segment;
			segment.x = 5;
			segment.y = y;
			map[segment.x][segment.y] = "$";
			push_front(segment);
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
		int i = 1 + rand() % 8;
		int j = 1 + rand() % 18;

		while (map[i][j] != " ")
		{
			i = 1 + rand() % 8;
			j = 1 + rand() % 18;
		}

		appleX = i;
		appleY = j;
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

void SaveRecord(int score)
{
	char name[32];
	std::cout << "Введите имя: ";
	std::cin >> name;

	Record rec;
	for (int i = 0; i < 32; i++)
	{
		rec.name[i] = 0;
	}
	rec.score = score;

	for (int i = 0; i < 32; i++)
	{
		rec.name[i] = name[i];
		if (name[i] == '\0') break;
	}

	std::ofstream file("records.bin", std::ios::binary | std::ios::app);

	if (!file.is_open())
	{
		return;
	}

	file.write(reinterpret_cast<const char*>(&rec), sizeof(rec));
	file.close();
}

void ShowRecords()
{
	std::ifstream file("records.bin", std::ios::binary);

	if (!file.is_open())
	{
		std::cout << "Файл рекордов пуст\n";
		return;
	}

	Record rec;
	while (file.read(reinterpret_cast<char*>(&rec), sizeof(rec)))
	{
		std::cout << rec.name << " - " << rec.score << "\n";
	}

	file.close();
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
		case 1:
			snake.GameProcess(map, false);
			SaveRecord(snake.score);
			break;
		case 2:
			if (snake.LoadGame() == true)
			{
				snake.GameProcess(map, true);
				SaveRecord(snake.score);
			}
			else
			{
				std::cout << "Файл сохранения не найден\n";
			}
			break;
		case 3:
			ShowRecords();
			break;
		case 4:
			exit = true;
			break;
		default:
			std::cout << "Такого пункта меню нет\n";
			break;
		}
	}
}