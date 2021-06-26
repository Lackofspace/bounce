#pragma once
#include <SFML/Graphics.hpp>                                                                // подключение заголовка для работы с графикой
#include <iostream>
#include <sstream>
#include "player.h"


int main() {

    /*!
    * RenderWindow создает окно;
    * window - объект окна,
    * у которого конструктор VideoMode с параметрами = размер окна 640х480
    * и дополнительными параметрами "Bounce",
    * который делает окно развернутым.
    *
    * @param VideoMode
    * @param String &title
    */
    sf::RenderWindow window(sf::VideoMode(640, 480), "Bounce");                            // создаем окно


    /*!
    * Вызов функции reset.
    *
    * @param rectLeft Координаты левого верхнего угла по оси Ох
    * @param rectTop Координаты левого верхнего угла по оси Оу
    * @param rectWidth Значение размера окна по оси Ох
    * @param rectHeight Значение размера окна по оси Оу
    */
    view.reset(sf::FloatRect(0, 0, 640, 480));                                              // размер камеры

    float currPlayerX, currPlayerY = 0;
    float currCameraX, currCameraY = 0;

    Ball p("bounce.png", INITIAL_POS_X, INITIAL_POS_Y, SPRITE_SIZE_X, SPRITE_SIZE_Y);       // создаем объект p класса player,задаем "bounce.png", координаты Х,У, ширина, высота.

    Image map_image;                                                                        // объект изображения для карты
    Texture map;                                                                            // текстура карты
    Sprite s_map;                                                                           // спрайт для карты

    /*!
    * Вызов функции loadFromFile класса Texture.
    *
    * @param string
    * @return bool
    */
    map_image.loadFromFile("images/map.png");                                               // загружаем файл для карты

    /*!
    * Вызов функции loadFromImage класса Texture.
    *
    * @param map_image
    * @return bool
    */
    map.loadFromImage(map_image);                                                           // загружаем текстуру картинкой

    /*!
    * Вызов функции setTexture класса Sprite.
    *
    * @param map
    */
    s_map.setTexture(map);                                                                  // устанавливаем текстуру спрайтом

    /// Вызов функции SetCameraSize.
    SetCameraSize();

    /// Конструктор по умолчанию, запускается автоматически.
    Clock clock;

    Clock gameTimeClock;                                                                    // хранение времени игры
    int gameTime = 0;                                                                   
    float deathX, deathY = 0;                                                               // координаты лопнутого мяча


    /*!
    * Вызов функции isOpen класса Window.
    *
    * @return bool
    */
    while (window.isOpen()) {                                                               // пока окно открыто

        /*!
        * Вызов функций getElapsedTime и asMicroseconds класса Time.
        *
        * @return time
        */
        float time = clock.getElapsedTime().asMicroseconds();
        if (p.alive) {                                                                      // если мяч не сдулся
            gameTime = gameTimeClock.getElapsedTime().asSeconds();                          // игровое время идёт
        }

        /*!
        * Вызов метода restart класса Time.
        *
        * Обнуляет счетчик времени
        */
        clock.restart();                                                                    // обнуление времени
        time = time / 300;                                                                  // скорость показа кадров
        /// Экземпляр sf::Event содержит тип события.
        sf::Event event;                                                                    // объект события

        currPlayerX = p.getPlayerCoordinateX();       /// Вызов функции getPlayerCoordinateX.
        currPlayerY = p.getPlayerCoordinateY();       /// Вызов функции getPlayerCoordinateY.
        currCameraX = view.getCenter().x;             /// Вызов функции getCenter, которая выводит центр вида по Ох.
        currCameraY = view.getCenter().y;             /// Вызов функции getCenter,которая выводит центр вида по Ох.

        /*!
        * Вызов функции pollEvent в цикле while.
        *
        * @param event чтобы обрабатывать ожидаемое событие
        * @return bool
        */
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed)                                        // событие срабатывает, если пользователь хочет закрыть окно
                /*!
                * Вызов функции close класса sf::Window,
                * закрывает окно и уничтожает все прилагаемые ресурсы.
                */
                window.close();
        }
        if (p.alive ||
            !p.isEndGame) {                                                             // если мяч не лопнул и не конец игры
            p.moveCamera(time, currPlayerX, currPlayerY, currCameraX, currCameraY);
        }

        /*!
        * Функция обновляет положения игрока на карте.
        *
        * @param time
        */
        p.update(time);

        /*!
        * Функция setView класса RenderTarget.
        *
        * @param view
        */
        window.setView(view);                                                            // "оживление" камеры

        /// Вызов функции clear класса RenderTarget, ничего не принимает и ничего не возвращает (void).
        window.clear();

        for (int i = 0; i < HEIGHT_MAP; i++)
            for (int j = 0; j < WIDTH_MAP; j++) {

                /// Вызов функции setTextureRect для того чтобы установить под него текстуру map.png и получить спрайт.
                if (TileMap[i][j] == ' ')
                    s_map.setTextureRect(IntRect(SPRITE_SIZE_X, 0, SPRITE_SIZE_X, SPRITE_SIZE_Y));        // если встретили символ пробел, то рисуем 2й квадратик
                if (TileMap[i][j] == 'h')
                    s_map.setTextureRect(IntRect(4 * SPRITE_SIZE_X, 0, SPRITE_SIZE_X, SPRITE_SIZE_Y));    // если встретили символ h, то рисуем 5й квадратик
                if ((TileMap[i][j] == '0'))
                    s_map.setTextureRect(IntRect(0, 0, SPRITE_SIZE_X, SPRITE_SIZE_Y));                    // если встретили символ 0, то рисуем 3й квадратик
                if ((TileMap[i][j] == 'f'))
                    s_map.setTextureRect(IntRect(3 * SPRITE_SIZE_X, 0, SPRITE_SIZE_X, SPRITE_SIZE_Y));    // добавили шип
                if ((TileMap[i][j] == 's'))
                    s_map.setTextureRect(IntRect(2 * SPRITE_SIZE_X, 0, SPRITE_SIZE_X, SPRITE_SIZE_Y));    // добавили бонус здоровья - сфера
                if ((TileMap[i][j] == 'p'))
                    s_map.setTextureRect(IntRect(5 * SPRITE_SIZE_X, 0, SPRITE_SIZE_X, SPRITE_SIZE_Y));    // телепорт

                /*!
                * Вызов функции setPosition класса Transformable.
                *
                * @param j * SPRITE_SIZE_Y
                * @param i * SPRITE_SIZE_X
                */
                s_map.setPosition(j * SPRITE_SIZE_Y, i * SPRITE_SIZE_X);                    // рисуем карту спрайтами

                /*!
                * Вызов функции draw класса RenderTarget.
                *
                * @param s_map
                */
                window.draw(s_map);                                                       // рисуем спрайты карты на экран
            }

        std::ostringstream playerHealthString, gameTimeString;                            
        playerHealthString << p.health;
        gameTimeString << gameTime;                                                       // занесли число здоровья

        if (p.alive && !p.isEndGame) {                                                    // если мяч не лопнул и игра не закончилась

            /*!
            * Функция draw класса RenderTarget.
            *
            * @param p.sprite
            */
            window.draw(p.sprite);                                                        // рисование мяча
        } 
        else {
            if (!p.alive) {                                                               // если мяч не лопнул
                Image deathImage;
                Texture deathTexture;
                Sprite deathSprite;

                deathImage.loadFromFile("images/boom.png");
                deathTexture.loadFromImage(deathImage);                                    // создаем из изображения текстуру
                deathSprite.setTexture(deathTexture);                                      // создаем спрайт из текстуры

                deathX = p.getPlayerCoordinateX();                                         // координата х места сдутия мяча
                deathY = p.getPlayerCoordinateY();                                         // координата у места сдутия мяча
                deathSprite.setPosition(deathX, deathY);                                   // место, где мяч сдулся

                /*!
                * Метод draw изображает объект для Render Target.
                *
                * @param deathSprite
                */
                window.draw(deathSprite);
            }
        }

        /// Вызов функции display класса Window.
        window.display();                                                                  // демонстрируем на экране все
    }
    return 0;
}
