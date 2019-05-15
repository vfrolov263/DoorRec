#pragma once
#include <opencv2/opencv.hpp>

#define PI 3.14159265        

using cv::Point2i;
using cv::Vec4i;

extern int getPointsDistance(int, int, int, int);

// �����, ����������� �������.
class CLine
{
	Point2i _point1, _point2;	// ��������� � �������� ����� �������.
	bool _bHorisontal;	// ���� ����������������.
	int _ilength;	// ������ �������.
	int _iAngle; // ���� �������.
	float _fA, _fB, _fC; // Ax + By + C = 0.
public:
	/*
	�����������. ���������������� ��������� ������� ������.
	*/
	CLine() : CLine(0, 0, 0, 0, 0) {}

	/*
	�����������.
	param[in] x1, y1, x2, y2 - ���������� �������.
	*/
	CLine(int x1, int y1, int x2, int y2) : CLine(x1, y1, x2, y2, getPointsDistance(x1, y1, x2, y2)) {}

	/*
	�����������.
	param[in] x1, y1, x2, y2 - ���������� �������.
	param[in] length - ������ �������.
	*/
	CLine(int x1, int y1, int x2, int y2, int length);

	/*
	����������.
	*/
	~CLine() {}

	/*
	���������� ���� �������.
	*/
	inline const int getAngle() const { return _iAngle; }

	/*
	���������� ������ �������.
	*/
	inline const int getLength() const { return _ilength; }

	/*
	���������� ���� ����������������: true - ������� ������ � ������ "��������������" ��������.
	*/
	inline const bool isHorizontal() const { return _bHorisontal; }

	/*
	���������� ���� ��������������: true - ������� ������ � ������ "������������" ��������.
	*/
	inline const bool isVertical() const { return !_bHorisontal; }

	/*
	���������� ����� A �������.
	*/
	inline Point2i getPointA() const { return _point1; }

	/*
	���������� ����� B �������.
	*/
	inline Point2i getPointB() const { return _point2; }

	/*
	���������� �������� A �� ��������� ������ Ax + By + C = 0.
	*/
	inline const float getA() const { return _fA; }

	/*
	���������� �������� B �� ��������� ������ Ax + By + C = 0.
	*/
	inline const float getB() const { return _fB; }

	/*
	���������� �������� C �� ��������� ������ Ax + By + C = 0.
	*/
	inline const float getC() const { return _fC; }

	/*
	������������ ��������� ������� �� �����������.
	param[in] x1, y1, x2, y2- ���������� �������.
	*/
	void setLine(int x1, int y1, int x2, int y2);
};