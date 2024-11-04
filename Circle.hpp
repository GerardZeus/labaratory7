#pragma once
#include <SFML/Graphics.hpp>
#include <iostream>

namespace mt
{

	class Circle // ���������� ������
	{
		float m_r; // ���������� ��� �������
		float m_x, m_y; //���������� ��� ��������� 
		sf::CircleShape m_shape; // ������ ������ CircleShape ��� �����
		sf::Texture m_TextureAsteroid; // ������ ������ ��� �������� �������� ���������
		bool m_active;

	public:
		Circle() = default; // ����������� ��-���������

		Circle(float x, float y, float r, const sf::Texture& texture) // ����������� �������������� ����������, ������ � �������� �����
		{
			Setup(x, y, r, texture);
		}

		bool Setup(float x, float y, float r, const sf::Texture& texture) // ������ ��� ��������� ���������� ����� 
		{
			m_x = x; // ������������� ���������� X
			m_y = y; // ������������� ���������� Y
			m_r = r; // ������������� ������
			if (!m_TextureAsteroid.loadFromFile("assets\\asteroid.png"))// ��������� �������� ��������� 
			{
				std::cout << "Error while loading Starship.png" << std::endl;
				return false; // ���� �������� �� ������ ���������, �� ������� ������
			} 
			m_shape.setOrigin(m_r, m_r); // ������������� ������ ��������� � ����� �����
			m_shape.setRadius(m_r); // ������������� ������ �����
			m_shape.setPosition(m_x, m_y); // ������������� ���������� �����
			m_shape.setTexture(&texture); // ������������� �������� �����
			m_active = true; // ������������� ��������� ����������
		}
		void SetActive(bool active) { m_active = active; } // ����� ��� ��������� ���������� ���������
		bool IsActive() const { return m_active; } // ����� ��� �������� ���������� ���������
		sf::CircleShape Get() // ������ ��� ��������� ����� 
		{
			return m_shape;
		}

		float X() { return m_x; } // ������ ��� ��������� X 
		float Y() { return m_y; } // ������ ��� ��������� Y
		float R() { return m_r; } // ������ ��� ��������� �������
	};
}

