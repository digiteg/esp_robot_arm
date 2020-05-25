
/*
    
    CircularBuffer.h - Braccio arm I2C communication  library Version 1.0
    Written by Milan Varga

*/

#ifndef CIRCULAR_BUFFER_H_
#define CIRCULAR_BUFFER_H_

#define MAX_CIRC_BUFF 10

template <typename T>
class _CircularBuffer {

private:

    volatile int readIndex=0;
    volatile int writeIndex=0;
    volatile int circBufferCount=0;

    volatile T circBuffer [MAX_CIRC_BUFF];

public:

    bool IsNotEmpty()   
    {
        return (circBufferCount > 0);       // true if buffer is not empty
    }

    bool IsEmpty()   
    {
        return (circBufferCount <= 0);       // true if buffer is not empty
    }


    bool IsFull()
    {
        return (circBufferCount >= MAX_CIRC_BUFF); // did we reach buffer max
    }

    T*  readCircBuff() 
    {
        int r =readIndex;                   // keep old index value
        readIndex = (readIndex+1) % MAX_CIRC_BUFF;  // calculate reading circular index 
        circBufferCount--;                  // internal usage counter
        return &circBuffer[r];
    }

    void write(T item)
    {
        circBuffer[writeIndex]= item;       // store item into buffer
        writeIndex = (writeIndex+1) % MAX_CIRC_BUFF;  // calculate writing circular index 
        circBufferCount++;
    }

    void writeCircBuff(byte * item)   // 8 bytes buffer
    {
        circBuffer[writeIndex]=item;  // read from buffer
        writeIndex = (writeIndex+1) % MAX_CIRC_BUFF;
        circBufferCount++;          // internal usage counter
    }

};

#endif // CIRCULAR_BUFFER_H_