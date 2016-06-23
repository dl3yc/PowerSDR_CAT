#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <libserialport.h>
#include "knwd.h"

#define TIMEOUT 1000

int knwd_getinfo(struct sp_port * port, uint32_t * frequency, int * mode, int * vfo)
{
	char buf[KNWD_INFO_RSPLEN];
	int nbytes;
	sprintf(buf, "%s;", KNWD_INFO_CMD);
	sp_blocking_write(port, buf, KNWD_REQLEN, TIMEOUT);
	nbytes = sp_blocking_read(port, buf, KNWD_INFO_RSPLEN, TIMEOUT);
	if (nbytes != KNWD_INFO_RSPLEN)
		return -1;
	if (strncmp(buf, KNWD_INFO_CMD, 2))
		return -2;
	if (buf[KNWD_INFO_RSPLEN-1] != ';')
		return -3;

	buf[13] = 0;
	*frequency = strtol(buf+2, NULL, 10);
	if (*frequency < 0)
		return -4;

	*mode = buf[29] - '0';
	*vfo = buf[30] - '0';

	return 0;
}

int knwd_setmode(struct sp_port * port, int mode)
{
	char buf[KNWD_MODE_SETLEN];
	sprintf(buf, "%s%d;", KNWD_MODE_CMD, mode);
	return (sp_blocking_write(port, buf, KNWD_MODE_SETLEN, TIMEOUT));
}

int knwd_setfreq(struct sp_port * port, uint32_t freq, int vfo)
{
	char buf[KNWD_FREQ_SETLEN];
	sprintf(buf, "%s%011d;", (vfo ? KNWD_FREQA_CMD : KNWD_FREQB_CMD), freq);
	return (sp_blocking_write(port, buf, KNWD_FREQ_SETLEN, TIMEOUT));
}
