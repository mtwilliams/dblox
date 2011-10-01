#ifndef DBLOX_PERLIN_NOISE_H
#define DBLOX_PERLIN_NOISE_H

#include <math.h>

class PerlinNoise
{
    public:
        PerlinNoise( unsigned int Seed );
        ~PerlinNoise();

        double Noise2D( double X, double Y );
        double SmoothNoise2D( double X, double Y, double Persistance, int Octaves );

    protected:
        inline double Random2D( double X, double Y )
        {
            int N = (int)X + (int)Y * m_Seed;
            N = (N << 13) ^ N;
            return 1.0 - ((double)((N * (N * N * 60493 + 19990303) + 1376312589) & 0x7fffffff) / 1073741824.0);
        }

        inline double Interpolate( double A, double B, double X )
        {
            double F = (1.0 - cos(X * 3.1415927)) * 0.5;
            return A * (1.0 - F) + B * F;
        }

    protected:
        unsigned int m_Seed;
};

#endif