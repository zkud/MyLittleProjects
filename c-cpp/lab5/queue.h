#ifndef LIST_H
#define LIST_H


// constants:
const int MAX_ATTEMPT = 10; // max allocation memory attempt count
const int MAX_SIZE = 20; // max string size

/////////////////////////////////////////////////////////
// structs:
typedef struct Time {
    int sec;
    int min;
    int hour;
} Time;


typedef struct Date {
    int year;
    int month;
    int day;
} Date;


typedef struct Data {
    int number;
    float cost;
    char aim[MAX_SIZE];
    Time time;
    Date date;
} Data;


typedef struct Flight {
    Data data;
    Flight* next;
} Flight;


typedef struct ListDesriptor{
    Flight* head;
    Flight* tail;
} QueueDescriptor;


typedef struct SearchingQuery {
    int types[2]; // types of searching
    Data data; // data for searching
} Query;
///////////////////////////////////////////////////
// functions:

// allocate memory to Flight
Flight* FlightAlloc(void);

// set as nullptr lists head and tail pointers
void QueueInit(QueueDescriptor &descriptor);

// adds new Flight to queue
// args: descriptor of queue and pointer on data of new node
// result: true if a new node has been added, false if not
bool AddItem(QueueDescriptor &descriptor, Data* data_ptr);

// clear all queue nodes
void ClearAll(QueueDescriptor* descriptor);

// delete node in queue by its number in order
// result: true - node has been deleted, false node hasn't been deleted
bool DeleteItem(QueueDescriptor &descriptor, int number);

// magic searching
// args: descriptor of queue and query for searhing
// result: queue node or nullptr
Flight* FindItem(QueueDescriptor descriptor, Query query);

// return true, if two data structures have equal property
int HaveEqualProperty(Data one, Data two, int type);

// returns true if the first data < the second or false
int DataCmp(Data data_one, Data data_two, int type);

// save all flights to file
bool saveInfoToFile(QueueDescriptor desriptor, const char* file_name);

// read flights from file
// return true if all is ok, and false if not
bool ReadFromFile(QueueDescriptor &descriptor, const char* file_name);

#endif // LIST_H
