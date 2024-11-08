#pragma once
#include <string>
#include <cmath>
#include "Ship.hpp"

namespace mt
{
	const float pi = acos(-1); // ��������� ��

	class Game
	{
		int m_width; // ����� ����
		int m_height; // ������ ����
		std::string m_capture; // ��������� ����
		mt::Circle* m_c; // ��������� �� ������ ������ 
		bool* m_col; // ������, ��� �������� �������� ������ 
		int m_n; // ���������� ������
		sf::RenderWindow m_window; // ����� ��� ��������� ����
		sf::Texture m_textureBackground; // �������� ����
		sf::Sprite m_spriteBackground; // ������ ����
		Ship m_ship; // ������ ������ ������
		sf::Texture m_AsteroidTexture; // �������� ���������

	public:
		Game(int width, int height, const std::string& capture) // ����������� �������������� �����, ������ � ��������� ����
		{
			m_width = width; // ������������� ���� ����
			m_height = height; // ������������ ������ ����
			m_capture = capture; // ������������� ��������� ���� 
		}

		bool Setup(int n) // ����� ��� ��������� ��������
		{
			m_n = n; // ���������� ����������
			m_window.create(sf::VideoMode(m_width, m_height), m_capture); // �������� ����
			if (!m_textureBackground.loadFromFile("assets\\background.jpg")) // �������� ����
			{
				std::cout << "Error while loading background.jpg" << std::endl;
				return false; // ������, ���� ��� �� �����������
			}
			m_spriteBackground.setTexture(m_textureBackground); // ��������� �������� ����
			if (!m_AsteroidTexture.loadFromFile("assets\\asteroid.png")) // �������� �������� ���������
			{
				std::cout << "Error while loading asteroid.png" << std::endl;
				return false; // ������, ���� �������� �� ����������� 
			}

			if (!m_ship.Setup(100, 100)) // ���������� ������ ������� �� ������ �������� ����
				return false;

			srand(time(0)); // ��������� ��������� ����� 

			m_c = new mt::Circle[m_n]; // ��������� ������ ��� ������ ����������
			m_col = new bool[m_n]; // ��������� ������� ��� ������ 

			for (int i = 0; i < m_n; i++) // ����, ������������� �����
			{
				int r = rand() % 50 + 15;// ��������� ������ ����� �� 15 �� 50
				int x, y; // ���������� �����
				bool Position = false; // ������� ���������� ��� ������� �����

				while (!Position) // ����, ��� ����, ����� ����� �� ������������� ���� �� ����� � �� �������� �� ������� ��� ���������
				{
					x = rand() % (1920 - 2 * r) + r; // ��������� ���������� X � ������ �������
					y = rand() % (1080 - 2 * r) + r; // ��������� ���������� Y � ������ �������
					Position = true; // ����� ��������� 

					for (int j = 0; j < i; j++) // �������� �� ��������� ������
					{
						float dist = sqrt(pow(m_c[j].X() - x, 2) + pow(m_c[j].Y() - y, 2)); // ���������� ���������� ����� �������
						if (dist < m_c[j].R() + r) // ���� ��� ������ ����� ��������
						{
							Position = false; // ������ ����� ����������� 
							break; // ����� �� �����
						}
						float distToShip = sqrt(pow(m_ship.X() - x, 2) + pow(m_ship.Y() - y, 2)); // ���������� ���������� �� ��������� �� �������
						if (distToShip < m_ship.Radius() + r + 20)  // ���� �������� ��������� ������� ������ � �������
						{
							Position = false;// �� �������� ����� ������������� ������ �� �������
							continue;
						}
					}
				}
				m_c[i].Setup(x, y, r, m_AsteroidTexture); // ������������� ��������� � ��������� �����������
				m_col[i] = true; // ������������� ������ �������� ��� �����
			}
			return true;
		}
		void CheckBulletCollisionWithCircles() // ����� ��� �������� ������������� ���� � ����������
		{
			for (int i = 0; i < m_n; i++)
			{
				if (m_col[i] && m_ship.BulletAvailable()) // ���� �������� ������� � ���� ��������
				{
					float dx = m_ship.GetBullet().getPosition().x - m_c[i].X(); 
					float dy = m_ship.GetBullet().getPosition().y - m_c[i].Y();
					float distance = sqrt(dx * dx + dy * dy); // ��������� ���������� ����� ����� � ����������

					if (distance < m_c[i].R() + m_ship.GetBullet().getRadius()) // ���� ���������� ������ ����� ��������
					{
						m_col[i] = false; // ������������ �������� 
						m_ship.BulletReset(); // ������������ ����
					}
				}
			}
		}
		bool CheckCollisionWithCircles() // ����� ��� �������� �������� ������� � ���������
		{
			for (int i = 0; i < m_n; i++)
			{
				if (m_col[i]) // ���� �������� �������
				{
					float dist = sqrt(pow(m_ship.X() - m_c[i].X(), 2) + pow(m_ship.Y() - m_c[i].Y(), 2)); // ��������� ���������� ����� ������� � ����������
					if (dist < m_ship.Radius() + m_c[i].R()) // ���� ���������� ������ ����� ��������
					{
						m_ship.RepelFrom(m_c[i].X(), m_c[i].Y()); // ����������� ������� �� ���������
						return true;  
					}
				}
			}
			return false;
		}
		void LifeCycle() // ����� ��� ����������� ����
		{
			sf::Clock clock; // ������ ������ ������ ��� clock ��� ������������ �������
			float fps = 100; // ������� ������
			//int fpsTextViewCnt = 0;

			while (m_window.isOpen()) // ���� ������� ����
			{
				sf::Event event;
				while (m_window.pollEvent(event)) // ������������ ������� 
				{
					if (event.type == sf::Event::Closed) // ���� ������� - ������� ����
						m_window.close(); // ��������� ����
				}
				if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up)) // ������ "�����" ����������� �������� �������
				{
					m_ship.setVelocity(0.003);
				}
				if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down)) // ������� "����" ��������� �������� �������
				{
					m_ship.setVelocity(-0.008);
				}
				if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left)) // ������ "������" ������������ ������� �����
				{
					m_ship.Rotate(-1);
				}
				if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right)) // ������ "������" ������������ ������� ������
				{
					m_ship.Rotate(1);
				}
				if (sf::Keyboard::isKeyPressed(sf::Keyboard::Space)) // ������� "������" �������� 
				{
					if (!m_ship.BulletAvailable()) // ���� ���� �� �������
						m_ship.Attack(); // ����� ����� ��������
				}
				if (sf::Keyboard::isKeyPressed(sf::Keyboard::R)) // ������ "R" ����������� � ���������� ������� ����
				{
					m_ship.BulletReset();
				}


				// ��� ����� ���� ������������� ��� ������������ �������������(�����) ������� ������, ����� �� ������ ����������� �������� ������� ���� ���������
				float dt = clock.getElapsedTime().asSeconds();
				if (1 / fps > dt)
				{
					sf::Time t = sf::seconds(1 / fps - dt);
					sf::sleep(t);
				}
				dt = clock.getElapsedTime().asSeconds();
				clock.restart();

				m_ship.Move(dt); // ����� ������� ����������� �������
				m_ship.UpdateBulletPosition(dt, m_width, m_height); // ���������� ������� ����
				CheckBulletCollisionWithCircles(); // �������� ������������ ���� � ����������

				m_window.clear(); // ������� ����
				m_window.draw(m_spriteBackground); // ����������� ����
				m_window.draw(m_ship.Get()); // ����������� �������

				for (int i = 0; i < m_n; i++) // ����������� ����������
				{
					if (m_col[i])
						m_window.draw(m_c[i].Get());
				}

				if (CheckCollisionWithCircles()) // �������� �������� ����� ������� � ����������
					m_ship.ReverSMove(); // �������� ��������

				if (m_ship.BulletAvailable()) // ����������� ����, ���� ��� ��������
					m_window.draw(m_ship.GetBullet());

				m_window.display();
			}
		}
	};
}

