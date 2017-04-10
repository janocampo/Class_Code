/* 
 * File:   main.cpp
 * Author: Jan Ocampo and Thomas Griffin
 * Class: CS433 Program 4
 * Title: Producer and Consumer Problem
 * Created on April 9, 2017, 1:50 PM
 */

#include <cstdlib>
#include <pthread.h>
#include <time.h>
#include <iostream>
#include <semaphore.h>
#include "buffer.h"


using namespace std;

/* The Global Variables */
buffer_item buffer[BUFFER_SIZE];
sem_t full, empty;
pthread_mutex_t mutex;

// Prototypes
int insert_item(buffer_item item);
int remove_item(buffer_item *item);
void *consumer (void *param); // TODO
void *producer(void *param); // TODO
void print_buffer; // TODO
int count; // Number of contents inside the buffer

   /* Task List
     * 1. Get command line arguments argv[1], argv[2], argv[3] .. Done
     * 2. Init buffer .. Done
     * 3. create pthread(s) ... Done
     * 4. create cthread(s)
     * 5. sleep
     * 6. exit
     */
    

/*
 * 
 */
int main()  {
    
    // Start 
    int i, num_prod, num_cons;
    rand(time(NULL)); // Pull random time
    pthread_mutex_init(&mutex, NULL); // Pulls the mutex global var and initialize
    
    // Gets Command Line arguments
    cout << "Please enter the number of producers threads." << endl;
    cin >> num_prod;
    cout << "Please enter the number of consumer threads." << endl;
    cin >> num_cons;
    
    // Initializes the semaphores with the buffer
    sem_init(&empty, 0, BUFFER_SIZE);
    sem_init(&full, 0, 0); 
    
    // Creates producer thread(s)
    pthread_t producers[num_prod];
    for (i = 0; i < num_prod; i++) {
        pthread_create(&producers[i], NULL, producer, NULL);
    };
    
    // Create the consumer thread(s)
    pthread_t consumers[num_cons];
    for (int i = 0; i < num_cons; i++) {
        pthread_create(&consumers[i], NULL, consumer, NULL);
    };
    
    // Sleep ?
    return 0;
}

/* insert_item function
 *  @buffer_item item   item to be added into the buffer
 *  @return int         returns 0 if item was inserted into the buffer else 1
 */
int insert_item(buffer_item item) {
    int success, in; // In is the item inserted
    sem_wait(&empty);
    pthread_mutex_lock(&mutex); // Mutex Lock ON
    
    // Adding an item into the buffer
    if (count != BUFFER_SIZE) {
        buffer[in] = item;
        in = (in + 1) % BUFFER_SIZE;
        count++;
        success = 0;
        cout << item << " was successfully placed into the Buffer" << endl;
    }
    else {
        success = 1; // Failure to insert womp womp
        cout << item << " failed to be placed into the Buffer" << endl;
    }
    
    pthread_mutex_unlock(&mutex); // Mutex Lock OFF release item
    sem_post(&full); // Semaphore is filled;
    
    return success;
}

/* remove_item function
 *  @buffer_item item   item to be added into the buffer
 *  @return int         returns 0 if item was removed from the buffer else -1
 */
int remove_item(buffer_item *item) {
    int success, out; // Out is the item to be removed
    sem_wait(&full);
    pthread_mutex_lock(&mutex); // Mutex Lock ON
    
    // Item removal commence
    if (count != 0) {
        *item = buffer[out];
        out = (out + 1) % BUFFER_SIZE;
        count--;
        success = 0;
        cout << item << " was successfully removed from the Buffer" << endl;
    }
    else {
        success = -1;
        cout << item << "failed to be removed from the Buffer" << endl;
    }
    
    pthread_mutex_unlock(&mutex); // Mutex Lock OFF 
    sem_post(&empty); // Semaphore is empty
    
    return success;
}
