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

int process_file(struct dirent *ep) {
  size_t fname_len = strlen(ep->d_name) + 1;
  size_t i = fname_len-1;
  const char* ext = ep->d_name + fname_len-1;
  while (i > 0 && ep->d_name[i] != '.') i--;
  if (i == 0 || !(strcmp(ext, ".PUP") || strcmp(ext, ".pup"))) {
    // no dot in name or not pup
    return 0;
  }
  
  printf_notification("Attempting decryption on %s...", ep->d_name);
  
  char output_format[PATH_MAX] = {0};
  strncat(output_format, ep->d_name, fname_len);
  strncat(output_format, "%s.dec", 8);

  decrypt_pups(ep->d_name, output_format);
  printf_notification("Decryption done for %s...", ep->d_name);
  return 1;
}

int _main(struct thread* td) {
  initKernel();
  initLibc();
  initPthread();

#ifdef DEBUG_SOCKET
  initNetwork();
  DEBUG_SOCK = SckConnect(DEBUG_IP, DEBUG_PORT);
#endif

  jailbreak();

  initSysUtil();

  prevtime = time(0);
  printf_notification("Running PS4 PUP Decrypter, waiting for USB");

  do {
    char usb_name[7] = {0};
    char usb_path[13] = {0};
    DIR *dp;
    struct dirent *ep;

    wait_for_usb(usb_name, usb_path);
    dp = opendir(usb_path);
    if (dp == NULL) {
      // cant open usb dir?
      break;
    }
    while ((ep = readdir(dp)) != NULL) {
      process_file(ep);
    }

  } while (0);
  printf_notification("Finished PS4 PUP Decrypter");

#ifdef DEBUG_SOCKET
  printf_debug("Closing socket...\n");
  SckClose(DEBUG_SOCK);
#endif

  return 0;
}
