#ifndef GAME_H_
# define GAME_H_

# define MAP(m, y, x) ((m)->map[(y)][(x)])
# define WIDTH(m) ((m)->width)
# define HEIGHT(m) ((m)->height)
# define TOP(p, m) (MAP(m, (p)->y - 1, (p)->x))
# define BOT(p, m) (MAP(m, (p)->y + 1, (p)->x))
# define RIGHT(p, m) (MAP(m, (p)->y, (p)->x + 1))
# define LEFT(p, m) (MAP(m, (p)->y, (p)->x - 1))
# define TOPL(p, m) (MAP(m, (p)->y - 1, (p)->x - 1))
# define TOPR(p, m) (MAP(m, (p)->y - 1, (p)->x + 1))
# define BOTL(p, m) (MAP(m, (p)->y + 1, (p)->x - 1))
# define BOTR(p, m) (MAP(m, (p)->y + 1, (p)->x + 1))

typedef struct s_player *Player;
typedef struct s_map *Map;
typedef int (*isCell)(Player, Map);

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

struct s_map
{
    int width;
    int height;
    int **map;
    int nbTeam;
};

struct s_player
{
    int     x;
    int     y;
    int     team;
    int     turn;
};

#endif /* !GAME_H_ */
