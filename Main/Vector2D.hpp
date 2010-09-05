#ifndef __Vector2D__
#define __Vector2D__

struct Vector2D
{
public:
	int x, y;

	Vector2D();
	Vector2D(int, int);

	Vector2D operator-() const;
	bool operator==(const Vector2D&) const;
	bool operator!=(const Vector2D&) const;
};

#endif