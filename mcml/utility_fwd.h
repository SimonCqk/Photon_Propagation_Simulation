#pragma once
#ifndef MCML_FUN_FWD_H
#define MCML_FUN_FWD_H

#include<vector>
using std::vector;

double RandomNum();
double Rspecular(vector<LayerClass>& LayerVec);
double SpinTheta(double g);
double RFresnel(double n1, double n2, double ca1, double& ca2_Ptr);

void CriticalAngle(size_t Layer_num,vector<LayerClass>& layerspecs);


#endif