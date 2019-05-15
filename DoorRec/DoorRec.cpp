// DoorRec.cpp : Этот файл содержит функцию "main". Здесь начинается и заканчивается выполнение программы.
//

#include "pch.h"
#include <iostream>
#include "DoorRec.h"
#include <opencv2/opencv.hpp>
#include "opencv2/core/core_c.h"
#include "opencv2/imgproc/imgproc_c.h"
#include "opencv2/highgui/highgui_c.h"
#include <stdlib.h>
#include <stdio.h>
#include <math.h>

using namespace std;
using namespace cv;

/*
Функция main.
Описание параметров алгоритма см. в файле DoorRec.h.
*/
int main(int argc, char** argv)
{
	Mat inputMat;		// Исходное изображение.
	Mat contoursMat;	// Изображение контуров.

	vector<CLine> horLinesVect, vertLinesVect; // Вектора горизонтальных и вертикальных линий.

	// Считываем исходное изображение.
	if (argc == 2)
		inputMat = imread(argv[1]);
	else
		inputMat = imread(DOOR_IMAGE);

	// Если файл не считан - возвращаем код ошибки.
	if (!inputMat.data)
		return -1;

	// Конвертируем изображение в оттенки серого.
	cvtColor(inputMat, contoursMat, CV_BGR2GRAY);

	// Размытие - убираем шумы матрицей размера (BLUR_SIZExBLUR_SIZE).
	blur(contoursMat, contoursMat, Size(BLUR_SIZE, BLUR_SIZE));

	// Обнаружение границ детектором Кэнни.
	Canny(contoursMat, contoursMat, LOW_THR, UP_THR);

	vector<Vec4i> linesVect; // Вектор линий.

	// Поиск отрезков преобразованием Хафа.
	HoughLinesP(contoursMat, linesVect, DOOR_LINES_RHO, DOOR_THETA, DOOR_LINES_THRESHOLD, DOOR_LINES_MIN_length, DOOR_LINES_MAX_GAP);

	// Если отрезков не обнаружено - алгоритм завершен.
	if (linesVect.size() == 0)
	{
		cout << "Can't see any lines!\n";
		horLinesVect.clear();
		vertLinesVect.clear();
		waitKey(0);
		return 0;
	}

	// Разбиваем отрезки на две группы - горизонтальные и вертикальные для упрощения дальнейших расчетов.
	for (int i = 0; i < linesVect.size(); ++i)
	{
		// В создаваемом объекте отрезка (в конструкторе) рассчитваются параметры отрезка по его координатам.
		CLine line(linesVect[i][0], linesVect[i][1], linesVect[i][2], linesVect[i][3]);

		// Производим выборку в одну из групп по углу отрезка. Отрезки, не попавшие ни в одну из групп, в дальнейших рассчетах не учавствуют.
		if (line.getAngle() >= -DOOR_LINES_HORIZONTAL_LIMIT && line.getAngle() <= DOOR_LINES_HORIZONTAL_LIMIT)
			horLinesVect.push_back(line);
		else if (line.getAngle() >= DOOR_LINES_VERTICAL_LIMIT || line.getAngle() <= -DOOR_LINES_VERTICAL_LIMIT)
			vertLinesVect.push_back(line);
	}

	linesVect.clear();
	linesSample(horLinesVect);
	linesSample(vertLinesVect);

	// Выводим изображение inputMat2, в которое выводим обнаруженные отрезки.
	Mat inputMat2;
	inputMat.copyTo(inputMat2);

	for (vector<CLine>::iterator it = horLinesVect.begin(); it != horLinesVect.end(); ++it)
		line(inputMat2, Point(it->getPointA().x, it->getPointA().y), Point(it->getPointB().x, it->getPointB().y), Scalar(DOOR_CONTOUR_COLOR), 3, LINE_AA);

	for (vector<CLine>::iterator it = vertLinesVect.begin(); it != vertLinesVect.end(); ++it)
		line(inputMat2, Point(it->getPointA().x, it->getPointA().y), Point(it->getPointB().x, it->getPointB().y), Scalar(DOOR_CONTOUR_COLOR), 3, LINE_AA);

	namedWindow("Lines", WINDOW_NORMAL);
	imshow("Lines", inputMat2);

	// horNodesVect - вектор списков всех пересечений с вертикальными отрезками для каждого горизонтального отрезка.
	// Для каждого горизонтального отрезка список пересечений хранится в формате пар "ключ-значение", где:
	// ключ - это id вертикального отрезка (по факту - итератор, т.е. адрес в памяти),
	// значение - координаты точки пересечения с горизонтальным отрезком.
	vector<map<vector<CLine>::iterator, Point2i>> horNodesVect;	

	// Вектор обнаруженных четырехугольников, удовлетворяющих заданным ограничениям.
	vector<CQuadro> quadrosVect; 

	// Четырехугольник, являющийся дверью с наибольшей вероятностью.
	CQuadro door;

	// Ищем все пересечения горизонтальных и вертикальных отрезков.
	getNodes(horNodesVect, horLinesVect, vertLinesVect);

	horLinesVect.clear();
	vertLinesVect.clear();

	// Если пересечений не обнаружено - алгоритм завершен.
	if (horNodesVect.size() == 0)
	{
		cout << "Can't see any intersections of horizontal and vertical lines!\n";
		waitKey(0);
		return 0;
	}

	// Ищем четырехугольники, удовлетворяющие заданным ограничениям.
	getQuadros(quadrosVect, horNodesVect);
	horNodesVect.clear();

	// Выбираем четырехугольник, являющийся дверью с наибольшей вероятностью.
	chooseDoor(door, quadrosVect);
	quadrosVect.clear();

	// Если такой четырехугольник не найден - алгоритм завершен. 
	if (door.getPerimetr() == 0)
	{
		cout << "Can't see any doors!\n";
		quadrosVect.clear();
		waitKey(0);
		return 0;
	}

	// Выводим изображение inputMat, в которое выводим контуры обнаруженной двери.
	for (int i = 0; i < 4; ++i)
	{
		CLine doorLine(door.getLine(i));
		line(inputMat, Point(doorLine.getPointA().x, doorLine.getPointA().y), Point(doorLine.getPointB().x, doorLine.getPointB().y), Scalar(DOOR_CONTOUR_COLOR), 3, LINE_AA);
	}

	namedWindow("DoorRec", WINDOW_NORMAL);
	imshow("DoorRec", inputMat);
	quadrosVect.clear();
	waitKey(0);
	return 0;
}

void chooseDoor(CQuadro &door, vector<CQuadro> &quadrosVect)
{
	if (quadrosVect.size() == 0)
		return;

	vector<CQuadro>::iterator resIt;	// Итератор четырехугольника с максимальнм периметром.
	int maxPerim = 0;	// Значение максимального периметра.

	// Проходим по всем четырехугольникам и ищем максимальный периметр.
	for (vector<CQuadro>::iterator it = quadrosVect.begin(); it != quadrosVect.end(); ++it)
		if (maxPerim < it->getPerimetr())
		{
			maxPerim = it->getPerimetr();
			resIt = it;
		}

	// Искомый четырехугольник.
	if (maxPerim)
		door = (*resIt); 
}

void getQuadrosFromCommonVect(vector<CQuadro> &quadrosVect, vector<Vec4i> &commonVertsVect)
{
	int px, py;	// Кооординаты точки возможного пересечения двух отрезков (необходимы для выхова функции getLinesDistance).
	int width, height, ratio; // Ширина, высота и соотношение высоты к ширине рассматриваемого четырехугольника.

	// Рассматриваем все пары вектора, используя двойной цикл.
	for (vector<Vec4i>::iterator it1 = commonVertsVect.begin(); it1 != commonVertsVect.end() - 1; ++it1)
	{
		CLine line1((*it1)[0], (*it1)[1], (*it1)[2], (*it1)[3]); // Первый вертикальный отрезок.

		// Если данный отрезок не удовлетворяет ограничениям - переходим к следующему.
		if (line1.getLength() < DOOR_MIN_HEIGHT || line1.getLength() > DOOR_MAX_HEIGHT)
			continue;

		for (vector<Vec4i>::iterator it2 = it1 + 1; it2 != commonVertsVect.end(); ++it2)
		{
			CLine line2((*it2)[0], (*it2)[1], (*it2)[2], (*it2)[3]); // Второй вертикальный отрезок.

			// Если разница в длинне отрезков не удовлетворяет ограничениям - переходим к следующему второму отрезку.
			if (abs(line1.getLength() - line2.getLength()) > DOOR_MAX_FAULT)
				continue;

			// Ищем расстояние между отрезками - ширину четырехугольника. 
			width = getLinesDistance(line1.getPointA().x, line1.getPointA().y, line1.getPointB().x, line1.getPointB().y, line1.getA(), line1.getB(), line1.getC(), line1.getAngle(),
				line2.getPointA().x, line2.getPointA().y, line2.getPointB().x, line2.getPointB().y, line2.getA(), line2.getB(), line2.getC(), line2.getAngle(), px, py);

			// Если ширина не удовлетворяет ограничениям - переходим к следующему второму отрезку.
			if (width < DOOR_MIN_HEIGHT || width > DOOR_MAX_HEIGHT)
				continue;

			// Ищем соотношение высоты к ширине.
			height = (line1.getLength() + line2.getLength()) / 2;
			ratio = height / width;

			// Если соотношение высоты к ширине не удовлетворяет ограничениям - переходим к следующему второму отрезку.
			if (ratio < DOOR_MIN_RATIO || ratio > DOOR_MAX_RATIO)
				continue;

			// Если четырехугольник прошел все проверки - добавляем его в вектор обнаруженных четырехуголников.
			quadrosVect.push_back(CQuadro(line1, line2)); 
		}
	}
}

void getQuadros(vector<CQuadro> &quadrosVect, vector<map<vector<CLine>::iterator, Point2i>> &horNodesVect)
{
	// commonVertsVect - вектор общих вертикальных отрезков для двух горизонтальных отрезков.
	// То есть данные вертикальные отрезки имеют пересечения с обоими горизонтальными отрезками.
	vector<Vec4i> commonVertsVect;	

	// lessIt - указатель на список пересечений горизонтального отрезка, имеющего меньше пересечений, чем второй.
	// moreIt - указатель на список пересечений горизонтального отрезка, имеющего больше пересечений.
	vector<map<vector<CLine>::iterator, Point2i>>::iterator lessIt, moreIt; 

	// Рассматриваем все пары вектора, используя двойной цикл.
	for (vector<map<vector<CLine>::iterator, Point2i>>::iterator it1 = horNodesVect.begin(); it1 != horNodesVect.end() - 1; ++it1)
	{
		// Если пересечений меньше двух - данный горизонтальный отрезок не является частью четырехугольника.
		if (it1->size() < 2)
			continue;

		for (vector<map<vector<CLine>::iterator, Point2i>>::iterator it2 = it1 + 1; it2 != horNodesVect.end(); ++it2)
		{
			if (it2->size() < 2)
				continue;

			commonVertsVect.clear(); // Для всех пар формируется новый вектор общих вертикальных отрезков.

			// Определяем, в каком из горизонтальных отрезков меньше пересечений, для ускорения поиска общих вертикальных отрезков. 
			if (it1->size() < it2->size())
			{
				lessIt = it1;
				moreIt = it2;
			}
			else
			{
				lessIt = it2;
				moreIt = it1;
			}
			
			// Проходим по всем пересечениям первого горизонтального отрезка.
			for (map<vector<CLine>::iterator, Point2i>::iterator nIt = lessIt->begin(); nIt != lessIt->end(); ++nIt)
			{
				// Смотрим, пересекается ли второй горизонтальный отрезок с данным вертикальным отрезком.  
				map<vector<CLine>::iterator, Point2i>::iterator fIt = moreIt->find(nIt->first);

				// Если пересечение найдено.
				if (fIt != moreIt->end())
				{
					Vec4i line;

					// Строим отрезок по двум точкам пересечений, задаем его направление слева-направо (необходимо для функции getLinesDistance). 
					if (fIt->second.x < nIt->second.x)
					{
						line[0] = fIt->second.x;
						line[1] = fIt->second.y;
						line[2] = nIt->second.x;
						line[3] = nIt->second.y;
					}
					else
					{
						line[0] = nIt->second.x;
						line[1] = nIt->second.y;
						line[2] = fIt->second.x;
						line[3] = fIt->second.y;
					}

					commonVertsVect.push_back(line);
				}
			}

			// Если у данных горизонтальных отрезков не меньше двух общих вертикальных отрезков - строим возможные четырехугольники.
			if (commonVertsVect.size() >= 2)
				getQuadrosFromCommonVect(quadrosVect, commonVertsVect);
		}
	}
}

void getNodes(vector<map<vector<CLine>::iterator, Point2i>> &horNodesVect, vector<CLine> &horLinesVect, vector<CLine> &vertLinesVect)
{
	int px, py; // Кооординаты точки пересечения горизонтального и вертикального отрезков.
	bool hasNewNodesLine; // Флаг добавления новой записи в horNodesVect (т.е. увеличения размера horNodesVect).

	// Проходим по всем горизонтальным отрезкам.
	for (vector<CLine>::iterator it1 = horLinesVect.begin(); it1 != horLinesVect.end(); ++it1)
	{
		hasNewNodesLine = true; // Для данного горизонтального отрезка место в horNodesVect еще не добавлено.

		// Проходим по всем вертикальным отрезкам.
		for (vector<CLine>::iterator it2 = vertLinesVect.begin(); it2 != vertLinesVect.end(); ++it2)
		{
			// Проверяем данные отрезки на пересечение.Если расстояние между ними меньше заданного параметра DOOR_INTERSECTION_DISTANCE - это тоже считается пересечением.
			if (getLinesDistance(it1->getPointA().x, it1->getPointA().y, it1->getPointB().x, it1->getPointB().y, it1->getA(), it1->getB(), it1->getC(), it1->getAngle(),
				it2->getPointA().x, it2->getPointA().y, it2->getPointB().x, it2->getPointB().y, it2->getA(), it2->getB(), it2->getC(), it2->getAngle(), px, py) <= DOOR_INTERSECTION_DISTANCE)
			{
				// Выделяем память для данного горизонтального отрезка при необходимости.
				if (hasNewNodesLine)
				{
					horNodesVect.resize(horNodesVect.size() + 1);
					hasNewNodesLine = false;
				}

				// Добавляем пересечение.
				horNodesVect.back().insert(pair<vector<CLine>::iterator, Point2i>(it2, Point2i(px, py)));
			}
		}
	}
}

inline int getPointsDistance(int x1, int y1, int x2, int y2)
{
	return (int)sqrt(pow(x1 - x2, 2) + pow(y1 - y2, 2));
}

void calculateIntersectPoint(float A1, float B1, float C1, float A2, float B2, float C2, int &px, int &py)
{
	// Вычисляем координаты точки пересечения прямых.
	py = (int)((A1 * C2 - C1 * A2) / (A2 * B1 - A1 * B2));
	px = (int)((-C1 - B1 * py) / A1);
}

bool setPointInLine(int &px, int &py, int x1, int y1, int x2, int y2)
{
	// Если отрезок расположен под прямым углом к оси абцисс, проверяем точку по координате y.
	if (x1 == x2)
	{
		if (py < y1)
		{
			px = x1;
			py = y1;
			return true;
		}
		else if (py > y2)
		{
			px = x2;
			py = y2;
			return true;
		}
	}
	// Точка левее отрезка.
	else if (px < x1)
	{
		px = x1;
		py = y1;
		return true;
	}
	// Точка правее отрезка.
	else if (px > x2)
	{
		px = x2;
		py = y2;
		return true;
	}

	// Если координаты точки (px, py) не изменились - точка уже лежала на отрезке.
	return false;
}

bool isPointOnStraight(int x, int y, float A, float B, float C)
{
	return A * x + B * y + C == 0; // Точка, лежащая на прямой, удовлетворяет уравнению данной прямой.
}

int getPointToLineDistance(int x0, int y0, int x1, int y1, int x2, int y2, float A1, float B1, float C1, int &px, int &py)
{
	// (nx, ny) - вектор нормали к прямой.
	float nx = abs(A1);
	float ny = abs(B1);

	// Зная координаты вектора нормали строим уравнение прямой, перпендикулярной по отношению к прямой A1x + B1y + C1 = 0
	// и проходящей через точку (x0, y0).
	float A2 = ny;
	float B2 = -nx;
	float C2 = -ny * x0 + nx * y0;

	// Ищем координаты точки пересечения данных прямых (px, py) - это проекция точки (x0, y0) на прямую A1x + B1y + C1 = 0.
	calculateIntersectPoint(A1, B1, C1, A2, B2, C2, px, py);

	// Если данная точка не лежит на отрезке - ее координаты приравниваются к координатам ближайшей к ней точки, лежащей на данном отрезке.
	setPointInLine(px, py, x1, y1, x2, y2);

	// Искомое расстояние - расстояние между точками (x0, y0) и (px, py).
	return getPointsDistance(x0, y0, px, py);
}

bool isLinesIntersects(int x1, int y1, int x2, int y2, float A1, float B1, float C1,
	int x3, int y3, int x4, int y4, float A2, float B2, float C2, int &px, int &py)
{
	// Частный случай - если отрезки лежат на одной прямой. Проверяем, не перекрывают ли они друг друга.
	if (isPointOnStraight(x1, y1, A2, B2, C2) && isPointOnStraight(x2, y2, A2, B2, C2))
	{
		// Проверяем для крайних точек первого отрезка принадлежность второму и наоборот.
		// Если отрезки пересекаются (перекрываются) - одно из условий будет выполнено.
		if (!setPointInLine(x1, y1, x3, y3, x4, y4))
		{
			px = x1;
			py = y1;
			return true;
		}
		else if (!setPointInLine(x2, y2, x3, y3, x4, y4))
		{
			px = x2;
			py = y2;
			return true;
		}
		else if (!setPointInLine(x3, y3, x1, y1, x2, y2))
		{
			px = x3;
			py = y3;
			return true;
		}
		else if (!setPointInLine(x4, y4, x1, y1, x2, y2))
		{
			px = x4;
			py = y4;
			return true;
		}

		return false;
	}

	// Ищем точку пересечения прямых, проходящих через отрезки.
	calculateIntersectPoint(A1, B1, C1, A2, B2, C2, px, py);

	// Если данная точка принадлежит обоим отрезкам - отрезки пересекаются.
	return !setPointInLine(px, py, x1, y1, x2, y2) && !setPointInLine(px, py, x3, y3, x4, y4);
}

int getLinesDistance(int x1, int y1, int x2, int y2, float A1, float B1, float C1, int phi1,
	int x3, int y3, int x4, int y4, float A2, float B2, float C2, int phi2, int &px, int &py)
{
	// Если отрезки пересекаются - расстояние между ними нулевое.
	if (isLinesIntersects(x1, y1, x2, y2, A1, B1, C1, x3, y3, x4, y4, A2, B2, C2, px, py))
		return 0;

	int dist[12];

	// Ищем расстояние от обеих точек первого отрезка до второго и наоборот.
	// Расстояние между двумя отрезками - это минимальное расстояние между их точками.
	// В случае непараллельных отрезков, одна из точек вектора расстояния между ними будет совпадать с крайней точкой одного из данных отрезков.
	dist[0] = getPointToLineDistance(x1, y1, x3, y3, x4, y4, A2, B2, C2, px, py);
	dist[1] = px;
	dist[2] = py;

	dist[3] = getPointToLineDistance(x2, y2, x3, y3, x4, y4, A2, B2, C2, px, py);
	dist[4] = px;
	dist[5] = py;

	dist[6] = getPointToLineDistance(x3, y3, x1, y1, x2, y2, A1, B1, C1, px, py);
	dist[7] = px;
	dist[8] = py;

	dist[9] = getPointToLineDistance(x4, y4, x1, y1, x2, y2, A1, B1, C1, px, py);
	dist[10] = px;
	dist[11] = py;

	int minDist = 10000;

	// Ищем минимальное из найденных расстояний.
	for (int i = 0; i < 4; ++i)
		if (minDist > dist[i])
		{
			minDist = dist[i * 3];
			px = dist[i * 3 + 1];
			py = dist[i * 3 + 2];
		}

	return minDist;
}

bool findSimilarLines(vector<CLine> &linesVect)
{
	int px, py; // Кооординаты точки возможного пересечения двух отрезков (необходимы для выхова функции getLinesDistance).

	// Рассматриваем все пары вектора, используя двойной цикл.
	for (vector<CLine>::iterator it1 = linesVect.begin(); it1 != linesVect.end()-1; ++it1)
	{
		for (vector<CLine>::iterator it2 = it1 + 1; it2 != linesVect.end(); ++it2)
		{
			// Если разница углов отрезков не больше чем DOOR_LINES_MIN_ANGLE_DELTA, и расстояние между ними не больше чем DOOR_LINES_MIN_DISTANCE,
			// данные отрезки считаются похожими и могут быть заменены одним отрезком.
			if (abs(it1->getAngle() - it2->getAngle()) <= DOOR_LINES_MIN_ANGLE_DELTA &&
				getLinesDistance(it1->getPointA().x, it1->getPointA().y, it1->getPointB().x, it1->getPointB().y, it1->getA(), it1->getB(), it1->getC(), it1->getAngle(),
					it2->getPointA().x, it2->getPointA().y, it2->getPointB().x, it2->getPointB().y, it2->getA(), it2->getB(), it2->getC(), it2->getAngle(), px, py) <= DOOR_LINES_MIN_DISTANCE)
			{
				getMiddleLine(linesVect, it1, it2); // Рассчитываем средний отрезок.
				return true;
			}
		}
	}

	return false;
}

void linesSample(vector<CLine> &linesVect)
{
	while (findSimilarLines(linesVect)); // Пока в векторе есть пары похожих отрезков - продолжаем их замену на один.
}

void getMiddleLine(vector<CLine> &linesVect, vector<CLine>::iterator it1, vector<CLine>::iterator it2)
{
	// Координаты среднего отрезка.
	int x1 = 10000,	y1 = 10000,	x2 = -10000, y2 = -10000; 

	// Точки похожих отрезков.
	Point2i points[4] = { it1->getPointA(), it1->getPointB(), it2->getPointA(), it2->getPointB() };

	// x1 - минимальная координата по оси OX из всех точек данных отрезков.
	// x2 - максимальная координата по оси OX из всех точек данных отрезков.
	// y1 - минимальная координата по оси OY из всех точек данных отрезков.
	// y2 - максимальная координата по оси OY из всех точек данных отрезков.
	for (int i = 0; i < 4; ++i)
	{
		if (x1 > points[i].x)
			x1 = points[i].x;

		if (x2 < points[i].x)
			x2 = points[i].x;

		if (y1 > points[i].y)
			y1 = points[i].y;

		if (y2 < points[i].y)
			y2 = points[i].y;
	}

	(*it1) = CLine(x1, y1, x2, y2); // Заменяем первый отрезок в векторе на найденный средний.
	linesVect.erase(it2); // Второй отрезок удаляем.
}

// Запуск программы: CTRL+F5 или меню "Отладка" > "Запуск без отладки"
// Отладка программы: F5 или меню "Отладка" > "Запустить отладку"

// Советы по началу работы 
//   1. В окне обозревателя решений можно добавлять файлы и управлять ими.
//   2. В окне Team Explorer можно подключиться к системе управления версиями.
//   3. В окне "Выходные данные" можно просматривать выходные данные сборки и другие сообщения.
//   4. В окне "Список ошибок" можно просматривать ошибки.
//   5. Последовательно выберите пункты меню "Проект" > "Добавить новый элемент", чтобы создать файлы кода, или "Проект" > "Добавить существующий элемент", чтобы добавить в проект существующие файлы кода.
//   6. Чтобы снова открыть этот проект позже, выберите пункты меню "Файл" > "Открыть" > "Проект" и выберите SLN-файл.

