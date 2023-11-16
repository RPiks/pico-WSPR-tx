#ifndef WSPRBEACON_H_
#define WSPRBEACON_H_

#include <stdint.h>
#include <string.h>
#include <TxChannel.h>

typedef struct
{
    uint8_t _pu8_callsign[12];
    uint8_t _pu8_locator[7];
    uint8_t _u8_txpower;

    uint8_t _pu8_outbuf[256];

    TxChannelContext *_pTX;

} WSPRbeaconContext;

WSPRbeaconContext *WSPRbeaconInit(const char *pcallsign, const char *pgridsquare, int txpow_dbm,
                                  PioDco *pdco);
void WSPRbeaconSetDialFreq(WSPRbeaconContext *pctx, uint32_t freq_hz);
int WSPRbeaconCreatePacket(WSPRbeaconContext *pctx);
int WSPRbeaconSendPacket(const WSPRbeaconContext *pctx);

#endif
