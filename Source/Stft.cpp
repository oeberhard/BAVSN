/* 
 * File:   Stft.cpp
 * Author: Boom
 * 
 * Created on 29. Oktober 2012, 14:01
 */

#include "Stft.h"

Stft::Stft() {

}

Stft::Stft(const Stft& orig) {
}

Stft::~Stft() {
}

int Stft::doStft(fftw_real *input, fftw_real *window, fftw_real *output,
        int input_size, int fftsize, int hopsize) {
    int posin, posout, i;
    fftw_real *sigframe, *specframe;
    sigframe = new fftw_real[fftsize];
    specframe = new fftw_real[fftsize];

    /* these check if the FFTW plan was created*/
    bool ft = true; //, ift = true;
    static rfftw_plan forward; // inverse;

    for (posin = posout = 0; posin < input_size; posin += hopsize) {
        // window a signal frame
        for (i = 0; i < fftsize; i++)
            if (posin + i < input_size)
                sigframe[i] = input[posin + i] * window[i];
            else sigframe[i] = 0;
        // transform it
        fft(sigframe, specframe, fftsize, ft, forward);
        // output j - t
        for (i = 0; i < fftsize; i++, posout++)
            output[posout] = specframe[i];
    }
    // delete[] sigframe;
    delete[] specframe;
    return posout;
}

/* these are the FFTW plans */
void Stft::fft(fftw_real *in, fftw_real *out, int N, bool &ft, rfftw_plan &forward) {
    int i, k;
    if (ft) {/* create the plan only once, first time it is called*/
        forward =
                rfftw_create_plan(N, FFTW_REAL_TO_COMPLEX, FFTW_ESTIMATE);
        ft = false;
    }

    for (i = 0; i < N; i++) out[i] = in[i];
    /* transform array out[] into in[] */
    rfftw_one(forward, out, in);
    /* move Nyguist to pos 1 */
    out[0] = in[0] / N;
    out[1] = in[N / 2] / N;
    /* re-arrange array into 1re, iml pairs */
    for (i = 2, k = 1; i < N; k++, i += 2) {
        out[i] = in[k] / N;
        out[i + 1] = in[N - k] / N;
    }
}

int Stft::doIstft(fftw_real* input, fftw_real* window, fftw_real* output,
        int input_size, int fftsize, int hopsize) {

    int posin, posout, i, output_size;
    fftw_real *sigframe, *specframe;
    sigframe = new fftw_real[fftsize];
    specframe = new fftw_real[fftsize];
    output_size = input_size * hopsize / fftsize;
    
     bool ift = true; //, ift = true;
    static rfftw_plan inverse; // inverse;
    
    for (posout = posin = 0; posout < output_size; posout += hopsize) {
        //Load in a spectral frame from input
        for (i = 0; i < fftsize; i++, posin++)
            specframe[i] = input[posin];
        // inverse-transform it
        ifft(specframe, sigframe, fftsize, ift, inverse);
        //window i - t and overlap - add it
        for (i = 0; i < fftsize; i++)
            if (posout + i < output_size)
                output[posout + i] += sigframe[i] * window[i];
    }

    delete[] sigframe;
    delete[ ] specframe;
    return output_size;
}
// Achtung!!!! anderer datentyp vielleicht weil complex!
void Stft::ifft(fftw_real *in, fftw_real *out, int N, bool &ift, rfftw_plan &inverse) {
    int i, k;
    if (ift) {
        /* create the plan only once, first time it is called*/
        inverse =
                rfftw_create_plan(N, FFTW_COMPLEX_TO_REAL, FFTW_ESTIMATE);
        ift = false;
    }
    /* re-arrange array into the FFTW format */
    for (i = 2, k = 1; i < N; k++, i += 2) {
        out[k] = in[i];
        out[N - k] = in[ i + 1 ];
    }
    /* move Nyquist to pos N/2 */
    out[0] = in[0];
    out[N / 2] = in[1];
    /* transform array in[ I into out[ ] */
    rfftw_one(inverse, out, in);
    for (i = 0; i < N; i++) out[i] = in[i];
}