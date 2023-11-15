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
    uint32_t _u32_dialfreqhz;

} WSPRbeaconContext;

WSPRbeaconContext *WSPRbeaconInit(const char *pcallsign, const char *pgridsquare, int txpow_dbm,
                                  void *pfsk4modulator);
void WSPRbeaconSetDialFreq(WSPRbeaconContext *pctx, uint32_t freq_hz);
int WSPRbeaconCreatePacket(WSPRbeaconContext *pctx);
int WSPRbeaconSendPacket(const WSPRbeaconContext *pctx);

//int WSPRbeaconFSK4mod(uint8_t bits_per_sample, uint8_t sample_val);

#endif
