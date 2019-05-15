#pragma once
#include "Line.h"

// �����, ����������� ����������������.
class CQuadro
{
	CLine _lines[4]; // �������.
public:
	/*
	�����������.. ���������������� ��������� ������� ������.
	*/
	CQuadro() : _lines{ CLine(), CLine(), CLine(), CLine() } {}

	/*
	�����������.
	param[in] line1, line2 - ������������ ������� ����������������.
	*/
	CQuadro(CLine line1, CLine line2);

	/*
	����������.
	*/
	~CQuadro() {}

	/*
	���������� �������.
	param[in] num - ����� ������� � ������� _lines.
	*/
	const CLine &getLine(unsigned int num);

	/*
	���������� �������� ����������������.
	*/
	int getPerimetr();
};
