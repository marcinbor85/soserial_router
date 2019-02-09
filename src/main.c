
#include <stdio.h>
#include <unistd.h>

#include "server.h"
#include "dispatch.h"

#include "net/queue.h"

int main(int argc , char *argv[])
{
        dispatch_start();
        server_start(8889);
}