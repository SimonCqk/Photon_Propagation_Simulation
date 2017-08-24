#include<random>
#include"mcml.h"


const int STANDARDTEST = 0;
/* testing program using fixed rnd seed. */

const double COSZERO = (1.0 - 1.0E-12);
/* cosine of about 1e-6 rad. */

const double  COS90D = 1.0E-6;
/* cosine of about 1.57 - 1e-6 rad. */


//use Mersenne twister to generate a random number
double RandomNum() {
    //set `static` to avoid generating same results
	static std::random_device Rand;
	static std::mt19937_64 RanEngine(Rand());
	static std::uniform_real_distribution<double> Distribution(0.0, 1.0);
	return Distribution(RanEngine);
}

/*
* Compute the specular reflection.
*
* If the first layer is a turbid medium, use the Fresnel reflection from the boundary of the first layer as the
* specular reflectance.
*
* If the first layer is glass, multiple reflections in the first layer is considered to get the specular reflectance.
*
* The subroutine assumes the Layerspecs array is correctly initialized.
*/
double Rspecular(const QVector<LayerClass>& LayerVec)
{
	/* direct reflections from the 1st and 2nd layers. */
	double temp = (LayerVec[0].layer->rfct_index - LayerVec[1].layer->rfct_index)
		/ (LayerVec[0].layer->rfct_index + LayerVec[1].layer->rfct_index);
	double r1 = temp*temp;
	if ((LayerVec[1].layer->abs_coef == 0.0) && (LayerVec[1].layer->scat_coef == 0.0)) { /* glass layer. */
		temp = (LayerVec[1].layer->rfct_index - LayerVec[2].layer->rfct_index)
			/ (LayerVec[1].layer->rfct_index + LayerVec[2].layer->rfct_index);
		double r2 = temp*temp;
		r1 = r1 + (1 - r1)*(1 - r1)*r2 / (1 - r1*r2);
	}
	return r1;
}

/*
* Choose (sample) a new theta angle for photon propagation according to the anisotropy.
*
* If anisotropy g is 0, then cos(theta) = 2*rand-1.
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
* Make sure that the cosine of the incident angle a1 is positive, and the case when the angle is greater
* than the critical angle is ruled out.
*
* Avoid trigonometric function operations as much as possible, because they are computation-intensive.
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
		double sa1 = sqrt(1 - ca1*ca1);
		double sa2 = n1*sa1 / n2;
		/* sine of the incident and transmission angles. */
		double ca2;
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



