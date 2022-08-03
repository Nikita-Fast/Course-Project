#include "firfilter.h"
#include <algorithm>
//получаем указатель на начало динамического массива, где лежит импульсная хар-ка
FIRFilter::FIRFilter(double imp_resp[], int len) : length(len)
{
    h = new double[length];
    h = imp_resp;
    delay_line = new short[length - 1];
    std::fill(delay_line, delay_line + length, 0);
}

void FIRFilter::filter(short* input, int input_length, short* output)
{
//    std::fill(output, output + input_length, 0); //надо ли занулять?
    for (int i = 0; i < input_length; i++) {
        output[i] = convolve(input, i);
    }
    update_delay_line(input, input_length);
}

short FIRFilter::convolve(short* input, int pos)
{
    double s = 0.0;
    for (int i = 0; i < length; i++) {
        s += get_sample(input, pos - i) * h[i];
    }
    return (short)s;
}

short FIRFilter::get_sample(short *input, int index)
{
    if (index < 0) {
        return delay_line[index + length - 1];
    }
    else {
        return input[index];
    }
}

void FIRFilter::update_delay_line(short *input, int input_length)
{
    for (int i = 0; i < length - 1; i++) {
        delay_line[i] = input[input_length - length + 1 + i];
    }
}
