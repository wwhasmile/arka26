#ifndef HAS_MATHS_INTERP_H
#define HAS_MATHS_INTERP_H

#include <maths/maths_funcs.h>

typedef enum {
    	INTERPOLATION_LINEAR,
	INTERPOLATION_EASE_IN_QUAD,
	INTERPOLATION_EASE_OUT_QUAD,
	INTERPOLATION_EASE_IN_OUT_QUAD,
	INTERPOLATION_EASE_IN_CUBIC,
	INTERPOLATION_EASE_OUT_CUBIC,
	INTERPOLATION_EASE_IN_OUT_CUBIC,
	INTERPOLATION_EASE_IN_EXP,
	INTERPOLATION_EASE_OUT_EXP,
	INTERPOLATION_EASE_IN_OUT_EXP,
	INTERPOLATION_EASE_IN_BACK,
	INTERPOLATION_EASE_OUT_BACK,
	INTERPOLATION_EASE_IN_OUT_BACK,
	INTERPOLATION_EASE_IN_ELASTIC,
	INTERPOLATION_EASE_OUT_ELASTIC,
	INTERPOLATION_EASE_IN_OUT_ELASTIC,
	INTERPOLATION_EASE_IN_BOUNCE,
	INTERPOLATION_EASE_OUT_BOUNCE,
	INTERPOLATION_EASE_IN_OUT_BOUNCE,
    	INTERPOLATION_ENUM_COUNT
} Interpolation;

// Formulas from: https://easings.net/
static inline f32 Interpolation_Calculate(f32 t, Interpolation interp)
{
	switch (interp) {
	case INTERPOLATION_LINEAR: return t;
	case INTERPOLATION_EASE_IN_QUAD: return t * t;
	case INTERPOLATION_EASE_OUT_QUAD: return 1.0f - (1.0f - t) * (1.0f - t);
	case INTERPOLATION_EASE_IN_OUT_QUAD:
			return t < 0.5f ?
			2.0f * t * t :
			1.0f - (-2.0f * t + 2.0f) * (-2.0f * t + 2.0f) * 0.5f;
	case INTERPOLATION_EASE_IN_CUBIC: return t * t * t;
	case INTERPOLATION_EASE_OUT_CUBIC: return 1.0f - (1.0f - t) * (1.0f - t) * (1.0f - t);
	case INTERPOLATION_EASE_IN_OUT_CUBIC:
			return t < 0.5f ?
			4.0f * t * t * t :
			1.0f - (-2.0f * t + 2.0f) * (-2.0f * t + 2.0f) * (-2.0f * t + 2.0f) * 0.5f;
	case INTERPOLATION_EASE_IN_EXP: return t == 0.0f ? 0.0f : Math_Pow(2.0f, 10.0f * t - 10.0f);
	case INTERPOLATION_EASE_OUT_EXP: return t == 1.0f ? 1.0f : 1.0f - Math_Pow(2.0f, -10.0f * t);
	case INTERPOLATION_EASE_IN_OUT_EXP: {
			if (t == 0.0f) return 0.0f;
			if (t == 1.0f) return 1.0f;
			return t < 0.5f ?
			Math_Pow(2.0f, 20.0f * t - 10.0f) * 0.5f :
			(2.0f - Math_Pow(2.0f, -20.0f * t + 10.0f)) * 0.5f;
	}
	case INTERPOLATION_EASE_IN_BACK: {
			f32 c1 = 1.70158f;
			f32 c3 = c1 + 1.0f;
			return c3 * t * t * t - c1 * t * t;
	}
	case INTERPOLATION_EASE_OUT_BACK: {
			f32 c1 = 1.70158f;
			f32 c3 = c1 + 1.0f;
			return 1.0f + c3 * (t - 1.0f) * (t - 1.0f) * (t - 1.0f) + c1 * (t - 1.0f) * (t - 1.0f);
	}
	case INTERPOLATION_EASE_IN_OUT_BACK: {
			f32 c1 = 1.70158f;
			f32 c2 = c1 * 1.525f;
			return t < 0.5f
			? ((2.0f * t) * (2.0f * t) * ((c2 + 1.0f) * 2.0f * t - c2)) * 0.5f
			: ((2.0f * t - 2.0f) * (2.0f * t - 2.0f) * ((c2 + 1.0f) * (t * 2.0f - 2.0f) + c2) + 2.0f) * 0.5f;
	}
	case INTERPOLATION_EASE_IN_ELASTIC: {
			f32 c4 = (2.0f * MATH_PI) / 3.0f;
			if (t == 0.0f) return 0.0f;
			if (t == 1.0f) return 1.0f;
			return -Math_Pow(2.0f, 10.0f * t - 10.0f) * Math_Sin((t * 10.0f - 10.75f) * c4);
	}
	case INTERPOLATION_EASE_OUT_ELASTIC: {
			f32 c4 = (2.0f * MATH_PI) / 3.0f;
			if (t == 0.0f) return 0.0f;
			if (t == 1.0f) return 1.0f;
			return Math_Pow(2.0f, -10.0f * t) * Math_Sin((t * 10.0f - 0.75f) * c4) + 1.0f;
	}
	case INTERPOLATION_EASE_IN_OUT_ELASTIC: {
			f32 c5 = (2.0f * MATH_PI) / 4.5f;
			if (t == 0.0f) return 0.0f;
			if (t == 1.0f) return 1.0f;
			return t < 0.5f
			? -Math_Pow(2.0f, 20.0f * t - 10.0f) * Math_Sin((20.0f * t - 11.125f) * c5) * 0.5f
			: Math_Pow(2.0f, -20.0f * t + 10.0f) * Math_Sin((20.0f * t - 11.125f) * c5) * 0.5f + 1.0f;
	}
	case INTERPOLATION_EASE_IN_BOUNCE:
		return 1.0f - Interpolation_Calculate(1.0f - t, INTERPOLATION_EASE_OUT_BOUNCE);
	case INTERPOLATION_EASE_OUT_BOUNCE: {
			f32 n1 = 7.5625f;
			f32 d1 = 2.75f;
			if (t < 1.0f / d1) return n1 * t * t;
			if (t < 2.0f / d1) {
			t -= 1.5f / d1;
			return n1 * t * t + 0.75f;
			}
			if (t < 2.5f / d1) {
			t -= 2.25f / d1;
			return n1 * t * t + 0.9375f;
			}
			t -= 2.625f / d1;
			return n1 * t * t + 0.984375f;
	}
	case INTERPOLATION_EASE_IN_OUT_BOUNCE:
			return t < 0.5f
			? (1.0f - Interpolation_Calculate(1.0f - 2.0f * t, INTERPOLATION_EASE_OUT_BOUNCE)) * 0.5f
			: (1.0f + Interpolation_Calculate(2.0f * t - 1.0f, INTERPOLATION_EASE_OUT_BOUNCE)) * 0.5f;
	default: return t;
	}
}

#endif // HAS_MATHS_INTERP_H
