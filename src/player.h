#include "map.h"                                                    // подключаем карту
#include "view.h"
#include <SFML/Graphics.hpp>

const float SPRITE_SIZE_X = 20;
const float SPRITE_SIZE_Y = 20;
const float INITIAL_HEALTH = 3;
const float HEALTH_BOOST = 1;
const float HEALTH_REDUCE = 1;
const float INITIAL_POS_X = 50;
const float INITIAL_POS_Y = 50;

using namespace sf;

/*!
* Класс Ball - класс персонажа = объекта = мяча.
* 
*/
class Ball {                                                        
private:
    /// Координата мяча по оси OX.
    float x = 0;
    /// Координата мяча по оси OY.
    float y = 0;                                                    
public:
    /// Ширина.
    float width;
    /// Высота.
    float height;
    /// Перемещение по оси ОХ мяча за некоторое время.
    float dx;
    /// Перемещение по оси ОУ мяча за некоторое время.
    float dy; 
    /// Скорость движения мяча.
    float speed = 0;
    /// Переменная для счёта полученных бонусов.
    int playerScore = 0; 
    /// Переменная, хранящая жизнь мяча.
    int health; 
    /// Логическая переменная обозначающая сдулся мяч или нет.
    bool alive; 
    /// Логическая переменная говорящая на поверхности мяч или нет.
    bool onGround;
    /// Логическая переменная обозначающая конец игры или нет.
    bool isEndGame;
    /// Добавляем перечисляемый тип (с областью видимости) - состояние объекта.
    enum class stateObject {                                        
        left,
        right,
        up,
        down,
        jump,
        stay
    };
    /// Переменная, которая будет показывать текущее состояние объекта.
    stateObject state; 

    /// Файл с расширением.
    String file;                                                    

    /// Сфмл изображение.
    Image image;                                                    

    /// Сфмл текстура.
    Texture texture;                                                

    /// Сфмл спрайт.
    Sprite sprite;                                                    

    /*!
    * Конструктор Ball класса Ball для создания спрайта.
    *
    * @param[in] F Имя файла с тайлом
    * @param[in] X Координата Х
    * @param[in] Y Координата У
    * @param[in] W Ширина спрайта
    * @param[in] H Высота спрайта
    */
    Ball(String F, float X, float Y, float W, float H) {            // Конструктор с параметрами(формальными) для класса Ball
        dx = 0;
        dy = 0;
        speed = 0;
        playerScore = 0;                                            // ускорение по оси Ох, Оу, скорость, игровой счет
        file = F;                                                    // имя файла+расширение
        health = INITIAL_HEALTH;                                    // инициализировали переменную жизни в конструкторе
        alive = true;                                                // инициализировали логическую переменную жизни
        onGround = false;                                            // инициализировали логическую переменную нахождения на поверхности
        isEndGame = false;                                            // инициализация конца игры
        width = W;
        height = H;                                        // высота и ширина

        /*!
        * Вызов функции loadFromFile класса Texture.
        *
        * @param file На вход подается строка с текстурой из файла на диске
        * @return bool
        */
        image.loadFromFile("images/" + file);                        // загружаем в image изображение

        /*!
        * Вызов функции loadFromImage класса Texture.
        *
        * @param image Изображения для создание текстуры
        * @return bool
        */
        texture.loadFromImage(image);                                // загружаем изображение в текстуру

        /*!
        * Вызов функции setTexture класса Sprite.
        *
        * @param texture Текстура
        */
        sprite.setTexture(texture);                                    // заливаем спрайт текстурой
        x = X;
        y = Y;                                                // координата появления спрайта

        /*!
        * Вызов функции setTextureRect.
        *
        * @param IntRect Текстура принимает на вход координаты для прямоугольника,
        * чтобы установить под него текстуру и получить спрайт,
        */
        sprite.setTextureRect(IntRect(0, 0, width, height));        // Задаем спрайту один прямоугольник
    }

    /*!
    * Метод control установки скорости под нажатием стрелки с клавиатуры.
    *
    */
    void control() {
        if (Keyboard::isKeyPressed(Keyboard::Left)) {                // если нажата клавиша лево
            state = stateObject::left;                                // объект движется влево
            speed = 0.1;                                            // со cкоростью 0.1 у. ед.
        }

        if (Keyboard::isKeyPressed(Keyboard::Right)) {                // если нажата клавиша право
            state = stateObject::right;                                // объект движется вправо
            speed = 0.1;                                            // со cкоростью 0.1 у. ед.
        }

        if ((Keyboard::isKeyPressed(Keyboard::Up)) && (onGround)) {    // если нажата клавиша верх и объект на земле
            state = stateObject::jump;                                // объект прыгает
            dy = -0.175;                                            // задаем высоту прыжка
            onGround = false;                                        // в прыжке мы не на земле
        }

        if (Keyboard::isKeyPressed(Keyboard::Down)) {                // если нажата клавиша низ
            state = stateObject::down;                                // объект движется вниз
            speed = 0.1;                                            // со cкоростью 0.1 у. ед.
        }
    }

    /// Метод update взаимодействие с картой.
    void update(float time){                                           // для двигающейся картинки
        if (alive) {                                                // если мяч не сдулся
            control();                                                // функция управления персонажем
        }

        switch (state) {                                            // какое состояние принимает мяч
            case stateObject::right:
                dx = speed;
                break;                    // состояние катиться вправо
            case stateObject::left:
                dx = -speed;
                break;                    // состояние катиться влево
            case stateObject::up:
                break;                                // будет состояние поднятия наверх (например по лестнице)
            case stateObject::jump:
                break;                                // состояние прыжка
        }

        x += dx * time;                                                // высчитываем смещение координат по Ох

        /*!
        * Вызов функции checkCollisionWithMap.
        *
        * @param dx Ускорение по оси Ох
        * @param dy Ускорение по оси Оу
        */
        checkCollisionWithMap(dx, 0);                                // обрабатываем столкновение по Х
        y += dy * time;                                                // высчитываем смещение координат по Оу

        /*!
        * Вызов функции checkCollisionWithMap.
        *
        * @param dx
        * @param dy
        */
        checkCollisionWithMap(0, dy);                                // обрабатываем столкновение по Y

        speed = 0;                                                    // зануляем скорость, чтобы мяч остановился

        /*!
        * Вызов функции setPosition класса Sprite.
        *
        * @param x
        * @param y
        */
        sprite.setPosition(x, y);                                    // выводим спрайт в позицию x y

        /// Вызов функции interactionWithMap.
        interactionWithMap();                                        // вызываем функцию, отвечающую за взаимодействие с картой

        if (health <= 0) {                                            // если жизней меньше либо равно 0, то мяч лопается и исключаем движение мяча после проигрыша
            alive = false;
            speed = 0;
        }

        if (!onGround) {                                            // если объект не на земле
            dy = dy + (0.0015 * time) / 10;                            // он падает на землю
        }
    }

    /// Метод interactionWithMap обрабатывает и проверяет столкновения с предметами.
    void interactionWithMap(){                                        
        for (int i = y / SPRITE_SIZE_Y; i < (y + height) / SPRITE_SIZE_Y; i++) {    // проходимся по всем квадратикам размера SPRITE_SIZE*SPRITE_SIZE
            for (int j = x / SPRITE_SIZE_X; j < (x + width) / SPRITE_SIZE_X; j++) {    // икс делим на SPRITE_SIZE, тем самым получаем левый квадратик, с которым персонаж соприкасается
                if (TileMap[i][j] == '0') {                            // если наш квадратик соответствует символу 0 (стена), то проверяем "направление скорости" персонажа
                    if (dy > 0) {                                    // если мы шли вниз
                        y = i * SPRITE_SIZE_Y - height;                // то блокируем координату игрек мяча
                    }
                    if (dy < 0) {
                        y = i * SPRITE_SIZE_Y + SPRITE_SIZE_Y;        // аналогично с ходьбой вверх : dy < 0, значит идем вверх
                    }
                    if (dx > 0) {
                        x = j * SPRITE_SIZE_X - width;                // если идем вправо, то координата Х равна стена (символ 0) минус ширина персонажа
                    }
                    if (dx < 0) {
                        x = j * SPRITE_SIZE_X + SPRITE_SIZE_X;        // идем влево
                    }
                }

                if (TileMap[i][j] == 'h') {                            // если символ равен 'h'
                    TileMap[i][j] = ' ';                            // убираем фигуру
                    health += HEALTH_BOOST;                            // увеличиваем здоровье
                }
                if (TileMap[i][j] == 'f') {                            // встретили шип
                    if (health > 0) {                                // если объект жив (здоровье больше 0)
                        health -= HEALTH_REDUCE;                    // объект теряет здоровье
                        TileMap[i][j] = ' ';                        // убрали шип
                    }
                    if (health <= 0) {                                // если здоровья меньше или равно 0
                        health = 0;                                    // считаем, что здоровье меньше 0 не мб и оно минимально в 0
                        TileMap[i][j] = 'f';                        // оставим шип на месте, когда здоровье станет меньше или равно нулю
                    }
                }
            }
        }
    }

    /*!
    * Метод getPlayerCoordinateX для того чтобы забирать координату Х.
    *
    * @return x Координата по оси Ох
    */
    float getPlayerCoordinateX() {                                    
        return x;
    }

    /*!
    * Метод getPlayerCoordinateУ для того чтобы забирать координату У.
    *
    * @return y Координаты по оси Оу
    */
    float getPlayerCoordinateY() {                                    
        return y;
    }

    /*!
    * Метод checkCollisionWithMap взаимодействие с стеной.
    *
    * @param Dx ускорение по оси Ох
    * @param Dy ускорение по оси Оу
    */
    void checkCollisionWithMap(float Dx, float Dy) {                                            
        for (int i = y / SPRITE_SIZE_Y;
             i < (y + height) / SPRITE_SIZE_Y; i++) {                // проходимся по элементам карты
            for (int j = x / SPRITE_SIZE_X; j < (x + width) / SPRITE_SIZE_X; j++) {
                if (TileMap[i][j] == '0') {
                    if (Dy > 0) {
                        y = i * SPRITE_SIZE_Y - height;
                        dy = 0;
                        onGround = true;
                    }                                                                            // по Y вниз : скорость = 0
                    if (Dy < 0) {
                        y = i * SPRITE_SIZE_Y + SPRITE_SIZE_Y;
                        dy = 0;
                    }                                                                            // столкновение с потолком
                    if (Dx > 0) {
                        x = j * SPRITE_SIZE_X - width;
                    }                                                                            // с правым краем карты
                    if (Dx < 0) {
                        x = j * SPRITE_SIZE_X + SPRITE_SIZE_X;
                    }                                                                            // с левым краем карты
                } 
                else {
                    onGround = false;
                }
                if (TileMap[i][j] == 'p') {
                    isEndGame = true;
                    if (Dy > 0) {
                        y = i * SPRITE_SIZE_Y - height;
                        dy = 0;
                    }                                                                           
                    if (Dy < 0) {
                        y = i * SPRITE_SIZE_Y + SPRITE_SIZE_Y;
                        dy = 0;
                    }                                                                            
                    if (Dx > 0) {
                        x = j * SPRITE_SIZE_X - width;
                    }                                                                            
                    if (Dx < 0) {
                        x = j * SPRITE_SIZE_X + SPRITE_SIZE_X;
                    }                                                                           
                }
            }
        }
    }

    /*!
    * Метод moveCamera перемещает камеру (при этом она не выходит за границы карты).
    *
    * @param time
    * @param currPlayerX Текущее положение мяча по оси Ох
    * @param currPlayerY Текущее положение мяча по оси Оу
    * @param currCameraX Текущее положение камеры по оси Оx
    * @param currCameraY Текущее положение камеры по оси Оy
    */
    void moveCamera(float time, float currPlayerX, float currPlayerY, float currCameraX, float currCameraY) {
        if (Keyboard::isKeyPressed(Keyboard::Left)) {                        // если нажата клавиша лево
            if (currCameraX - currPlayerX >= 70) {                            // проходим помимо 50 у.ед. расстояния еще расстояние-размер спрайта мячика, который по оси Х равен 20
                if (currCameraX > 200) {
                    /// Метод move добавляет к текущему положению объекта параметры смещения по Ох и Оу.
                    view.move(-0.1 * time, 0);                                // камера чтоб не выходила за границы карты
                }
            }
        }

        if (Keyboard::isKeyPressed(Keyboard::Right)) {                        // если нажата клавиша право
            if (currPlayerX - currCameraX >= 50) {
                if (currCameraX < 2060) {
                    view.move(0.1 * time, 0);
                }
            }
        }

        if (Keyboard::isKeyPressed(Keyboard::Up)) {
            if (currCameraY - currPlayerY >= 70) {
                if (currCameraY >= 150) {
                    view.move(0, -0.1 * time);
                }
            }
        }

        if (currPlayerY - currCameraY >= 50) {                                // если мяч двигается вниз, то камера последует за ним независимо от нажатой клавиши
            if (currCameraY < 350) {
                view.move(0, 0.1 * time);
            }
        }
    }
};