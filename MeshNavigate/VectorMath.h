/*
**�ļ����ƣ�VectorMath.h
**����������
**�ļ�˵����
**��    �ߣ�����
**����ʱ�䣺2012-02-21
**��    �ģ�
*/

#pragma once

struct Vector;

extern void VectorCrossProduct(Vector* result, const Vector* a, const Vector* b);

extern float VectorDotProduct(const Vector* a, const Vector* b);
