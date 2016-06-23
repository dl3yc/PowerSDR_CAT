#ifndef	KNWD_H_
#define	KNWD_H_

int knwd_setfreq(struct sp_port * port, uint32_t freq, int vfo);
int knwd_setmode(struct sp_port * port, int mode);
int knwd_setvfo(struct sp_port * port, int vfo);
int knwd_getinfo(struct sp_port * port, uint32_t * freq, int * mode, int * vfo);

#define KNWD_REQLEN	3

#define KNWD_INFO_CMD		"IF"
#define KNWD_INFO_RSPLEN	(KNWD_REQLEN+35)

#define KNWD_FREQA_CMD		"FA"
#define KNWD_FREQB_CMD		"FB"
#define KNWD_FREQ_SETLEN	(KNWD_REQLEN+11)

#define KNWD_MODE_CMD		"MD"
#define KNWD_MODE_SETLEN	(KNWD_REQLEN+1)

#endif /* KNWD_H_ */
