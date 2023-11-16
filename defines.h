#ifndef DEFINESWSPR_H
#define DEFINESWSPR_H

#define DEBUG

#ifdef DEBUG
#define DEBUGPRINTF(x) StampPrintf(x);
#else
#define DEBUGPRINTF(x) { }
#endif

#define FALSE 0                                     /* Something is false. */
#define TRUE 1                                       /* Something is true. */
#define BAD 0                                         /* Something is bad. */
#define GOOD 1                                       /* Something is good. */
#define INVALID 0                                 /* Something is invalid. */
#define VALID 1                                     /* Something is valid. */
#define NO 0                                          /* The answer is no. */
#define YES 1                                        /* The answer is yes. */
#define OFF 0                                       /* Turn something off. */
#define ON 1                                         /* Turn something on. */

#define RAM __not_in_flash_func         /* Place time-critical func in RAM */
#define RAM_A __not_in_flash("A")        /* Place time-critical var in RAM */

     /* A macro for arithmetic right shifts, with casting of the argument. */
#define iSAR(arg, rcount) (((int32_t)(arg)) >> (rcount))

  /* A macro of multiplication guarantees of doing so using 1 ASM command. */
#define iMUL32ASM(a,b) __mul_instruction((int32_t)(a), (int32_t)(b))

                                          /* Performing the square by ASM. */
#define iSquare32ASM(x) (iMUL32ASM((x), (x))) 
                                                         /* Hardware defs. */
#define kHz 1000UL
#define MHz 1000000UL
#define PLL_SYS_MHZ 270UL
                                                             /* WSPR defs. */
#define WSPR_DIAL_FREQ_HZ       7040000UL
#define WSPR_SHIFT_FREQ_HZ      50UL
//#define WSPR_FREQ_STEP_MILHZ    1465UL
#define WSPR_FREQ_STEP_MILHZ    2930UL

#endif
