#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "queue.h"
#include "mainhandler.h"


// allocate memory to Flight
Flight* FlightAlloc(void) {
    // variables:
    int counter = MAX_ATTEMPT;
    Flight* temporary_pointer = nullptr;

    // try MAX_ATTEMPT times to allocate memory for new Flight
    while (counter) {
        // allocate memory
        temporary_pointer =(Flight*) calloc(1, sizeof(Flight));

        // check if allocation was good
        if (temporary_pointer)
            return temporary_pointer;

        counter--;
    }

    return nullptr;
}


// set as nullptr queue head and tail pointers
void QueueInit(QueueDescriptor &descriptor) {
    descriptor.head = nullptr;
    descriptor.tail = nullptr;
}


// adds new Flight to queue
// args: descriptor of queue and pointer on data of new node
// result: true if a new node has been added, false if not
bool AddItem(QueueDescriptor &descriptor, Data* data_ptr) {
    // if queue is empty
    if(descriptor.head == nullptr) {
        // try to allocate memory for new node
        descriptor.head = FlightAlloc();
        if(descriptor.head == nullptr)
            return false;

        // link tail with single node
        descriptor.tail = descriptor.head;

        // set single node
        descriptor.head->data = *data_ptr;
        descriptor.head->next = nullptr;
    }
    // if queue isn't empty
    else {
        // try to allocate memory for new node
        Flight* new_node = FlightAlloc();
        if(new_node == nullptr)
            return false;

        // link new node with queue
        descriptor.tail->next = new_node;
        descriptor.tail = new_node;

        // set new node
        new_node->data = *data_ptr;
        new_node->next = nullptr;
    }

    return true;
}


// clear all queue elements
void ClearAll(QueueDescriptor* descriptor) {
    // variables:
    Flight* temp_ptr = descriptor->head;

    // while count of nodes >1 delete them from head
    while(descriptor->head != descriptor->tail) {
        // remember the next node
        descriptor->head = descriptor->head->next;

        // delete current
        free(temp_ptr);

        // set new current
        temp_ptr = descriptor->head;
    }

    // delete the last (or single) element
    if (descriptor->tail != nullptr)
        free(descriptor->tail);
}

// delete node in queue by itss number in order
// result: true - node has been deleted, false node hasn't been deleted
bool DeleteItem(QueueDescriptor &descriptor, int number) {
    // variables:
    Flight* previous_pointer = nullptr;
    Flight* temp_pointer = descriptor.head;

    // check if queue is not empty or wrong input number
    if (descriptor.head == nullptr || number < 0)
        return false;

    // go to the bottom of list to find item to delete
    while (temp_pointer != nullptr && number > 0) {
        // remember previous item
        previous_pointer = temp_pointer;

        // go to the next item
        temp_pointer = temp_pointer->next;

        number--;
    }

    // check if item to delete doesn't exist
    if (temp_pointer == nullptr)
        return false;

    // link nodes
    // if delete middle or tail node (queues length is not one)
    if (previous_pointer != nullptr)
        // link if delete tail
        if (temp_pointer == descriptor.tail)                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                        // easter egg
            previous_pointer->next = nullptr, descriptor.tail = previous_pointer;

        // link if delete node in the middle
        else
            previous_pointer->next = temp_pointer->next;

    else // if delete head node
        // link if delete head and length of queue is one
        if (descriptor.head == descriptor.tail)
            descriptor.head = nullptr, descriptor.tail = nullptr;

        // link if delete head and length of queue is not one
        else
            descriptor.head = descriptor.head->next;

    // remove element
    free(temp_pointer);
    return true;
}


// magic searching
// args: descriptor of queue and query for searhing
// result: queue node or nullptr
Flight* FindItem(QueueDescriptor descriptor, Query query) {
    // variables:
    Flight* temporary_pointer = descriptor.head;
    Flight* result_pointer = nullptr;

    // find the first suitable node
    while (temporary_pointer != nullptr) {
        if (HaveEqualProperty(temporary_pointer->data, query.data, query.types[0])) {
            result_pointer = temporary_pointer;
            temporary_pointer = temporary_pointer->next;
            break;
        }

        temporary_pointer = temporary_pointer->next;
    }

    // if it doesn't exist, exit
    if (result_pointer == nullptr)
        return nullptr;

    // find other nodes
    while (temporary_pointer != nullptr) {
        // it better item has been founded, remember it
        if (HaveEqualProperty(temporary_pointer->data, query.data, query.types[0]))
            if (DataCmp(result_pointer->data, temporary_pointer->data, query.types[1]))
                result_pointer = temporary_pointer;

        temporary_pointer = temporary_pointer->next;
    }

    return result_pointer;
}


// return true, if two data structures have equal property
int HaveEqualProperty(Data one, Data two, int type) {
    // compare by:
    switch (type) {
    // number
    case 0:
        if (one.number == two.number)
            return 1;
        break;

    // aim
    case 1:
        if (!strcmp(one.aim, two.aim))
            return 1;
        break;

    // time
    case 2:
        if (one.time.hour == two.time.hour && one.time.sec == two.time.sec)
            if (one.time.min == two.time.min)
                return 1;
        break;

    // date
    case 3:
        if (one.date.year == two.date.year && one.date.month == two.date.month)
            if (one.date.day == two.date.day)
                return 1;
        break;

    // cost
    case 4:
        if (one.cost == two.cost)
            return 1;
        break;

    default:
        return -1;
    }

    return 0;
}


// returns true if the first data < the second or false
int DataCmp(Data data_one, Data data_two, int type) {
    // compare by:
    switch (type) {
    // number
    case 0:
        if (data_one.number < data_two.number)
            return 1;
        else
            return 0;

    // aim
    case 1:
        if (strcmp(data_one.aim, data_two.aim) < 0)
            return 1;
        else
            return 0;

    // time
    case 2:
        if (data_one.time.hour > data_two.time.hour)
            return 0;
        if (data_one.time.hour < data_two.time.hour)
            return 1;
        if (data_one.time.min > data_two.time.min)
            return 0;
        if (data_one.time.min < data_two.time.min)
            return 1;
        if (data_one.time.sec >= data_two.time.sec)
            return 0;
        else
            return 1;

    // date
    case 3:
        if (data_one.date.year > data_two.date.year)
            return 0;
        if (data_one.date.year < data_two.date.year)
            return 1;
        if (data_one.date.month > data_two.date.month)
            return 0;
        if (data_one.date.month < data_two.date.month)
            return 1;
        if (data_one.date.day >= data_two.date.day)
            return 0;
        else
            return 1;

    // cost
    case 4:
        if (data_one.cost < data_two.cost)
            return 1;
        else
            return 0;

    default:
        return -1;
    }
}


// save all flights to file
bool saveInfoToFile(QueueDescriptor desriptor, const char* file_name) {
    // variables
    int n_nodes = 0; // number of flights
    Flight* temp_pointer = desriptor.head;
    FILE* file;

    // try to open a file
    file = fopen(file_name, "wb");
    if (file == nullptr)
        return false;

    // leave empty space for quantity of nodes
    fseek(file, sizeof(int), 0);

    // write all flights to file
    while (temp_pointer != nullptr) {
        fwrite(&temp_pointer->data, sizeof(Data), 1, file);
        temp_pointer = temp_pointer->next;
        n_nodes++;
    }

    // white a quantity of nodes
    rewind(file);
    fwrite(&n_nodes, sizeof(int), 1, file);

    fclose(file);
    return true;
}


// read flights from file
// return true if all is ok, and false if not
bool ReadFromFile(QueueDescriptor &descriptor, const char* file_name) {
    // variables:
    int n_nodes;
    Data* temporary_data;
    FILE* file;

    // try to open a file
    file = fopen(file_name, "rb");
    if (file == nullptr)
        return false;

    // try to allocate memory for temporary data structure
    temporary_data = (Data*)malloc(sizeof(Data));
    if (temporary_data == nullptr) {
        fclose(file);
        return false;
    }

    // read a quantity of file nodes
    fread(&n_nodes, sizeof(int), 1, file);

    // read all nodes from file
    for (int i = 0; i < n_nodes; i++) {
        // set a node from a file
        fread(temporary_data, sizeof(Data), 1, file);

        // try to add a node to tail
        if(AddItem(descriptor, temporary_data) == false) {
            fclose(file);
            free(temporary_data);
            return false;
        }
    }

    fclose(file);
    free(temporary_data);
    return true;
}
