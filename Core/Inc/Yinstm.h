#include "main.h"
#include "definitions.h"


void Yin_Diff(int8_t* src, float* dest){
	float delta;

	for(int delay = 0; delay < W_SIZE; delay++){
		for(int i = 0; i < W_SIZE; i++){
			delta = src[i]-src[i+delay];
			dest[delay] += delta * delta;
     		}
     	}
}

void Yin_CMNDF(float* dest){
	float sum = 0;

	dest[0] = 1;

	for (int delay = 1; delay < W_SIZE; delay++){
		sum += dest[delay];
		dest[delay] *= delay/sum;
	}
}


uint16_t Yin_AbsThresh(float *yin, float thresh, float* probability){

	uint16_t tau;
	for (tau = 2; tau < W_SIZE; tau++){
		if(yin[tau] < thresh) {
			while ((tau + 1) < W_SIZE && yin[tau+1] < yin[tau])
	  			tau++;
      		}

	*probability = 1 - yin[tau]; 
	break;		
	}
	if (tau == W_SIZE || yin[tau] >= thresh){
		tau = -1;
		*probability = 0;
	}

	return tau;


}

//We are taking the index above and below the current index, making boundary checks
//So if we are at the bottom first, it picks between x0 and x2 based on what is smallest
//If we are at the top first, it pickes between x0 and x2 based on what is smallest
//Finally if we are in a normal position, we do the plynomial interpolation
float Yin_ParaInterp(float* yin, int16_t intTau){
	float tauInterp;
	int16_t x0;
	int16_t x2;

	if (intTau < 1)
		x0 = intTau;
	else
		x0 = intTau - 1;

	if (intTau + 1 < W_SIZE) 
		x2 = intTau + 1;
	else
		x2 = intTau;
	

	if (x0 == intTau){
		if (yin[intTau] <= yin[x2])
			tauInterp = intTau;
		else 
			tauInterp = x2;

	}
	else if (x2 == intTau){
		if (yin[intTau] <= yin[x0])
			tauInterp = intTau;
		else 
			tauInterp = x0;

	}
	else {
		float s0,s1,s2;
		s0 = yin[x0];
		s1 = yin[intTau];
		s2 = yin[x2];
		tauInterp = intTau + (s2 - s0)/(2*(2*s1 - s2 - s0));
	}

	return tauInterp;
}

	




