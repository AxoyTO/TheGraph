#include <cstdlib>

struct Boss {
  int health;
  int atk;
  int stamina_consumption;
  Boss() {
    health = std::rand() % 100;
    atk = std::rand() % 20;
    stamina_consumption = std::rand() % 10;
  }
};

struct Camp {
  int stamina_boost;
  Camp() { stamina_boost = std::rand() % 50 + 20; }
};
