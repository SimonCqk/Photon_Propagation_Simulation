#pragma once
#ifndef MCML_IMPL_H
#define MCML_IMPL_H

using std::string;
using std::vector;

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



/*
* Classes for scoring physical quantities.
* z and r represent z and r coordinates of the [圆柱坐标系的坐标] cylindrical coordinate system. [cm]
* a is the angle alpha between the photon exiting direction and the normal to the surfaces. [radian]
* See comments of the InputStruct.
* See manual for the physcial quantities.
*/

struct OutStruct
{
    double spec_reflect; /* specular reflectance. [-]  [镜面反射率]*/
    vector<vector<double>> diff_reflect_2d; /* 2D distribution of diffuse [二维弥散分布] */
                                  /* reflectance. [1/(cm2 sr)] */
    vector<double> diff_reflect_rdl; /* 1D radial distribution of diffuse [一维径向弥散分布]*/
                         /* reflectance. [1/cm2] */
    vector<double> diff_reflect_agl; /* 1D angular distribution of diffuse [一维角度弥散分布] */
                         /* reflectance. [1/sr] */
    double diff_reflect; /* total diffuse reflectance. [-] [总漫反射] */

    vector<vector<double>> abs_prob_rz; /* 2D probability density in turbid [二维浑浊概率密度] */
                                 /* media over r & z. [1/cm3] */
    vector<double> abs_prob_z; /* 1D probability density over z. */
                        /* [1/cm] */
    vector<double> abs_prob_layer; /* each layer's absorption */
                        /* probability. [-] [每层的吸收概率]*/
    double abs_prob; /* total absorption probability. [-] [总吸收概率] */

    vector<vector<double>> total_trans_2d; /* 2D distribution of total */
                                  /* transmittance. [1/(cm2 sr)] [二维总透光率分布] */
    vector<double> total_trans_rdl; /* 1D radial distribution of */
                         /* transmittance. [1/cm2] [一维径向透光率分布]*/
    vector<double> total_trans_agl; /* 1D angular distribution of */
                         /* transmittance. [1/sr] [一维角透光率分布]*/
    double total_trans; /* total transmittance. [-] [总透光率] */
};
#endif MCML_IMPL_H
