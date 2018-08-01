"""module contains:
    1. Vertex class
        number is a name of vertex
        neigbors is generator of tuple like (distination of edge, cost of edge)

    2. RandomGraph class
        absolutely random graph with number of vertexes in (vertex_min_count, vertex_max_count),
        number of edges from one vertex in (edges_min_count, edges_max_count),
        and cost in (min_cost, max_cost)
    """
import random
import time


class Vertex:
    """Graph vertex class"""

    __slots__ = ['_number', '_edges']
    
    def __init__(self, number: int):
        self._number = number
        self._edges = dict()
    
    def set_edges(self, edges: dict):
        # edge is a dict with number of neighbor and cost of edge
        self._edges = edges

    # neighbors (edges) generator
    def neighbors(self) -> tuple:
        for neighbor, cost in self._edges.items():
            yield (neighbor, cost)

    @property
    def number(self):
        return self._number


class RandomGraph(list):
    """Graph with random edges and vertexes
        vertex_min_count <= vertex in graph count <= vertex_max_count
        edges_min_count <= edges from one vertex count <= edges_max_count 
    """
    def _randint_with_exc(self, min_value: int, max_value: int,
                        is_exception, end_counter=None) -> int:
        """
            method generates integer in interval (min_value, max_value)
            and check if it's is not an exception
            :param min_value:
                left interval border 
            :param max_value: 
                right interval border
            :param is_exception:
                function, thas returns true if number is exception and false if it isn't
            :param end_counter=None:
                max count of attempts to generate
        """ 
        # check args
        if end_counter is not None:
            if end_counter <= 0:
                raise ValueError("end counter must be > 0")
        if is_exception is None or min_value is None or max_value is None:
            raise ValueError("args can't be None")
        if min_value > max_value:
            raise ValueError("min value must be less than or equal to max value")

        # try to create suitable number
        number = random.randint(min_value, max_value)

        # if there is not any limit try infinity times to create suitable number
        if end_counter is None:
            while is_exception(number):
                number = random.randint(min_value, max_value)
        else:
            # else try end_counter times to create suitable number
            while is_exception(number) and end_counter is not 0:
                number = random.randint(min_value, max_value)
                end_counter -= 1

        if end_counter is 0:
            raise TimeoutError

        return number

    def __init__(self, vertex_min_count: int, vertex_max_count: int,
        edges_min_count: int, edges_max_count: int, min_cost: int, max_cost: int):
        random.seed(int(time.time()))

        # create list of vertexes
        vertex_count = random.randint(vertex_min_count, vertex_max_count)
        graph = [Vertex(counter) for counter in range(vertex_count)]

        # create edges in graph
        for vertex in graph:
            edges = dict()
            edges_count = random.randint(edges_min_count, edges_max_count)

            # generate edges of single vertex
            while edges_count is not 0:
                neighbor_number = self._randint_with_exc(0, vertex_count - 1,
                                                     lambda x: x == vertex.number)
                edge_cost = random.randint(min_cost, max_cost)
                edges[neighbor_number] = edge_cost
            
                edges_count -= 1
        
                vertex.set_edges(edges)

        super().__init__(graph)