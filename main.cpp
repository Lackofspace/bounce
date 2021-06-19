#include <SFML/Graphics.hpp>										// подключение заголовка для работы с графикой
#include <iostream>
#include "map.h"													// подключаем карту
#include "view.h"													// подключаем камеру 
#include <sstream>
#define SPRITE_SIZE_X 20
#define SPRITE_SIZE_Y 20
#define INITIAL_HEALTH 3
#define HEALTH_BOOST 1
#define HEALTH_REDUCE 1
#define INITIAL_POS_X 50
#define INITIAL_POS_Y 50
#define FONT_SIZE 20

/// пространство имен библиотеки sfml
using namespace sf;
class Ball {														// класс персонажа = объекта = мяча
private:
	float x, y = 0;													// координаты мяча по оси OX и OY соответственно
public:
	float width, height, dx, dy, speed = 0;							// высота и ширина спрайта, ускорение (по OX и по OY), скорость движения мяча
	int playerScore = 0;											// переменная для счёта полученных бонусов
	int health;														// переменная, хранящая жизнь мяча
	bool alive, onGround, isEndGame;								// логическая переменная обозначающая сдулся мяч или нет и переменная, говорящая на поверхности мяч или нет, конец игры или нет
	enum class stateObject {										// добавляем перечислемый тип (с областью видимости) - состояние объекта
		left,
		right,
		up,
		down,
		jump,
		stay
	};
	stateObject state;												// переменная, которая будет показывать текущее состояние объекта
	/// Класс утилиты строки, который автоматически обрабатывает конверсии между типами и кодированиями
	String file;													// файл с расширением
	
	/// Класс для загрузки, манипулирования и сохранения изображений
	Image image;													// сфмл изображение
	
	/// Класс текстуры, использует изображение, живущее на видеокарте, для рисования
	Texture texture;												// сфмл текстура
	
	/// Нарисованное представление текстуры, с ее собственными преобразованиями, цвет и т.д.
	Sprite sprite;													// сфмл спрайт
	
	/*!
	* Функция Ball класса Ball для создания спрайта.
    *
	* @param[in] F Имя файла с тайлом
    * @param[in] X Координата Х
    * @param[in] Y Координата У
    * @param[in] W Ширина спрайта
    * @param[in] H Высота спрайта
	*/
	Ball(String F, float X, float Y, float W, float H) {			// Конструктор с параметрами(формальными) для класса Ball
		dx = 0; dy = 0; speed = 0; playerScore = 0;					// ускорение по оси Ох, Оу, скорость, игровой счет
		file = F;													// имя файла+расширение
		health = INITIAL_HEALTH;									// инициализировали переменную жизни в конструкторе
		alive = true;												// инициализировали логическую переменную жизни
		onGround = false;											// инициализировали логическую переменную нахождения на поверхности
		isEndGame = false;											// инициализация конца игры
		width = W; height = H;										// высота и ширина

		/*!
		* Вызов функции loadFromFile класса Texture.
        *
        * @param string На вход подается строка с текстурой из файла на диске
		* @return bool
		*/
		image.loadFromFile("images/" + file);						// запихиваем в image наше изображение вместо file мы передадим то, что пропишем при создании объекта
		
		/*!
		* Вызов функции loadFromImage класса Texture.
        *
        * @param image Изображения для создание текстуры
		* @return bool
		*/
		texture.loadFromImage(image);								// закидываем наше изображение в текстуру

		/*!
		* Вызов функции setTexture класса Sprite.
        *
        * @param texture Текстура
		*/
		sprite.setTexture(texture);									// заливаем спрайт текстурой
		x = X; y = Y;												// координата появления спрайта

		/*!
		* Вызов функции setTextureRect.
        *
        * @param IntRect Текстура принимает на вход координаты для прямоугольника,
		* чтобы установить под него текстуру и получить спрайт,
		*/
		sprite.setTextureRect(IntRect(0, 0, width, height));		// Задаем спрайту один прямоугольник
	}

	/*!
	* Функция control для движения объекта.
    *
	*/
	void control() {
		if (Keyboard::isKeyPressed(Keyboard::Left)) {				// если нажата клавиша лево
			state = stateObject::left;								// объект движется влево
			speed = 0.1;											// со cкоростью 0.1 у. ед.
		}

		if (Keyboard::isKeyPressed(Keyboard::Right)) {				// если нажата клавиша право
			state = stateObject::right;								// объект движется вправо
			speed = 0.1;											// со cкоростью 0.1 у. ед.
		}

		if ((Keyboard::isKeyPressed(Keyboard::Up)) && (onGround)) {	// если нажата клавиша верх и объект на земле
			state = stateObject::jump;								// объект прыгает
			dy = -0.175;											// задаем высоту прыжка
			onGround = false;										// в прыжке мы не на земле
		}

		if (Keyboard::isKeyPressed(Keyboard::Down)) {				// если нажата клавиша низ
			state = stateObject::down;								// объект движется вниз
			speed = 0.1;											// со cкоростью 0.1 у. ед.
		}
	}

	/// Функция update принимает в себя время SFML
	void update(float time)											// функция "оживления" объекта класса. update - обновление. Работает бесконечно, давая персонажу движение.
	{
		if (alive) {												// если мяч не сдулся
			control();												// функция управления персонажем
		}

		switch (state) {											// тут делаются различные действия в зависимости от состояния
		case stateObject::right: dx = speed; break;					// состояние катиться вправо
		case stateObject::left: dx = -speed; break;					// состояние катиться влево
		case stateObject::up: break;								// будет состояние поднятия наверх (например по лестнице)
		case stateObject::jump: break;								// состояние прыжка
		}

		x += dx * time;												// высчитываем смещение координат и как следствие движение

		/*!
		* Вызов функции checkCollisionWithMap.
        * 
		* @param dx Ускорение по оси Ох
        * @param dy Ускорение по оси Оу
		*/
		checkCollisionWithMap(dx, 0);								// обрабатываем столкновение по Х
		y += dy * time;												// аналогично по OY

		/*!
		* Вызов функции checkCollisionWithMap.
        *
		* @param dx
		* @param dy
		*/
		checkCollisionWithMap(0, dy);								// обрабатываем столкновение по Y

		speed = 0;													// зануляем скорость, чтобы мяч остановился

		/*!
		* Вызов функции setPosition класса Sprite.
        *
		* @param x
		* @param y
		*/
		sprite.setPosition(x, y);									// выводим спрайт в позицию x y , посередине. бесконечно выводим в этой функции, иначе бы наш спрайт стоял на месте
		
        /// Вызов функции interactionWithMap.
		interactionWithMap();										// вызываем функцию, отвечающую за взаимодействие с картой

		if (health <= 0) {											// если жизней меньше либо равно 0, то мяч лопается и исключаем движение мяча после проигрыша
			alive = false;
			speed = 0;
		}

		if (!onGround) {											// если объект не на земле
			dy = dy + (0.0015 * time) / 10;							// он падает на землю
		}
	}

	/// Функция interactionWithMap.
	void interactionWithMap()										// ф-ция взаимодействия с картой
	{
		for (int i = y / SPRITE_SIZE_Y; i < (y + height) / SPRITE_SIZE_Y; i++) { 	// проходимся по тайликам, контактирующим с игроком, то есть по всем квадратикам размера SPRITE_SIZE*SPRITE_SIZE
			for (int j = x / SPRITE_SIZE_X; j < (x + width) / SPRITE_SIZE_X; j++) {	// икс делим на SPRITE_SIZE, тем самым получаем левый квадратик, с которым персонаж соприкасается. (он ведь больше размера 32*32, поэтому может одновременно стоять на нескольких квадратах). А j<(x + w) / 32 - условие ограничения координат по иксу. то есть координата самого правого квадрата, который соприкасается с персонажем. таким образом идем в цикле слева направо по иксу, проходя по от левого квадрата (соприкасающегося с героем), до правого квадрата (соприкасающегося с героем)
				if (TileMap[i][j] == '0') {							// если наш квадратик соответствует символу 0 (стена), то проверяем "направление скорости" персонажа:
					if (dy > 0) {									// если мы шли вниз,
						y = i * SPRITE_SIZE_Y - height;				// то блокируем координату игрек мяча: сначала получаем координату нашего квадратика на карте(стены) и затем вычитаем из высоты спрайта мяча.
					}
					if (dy < 0) {
						y = i * SPRITE_SIZE_Y + SPRITE_SIZE_Y;		// аналогично с ходьбой вверх. dy < 0, значит мы идем вверх (вспоминаем координаты паинта)
					}
					if (dx > 0) {
						x = j * SPRITE_SIZE_X - width;				// если идем вправо, то координата Х равна стена (символ 0) минус ширина персонажа
					}
					if (dx < 0) {
						x = j * SPRITE_SIZE_X + SPRITE_SIZE_X;		// аналогично идем влево
					}
				}

				if (TileMap[i][j] == 'h') {							// если символ равен 'h' 
					TileMap[i][j] = ' ';							// убираем фигуру
					health += HEALTH_BOOST;							// увеличиваем здоровье
				}
				if (TileMap[i][j] == 'f') {							// встретили шип
					if (health > 0) {								// если объект жив (здоровье больше 0)
						health -= HEALTH_REDUCE;					// объект теряет здоровье
						TileMap[i][j] = ' ';						// убрали шип
					}
					if (health <= 0) {								// если здоровья меньше или равно 0
						health = 0;									// считаем, что здоровье меньше 0 не мб и оно минимально в 0
						TileMap[i][j] = 'f';						// оставим шип на месте, когда здоровье станет меньше или равно нулю
					}
				}

			}
		}
	}

	/*!
	* Функция getPlayerCoordinateX.
	* 
	* @return x Координата по оси Ох
	*/
	float getPlayerCoordinateX() {									// этим методом будем забирать координату Х	
		return x;
	}

	/*!
	* Функция getPlayerCoordinateY.
    *
	* @return y Координаты по оси Оу
	*/
	float getPlayerCoordinateY() {									// этим методом будем забирать координату Y 	
		return y;
	}

	/*!
	* Функция checkCollisionWithMap.
    *
	* @param Dx ускорение по оси Ох
	* @param Dy ускорение по оси Оу
	*/
	void checkCollisionWithMap(float Dx, float Dy) {											// функция проверки столкновений с картой
		for (int i = y / SPRITE_SIZE_Y; i < (y + height) / SPRITE_SIZE_Y; i++) {				// проходимся по элементам карты
			for (int j = x / SPRITE_SIZE_X; j < (x + width) / SPRITE_SIZE_X; j++) {
				if (TileMap[i][j] == '0') {
					if (Dy > 0) {
						y = i * SPRITE_SIZE_Y - height;
						dy = 0; 
						onGround = true;
					}																			// по Y вниз=>идем в пол(стоим на месте) или падаем. В этот момент надо вытолкнуть персонажа и поставить его на землю, при этом говорим что мы на земле тем самым снова можем прыгать
					if (Dy < 0) {
						y = i * SPRITE_SIZE_Y + SPRITE_SIZE_Y;
						dy = 0;
					}																			// столкновение с верхними краями карты(может и не пригодиться)
					if (Dx > 0) {
						x = j * SPRITE_SIZE_X - width;
					}																			// с правым краем карты
					if (Dx < 0) {
						x = j * SPRITE_SIZE_X + SPRITE_SIZE_X;
					}																			// с левым краем карты
				}
				else {
					onGround = false;
				}
				if (TileMap[i][j] == 'p') {
					isEndGame = true;
					if (Dy > 0) {
						y = i * SPRITE_SIZE_Y - height;
						dy = 0;
					}																			// по Y вниз => идем в пол(стоим на месте) или падаем. В этот момент надо вытолкнуть персонажа и поставить его на землю, при этом говорим что мы на земле тем самым снова можем прыгать
					if (Dy < 0) {
						y = i * SPRITE_SIZE_Y + SPRITE_SIZE_Y;
						dy = 0;
					}																			// столкновение с верхними краями карты(может и не пригодиться)
					if (Dx > 0) {
						x = j * SPRITE_SIZE_X - width;
					}																			// с правым краем карты
					if (Dx < 0) {
						x = j * SPRITE_SIZE_X + SPRITE_SIZE_X;
					}																			// с левым краем карты
				}
			}
		}
	}

	/*!
    * Функция moveCamera перемещает камеру.
    *
	* @param time
	* @param currPlayerX Текущее положение мяча по оси Ох
	* @param currPlayerY Текущее положение мяча по оси Оу
	* @param currCameraX Текущее положение камеры по оси Оx
	* @param currCameraY Текущее положение камеры по оси Оy
	*/
	void moveCamera(float time, float currPlayerX, float currPlayerY, float currCameraX, float currCameraY) {
		if (Keyboard::isKeyPressed(Keyboard::Left)) {						// если нажата клавиша лево
			if (currCameraX - currPlayerX >= 70) {							// проходим помимо 50 у.ед. расстояния еще расстояние-размер спрайта мячика, который по оси Х равен 20
				if (currCameraX > 200) {
					/// Метод move добавляет к текущему положению объекта параметры смещения по Ох и Оу.
					view.move(-0.1 * time, 0);								// камера чтоб не выходила за границы карты	
				}
			}
		}

		if (Keyboard::isKeyPressed(Keyboard::Right)) {						// если нажата клавиша право
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

		if (currPlayerY - currCameraY >= 50) {								// если мяч двигается вниз, то камера последует за ним независимо от нажатой клавиши
			if (currCameraY < 350) {
				view.move(0, 0.1 * time);
			}
		}
	}
};
int main(){

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
	sf::RenderWindow window(sf::VideoMode(640, 480), "Bounce");							// создаем окно


	/*!
	* Вызов функции reset.
    *
	* @param rectLeft Координаты левого верхнего угла по оси Ох
	* @param rectTop Координаты левого верхнего угла по оси Оу
	* @param rectWidth Значение размера окна по оси Ох
	* @param rectHeight Значение размера окна по оси Оу
	*/
	view.reset(sf::FloatRect(0, 0, 640, 480));											// размер "вида" камеры при создании объекта вида камеры. (потом можем менять как хотим)

	float currPlayerX, currPlayerY = 0;
	float currCameraX, currCameraY = 0;

	Ball p("bounce.png", INITIAL_POS_X, INITIAL_POS_Y, SPRITE_SIZE_X, SPRITE_SIZE_Y);	// создаем объект p класса player,задаем "hero.png" как имя файла+расширение, далее координата Х,У, ширина, высота.

	Image map_image;																	// объект изображения для карты
	Texture map;																		// текстура карты
	Sprite s_map;																		// создаём спрайт для карты

	/*!
	* Вызов функции loadFromFile класса Texture.
    *
	* @param string
	* @return bool
	*/
	map_image.loadFromFile("images/map.png");											// загружаем файл для карты

	/*!
	* Вызов функции loadFromImage класса Texture.
    *
	* @param map_image
	* @return bool
	*/
	map.loadFromImage(map_image);														// заряжаем текстуру картинкой

	/*!
	* Вызов функции setTexture класса Sprite.
    *
	* @param map
	*/
	s_map.setTexture(map);																// заливаем текстуру спрайтом

	/// Вызов функции SetCameraSize.
	SetCameraSize();

	/// Конструктор по умолчанию, запускается автоматически
	Clock clock;

	Clock gameTimeClock;																// переменная игрового времени, хранение времени игры 
	int gameTime = 0;																	// объявили игровое время
	float deathX, deathY = 0;															// координаты лопнутого мяча


	/*!
	* Вызов функции isOpen класса Window.
    *
	* @return bool
	*/
	while (window.isOpen()){															// пока окно открыто

		/*!
		* Вызов функций getElapsedTime и asMicroseconds класса Time.
        *
		* @return time
		*/
		float time = clock.getElapsedTime().asMicroseconds();
		if (p.alive){																	// если мяч не сдулся
			gameTime = gameTimeClock.getElapsedTime().asSeconds();						// игровое время в секундах идёт вперед, пока не конец игры
		}

		/*!
		* Вызов метода restart класса Time.
        *
		* Обнуляет счетчик времени
		*/
		clock.restart();																// начинаем отсчет времени с нуля, чтобы определять прошедший временной диапазон с предыдущего кадра
		time = time / 300;																// определяем скорость рисования игры
		/// Экземпляр sf::Event содержит тип события
		sf::Event event;																// создаем объект события

		currPlayerX = p.getPlayerCoordinateX();		/// Вызов функции getPlayerCoordinateX
		currPlayerY = p.getPlayerCoordinateY();		/// Вызов функции getPlayerCoordinateY
		currCameraX = view.getCenter().x;			/// Вызов функции getCenter, которая выводит центр вида по Ох
		currCameraY = view.getCenter().y;			/// Вызов функции getCenter,которая выводит центр вида по Ох

		/*!
		* Вызов функции pollEvent в цикле while.
        *
		* @param event чтобы обрабатывать ожидаемое событие
		* @return bool
		*/
		while (window.pollEvent(event)){
			if (event.type == sf::Event::Closed)										// событие срабатывает, если пользователь хочет закрыть окно
				/*!
				* Вызов функции close класса sf::Window,
				* закрывает окно и уничтожает все прилагаемые ресурсы.
				*/
				window.close();
		}
		if (p.alive || !p.isEndGame){													// если мяч не лопнул и не конец игры
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
		window.setView(view);															// "оживляем" камеру в окне sfml

		/// Вызов функции clear класса RenderTarget, ничего не принимает и ничего не возвращает (void)
		window.clear();
		
		for (int i = 0; i < HEIGHT_MAP; i++)
			for (int j = 0; j < WIDTH_MAP; j++){

				/// Вызов функции setTextureRect для того чтобы установить под него текстуру map.png и получить спрайт.
				if (TileMap[i][j] == ' ')  s_map.setTextureRect(IntRect(SPRITE_SIZE_X, 0, SPRITE_SIZE_X, SPRITE_SIZE_Y));		// если встретили символ пробел, то рисуем 2й квадратик
				if (TileMap[i][j] == 'h')  s_map.setTextureRect(IntRect(4 * SPRITE_SIZE_X, 0, SPRITE_SIZE_X, SPRITE_SIZE_Y));	// если встретили символ h, то рисуем 5й квадратик
				if ((TileMap[i][j] == '0')) s_map.setTextureRect(IntRect(0, 0, SPRITE_SIZE_X, SPRITE_SIZE_Y));					// если встретили символ 0, то рисуем 3й квадратик
				if ((TileMap[i][j] == 'f')) s_map.setTextureRect(IntRect(3 * SPRITE_SIZE_X, 0, SPRITE_SIZE_X, SPRITE_SIZE_Y));	// добавили шип
				if ((TileMap[i][j] == 's')) s_map.setTextureRect(IntRect(2 * SPRITE_SIZE_X, 0, SPRITE_SIZE_X, SPRITE_SIZE_Y));	// добавили бонус здоровья - сфера
				if ((TileMap[i][j] == 'p')) s_map.setTextureRect(IntRect(5 * SPRITE_SIZE_X, 0, SPRITE_SIZE_X, SPRITE_SIZE_Y));	// телепорт

				/*!
				* Вызов функции setPosition класса Transformable.
                *
				* @param j * SPRITE_SIZE_Y
				* @param i * SPRITE_SIZE_X
				*/
				s_map.setPosition(j * SPRITE_SIZE_Y, i * SPRITE_SIZE_X);				// по сути раскидывает квадратики, превращая в карту. то есть задает каждому из них позицию. если убрать, то вся карта нарисуется в одном квадрате 20*20 и мы увидим один квадрат

				/*!
				* Вызов функции draw класса RenderTarget.
                *
				* @param s_map 
				*/
				window.draw(s_map);														// рисуем квадратики на экран
			}

		std::ostringstream playerHealthString, gameTimeString;							// объявили переменную
		playerHealthString << p.health; gameTimeString << gameTime;						// занесли в нее число здоровья, то есть формируем строку


		if (p.alive && !p.isEndGame){													//	если мяч не лопнул и игра не закончилась, выполняется следующее

			/*!
			* Функция draw класса RenderTarget.
            *
			* @param p.sprite 
			*/
			window.draw(p.sprite);														// рисование мяча
		}
		else
		{
			if (!p.alive) {																// если мяч не лопнул
				Image deathImage;
				Texture deathTexture;
				Sprite deathSprite;

				deathImage.loadFromFile("images/boom.png");
				deathTexture.loadFromImage(deathImage);									// создаем из изображения текстуру
				deathSprite.setTexture(deathTexture);									// создаем спрайт из текстуры

				deathX = p.getPlayerCoordinateX();										// координата х места сдутия мяча
				deathY = p.getPlayerCoordinateY();										// координата у места сдутия мяча
				deathSprite.setPosition(deathX, deathY);								// место, где мяч сдулся

				/*!
				* Метод draw изображает объект для Render Target.
                *
				* @param deathSprite 
				*/
				window.draw(deathSprite);
			}
		}

		/// Вызов функции display класса Window.
		window.display();																// показывает на экране все, что происходит
	}				
	return 0;
}