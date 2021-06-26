#pragma once
#include "gtest/gtest.h"
#include <SFML/Graphics.hpp>
#include <iostream>
#include <sstream>
#include <stdexcept>
#include "../src/player.h"

TEST(Ball, getPlayerCoordinate) {
  Ball ball("bounce.png", 50, 50, 20, 20);
  ASSERT_EQ(ball.getPlayerCoordinateY(), 50);
  ASSERT_EQ(ball.getPlayerCoordinateX(), 50);
  ASSERT_TRUE(ball.height == 20);
  ASSERT_TRUE(ball.width == 20);
  ASSERT_TRUE(ball.onGround == false);
}

TEST(Map, mapSize) {
	ASSERT_TRUE(HEIGHT_MAP == 15);
	ASSERT_TRUE(WIDTH_MAP == 113);
}

TEST(Camera, cameraCoordinates) {
	SetCameraSize();
	ASSERT_EQ(view.getCenter(), sf::Vector2f(200.f, 150.f));
	ASSERT_EQ(view.getSize(), sf::Vector2f(400.f, 300.f));
}

TEST(gameInit1, gameCoordinates) {
    sf::RenderWindow window(sf::VideoMode(640, 480), "Bounce");							
	view.reset(sf::FloatRect(0, 0, 640, 480));											
	float currPlayerX, currPlayerY = 0;
	float currCameraX, currCameraY = 0;


	Ball p("bounce.png", INITIAL_POS_X, INITIAL_POS_Y, SPRITE_SIZE_X, SPRITE_SIZE_Y);	

	Image map_image;																	
	Texture map;																	
	Sprite s_map;																		

	map_image.loadFromFile("images/map.png");											

	map.loadFromImage(map_image);														

	s_map.setTexture(map);																
	
	SetCameraSize();

	Clock clock;

	Clock gameTimeClock;																
	int gameTime = 0;																	
	float deathX, deathY = 0;															
	TileMap[4][3] = 'f';
	while (gameTime < 3) {															
		float time = clock.getElapsedTime().asMicroseconds();
		if (p.alive) {																	
			gameTime = gameTimeClock.getElapsedTime().asSeconds();						
		}
		clock.restart();																
		time = time / 300;																
		
		sf::Event event;															

		currPlayerX = p.getPlayerCoordinateX();		
		currPlayerY = p.getPlayerCoordinateY();		
		currCameraX = view.getCenter().x;			
		currCameraY = view.getCenter().y;			

		while (window.pollEvent(event)){
			if (event.type == sf::Event::Closed)										
				window.close();
		}
		if (p.alive || !p.isEndGame) {													
			p.moveCamera(time, currPlayerX, currPlayerY, currCameraX, currCameraY);
		}

		p.update(time);

		window.setView(view);															
		window.clear();
		
		for (int i = 0; i < HEIGHT_MAP; i++)
			for (int j = 0; j < WIDTH_MAP; j++){

				if (TileMap[i][j] == ' ')  s_map.setTextureRect(IntRect(SPRITE_SIZE_X, 0, SPRITE_SIZE_X, SPRITE_SIZE_Y));		
				if (TileMap[i][j] == 'h')  s_map.setTextureRect(IntRect(4 * SPRITE_SIZE_X, 0, SPRITE_SIZE_X, SPRITE_SIZE_Y));	
				if ((TileMap[i][j] == '0')) s_map.setTextureRect(IntRect(0, 0, SPRITE_SIZE_X, SPRITE_SIZE_Y));					
				if ((TileMap[i][j] == 'f')) s_map.setTextureRect(IntRect(3 * SPRITE_SIZE_X, 0, SPRITE_SIZE_X, SPRITE_SIZE_Y));
				if ((TileMap[i][j] == 's')) s_map.setTextureRect(IntRect(2 * SPRITE_SIZE_X, 0, SPRITE_SIZE_X, SPRITE_SIZE_Y));	
				if ((TileMap[i][j] == 'p')) s_map.setTextureRect(IntRect(5 * SPRITE_SIZE_X, 0, SPRITE_SIZE_X, SPRITE_SIZE_Y));	

				s_map.setPosition(j * SPRITE_SIZE_Y, i * SPRITE_SIZE_X);				

				window.draw(s_map);														
			}

		std::ostringstream playerHealthString, gameTimeString;							
		playerHealthString << p.health; gameTimeString << gameTime;						


		if (p.alive && !p.isEndGame){													

			window.draw(p.sprite);														
		}
		else
		{
			if (!p.alive) {																
				Image deathImage;
				Texture deathTexture;
				Sprite deathSprite;

				deathImage.loadFromFile("images/boom.png");
				
				deathTexture.loadFromImage(deathImage);								
				deathSprite.setTexture(deathTexture);									

				deathX = p.getPlayerCoordinateX();										
				deathY = p.getPlayerCoordinateY();										
				deathSprite.setPosition(deathX, deathY);								

				window.draw(deathSprite);
			}
		}

		window.display();																
	}
	ASSERT_EQ(p.onGround, true);
	ASSERT_EQ(p.health, 2);
	ASSERT_EQ(p.alive, true);
}

TEST(gameInit2, gameCoordinates) {
    sf::RenderWindow window(sf::VideoMode(640, 480), "Bounce");							
	view.reset(sf::FloatRect(0, 0, 640, 480));											

	float currPlayerX, currPlayerY = 0;
	float currCameraX, currCameraY = 0;

	Ball p("bounce.png", INITIAL_POS_X, INITIAL_POS_Y, SPRITE_SIZE_X, SPRITE_SIZE_Y);	

	Image map_image;																	
	Texture map;																		
	Sprite s_map;																		

	map_image.loadFromFile("images/map.png");											

	map.loadFromImage(map_image);														

	s_map.setTexture(map);																
	
	
	SetCameraSize();


	Clock clock;

	Clock gameTimeClock;																
	int gameTime = 0;																	
	float deathX, deathY = 0;															
	TileMap[6][3] = 'f';
	TileMap[5][3] = 'f';
	TileMap[4][3] = 'f';
	int a = 0;
	while ((gameTime < 2) && (a == 0)) {															
		float time = clock.getElapsedTime().asMicroseconds();
		if (p.alive) {																	
			gameTime = gameTimeClock.getElapsedTime().asSeconds();						
		}

		clock.restart();																
		time = time / 300;																

		sf::Event event;																

		currPlayerX = p.getPlayerCoordinateX();		
		currPlayerY = p.getPlayerCoordinateY();		
		currCameraX = view.getCenter().x;			
		currCameraY = view.getCenter().y;			

		while (window.pollEvent(event)){
			if (event.type == sf::Event::Closed)										
				window.close();
		}
		if (p.alive || !p.isEndGame) {													
			p.moveCamera(time, currPlayerX, currPlayerY, currCameraX, currCameraY);
		}

		p.update(time);

		window.setView(view);															

		window.clear();
		
		for (int i = 0; i < HEIGHT_MAP; i++)
			for (int j = 0; j < WIDTH_MAP; j++){

				if (TileMap[i][j] == ' ')  s_map.setTextureRect(IntRect(SPRITE_SIZE_X, 0, SPRITE_SIZE_X, SPRITE_SIZE_Y));		
				if (TileMap[i][j] == 'h')  s_map.setTextureRect(IntRect(4 * SPRITE_SIZE_X, 0, SPRITE_SIZE_X, SPRITE_SIZE_Y));	
				if ((TileMap[i][j] == '0')) s_map.setTextureRect(IntRect(0, 0, SPRITE_SIZE_X, SPRITE_SIZE_Y));					
				if ((TileMap[i][j] == 'f')) s_map.setTextureRect(IntRect(3 * SPRITE_SIZE_X, 0, SPRITE_SIZE_X, SPRITE_SIZE_Y));	
				if ((TileMap[i][j] == 's')) s_map.setTextureRect(IntRect(2 * SPRITE_SIZE_X, 0, SPRITE_SIZE_X, SPRITE_SIZE_Y));	
				if ((TileMap[i][j] == 'p')) s_map.setTextureRect(IntRect(5 * SPRITE_SIZE_X, 0, SPRITE_SIZE_X, SPRITE_SIZE_Y));	

				s_map.setPosition(j * SPRITE_SIZE_Y, i * SPRITE_SIZE_X);				
				window.draw(s_map);														
			}

		std::ostringstream playerHealthString, gameTimeString;							
		playerHealthString << p.health; gameTimeString << gameTime;						


		if (p.alive && !p.isEndGame){													
			window.draw(p.sprite);														
		}
		else
		{
			if (!p.alive) {																
				Image deathImage;
				Texture deathTexture;
				Sprite deathSprite;

				deathImage.loadFromFile("images/boom.png");
				
				deathTexture.loadFromImage(deathImage);							
				deathSprite.setTexture(deathTexture);							
				deathX = p.getPlayerCoordinateX();									
				deathY = p.getPlayerCoordinateY();									
				deathSprite.setPosition(deathX, deathY);							
				a = 1;
				window.draw(deathSprite);
			}
		}

		window.display();															
	}
	ASSERT_EQ(p.onGround, false);
	ASSERT_EQ(p.health, 0);
	ASSERT_EQ(p.alive, false);
	
}


int main(int argc, char** argv) {
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
