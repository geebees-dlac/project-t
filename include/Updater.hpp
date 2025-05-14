//interpolation

#ifndef INTERPOLATE_HPP
#define INTERPOLATE_HPP

/////////////////////////////////
//c++ port of Penner easing
//equations specifically with
//SFML in mind
// remember gian and zeann for this and use it as guide
//Adapted from c++ port by Jesus Gollonet
//http://www.jesusgollonet.com/blog/2007/09/24/penner-easing-cpp
//t: start time
//b: starting value being interpolated
//c: change in value
//d: duration
/////////////////////////////////

#include <math.h>
#include <iostream>

namespace math { namespace interpolate {
    // Dummy implementations for compilation if not fully provided
    inline float quadraticEaseInOut(float t, float b, float c, float d) {
        t /= d / 2;
        if (t < 1) return c / 2 * t * t + b;
        t--;
        return -c / 2 * (t * (t - 2) - 1) + b;
    }
    inline float sineEaseIn(float t, float b, float c, float d) {
        // Simplified version, actual SFML easing functions or your own might be different
        if (t >= d) return b + c;
        return c * (1 - std::cos(t / d * (3.14159f / 2))) + b;
    }
}}


/*namespace math
{
	#define PI 3.14

	class interpolate
	{
	public:
		interpolate();
		~interpolate();

		static float linear(float t, float b, float c, float d);
		static float expoEaseIn(float t, float b, float c, float d);
		static float expoEaseOut(float t, float b, float c, float d);
		static float expoEaseInOut(float t, float b, float c, float d);
		static float cubicEaseIn(float t, float b, float c, float d);
		static float cubicEaseOut(float t, float b, float c, float d);
		static float cubicEaseInOut(float t, float b, float c, float d);
		static float quarticEaseIn(float t, float b, float c, float d);
		static float quarticEaseOut(float t, float b, float c, float d);
		static float quarticEaseInOut(float t, float b, float c, float d);
		static float quinticEaseIn(float t, float b, float c, float d);
		static float quinticEaseOut(float t, float b, float c, float d);
		static float quinticEaseInOut(float t, float b, float c, float d);
		static float quadraticEaseIn(float t, float b, float c, float d);
		static float quadraticEaseOut(float t, float b, float c, float d);
		static float quadraticEaseInOut(float t, float b, float c, float d);
		static float sineEaseIn(float t, float b, float c, float d);
		static float sineEaseOut(float t, float b, float c, float d);
		static float sineEaseInOut(float t, float b, float c, float d);
		static float circularEaseIn(float t, float b, float c, float d);
		static float circularEaseOut(float t, float b, float c, float d);
		static float circularEaseInOut(float t, float b, float c, float d);
		static float backEaseIn(float t, float b, float c, float d);
		static float backEaseOut(float t, float b, float c, float d);
		static float backEaseInOut(float t, float b, float c, float d);
		static float elasticEaseIn(float t, float b, float c, float d);
		static float elasticEaseOut(float t, float b, float c, float d);
		static float elasticEaseInOut(float t, float b, float c, float d);
	};
} */
#endif // INTERPOLATE_HPP