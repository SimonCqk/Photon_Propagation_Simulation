#include"utility_fwd.h"
#include"mcml.h"
#include<QDebug>


void InitOutputData(const InputClass& In_Parm,OutClass& Out_Ptr)
{
    size_t nz = In_Parm.input->nz;
    size_t nr = In_Parm.input->nr;
    size_t na = In_Parm.input->na;
    size_t nl = In_Parm.input->num_layers;

    /* Init pure numbers. */
    Out_Ptr.out->spec_reflect = 0.0;
    Out_Ptr.out->diff_reflect = 0.0;
    Out_Ptr.out->abs_prob = 0.0;
    Out_Ptr.out->total_trans= 0.0;

    AllocMatrix<double>(Out_Ptr.out->diff_reflect_2d,nr,na);
    AllocVector<double>(Out_Ptr.out->diff_reflect_rdl, nr);
    AllocVector<double>(Out_Ptr.out->diff_reflect_agl, na);

    AllocMatrix<double>(Out_Ptr.out->abs_prob_rz, nr , nz );
    AllocVector<double>( Out_Ptr.out->abs_prob_z,nz);
    AllocVector<double>(Out_Ptr.out->abs_prob_layer, nl + 2);

    AllocMatrix<double>(Out_Ptr.out->total_trans_2d,nr, na);
    AllocVector<double>(Out_Ptr.out->total_trans_rdl, nr );
    AllocVector<double>(Out_Ptr.out->total_trans_agl, na);
}


/*
  Compute the critical angles for total internal reflection according to
  the relative refractive index of the layer.
  All layers are processed.
*/
void CriticalAngle(const size_t& Layer_num, QVector<LayerClass>& layerspecs)
{
    double n1, n2;
    for (int i = 1; i <= Layer_num; ++i) {
        n1 = layerspecs[i].layer->rfct_index;
        n2 = layerspecs[i - 1].layer->rfct_index;

        layerspecs[i].layer->cos_crit_up = (n1>n2) ? sqrt(1.0 - n2*n2 / n1*n1) : 0.0;

        n2 = layerspecs[i + 1].layer->rfct_index;

        layerspecs[i].layer->cos_crit_down = (n1>n2) ? sqrt(1.0 - n2*n2 / n1*n1) : 0.0;
    }
}

/*
  Get 1D array elements by summing the 2D array elements.
*/
void Sum2D_DiffReflect(const InputClass& In,OutClass& Out)
{
    size_t nr=In.input->nr;
    size_t na=In.input->na;
    size_t ir,ia;

    double sum;

    for (ir = 0; ir < nr; ir++) {
        sum = 0.0;
        for (ia = 0; ia < na; ia++) sum += Out.out->diff_reflect_2d[ir][ia];
        Out.out->diff_reflect_rdl[ir] = sum;
    }

    for (ia = 0; ia < na; ia++) {
        sum = 0.0;
        for (ir = 0; ir < nr; ir++) sum += Out.out->diff_reflect_2d[ir][ia];
        Out.out->diff_reflect_agl[ia] = sum;

    }

    sum = 0.0;
    for (ir = 0; ir < nr; ir++) sum += Out.out->diff_reflect_rdl[ir];
    Out.out->diff_reflect = sum;
}

/*
 * Return the index to the layer according to the index
 * to the grid line system in z direction (Iz).
 * Use the center of box.
 */
size_t IndexToLayer(const size_t& Index, const InputClass& In_Parm)
{
    size_t i = 1; /* index to layer. */
    size_t num_layers = In_Parm.input->num_layers;
    double dz = In_Parm.input->dz;

    while ((Index + 0.5)*dz >= In_Parm.input->layerspecs[i].layer->z1
        && i < num_layers) i++;

    return i;
}

/*
 * Get 1D array elements by summing the 2D array elements.
 */
void Sum2D_AbsProb(const InputClass& In_Parm, OutClass& Out_Ptr)
{
    size_t nz = In_Parm.input->nz;
    size_t nr = In_Parm.input->nr;
    size_t iz, ir;
    double sum;

    for (iz = 0; iz < nz; iz++) {
        sum = 0.0;
        for (ir = 0; ir < nr; ir++) sum += Out_Ptr.out->abs_prob_rz[ir][iz];
        Out_Ptr.out->abs_prob_z[iz] = sum;

    }
    sum = 0.0;
    for (iz = 0; iz < nz; iz++) {

        sum += Out_Ptr.out->abs_prob_z[iz];
        Out_Ptr.out->abs_prob_layer[IndexToLayer(iz, In_Parm)]+= Out_Ptr.out->abs_prob_z[iz];
    }
    Out_Ptr.out->abs_prob = sum;
}

/*
 * Get 1D array elements by summing the 2D array elements.
 */
void Sum2D_TotalTrans(const InputClass& In_Parm, OutClass& Out_Ptr)
{
    size_t nr = In_Parm.input->nr;
    size_t na = In_Parm.input->na;
    size_t ir, ia;
    double sum;

    for (ir = 0; ir < nr; ir++) {
        sum = 0.0;
        for (ia = 0; ia < na; ia++) sum += Out_Ptr.out->total_trans_2d[ir][ia];
        Out_Ptr.out->total_trans_rdl[ir] = sum;

    }

    for (ia = 0; ia < na; ia++) {
        sum = 0.0;
        for (ir = 0; ir < nr; ir++) sum += Out_Ptr.out->total_trans_2d[ir][ia];
        Out_Ptr.out->total_trans_agl[ia] = sum;

    }
    sum = 0.0;
    for (ir = 0; ir < nr; ir++) sum += Out_Ptr.out->total_trans_rdl[ir];
    Out_Ptr.out->total_trans = sum;
}
/*
 * Scale diff_reflect and total_trans properly. "a" stands for angle alpha.
 *
 * Scale diff_reflect(r,a) and total_trans(r,a) by (area perpendicular to photon direction)
 * x(solid angle)x(No. of photons). or [2*PI*r*dr*cos(a)]x[2*PI*sin(a)*da]x[No. of photons]
 * or [2*PI*PI*dr*da*r*sin(2a)]x[No. of photons]
 *
 * Scale diff_reflect(r) and total_trans(r) by (area on the surface)x(No. of photons).
 * Scale diff_reflect(a) and total_trans(a) by (solid angle)x(No. of photons).
 */
void Scale_DiffReflect_TotalTrans(const InputClass& In_Parm, OutClass& Out_Ptr)
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
            Out_Ptr.out->diff_reflect_2d[ir][ia] *= scale2;
            Out_Ptr.out->total_trans_2d[ir][ia] *= scale2;
        }

    scale1 = 2.0*PI*dr*dr*In_Parm.input->num_photons;
    /* area is 2*PI*[(ir+0.5)*dr]*dr. ir+0.5 to be added. */

    for (ir = 0; ir < nr; ir++) {
        scale2 = 1.0 / ((ir + 0.5)*scale1);
        Out_Ptr.out->diff_reflect_rdl[ir] *= scale2;
        Out_Ptr.out->total_trans_rdl[ir] *= scale2;

    }

    scale1 = 2.0*PI*da*In_Parm.input->num_photons;
    /* solid angle is 2*PI*sin(a)*da. sin(a) to be added. */
    for (ia = 0; ia < na; ia++) {
        scale2 = 1.0 / (sin((ia + 0.5)*da)*scale1);
        Out_Ptr.out->diff_reflect_agl[ia] *= scale2;
        Out_Ptr.out->total_trans_agl[ia] *= scale2;
    }
    scale2 = 1.0 /static_cast<double>(In_Parm.input->num_photons);
    Out_Ptr.out->diff_reflect *= scale2;
    Out_Ptr.out->total_trans *= scale2;
}
/*
 * Scale absorption arrays properly.
 */
void Scale_AbsProb(const InputClass& In_Parm, OutClass& Out_Ptr)
{
    size_t nz = In_Parm.input->nz;
    size_t nr = In_Parm.input->nr;
    double dz = In_Parm.input->dz;
    double dr = In_Parm.input->dr;
    size_t nl = In_Parm.input->num_layers;
    size_t iz, ir,il;
    double scale1;
    /* Scale abs_prob_rz. */
    scale1 = 2.0*PI*dr*dr*dz*In_Parm.input->num_photons;
    /* volume is 2*pi*(ir+0.5)*dr*dr*dz. ir+0.5 to be added. */
    for (iz = 0; iz < nz; iz++)
        for (ir = 0; ir < nr; ir++)
            Out_Ptr.out->abs_prob_rz[ir][iz] /= (ir + 0.5)*scale1;
    /* Scale abs_prob_z. */
    scale1 = 1.0 / (dz*In_Parm.input->num_photons);
    for (iz = 0; iz < nz; iz++)
        Out_Ptr.out->abs_prob_z[iz] *= scale1;
    /* Scale abs_prob_layer. Avoid int/int. */
    scale1 = 1.0 / static_cast<double>(In_Parm.input->num_photons);
    for (il = 0; il <= nl + 1; il++)
        Out_Ptr.out->abs_prob_layer[il] *= scale1;
    Out_Ptr.out->abs_prob *= scale1;
}

/* Sum and scale results of current run. */
void SumScaleResult(const InputClass& In_Parm, OutClass& Out_Ptr)
{
    /* Get 1D & 0D results. */
    Sum2D_DiffReflect(In_Parm, Out_Ptr);
    Sum2D_AbsProb(In_Parm, Out_Ptr);
    Sum2D_TotalTrans(In_Parm, Out_Ptr);

    Scale_DiffReflect_TotalTrans(In_Parm, Out_Ptr);
    Scale_AbsProb(In_Parm, Out_Ptr);
}
