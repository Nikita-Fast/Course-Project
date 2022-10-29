#ifndef FIRFILTER_H
#define FIRFILTER_H


class FIRFilter
{
public:
    FIRFilter(double imp_resp[], int len);
    void filter(short* input, int length, short* output);

private:
    const int length;
    double *h;
    short *delay_line;

    short convolve(short *input, int pos);
    short get_sample(short *input, int pos);
    void update_delay_line(short* input, int input_length);
};

#endif // FIRFILTER_H
