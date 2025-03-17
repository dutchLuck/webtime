/*
 * C O N F I G . C
 *
 * Last Modified on Wed Jul 31 13:25:15 2024
 *
 */

#include <stdio.h>    /* printf() */
#include <stdlib.h>   /* floating point double */
#include <unistd.h>   /* getopt() */
#include <ctype.h>    /* isprint()*/
#include <limits.h>   /* INT_MIN INT_MAX LONG_MIN LONG_MAX */
#include <float.h>    /* DBL_MIN DBL_MAX */
#include <string.h>   /* strchr() */
#include "config.h"   /* struct config */
#include "genFun.h"   /* convertOptionStringToInteger() limitIntegerValueToEqualOrWithinRange() */


int  configureIntegerOption( struct optInt *  intStructPtr, char *  intString ) {
  intStructPtr->active = TRUE;
  intStructPtr->optionInt = convertOptionStringToInteger( intStructPtr->defaultVal, intString,
    intStructPtr->optID, &intStructPtr->active, TRUE );
  intStructPtr->optionInt = limitIntegerValueToEqualOrWithinRange( intStructPtr->optionInt,
    intStructPtr->mostNegLimit, intStructPtr->mostPosLimit );
  return( intStructPtr->active );
}


int  configureLongOption( struct optLng *  longStructPtr, char *  longString ) {
  longStructPtr->active = TRUE;
  longStructPtr->optionLng = convertOptionStringToLong( longStructPtr->defaultVal, longString,
    longStructPtr->optID, &longStructPtr->active, TRUE );
  longStructPtr->optionLng = limitLongValueToEqualOrWithinRange( longStructPtr->optionLng,
    longStructPtr->mostNegLimit, longStructPtr->mostPosLimit );
  return( longStructPtr->active );
}


int  configureDoubleOption( struct optDbl *  dblStructPtr, char *  dblString ) {
  dblStructPtr->active = TRUE;
  dblStructPtr->optionDbl = convertOptionStringToDouble( dblStructPtr->defaultVal, dblString,
    dblStructPtr->optID, &dblStructPtr->active, TRUE );
  dblStructPtr->optionDbl = limitDoubleValueToEqualOrWithinRange( dblStructPtr->optionDbl,
    dblStructPtr->mostNegLimit, dblStructPtr->mostPosLimit );
  return( dblStructPtr->active );
}


// Functions for Command Line Options Configuration from JSON Data
void  usage( struct config *  opt, char *  exeName )  {
  printf( "Usage:\n");
  printf( " %s [-D][-g][-h][-i INT][-l][-p INT][-v INT][-V] SERVER_1 [.. [SERVER_N]]\n", exeName );
  printf( " %s %s\n", opt->D.optID, opt->D.helpStr ); /* debug */
  printf( " %s %s\n", opt->g.optID, opt->g.helpStr ); /* get */
  printf( " %s %s\n", opt->h.optID, opt->h.helpStr ); /* help */
  printf( " %s %s\n", opt->i.optID, opt->i.helpStr ); /* inet4 */
  printf( " %s %s\n", opt->l.optID, opt->l.helpStr ); /* localtime */
  printf( " %s %s\n", opt->p.optID, opt->p.helpStr ); /* port */
  printf( " %s %s\n", opt->v.optID, opt->v.helpStr ); /* verbose */
  printf( " %s %s\n", opt->V.optID, opt->V.helpStr ); /* version */
  printf( " %s %s\n", "SERVER_1 [.. [SERVER_N]]", "Mandatory Name or IP Address of a web server to query" ); /* posParam1 */
}

void  initConfiguration ( struct config *  opt )  {
// posParam1: positionParam
  opt->posParam1.paramNameStr = "SERVER_1 [.. [SERVER_N]]";
  opt->posParam1.helpStr = "Mandatory Name or IP Address of a web server to query";
// debug: optFlg
  opt->D.active = FALSE;
  opt->D.optID = "-D";
  opt->D.helpStr = "...... enable debug output mode";
// get: optFlg
  opt->g.active = FALSE;
  opt->g.optID = "-g";
  opt->g.helpStr = "...... switch from head request to get request";
// help: optFlg
  opt->h.active = FALSE;
  opt->h.optID = "-h";
  opt->h.helpStr = "...... this help / usage information";
// inet4: optInt
  opt->i.active = FALSE;
  opt->i.optID = "-i";
  opt->i.helpStr = "INT .. set IP version to 4, 6 or 46 - where 0 <= INT <= 64";
  opt->i.mostPosLimit = 64;
  opt->i.mostNegLimit = 0;
  opt->i.optionInt = 0;
  opt->i.defaultVal = 0;
// localtime: optFlg
  opt->l.active = FALSE;
  opt->l.optID = "-l";
  opt->l.helpStr = "...... enable local time information output";
// port: optInt
  opt->p.active = FALSE;
  opt->p.optID = "-p";
  opt->p.helpStr = "INT .. set port number to INT - where 0 <= INT <= 65535";
  opt->p.mostPosLimit = 65535;
  opt->p.mostNegLimit = 0;
  opt->p.optionInt = 80;
  opt->p.defaultVal = 80;
// verbose: optInt
  opt->v.active = FALSE;
  opt->v.optID = "-v";
  opt->v.helpStr = "INT .. verbosity level set to INT - where 0 <= INT <= 4";
  opt->v.mostPosLimit = 4;
  opt->v.mostNegLimit = 0;
  opt->v.optionInt = 1;
  opt->v.defaultVal = 1;
// version: optFlg
  opt->V.active = FALSE;
  opt->V.optID = "-V";
  opt->V.helpStr = "...... enable version information output";
}

int  setConfiguration ( int  argc, char *  argv[], struct config *  opt )  {
  int c;

  opterr = 0;
  while ((c = getopt (argc, argv, OPTIONS )) != -1 ) {
    switch ( c ) {
      case 'D': opt->D.active = TRUE; break; /* debug */
      case 'g': opt->g.active = TRUE; break; /* get */
      case 'h': opt->h.active = TRUE; break; /* help */
      case 'i': configureIntegerOption( &opt->i, optarg ); break; /* inet */
      case 'l': opt->l.active = TRUE; break; /* localtime */
      case 'p': configureIntegerOption( &opt->p, optarg ); break; /* port */
      case 'v': configureIntegerOption( &opt->v, optarg ); break; /* verbose */
      case 'V': opt->V.active = TRUE; break; /* version */
      case '?' : {
        if ( strchr( "ipv", optopt ) != NULL ) {
          fprintf (stderr, "Error: Option -%c requires an argument.\n", optopt);
          switch ( optopt ) {
            case 'i': opt->i.active = FALSE; break;
            case 'p': opt->p.active = FALSE; break;
            case 'v': opt->v.active = FALSE; break;
          }
        }
        else if (isprint (optopt))
          fprintf (stderr, "Warning: Unknown option \"-%c\" ? - ignoring it!\n", optopt);
        else
          fprintf (stderr, "Warning: Unknown non-printable option character 0x%x ? - ignoring it!\n", optopt);
        break;
      }
    }
  }
  return( optind );
}

void  configuration_status( struct config *  opt )  {
  printf( "Debug: option -D is %sctive (-D %s)\n", (opt->D.active) ? "a" : "ina", opt->D.helpStr); /* debug */
  printf( "Debug: option -g is %sctive (-g %s)\n", (opt->g.active) ? "a" : "ina", opt->g.helpStr); /* get */
  printf( "Debug: option -h is %sctive (-h %s)\n", (opt->h.active) ? "a" : "ina", opt->h.helpStr); /* help */
  printf( "Debug: option -i is %sctive (-i %s)\n", (opt->i.active) ? "a" : "ina", opt->i.helpStr); /* inet4 */
  printf( "Debug: option -i value is %d, limits: %d .. %d\n", opt->i.optionInt, opt->i.mostNegLimit, opt->i.mostPosLimit); /* inet4 */
  printf( "Debug: option -l is %sctive (-l %s)\n", (opt->l.active) ? "a" : "ina", opt->l.helpStr); /* localtime */
  printf( "Debug: option -p is %sctive (-p %s)\n", (opt->p.active) ? "a" : "ina", opt->p.helpStr); /* port */
  printf( "Debug: option -p value is %d, limits: %d .. %d\n", opt->p.optionInt, opt->p.mostNegLimit, opt->p.mostPosLimit); /* port */
  printf( "Debug: option -v is %sctive (-v %s)\n", (opt->v.active) ? "a" : "ina", opt->v.helpStr); /* verbose */
  printf( "Debug: option -v value is %d, limits: %d .. %d\n", opt->v.optionInt, opt->v.mostNegLimit, opt->v.mostPosLimit); /* verbose */
  printf( "Debug: option -V is %sctive (-V %s)\n", (opt->V.active) ? "a" : "ina", opt->V.helpStr); /* version */
  printf( "Debug: %s (%s)\n", opt->posParam1.paramNameStr, opt->posParam1.helpStr);
}
