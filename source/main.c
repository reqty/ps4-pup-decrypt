#include "decrypt.h"

extern int DEBUG_SOCK;

time_t prevtime;

uint8_t GetElapsed(uint64_t ResetInterval) {

 time_t currenttime = time(0);
 uint64_t elapsed = currenttime - prevtime;

 if ((ResetInterval == 0) || (elapsed >= ResetInterval)) {
    prevtime = currenttime;
    return 1;
 }

 return 0;
}

int _main(struct thread* td) {
  initKernel();
  initLibc();
  initPthread();
  initNetwork();

#ifdef DEBUG_SOCKET
  struct sockaddr_in server;

  server.sin_len = sizeof(server);
  server.sin_family = AF_INET;
  server.sin_addr.s_addr = DEBUG_ADDR;                //in defines.h
  server.sin_port = sceNetHtons(DEBUG_PORT);          //in defines.h
  memset(server.sin_zero, 0, sizeof(server.sin_zero));
  DEBUG_SOCK = sceNetSocket("debug", AF_INET, SOCK_STREAM, 0);
  sceNetConnect(DEBUG_SOCK, (struct sockaddr *)&server, sizeof(server));

  int flag = 1;
  sceNetSetsockopt(DEBUG_SOCK, IPPROTO_TCP, TCP_NODELAY, (char *)&flag, sizeof(int));
#endif

  jailbreak();

  initSysUtil();

  prevtime = time(0);

  printf_notification("Running PS4 PUP Decrypter");
  decrypt_pups("/mnt/usb0/safe.PS4UPDATE.PUP", "/mnt/usb0/%s.dec");
  printf_notification("Finished PS4 PUP Decrypter");

  return 0;
}
