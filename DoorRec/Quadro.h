#pragma once
#include "Line.h"

// Класс, описывающий четыырехугольник.
class CQuadro
{
	CLine _lines[4]; // Отрезки.
public:
	/*
	Констурктор.. Инициализируется отрезками нулевой длинны.
	*/
	CQuadro() : _lines{ CLine(), CLine(), CLine(), CLine() } {}

	/*
	Констурктор.
	param[in] line1, line2 - вертикальные отрезки четырехугольника.
	*/
	CQuadro(CLine line1, CLine line2);

	/*
	Деструктор.
	*/
	~CQuadro() {}

	/*
	Возвращает отрезок.
	param[in] num - номер отрезка в массиве _lines.
	*/
	const CLine &getLine(unsigned int num);

	/*
	Возвращает периметр четырехугольника.
	*/
	int getPerimetr();
};
