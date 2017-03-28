/*
** game.h for lemipc in PSU_2016_lemipc/src
**
** Made by brout_m
** Login   <marc.brout@epitech.eu>
**
** Started on  Tue Mar 28 19:08:56 2017 brout_m
** Last update Tue Mar 28 19:10:52 2017 brout_m
*/

#ifndef GAME_H_
# define GAME_H_

# include <stdbool.h>

# define WIDTH 10
# define HEIGHT 10
# define MAP(m, y, x) ((m)[(y) * HEIGHT + (x)])
# define TOP(p, m) (MAP(m, (p)->y - 1, (p)->x))
# define BOT(p, m) (MAP(m, (p)->y + 1, (p)->x))
# define RIGHT(p, m) (MAP(m, (p)->y, (p)->x + 1))
# define LEFT(p, m) (MAP(m, (p)->y, (p)->x - 1))
# define TOPL(p, m) (MAP(m, (p)->y - 1, (p)->x - 1))
# define TOPR(p, m) (MAP(m, (p)->y - 1, (p)->x + 1))
# define BOTL(p, m) (MAP(m, (p)->y + 1, (p)->x - 1))
# define BOTR(p, m) (MAP(m, (p)->y + 1, (p)->x + 1))
# define MESSAGE_LIFETIME 5

typedef struct s_player *Player;
typedef struct s_target *Target;
typedef int *Map;
typedef int (*isCell)(Player, Map);
typedef bool (*moveTo)(Player, Map);
typedef bool (*tryTo)(Player, Map, Target const);

typedef enum	e_sema
  {
    LOOP = 0,
    GRAPH,
    NB_SEM
  }		t_sema;

enum DIR
  {
    TOP = 0,
    TOPR,
    RIGHT,
    BOTR,
    BOT,
    BOTL,
    LEFT,
    TOPL
  };

typedef struct	s_player
{
  int		x;
  int		y;
  int		team;
  int		turn;
}		t_player;

typedef struct	s_target
{
  int		x;
  int		y;
  int		sender;
  int		lifeTime;
  int		dist;
}		t_target;

typedef struct	s_msg
{
  long		mtype;
  t_target	target;
}		t_msg;

int isTop(Player, Map);
int isTopRight(Player, Map);
int isRight(Player, Map);
int isBotRight(Player, Map);
int isBot(Player, Map);
int isBotLeft(Player, Map);
int isLeft(Player, Map);
int isTopLeft(Player, Map);

bool moveTop(Player, Map);
bool moveRight(Player, Map);
bool moveBot(Player, Map);
bool moveLeft(Player, Map);

bool checkDead(Player, Map);
bool allDone(bool const done[4]);
bool isGameOver(Map const);
void moveAtRandom(int, Player, Map);

bool tryToMoveTo(Player, Map, Target const);

void sendOrder(int, Player, Map);
bool getOrder(int, Player, Map, bool*);

#endif /* !GAME_H_ */
