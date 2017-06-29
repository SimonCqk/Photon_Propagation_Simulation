#include"utility_fwd.h"
using std::vector;

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
    AllocMatrix(Out.out->Rd_ra,nr,na);
    AllocVector(Out.out->Rd_a,na);

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
size_t IzToLayer(size_t Iz, InputStruct In_Parm)
{
	short i = 1; /* index to layer. */
	short num_layers = In_Parm.num_layers;
	double dz = In_Parm.dz;

	while ((Iz + 0.5)*dz >= In_Parm.layerspecs[i].z1
		&& i < num_layers) i++;

	return(i);
}
