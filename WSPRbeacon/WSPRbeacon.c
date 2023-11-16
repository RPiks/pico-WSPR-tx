#include "WSPRbeacon.h"

#include <WSPRutility.h>

/// @brief Initializes a new WSPR beacon context.
/// @param pcallsign HAM radio callsign, 12 chr max.
/// @param pgridsquare Maidenhead locator, 7 chr max.
/// @param txpow_dbm TX power, db`mW.
/// @param pdco Ptr to working DCO.
/// @return the context.
WSPRbeaconContext *WSPRbeaconInit(const char *pcallsign, const char *pgridsquare, int txpow_dbm,
                                  PioDco *pdco)
{
    assert_(pdco);

    WSPRbeaconContext *p = calloc(1, sizeof(WSPRbeaconContext));
    assert_(p);

    strncpy(p->_pu8_callsign, pcallsign, sizeof(p->_pu8_callsign));
    strncpy(p->_pu8_locator, pgridsquare, sizeof(p->_pu8_locator));
    p->_u8_txpower = txpow_dbm;

    p->_pTX = TxChannelInit(682667, 0, pdco);
    assert_(p->_pTX);

    return p;
}

/// @brief Sets dial (baseband minima) freq.
/// @param pctx Context.
/// @param freq_hz the freq., Hz.
void WSPRbeaconSetDialFreq(WSPRbeaconContext *pctx, uint32_t freq_hz)
{
    assert_(pctx);
    pctx->_pTX->_u32_dialfreqhz = freq_hz;
}

/// @brief Constructs a new WSPR packet using the data available.
/// @param pctx Context
/// @return 0 if OK.
int WSPRbeaconCreatePacket(WSPRbeaconContext *pctx)
{
    assert_(pctx);

    wspr_encode(pctx->_pu8_callsign, pctx->_pu8_locator, pctx->_u8_txpower, pctx->_pu8_outbuf);

    return 0;
}

/// @brief Sends a prepared WSPR packet using TxChannel.
/// @param pctx Context.
/// @return 0, if OK.
int WSPRbeaconSendPacket(const WSPRbeaconContext *pctx)
{
    assert_(pctx);
    assert_(pctx->_pTX);
    assert_(pctx->_pTX->_u32_dialfreqhz > 1100 * kHz);

    memcpy(pctx->_pTX->_pbyte_buffer, pctx->_pu8_outbuf, WSPR_SYMBOL_COUNT);
    pctx->_pTX->_ix_input += WSPR_SYMBOL_COUNT;

    return 0;
}
