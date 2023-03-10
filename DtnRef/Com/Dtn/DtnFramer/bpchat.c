/*
 * bpchat.c
 * Andrew Jenkins <andrew.jenkins@colorado.edu>
 * Reads lines from stdin and sends those in bundles.
 * Receives bundles and writes them to stdout.
 */

#include <stdlib.h>
#include <stdio.h>
#include "bpchat.h"

static BpSAP                sap;
static Sdr                  sdr;
static pthread_mutex_t      sdrmutex = PTHREAD_MUTEX_INITIALIZER;
static char                 *destEid = NULL;
static char                 *ownEid = NULL;
static BpCustodySwitch      custodySwitch = NoCustodyRequested;
static int                  running = 1;

const char usage[] =
"Usage: bpchat.c <source EID> <dest EID> [ct]\n\n"
"Reads lines from stdin and sends these lines in bundles.\n"
"Receives bundles and writes them to stdout.\n"
"If \"ct\" is specified, sent bundles have the custody transfer flag set\n";

static pthread_t    sendLinesThread;
static void *       sendLines(void *args)
{
	int             lineLength = 0;
	char            lineBuffer[1024];

	while(running) {
		/* Read a line from stdin */
		if(fgets(lineBuffer, sizeof(lineBuffer), stdin) == NULL) {
			fprintf(stderr, "EOF\n");
			running = 0;
			bp_interrupt(sap);
			break;
		}

		lineLength = strlen(lineBuffer);
        if (!bpchat_send(lineBuffer, lineLength))
        {
            break;
        }
	}
	return NULL;
}

int bpchat_send(const char *buffer, size_t size)
{
	Object          bundleZco, bundlePayload;
	Object          newBundle;   /* We never use but bp_send requires it. */
	/* Wrap the linebuffer in a bundle payload. */
	if(pthread_mutex_lock(&sdrmutex) != 0) {
		putErrmsg("Couldn't take sdr mutex.", NULL);
		printf("Couldn't take sdr mutex.\n");
		return 0;
	}

	oK(sdr_begin_xn(sdr));
	bundlePayload = sdr_malloc(sdr, size);
	if(bundlePayload) {
		sdr_write(sdr, bundlePayload, buffer, size);
	}

	if(sdr_end_xn(sdr) < 0) {
		pthread_mutex_unlock(&sdrmutex);
		bp_close(sap);
		putErrmsg("No space for bpchat payload.", NULL);
		printf("No space for bpchat payload.\n");
		return 0;
	}

	bundleZco = ionCreateZco(ZcoSdrSource, bundlePayload, 0, 
		size, BP_STD_PRIORITY, 0, ZcoOutbound, NULL);
	if(bundleZco == 0 || bundleZco == (Object) ERROR) {
		pthread_mutex_unlock(&sdrmutex);
		bp_close(sap);
		putErrmsg("bpchat can't create bundle ZCO.", NULL);
		printf("bpchat can't create bundle ZCO.\n");
		return 0;
	}
	pthread_mutex_unlock(&sdrmutex);

	/* Send the bundle payload. */
	if(bp_send(sap, destEid, NULL, 86400, BP_STD_PRIORITY,
			custodySwitch, 0, 0, NULL, bundleZco,
			&newBundle) <= 0)
	{
		putErrmsg("bpchat can't send bundle.", NULL);
		printf("bpchat can't send bundle.\n");
		return 0;
	}

    return 1;
}

static pthread_t    recvBundlesThread;
static void *       recvBundles(void *args)
{
	BpDelivery      dlv;
	ZcoReader       reader;
	char            buffer[1024];
	int             bundleLenRemaining;
	int             rc;
	int             bytesToRead;

	while(running)
	{
		if(bp_receive(sap, &dlv, BP_BLOCKING) < 0)
		{
			putErrmsg("bpchat bundle reception failed.", NULL);
			break;
		}

		if(dlv.result == BpReceptionInterrupted || dlv.adu == 0)
		{
			bp_release_delivery(&dlv, 1);
			continue;
		}

		if(dlv.result == BpEndpointStopped)
		{
			bp_release_delivery(&dlv, 1);
			break;
		}

		if(pthread_mutex_lock(&sdrmutex) != 0)
		{
			putErrmsg("Couldn't take sdr mutex.", NULL);
			break;
		}

		oK(sdr_begin_xn(sdr));
		bundleLenRemaining = zco_source_data_length(sdr, dlv.adu);
		zco_start_receiving(dlv.adu, &reader);
		while(bundleLenRemaining > 0)
		{
			bytesToRead = MIN(bundleLenRemaining, sizeof(buffer)-1);
			rc = zco_receive_source(sdr, &reader, bytesToRead,
					buffer);
			if(rc < 0) break;
			bundleLenRemaining -= rc;
			printf("%.*s", rc, buffer);
			fflush(stdout);
		}

		if (sdr_end_xn(sdr) < 0)
		{
			running = 0;
		}

		pthread_mutex_unlock(&sdrmutex);
		bp_release_delivery(&dlv, 1);
	}        
	return NULL;
}

static void handleQuit(int sig)
{
	running = 0;
	pthread_end(sendLinesThread);
	bp_interrupt(sap);
}

int bpchat_start(char *_ownEid, char *_destEid)
{
	ownEid  = _ownEid;
	destEid = _destEid;
	char *ctArg = NULL;

	if(ctArg && strncmp(ctArg, "ct", 3) == 0) {
		custodySwitch = SourceCustodyRequired;
	}

	if(bp_attach() < 0) {
		putErrmsg("Can't bp_attach()", NULL);
		exit(1);
	}

	if(bp_open(ownEid, &sap) < 0) 
	{
		putErrmsg("Can't open own endpoint.", ownEid);
		exit(1);
	}

	sdr = bp_get_sdr();

	signal(SIGINT, handleQuit);

	/* Start receiver thread and sender thread. */
	if(pthread_begin(&sendLinesThread, NULL, sendLines, NULL) < 0) {
		putErrmsg("Can't make sendLines thread.", NULL);
		bp_interrupt(sap);
		exit(1);
	}

	if(pthread_begin(&recvBundlesThread, NULL, recvBundles, NULL) < 0) {
		putErrmsg("Can't make recvBundles thread.", NULL);
		bp_interrupt(sap);
		exit(1);
	}

	//pthread_detach(sendLinesThread);
	//pthread_detach(recvBundlesThread);
    // TODO currently detaching, do not want to wait for threads to finish
	pthread_join(sendLinesThread, NULL);
	pthread_join(recvBundlesThread, NULL);

	bp_close(sap);
	bp_detach();
	return 0;
}
