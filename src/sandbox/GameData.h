// GameData.h

#ifndef _GAMEDATA_h
#define _GAMEDATA_h

#include "Arduino.h"


class GameData
{

public:
  GameData();
  int getPlayer();
  int getAmmo();
  int getHealth();
  void setAmmo(int ammo);
  void setHealth(int health);
private:
  int myAmmo;
  int myHealth;
};

extern GameData GAMEDATA;

#endif


