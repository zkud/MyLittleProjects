# bellman - ford example
# L1ttl3S1st3r 21.07.2018
import copy
import randomgraph


class ShortestWaysPreviousList(list):
    """shortest ways in random graph from some vertex created by Bellman-Ford algo"""
    def __init__(self, graph: randomgraph.RandomGraph, start_number: int):
        """
            :param graph:list:
                List of Vertexes
            :param first_number:int:
                Number of the start vertex
        """
        # check args
        bad_value = lambda x: x > len(graph) or x < 0
        if  bad_value(start_number):
            raise ValueError("start number is out of range")

        # variables block
        costs = [None for vertex in graph]
        costs[start_number] = 0 # because the first vertex is a start
        previous = [None for vertex in graph]
    
        # update previous & costs len(graph) - 1 times
        counter = len(graph) - 1
        while counter != 0:
            # use all edges in update
            for vertex in graph:
                for edge in vertex.neighbors():
                    edge = [vertex.number] + list(edge)
                    costs, previous = self._update(edge, costs, previous)

            counter -= 1

        # check on bad cycles 
        if self._cycle_check(graph, costs, previous):
            raise CycleofNegativeWeightError 
        else:
            super().__init__(previous)

    def _update(self, edge: tuple, costs: list, previous: list):
        """
        updates list of previous and costs lists considering costs in edge tuple
            :param edge:tuple:
            (number of source, number of dist, cost)
            :param costs:list:
            list of costs, None is similar to infinity
            :param previous:list:
            list of previous vertexes in the shortest ways
        """
        # if cost[source] = infinity
        if costs[edge[0]] is None:
            return costs, previous

        # if cost[dist] = infinity
        if costs[edge[1]] is None:
            costs[edge[1]] = (costs[edge[0]] + edge[2])
            previous[edge[1]] = edge[0]

        # if cost[dist] >= cost[source] + cost of edge
        if costs[edge[1]] >= (costs[edge[0]] + edge[2]):
            costs[edge[1]] = (costs[edge[0]] + edge[2])
            previous[edge[1]] = edge[0]
        
        return costs, previous

    def _cycle_check(self, graph: list, costs: list, previous: list) -> bool:
        """
        return True if cycles of negative weight are exist in graph
        """
        # variables block
        new_costs, new_previous = copy.deepcopy(costs), copy.deepcopy(previous)
    
        # update new_costs and new_previous
        for vertex in graph:
            for edge in vertex.neighbors():
                new_costs, new_previous = self._update(tuple([vertex.number] + list(edge)),
                                                new_costs, new_previous)

        # if (new_costs, new_previous) != (costs, previous) cycles exist
        if new_costs != costs or new_previous != previous:
            return True
        else:
            return False


class ShortestWay(list):
    """shortest way in random graph between two vertexes"""

    def __init__(self, previous_list: ShortestWaysPreviousList, first_number: int,
                second_number: int):
        """
            :param previous_list:
                result of Bellman Ford algo for start vertex with first_number
            :param first_number:
                start vertex number
            :param second_number:
                end vertex number
        """
        # check args
        bad_value = lambda x: x > len(previous_list) or x < 0
        if  bad_value(first_number) or bad_value(second_number):
            raise ValueError("first number or second number are out of range")

        # variables block
        value = list()
        not_founded = True #"way hasn't founded yet" flag
        self._not_exist = False

        # while way hasn't founded yet add previous to way list
        current_number = second_number
        while not_founded:            
            # add number to way
            value = [current_number] + value

            # check number
            if current_number is first_number:
                not_founded = False

            # if current number is None way doesn't exist
            if current_number is None:
                self._not_exist = True
                break

            # go to the next
            current_number = previous_list[current_number]

        super().__init__(value)

    @property
    def is_not_exist(self) -> bool:
        return self._not_exist


class ResultView:
    """answer of program"""
    def __init__(self, shortestway: ShortestWay, graph: randomgraph.RandomGraph):
        self._way = shortestway
        self._graph = graph

    def __repr__(self):
        return self.graph_view + '\n' + self.way_view

    @property
    def way_view(self) -> str:
        if self._way.is_not_exist:
            return "There are no ways between chosen vertexes"
    
        result_str = "The shortest way between {} and {} in graph:\n".format(
            self._way[0], self._way[-1]
        )

        for number in self._way:
            result_str += "{} -> ".format(str(number))

        return result_str[:-4]

    @property
    def graph_view(self) -> str:
        result_str = "Graph content:\n"

        # graph content in form number of vertex : edges
        for vertex in self._graph:
            result_str += "{} : {}\n".format(vertex.number, self._dists(vertex))

        return result_str + "\n"

    def _dists(self, vertex: randomgraph.Vertex) -> str:
        """returns ends of all edges from vertex"""
        ends_str = ""

        for edge in vertex.neighbors():
            ends_str += " {}({}),".format(edge[0], edge[1])

        return ends_str[:-1]

    
class CycleofNegativeWeightError(ArithmeticError):
    pass


def main():
    """ main prog's function, describes all actions"""
    # create random graph
    graph = randomgraph.RandomGraph(
        int(input("Min vertex count:")),
        int(input("Max vertex count:")),
        int(input("Min edges count:")),
        int(input("Max edges count:")),
        int(input("Min cost:")),
        int(input("Max cost:"))
    )

    # use Bellman - Ford algo, get shortest way from previous list, wrap in result
    try:
        start_number = int(input("Start number:"))
        end_number = int(input("End number(< {}):".format(len(graph))))
        result = ResultView(
            ShortestWay(ShortestWaysPreviousList(graph, start_number), start_number, end_number),
            graph
        )

        # print result
        print(result)
    except CycleofNegativeWeightError:
        print("Can't find way due to cycle of negative weight in graph")


main()
    