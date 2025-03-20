/*
 * W E B T I M E . C
 *
 * webtime.c last edited on Thu Mar 20 10:12:50 2025 by ofh
 *
 * Fetch the time from a WWW server by requesting a HEAD 
 * response. The date and time in a HEAD response should
 * be the UTC time.
 * 
 * descendant of getWebTime 0v6
 *
 * download and compile with the following steps; -
 *  git clone https://github.com/dutchLuck/webtime
 *  cd webtime
 *  ./configure
 *  make clean
 *  make
 *  make check
 *
 * The verbosity level has the following effects; -
 *  -v 0 .. Output only the date and time 
 *  -v 1 or no -v option specified .. Default verbosity level. It labels the date string.
 *  -v 2 .. Adds the server name into the output string provided by -v1.
 *  -v 3 .. Displays the request lines sent to the server and the reply lines from the server.
 *  -v 4 .. Adds a count of the number of characters in each server reply line.
 *
 * Known problems yet to be fixed; -
 * 1. No automatic recovery from lack of server response
 * 
 */

#include  <stdio.h>			/* printf() sprintf() */
#include  <string.h>		/* strdup() strcasestr() */
#include  <sys/time.h>		/* gettimeofday() */
#include  <libgen.h>		/* basename() */
#include  <time.h>			/* asctime() */
#include  "sockhelp.h"
#include  "config.h"		/* struct config */

#ifndef  FALSE
#define  FALSE  ((int)0)
#endif
#ifndef  TRUE
#define  TRUE  (! FALSE)
#endif

#define  VERSION_INFO "0v2"
#define  HEADER  "webtime"
#define  BFR_SIZE  1024

char *  exePath = NULL;
char *  exeName = NULL;

void  cleanupStorage( void );
void  setExecutableName( char *  argv[] );
int  processNonSwitchCommandLineParameters( struct config *  cfg, int  frstIndx, int  lstIndx, char *  cmdLnStrngs[] );


int  main( int  argc, char *  argv[] )  {
	struct config  config;
	int indexToFirstNonConfig;
	int index;
	
	/* Ensure any allocated memory is free'd */
	atexit( cleanupStorage );
	/* setup the name of this program */
	setExecutableName( argv );
	/* set defaults for all configuration options */
	initConfiguration( &config );
	/* set any configuration options that have been specified in the command line */
	indexToFirstNonConfig = setConfiguration( argc, argv, &config );
	/* if -D (debug mode) then show the state of the configuration options */
	if ( config.D.active )  configuration_status( &config );
	/* if -V, -v[34] or -D then show version information */
	if ( config.V.active || ( config.v.optionInt > 2 ) || config.D.active )
		printf( "%s version %s\n", exeName, VERSION_INFO );
	/* if -h (help) specified then show the help/usage information but don't finish */
	if ( config.h.active )  usage( &config, exeName );
	/* if -D (debug mode) then show the positional command line arguments */
	if ( config.D.active )  {
		for ( index = indexToFirstNonConfig; index < argc; index++)
			printf ( "Debug: Non-option argument ( %d ): \"%s\"\n", index, argv[index]);
	}
	/*  If verbosity level is 4 then output more info  */
	if ( config.D.active || ( config.v.active && ( config.v.optionInt > 3 )))  {
		printf( "%s %s\n", HEADER, VERSION_INFO );
		printf( "source file %s, compiled on %s at %s\n",
			__FILE__, __DATE__, __TIME__ );
	}
	/* Attempt to get time from server or servers and return 0 if successful */
	return( processNonSwitchCommandLineParameters( &config, indexToFirstNonConfig, argc - 1, argv ));
}


void  cleanupStorage( void )  {
	if( exePath != NULL )  free(( void *) exePath );
}


void  setExecutableName( char *  argv[] )  {
/* Isolate the name of the executable */
  if(( exePath = strdup( argv[0] )) == NULL )
    perror( "Warning: Unable to create duplicate of path to this executable" );
  else if(( exeName = basename( exePath )) == NULL )  {
    perror( "Warning: Unable to obtain the name of this executable" );
  }
  if ( exeName == NULL )  exeName = argv[ 0 ];
}


void  sendString( struct config *  cfg, int sckt, char *  str )  {
	if( cfg->v.optionInt > 2 )  printf( "%s", str );
	sock_puts( sckt, str );
}


int  processA_SingleCommandLineParameter( struct config *  cfg, char *  nameStrng )  {
	char  out[ BFR_SIZE ];
	char *  localTimeStr;
	char *  serverTimeStr;
	int  result;
	int  socket;
	int  count = 0;
	struct timeval  now;		/* current time on local computer */

	if( cfg->D.active )  {
    	printf( "Executing: processA_SingleCommandLineParameter( %s )\n", nameStrng );
	}
	sprintf( out, "%d", cfg->p.optionInt );		/* put port number into out string */
  	/* socket = make_connection( out, SOCK_STREAM, nameStrng, cfg->v.active ); */
	if ( cfg->i.optionInt == 4 )  count = 4;		/* Try to connnect with IP version 4 */
	else if ( cfg->i.optionInt == 6 )  count = 6;	/* Try to connect with IP version 6 */
	else  count = 0;	/* Don't care whether it is version 4 or 6 */
	socket = establish_sock_stream_connection ( nameStrng, out, count, ( cfg->v.optionInt > 1 ));
	if (( result = ( socket == -1 )))
		fprintf( stderr, "Error: unable to connect to \"%s:%d\"\n", nameStrng, cfg->p.optionInt );
	else  {
		if( cfg->g.active )  sprintf( out, "GET http://%s/ HTTP/1.0\r\n", nameStrng );
		else  sprintf( out, "HEAD http://%s/ HTTP/1.0\r\n", nameStrng );
		sendString( cfg, socket, out );		/* send first line with either a HEAD or GET */
		sendString( cfg, socket, "Pragma: no-cache\r\n" );
		sendString( cfg, socket, "User-Agent: webtime/0.10 [en]\r\n" );
		sprintf( out, "Host: %s\r\n", nameStrng );
		sendString( cfg, socket, out );
		sendString( cfg, socket, "Accept: text/html, text/plain\r\n" );
		sendString( cfg, socket, "Accept-Language: en\r\n" );
		sendString( cfg, socket, "Accept-Charset: iso-8859-1,*,utf-8\r\n" );
		sendString( cfg, socket, "\r\n" );
		if ( cfg->l.active && ( gettimeofday( &now, NULL ) == 0 ))		/* get local date and time if required */
			localTimeStr = strdup( asctime( gmtime( &now.tv_sec )));
		while(( count = sock_gets( socket, out, BFR_SIZE - 1 )) > -1 )  {
			if ( count >= BFR_SIZE )  out[ BFR_SIZE - 1 ] = '\0';		/* ensure terminated string */
			else if ( count > 0 )  out[ count ] = '\0';
			else  out[ 0 ] = '\0';
			if ( cfg->D.active || ( cfg->v.optionInt > 2 ))  {		/* if debug or verbose then output the reply from the server */
				if ( cfg->v.optionInt > 3 )  printf( "%05d: ", count );		/* start with line length */
				printf( "%s\n", out );
			}
			if ( strcasestr( out, "date: " ) != NULL )  {
				serverTimeStr = strdup( out );		/* capture server date and time */
			}
		}
		close( socket );
		if ( cfg->l.active )  {		/* print local date/time */
			if ( localTimeStr != NULL )  {
				if ( cfg->v.optionInt > 0 )  printf( "Local Host GMTime: " );
				printf( "%s", localTimeStr );
				free( localTimeStr );
				if ( cfg->v.optionInt > 0 )  printf( "Server " );
			}
		}
		if ( serverTimeStr != NULL )  {
			if ( cfg->v.optionInt > 1 )  printf( "\"%s\": ", nameStrng );	/* show the server name */
			printf( "%s\n", serverTimeStr + (( cfg->v.optionInt < 1 ) ? 6 : 0 ));	/* print server date/time */
			free( serverTimeStr );
		}
	}
	return( result );
}


int  processNonSwitchCommandLineParameters( struct config *  cfg, int  frstIndx, int  lstIndx, char *  cmdLnStrngs[] )  {
  int  result, indx;

  result = 0;
  if( cfg->D.active )  {
    printf( "Executing: processNonSwitchCommandLineParameters()\n" );
    printf( "first index is %d and last index is %d\n", frstIndx, lstIndx );
    for( indx = frstIndx; indx <= lstIndx; indx++ )
      printf( "cmdLnStrngs[ %d ] string is '%s'\n", indx, cmdLnStrngs[ indx ] );
  }
  for( indx = frstIndx; indx <= lstIndx; indx++ )
    result |= processA_SingleCommandLineParameter( cfg, cmdLnStrngs[ indx ] );
  return( result );
}
