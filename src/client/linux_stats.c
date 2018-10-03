#include <stdio.h>
#include <stdlib.h>
#include <definitions.h>
#include <unistd.h>

typedef struct {
  unsigned long long int total;
  unsigned long long int idle;
} cpu_time;

cpu_time GetCPUTime() {

  /* Code from HTOP */

  FILE* file = fopen(PROCSTATFILE, "r");
  if (file == NULL) {
     printf("Cannot open " PROCSTATFILE);
     exit(1);
  }
  char buffer[PROC_LINE_LENGTH + 1];
  unsigned long long int usertime, nicetime, systemtime, idletime;
  unsigned long long int ioWait, irq, softIrq, steal, guest, guestnice;
  ioWait = irq = softIrq = steal = guest = guestnice = 0;
  // Depending on your kernel version,
  // 5, 7, 8 or 9 of these fields will be set.
  // The rest will remain at zero.
  char* ok = fgets(buffer, PROC_LINE_LENGTH, file);
  if (!ok) buffer[0] = '\0';
  sscanf(buffer,   "cpu  %16llu %16llu %16llu %16llu %16llu %16llu %16llu %16llu %16llu %16llu",         &usertime, &nicetime, &systemtime, &idletime, &ioWait, &irq, &softIrq, &steal, &guest, &guestnice);
  // Guest time is already accounted in usertime
  usertime = usertime - guest;
  nicetime = nicetime - guestnice;
  // Fields existing on kernels >= 2.6
  // (and RHEL's patched kernel 2.4...)
  unsigned long long int idlealltime = idletime + ioWait;
  unsigned long long int systemalltime = systemtime + irq + softIrq;
  unsigned long long int virtalltime = guest + guestnice;
  unsigned long long int totaltime = usertime + nicetime + systemalltime + idlealltime + steal + virtalltime;
  fclose(file);

  cpu_time cputime;
  cputime.total = totaltime;
  cputime.idle  = idlealltime;
  return cputime;
}

float GetCPULoad() {
  cpu_time prev_time = GetCPUTime();
  sleep(1);
  cpu_time curr_time = GetCPUTime();

  unsigned long long int total_diff = curr_time.total - prev_time.total;
  unsigned long long int idle_diff  = curr_time.idle  - prev_time.idle;
  float cpu_percent = (((total_diff - idle_diff) / (float)total_diff)) * 100;

  /* check for invalid */
  if(cpu_percent < 0 || cpu_percent > 100)
    cpu_percent  = 0;

  return cpu_percent;  
}
