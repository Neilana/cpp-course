#include "ScreenMenu.h"

#include "constants.h"

using IlluminatiConfirmed::ScreenMenu;

ScreenMenu::ScreenMenu() {
  //  alpha_max = 3 * 255;
  // alpha_div = 3;
  playing = false;

  float x = WINDOW_WIDTH / 2.0 - 50;
  float y = WINDOW_HEIGHT / 2.0 - 140;

  float fontSize = 35;
  int deltaY = 35;

  if (!m_backgroundTexture.loadFromFile(
          "../Game/sprites/backgrounds/menu3_1.png")) {
    // std::cout << "Error loading presentation.gif" << std::endl;
    // return (-1);
  }
  m_backgroundSprite.setTexture(m_backgroundTexture);
  // sprite.setColor(sf::Color(255, 255, 255, alpha));
  if (!m_font.loadFromFile("../Game/res/Franchise-Bold-hinted.ttf")) {
    // std::cerr << "Error loading verdanab.ttf" << std::endl;
    // return (-1);
  }

  m_selectedOption = 0;

  // 0
  sf::Text option;
  option.setFont(m_font);
  option.setCharacterSize(fontSize);
  option.setString("New Game");
  option.setPosition(x, y);
  option.setColor(sf::Color::Green);
  m_menuOptions.push_back(option);
  m_screenNameByOption[0] = ScreenName::Game;

  // 1
  y += deltaY;
  option.setColor(sf::Color::White);
  option.setString("Continue");
  option.setPosition(x, y);
  m_menuOptions.push_back(option);
  m_screenNameByOption[1] = ScreenName::Game;

  //  y += deltaY;
  //  option.setColor(sf::Color::White);
  //  option.setString("Load");
  //  option.setPosition(x, y);
  //  m_menuOptions.push_back(option);

  //  y += deltaY;
  //  option.setColor(sf::Color::White);
  //  option.setString("Save");
  //  option.setPosition(x, y);
  //  m_menuOptions.push_back(option);

  y += deltaY;
  option.setString("Exit");
  option.setPosition(x, y);
  m_menuOptions.push_back(option);
  // m_screenByOption[0] = ScreenName::Game;
}

ScreenName ScreenMenu::run(Game &game, sf::RenderWindow &window) {
  sf::Event Event;
  bool running = true;
  ScreenName screen = ScreenName::MainMenu;
  while (running) {
    // Verifying events
    while (window.pollEvent(Event)) {
      // Window closed
      if (Event.type == sf::Event::Closed) {
        running = false;
        break;
        // return (-1);
      }
      // Key pressed
      if (Event.type == sf::Event::KeyPressed) {
        m_menuOptions[m_selectedOption].setColor(sf::Color::White);
        switch (Event.key.code) {
          case sf::Keyboard::Up:
            m_selectedOption = m_selectedOption == 0 ? m_menuOptions.size() - 1
                                                     : m_selectedOption - 1;
            break;
          case sf::Keyboard::Down:
            m_selectedOption = m_selectedOption < m_menuOptions.size() - 1
                                   ? m_selectedOption + 1
                                   : 0;
            break;
          case sf::Keyboard::Return:

            if (m_selectedOption == m_menuOptions.size() - 1) window.close();
            if (m_selectedOption == 0) screen = ScreenName::Game;
            screen = m_screenNameByOption[m_selectedOption];
            running = false;

            break;
        }
        m_menuOptions[m_selectedOption].setColor(sf::Color::Green);
      }

      // Clearing screen
      window.clear();

      // Drawing
      window.draw(m_backgroundSprite);

      // рисуем пункты меню
      for (auto &&it : m_menuOptions) window.draw(it);
    }
    window.display();
  }

  return screen;
}
