#pragma once
#include <string>
#include <cmath>
#include "Ship.hpp"

namespace mt
{
	const float pi = acos(-1); // Константа пи

	class Game
	{
		int m_width; // Длина окна
		int m_height; // Ширина окна
		std::string m_capture; // Заголовок окна
		mt::Circle* m_c; // Указатель на массив кругов 
		bool* m_col; // Массив, для указания коллизии кругов 
		int m_n; // Количество кругов
		sf::RenderWindow m_window; // Объкт для отрисовки окна
		sf::Texture m_textureBackground; // Текстура фона
		sf::Sprite m_spriteBackground; // Спрайт фона
		Ship m_ship; // Объект класса корбль
		sf::Texture m_AsteroidTexture; // Текстура астероида

	public:
		Game(int width, int height, const std::string& capture) // Конструктор инициализирует длину, ширину и заголовок окна
		{
			m_width = width; // Устанавливаем дину окна
			m_height = height; // Устанавливае ширину окна
			m_capture = capture; // Устанавливаем заголовок окна 
		}

		bool Setup(int n) // Метод для настройки объектов
		{
			m_n = n; // Количество астероидов
			m_window.create(sf::VideoMode(m_width, m_height), m_capture); // Создание окна
			if (!m_textureBackground.loadFromFile("assets\\background.jpg")) // Загрузка фона
			{
				std::cout << "Error while loading background.jpg" << std::endl;
				return false; // Ошибка, если фон не загружается
			}
			m_spriteBackground.setTexture(m_textureBackground); // Установка текстуры фона
			if (!m_AsteroidTexture.loadFromFile("assets\\asteroid.png")) // Загрузка текстуры астероида
			{
				std::cout << "Error while loading asteroid.png" << std::endl;
				return false; // Ошибка, если текстура не загрузилась 
			}

			if (!m_ship.Setup(100, 100)) // Расстояние спавна корабля от левого верхнего угла
				return false;

			srand(time(0)); // Генератор сулчайных чисел 

			m_c = new mt::Circle[m_n]; // Выделение памяти под массив астероидов
			m_col = new bool[m_n]; // Выделение паямяти под массив 

			for (int i = 0; i < m_n; i++) // Цикл, настраивающий круги
			{
				int r = rand() % 50 + 15;// Случайный радиус круга от 15 до 50
				int x, y; // Координаты круга
				bool Position = false; // Булевая переменная для позиции круга

				while (!Position) // Цикл, для того, чтобы круги не накладывались друг на друга и не выходили за границу при генерации
				{
					x = rand() % (1920 - 2 * r) + r; // Генерация координаты X с учётом радиуса
					y = rand() % (1080 - 2 * r) + r; // Генерация координаты Y с учётом радиуса
					Position = true; // Место допустимо 

					for (int j = 0; j < i; j++) // Проверка на наложение кругов
					{
						float dist = sqrt(pow(m_c[j].X() - x, 2) + pow(m_c[j].Y() - y, 2)); // Вычисление расстояния между кругами
						if (dist < m_c[j].R() + r) // Если оно меньше суммы радиусов
						{
							Position = false; // Значит место недопустимо 
							break; // Выход из цикла
						}
						float distToShip = sqrt(pow(m_ship.X() - x, 2) + pow(m_ship.Y() - y, 2)); // Вычисление расстояния от астероида до корабля
						if (distToShip < m_ship.Radius() + r + 20)  // Если астероид находится слишком близко к кораблю
						{
							Position = false;// То астероид нужно сгенерировать дальше от корабля
							continue;
						}
					}
				}
				m_c[i].Setup(x, y, r, m_AsteroidTexture); // Инициализация астероида с заданными параметрами
				m_col[i] = true; // Устанавливаем статус коллизии для круга
			}
			return true;
		}
		void CheckBulletCollisionWithCircles() // Метод для проверки столконовения пули с астероидом
		{
			for (int i = 0; i < m_n; i++)
			{
				if (m_col[i] && m_ship.BulletAvailable()) // Если астероид активен и пуля доступна
				{
					float dx = m_ship.GetBullet().getPosition().x - m_c[i].X(); 
					float dy = m_ship.GetBullet().getPosition().y - m_c[i].Y();
					float distance = sqrt(dx * dx + dy * dy); // Вычисляем расстояние между пулей и астероидом

					if (distance < m_c[i].R() + m_ship.GetBullet().getRadius()) // Если расстояние меньши суммы радиусов
					{
						m_col[i] = false; // Деактивируем астероид 
						m_ship.BulletReset(); // Перезарежаем пулю
					}
				}
			}
		}
		bool CheckCollisionWithCircles() // Метод для проверки коллизии корабля и астероида
		{
			for (int i = 0; i < m_n; i++)
			{
				if (m_col[i]) // Если астероид активен
				{
					float dist = sqrt(pow(m_ship.X() - m_c[i].X(), 2) + pow(m_ship.Y() - m_c[i].Y(), 2)); // Вычисляем расстояние между кораблём и астероидом
					if (dist < m_ship.Radius() + m_c[i].R()) // Если расстояние меньши суммы радиусов
					{
						m_ship.RepelFrom(m_c[i].X(), m_c[i].Y()); // Отталкиваем корабль от астероида
						return true;  
					}
				}
			}
			return false;
		}
		void LifeCycle() // Метод для отображения игры
		{
			sf::Clock clock; // Создаём объект класса для clock для отслеживания времени
			float fps = 100; // Частота кадров
			//int fpsTextViewCnt = 0;

			while (m_window.isOpen()) // Пока открыто окно
			{
				sf::Event event;
				while (m_window.pollEvent(event)) // Обрабатывать события 
				{
					if (event.type == sf::Event::Closed) // Если событие - закрыть окно
						m_window.close(); // Закрываем онно
				}
				if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up)) // Кнопка "вверх" увеличивает скорость корабля
				{
					m_ship.setVelocity(0.003);
				}
				if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down)) // Конопка "вниз" уменьшает скорость корабля
				{
					m_ship.setVelocity(-0.008);
				}
				if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left)) // Кнопка "влеово" поворачивает корабль влево
				{
					m_ship.Rotate(-1);
				}
				if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right)) // Кнопка "вправо" поворачивает корабль вправо
				{
					m_ship.Rotate(1);
				}
				if (sf::Keyboard::isKeyPressed(sf::Keyboard::Space)) // Кнорпка "пробел" стреляет 
				{
					if (!m_ship.BulletAvailable()) // Если плуя не активна
						m_ship.Attack(); // Тогда можео стрелять
				}
				if (sf::Keyboard::isKeyPressed(sf::Keyboard::R)) // Кнопка "R" перезарядка и деактвация летящей пули
				{
					m_ship.BulletReset();
				}


				// Эта часть кода предназначена для установления фиксированной(почти) частоты кадров, чтобы на разных компьютерах скорость корабля была одинаквой
				float dt = clock.getElapsedTime().asSeconds();
				if (1 / fps > dt)
				{
					sf::Time t = sf::seconds(1 / fps - dt);
					sf::sleep(t);
				}
				dt = clock.getElapsedTime().asSeconds();
				clock.restart();

				m_ship.Move(dt); // Вызов функции перемещения корабля
				m_ship.UpdateBulletPosition(dt, m_width, m_height); // Обновление позиции пули
				CheckBulletCollisionWithCircles(); // Проверка столкновения пули с астероидом

				m_window.clear(); // Очистка окна
				m_window.draw(m_spriteBackground); // Отображение фона
				m_window.draw(m_ship.Get()); // Отображение корабля

				for (int i = 0; i < m_n; i++) // Отображение астероидов
				{
					if (m_col[i])
						m_window.draw(m_c[i].Get());
				}

				if (CheckCollisionWithCircles()) // Проверка коллизии между кораблём и астероидом
					m_ship.ReverSMove(); // Обратное движение

				if (m_ship.BulletAvailable()) // Отображение пули, если она доступна
					m_window.draw(m_ship.GetBullet());

				m_window.display();
			}
		}
	};
}

