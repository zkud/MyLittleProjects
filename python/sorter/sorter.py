# training in multithreading
# L1ttl3S1st3r
# merge sort
import threading
from time import sleep as wait

# sorting thread class
class SortingThread(threading.Thread):
    def __init__(self, raw_list):
        self.list = raw_list
        threading.Thread.__init__(self)

    def run(self):
        self.list.sort()

# multithreading merge sort
def lists_multisort(lists, threads_count):
    # variables:
    threads_list = list()

    # set threads and start parallel sorting of all lists
    for i in range(threads_count):
        new_thread = SortingThread(lists[i])
        threads_list.append(new_thread)
        new_thread.start()

    # end parallel sorting of all lists
    for thread in threads_list:
        thread.join()

    # sort result list
    counter = len(lists)
    while counter != 0:
        lists.extend(lists[0])
        lists = lists[1:]
        counter -= 1
    lists.sort()

    return lists

# create list of lists from file
def set_lists(input_file_name, threads_count, list_size):
    # variables
    lists = list()

    # initialize lists
    with open(input_file_name, "r+") as input_file:
        while threads_count > 0:
            # variables
            counter = list_size
            new_list = list()

            # add variables to new list
            while counter != 0:
                # input a new line
                line = input_file.readline()
                
                # check if file hasn't ended
                if len(line) == 0:
                    break

                # add line to new list
                new_list.append(line)

                counter -= 1
                    
            # add it to main list
            lists.append(new_list)
            threads_count -= 1

    return lists

def main():
    # check and read config:
    try:
        with open("config.txt", "r+") as config_file:
            input_file_name = config_file.readline().split()[2]
            threads_count = int(config_file.readline().split()[2])
            list_size = int(config_file.readline().split()[2])
    except:
        print("Error: there is no config")
        wait(2)
        return (-1)

    # initialize lists from file
    lists = set_lists(input_file_name, threads_count, list_size)
    
    # sort lists with multithreading
    result_list = lists_multisort(lists, threads_count)

    # return result in file
    with open("\'" + input_file_name + "\'" + "result.txt", "w+") as output_file:
        for variable in result_list:
            output_file.write(str(variable) + '\n')

    return 0

if __name__ == '__main__':
    main()

