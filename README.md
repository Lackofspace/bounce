# Bounce


## Цель создания игры

В далёком прошлом я с друзьями любил играть в игру под названием "Bounce". Возможно, это была моя самая первая игра, в которой я тратил своё свободное время. Недавно я захотел отдать дань памяти этой простой и в то же время интересной игре, написав простенькую копию с первым уровнем игры из оригинала. Ностальгия...


## Краткое описание игры

Задача игрока заключается в ведении мяча до портала. Управление осуществляется с помощью кнопок "вверх", "вниз", "вправо", "влево".


## Как установить эту игру (На Windows)

### Для начала в корне проекта следует выполнить команды

```git submodule init```

```git submodule add https://github.com/google/googletest```

Эти 2 команды создадут директорию в корне под названием googletest, где будут храниться исходники наших тестов.
Далее все стандартно: запускаем conan (подключаем библиотеку) и собираем с помощью cmake.
При клоинровании, чтобы выкачать зависимости (googletest) нужно будет выполнить команду 

```git submodule update --init --recursive```

### В cmd строке прописать следующие команды :

```conan remote add bincrafters https://api.bintray.com/conan/bincrafters/public-conan```

```conan profile new default --detect```

```conan profile update settings.cppstd=17 default```

### В директории, где находится проект, выполнить команды :

```conan install . -if build```

```cmake -G "Visual Studio 16 2019" -B build -S .```

```cmake --build build --config Release```

```start ./build/bin/bounce.exe```


## Дополнительная литература

### Работа с CMake

https://habr.com/ru/post/155467/

https://docs.microsoft.com/ru-ru/cpp/build/cmake-projects-in-visual-studio?view=msvc-160

https://habr.com/ru/post/342982/

https://docs.microsoft.com/en-us/cpp/build/customize-cmake-settings?view=msvc-160

### Работа с библиотекой SFML

https://ps-group.github.io/ps1/lesson_sfml_1

https://www.sfml-dev.org

https://ravesli.com/graficheskaya-biblioteka-sfml-vstuplenie-i-ustanovka/

https://habr.com/ru/post/279279/