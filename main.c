#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <signal.h>
#include <unistd.h>
#include <libserialport.h>
#include "knwd.h"

#define BAUDRATE	4800
#define BITS		8
#define	STOPBITS	2
#define PARITY		SP_PARITY_NONE

static volatile int keepRunning = 1;

void intHandler(int dummy) {
	keepRunning = 0;
}

void handle_rig(struct sp_port * rig_port, struct sp_port * sdr_port)
{
	uint32_t freq;
	int vfo, mode, e;
	if (knwd_getinfo(rig_port, &freq, &mode, &vfo) < 0) {
		printf("cannot get rig info\n");
		return;
	}
	if (vfo == 2) {
		printf("vfo=2 unsupported!\n");
		return;
	}
	if (mode == 3)
		if (freq < 10000000)
			mode = 7;
	//printf("VFO: %d\tfreq: %d\tmode: %d\n",vfo, freq, mode);
	e = knwd_setfreq(sdr_port, freq, vfo);
	if (e < 0)
		printf("cannot set sdr frequency(%d)\n", e);
	e = knwd_setmode(sdr_port, mode);
	if (e < 0)
		printf("cannot set sdr mode(%d)\n",e);
}

int main(int argc, char **argv)
{
	struct sp_port * rig_port;
	struct sp_port * sdr_port;

	char *rigport;
	char *sdrport;

	if (argc == 3) {
		rigport = argv[1];
		sdrport = argv[2];
	} else {
        fprintf(stderr, "usage: %s rigport sdrport\n", argv[0]);
		exit(1);
	}

	signal(SIGINT, intHandler);

	if (sp_get_port_by_name (rigport, &rig_port) != SP_OK) {
		fprintf(stderr, "No such port %s\n", rigport);
		exit(1);
	}
	if (sp_get_port_by_name (sdrport, &sdr_port) != SP_OK) {
		fprintf(stderr, "No such port %s\n", sdrport);
		sp_free_port(rig_port);
		exit(1);
	}

	if (sp_open (rig_port, SP_MODE_READ_WRITE ) != SP_OK) {
		fprintf(stderr, "No such port %s\n", rigport);
		exit(1);
	}
	if (sp_open (sdr_port, SP_MODE_READ_WRITE ) != SP_OK) {
		fprintf(stderr, "No such port %s\n", sdrport);
		sp_free_port(rig_port);
		exit(1);
	}

	sp_set_baudrate(rig_port, BAUDRATE);
	sp_set_baudrate(sdr_port, BAUDRATE);
	sp_set_bits(rig_port, BITS);
	sp_set_bits(sdr_port, BITS);
	sp_set_stopbits(rig_port, STOPBITS);
	sp_set_stopbits(sdr_port, STOPBITS);
	sp_set_parity(rig_port, PARITY);
	sp_set_parity(sdr_port, PARITY);

	while (keepRunning) {
		handle_rig(rig_port, sdr_port);
		usleep(10000);
	}

	sp_close(rig_port);
	sp_close(sdr_port);
	sp_free_port(rig_port);
	sp_free_port(sdr_port);
	return 0;
}
