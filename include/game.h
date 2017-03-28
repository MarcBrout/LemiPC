#ifndef GAME_H_
# define GAME_H_

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

typedef struct s_player *Player;
typedef int *Map;
typedef int (*isCell)(Player, Map);
typedef bool (*moveTo)(Player, Map);

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

struct s_player
{
    int     x;
    int     y;
    int     team;
    int     turn;
};

#endif /* !GAME_H_ */
