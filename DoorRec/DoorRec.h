#pragma once
#include <vector>
#include "Line.h"
#include "Quadro.h"

using namespace std;

/*
Значения ниже задают ключевые параметры алгоритма распознавания дверей.
*/

#define BLUR_SIZE	3								//  Размер матрицы, используемой при размытии изображения.
#define LOW_THR		20								//	Нижний порог детектора Кэнни.
#define UP_THR		60								//	Верхний порог детектора Кэнни.

#define DOOR_LINES_RHO			1					// Разрешение расстояния между точками, рассчитываемого преобразованием Хафа (определяет точность рассчитываемого расстояния).
#define DOOR_THETA				CV_PI / 180			// Разрешение углов, расчитываемых преобразованием Хафа (задается в радианах, определяет точность расчитываемых углов).
#define DOOR_LINES_THRESHOLD	150					// Пороговое значение для выборки отрезков, обнаруженных преобразованием Хафа.
#define DOOR_LINES_MIN_length	100					// Минимальная длинна отрезка, обнаруженные отрезки меньшей длинны не рассматриваются (испрользуется в преобразовании Хафа).
#define DOOR_LINES_MAX_GAP		250					// Максимальный разрыв между двумя точками одной линии (испрользуется в преобразовании Хафа).

#define DOOR_LINES_HORIZONTAL_LIMIT	20				// Отрезки с углом в диапазоне [-DOOR_LINES_HORIZONTAL_LIMIT; DOOR_LINES_HORIZONTAL_LIMIT] принимаются за горизонтальные.
#define DOOR_LINES_VERTICAL_LIMIT	70				// Отрезки с углом в диапазоне [-90; -DOOR_LINES_HORIZONTAL_LIMIT] && [DOOR_LINES_HORIZONTAL_LIMIT; 90] принимаются за вертикальные.

#define DOOR_LINES_MIN_ANGLE_DELTA	5				// Разница в углах отрезков, при которой отрезки могут рассматриваться, как отрезки с одинаковым углом.
#define DOOR_LINES_MIN_DISTANCE		5				// Расстояние между отрезками, при котором отрезки могут рассматриваться, как отрезки с нулевым расстоянием.

#define DOOR_INTERSECTION_DISTANCE	5				// Расстояние между горизонтальным и вертикальным отрезком, при которм данные отрезки можно считать пересекающимися.

#define DOOR_MIN_WIDTH				400				// Минимальная ширина двери.
#define DOOR_MAX_WIDTH				1500			// Максимальная ширина двери.
#define DOOR_MIN_HEIGHT				400				// Минимальная высота двери.
#define DOOR_MAX_HEIGHT				1500			// Максимальная высота двери.

#define DOOR_MAX_FAULT				30				// Максимальная допускаемая разница в длинне параллельных сторон двери.
#define DOOR_MIN_RATIO				1.0f			// Минимальное допускаемое соотношение высоты двери к ее ширине.
#define DOOR_MAX_RATIO				5.0f			// Максимальное допускаемое соотношение высоты двери к ее ширине.

#define DOOR_CONTOUR_COLOR			255, 0, 0		// Цвет контура для выделения двери.

#define DOOR_IMAGE		"images/33.jpg"				// Изображение, на котором необходимо произвести поиск двери.

/*
Возвращает расстояние межу двумя точками.
param[in] x1, y1, x2, y2 - координаты точек.
*/
inline int getPointsDistance(int x1, int y1, int x2, int y2);

/*
Рассчитывает координаты точки пересечения двух прямых. 
param[in] A1, B1, C1 - параметры первой прямой (A1x + B1y + C1 = 0).
param[in] A2, B2, C2 - параметры второй прямой (A2x + B2y + C2 = 0).
param[out] px, py - координаты точки пересечения.
*/
void calculateIntersectPoint(float A1, float B1, float C1, float A2, float B2, float C2, int &px, int &py);

/*
Возвращает true, если точка (px, py) принадлежит отрезку (x1, y1) - (x2, y2).
Если точка (px, py) не принадлежит отрезку, значения ее координат приравниваются к значениям ближайшей к ней точке, принадлежащей отрезку.
param[in] x1, y1, x2, y2 - координаты отрезка.
param[out] px, py - координаты рассматриваемой точки.
*/
bool setPointInLine(int &px, int &py, int x1, int y1, int x2, int y2);

/*
Возвращает true, если точка (x, y) лежит на прямой Ax + By + C = 0.
param[in] x, y - координаты рассматриваемой точки.
param[in] A, B, C - параметры прямой (Ax + By + C = 0).
*/
bool isPointOnStraight(int x, int y, float A, float B, float C);

/*
Возвращает расстояние от точки до отрезка.
param[in] x, y - координаты рассматриваемой точки.
param[in] x1, y1, x2, y2 - координаты отрезка.
param[in] A, B, C - параметры прямой (Ax + By + C = 0), проходящей через отрезок.
param[out] px, py - координаты точки, ближайшей к точке (x, y) и лежащей на отрезке.
*/
int getPointToLineDistance(int x, int y, int x1, int y1, int x2, int y2, float A1, float B1, float C1, int &px, int &py);

/*
Возвращает true, если два отрезка пересекаются.
param[in] x1, y1, x2, y2 - координаты первого отрезка.
param[in] A1, B1, C1 - параметры прямой (A1x + B1y + C1 = 0), проходящей через первый отрезок.
param[in] x3, y3, x4, y4 - координаты второго отрезка.
param[in] A2, B2, C2 - параметры прямой (A2x + B2y + C2 = 0), проходящей через второй отрезок.
param[out] px, py - координаты точки пересечения.
*/
bool isLinesIntersects(int x1, int y1, int x2, int y2, float A1, float B1, float C1, int x3, int y3, int x4, int y4, float A2, float B2, float C2, int &px, int &py);

/*
Возвращает расстояние между отрезками.
param[in] x1, y1, x2, y2 - координаты первого отрезка.
param[in] A1, B1, C1 - параметры прямой (A1x + B1y + C1 = 0), проходящей через первый отрезок.
param[in] phi1 - угол первого отрезка.
param[in] x3, y3, x4, y4 - координаты второго отрезка.
param[in] A2, B2, C2 - параметры прямой (A2x + B2y + C2 = 0), проходящей через второй отрезок.
param[in] phi2 - угол второго отрезка.
param[out] px, py - если отрезки пересекаются - координаты точки пересечения.
*/
int getLinesDistance(int x1, int y1, int x2, int y2, float A1, float B1, float C1, int phi1, int x3, int y3, int x4, int y4, float A2, float B2, float C2, int phi2, int &px, int &py);

/*
Выбирает четырехугольник, являющийся дверью с наибольшей вероятностью (критерий в данной программе - максимальный периметр).
param[in] quadrosVect - вектор четырехугольников.
param[out] door - четырехугольник, являющийся дверью с наибольшей вероятностью.
*/
void chooseDoor(CQuadro &door, vector<CQuadro> &quadrosVect);

/*
Ищет среди всех пересечений горизонтальных и вертикальных отрезков четырехугольники, удовлетворяющие заданным ограничениям.
param[in] horNodesVect - вектор всех пересечений горизонтальных и вертикальных отрезков.
param[out] quadrosVect - обнаруженные четырехугольники.
*/
void getQuadros(vector<CQuadro> &quadrosVect, vector<map<vector<CLine>::iterator, Point2i>> &horNodesVect);

/*
Для всех пар вертикальных отрезков из вектора commonVertsVect состовляет четырехугольники, и отсеивает те, которые не удовлетворяют заданным ограничениям.
Данная функция используется функцией getQuadros.
param[in] commonVertsVect - вектор вертикальных линий.
param[out] quadrosVect - обнаруженные четырехугольники.
*/
void getQuadrosFromCommonVect(vector<CQuadro> &quadrosVect, vector<Vec4i> &commonVertsVect);

/*
Ищет все пересечения горизонтальных и вертикальных отрезков.
param[in] horNodesVect - вектор горизонтальных отрезков.
param[in] vertLinesVect - вектор вертикальных отрезков.
param[out] horNodesVect - все обнаруженные пересечения горизонтальных и вертикальных отрезков.
*/
void getNodes(vector<map<vector<CLine>::iterator, Point2i>> &horNodesVect, vector<CLine> &horLinesVect, vector<CLine> &vertLinesVect);

/*
Рассчитывает средний отрезок, который заменит два похожих отрезка (похожие отрезки определяетмя параметрами DOOR_LINES_MIN_ANGLE_DELTA и DOOR_LINES_MIN_DISTANCE).
param[in] it1 - итератор, указывающий на первый отрезок.
param[in] it2 - итератор, указывающий на второй отрезок.
param[out] linesVect - вектор отрезков, который хранит оба отрезка и в который необходимо записать результат.
*/
void getMiddleLine(vector<CLine> &linesVect, vector<CLine>::iterator it1, vector<CLine>::iterator it2);

/*
Исследует вектор отрезков на предмет похожиж отрезков и заменяет их на средний отрезок, вычисленный при помощи функции getMiddleLine.
param[out] linesVect - вектор отрезков.
*/
void linesSample(vector<CLine> &linesVect);

/*
Находит пару похожих отрезков в векторе отрезков и заменяет ее на средний отрезок.
Возвращает true, если такая пара была найдена и заменена.
Данная функция используется функцией linesSample.
param[out] linesVect - вектор отрезков.
*/
bool findSimilarLines(vector<CLine> &linesVect);