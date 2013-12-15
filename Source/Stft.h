/* 
 * File:   Stft.h
 * Author: Boom
 *
 * Created on 29. Oktober 2012, 14:01
 */

#ifndef STFT_H
#define	STFT_H
#include "rfftw.h"

class Stft {
public:
    Stft();
    Stft(const Stft& orig);
    virtual ~Stft();
    int doStft(fftw_real *input, fftw_real *window, fftw_real *output,
            int input_size, int fftsize, int hopsize);
    int doIstft(fftw_real* input, fftw_real* window, fftw_real* output,
        int input_size, int fftsize, int hopsize);
private:
    void fft(fftw_real *in, fftw_real *out, int N, bool &ft, rfftw_plan &forward);
    void ifft(fftw_real *in, fftw_real *out, int N, bool &ift, rfftw_plan &inverse);
};

#endif	/* STFT_H */

