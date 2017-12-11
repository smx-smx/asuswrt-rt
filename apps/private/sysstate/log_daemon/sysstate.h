/*

	sysstat.h
	Copyright (C) 2016 Renjie Lee

*/

//cpu usage
#if 0
#define DUMP_PATH_CPUUSAGE "/tmp/asusfbsvcs/cpuusage_log.txt"
#define DUMP_PATH_DETAIL_CPUUSAGE "/tmp/asusfbsvcs/cpuusage_detail_log.txt"

typedef unsigned long long cputime64_t;

struct cpu_usage_stat {
	cputime64_t user;
	cputime64_t nice;
	cputime64_t system;
	cputime64_t softirq;
	cputime64_t irq;
	cputime64_t idle;
	cputime64_t iowait;
	cputime64_t steal;
};

struct cpu_usage_diff {
	cputime64_t user;
	cputime64_t nice;
	cputime64_t system;
	cputime64_t softirq;
	cputime64_t irq;
	cputime64_t idle;
	cputime64_t iowait;
	cputime64_t steal;
	double factor;
};

struct cpu_usage_record_s {
	time_t timestamp;
	struct cpu_usage_diff* cuRecord;
	struct cpu_usage_record_s* next;
};

int is_cpu_prefix(char *line);
unsigned int get_cpu_amount(void);
void get_cpu_jiffies(struct cpu_usage_stat *result);
void calc_diff(struct cpu_usage_stat *prev, struct cpu_usage_stat *curr, struct cpu_usage_diff *diff);
void store_prev_value(struct cpu_usage_stat *prev, struct cpu_usage_stat *curr);
void cpuusage_main(int interval);
void cpuusage_dump(void);
void cpuusage_dump_detail(void);
void init_cpuusage(void);
#endif
//ram usage


