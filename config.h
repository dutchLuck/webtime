/*
 * C O N F I G . H
 *
 * Last Modified on Mon Mar 10 15:08:59 2025
 *
 */

#include <stdio.h>

#ifndef  CONFIG_H
#define  CONFIG_H

#ifndef FALSE
#define FALSE 0
#endif
#ifndef TRUE
#define TRUE (!(FALSE))
#endif

struct positionParam {
  char *  paramNameStr;
  char *  helpStr;
};

struct optFlg {
  int active;
  char *  optID;
  char *  helpStr;
};

struct optChr {
  int active;
  char *  optID;
  char *  helpStr;
  int  optionChr;
};

struct optStr {
  int active;
  char *  optID;
  char *  helpStr;
  char *  optionStr;
};

struct optInt {
  int active;
  char *  optID;
  char *  helpStr;
  int defaultVal;
  int mostPosLimit;
  int mostNegLimit;
  int optionInt;
};

struct optLng {
  int active;
  char *  optID;
  char *  helpStr;
  long defaultVal;
  long mostPosLimit;
  long mostNegLimit;
  long optionLng;
};

struct optDbl {
  int active;
  char *  optID;
  char *  helpStr;
  double defaultVal;
  double mostPosLimit;
  double mostNegLimit;
  double optionDbl;
};

// Command Line Options Configuration Data
struct config {
  struct positionParam posParam1;  /* (posParam1) Mandatory Name or IP Address of a web server to query */
  struct optFlg D;  /* (debug) ...... enable debug output mode */
  struct optFlg g;  /* (get) ...... switch from head request to get request */
  struct optFlg h;  /* (help) ...... this help / usage information */
  struct optFlg l;  /* (localtime) ...... enable local time information output */
  struct optInt p;  /* (port) INT .. set port number to INT - where 0 <= INT <= 65535 */
  struct optInt v;  /* (verbose) INT .. verbosity level set to INT - where 0 <= INT <= 4 */
  struct optFlg V;  /* (version) ...... enable version information output */
};

// getopt() option string
#define OPTIONS ":Dghlp:v:V"

void  usage ( struct config *  optStructPtr, char *  exeName );
void  initConfiguration ( struct config *  optStructPtr );
int  setConfiguration ( int  argc, char *  argv[], struct config *  optStructPtr );
void  configuration_status( struct config *  opt );

#endif
