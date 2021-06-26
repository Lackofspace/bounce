#include <SFML/Graphics.hpp>

using namespace sf;

sf::View view;                                                      // объявили sfml объект "вид", который и является камерой
/// Функция SetCameraSize.
void SetCameraSize() {
    view.setCenter(200.f, 150.f);                                   // центр камеры
    view.setSize(400.f, 300.f);                                     // размер камеры
}
