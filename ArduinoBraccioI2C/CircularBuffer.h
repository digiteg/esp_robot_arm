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

    // Returns false if buffer is empty
    bool isNotEmpty()   
    {
        return (circBufferCount > 0);       // true if buffer is not empty
    }

    // Returns true if buffer is empty
    bool isEmpty()   
    {
        return (circBufferCount <= 0);       // true if buffer is not empty
    }

    // Returns true in case of buffer overflow 
    bool isFull()
    {
        return (circBufferCount >= MAX_CIRC_BUFF); // did we reach buffer max
    }

    // Remove top element
    T*  pop() 
    {
        int r =readIndex;                   // keep old index value
        readIndex = (readIndex+1) % MAX_CIRC_BUFF;  // calculate reading circular index 
        circBufferCount--;                  // internal usage counter
        return &circBuffer[r];
    }

    // Insert element
    void push(T item)
    {
        circBuffer[writeIndex]= item;       // store item into buffer
        writeIndex = (writeIndex+1) % MAX_CIRC_BUFF;  // calculate writing circular index 
        
        if (circBufferCount<=0)
            circBufferCount=1;
        else
            circBufferCount++;
    }

    // Insert element from byte array
    void push(byte * item)   // 8 bytes buffer
    {
        circBuffer[writeIndex]=item;  // read from buffer
        writeIndex = (writeIndex+1) % MAX_CIRC_BUFF;
        circBufferCount++;          // internal usage counter
    }

    // Reset counters back to initial state
    void reset()
    {
        readIndex=0;
        writeIndex=0;
        circBufferCount=0;  
    }

};

#endif // CIRCULAR_BUFFER_H_