/* ------------------------------------------------------------
author: "Jean Pierre Cimalando"
license: "BSD-2-Clause"
name: "sfz_filters"
Code generated with Faust 2.20.2 (https://faust.grame.fr)
Compilation options: -lang cpp -inpl -double -ftz 0
------------------------------------------------------------ */

#ifndef  __faustHpf4p_H__
#define  __faustHpf4p_H__

#ifndef FAUSTFLOAT
#define FAUSTFLOAT float
#endif

#include <algorithm>
#include <cmath>
#include <math.h>


#ifndef FAUSTCLASS
#define FAUSTCLASS faustHpf4p
#endif

#ifdef __APPLE__
#define exp10f __exp10f
#define exp10 __exp10
#endif

class faustHpf4p : public sfzFilterDsp {

 public:

	int fSampleRate;
	double fConst0;
	double fConst1;
	double fConst2;
	FAUSTFLOAT fCutoff;
	FAUSTFLOAT fQ;
	double fRec0[2];
	double fRec3[2];
	double fRec4[2];
	double fRec2[3];
	double fRec5[2];
	double fRec1[3];

 public:

	void metadata(Meta* m) {
	}

	virtual int getNumInputs() {
		return 1;
	}
	virtual int getNumOutputs() {
		return 1;
	}
	virtual int getInputRate(int channel) {
		int rate;
		switch ((channel)) {
			case 0: {
				rate = 1;
				break;
			}
			default: {
				rate = -1;
				break;
			}
		}
		return rate;
	}
	virtual int getOutputRate(int channel) {
		int rate;
		switch ((channel)) {
			case 0: {
				rate = 1;
				break;
			}
			default: {
				rate = -1;
				break;
			}
		}
		return rate;
	}

	static void classInit(int sample_rate) {
	}

	virtual void instanceConstants(int sample_rate) {
		fSampleRate = sample_rate;
		fConst0 = std::min<double>(192000.0, std::max<double>(1.0, double(fSampleRate)));
		fConst1 = std::exp((0.0 - (1000.0 / fConst0)));
		fConst2 = (6.2831853071795862 / fConst0);
	}

	virtual void instanceResetUserInterface() {
		fCutoff = FAUSTFLOAT(440.0);
		fQ = FAUSTFLOAT(0.0);
	}

	virtual void instanceClear() {
		for (int l0 = 0; (l0 < 2); l0 = (l0 + 1)) {
			fRec0[l0] = 0.0;
		}
		for (int l1 = 0; (l1 < 2); l1 = (l1 + 1)) {
			fRec3[l1] = 0.0;
		}
		for (int l2 = 0; (l2 < 2); l2 = (l2 + 1)) {
			fRec4[l2] = 0.0;
		}
		for (int l3 = 0; (l3 < 3); l3 = (l3 + 1)) {
			fRec2[l3] = 0.0;
		}
		for (int l4 = 0; (l4 < 2); l4 = (l4 + 1)) {
			fRec5[l4] = 0.0;
		}
		for (int l5 = 0; (l5 < 3); l5 = (l5 + 1)) {
			fRec1[l5] = 0.0;
		}
	}

	virtual void init(int sample_rate) {
		classInit(sample_rate);
		instanceInit(sample_rate);
	}
	virtual void instanceInit(int sample_rate) {
		instanceConstants(sample_rate);
		instanceResetUserInterface();
		instanceClear();
	}

	virtual faustHpf4p* clone() {
		return new faustHpf4p();
	}

	virtual int getSampleRate() {
		return fSampleRate;
	}

	virtual void buildUserInterface(UI* ui_interface) {
	}

	virtual void compute(int count, FAUSTFLOAT** inputs, FAUSTFLOAT** outputs) {
		FAUSTFLOAT* input0 = inputs[0];
		FAUSTFLOAT* output0 = outputs[0];
		double fSlow0 = (fSmoothEnable ? fConst1 : 0.0);
		double fSlow1 = (fConst2 * std::max<double>(0.0, double(fCutoff)));
		double fSlow2 = std::cos(fSlow1);
		double fSlow3 = (0.5 * (std::sin(fSlow1) / std::max<double>(0.001, std::pow(10.0, (0.050000000000000003 * double(fQ))))));
		double fSlow4 = (fSlow3 + 1.0);
		double fSlow5 = (1.0 - fSlow0);
		double fSlow6 = (((-1.0 - fSlow2) / fSlow4) * fSlow5);
		double fSlow7 = (((0.0 - (2.0 * fSlow2)) / fSlow4) * fSlow5);
		double fSlow8 = (((1.0 - fSlow3) / fSlow4) * fSlow5);
		double fSlow9 = ((0.5 * ((fSlow2 + 1.0) / fSlow4)) * fSlow5);
		for (int i = 0; (i < count); i = (i + 1)) {
			double fTemp0 = double(input0[i]);
			fRec0[0] = ((fSlow0 * fRec0[1]) + fSlow6);
			fRec3[0] = ((fSlow0 * fRec3[1]) + fSlow7);
			fRec4[0] = ((fSlow0 * fRec4[1]) + fSlow8);
			fRec2[0] = (fTemp0 - ((fRec3[0] * fRec2[1]) + (fRec4[0] * fRec2[2])));
			fRec5[0] = ((fSlow0 * fRec5[1]) + fSlow9);
			fRec1[0] = (((fRec0[0] * fRec2[1]) + (fRec5[0] * (fRec2[0] + fRec2[2]))) - ((fRec3[0] * fRec1[1]) + (fRec4[0] * fRec1[2])));
			output0[i] = FAUSTFLOAT(((fRec0[0] * fRec1[1]) + (fRec5[0] * (fRec1[0] + fRec1[2]))));
			fRec0[1] = fRec0[0];
			fRec3[1] = fRec3[0];
			fRec4[1] = fRec4[0];
			fRec2[2] = fRec2[1];
			fRec2[1] = fRec2[0];
			fRec5[1] = fRec5[0];
			fRec1[2] = fRec1[1];
			fRec1[1] = fRec1[0];
		}
	}

};

#endif
