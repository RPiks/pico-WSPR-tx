#include "WSPRbeacon.h"

#include <WSPRutility.h>

WSPRbeaconContext *WSPRbeaconInit(const char *pcallsign, const char *pgridsquare, int txpow_dbm,
                                  void *pfsk4modulator)
{
    WSPRbeaconContext *p = calloc(1, sizeof(WSPRbeaconContext));
    assert_(p);

    strncpy(p->_pu8_callsign, pcallsign, sizeof(p->_pu8_callsign));
    strncpy(p->_pu8_locator, pgridsquare, sizeof(p->_pu8_locator));
    p->_u8_txpower = txpow_dbm;

    p->_pTX = TxChannelInit(682667, 0, pfsk4modulator);
    assert_(p->_pTX);

    return p;
}

void WSPRbeaconSetDialFreq(WSPRbeaconContext *pctx, uint32_t freq_hz)
{
    assert_(pctx);
    pctx->_u32_dialfreqhz = freq_hz;
}

int WSPRbeaconCreatePacket(WSPRbeaconContext *pctx)
{
    assert_(pctx);

    wspr_encode(pctx->_pu8_callsign, pctx->_pu8_locator, pctx->_u8_txpower, pctx->_pu8_outbuf);
}
