#include "game.h"
#include "object.h"
#include <stdio.h>
#include <time.h>

typedef struct Map Map;

void *Map_move(void *self, Direction direction);
int Map_attack(void *self, int damage);
int Map_init(void *self);

extern Object MapProto;

struct Map {
  Object proto;
  Room *start;
  Room *location;
  int player_hp;
};

void *Map_move(void *self, Direction direction) {
  Map *map = self;
  Room *location = map->location;
  Room *next = NULL;

  next = location->_(move)(location, direction);

  if (next) {
    map->location = next;
    map->player_hp -= 1;
  }

  return next;
}

int Map_attack(void *self, int damage) {
  Map *map = self;
  Room *location = map->location;

  return location->_(attack)(location, damage);
}

int Map_init(void *self) {
  Map *map = self;

  // make some rooms for a small map
  Room *hall = NEW(Room, "The great Hall");
  Room *throne = NEW(Room, "The throne room");
  Room *arena = NEW(Room, "The arena, with the minotaur");
  Room *kitchen = NEW(Room, "Kitchen, you have the knife now");

  // put the bad guy in the arena
  arena->bad_guy = NEW(Monster, "The little yellow duck");

  // setup the map rooms
  hall->north = throne;

  throne->west = arena;
  throne->east = kitchen;
  throne->south = hall;

  arena->east = throne;
  kitchen->west = throne;

  // start the map and the character off in the hall
  map->start = hall;
  map->location = hall;

  map->player_hp = 5;

  return 1;
}

Object MapProto = {.init = Map_init, .move = Map_move, .attack = Map_attack};

int process_input(Map *game) {
  printf("\n> ");

  char ch = getchar();
  getchar(); // eat ENTER

  int damage = rand() % 4;

  switch (ch) {
  case EOF:
    printf("Giving up? You suck.\n");
    return 0;
    break;

  case 'n':
    game->_(move)(game, NORTH);
    break;

  case 's':
    game->_(move)(game, SOUTH);
    break;

  case 'e':
    game->_(move)(game, EAST);
    break;

  case 'w':
    game->_(move)(game, WEST);
    break;

  case 'a':

    game->_(attack)(game, damage);
    break;
  case 'l':
    printf("You can go:\n");
    if (game->location->north)
      printf("NORTH\n");
    if (game->location->south)
      printf("SOUTH\n");
    if (game->location->east)
      printf("EAST\n");
    if (game->location->west)
      printf("WEST\n");
    break;

  default:
    printf("What?: %d\n", ch);
  }

  if (game->player_hp == 0) {
    printf("You have cost all your hp. You suck.\n");
    return 0;
  }
  Monster *duck = game->location->bad_guy;
  if (duck && duck->hit_points <= 0) {
    printf("You win!\n");
    return 0;
  }

  return 1;
}

int main(int argc, char *argv[]) {
  // simple way to setup the randomness
  srand(time(NULL));

  // make our map to work with
  Map *game = NEW(Map, "The Hall of the Minotaur.");

  printf("You enter the ");
  game->location->_(describe)(game->location);

  while (process_input(game)) {
  }

  return 0;
}
