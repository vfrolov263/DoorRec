#pragma once
#include <vector>
#include "Line.h"
#include "Quadro.h"

using namespace std;

/*
�������� ���� ������ �������� ��������� ��������� ������������� ������.
*/

#define BLUR_SIZE	3								//  ������ �������, ������������ ��� �������� �����������.
#define LOW_THR		20								//	������ ����� ��������� �����.
#define UP_THR		60								//	������� ����� ��������� �����.

#define DOOR_LINES_RHO			1					// ���������� ���������� ����� �������, ��������������� ��������������� ���� (���������� �������� ��������������� ����������).
#define DOOR_THETA				CV_PI / 180			// ���������� �����, ������������� ��������������� ���� (�������� � ��������, ���������� �������� ������������� �����).
#define DOOR_LINES_THRESHOLD	150					// ��������� �������� ��� ������� ��������, ������������ ��������������� ����.
#define DOOR_LINES_MIN_length	100					// ����������� ������ �������, ������������ ������� ������� ������ �� ��������������� (������������� � �������������� ����).
#define DOOR_LINES_MAX_GAP		250					// ������������ ������ ����� ����� ������� ����� ����� (������������� � �������������� ����).

#define DOOR_LINES_HORIZONTAL_LIMIT	20				// ������� � ����� � ��������� [-DOOR_LINES_HORIZONTAL_LIMIT; DOOR_LINES_HORIZONTAL_LIMIT] ����������� �� ��������������.
#define DOOR_LINES_VERTICAL_LIMIT	70				// ������� � ����� � ��������� [-90; -DOOR_LINES_HORIZONTAL_LIMIT] && [DOOR_LINES_HORIZONTAL_LIMIT; 90] ����������� �� ������������.

#define DOOR_LINES_MIN_ANGLE_DELTA	5				// ������� � ����� ��������, ��� ������� ������� ����� ���������������, ��� ������� � ���������� �����.
#define DOOR_LINES_MIN_DISTANCE		5				// ���������� ����� ���������, ��� ������� ������� ����� ���������������, ��� ������� � ������� �����������.

#define DOOR_INTERSECTION_DISTANCE	5				// ���������� ����� �������������� � ������������ ��������, ��� ������ ������ ������� ����� ������� ���������������.

#define DOOR_MIN_WIDTH				400				// ����������� ������ �����.
#define DOOR_MAX_WIDTH				1500			// ������������ ������ �����.
#define DOOR_MIN_HEIGHT				400				// ����������� ������ �����.
#define DOOR_MAX_HEIGHT				1500			// ������������ ������ �����.

#define DOOR_MAX_FAULT				30				// ������������ ����������� ������� � ������ ������������ ������ �����.
#define DOOR_MIN_RATIO				1.0f			// ����������� ����������� ����������� ������ ����� � �� ������.
#define DOOR_MAX_RATIO				5.0f			// ������������ ����������� ����������� ������ ����� � �� ������.

#define DOOR_CONTOUR_COLOR			255, 0, 0		// ���� ������� ��� ��������� �����.

#define DOOR_IMAGE		"images/33.jpg"				// �����������, �� ������� ���������� ���������� ����� �����.

/*
���������� ���������� ���� ����� �������.
param[in] x1, y1, x2, y2 - ���������� �����.
*/
inline int getPointsDistance(int x1, int y1, int x2, int y2);

/*
������������ ���������� ����� ����������� ���� ������. 
param[in] A1, B1, C1 - ��������� ������ ������ (A1x + B1y + C1 = 0).
param[in] A2, B2, C2 - ��������� ������ ������ (A2x + B2y + C2 = 0).
param[out] px, py - ���������� ����� �����������.
*/
void calculateIntersectPoint(float A1, float B1, float C1, float A2, float B2, float C2, int &px, int &py);

/*
���������� true, ���� ����� (px, py) ����������� ������� (x1, y1) - (x2, y2).
���� ����� (px, py) �� ����������� �������, �������� �� ��������� �������������� � ��������� ��������� � ��� �����, ������������� �������.
param[in] x1, y1, x2, y2 - ���������� �������.
param[out] px, py - ���������� ��������������� �����.
*/
bool setPointInLine(int &px, int &py, int x1, int y1, int x2, int y2);

/*
���������� true, ���� ����� (x, y) ����� �� ������ Ax + By + C = 0.
param[in] x, y - ���������� ��������������� �����.
param[in] A, B, C - ��������� ������ (Ax + By + C = 0).
*/
bool isPointOnStraight(int x, int y, float A, float B, float C);

/*
���������� ���������� �� ����� �� �������.
param[in] x, y - ���������� ��������������� �����.
param[in] x1, y1, x2, y2 - ���������� �������.
param[in] A, B, C - ��������� ������ (Ax + By + C = 0), ���������� ����� �������.
param[out] px, py - ���������� �����, ��������� � ����� (x, y) � ������� �� �������.
*/
int getPointToLineDistance(int x, int y, int x1, int y1, int x2, int y2, float A1, float B1, float C1, int &px, int &py);

/*
���������� true, ���� ��� ������� ������������.
param[in] x1, y1, x2, y2 - ���������� ������� �������.
param[in] A1, B1, C1 - ��������� ������ (A1x + B1y + C1 = 0), ���������� ����� ������ �������.
param[in] x3, y3, x4, y4 - ���������� ������� �������.
param[in] A2, B2, C2 - ��������� ������ (A2x + B2y + C2 = 0), ���������� ����� ������ �������.
param[out] px, py - ���������� ����� �����������.
*/
bool isLinesIntersects(int x1, int y1, int x2, int y2, float A1, float B1, float C1, int x3, int y3, int x4, int y4, float A2, float B2, float C2, int &px, int &py);

/*
���������� ���������� ����� ���������.
param[in] x1, y1, x2, y2 - ���������� ������� �������.
param[in] A1, B1, C1 - ��������� ������ (A1x + B1y + C1 = 0), ���������� ����� ������ �������.
param[in] phi1 - ���� ������� �������.
param[in] x3, y3, x4, y4 - ���������� ������� �������.
param[in] A2, B2, C2 - ��������� ������ (A2x + B2y + C2 = 0), ���������� ����� ������ �������.
param[in] phi2 - ���� ������� �������.
param[out] px, py - ���� ������� ������������ - ���������� ����� �����������.
*/
int getLinesDistance(int x1, int y1, int x2, int y2, float A1, float B1, float C1, int phi1, int x3, int y3, int x4, int y4, float A2, float B2, float C2, int phi2, int &px, int &py);

/*
�������� ���������������, ���������� ������ � ���������� ������������ (�������� � ������ ��������� - ������������ ��������).
param[in] quadrosVect - ������ �����������������.
param[out] door - ���������������, ���������� ������ � ���������� ������������.
*/
void chooseDoor(CQuadro &door, vector<CQuadro> &quadrosVect);

/*
���� ����� ���� ����������� �������������� � ������������ �������� ����������������, ��������������� �������� ������������.
param[in] horNodesVect - ������ ���� ����������� �������������� � ������������ ��������.
param[out] quadrosVect - ������������ ����������������.
*/
void getQuadros(vector<CQuadro> &quadrosVect, vector<map<vector<CLine>::iterator, Point2i>> &horNodesVect);

/*
��� ���� ��� ������������ �������� �� ������� commonVertsVect ���������� ����������������, � ��������� ��, ������� �� ������������� �������� ������������.
������ ������� ������������ �������� getQuadros.
param[in] commonVertsVect - ������ ������������ �����.
param[out] quadrosVect - ������������ ����������������.
*/
void getQuadrosFromCommonVect(vector<CQuadro> &quadrosVect, vector<Vec4i> &commonVertsVect);

/*
���� ��� ����������� �������������� � ������������ ��������.
param[in] horNodesVect - ������ �������������� ��������.
param[in] vertLinesVect - ������ ������������ ��������.
param[out] horNodesVect - ��� ������������ ����������� �������������� � ������������ ��������.
*/
void getNodes(vector<map<vector<CLine>::iterator, Point2i>> &horNodesVect, vector<CLine> &horLinesVect, vector<CLine> &vertLinesVect);

/*
������������ ������� �������, ������� ������� ��� ������� ������� (������� ������� ������������ ����������� DOOR_LINES_MIN_ANGLE_DELTA � DOOR_LINES_MIN_DISTANCE).
param[in] it1 - ��������, ����������� �� ������ �������.
param[in] it2 - ��������, ����������� �� ������ �������.
param[out] linesVect - ������ ��������, ������� ������ ��� ������� � � ������� ���������� �������� ���������.
*/
void getMiddleLine(vector<CLine> &linesVect, vector<CLine>::iterator it1, vector<CLine>::iterator it2);

/*
��������� ������ �������� �� ������� ������� �������� � �������� �� �� ������� �������, ����������� ��� ������ ������� getMiddleLine.
param[out] linesVect - ������ ��������.
*/
void linesSample(vector<CLine> &linesVect);

/*
������� ���� ������� �������� � ������� �������� � �������� �� �� ������� �������.
���������� true, ���� ����� ���� ���� ������� � ��������.
������ ������� ������������ �������� linesSample.
param[out] linesVect - ������ ��������.
*/
bool findSimilarLines(vector<CLine> &linesVect);