#pragma once
#include "EasyAutoControlExDef.h"
class __declspec(dllexport) CPlanFit
{
public:
	CPlanFit(void);
	~CPlanFit(void);
private:
	double m_dlA[3];
	void SolveEquations(double **dlValue);
public:
	double GetPlanZ(Point3D point3D);
	double FitLeastPlan(vector<Point3D>&vPoint);
};

