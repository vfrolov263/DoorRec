#pragma once
#include <opencv2/opencv.hpp>

#define PI 3.14159265        

using cv::Point2i;
using cv::Vec4i;

extern int getPointsDistance(int, int, int, int);

// Класс, описывающий отрезок.
class CLine
{
	Point2i _point1, _point2;	// Начальная и конечная точки отрезка.
	bool _bHorisontal;	// Флаг горизонтальности.
	int _ilength;	// Длинна отрезка.
	int _iAngle; // Угол отрезка.
	float _fA, _fB, _fC; // Ax + By + C = 0.
public:
	/*
	Констурктор. Инициализируется отрезками нулевой длинны.
	*/
	CLine() : CLine(0, 0, 0, 0, 0) {}

	/*
	Констурктор.
	param[in] x1, y1, x2, y2 - координаты отрезка.
	*/
	CLine(int x1, int y1, int x2, int y2) : CLine(x1, y1, x2, y2, getPointsDistance(x1, y1, x2, y2)) {}

	/*
	Констурктор.
	param[in] x1, y1, x2, y2 - координаты отрезка.
	param[in] length - длинна отрезка.
	*/
	CLine(int x1, int y1, int x2, int y2, int length);

	/*
	Деструктор.
	*/
	~CLine() {}

	/*
	Возвращает угол отрезка.
	*/
	inline const int getAngle() const { return _iAngle; }

	/*
	Возвращает длинну отрезка.
	*/
	inline const int getLength() const { return _ilength; }

	/*
	Возвращает флаг горизонтальности: true - отрезок входит в группу "горизонтальных" отрезков.
	*/
	inline const bool isHorizontal() const { return _bHorisontal; }

	/*
	Возвращает флаг вертикальности: true - отрезок входит в группу "вертикальных" отрезков.
	*/
	inline const bool isVertical() const { return !_bHorisontal; }

	/*
	Возвращает точку A отрезка.
	*/
	inline Point2i getPointA() const { return _point1; }

	/*
	Возвращает точку B отрезка.
	*/
	inline Point2i getPointB() const { return _point2; }

	/*
	Возвращает параметр A из уравнения прямой Ax + By + C = 0.
	*/
	inline const float getA() const { return _fA; }

	/*
	Возвращает параметр B из уравнения прямой Ax + By + C = 0.
	*/
	inline const float getB() const { return _fB; }

	/*
	Возвращает параметр C из уравнения прямой Ax + By + C = 0.
	*/
	inline const float getC() const { return _fC; }

	/*
	Рассчитывает параметры отрезка по координатам.
	param[in] x1, y1, x2, y2- координаты отрезка.
	*/
	void setLine(int x1, int y1, int x2, int y2);
};