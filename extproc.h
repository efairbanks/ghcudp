#ifndef EXTPROC_H
#define EXTPROC_H

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <stdarg.h>

class ExtProc {
#define PIPE_R 0
#define PIPE_W 1
  private:  
    static void _split(
        char* path,
        int* psid, int* psod, int* psed,
        FILE* psip, FILE* psop, FILE* psep
        ) {
      if(fork()==0) {
        close(psid[PIPE_W]);
        close(psod[PIPE_R]);
        close(psed[PIPE_R]);
        dup2(psid[PIPE_R],fileno(stdin));
        dup2(psod[PIPE_W],fileno(stdout));
        dup2(psed[PIPE_W],fileno(stderr));
        execl(path,path,NULL);
      } else {
        close(psid[PIPE_R]);
        close(psod[PIPE_W]);
        close(psed[PIPE_W]);
      }
    }
  public:
    int procStdInDesc[2];
    int procStdOutDesc[2];
    int procStdErrorDesc[2];
    FILE* procStdInFP;
    FILE* procStdOutFP;
    FILE* procStdErrorFP;
    ExtProc(char* path) {
      pipe(procStdInDesc);
      pipe(procStdOutDesc);
      fcntl(procStdOutDesc[PIPE_R], F_SETFL, O_NONBLOCK);
      pipe(procStdErrorDesc);
      fcntl(procStdErrorDesc[PIPE_R], F_SETFL, O_NONBLOCK);
      _split( path,
          procStdInDesc,
          procStdOutDesc,
          procStdErrorDesc,
          procStdInFP,
          procStdOutFP,
          procStdErrorFP);
      procStdInFP = fdopen(procStdInDesc[PIPE_W],"w");
      procStdOutFP = fdopen(procStdOutDesc[PIPE_R],"r");
      procStdErrorFP = fdopen(procStdErrorDesc[PIPE_R],"r");
    }
    void Print(char* fmt, ...)
    {
      va_list args;
      va_start(args,fmt);
      vfprintf(procStdInFP,fmt,args);
      va_end(args);
      fflush(procStdInFP);
    }
    int ReadStdOutChar() {
      int c = getc(procStdOutFP);
      return c;
    }
    int ReadStdErrorChar() {
      int c = getc(procStdErrorFP);
      return c;
    }
};

#endif
