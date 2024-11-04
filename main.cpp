#include "Circle.hpp"
#include "Game.hpp"
#include <SFML/Graphics.hpp>

int main()
{
    mt::Game game(1920, 1080, "Game!!!"); //Создаём объект класса с заданной длиной, шириной и заголовком
    if (!game.Setup(10))  //Создаём игру с 10 астероидами
        return -1;

    game.LifeCycle(); // Запускаем функцию отображения всего происходящего


    return 0;
}