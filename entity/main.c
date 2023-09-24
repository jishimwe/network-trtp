#include <stdio.h>
#include <string.h>
#include "receiver.h"
#include "sender.h"
#include "log.h"

int main(int argc,char ** argv) {
    if(strcmp(argv[1], "sender") == 0)
    {
        sender_main(argc, argv);
    }
    else if(strcmp(argv[1], "receiver") == 0)
    {
        receiver_main(argc, argv);
    }
    else
    {
        print_usage("Sender or receiver not specified");
    }
}
