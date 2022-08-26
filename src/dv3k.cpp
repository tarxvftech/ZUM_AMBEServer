#include <stdint.h>

/*
   AMBE-3000R codec communications:
       Seems to be meant for embedded analog audio in. Not relevant for AMBEServer, then.

   AMBE-3000R packet communications:
       Header
           START byte (magic 0x61)
           LENGTH uint16 (does not include START, LENGTH, TYPE, but does include parity (if used)
           TYPE byte (three values - control/config 0x00, channel data 0x01, speech data 0x02).
               In documentation, DVSI uses "speech" data to refer to PCM samples,
               and "channel" data to refer to encoded bits. In other words, you get
               speech data from a microphone, encode it to channel data, decode the
               channel data back to speech and play that speech data over a speaker.

               Note that AMBE-3000 supports a single channel (i.e. one stream at
               a time). AMBE3003 appears to support three simultaneous channels,
               with obvious utility for transcoding.
       Data 
           control/config 0x00:
               reply is given to every packet. Reply is usually just the control field ID plus a 0x00 to indicate no errors.
               See full documentation for details
       Footer (parity field) (optional, but required by default)
           Parity magic (indicates parity field) 0x2f
           Parity byte (xor all bytes of packet EXCEPT for START BYTE 0x61 and PARITY BYTE 0x2f together)
*/

typedef struct _DV3_HDR {
    uint8_t  magic; //must be 0x61
    uint16_t pktlength;
    uint8_t  type;
} dv3_hdr;

typedef struct _DV3_FTR {
    uint8_t magic; //must be 0x2f
    uint8_t parity;
} dv3_ftr;


uint8_t dv3_calc_parity( uint8_t * pkt ){
    //requires packet to be _fully_ filled out
    //if you run this on a packet with parity field provided, it is a good packet if it returns zero
    //if you run it on a packet with a zeroed parity field, it will return the parity value to store there
    
    dv3_hdr * hdr = (dv3_hdr *) pkt;
    int parity_len = hdr.pktlength + 4; //pktlength ignores header (len=4) but we need whole packet for calculating parity
    //parity is _all_ bytes of a packet except START_BYTE 0x61 and
    //PARITY_BYTE 0x2f. We're just going to iterate over all bytes and
    //undor the START_BYTE and PARITY_BYTE to keep it simple
    uint8_t acc = 0;
    for( int i = 0; i < len+4; i++ ){
        acc ^= pkt+i;
    }
    acc ^= 0x2f ^ 0x61;  //un-xor the magic bytes
    return acc;
}
void dv3_calc_hdr( uint8_t * pkt ){
    dv3_hdr * hdr = (dv3_hdr *) pkt;
    hdr.magic = 0x61;
}
void dv3_calc_ftr( uint8_t * pkt ){
    dv3_hdr * hdr = (dv3_hdr *) pkt;
    int ftr_offset = hdr.pktlength+4-2; //4 for the header size, 2 for the footer size
    dv3_ftr * ftr = (dv3_ftr *) (pkt+ftr_offset);
    ftr.magic = 0x2f;
    ftr.parity = 0; //clean slate for setting parity
    ftr.parity = dv3_calc_parity(pkt);
}
void dv3_calc( uint8_t * pkt ){
    //pktlength gets set outside beforehand
    //type gets set outside beforehand
    dv3_calc_hdr( pkt );
    dv3_calc_ftr( pkt );
}

enum dv3k_commands { 
    //sorted to match order from documentation, please do not change
    PKT_CHANNEL0  = 0x40,
    PKT_ECMODE    = 0x05,
    PKT_DCMODE    = 0x06,
    PKT_COMPAND   = 0x32,
    PKT_RATET     = 0x09,
    PKT_RATEP     = 0x0A,
    PKT_INIT      = 0x0B,
    PKT_LOWPOWER  = 0x10,
    PKT_CODECCFG  = 0x38,
    PKT_CODECSTART= 0x2A,
    PKT_CODECSTOP = 0x2B,
    PKT_CHANFMT   = 0x15,
    PKT_SPCHFMT   = 0x16,
    PKT_PRODID    = 0x30,
    PKT_VERSTRING = 0x31,
    PKT_READY     = 0x39,
    PKT_HALT      = 0x35,
    PKT_RESET     = 0x33,
    PKT_RESETSOFTCFG = 0x34,
    PKT_GETCFG    = 0x36,
    PKT_READCFG   = 0x37,
    PKT_PARITYMODE= 0x3f,
    PKT_WRITEI2C  = 0x44,
    PKT_CLRCODECRESET = 0x46,
    PKT_SETCODECRESET = 0x47,
    PKT_DISCARDCODEC = 0x48,
    PKT_DELAYNUS  = 0x49,
    PKT_DELAYNNS  = 0x4A,
    PKT_RTSTHRESH = 0x4E,
    PKT_GAIN      = 0x4B,
};
enum directions {
    DIR_IN = 1,
    DIR_OUT = 2
};
#define DIR_BOTH ((enum directions) (DIR_IN & DIR_OUT))
typedef struct _dv3_cmd {
    char name[20];
    enum dv3k_commands code;
    int32_t cmddatalen;   //-1 if varies
    int32_t replydatalen; //-1 if varies
    enum directions supported_directions;
    char description[64];
} dv3_cmd;

dv3_cmd dv3_cmd_table[] = {
    //matches order from documentation again, including the column order
    
    //type codes are also in the enum, since we'll need to
    //refer to those by name in the code later without a lookup cost at runtime
    
    {"PKT_CHANNEL0",      PKT_CHANNEL0,   0, 0, DIR_BOTH, "Select first channel (only channel for AMBE3000 chips)"},
    {"PKT_ECMODE",        PKT_ECMODE,     2, 0, DIR_BOTH, "Encode flags (current channel)"},
    {"PKT_DCMODE",        PKT_DCMODE,     2, 0, DIR_BOTH, "Decoder flags"},
    {"PKT_COMPAND",       PKT_COMPAND,    1, 0, DIR_BOTH, "Companding and a-law/u-law"},
    {"PKT_RATET",         PKT_RATET,      1, 0, DIR_BOTH, "Select rate from table (current channel)"},
    {"PKT_RATEP",         PKT_RATEP,     12, 0, DIR_BOTH, "select custom rate (current channel)"},
    {"PKT_INIT",          PKT_INIT,       1, 0, DIR_BOTH, "Initialize encoder/decoder (current channel)"},
    {"PKT_LOWPOWER",      PKT_LOWPOWER,   1, 0, DIR_BOTH, "enable or disable low power mode"},
    {"PKT_CODECCFG",      PKT_CODECCFG,  -1, 0, DIR_BOTH, ""},
    {"PKT_CODECSTART",    PKT_CODECSTART, 1, 0, DIR_BOTH, ""},
    {"PKT_CODECSTOP",     PKT_CODECSTOP,  0, 0, DIR_BOTH, ""},
    {"PKT_CHANFMT",       PKT_CHANFMT,    2, 0, DIR_BOTH, "Sets channel format (encoded bits)"},
    {"PKT_SPCHFMT",       PKT_SPCHFMT,    2, 0, DIR_BOTH, "sets speech format (decoded/not-encoded audio)"},
    {"PKT_PRODID",        PKT_PRODID,     0,-1, DIR_BOTH, "Query for Product ID"},
    {"PKT_VERSTRING",     PKT_VERSTRING,  0,48, DIR_BOTH, "Query for Version String"},
    {"PKT_READY",         PKT_READY,      0, 0,  DIR_OUT, "Chip is ready to receive packets"},
    {"PKT_HALT",          PKT_HALT,       0, 0,   DIR_IN, "Ultra low power mode"},
    {"PKT_RESET",         PKT_RESET,      0, 0,   DIR_IN, "Reset to configuration specified with hardware pins"},
    {"PKT_RESETSOFTCFG",  PKT_RESETSOFTCFG, 6,0,  DIR_IN, "Reset to software configuration"},
    {"PKT_GETCFG",        PKT_GETCFG,     0, 3, DIR_BOTH, ""},
    {"PKT_READCFG",       PKT_READCFG,    0, 3, DIR_BOTH, ""},
    {"PKT_PARITYMODE",    PKT_PARITYMODE, 1, 0, DIR_BOTH, ""},
    {"PKT_WRITEI2C",      PKT_WRITEI2C,  -1, 0, DIR_BOTH, ""},
    {"PKT_CLRCODECRESET", PKT_CLRCODECRESET, 0,0, DIR_BOTH, ""},
    {"PKT_SETCODECRESET", PKT_SETCODECRESET, 0,0, DIR_BOTH, ""},
    {"PKT_DISCARDCODEC",  PKT_DISCARDCODEC, 2,0,DIR_BOTH, ""},
    {"PKT_DELAYNUS",      PKT_DELAYNUS,   2, 0, DIR_BOTH, ""},
    {"PKT_DELAYNNS",      PKT_DELAYNNS,   2, 0, DIR_BOTH, ""},
    {"PKT_RTSTHRESH",     PKT_RTSTHRESH,  5, 0, DIR_BOTH, ""},
    {"PKT_GAIN",          PKT_GAIN,       2, 0, DIR_BOTH, ""}
};

int8_t dv3_get_cmd_pkt_entry_idx( enum dv3k_commands type ){
    int entries = sizeof(dv3_cmd_table)/sizeof(dv3_cmd);
    for( int i = 0; i < entries; i++){
        if( dv3_cmd_table[i].code == type ){
            return i;
        }
    }
    return -1;
}
uint16_t dv3_pkt_length( enum dv3k_commands type, enum directions dir, bool with_parity){
    int idx = dv3_get_cmd_pkt_entry_idx( type );
    int sz = dir == DIR_IN ? dv3_cmd_table[idx].cmddatalen : dv3_cmd_table[idx].replydatalen;
    if( sz == -1 ){
        return -1; //encodes variable length
    }
    return sz + with_parity? 2:0;
}


