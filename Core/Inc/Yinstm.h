#include "main.h"
#include "definitions.h"
#include <math.h>
#include "OLED_driver.h"
#include "ctrldFM.h"
#include "arm_math.h"



const char notes [][4] = {"B 0",
		"C 1",
		"C#1",
		"D 1",
		"D#1",
		"E 1",
		"F 1",
		"F#1",
		"G 1",
		"G#1",
		"A 1",
		"A#1",
		"B 1",
		"C 2",
		"C#2",
		"D 2",
		"D#2",
		"E 2",
		"F 2",
		"F#2",
		"G 2",
		"G#2",
		"A 2",
		"A#2",
		"B 2",
		"C 3",
		"C#3",
		"D 3",
		"D#3",
		"E 3",
		"F 3",
		"F#3",
		"G 3",
		"G#3",
		"A 3",
		"A#3",
		"B 3",
		"C 4",
		"C#4",
		"D 4",
		"D#4",
		"E 4",
		"F 4",
		"F#4",
		"G 4",
		"G#4",
		"A 4",
		"A#4",
		"B 4",
		"C 5",
		"C#5",
		"D 5",
		"D#5",
		"E 5",
		"F 5",
		"F#5",
		"G 5",
		"G#5",
		"A 5",
		"A#5",
		"B 5",
		"C 6",
		"C#6",
		"D 6",
		"D#6",
		"E 6",
		"F 6",
		"F#6",
		"G 6",
		"G#6",
		"A 6",
		"A#6",
		"B 6",
		"C 7",
		"C#7",
		"D 7",
		"D#7",
		"E 7",
		"F 7",
		"F#7",
		"G 7",
		"G#7",
		"A 7",
		"A#7",
		"B 7"};
void Yin_Diff(const uint8_t* src, float* dest){

	for(int delay = 0; delay < W_SIZE; delay++){
		
		float sum = 0.0f;
		for(int i = 0; i < W_SIZE; i++){
			float delta = src[i]-src[i+delay];
			sum += delta * delta;
     		}

		dest[delay] = sum;
     	}
}

uint32_t crude(float* yin, uint16_t size){
	uint32_t delta = 10;

	for (uint16_t i = delta; i < 250; i++){
		if (yin[i] < yin[delta])
			delta = i;
	}

	return delta;
}
void Yin_FFT(float* src, float* power, float* dest, uint16_t destSize, arm_rfft_fast_instance_f32* fft_instance ){
	arm_rfft_fast_f32(fft_instance, src, dest, 0);
	arm_cmplx_mag_f32(dest, power, destSize);
	for (int i = 0; i < destSize; i++){
		power[i] *= power[i];
	}
	arm_rfft_fast_f32(fft_instance, power, dest, 1);
	
	for (int i = 0; i < destSize; i++)
	{
		dest[i] = (dest[0]*2) - (2*dest[i]);
	}

	dest[0] = 0.0;
}
void adcToFloat(uint8_t* src, float* dest, uint16_t destSize){
	
	for(int i = 0; i < destSize; i++){
	dest[i]= (float)src[i];
	}

}
void Yin_CMNDF(float* dest){
	float sum = 0.0f;

	dest[0] = 1;

	for (int delay = 1; delay < W_SIZE; delay++){
		sum += dest[delay];
		dest[delay] *= delay/sum;
	}
}


uint32_t Yin_AbsThresh(float *yin, float thresh, float* probability){

	uint32_t tau;
	for (int i = 2; i < W_SIZE; i++){
		if(yin[i] < 0.1) {
	*probability = 1 - yin[i];
	tau = i;
	i = W_SIZE;	
//			while (((i + 1) < W_SIZE) && (yin[i+1] < yin[i]))
//	  			i++;
      		}

	
	}
	//if (tau == W_SIZE || yin[tau] >= thresh){
	//	tau = -1;
	//	*probability = 0;
	//}
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

void printPitch(uint8_t* buffer, size_t size, float freq){
	float pitch = 12*log2(freq/440.0);
	unsigned int target = roundf((pitch + 46));

	printText(12, 32, 16, notes[target], FONT ,buffer);

}
