#include "ConvolutionKernel.h"

const ConvolutionKernel* ConvolutionKernel::AVERAGE = new ConvolutionKernel( 1, 1, 1, 1, 1, 1, 1, 1, 1 );
const ConvolutionKernel* ConvolutionKernel::GAUSSIAN = new ConvolutionKernel( 1, 2, 1, 2, 4, 2, 1, 2, 1 );
const ConvolutionKernel* ConvolutionKernel::LAPLACIAN4 = new ConvolutionKernel( 0, -1, 0, -1, 4, -1, 0, -1, 0 );
const ConvolutionKernel* ConvolutionKernel::LAPLACIAN8 = new ConvolutionKernel( -1, -1, -1, -1, 8, -1, -1, -1, -1 );
const ConvolutionKernel* ConvolutionKernel::HIGHPASS5 = new ConvolutionKernel( 0, -1, 0, -1, 5, -1, 0, -1, 0 );
const ConvolutionKernel* ConvolutionKernel::HIGHPASS9 = new ConvolutionKernel( -1, -1, -1, -1, 9, -1, -1, -1, -1 );