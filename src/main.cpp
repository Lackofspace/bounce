#pragma once
#include <SFML/Graphics.hpp>                                        // подключение заголовка для работы с графикой
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
    sf::RenderWindow window(sf::VideoMode(640, 480), "Bounce");                            


    /*!
    * Вызов функции reset для создания размера камеры.
    *
    * @param rectLeft Координаты левого верхнего угла по оси Ох
    * @param rectTop Координаты левого верхнего угла по оси Оу
    * @param rectWidth Значение размера окна по оси Ох
    * @param rectHeight Значение размера окна по оси Оу
    */
    view.reset(sf::FloatRect(0, 0, 640, 480));                                            

    float currPlayerX, currPlayerY = 0;
    float currCameraX, currCameraY = 0;

    /// Создаем объект p класса player,задаем "bounce.png", координаты Х,У, ширина, высота.
    Ball p("bounce.png", INITIAL_POS_X, INITIAL_POS_Y, SPRITE_SIZE_X, SPRITE_SIZE_Y);    

    // Объект изображения для карты.
    Image map_image;
    /// Текстура карты.                                                                    
    Texture map; 
    /// Спрайт для карты.                                                                       
    Sprite s_map;                                                                        

    /*!
    * Вызов функции loadFromFile класса Texture чтобы загрузить файл для карты.
    *
    * @param string
    * @return bool
    */
    map_image.loadFromFile("images/map.png");                                            

    /*!
    * Вызов функции loadFromImage класса Texture чтобы загрузить текстуру картинкой.
    *
    * @param map_image
    * @return bool
    */
    map.loadFromImage(map_image);                                                        

    /*!
    * Вызов функции setTexture класса Sprite чтобы устанавливаем текстуру спрайтом
.
    *
    * @param map
    */
    s_map.setTexture(map);                                                                
    /// Вызов функции SetCameraSize.
    SetCameraSize();

    /// Конструктор по умолчанию для работы со временем в игре.
    Clock clock;

    /// Хранение времени игры.
    Clock gameTimeClock;                                                                
    int gameTime = 0; 
    /// Координаты лопнутого мяча.                                                                  
    float deathX, deathY = 0;                                                            


    /*!
    * Вызов функции isOpen класса Window (пока окно открыто).
    *
    * @return bool
    */
    while (window.isOpen()) {                                                            

        /*!
        * Вызов функций getElapsedTime и asMicroseconds класса Time.
        *
        * @return time
        */
        float time = clock.getElapsedTime().asMicroseconds();
        if (p.alive) {                                                                    // если мяч не сдулся
            gameTime = gameTimeClock.getElapsedTime().asSeconds();                        // игровое время идёт
        }

        /*!
        * Вызов метода restart класса Time.
        *
        * Обнуляет счетчик времени
        */
        clock.restart();                                                                // обнуление времени
        time = time / 300;                                                                // скорость показа кадров
        /// Экземпляр sf::Event содержит тип события.
        sf::Event event;                                                                // объект события

        currPlayerX = p.getPlayerCoordinateX();        /// Вызов функции getPlayerCoordinateX.
        currPlayerY = p.getPlayerCoordinateY();        /// Вызов функции getPlayerCoordinateY.
        currCameraX = view.getCenter().x;            /// Вызов функции getCenter, которая выводит центр вида по Ох.
        currCameraY = view.getCenter().y;            /// Вызов функции getCenter,которая выводит центр вида по Ох.

        /*!
        * Вызов функции pollEvent в цикле while (событие срабатывает, если пользователь хочет закрыть окно).
        *
        * @param event чтобы обрабатывать ожидаемое событие
        * @return bool
        */
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed)                                        
                /*!
                * Вызов функции close класса sf::Window,
                * закрывает окно и уничтожает все прилагаемые ресурсы.
                */
                window.close();
        }
        if (p.alive || !p.isEndGame) {                                                    // если мяч не лопнул и не конец игры
            p.moveCamera(time, currPlayerX, currPlayerY, currCameraX, currCameraY);
        }

        /*!
        * Функция обновляет положения игрока на карте.
        *
        * @param time
        */
        p.update(time);

        /*!
        * Функция setView класса RenderTarget для "оживление" камеры.
        *
        * @param view
        */
        window.setView(view);                                                            

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
                * Вызов функции setPosition класса Transformable для рисования карты спрайтами.
                *
                * @param j * SPRITE_SIZE_Y
                * @param i * SPRITE_SIZE_X
                */
                s_map.setPosition(j * SPRITE_SIZE_Y, i * SPRITE_SIZE_X);                
                /*!
                * Вызов функции draw класса RenderTarget для показа спрайтов карты на экране.
                *
                * @param s_map
                */
                window.draw(s_map);                                                        
            }

        std::ostringstream playerHealthString, gameTimeString;                            
        playerHealthString << p.health;
        gameTimeString << gameTime;                        // занесли число здоровья

        if (p.alive && !p.isEndGame) {                                                    //	если мяч не лопнул и игра не закончилась

            /*!
            * Функция draw класса RenderTarget.
            *
            * @param p.sprite
            */
            window.draw(p.sprite);                                                        // рисование мяча
        } 
        else {
            if (!p.alive) {                                                                // если мяч не лопнул
                Image deathImage;
                Texture deathTexture;
                Sprite deathSprite;

                deathImage.loadFromFile("images/boom.png");
                deathTexture.loadFromImage(deathImage);                                    // создаем из изображения текстуру
                deathSprite.setTexture(deathTexture);                                    // создаем спрайт из текстуры

                deathX = p.getPlayerCoordinateX();                                        // координата х места сдутия мяча
                deathY = p.getPlayerCoordinateY();                                        // координата у места сдутия мяча
                deathSprite.setPosition(deathX, deathY);                                // место, где мяч сдулся

                /*!
                * Метод draw изображает объект для Render Target.
                *
                * @param deathSprite
                */
                window.draw(deathSprite);
            }
        }

        /// Вызов функции display класса Window демонстрируем на экране все.
        window.display();                                                                
    }
    return 0;
}