#include "batches.h"
#include <string>

// random initialization of sequences etc.

namespace {

// very simple "random" number generator; this
// is just used for initializations

double state = getenv("seed")?atof(getenv("seed")):0.1;

inline double randu() {
  state = 189843.9384938 * state + 0.328340981343;
  state -= floor(state);
  return state;
}

inline double randn() {
  double u1 = randu();
  double u2 = randu();
  double r = -2*log(u1);
  double theta = 2*M_PI*u2;
  double z0 = r * cos(theta);
  return z0;
}

}

namespace ocropus {

// Random initializations with different distributions.

void rinit(TensorMap2 m, Float s, const char *mode_, Float offset) {
  std::string mode(mode_);
  if (mode == "unif") {
    for(int i=0; i<rows(m); i++)
      for(int j=0;j<cols(m); j++)
        m(i,j) = 2 * s * randu() - s + offset;
  } else if (mode == "negbiased") {
    for(int i=0; i<rows(m); i++)
      for(int j=0;j<cols(m); j++)
        m(i,j) = 3 * s * randu() - 2 * s + offset;
  } else if (mode == "pos") {
    for(int i=0; i<rows(m); i++)
      for(int j=0;j<cols(m); j++)
        m(i,j) = s * randu() + offset;
  } else if (mode == "neg") {
    for(int i=0; i<rows(m); i++)
      for(int j=0;j<cols(m); j++)
        m(i,j) = - s * randu() + offset;
  } else if (mode == "normal") {
    for(int i=0; i<rows(m); i++)
      for(int j=0;j<cols(m); j++)
        m(i,j) = s * randn() + offset;
  }
}

void rinit(Params &m, int r, int c, Float s, const char *mode, Float offset) {
  m.resize(r,c);
  rinit(m.v(), s, mode, offset);
}

void rinit(Batch &m, int r, int c, Float s, const char *mode, Float offset) {
  m.resize(r,c);
  rinit(m.v(), s, mode, offset);
}

void rinit(Sequence &m, int N, int r, int c, Float s, const char *mode, Float offset) {
  m.resize(N,r,c);
  for(int t=0; t<N; t++)
    rinit(m[t].v(), s, mode, offset);
}


// checking for NaNs in different objects

bool anynan(TensorMap2 a) {
  for (int j = 0; j < rows(a); j++) {
    for (int k = 0; k < cols(a); k++) {
      float x = a(j, k);
      if (isnan(x)) return true;
    }
  }
  return false;
}

bool anynan(Batch &a) { return anynan(a.v()) || anynan(a.d()); }

bool anynan(Sequence &a) {
  for (int i = 0; i < a.size(); i++)
    if (anynan(a[i])) return true;
  return false;
}
}