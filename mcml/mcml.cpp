//The implementation of Monte Carlo Model

#include"r_mcml.h"
#include<random>


#define  PARTIALREFLECTION  0
/* 1=split photon, 0=statistical reflection. */

const int STANDARDTEST = 0;
/* testing program using fixed rnd seed. */

const double COSZERO = (1.0 - 1.0E-12);
/* cosine of about 1e-6 rad. */

const double  COS90D = 1.0E-6;
/* cosine of about 1.57 - 1e-6 rad. */

//use Mersenne twister to generate a random number
double RandomNum() {
	//set `static` to avoid generating some results
	static std::random_device Rand;
	static std::mt19937_64 RanEngine(Rand());
	static std::uniform_real_distribution<double> Distribution(0.0, 1.0);
	return Distribution(RanEngine);
}

/*
 * Compute the specular reflection.
 *
 * If the first layer is a turbid medium, use the Fresnel
 * reflection from the boundary of the first layer as the
 * specular reflectance.
 *
 * If the first layer is glass, multiple reflections in
 * the first layer is considered to get the specular
 * reflectance.
 *
 * The subroutine assumes the Layerspecs array is correctly
 * initialized.
*/
double Rspecular(vector<LayerClass>& LayerVec)
{
	/* direct reflections from the 1st and 2nd layers. */
	double temp = (LayerVec[0].rfct_index - LayerVec[1].rfct_index)
		/ (LayerVec[0].rfct_index + LayerVec[1].rfct_index);
	double r1 = temp*temp;
	double r2;
	if ((LayerVec[1].abs_coef == 0.0) && (LayerVec[1].scat_coef == 0.0)) { /* glass layer. */
		temp = (LayerVec[1].rfct_index - LayerVec[2].rfct_index)
			/ (LayerVec[1].rfct_index + LayerVec[2].rfct_index);
		r2 = temp*temp;
		r1 = r1 + (1 - r1)*(1 - r1)*r2 / (1 - r1*r2);
	}
	return r1;
}

/*
 * Choose (sample) a new theta angle for photon propagation
 * according to the anisotropy.
 *
 * If anisotropy g is 0, then
 * cos(theta) = 2*rand-1.
 * otherwise
 * sample according to the Henyey-Greenstein function.
 *
 * Returns the cosine of the polar deflection angle theta.
*/
double SpinTheta(double g)
{
	double cost;
	if (g == 0.0)
		cost = 2 * RandomNum() - 1;
	else {
		double temp = (1 - g*g) / (1 - g + 2 * g*RandomNum());
		cost = (1 + g*g - temp*temp) / (2 * g);
	}
	return cost;
}

/*
 * Compute the Fresnel reflectance.
 *
 * Make sure that the cosine of the incident angle a1
 * is positive, and the case when the angle is greater
 * than the critical angle is ruled out.
 *
 * Avoid trigonometric function operations as much as
 * possible, because they are computation-intensive.
 */
double RFresnel(double n1, /* incident refractive index.*/
	double n2, /* transmit refractive index.*/
	double ca1, /* cosine of the incident */
				/* angle. 0<a1<90 degrees. */
	double& ca2_Ptr) /* pointer to the */
					  /* cosine of the transmission */
					  /* angle. a2>0. */
{
	double r;
	if (n1 == n2) { /** matched boundary. **/
		ca2_Ptr = ca1;
		r = 0.0;
	}
	else if (ca1 > COSZERO) { /** normal incident. **/
		ca2_Ptr = ca1;
		r = (n2 - n1) / (n2 + n1);
		r *= r;

	}
	else if (ca1 < COS90D) { /** very slant. **/
		ca2_Ptr = 0.0;
		r = 1.0;

	}
	else { /** general. **/
		double sa1, sa2;
		/* sine of the incident and transmission angles. */
		double ca2;
		sa1 = sqrt(1 - ca1*ca1);
		sa2 = n1*sa1 / n2;
		if (sa2 >= 1.0) {
			/* double check for total internal reflection. */
			ca2_Ptr = 0.0;
			r = 1.0;

		}
		else {
			double cap, cam; /* cosines of the sum ap or */
							 /* difference am of the two */
							 /* angles. ap = a1+a2 */
							 /* am = a1 - a2. */
			double sap, sam; /* sines. */
			ca2_Ptr = ca2 = sqrt(1 - sa2*sa2);
			cap = ca1*ca2 - sa1*sa2; /* c+ = cc - ss. */
			cam = ca1*ca2 + sa1*sa2; /* c- = cc + ss. */
			sap = sa1*ca2 + ca1*sa2; /* s+ = sc + cs. */
			sam = sa1*ca2 - ca1*sa2; /* s- = sc - cs. */
			r = 0.5*sam*sam*(cam*cam + cap*cap) / (sap*sap*cam*cam);
			/* rearranged for speed. */

		}

	}
	return r;
}

//The specific declaration of mem-function in PhotonClass
void PhotonClass::launch(double Rspecular, vector<LayerClass>& LayerVec)
{
	weight = 1.0 - Rspecular;
	dead = 0;
	layer = 1;
	cur_step = step_left = 0;
	x =	y = z = 0.0;
	dcos_x = dcos_y = 0.0;
	dcos_z = 1.0;
	if ((LayerVec[1].abs_coef == 0.0) && (LayerVec[1].scat_coef == 0.0)) { /* glass layer. */
		layer = 2;
		z = LayerVec[2].z0;
	}
}

/*
 * Choose a new direction for photon propagation by
 * sampling the polar deflection angle theta and the
 * azimuthal angle psi.
 *
 * Note:
 * theta: 0 - pi so sin(theta) is always positive
 * feel free to use sqrt() for cos(theta).
 *
 * psi: 0 - 2pi
 * for 0-pi sin(psi) is +
 * for pi-2pi sin(psi) is -
*/

void PhotonClass::spin(double g) {
	double cost = SpinTheta(g);
	double sint = sqrt(1.0 - cost*cost); /* cosine and sine of the */
					   /* polar deflection angle theta. */
	double psi = 2.0*PI*RandomNum(); /* spin psi 0-2pi. */
	double cosp = cos(psi), sinp; /* cosine and sine of the */
					   /* azimuthal angle psi. */
	if (psi < PI)
		sinp = sqrt(1.0 - cosp*cosp);
	/* sqrt() is faster than sin(). */
	else
		sinp = -sqrt(1.0 - cosp*cosp);
	if (fabs(dcos_z) > COSZERO) { /* normal incident. */
		dcos_x = sint*cosp;
		dcos_y = sint*sinp;
		dcos_z = cost*SIGN(dcos_z);
		/* SIGN() is faster than division. */
	}
	else { /* regular incident. */
		double temp = sqrt(1.0 - dcos_z*dcos_z);
		dcos_x = sint*(dcos_x*dcos_z*cosp - dcos_y*sinp)
			/ temp + dcos_x*cost;
		dcos_y = sint*(dcos_y*dcos_z*cosp + dcos_x*sinp)
			/ temp + dcos_y*cost;
		dcos_z = -sint*cosp*temp + dcos_z*cost;
	}
}

/*
	Move the photon s away in the current layer of medium.
*/
void PhotonClass::hop() {
	double s = cur_step;
	x += s*dcos_x;
	y += s*dcos_y;
	z += s*dcos_z;
}

/*
 * If dcos_z != 0, return the photon step size in glass,
 * Otherwise, return 0.
 *
 * The step size is the distance between the current
 * position and the boundary in the photon direction.
 *
 * Make sure dcos_z !=0 before calling this function.

*/
void PhotonClass::stepSizeInGlass(const InputClass& In) {
	double dl_b; /* step size to boundary. */
	short layer = this->layer;
	double uz = this->dcos_z;

	/* Stepsize to the boundary. */
	if (uz > 0.0)
		dl_b = (In.layerspecs[layer].z1 - this->z)
		/ uz;
	else if (uz < 0.0)
		dl_b = (In.layerspecs[layer].z0 - this->z)
		/ uz;
	else
		dl_b = 0.0;
	cur_step = dl_b;
}

/*
 * Pick a step size for a photon packet when it is in
 * tissue.
 * If the member sleft is zero, make a new step size
 * with: -log(rnd)/(mua+mus).
 * Otherwise, pick up the leftover in sleft.
 *
 * Layer is the index to layer.
 * In_Ptr is the input parameters.
 */

void PhotonClass::stepSizeInTissue(const InputClass& In)
{
	short layer = this->layer;
	double mua = In.layerspecs[layer].abs_coef;
	double mus = In.layerspecs[layer].scat_coef;
	if (step_left == 0.0) { /* make a new step. */
		double rnd;
		do rnd = RandomNum();
		while (rnd <= 0.0); /* avoid zero. */
		cur_step = -log(rnd) / (mua + mus);

	}
	else { /* take the leftover. */
		cur_step = step_left / (mua + mus);
		step_left = 0.0;
	}
}

/*
 * Check if the step will hit the boundary.
 * Return 1 if hit boundary.
 * Return 0 otherwise.
 *
 * If the projected step hits the boundary, the members
 * s and sleft of Photon_Ptr are updated.
*/
bool PhotonClass::hitBoundary(const InputClass& In)
{
	double dl_b; /* length to boundary. */
	short layer = this->layer;
	double uz = this->dcos_z;
	bool hit;
	/* Distance to the boundary. */
	if (uz > 0.0)

		dl_b = (In.layerspecs[layer].z1 - this->z) / uz; /* dl_b>0. */
	else if (uz < 0.0)
		dl_b = (In.layerspecs[layer].z0 - this->z) / uz; /* dl_b>0. */

	if (uz != 0.0 && this->cur_step > dl_b) {
		/* not horizontal & crossing. */
		double mut = In.layerspecs[layer].abs_coef
			+ In.layerspecs[layer].scat_coef;
		this->step_left = (this->cur_step - dl_b)*mut;
		this->cur_step = dl_b;
		hit = true;
	}
	else
		hit = false;
	return hit;
}


/*
 * Drop photon weight inside the tissue (not glass).
 *
 * The photon is assumed not dead.
 *
 * The weight drop is dw = w*mua/(mua+mus).
 *
 * The dropped weight is assigned to the absorption array
 * elements.
*/
void PhotonClass::drop(const InputClass& In , OutClass& Out)
{
	double dwa; /* absorbed weight.*/
	double x = this->x;
	double y = this->y;
	short iz, ir; /* index to z & r. */
	short layer = this->layer;
	double mua, mus;

	/* compute array indices. */
	iz = static_cast<short>(this->z / In.dz);
	if (iz > In.nz - 1) iz = In.nz - 1;

	ir = static_cast<short>(sqrt(x*x + y*y) / In.dr);
	if (ir > In.nr - 1) ir = In.nr - 1;

	/* update photon weight. */
	mua = In.layerspecs[layer].abs_coef;
	mus = In.layerspecs[layer].scat_coef;
	dwa = this->weight * mua / (mua + mus);
	this->weight -= dwa;
	/* assign dwa to the absorption array element. */
	Out.A_rz[ir][iz] += dwa;
}

/*
 * The photon weight is small, and the photon packet tries
 * to survive a roulette.
 */
void PhotonClass::roulette()
{
	if (this->weight == 0.0)
		this->dead = true;

	else if (RandomNum() < CHANCE) /* survived the roulette.*/
		this->weight /= CHANCE;
	else
		this->dead = true;
}

/*
 * Record the photon weight exiting the first layer(uz<0),
 * no matter whether the layer is glass or not, to the
 * reflection array.
 *
 * Update the photon weight as well.
 */
void PhotonClass::recordWeightFirstLayer(double Refl, /* reflectance. */
							   const InputClass& In,
							   OutClass& Out)
{
	double x = this->x;
	double y = this->y;
	 /* index to r & angle. */
	short ia = static_cast<short>(acos(-this->dcos_z) / In.da);
	short ir = static_cast<short>(sqrt(x*x + y*y) / In.dr);
	if (ir > In.nr - 1) ir = In.nr - 1;

	if (ia > In.na - 1) ia = In.na - 1;
	/* assign photon to the reflection array element. */
	Out.Rd_ra[ir][ia] += this->weight*(1.0 - Refl);

	this->weight *= Refl;
}

/*
 * Record the photon weight exiting the last layer(uz>0),
 * no matter whether the layer is glass or not, to the
 * transmittance array.
 *
 * Update the photon weight as well.
 */
void PhotonClass::recordWeightLastLayer(double Refl, const InputClass& In, OutClass& Out)
{
	double x = this->x;
	double y = this->y;
	short ir, ia; /* index to r & angle. */
	ir = static_cast<short>(sqrt(x*x + y*y) / In.dr);
	if (ir > In.nr - 1) ir = In.nr - 1;
	ia = static_cast<short>(acos(this->dcos_z) / In.da);
	if (ia > In.na - 1) ia = In.na - 1;
	/* assign photon to the transmittance array element. */
	Out.Tt_ra[ir][ia] += this->weight*(1.0 - Refl);
	this->weight *= Refl;
}

/*
 * Decide whether the photon will be transmitted or
 * reflected on the upper boundary (dcos_z<0) of the current
 * layer.
 *
 * If "layer" is the first layer, the photon packet will
 * be partially transmitted and partially reflected if
 * PARTIALREFLECTION is set to 1,
 * or the photon packet will be either transmitted or
 * reflected determined statistically if PARTIALREFLECTION
 * is set to 0.
 *
 * Record the transmitted photon weight as reflection.
 *
 * If the "layer" is not the first layer and the photon
 * packet is transmitted, move the photon to "layer-1".
 *
 * Update the photon parmameters.
 */
void PhotonClass::crossUpOrNot(const InputClass& In,OutClass& Out)
{
	double uz = this->dcos_z; /* z directional cosine. */
	double uz1; /* cosines of transmission alpha. always */
				/* positive. */
	double r = 0.0; /* reflectance */
	short layer = this->layer;
	double ni = In.layerspecs[layer].rfct_index;
	double nt = In.layerspecs[layer - 1].rfct_index;

	/* Get r. */
	if (-uz <= In.layerspecs[layer].cos_crit_up)
		r = 1.0; /* total internal reflection. */
	else r = RFresnel(ni, nt, -uz, uz1);

#if PARTIALREFLECTION
	if (layer == 1 && r < 1.0) { /* partially transmitted. */
		this->dcos_z = -uz1; /* transmitted photon. */
		recordWeightFirstLayer(r, In, Out);
		this->uz = -uz; /* reflected photon. */

	}
	else if (RandomNum() > r) {/* transmitted to layer-1. */
		this->layer--;
		this->dcos_x *= ni / nt;
		this->dcos_y *= ni / nt;
		this->dcos_z = -uz1;

	}
	else /* reflected. */
		this->dcos_z = -uz;
#else
	if (RandomNum() > r) { /* transmitted to layer-1. */
		if (layer == 1) {
			this->dcos_z = -uz1;
			recordWeightFirstLayer(0.0, In,Out);
			this->dead = true;

		}
		else {
			this->layer--;
			this->dcos_x *= ni / nt;
			this->dcos_y *= ni / nt;
			this->dcos_z = -uz1;
		}

	}
	else /* reflected. */
		this->dcos_z = -uz;
#endif
}


/*
 * Decide whether the photon will be transmitted or be
 * reflected on the bottom boundary (dcos_z>0) of the current
 * layer.
 *
 * If the photon is transmitted, move the photon to
 * "layer+1". If "layer" is the last layer, record the
 * transmitted weight as transmittance. See comments for
 * CrossUpOrNot.
 *
 * Update the photon parmameters.
 */
void PhotonClass::crossDownOrNot(const InputClass& In, OutClass& Out)
{
	double uz = this->dcos_z; /* z directional cosine. */
	double uz1; /* cosines of transmission alpha. */
	double r = 0.0; /* reflectance */
	short layer = this->layer;
	double ni = In.layerspecs[layer].rfct_index;
	double nt = In.layerspecs[layer + 1].rfct_index;
	/* Get r. */
	if (uz <= In.layerspecs[layer].cos_crit_down)
		r = 1.0; /* total internal reflection. */
	else r = RFresnel(ni, nt, uz, uz1);

#if PARTIALREFLECTION
	if (layer == In.num_layers && r < 1.0) {
		this->uz = uz1;
		recordWeightLastLayer(r, In,Out);
		this->uz = -uz;

	}
	else if (RandomNum() > r) {/* transmitted to layer+1. */
		this->layer++;
		this->dcos_x *= ni / nt;
		this->dcos_y *= ni / nt;
		this->dcos_z = uz1;

	}
	else /* reflected. */
		this->dcos_z = -uz;
#else
	if (RandomNum() > r) { /* transmitted to layer+1. */
		if (layer == In.num_layers) {
			this->dcos_z = uz1;
			recordWeightLastLayer(0.0, In,Out);
			this->dead = true;

		}
		else {
			this->layer++;
			this->dcos_x *= ni / nt;
			this->dcos_y *= ni / nt;
			this->dcos_z = uz1;

		}

	}
	else /* reflected. */
		this->dcos_z = -uz;
#endif
}


void PhotonClass::crossOrNot(const InputClass& In, OutClass& Out)
{
	if (this->dcos_z < 0.0)
		crossUpOrNot(In, Out);
	else
		crossDownOrNot(In,  Out);
}

/*
 * Move the photon packet in glass layer.
 * Horizontal photons are killed because they will
 * never interact with tissue again.
 */
void PhotonClass::hopInGlass(const InputClass& In, OutClass& Out) {

	double dl; /* step size. 1/cm */

	if (this->dcos_z == 0.0) {
		/* horizontal photon in glass is killed. */
		this->dead = true;

	}
	else {
		stepSizeInGlass(In);
		hop();
		crossOrNot(In, Out);
	}
}

/*
 * Set a step size, move the photon, drop some weight,
 * choose a new photon direction for propagation.
 *
 * When a step size is long enough for the photon to
 * hit an interface, this step is divided into two steps.
 * First, move the photon to the boundary free of
 * absorption or scattering, then decide whether the
 * photon is reflected or transmitted.
 * Then move the photon in the current or transmission
 * medium with the unfinished stepsize to interaction
 * site. If the unfinished stepsize is still too long,
 * repeat the above process.
 */
void PhotonClass::hopDropSpinInTissue(const InputClass& In, OutClass& Out) {
	stepSizeInTissue( In);
	if (hitBoundary( In)) {
		hop(); /* move to boundary plane. */
		crossOrNot(In, Out);
	}
	else {
		hop();
		drop(In, Out);
		spin(In.layerspecs[this->layer].anisotropy);
	}
}

void PhotonClass::hopDropSpin(const InputClass& In, OutClass& Out) {
	short layer = this->layer;
	if ((In.layerspecs[layer].abs_coef == 0.0)
		&& (In.layerspecs[layer].scat_coef == 0.0))
		/* glass layer. */
		hopInGlass(In, Out);
	else
		hopDropSpinInTissue(In, Out);

	if (this->weight < In.Wth && !this->dead)
		roulette();
}

