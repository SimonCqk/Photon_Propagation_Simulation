#pragma once
#ifndef R_MCML_H
#define R_MCML_H

#include<iostream>
#include<functional>
#include<memory>
#include<string>
#include<vector>
#include<fstream>
using std::vector;
using std::function;
using std::string;
using std::shared_ptr;
using std::weak_ptr;
using std::make_shared;
using std::ifstream;

function<int(int)>SIGN = [](int x) {return (x >= 0) ? 1 : -1; };

const double PI = 3.1415926;
const double WEIGHT = 1E-4;/* Critical weight for roulette. */
const double CHANCE = 0.1; /* Chance of roulette survival. */

						   // Class used to describe a photon packet.[光子]

struct PhotonStruct
{
	double x, y, z; /* Cartesian coordinates.[cm] [笛卡尔坐标] [cm]为单位*/
	double dcos_x, dcos_y, dcos_z;/* directional cosines of a photon. [光子的方向余弦] */
	double weight; /* weight. */
	bool dead; /* true if photon is terminated. */
	size_t layer; /* index to layer where the photon  packet resides. */
	double cur_step; /* current step size. [cm]. [当前步长] */
	double step_left; /* step size left. dimensionless [-]. [剩余步长] */
};

class PhotonClass {
public:
	PhotonClass() :
		photon(make_shared<PhotonStruct>()) {}
	PhotonClass(const PhotonStruct& item) :
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

/****
* Structure used to describe the geometry and optical [几何光学] properties of a layer.
* z0 and z1 are the z coordinates for the upper boundary  and lower boundary respectively.
* cos_crit0 and cos_crit1 are the cosines of the critical angle of total internal reflection for the
* upper boundary and lower boundary respectively.
* They are set to zero if no total internal reflection exists.
* They are used for computation speed.
****/

struct LayerStruct
{
	double z0, z1; /* z coordinates of a layer. [cm] [层的坐标] */
	double rfct_index; /* refractive index of a layer. */
	double abs_coef; /* absorption coefficient. [1/cm] [吸收系数]*/
	double scat_coef; /* scattering coefficient. [1/cm] [散射系数]*/
	double anisotropy; /* anisotropy. [各向异性常数] */
	double cos_crit_up;  /* [全内反射临界角上边界的余弦值] */
	double cos_crit_down;	/*[全内反射临界角下边界的余弦值]*/
};

class LayerClass {
	friend double Rspecular(vector<LayerClass>& LayerVec);
	friend void ReadAmbient(ifstream& File_Ptr, LayerClass& Layer_Ptr, string& side);
	friend bool ReadOneLayer(ifstream& File_Ptr, LayerClass& Layer_Ptr, double& Z_Ptr);

	friend class PhotonClass;

public:
	LayerClass() :
		layer(make_shared<LayerStruct>()) {}
	LayerClass(const LayerStruct& item) :
		layer(make_shared<LayerStruct>(item)) {}
	//forbid some constructors
	LayerClass(const LayerClass&) = delete;
	LayerClass& operator=(const LayerClass&) = delete;
	~LayerClass() = default;


private:
	shared_ptr<LayerStruct> layer;
};

/****
* Input parameters for each independent run.
*
* z and r are for the cylindrical coordinate system. [cm] [圆柱坐标系统]
* a is for the angle alpha between the photon exiting direction and the surface normal. [radian]
*
* The grid line separations in z, r, and alpha directions are dz, dr, and da respectively. The numbers
* of grid lines in z, r, and alpha directions are nz, nr, and na respectively.
*
* The member layerspecs will point to an array of structures which store parameters of each layer.
* This array has (number_layers + 2) elements. One element is for a layer.
* The layers 0 and (num_layers + 1) are for top ambient medium and the bottom ambient medium respectively.
*/

struct InputStruct
{
	string out_fname; /* output file name. */
	char out_fformat; /* output file format. */
					  /* 'A' for ASCII, */
					  /* 'B' for binary. */
	long int num_photons; /* to be traced. */
	double Wth; /* play roulette if photon */
				/* weight < Wth.*/

	double dz; /* z grid separation.[cm] */
	double dr; /* r grid separation.[cm] */
	double da; /* alpha grid separation. */
			   /* [radian] */
	size_t nz; /* array range 0..nz-1. */
	size_t nr; /* array range 0..nr-1. */
	size_t na; /* array range 0..na-1. */

	size_t num_layers; /* number of layers. */
	vector<LayerClass> layerspecs;   /* layer parameters. */
};

class InputClass {
	friend class PhotonClass;

	friend void ReadFnameFormat(ifstream& File_Ptr, InputClass& In_Ptr);
	friend void ReadNumPhotons(ifstream& File_Ptr, InputClass& In_Ptr);
	friend void ReadDzDr(ifstream& File_Ptr, InputClass& In_Ptr);
	friend void ReadNzNrNa(ifstream& File_Ptr, InputClass& In_Ptr);
	friend void ReadNumLayers(ifstream& File_Ptr, InputClass& In_Ptr);
public:
	InputClass() :
		input(make_shared<InputStruct>()) {}
	InputClass(const InputStruct& item) :
		input(make_shared<InputStruct>(item)) {}
	//forbid some constructors
	InputClass(const InputClass&) = delete;
	InputClass& operator=(const InputClass&) = delete;
	~InputClass() = default;
private:
	shared_ptr<InputStruct> input;
};



/*
* Classes for scoring physical quantities.
* z and r represent z and r coordinates of the [圆柱坐标系的坐标] cylindrical coordinate system. [cm]
* a is the angle alpha between the photon exiting direction and the normal to the surfaces. [radian]
* See comments of the InputStruct.
* See manual for the physcial quantities.
*/

struct OutStruct
{
	double Rsp; /* specular reflectance. [-]  [镜面反射率]*/
	vector<vector<double>> Rd_ra; /* 2D distribution of diffuse [二维弥散分布] */
								  /* reflectance. [1/(cm2 sr)] */
	vector<double> Rd_r; /* 1D radial distribution of diffuse [一维径向弥散分布]*/
						 /* reflectance. [1/cm2] */
	vector<double> Rd_a; /* 1D angular distribution of diffuse [一维角度弥散分布] */
						 /* reflectance. [1/sr] */
	double Rd; /* total diffuse reflectance. [-] [总漫反射] */

	vector<vector<double>> A_rz; /* 2D probability density in turbid [二维浑浊概率密度] */
								 /* media over r & z. [1/cm3] */
	vector<double> A_z; /* 1D probability density over z. */
						/* [1/cm] */
	vector<double> A_l; /* each layer's absorption */
						/* probability. [-] [每层的吸收概率]*/
	double A; /* total absorption probability. [-] [总吸收概率] */

	vector<vector<double>> Tt_ra; /* 2D distribution of total */
								  /* transmittance. [1/(cm2 sr)] [二维总透光率分布] */
	vector<double> Tt_r; /* 1D radial distribution of */
						 /* transmittance. [1/cm2] [一维径向透光率分布]*/
	vector<double> Tt_a; /* 1D angular distribution of */
						 /* transmittance. [1/sr] [一维角透光率分布]*/
	double Tt; /* total transmittance. [-] [总透光率] */
};

class OutClass
{
	friend class PhotonClass;

public:
	OutClass() :
		out(make_shared<OutStruct>()) {}
	OutClass(const OutStruct& item) :
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
