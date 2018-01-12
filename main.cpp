#include "dirtyudp.h"
#include "extproc.h"
// -------------------------------------------------------------- //
// Demo app using my dirtyudp lib, which is a quick-and-dirty,    //
// object-oriented implementation of both halves of a UDP server. //
// -------------------------------------------------------------- //
// Compile with sender set to true to build an example UDP client //
// or set sender to false to build an example UDP server.         //
// -------------------------------------------------------------- //
ExtProc* extproc;
void udpCallback(char* packet, int packetSize, void* data) {
  for(int i=0; i<packetSize; i++) extproc->Print("%c", packet[i]); //fprintf(stdout,"%c", packet[i]);
}
int main(int argc, char** argv) {
  //#ifdef SEND
  //char* message = "Whaasssssaaaaaap?";
  //UdpSender* sender = new UdpSender("127.0.0.1", 9999, 1024);
  //sender->Send(message, strlen(message));
  //#else
  extproc = new ExtProc("/usr/bin/ghci");
  UdpReceiver* receiver = new UdpReceiver(9999, 1024);
  while(1) {
    receiver->Loop(udpCallback, NULL);
    //extproc->Print("test\n");
    char c;
    while((c=extproc->ReadStdOutChar())!=EOF) fprintf(stdout, "%c", c);
    while((c=extproc->ReadStdErrorChar())!=EOF) fprintf(stdout, "%c", c);
  }
  //#endif
  return 0;
}
