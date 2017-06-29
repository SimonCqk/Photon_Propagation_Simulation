#include"utility_fwd.h"
using std::vector;
const double PI = 3.1415926;
const double WEIGHT = 1E-4;/* Critical weight for roulette. */
const double CHANCE = 0.1; /* Chance of roulette survival. */

/*
Compute the critical angles for total internal reflection according to the relative refractive index of the layer.
All layers are processed.
*/
void CriticalAngle(size_t Layer_num,vector<LayerClass>& layerspecs)
{
    double n1,n2;
    for(int i=1;i<=Layer_num;++i){
        n1=layerspecs[i].layer->scat_coef;
        n2=layerspecs[i-1].layer->scat_coef;

        layerspecs[i].layer->cos_crit_up=(n1>n2)?sqrt(1.0-n2*n2/n1*n1):0.0;

        n2=layerspecs[i+1].layer->scat_coef;
        
        layerspecs[i].layer->cos_crit_down=(n1>n2)?sqrt(1.0-n2*n2/n1*n1):0.0;
    }
}


/*
Get 1D array elements by summing the 2D array elements.
*/
void Sum2DRd(const InputClass& In,OutClass& Out)
{
    size_t nr=In.input->nr;
    size_t na=In.input->na;
    size_t ir,ia;

    double sum;

	for (ir = 0; ir < nr; ir++) {
		sum = 0.0;
		for (ia = 0; ia < na; ia++) sum += Out.out->Rd_ra[ir][ia];
		Out.out->Rd_r[ir] = sum;
	}

	for (ia = 0; ia < na; ia++) {
		sum = 0.0;
		for (ir = 0; ir < nr; ir++) sum += Out.out->Rd_ra[ir][ia];
		Out.out->Rd_a[ia] = sum;

	}

	sum = 0.0;
	for (ir = 0; ir < nr; ir++) sum += Out.out->Rd_r[ir];
	Out.out->Rd = sum;
}

/*
 * Return the index to the layer according to the index
 * to the grid line system in z direction (Iz).
 * Use the center of box.
 */
size_t IzToLayer(size_t Iz, const InputClass& In_Parm)
{
	size_t i = 1; /* index to layer. */
	size_t num_layers = In_Parm.input->num_layers;
	double dz = In_Parm.input->dz;

	while ((Iz + 0.5)*dz >= In_Parm.input->layerspecs[i].layer->z1;
		&& i < num_layers) i++;

	return i;
}

/*
 * Get 1D array elements by summing the 2D array elements.
 */
void Sum2DA(const InputClass& In_Parm, OutClass& Out_Ptr)
{
	size_t nz = In_Parm.input->nz;
	size_t nr = In_Parm.input->nr;
	size_t iz, ir;
	double sum;

	for (iz = 0; iz < nz; iz++) {
		sum = 0.0;
		for (ir = 0; ir < nr; ir++) sum += Out_Ptr.out->A_rz[ir][iz];
		Out_Ptr.out->A_z[iz] = sum;

	}
	sum = 0.0;
	for (iz = 0; iz < nz; iz++) {

		sum += Out_Ptr.out->A_z[iz];
		Out_Ptr.out->A_l[IzToLayer(iz, In_Parm)]+= Out_Ptr.out->A_z[iz];
	}
	Out_Ptr.out->A = sum;
}

/*
 * Get 1D array elements by summing the 2D array elements.
 */
void Sum2DTt(const InputClass& In_Parm, OutClass& Out_Ptr)
{
	size_t nr = In_Parm.input->nr;
	size_t na = In_Parm.input->na;
	size_t ir, ia;
	double sum;

	for (ir = 0; ir < nr; ir++) {
		sum = 0.0;
		for (ia = 0; ia < na; ia++) sum += Out_Ptr.out->Tt_ra[ir][ia];
		Out_Ptr.out->Tt_r[ir] = sum;

	}

	for (ia = 0; ia < na; ia++) {
		sum = 0.0;
		for (ir = 0; ir < nr; ir++) sum += Out_Ptr.out->Tt_ra[ir][ia];
		Out_Ptr.out->Tt_a[ia] = sum;

	}
	sum = 0.0;
	for (ir = 0; ir < nr; ir++) sum += Out_Ptr.out->Tt_r[ir];
	Out_Ptr.out->Tt = sum;
}
/*
    * Scale Rd and Tt properly. "a" stands for angle alpha.
    *
    * Scale Rd(r,a) and Tt(r,a) by (area perpendicular to photon direction)
    * x(solid angle)x(No. of photons). or [2*PI*r*dr*cos(a)]x[2*PI*sin(a)*da]x[No. of photons]
    * or [2*PI*PI*dr*da*r*sin(2a)]x[No. of photons]
    *
    * Scale Rd(r) and Tt(r) by (area on the surface)x(No. of photons).
    * Scale Rd(a) and Tt(a) by (solid angle)x(No. of photons).
*/
void ScaleRdTt(const InputClass& In_Parm, OutClass& Out_Ptr)
{
	size_t nr = In_Parm.input->nr;
	size_t na = In_Parm.input->na;
	double dr = In_Parm.input->dr;
	double da = In_Parm.input->da;
	size_t ir, ia;
	double scale1, scale2;

	scale1 = 4.0*PI*PI*dr*sin(da / 2)*dr*In_Parm.input->num_photons;
	/* The factor (ir+0.5)*sin(2a) to be added. */

	for (ir = 0; ir < nr; ir++)
		for (ia = 0; ia < na; ia++) {
			scale2 = 1.0 / ((ir + 0.5)*sin(2.0*(ia + 0.5)*da)*scale1);
			Out_Ptr.out->Rd_ra[ir][ia] *= scale2;
			Out_Ptr.out->Tt_ra[ir][ia] *= scale2;
		}

	scale1 = 2.0*PI*dr*dr*In_Parm.input->num_photons;
	/* area is 2*PI*[(ir+0.5)*dr]*dr. ir+0.5 to be added. */

	for (ir = 0; ir < nr; ir++) {
		scale2 = 1.0 / ((ir + 0.5)*scale1);
		Out_Ptr.out->Rd_r[ir] *= scale2;
		Out_Ptr.out->Tt_r[ir] *= scale2;

	}

	scale1 = 2.0*PI*da*In_Parm.input->num_photons;
	/* solid angle is 2*PI*sin(a)*da. sin(a) to be added. */
	for (ia = 0; ia < na; ia++) {
		scale2 = 1.0 / (sin((ia + 0.5)*da)*scale1);
		Out_Ptr.out->Rd_a[ia] *= scale2;
		Out_Ptr.out->Tt_a[ia] *= scale2;
	}
	scale2 = 1.0 /static_cast<double>(In_Parm.input->num_photons);
	Out_Ptr.out->Rd *= scale2;
	Out_Ptr.out->Tt *= scale2;
}
/*
 * Scale absorption arrays properly.
 */
void ScaleA(const InputClass& In_Parm, OutClass& Out_Ptr)
{
	size_t nz = In_Parm.input->nz;
	size_t nr = In_Parm.input->nr;
	double dz = In_Parm.input->dz;
	double dr = In_Parm.input->dr;
	size_t nl = In_Parm.input->num_layers;
	size_t iz, ir,il;
	double scale1;
	/* Scale A_rz. */
	scale1 = 2.0*PI*dr*dr*dz*In_Parm.input->num_photons;
	/* volume is 2*pi*(ir+0.5)*dr*dr*dz. ir+0.5 to be added. */
	for (iz = 0; iz < nz; iz++)
		for (ir = 0; ir < nr; ir++)
			Out_Ptr.out->A_rz[ir][iz] /= (ir + 0.5)*scale1;
	/* Scale A_z. */
	scale1 = 1.0 / (dz*In_Parm.input->num_photons);
	for (iz = 0; iz < nz; iz++)
		Out_Ptr.out->A_z[iz] *= scale1;
	/* Scale A_l. Avoid int/int. */
	scale1 = 1.0 / static<double>(In_Parm.input->num_photons);
	for (il = 0; il <= nl + 1; il++)
		Out_Ptr.out->A_l[il] *= scale1;
	Out_Ptr.out->A *= scale1;
}

/* Sum and scale results of current run. */
void SumScaleResult(const InputClass& In_Parm, OutClass& Out_Ptr)
{
	/* Get 1D & 0D results. */
	Sum2DRd(In_Parm, Out_Ptr);
	Sum2DA(In_Parm, Out_Ptr);
	Sum2DTt(In_Parm, Out_Ptr);

	ScaleRdTt(In_Parm, Out_Ptr);
	ScaleA(In_Parm, Out_Ptr);
}