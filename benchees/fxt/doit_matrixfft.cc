#define NAME "fxt-matrixfft"
#define NOTES "Four-step algorithm, uses FHT for sub-FFTs, split real/imag format."

#define NO_Complex 1

#define DOIT_FFT matrix_fft(inr, ini, m, is)

#include "doit.cc"
