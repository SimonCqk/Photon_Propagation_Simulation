#pragma once
#ifndef MCML_FUN_FWD_H
#define MCML_FUN_FWD_H

#include"mcml/mcml_fwd.h"
#include<QVector>
#include<vector>
#include<QDebug>
using std::vector;

double RandomNum();
double Rspecular(const QVector<LayerClass>& LayerVec);
double SpinTheta(double g);
double RFresnel(double n1, double n2, double ca1, double& ca2_Ptr);

void InitOutputData(const InputClass& In_Parm,OutClass& Out_Ptr);
void CriticalAngle(size_t Layer_num,QVector<LayerClass>& layerspecs);
void Sum2D_DiffReflect(const InputClass& In,OutClass& Out);
size_t IndexToLayer(size_t Iz, const InputClass& In_Parm);
void Sum2D_AbsProb(const InputClass& In_Parm, OutClass& Out_Ptr);
void Sum2D_TotalTrans(const InputClass& In_Parm, OutClass& Out_Ptr);
void Scale_DiffReflect_TotalTrans(const InputClass& In_Parm, OutClass& Out_Ptr);
void Scale_AbsProb(const InputClass& In_Parm, OutClass& Out_Ptr);
void SumScaleResult(const InputClass& In_Parm, OutClass& Out_Ptr);

void DoOneRun(InputClass* In_Ptr,OutClass& out_parm);  // main running control function.

template<class Type>
inline void AllocVector(QVector<Type>& vec,size_t row)
{
    vec.resize(row+1);
    vec.fill(0.0);
}


template<class Type>
inline void AllocMatrix(QVector<QVector<Type>>& mat,size_t row,size_t col)
{
    mat.resize(row+1);
    for(size_t i=0;i<=col;++i)
    {
        mat[i].resize(col+1);
        mat[i].fill(0.0);
    }
}


#endif
