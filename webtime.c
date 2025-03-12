/*
 * W E B T I M E . C
 *
 * webtime.c last edited on Wed Mar 12 11:28:01 2025 by ofh
 *
 * Fetch the time from a WWW server
 * 
 * descendant of getWebTime 0v6
 *
 */

#include  <stdio.h>			/* printf() sprintf() */
#include  <string.h>		/* strstr() */
#include  <sys/time.h>		/* gettimeofday() */
#include  <libgen.h>		/* basename() */
#include  "sockhelp.h"
#include  "config.h"		/* struct config */

#ifndef  FALSE
#define  FALSE  ((int)0)
#endif
#ifndef  TRUE
#define  TRUE  (! FALSE)
#endif

#define  VERSION_INFO "0v1"
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
	if ( config.D.active )  configuration_status( &config);
	/* if -V, -v or -D then show version information */
	if ( config.V.active || config.v.active || config.D.active )
    	printf( "%s version %s\n", exeName, VERSION_INFO );
	/* if -h (help) specified then show the help/usage information but don't finish */
	if ( config.h.active )  usage( &config, exeName );
	/* if -D (debug mode) then show the positional command line arguments */
    if ( config.D.active )
    	for ( index = indexToFirstNonConfig; index < argc; index++)
        	printf ( "Debug: Non-option argument ( %d ): \"%s\"\n", index, argv[index]);
	/*  If verbosity level is 2 or more then output more info  */
	if( config.D.active || ( config.v.active && ( config.v.optionInt > 1 )))  {
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


void  setExecutableName( char *  argv[] ) {
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
	int  result;
	int  socket, count;
	int  dateFound = FALSE;
	struct timeval  now;		/* current time on local computer */

	if( cfg->D.active )  {
    	printf( "Executing: processA_SingleCommandLineParameter( %s )\n", nameStrng );
	}
	sprintf( out, "%d", cfg->p.optionInt );		/* put port number into out string */
  	socket = make_connection( out, SOCK_STREAM, nameStrng, cfg->v.active );
	if (( result = ( socket == -1 )))  {
		printf( "?? unable to connect to %s\n", nameStrng );
	}
	else  {
		if( cfg->g.active ) 
			sprintf( out, "GET http://%s/ HTTP/1.0\r\n", nameStrng );
		else
			sprintf( out, "HEAD http://%s/ HTTP/1.0\r\n", nameStrng );
		sendString( cfg, socket, out );
		sendString( cfg, socket, "Pragma: no-cache\r\n" );
		sendString( cfg, socket, "User-Agent: webtime/0.10 [en]\r\n" );
		sprintf( out, "Host: %s\r\n", nameStrng );
		sendString( cfg, socket, out );
		sendString( cfg, socket, "Accept: text/html, text/plain\r\n" );
		sendString( cfg, socket, "Accept-Language: en\r\n" );
		sendString( cfg, socket, "Accept-Charset: iso-8859-1,*,utf-8\r\n" );
		sendString( cfg, socket, "\r\n" );
		while(( count = sock_gets( socket, out, BFR_SIZE - 1 )) > -1 )  {
			if ( count >= BFR_SIZE )  out[ BFR_SIZE - 1 ] = '\0';		/* ensure terminated string */
			else if ( count > 0 )  out[ count ] = '\0';
			else  out[ 0 ] = '\0';
			if (( dateFound = ( strstr( out, "Date: " ) != NULL )) || ( cfg->v.optionInt > 1 ))  {
				if ( cfg->l.active && dateFound && ( gettimeofday( &now, NULL ) == 0 ))
					printf( "Local Host GMTime: %s", asctime( gmtime( &now.tv_sec )));
				if ( cfg->v.optionInt > 2 )  printf( "%05d: ", count );
				if ( cfg->l.active && dateFound )  printf( "Server " );
				printf( "%s\n", out );
			}
		}
		close( socket );
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
