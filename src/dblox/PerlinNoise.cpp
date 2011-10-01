#include "dblox/PerlinNoise.h"

PerlinNoise::PerlinNoise( unsigned int Seed )
    : m_Seed(Seed)
{
}

PerlinNoise::~PerlinNoise()
{
}

double PerlinNoise::Noise2D( double X, double Y )
{
    double FloorX = (double)((int)X);
    double FloorY = (double)((int)Y);

    double S, T, U, V;
    S = Random2D(FloorX, FloorY); 
    T = Random2D(FloorX + 1, FloorY);
    U = Random2D(FloorX, FloorY + 1);
    V = Random2D(FloorX + 1, FloorY + 1);

    return Interpolate(Interpolate(S, T, X - FloorX), Interpolate(U, V, X - FloorX), Y - FloorY);
}

double PerlinNoise::SmoothNoise2D( double X, double Y, double Persistance, int Octaves )
{
    double Noise = 0.0;
    for( int i = 0; i < Octaves - 1; ++i )
    {
        double Frequency = pow(2, (double)i);
        double Amplitude = pow(Persistance, (double)i);

        Noise += Noise2D(X * Frequency, Y * Frequency) * Amplitude;
    }

    return Noise;
}
