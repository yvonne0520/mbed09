
#include "mbed.h"
#include "mbed_rpc.h"

/**
 *  This example program has been updated to use the RPC implementation in the new mbed libraries.
 *  This example demonstrates using RPC over serial
**/
/*RpcDigitalOut myled1(LED1, "myled1");
RpcDigitalOut myled2(LED2, "myled2");
RpcDigitalOut myled3(LED3, "myled3");*/
Ticker timeup;
DigitalOut RED(LED1);
DigitalOut BLUE(LED3);
Serial pc(USBTX, USBRX);
void Control(Arguments *in, Reply *out);
void blink();
RPCFunction rpcLED(&Control, "Control");
double x;
int on_or = 0;

int main()
{
    RED  = 0;
    BLUE = 1;
    timeup.attach(&blink, 0.3);
    //The mbed RPC classes are now wrapped to create an RPC enabled version - see RpcClasses.h so don't add to base class

    // receive commands, and send back the responses
    char buf[256], outbuf[256];
    while (1)
    {
        memset(buf, 0, 256);
        for (int i = 0;; i++)
        {
            char recv = pc.getc();
            if (recv == '\r')
            {
                pc.printf("\r\n");
                break;
            }

            buf[i] = pc.putc(recv);
        }

        //Call the static call method on the RPC class
        RPC::call(buf, outbuf);
        pc.printf("%s\r\n", outbuf);
    }
}

// Make sure the method takes in Arguments and Reply objects.
void Control(Arguments *in, Reply *out)
{   x = in->getArg<double>();
    on_or = x;
}

void blink () 
{
    if (on_or == 1)
    {
        RED  = !RED;
        BLUE = !BLUE;
    }
}