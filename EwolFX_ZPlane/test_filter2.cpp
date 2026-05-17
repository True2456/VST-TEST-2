#include <iostream>
#include <cmath>

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

struct Biquad {
  double b0 = 0, b1 = 0, b2 = 0;
  double a1 = 0, a2 = 0;
  double x1 = 0, x2 = 0;
  double y1 = 0, y2 = 0;
  void Reset() { x1 = x2 = y1 = y2 = 0.0; }
};

class ZPlaneFilter {
public:
  void Reset(double sampleRate) {
    mSampleRate = sampleRate;
    mLowpass.Reset();
    mPhaser.Reset();
    mFormant.Reset();
    mBiquad.Reset();
  }
  void SetCutoff(double cutoffHz) { mCutoff = cutoffHz; }
  void SetResonance(double res) { mResonance = res; }
  void SetMorph(double morph) { mMorph = morph; }

  double Process(double input) {
    ComputeCoefficients();
    double output = mBiquad.b0 * input + mBiquad.b1 * mBiquad.x1 +
                    mBiquad.b2 * mBiquad.x2 - mBiquad.a1 * mBiquad.y1 -
                    mBiquad.a2 * mBiquad.y2;
    mBiquad.x2 = mBiquad.x1;
    mBiquad.x1 = input;
    mBiquad.y2 = mBiquad.y1;
    mBiquad.y1 = output;
    return output;
  }

private:
  double mSampleRate = 44100.0;
  double mCutoff = 1000.0;
  double mResonance = 0.5;
  double mMorph = 0.0;
  Biquad mBiquad, mLowpass, mPhaser, mFormant;

  void ComputeCoefficients() {
    double cutoff = mCutoff;
    double res = mResonance;
    if (cutoff < 10.0) cutoff = 10.0;
    if (cutoff > mSampleRate * 0.49) cutoff = mSampleRate * 0.49;
    if (res < 0.0) res = 0.0;
    if (res > 0.99) res = 0.99;

    double wc = 2.0 * M_PI * cutoff / mSampleRate;
    double cosWc = cos(wc);
    double sinWc = sin(wc);

    double Q = 0.5 / (1.0 - res);
    double alpha = sinWc / (2.0 * Q);
    double a0 = 1.0 + alpha;

    mLowpass.b0 = ((1.0 - cosWc) / 2.0) / a0;
    mLowpass.b1 = (1.0 - cosWc) / a0;
    mLowpass.b2 = ((1.0 - cosWc) / 2.0) / a0;
    mLowpass.a1 = (-2.0 * cosWc) / a0;
    mLowpass.a2 = (1.0 - alpha) / a0;

    mPhaser.b0 = 1.0 / a0;
    mPhaser.b1 = (-2.0 * cosWc) / a0;
    mPhaser.b2 = 1.0 / a0;
    mPhaser.a1 = (-2.0 * cosWc) / a0;
    mPhaser.a2 = (1.0 - alpha) / a0;

    double Qformant = Q * 2.0; 
    double alphaF = sinWc / (2.0 * Qformant);
    double a0F = 1.0 + alphaF;

    mFormant.b0 = alphaF / a0F;
    mFormant.b1 = 0.0;
    mFormant.b2 = -alphaF / a0F;
    mFormant.a1 = (-2.0 * cosWc) / a0F;
    mFormant.a2 = (1.0 - alphaF) / a0F;

    double morph = mMorph;
    if (morph <= 0.5) {
      double t = morph * 2.0; 
      double it = 1.0 - t;
      mBiquad.b0 = it * mLowpass.b0 + t * mPhaser.b0;
      mBiquad.b1 = it * mLowpass.b1 + t * mPhaser.b1;
      mBiquad.b2 = it * mLowpass.b2 + t * mPhaser.b2;
      mBiquad.a1 = it * mLowpass.a1 + t * mPhaser.a1;
      mBiquad.a2 = it * mLowpass.a2 + t * mPhaser.a2;
    } else {
      double t = (morph - 0.5) * 2.0; 
      double it = 1.0 - t;
      mBiquad.b0 = it * mPhaser.b0 + t * mFormant.b0;
      mBiquad.b1 = it * mPhaser.b1 + t * mFormant.b1;
      mBiquad.b2 = it * mPhaser.b2 + t * mFormant.b2;
      mBiquad.a1 = it * mPhaser.a1 + t * mFormant.a1;
      mBiquad.a2 = it * mPhaser.a2 + t * mFormant.a2;
    }
  }
};

int main() {
    ZPlaneFilter f;
    f.Reset(44100.0);
    f.SetResonance(0.99);
    f.SetMorph(1.0); // Formant is most unstable usually
    
    double val = 1.0;
    for(double c = 20.0; c <= 20000.0; c *= 1.0001) {
        f.SetCutoff(c);
        val = f.Process(1.0);
        if (std::isnan(val) || std::isinf(val)) {
            std::cout << "FAILED AT CUTOFF " << c << "!\n";
            return 1;
        }
        if (std::abs(val) > 1000000.0) {
             std::cout << "EXPLODED AT CUTOFF " << c << ": " << val << "\n";
             return 1;
        }
    }
    std::cout << "SWEEP SUCCESS: " << val << "\n";
    return 0;
}
