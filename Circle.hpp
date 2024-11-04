#pragma once
#include <SFML/Graphics.hpp>
#include <iostream>

namespace mt
{

	class Circle // Объявление класса
	{
		float m_r; // Переменная для радиуса
		float m_x, m_y; //Переменные для координат 
		sf::CircleShape m_shape; // Объект класса CircleShape для круга
		sf::Texture m_TextureAsteroid; // Объект класса для хранения текстуры астероида
		bool m_active;

	public:
		Circle() = default; // Конструктор по-умолчанию

		Circle(float x, float y, float r, const sf::Texture& texture) // Конструктор инициализирует координаты, радиус и текстуру круга
		{
			Setup(x, y, r, texture);
		}

		bool Setup(float x, float y, float r, const sf::Texture& texture) // Функия для установки параметров круга 
		{
			m_x = x; // Устанавливаем координату X
			m_y = y; // Устанавливаем координату Y
			m_r = r; // Устанавливаем радиус
			if (!m_TextureAsteroid.loadFromFile("assets\\asteroid.png"))// Загружаем текстуру астероида 
			{
				std::cout << "Error while loading Starship.png" << std::endl;
				return false; // Если текстуру не удаётся загрузить, то вывести ошибку
			} 
			m_shape.setOrigin(m_r, m_r); // Устанавливаем начало координат в центр круга
			m_shape.setRadius(m_r); // Устанавливаем радиус круга
			m_shape.setPosition(m_x, m_y); // Устанавливаем координаты круга
			m_shape.setTexture(&texture); // Устанавливаем текстуру кругу
			m_active = true; // Устанавливаем состояние активности
		}
		void SetActive(bool active) { m_active = active; } // Метод для установки активности астероида
		bool IsActive() const { return m_active; } // Метод для проверки активности астероида
		sf::CircleShape Get() // Геттер для получения круга 
		{
			return m_shape;
		}

		float X() { return m_x; } // Геттер для получения X 
		float Y() { return m_y; } // Геттер для получения Y
		float R() { return m_r; } // Геттер для получения радиуса
	};
}

