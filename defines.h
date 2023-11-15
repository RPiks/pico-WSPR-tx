#ifndef DEFINESWSPR_H
#define DEFINESWSPR_H

#define DEBUG

#ifdef DEBUG
#define DEBUGPRINTF(x) StampPrintf(x);
#else
#define DEBUGPRINTF(x) { }
#endif

#endif
