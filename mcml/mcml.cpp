//The implementation of Monte Carlo Model

#include"mcml.h"

#define  PARTIALREFLECTION  0
/* 1=split photon, 0=statistical reflection. */

const int STANDARDTEST = 0;
/* testing program using fixed rnd seed. */

const double COSZERO = (1.0 - 1.0E-12);
/* cosine of about 1e-6 rad. */

const double  COS90D = 1.0E-6;
/* cosine of about 1.57 - 1e-6 rad. */


//The specific declaration of mem-function in PhotonClass
void PhotonClass::launch(double Rspecular, vector<LayerClass>& LayerVec)
{
	photon->weight = 1.0 - Rspecular;
	photon->dead = false;
	photon->layer = 1;
	photon->cur_step = photon->step_left = 0;
	photon->x = photon->y = photon->z = 0.0;
	photon->dcos_x = photon->dcos_y = 0.0;
	photon->dcos_z = 1.0;
	if ((LayerVec[1].layer->abs_coef == 0.0) && (LayerVec[1].layer->scat_coef == 0.0)) { /* glass layer. */
		photon->layer = 2;
		photon->z = LayerVec[2].layer->z0;
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
	if (fabs(photon->dcos_z) > COSZERO) { /* normal incident. */
		photon->dcos_x = sint*cosp;
		photon->dcos_y = sint*sinp;
		photon->dcos_z = cost*SIGN(photon->dcos_z);
		/* SIGN() is faster than division. */
	}
	else { /* regular incident. */
		double temp = sqrt(1.0 - photon->dcos_z*photon->dcos_z);
		photon->dcos_x = sint*(photon->dcos_x*photon->dcos_z*cosp - photon->dcos_y*sinp)
			/ temp + photon->dcos_x*cost;
		photon->dcos_y = sint*(photon->dcos_y*photon->dcos_z*cosp + photon->dcos_x*sinp)
			/ temp + photon->dcos_y*cost;
		photon->dcos_z = -sint*cosp*temp + photon->dcos_z*cost;
	}
}

/*
	Move the photon s away in the current layer of medium.
*/
void PhotonClass::hop() {
	double s = photon->cur_step;
	photon->x += s*photon->dcos_x;
	photon->y += s*photon->dcos_y;
	photon->z += s*photon->dcos_z;
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
	short layer = photon->layer;
	double uz = photon->dcos_z;

	/* Stepsize to the boundary. */
	if (uz > 0.0)
		dl_b = (In.input->layerspecs[layer].layer->z1 - photon->z)
		/ uz;
	else if (uz < 0.0)
		dl_b = (In.input->layerspecs[layer].layer->z0 - photon->z)
		/ uz;
	else
		dl_b = 0.0;
	photon->cur_step = dl_b;
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
	short layer = photon->layer;
	double mua = In.input->layerspecs[layer].layer->abs_coef;
	double mus = In.input->layerspecs[layer].layer->scat_coef;
	if (photon->step_left == 0.0) { /* make a new step. */
		double rnd = RandomNum();
		photon->cur_step = -log(rnd) / (mua + mus);
	}
	else { /* take the leftover. */
		photon->cur_step = photon->step_left / (mua + mus);
		photon->step_left = 0.0;
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
	short layer = photon->layer;
	double uz = photon->dcos_z;
	bool hit;
	/* Distance to the boundary. */
	if (uz > 0.0)
		dl_b = (In.input->layerspecs[layer].layer->z1 - photon->z) / uz; /* dl_b>0. */
	else if (uz < 0.0)
		dl_b = (In.input->layerspecs[layer].layer->z0 - photon->z) / uz; /* dl_b>0. */

	if (uz != 0.0 && photon->cur_step > dl_b) {
		/* not horizontal & crossing. */
		double mut = In.input->layerspecs[layer].layer->abs_coef
			+ In.input->layerspecs[layer].layer->scat_coef;
		photon->step_left = (photon->cur_step - dl_b)*mut;
		photon->cur_step = dl_b;
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
	double x = photon->x;
	double y = photon->y;
	short iz, ir; /* index to z & r. */
	short layer = photon->layer;
	double mua, mus;

	/* compute array indices. */
	iz = static_cast<short>(photon->z / In.input->dz);
	if (iz > In.input->nz - 1) iz = In.input->nz - 1;

	ir = static_cast<short>(sqrt(x*x + y*y) / In.input->dr);
	if (ir > In.input->nr - 1) ir = In.input->nr - 1;

	/* update photon weight. */
	mua = In.input->layerspecs[layer].layer->abs_coef;
	mus = In.input->layerspecs[layer].layer->scat_coef;
	dwa = photon->weight * mua / (mua + mus);
	photon->weight -= dwa;
	/* assign dwa to the absorption array element. */
	Out.out->A_rz[ir][iz] += dwa;
}

/*
 * The photon weight is small, and the photon packet tries
 * to survive a roulette.
 */
void PhotonClass::roulette()
{
	if (photon->weight == 0.0)
		photon->dead = true;

	else if (RandomNum() < CHANCE) /* survived the roulette.*/
		photon->weight /= CHANCE;
	else
		photon->dead = true;
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
	double x = photon->x;
	double y = photon->y;
	 /* index to r & angle. */
	short ia = static_cast<short>(acos(-photon->dcos_z) / In.input->da);
	short ir = static_cast<short>(sqrt(x*x + y*y) / In.input->dr);
	if (ir > In.input->nr - 1) ir = In.input->nr - 1;

	if (ia > In.input->na - 1) ia = In.input->na - 1;
	/* assign photon to the reflection array element. */
	Out.out->Rd_ra[ir][ia] += photon->weight*(1.0 - Refl);

	photon->weight *= Refl;
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
	double x = photon->x;
	double y = photon->y;
	short ir, ia; /* index to r & angle. */
	ir = static_cast<short>(sqrt(x*x + y*y) / In.input->dr);
	if (ir > In.input->nr - 1) ir = In.input->nr - 1;
	ia = static_cast<short>(acos(photon->dcos_z) / In.input->da);
	if (ia > In.input->na - 1) ia = In.input->na - 1;
	/* assign photon to the transmittance array element. */
	Out.out->Tt_ra[ir][ia] += photon->weight*(1.0 - Refl);
	photon->weight *= Refl;
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
	double uz = photon->dcos_z; /* z directional cosine. */
	double uz1; /* cosines of transmission alpha. always positive. */
	double r = 0.0; /* reflectance */
	short layer = photon->layer;
	double ni = In.input->layerspecs[layer].layer->rfct_index;
	double nt = In.input->layerspecs[layer - 1].layer->rfct_index;

	/* Get r. */
	if (-uz <= In.input->layerspecs[layer].layer->cos_crit_up)
		r = 1.0; /* total internal reflection. */
	else r = RFresnel(ni, nt, -uz, uz1);

#if PARTIALREFLECTION
	if (layer == 1 && r < 1.0) { /* partially transmitted. */
		photon->dcos_z = -uz1; /* transmitted photon. */
		recordWeightFirstLayer(r, In, Out);
		photon->uz = -uz; /* reflected photon. */

	}
	else if (RandomNum() > r) {/* transmitted to layer-1. */
		photon->layer--;
		photon->dcos_x *= ni / nt;
		photon->dcos_y *= ni / nt;
		photon->dcos_z = -uz1;

	}
	else /* reflected. */
		photon->dcos_z = -uz;
#else
	if (RandomNum() > r) { /* transmitted to layer-1. */
		if (layer == 1) {
			photon->dcos_z = -uz1;
			recordWeightFirstLayer(0.0, In,Out);
			photon->dead = true;
		}
		else {
			photon->layer--;
			photon->dcos_x *= ni / nt;
			photon->dcos_y *= ni / nt;
			photon->dcos_z = -uz1;
		}

	}
	else /* reflected. */
		photon->dcos_z = -uz;
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
	double uz = photon->dcos_z; /* z directional cosine. */
	double uz1; /* cosines of transmission alpha. */
	double r = 0.0; /* reflectance */
	short layer = photon->layer;
	double ni = In.input->layerspecs[layer].layer->rfct_index;
	double nt = In.input->layerspecs[layer + 1].layer->rfct_index;
	/* Get r. */
	if (uz <= In.input->layerspecs[layer].layer->cos_crit_down)
		r = 1.0; /* total internal reflection. */
	else r = RFresnel(ni, nt, uz, uz1);

#if PARTIALREFLECTION
	if (layer == In.input->num_layers && r < 1.0) {
		photon->uz = uz1;
		recordWeightLastLayer(r, In,Out);
		photon->uz = -uz;

	}
	else if (RandomNum() > r) {/* transmitted to layer+1. */
		photon->layer++;
		photon->dcos_x *= ni / nt;
		photon->dcos_y *= ni / nt;
		photon->dcos_z = uz1;

	}
	else /* reflected. */
		photon->dcos_z = -uz;
#else
	if (RandomNum() > r) { /* transmitted to layer+1. */
		if (layer == In.input->num_layers) {
			photon->dcos_z = uz1;
			recordWeightLastLayer(0.0, In,Out);
			photon->dead = true;

		}
		else {
			photon->layer++;
			photon->dcos_x *= ni / nt;
			photon->dcos_y *= ni / nt;
			photon->dcos_z = uz1;

		}

	}
	else /* reflected. */
		photon->dcos_z = -uz;
#endif
}


void PhotonClass::crossOrNot(const InputClass& In, OutClass& Out)
{
	if (photon->dcos_z < 0.0)
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

	if (photon->dcos_z == 0.0) {
		/* horizontal photon in glass is killed. */
		photon->dead = true;

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
		spin(In.input->layerspecs[photon->layer].layer->anisotropy);
	}
}

void PhotonClass::hopDropSpin(const InputClass& In, OutClass& Out) {
	short layer = photon->layer;
	if ((In.input->layerspecs[layer].layer->abs_coef == 0.0)
		&& (In.input->layerspecs[layer].layer->scat_coef == 0.0))
		/* glass layer. */
		hopInGlass(In, Out);
	else
		hopDropSpinInTissue(In, Out);

	if (photon->weight < In.input->Wth && !photon->dead)
		roulette();
}

