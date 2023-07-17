#ifndef FRAME_H
#define FRAME_H

struct Coordinates
{
	float U;
	float V;
};

class Frame
{
public:
	Frame();
	~Frame();
	Coordinates frameCoords[4];
};
#endif // FRAME_H