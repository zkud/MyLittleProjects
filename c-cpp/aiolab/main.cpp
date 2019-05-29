/*
 * lab5: AIO
 *
 * made by L1ttl3S1st3r
*/

#include <iostream>
#include <dlfcn.h>
#include <string>

using namespace std;

typedef void (*Concater)(int argc, char* argv[]);

int main(int argc, char* argv[])
{
    int freeResult, runTimeLinkSuccess = 0;
    void* soHandle = NULL;
    Concater concater = NULL;

    //Load the .so and keep the handle to it
    soHandle = dlopen("libasyncio.so", RTLD_NOW);

    // If the handle is valid, try to get the function address.
    if (NULL != soHandle)
    {
        //Get pointer to our function using dlsym:
        concater = (Concater)dlsym(soHandle,
            "mainLike");

        // If the function address is valid, call the function.
        if (runTimeLinkSuccess = (NULL != concater))
        {
            cout << "Starting processing..." << endl;
            concater(argc, argv);
            cout << "The task was executed..." << endl;
            cout << "Press any key to continue..." << endl;
            cin.ignore();
        }

        //Free the library:
        cout << "Unattaching .so library..." << endl;
        freeResult = dlclose(soHandle);
    }

    //If unable to call the .so function, use an alternative.
    if (!runTimeLinkSuccess)
        cout << "Unable to call .so function" << endl;

    if(soHandle == NULL)
        cout << "so sorry : " << dlerror() << endl;

    return 0;
}
