#define DLA_STATE_STOPPED	0
#define DLA_STATE_RUNNING	1

void StartDLA(void);
void StopDLA(void);
void NotifyDLA(long uptime, int xdsl_link_status);
int GetDLAStatus(void);
void ShowDLAInfo(void);
int IsDLAModified(void);
void PauseDLA(void);
void ResumeDLA(void);
