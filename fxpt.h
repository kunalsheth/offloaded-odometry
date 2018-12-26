#define fxpt_t int64_t
#define fxpt_shift 15

#define i_to_fx(X) ((fxpt_t)X << fxpt_shift)
#define fx_to_i(X) (X >> fxpt_shift)

#define fl_to_fx(X) ((fxpt_t)(X * (1L << fxpt_shift)))
#define fx_to_fl(X) ((double)X / (1L << fxpt_shift))

#define fxpt_two_pi fl_to_fx(TWO_PI)
#define fxpt_three_half_pi fl_to_fx(3*HALF_PI)
#define fxpt_pi fl_to_fx(PI)
#define fxpt_half_pi fl_to_fx(HALF_PI)
#define fxpt_zero fl_to_fx(0)
