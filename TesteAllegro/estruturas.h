enum IDS{BULLET, ENEMY};

//struct SpaceShip
//{
//	int ID;
//	int x;
//	int y;
//	int lives;
//	int speed;
//	int boundx;
//	int boundy;
//	int score;
//};

typedef struct Bullet
{
	int ID;
	int x;
	int y;
	bool live;
	int speed;
	int pos_x,pos_y;
}Bullet;
