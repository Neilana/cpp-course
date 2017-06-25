#include "Factories.h"

using namespace IlluminatiConfirmed;
using namespace experimental;

std::pair<std::shared_ptr<Ground>,
          std::vector<std::shared_ptr<experimental::BaseInterface>>>
FactoryObjects::create_map(const std::string &file, b2World *world) {
  Level level;
  level.loadMapFromFile(file);

  auto p_texture = Instance().getTexture(std::string(MAP_DIRECTORY) +
                                         level.GetMapInfo().m_name_of_tileset);

  std::shared_ptr<Ground> vec_of_graund;
  vec_of_graund = std::make_shared<Ground>(p_texture.get(),
                                           level.GetLayerByName(LAYER_GROUND));

  std::vector<std::shared_ptr<experimental::BaseInterface>> p_vec_of_maps_stuff;
  auto big_objs =
      level.GetVecOfBigObjs(OBJECT_SPRITE, OBJECT_BODY, LAYER_BUILDINGS);
  for (auto &&it : big_objs) {
    auto pointer = std::static_pointer_cast<experimental::BaseInterface>(
        std::make_shared<experimental::Building>(world, p_texture.get(),
                                                 std::move(it)));

    p_vec_of_maps_stuff.push_back(std::move(pointer));
  }
  return {std::move(vec_of_graund), std::move(p_vec_of_maps_stuff)};
}

std::shared_ptr<experimental::BaseInterface> FactoryObjects::create_character(
    int id, b2World *world) {
  static GenericObjectFactory<std::string, experimental::BaseCharacter,
                              b2World *, sf::Texture *,
                              experimental::CharacterSpriteInfo>
      characters_factory = registrationTypesOfCharacters();

  QString str =
      "SELECT * FROM CharactersImages WHERE Id=" + QString::number(id);
  QSqlQuery query;
  query.exec(str);
  while (query.next()) {
    std::string fileName =
        CHARACTERS_SPRITES_DIRECTORY + query.value(1).toString().toStdString();
    int width = query.value(2).toInt();
    int height = query.value(3).toInt();
    int frames = query.value(4).toInt();
    std::string master = query.value(7).toString().toStdString();
    int size = query.value(8).toInt();

    auto p_texture = Instance().getTexture(fileName);

    auto pers =
        std::shared_ptr<experimental::BaseInterface>(characters_factory.get(
            master)(world, p_texture.get(),
                    experimental::CharacterSpriteInfo(
                        {width, height, size, frames, 300, 300})));
    return pers;
  }
  throw EXCEPTION("Something wrong", nullptr);
}

std::string FactoryObjects::getList() {
  //как то надо вывести все чо есть, заготовка для dll
  return std::string();
}

std::shared_ptr<sf::Texture> FactoryObjects::getTexture(
    const std::string &file) {
  return m_textures.getResource(file);
}

std::shared_ptr<IlluminatiSound> FactoryObjects::getSound(
    const std::string &file) {
  return m_sounds.getResource(file);
}
