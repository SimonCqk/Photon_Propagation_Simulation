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

