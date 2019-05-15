#include "pch.h"
#include "Line.h"
#include "DoorRec.h"
#include <math.h>

/*
¬озвращает угол отрезка по координатам.
param[in] x1, y1, x2, y2- координаты отрезка.
*/
int calcAngle(int x1, int y1, int x2, int y2);

CLine::CLine(int x1, int y1, int x2, int y2, int length): _point1(x1, y1), _point2(x2, y2), _ilength(length)
{
	setLine(x1, y1, x2, y2); // –ассчитываем параметры отрезка.
}

int calcAngle(int x1, int y1, int x2, int y2)
{
	// –ассчитываем угол отрезка.
	float fDeltaX = (float)(x1 - x2);
	float fDeltaY = (float)(y1 - y2);
	return (int)(atan(fDeltaY / fDeltaX) * 180.0f / PI);
}

void CLine::setLine(int x1, int y1, int x2, int y2)
{
	// «адаем направление отрезка слева - направо дл€ упрощени€ расчетов над отрезками.
	if (x1 > x2)
	{
		_point1.x = x2;
		_point1.y = y2;
		_point2.x = x1;
		_point2.y = y1;
	}
	else
	{
		_point1.x = x1;
		_point1.y = y1;
		_point2.x = x2;
		_point2.y = y2;
	}

	_iAngle = calcAngle(x1, y1, x2, y2); // –ассчитываем угол.

	// –ассчитываем параметры отрезка.
	_fA = (float)(y1 - y2);
	_fB = (float)(x2 - x1);
	_fC = (float)(x1 * y2 - x2 * y1);
}