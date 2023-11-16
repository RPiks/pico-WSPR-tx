
#include <stdint.h>

#include "pico-hf-oscillator/lib/assert.h"
#include <piodco.h>
#include "defines.h"

extern PioDco DCO;

/// @brief The code of dedicated core' program running HF oscillator.
void Core1Entry()
{
    const uint32_t clkhz = PLL_SYS_MHZ * MHz;
    const uint32_t freq_hz = WSPR_DIAL_FREQ_HZ + WSPR_SHIFT_FREQ_HZ;

    /* Initialize DCO */
    assert_(0 == PioDCOInit(&DCO, 6, clkhz));

    /* Run DCO. */
    PioDCOStart(&DCO);

    /* Set initial freq. */
    assert_(0 == PioDCOSetFreq(&DCO, freq_hz, 0u));

    /* Run the main DCO algorithm. It spins forever. */
    PioDCOWorker(&DCO);
}
