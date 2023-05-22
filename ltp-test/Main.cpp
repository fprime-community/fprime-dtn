#include <cstdio>
#include <chrono>
#include <thread>

// Taken from `ion-core/Makefile`
#define DSPACE_ORDER 3
#define BP_EXTENDED

#include <pthread.h>  // TODO needed because some ION preprocessor logic is broken
#include <sys/time.h> // TODO needed because some ION preprocessor logic is broken
#include "bp.h"
#include "ltpP.h"

// TODO flip these around for use in flight
static char ownEid[]  = "ipn:3.1";
static char destEid[] = "ipn:2.1";
static pthread_mutex_t sdrmutex = PTHREAD_MUTEX_INITIALIZER;

static pthread_t bpThread;
static BpSAP sap;

static pthread_t ltpThread;

void *bp(void *arg)
{
    BpCustodySwitch custodySwitch = NoCustodyRequested;

    if (bp_attach() < 0)
    {
        putErrmsg("Can't bp_attach()", NULL);
        exit(1);
    }

    if (bp_open(ownEid, &sap) < 0)
    {
        putErrmsg("Can't open own endpoint.", ownEid);
        exit(1);
    }

    Sdr sdr = bp_get_sdr();

    char buffer[128] = "Hello world\0";
    size_t size = 128;
    
    for (int i = 0; ; i++)
    {
        // TODO these can be defined outside of this method instead
        // of instantiated each time this method is called.
        // Maybe make them `static`?
        Object bundleZco, bundlePayload;
        Object newBundle;   /* We never use but bp_send requires it. */

        if (pthread_mutex_lock(&sdrmutex) != 0)
        {
            putErrmsg("Couldn't take sdr mutex.", NULL);
            return 0;
        }

        /* Wrap the linebuffer in a bundle payload. */
        oK(sdr_begin_xn(sdr));
        bundlePayload = sdr_malloc(sdr, size);
        if (bundlePayload)
        {
            sdr_write(sdr, bundlePayload, buffer, size);
        }

        if (sdr_end_xn(sdr) < 0)
        {
            pthread_mutex_unlock(&sdrmutex);
            bp_close(sap);
            putErrmsg("No space for bpchat payload.", NULL);
            return 0;
        }

        bundleZco = ionCreateZco(ZcoSdrSource, bundlePayload, 0,
            size, BP_STD_PRIORITY, 0, ZcoOutbound, NULL);
        if (bundleZco == 0 || bundleZco == (Object) ERROR)
        {
            pthread_mutex_unlock(&sdrmutex);
            bp_close(sap);
            putErrmsg("bpchat can't create bundle ZCO.", NULL);
            return 0;
        }

        pthread_mutex_unlock(&sdrmutex);

        /* Send the bundle payload. */
        printf("[ltp-test] BP: Calling bp_send()\n");
        if (bp_send(sap, destEid, NULL, 86400, BP_STD_PRIORITY,
                custodySwitch, 0, 0, NULL, bundleZco,
                &newBundle) <= 0)
        {
            putErrmsg("bpchat can't send bundle.", NULL);
            return 0;
        }
        printf("[ltp-test] BP: sent \"%s\"\n", buffer);

        std::this_thread::sleep_for(std::chrono::seconds(1));
    }
    return nullptr;
}

void *ltp(void *arg)
{
    if (ltpInit(0) < 0)
    {
        printf("[ltp-test] LTP: Error initializing LTP\n");
    }

    LtpVspan *vspan;
    PsmAddress vspanElt; // Is an unsigned long

    if (pthread_mutex_lock(&sdrmutex) != 0)
    {
        putErrmsg("Couldn't take sdr mutex.", NULL);
        return nullptr;
    }

    Sdr sdr = getIonsdr();
    if (!sdr_begin_xn(sdr)) // Needed to get "lock" (see `udplso.c`)
    {
        pthread_mutex_unlock(&sdrmutex);
        printf("[ltp-test] LTP: Error starting SDR transaction\n");
        return nullptr;
    }

    unsigned long long remoteEngineId = 2; // TODO make this `3` for flight
    findSpan(remoteEngineId, &vspan, &vspanElt);
    if (vspanElt == 0)
    {
        sdr_exit_xn(sdr);
        pthread_mutex_unlock(&sdrmutex);
        printf("[ltp-test] LTP: No such engine in database: %llu\n", remoteEngineId);
        return nullptr;
    }
    printf("[ltp-test] LTP: Found span: %lu\n", vspanElt);

    sdr_exit_xn(sdr); // "Exit" is okay here as we don't want to persist any changes to SDR DB
    pthread_mutex_unlock(&sdrmutex);

    char *segment;
    for (int i = 0; ; i++)
    {
        if (sm_SemEnded(vspan->segSemaphore))
        {
            printf("[ltp-test] LTP: Semaphore ended prematurely for vspan");
            return nullptr;
        }

        printf("[ltp-test] LTP: Calling ltpDequeueOutboundSegment() (count = %d)\n", i+1);
        int segmentLen = ltpDequeueOutboundSegment(vspan, &segment);
        if (segmentLen < 0)
        {
            printf("[ltp-test] LTP: Bad segment length: %d\n", segmentLen);
            return nullptr;
        }
        printf("[ltp-test] LTP: Dequeued %d bytes\n", segmentLen);

        //printf("[ltp-test] LTP: Hex dump start\n");
        //for (int i = 0; i < segmentLen; i++)
        //{
        //    printf("%02x ", (unsigned char)segment[i]);
        //}
        //printf("\n");
        //printf("[ltp-test] LTP: Hex dump end\n");

        sm_TaskYield();
    }
    return nullptr;
}

static void handleQuit(int sig)
{
    pthread_end(bpThread);
    bp_interrupt(sap);

    pthread_end(ltpThread);
}

int main()
{
    int status = pthread_create(&bpThread, NULL, bp, NULL);
    if (status != 0)
    {
        printf("[ltp-test] Error creating BP thread\n");
    }

    status = pthread_create(&ltpThread, NULL, ltp, NULL);
    if (status != 0)
    {
        printf("[ltp-test] Error creating LTP thread\n");
    }

    signal(SIGINT, handleQuit);

    pthread_join(bpThread, NULL);
    pthread_join(ltpThread, NULL);
}
