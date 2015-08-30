#include <cmath>

void WavelengthToRGB(double Wavelength, int *r, int *g, int *b){

    double Gamma = 0.80,
    IntensityMax = 255,
    factor,
    Red,Green,Blue;
Wavelength=trunc(Wavelength);
    if((Wavelength >= 380) && (Wavelength<440)){
        Red = -(Wavelength - 440) / (440 - 380);
        Green = 0.0;
        Blue = 1.0;
    }else if((Wavelength >= 440) && (Wavelength<490)){
        Red = 0.0;
        Green = (Wavelength - 440) / (490 - 440);
        Blue = 1.0;
    }else if((Wavelength >= 490) && (Wavelength<510)){
        Red = 0.0;
        Green = 1.0;
        Blue = -(Wavelength - 510) / (510 - 490);
    }else if((Wavelength >= 510) && (Wavelength<580)){
        Red = (Wavelength - 510) / (580 - 510);
        Green = 1.0;
        Blue = 0.0;
    }else if((Wavelength >= 580) && (Wavelength<645)){
        Red = 1.0;
        Green = -(Wavelength - 645) / (645 - 580);
        Blue = 0.0;
    }else if((Wavelength >= 645) && (Wavelength<781)){
        Red = 1.0;
        Green = 0.0;
        Blue = 0.0;
    }else{
        Red = 0.0;
        Green = 0.0;
        Blue = 0.0;
    };

    // Let the intensity fall off near the vision limits
    if((Wavelength >= 380) && (Wavelength<420)){
        factor = 0.3 + 0.7*(Wavelength - 380) / (420 - 380);
    }else if((Wavelength >= 420) && (Wavelength<701)){
        factor = 1.0;
    }else if((Wavelength >= 701) && (Wavelength<781)){
        factor = 0.3 + 0.7*(780 - Wavelength) / (780 - 700);
    }else{
        factor = 0.0;
    };

    // Don't want 0^x = 1 for x <> 0
    Red = Red==0.0 ? 0 : (int) round(IntensityMax * pow(Red * factor, Gamma));
    Green = Green==0.0 ? 0 : (int) round(IntensityMax * pow(Green * factor, Gamma));
    Blue = Blue==0.0 ? 0 : (int) round(IntensityMax * pow(Blue * factor, Gamma));


    if((Wavelength < 380) || (Wavelength>781)){
        Red = 255;
        Green = 255;
        Blue = 255;
    }

    *r=Red;
    *g=Green;
    *b=Blue;
}
