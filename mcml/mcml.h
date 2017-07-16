#pragma once
#ifndef R_MCML_H
#define R_MCML_H

#include<iostream>
#include<functional>
#include<memory>
#include"mcml_impl.h"
#include"utility_fwd.h"
using std::vector;
using std::function;
using std::string;
using std::shared_ptr;
using std::weak_ptr;
using std::make_shared;

function<int(int)>SIGN = [](int x) {return (x >= 0) ? 1 : -1; };

const double PI = 3.1415926;
const double WEIGHT = 1E-4;/* Critical weight for roulette. */
const double CHANCE = 0.1; /* Chance of roulette survival. */

class OutClass;
class InputClass;

// Class used to describe a photon packet.[¹â×Ó]

class PhotonClass {
public:
    PhotonClass() :
        photon(make_shared<PhotonStruct>(PhotonStruct())) {}
    explicit PhotonClass(const PhotonStruct& item) :
        photon(make_shared<PhotonStruct>(item)) {}
    //forbid some constructors
    PhotonClass(const PhotonClass&) = delete;
    PhotonClass& operator=(const PhotonClass&) = delete;
    ~PhotonClass() = default;

    void launch(double Rspecular, vector<LayerClass>& LayerVec); //Initialize a photon packet.
    void spin(double g);  //Choose a new direction for photon propagation
    void hop();  //Move the photon s away in the current layer of medium.
    void stepSizeInGlass(const InputClass& In);  // return the photon step size in glass
    void stepSizeInTissue(const InputClass& In);  //Pick a step size for a photon packet when it is in tissue.
    bool hitBoundary(const InputClass& In);  //Check if the step will hit the boundary.
    void drop(const InputClass& In, OutClass& Out);  //Drop photon(assumed not dead) weight inside the tissue (not glass).
    void roulette();  //tries to survive a roulette.
    void recordWeightFirstLayer(double Refl, const InputClass& In, OutClass& Out); //Record the photon weight exiting the first layer(dcos_z<0)
    void recordWeightLastLayer(double Refl, const InputClass& In, OutClass& Out); //Record the photon weight exiting the first layer(dcos_z>0)
    void crossUpOrNot(const InputClass& In, OutClass& Out);  //Decide whether the photon will be transmitted or reflected on the upper boundary(dcos_z<0) of the current layer.
    void crossDownOrNot(const InputClass& In, OutClass& Out);  //Decide whether the photon will be transmitted or reflected on the bottom boundary(dcos_z>0) of the current layer.
    void crossOrNot(const InputClass& In, OutClass& Out);
    void hopInGlass(const InputClass& In, OutClass& Out);  //Move the photon packet in glass layer.
    void hopDropSpinInTissue(const InputClass& In, OutClass& Out);  //Set a step size, move the photon, drop some weight, choose a new photon direction for propagation.
    void hopDropSpin(const InputClass& In, OutClass& Out);

private:
    shared_ptr<PhotonStruct> photon;
};

class LayerClass {
    friend double Rspecular(vector<LayerClass>& LayerVec);
    friend	void CriticalAngle(size_t Layer_num,vector<LayerClass>& layerspecs);
    friend size_t IndexToLayer(size_t Iz, const InputClass& In_Parm);
    friend class PhotonClass;

public:
    LayerClass() :
        layer(make_shared<LayerStruct>(LayerStruct())) {}
    explicit LayerClass(const LayerStruct& item) :
        layer(make_shared<LayerStruct>(item)) {}
    //forbid some constructors
    LayerClass(const LayerClass&) = delete;
    LayerClass& operator=(const LayerClass&) = delete;
    ~LayerClass() = default;


private:
    shared_ptr<LayerStruct> layer;
};


class InputClass {
    friend class PhotonClass;

    friend void Sum2D_DiffReflect(const InputClass& In, OutClass& Out);
    friend size_t IndexToLayer(size_t Iz, const InputClass& In_Parm);
    friend void Sum2D_AbsProb(const InputClass& In_Parm, OutClass& Out_Ptr);
    friend void Sum2D_TotalTrans(const InputClass& In_Parm, OutClass& Out_Ptr);
    friend void Scale_DiffReflect_TotalTrans(const InputClass& In_Parm, OutClass& Out_Ptr);
    friend void Scale_AbsProb(const InputClass& In_Parm, OutClass& Out_Ptr);
    friend void SumScaleResult(const InputClass& In_Parm, OutClass& Out_Ptr);

public:
    InputClass() :
        input(make_shared<InputStruct>(InputStruct())) {}
    explicit InputClass(const InputStruct& item) :
        input(make_shared<InputStruct>(item)) {}
    //forbid some constructors
    InputClass(const InputClass&) = delete;
    InputClass& operator=(const InputClass&) = delete;
    ~InputClass() = default;
private:
    shared_ptr<InputStruct> input;
};



class OutClass
{
    friend class PhotonClass;

    friend void Sum2D_DiffReflect(const InputClass& In, OutClass& Out);
    friend void Sum2D_AbsProb(const InputClass& In_Parm, OutClass& Out_Ptr);
    friend void Sum2D_TotalTrans(const InputClass& In_Parm, OutClass& Out_Ptr);
    friend void Scale_DiffReflect_TotalTrans(const InputClass& In_Parm, OutClass& Out_Ptr);
    friend void Scale_AbsProb(const InputClass& In_Parm, OutClass& Out_Ptr);
    friend void SumScaleResult(const InputClass& In_Parm, OutClass& Out_Ptr);

public:
    OutClass() :
        out(make_shared<OutStruct>(OutStruct())) {}
    explicit OutClass(const OutStruct& item) :
        out(make_shared<OutStruct>(item)) {}
    //forbid some constructors
    OutClass(const OutClass&) = delete;
    OutClass& operator=(const OutClass&) = delete;
    ~OutClass() = default;

private:
    shared_ptr<OutStruct> out;
};

double RandomNum();
double Rspecular(vector<LayerClass>& LayerVec);
double SpinTheta(double g);
double RFresnel(double n1, double n2, double ca1, double& ca2_Ptr);


#endif // !R_MCML_H
