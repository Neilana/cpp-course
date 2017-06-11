#include <SFML/Graphics.hpp>
#include "Box2D/Box2D.h"
#include "SFMLDebugDraw.h"

#include <exception>
#include <iostream>
#include <vector>

#include "../Exceptions/Exception.h"
#include "Character.h"
#include "Game.h"
#include "Level.h"
#include "Screen.h"
#include "ScreenGame.h"
#include "ScreenMenu.h"
//#include "constants.h"

using namespace sf;
using namespace std;

using IlluminatiConfirmed::Character;
using IlluminatiConfirmed::Game;
using IlluminatiConfirmed::Screen;
using IlluminatiConfirmed::ScreenMenu;
using IlluminatiConfirmed::ScreenGame;

int main() {
  try {
    std::map<ScreenName, Screen *> screenNameToScreen;

    // menu = 0
    ScreenMenu screen0;
    screenNameToScreen[ScreenName::MainMenu] = &screen0;

    // game = 1
    ScreenGame screen1;
    screenNameToScreen[ScreenName::Game] = &screen1;

    sf::RenderWindow window;
    window.create(sf::VideoMode(WINDOW_WIDTH, WINDOW_HEIGHT), "Level.h test");
    window.setFramerateLimit(60);

    std::stringstream sstream;
    sf::Text fpsCounter;
    sf::Font mainFont;
    if (!mainFont.loadFromFile(
            "../Game/res/Franchise-Bold-hinted.ttf"))  // Set path to your font
      throw EXCEPTION("I can't open file with font.", nullptr);
    fpsCounter.setFont(mainFont);
    fpsCounter.setColor(sf::Color::White);

    Game game(window);
    game.initNewGame(MAP_FILE_1);

    Clock clock;

    // while (screenName != ScreenName::Exit) {
    ScreenName screenName = ScreenName::MainMenu;
    while (window.isOpen()) {
      screenName = screenNameToScreen[screenName]->run(game, window);
    }
  } catch (IlluminatiConfirmed::Exception &e) {
    std::cout << e.what();
  }

  return 0;
}
