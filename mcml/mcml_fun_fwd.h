#include<vector>
using std::vector;

double RandomNum();
double Rspecular(vector<LayerClass>& LayerVec);
double SpinTheta(double g);
double RFresnel(double n1, double n2, double ca1, double& ca2_Ptr);
