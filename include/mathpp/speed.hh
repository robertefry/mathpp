
#ifndef __HH_MPP_SPEED
#define __HH_MPP_SPEED

/* ************************************************************************** */
// Definitions
/* ************************************************************************** */

namespace mpp
{

    float fast_isqrt(float);

} /* namespace mpp */

/* ************************************************************************** */
// Implementation
/* ************************************************************************** */

namespace mpp
{

    float fast_isqrt(float y)
    {
        float x = y * 0.5f;

        long i = *(long*) &y;
        i = 0x5F3759DF - ( i >> 1 );
        y = *(float*) &i;

        float const threehalfs = 1.5f;
        y = y * (threehalfs - x*y*y);
        y = y * (threehalfs - x*y*y);

        return y;
    }

} // namespace mpp

#endif /* __HH_MPP_SPEED */
