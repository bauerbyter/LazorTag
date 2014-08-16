
#include "GameData.h"

const int myPlayer = 2;
int myHealth;
int myAmmo;


GameData::GameData():
myHealth(5),myAmmo(100){
}

int GameData::getPlayer()
{
  return myPlayer;
}

int GameData::getAmmo()
{
  return myAmmo;
}

void GameData::setAmmo(int ammo)
{
  myAmmo = ammo;
}

int GameData::getHealth()
{
  return myHealth;
}

void GameData::setHealth(int health)
{
  myHealth = health;
}

