import csv
import logging
import fire


class Program:
    __criminal_dict__ = dict()

    def find_stuff(self, path: str) -> str:
        """
            finds a crime ocurred max times in 2015
            @return string with result
            :param path: str:
                path to csv statistic file (Crimes.csv)
        """
        # variables
        max_value: int = 0
        max_name: str = None

        # try to open table file
        logging.info('try to open file')
        with open(path, 'r+') as csvfile:

            logging.info('try to read all nodes')
            reader = csv.reader(csvfile)
            # read all nodes in table
            for node in reader:
                if '2015' in node[2]:
                    # if node already exist in dictionarys keys update value
                    if node[5] in self.__criminal_dict__.keys():
                        logging.info('try to update node {} in dict'.format(node[5]))
                        self.__criminal_dict__[node[5]] += 1
                        current_value = self.__criminal_dict__[node[5]]

                        # update max:
                        if current_value > max_value:
                            logging.info(
                                'update max value: new max is ({}, {})'.format(node[5], current_value)
                            )
                            max_value = current_value
                            max_name = node[5]

                    # else add new key
                    else:
                        logging.info('try to add node {} in dict'.format(node[5]))
                        self.__criminal_dict__[node[5]] = 1

                        # update max
                        if 1 > max_value:
                            logging.info(
                                'update max value: new max is ({}, {})'.format(node[5], 1)
                            )
                            max_name = node[5]
                            current_value = 1

        # return result
        return "max is {} (name is {})".format(max_value, max_name)
                        

if __name__ == '__main__':
    logging.basicConfig(filename='progLog.log', level=0)
    fire.Fire(Program)
