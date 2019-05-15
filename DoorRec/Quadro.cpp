#include "pch.h"
#include "Quadro.h"

CQuadro::CQuadro(CLine line1, CLine line2) : _lines{ line1, line2, CLine(), CLine() }
{
	// Строим на основе вертикальных отрезков горизонтальные. Если не соблюсти порядок соединения - получим диагонали, поэтому необходима проверка.
	if ((_lines[0].getPointA().y - _lines[0].getPointB().y) * (_lines[1].getPointA().y - _lines[1].getPointB().y) < 0)
	{
		_lines[2].setLine(_lines[0].getPointA().x, _lines[0].getPointA().y, _lines[1].getPointB().x, _lines[1].getPointB().y);
		_lines[3].setLine(_lines[0].getPointB().x, _lines[0].getPointB().y, _lines[1].getPointA().x, _lines[1].getPointA().y);
	}
	else
	{
		_lines[2].setLine(_lines[0].getPointA().x, _lines[0].getPointA().y, _lines[1].getPointA().x, _lines[1].getPointA().y);
		_lines[3].setLine(_lines[0].getPointB().x, _lines[0].getPointB().y, _lines[1].getPointB().x, _lines[1].getPointB().y);
	} 
}

const CLine &CQuadro::getLine(unsigned int num)
{ 
	// Если задан неверный номер - возвращаем нулевой отрезок.
	if (num > 3)
		_lines[3];

	return _lines[num];
}

int CQuadro::getPerimetr()
{
	int perim = 0;

	// Суммируем длинну всех отрезков.
	for (int i = 0; i < 4; ++i)
		perim += _lines[i].getLength();

	return perim;
}