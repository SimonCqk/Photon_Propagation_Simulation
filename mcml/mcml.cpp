// The implementation of Monte Carlo Model
#include "mcml.h"
#include <QDebug>

#define PARTIALREFLECTION 0
/* 1=split photon, 0=statistical reflection. */

auto SIGN = [](int x) { return (x >= 0) ? 1 : -1; };

// initialize a photon packet.
void PhotonClass::launch(const double &Rspecular,
                         const QVector<LayerClass> &LayerVec) {
  photon->weight = 1.0 - Rspecular;
  photon->dead = false;
  photon->layer = 1;
  photon->cur_step = photon->step_left = 0;
  photon->x = photon->y = photon->z = 0.0;
  photon->dcos_x = photon->dcos_y = 0.0;
  photon->dcos_z = 1.0;
  if ((LayerVec[1].layer->abs_coef == 0.0) &&
      (LayerVec[1].layer->scat_coef == 0.0)) { /* glass layer. */
    photon->layer = 2;
    photon->z = LayerVec[2].layer->z0;
  }
}

/*
 * Choose a new direction for photon propagation by sampling the polar
 * deflection angle theta and the
 * azimuthal angle psi.
 *
 * Note:
 * theta: 0 - pi so sin(theta) is always positive feel free to use sqrt() for
 * cos(theta).
 *
 * psi: 0 - 2pi
 * for 0-pi sin(psi) is +
 * for pi-2pi sin(psi) is -
*/

void PhotonClass::spin(const double &g) {
  double cost = SpinTheta(g);
  double sint = sqrt(1.0 - cost * cost); /* cosine and sine of the */
                                         /* polar deflection angle theta. */
  double psi = 2.0 * PI * RandomNum();   /* spin psi 0-2pi. */
  double cosp = cos(psi), sinp;          /* cosine and sine of the */
                                         /* azimuthal angle psi. */
  double dcos_x = photon->dcos_x;
  double dcos_y = photon->dcos_y;
  double dcos_z = photon->dcos_z;

  if (psi < PI)
    sinp = sqrt(1.0 - cosp * cosp);
  /* sqrt() is faster than sin(). */
  else
    sinp = -sqrt(1.0 - cosp * cosp);
  if (fabs(photon->dcos_z) > COSZERO) { /* normal incident. */
    photon->dcos_x = sint * cosp;
    photon->dcos_y = sint * sinp;
    photon->dcos_z = cost * (SIGN(dcos_z));
    /* SIGN() is faster than division. */
  } else { /* regular incident. */
    double temp = sqrt(1.0 - dcos_z * dcos_z);
    photon->dcos_x =
        sint * (dcos_x * dcos_z * cosp - dcos_y * sinp) / temp + dcos_x * cost;
    photon->dcos_y =
        sint * (dcos_y * dcos_z * cosp + dcos_x * sinp) / temp + dcos_y * cost;
    photon->dcos_z = -sint * cosp * temp + dcos_z * cost;
  }
}

/*
        Move the photon s away in the current layer of medium.
*/
void PhotonClass::hop() {
  double s = photon->cur_step;
  photon->x += s * photon->dcos_x;
  photon->y += s * photon->dcos_y;
  photon->z += s * photon->dcos_z;
}

/*
 * If dcos_z != 0, return the photon step size in glass, Otherwise, return 0.
 *
 * The step size is the distance between the current position and the boundary
 in the photon direction.
 *
 * Make sure dcos_z !=0 before calling this function.

*/
void PhotonClass::stepSizeInGlass(const InputClass &In) {
  double step_size; /* step size to boundary. */
  size_t layer = photon->layer;
  double dcos_z = photon->dcos_z;

  /* Stepsize to the boundary. */
  if (dcos_z > 0.0)
    step_size = (In.input->layerspecs[layer].layer->z1 - photon->z) / dcos_z;
  else if (dcos_z < 0.0)
    step_size = (In.input->layerspecs[layer].layer->z0 - photon->z) / dcos_z;
  else
    step_size = 0.0;
  photon->cur_step = step_size;
}

/*
 * Pick a step size for a photon packet when it is in tissue.
 * If the member sleft is zero, make a new step size
 * with: -log(rnd)/(mua+mus).
 * Otherwise, pick up the leftover in sleft.
 *
 * Layer is the index to layer.
 * In_Ptr is the input parameters.
 */

void PhotonClass::stepSizeInTissue(const InputClass &In) {
  size_t layer = photon->layer;
  double abs_coef = In.input->layerspecs[layer].layer->abs_coef;
  double scat_coef = In.input->layerspecs[layer].layer->scat_coef;
  if (photon->step_left == 0.0) { /* make a new step. */
    double rnd = RandomNum();
    photon->cur_step = -log(rnd) / (abs_coef + scat_coef);
  } else { /* take the leftover. */
    photon->cur_step = photon->step_left / (abs_coef + scat_coef);
    photon->step_left = 0.0;
  }
}

/*
 * Check if the step will hit the boundary.
 *
 * If the projected step hits the boundary, the members cur_step and step_left
 * of Photon are updated.
*/
bool PhotonClass::hitBoundary(const InputClass &In) {
  double step_size; /* length to boundary. */
  size_t layer = photon->layer;
  double dcos_z = photon->dcos_z;
  bool hitted;
  /* Distance to the boundary. */
  if (dcos_z > 0.0)
    step_size = (In.input->layerspecs[layer].layer->z1 - photon->z) /
                dcos_z; /* dl_b>0. */
  else if (dcos_z < 0.0)
    step_size = (In.input->layerspecs[layer].layer->z0 - photon->z) /
                dcos_z; /* dl_b>0. */

  if (dcos_z != 0.0 && (photon->cur_step > step_size)) {
    /* not horizontal & crossing. */
    double mut = In.input->layerspecs[layer].layer->abs_coef +
                 In.input->layerspecs[layer].layer->scat_coef;
    photon->step_left = (photon->cur_step - step_size) * mut;
    photon->cur_step = step_size;
    hitted = true;
  } else
    hitted = false;
  return hitted;
}

/*
 * Drop photon weight inside the tissue (not glass).
 *
 * The photon is assumed not dead.
 *
 * The weight drop is d_weight = weight*abs_coef / (abs_coef + scat_coef).
 *
 * The dropped weight is assigned to the absorption array elements.
*/
void PhotonClass::drop(const InputClass &In, OutClass &Out) {
  double dwa; /* absorbed weight.*/
  double x = photon->x;
  double y = photon->y;
  size_t iz, ir; /* index to z & r. */
  size_t layer = photon->layer;
  double abs_coef, scat_coef;

  /* compute array indices. */
  iz = static_cast<size_t>(photon->z / In.input->dz);
  if (iz > In.input->nz - 1)
    iz = In.input->nz - 1;

  ir = static_cast<size_t>(sqrt(x * x + y * y) / In.input->dr);
  if (ir > In.input->nr - 1)
    ir = In.input->nr - 1;

  /* update photon weight. */
  abs_coef = In.input->layerspecs[layer].layer->abs_coef;
  scat_coef = In.input->layerspecs[layer].layer->scat_coef;
  dwa = photon->weight * abs_coef / (abs_coef + scat_coef);
  photon->weight -= dwa;
  /* assign dwa to the absorption array element. */
  Out.out->abs_prob_rz[ir][iz] += dwa;
}

/*
 * The photon weight is small, and the photon packet tries to survive a
 * roulette.
 */
void PhotonClass::roulette() {
  if (photon->weight == 0.0)
    photon->dead = true;

  else if (RandomNum() < CHANCE) /* survived the roulette.*/
    photon->weight /= CHANCE;
  else
    photon->dead = true;
}

/*
 * Record the photon weight exiting the first layer(dcos_z < 0),
 * no matter whether the layer is glass or not, to the reflection array.
 *
 * Update the photon weight as well.
 */
void PhotonClass::recordWeightFirstLayer(const double &Refl, /* reflectance. */
                                         const InputClass &In, OutClass &Out) {
  double x = photon->x;
  double y = photon->y;
  /* index to r & angle. */
  size_t ia = static_cast<size_t>(acos(-photon->dcos_z) / In.input->da);
  size_t ir = static_cast<size_t>(sqrt(x * x + y * y) / In.input->dr);
  if (ir > In.input->nr - 1)
    ir = In.input->nr - 1;

  if (ia > In.input->na - 1)
    ia = In.input->na - 1;
  /* assign photon to the reflection array element. */
  Out.out->diff_reflect_2d[ir][ia] += photon->weight * (1.0 - Refl);
  photon->weight *= Refl;
}

/*
 * Record the photon weight exiting the last layer(dcos_z>0),
 * no matter whether the layer is glass or not, to the transmittance array.
 *
 * Update the photon weight as well.
 */
void PhotonClass::recordWeightLastLayer(const double &Refl,
                                        const InputClass &In, OutClass &Out) {
  double x = photon->x;
  double y = photon->y;
  size_t ir, ia; /* index to r & angle. */
  ir = static_cast<size_t>(sqrt(x * x + y * y) / In.input->dr);
  if (ir > In.input->nr - 1)
    ir = In.input->nr - 1;
  ia = static_cast<size_t>(acos(photon->dcos_z) / In.input->da);
  if (ia > In.input->na - 1)
    ia = In.input->na - 1;
  /* assign photon to the transmittance array element. */
  Out.out->total_trans_2d[ir][ia] += photon->weight * (1.0 - Refl);
  photon->weight *= Refl;
}

/*
 * Decide whether the photon will be transmitted or
 * reflected on the upper boundary (dcos_z<0) of the current layer.
 *
 * If "layer" is the first layer, the photon packet will be partially
 * transmitted and partially reflected if
 * PARTIALREFLECTION is set to 1, or the photon packet will be either
 * transmitted or
 * reflected determined statistically if PARTIALREFLECTION is set to 0.
 *
 * Record the transmitted photon weight as reflection.
 *
 * If the "layer" is not the first layer and the photon packet is transmitted,
 * move the photon to "layer-1".
 *
 * Update the photon parmameters.
 */
void PhotonClass::crossUpOrNot(const InputClass &In, OutClass &Out) {
  double dcos_z = photon->dcos_z; /* z directional cosine. */
  double tcos_a;  /* cosines of transmission alpha. always positive. */
  double r = 0.0; /* reflectance */
  size_t layer = photon->layer;
  double ni = In.input->layerspecs[layer].layer->rfct_index;
  double nt = In.input->layerspecs[layer - 1].layer->rfct_index;

  /* Get r. */
  if (-dcos_z <= In.input->layerspecs[layer].layer->cos_crit_up)
    r = 1.0; /* total internal reflection. */
  else
    r = RFresnel(ni, nt, -dcos_z, tcos_a);

#if PARTIALREFLECTION
  if (layer == 1 && r < 1.0) { /* partially transmitted. */
    photon->dcos_z = -uz1;     /* transmitted photon. */
    recordWeightFirstLayer(r, In, Out);
    photon->uz = -uz; /* reflected photon. */

  } else if (RandomNum() > r) { /* transmitted to layer-1. */
    photon->layer--;
    photon->dcos_x *= ni / nt;
    photon->dcos_y *= ni / nt;
    photon->dcos_z = -uz1;

  } else /* reflected. */
    photon->dcos_z = -uz;
#else
  if (RandomNum() > r) { /* transmitted to layer-1. */
    if (layer == 1) {
      photon->dcos_z = -tcos_a;
      recordWeightFirstLayer(0.0, In, Out);
      photon->dead = true;
    } else {
      photon->layer--;
      photon->dcos_x *= ni / nt;
      photon->dcos_y *= ni / nt;
      photon->dcos_z = -tcos_a;
    }

  } else /* reflected. */
    photon->dcos_z = -dcos_z;
#endif
}

/*
 * Decide whether the photon will be transmitted or be
 * reflected on the bottom boundary (dcos_z > 0) of the current layer.
 *
 * If the photon is transmitted, move the photon to "layer+1". If "layer" is the
 * last layer, record the
 * transmitted weight as transmittance. See comments for CrossUpOrNot.
 *
 * Update the photon parmameters.
 */
void PhotonClass::crossDownOrNot(const InputClass &In, OutClass &Out) {
  double dcos_z = photon->dcos_z; /* z directional cosine. */
  double tcos_a;                  /* cosines of transmission alpha. */
  double r = 0.0;                 /* reflectance */
  size_t layer = photon->layer;
  double ni = In.input->layerspecs[layer].layer->rfct_index;
  double nt = In.input->layerspecs[layer + 1].layer->rfct_index;
  /* Get r. */
  if (dcos_z <= In.input->layerspecs[layer].layer->cos_crit_down)
    r = 1.0; /* total internal reflection. */
  else
    r = RFresnel(ni, nt, dcos_z, tcos_a);

#if PARTIALREFLECTION
  if (layer == In.input->num_layers && r < 1.0) {
    photon->uz = uz1;
    recordWeightLastLayer(r, In, Out);
    photon->uz = -uz;

  } else if (RandomNum() > r) { /* transmitted to layer+1. */
    photon->layer++;
    photon->dcos_x *= ni / nt;
    photon->dcos_y *= ni / nt;
    photon->dcos_z = uz1;

  } else /* reflected. */
    photon->dcos_z = -uz;
#else
  if (RandomNum() > r) { /* transmitted to layer+1. */
    if (layer == In.input->num_layers) {
      photon->dcos_z = tcos_a;
      recordWeightLastLayer(0.0, In, Out);
      photon->dead = true;

    } else {
      photon->layer++;
      photon->dcos_x *= ni / nt;
      photon->dcos_y *= ni / nt;
      photon->dcos_z = tcos_a;
    }
  } else /* reflected. */
    photon->dcos_z = -dcos_z;
#endif
}

void PhotonClass::crossOrNot(const InputClass &In, OutClass &Out) {
  if (photon->dcos_z < 0.0)
    crossUpOrNot(In, Out);
  else
    crossDownOrNot(In, Out);
}

/*
 * Move the photon packet in glass layer.
 * Horizontal photons are killed because they will never interact with tissue
 * again.
 */
void PhotonClass::hopInGlass(const InputClass &In, OutClass &Out) {

  // double dl; /* step size. 1/cm */

  if (photon->dcos_z == 0.0) {
    /* horizontal photon in glass is killed. */
    photon->dead = true;

  } else {
    stepSizeInGlass(In);
    hop();
    crossOrNot(In, Out);
  }
}

/*
 * Set a step size, move the photon, drop some weight,
 * choose a new photon direction for propagation.
 *
 * When a step size is long enough for the photon to hit an interface, this step
 * is divided into two steps.
 * First, move the photon to the boundary free of absorption or scattering, then
 * decide whether the
 * photon is reflected or transmitted.
 * Then move the photon in the current or transmission medium with the
 * unfinished stepsize to interaction
 * site. If the unfinished stepsize is still too long, repeat the above process.
 */
void PhotonClass::hopDropSpinInTissue(const InputClass &In, OutClass &Out) {
  stepSizeInTissue(In);

  if (hitBoundary(In)) {
    hop(); /* move to boundary plane. */
    crossOrNot(In, Out);
  } else {
    hop();
    drop(In, Out);
    spin(In.input->layerspecs[photon->layer].layer->anisotropy);
  }
}

void PhotonClass::hopDropSpin(const InputClass &In, OutClass &Out) {
  size_t layer = photon->layer;
  if ((In.input->layerspecs[layer].layer->abs_coef == 0.0) &&
      (In.input->layerspecs[layer].layer->scat_coef == 0.0))
    /* glass layer. */
    hopInGlass(In, Out);
  else
    hopDropSpinInTissue(In, Out);

  if (photon->weight < In.input->Wth && !photon->dead)
    roulette();
}

void InputClass::resetData()
{
    input.reset(new InputStruct);
}
