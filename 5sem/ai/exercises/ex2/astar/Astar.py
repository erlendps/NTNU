import time
from Map import Map_Obj
import math


class Astar:
    """
    A* solver class.

    Takes in a task number and creates a Map_Obj instance.
    The algorithm is based on the pseudo-code from wikipedia:
    https://en.wikipedia.org/wiki/A*_search_algorithm#Pseudocode

    str(node) is used as keys in most dictionaries, as lists are not a valid
    key. I have since realized that tuples are valid, but I haven't refactored.


    Methods
    ----------

    backtrace_path(came_from, current_node)
        returns the path with the lowest cost from current_node to start as a list

    manhattan_func(node)
        returns the manhattan distance from a node to the goal

    euclid_func(node)
        returns the euclidian distance from a node to the goal

    show_solution(function)
        shows the path found by calling the astar method and setting the visited
        nodes to a different color, before "creating" an image (see Map.py).

    astar(function)
        returns a dictionary containing the path taken from start to goal. Uses the
        heuristic function declared in the function argument.
    """

    def __init__(self, task: int = 1) -> None:
        """
        Initiates the astar class, by saving a Map_Obj and setting the start and goal
        position.
        """
        self.map = Map_Obj(task=task)
        self.start = self.map.get_start_pos()
        self.goal = self.map.get_goal_pos()

    def backtrace_path(self, came_from: dict, current_node: list[list, list]) -> list:
        """
        Backtraces the path.

        Given a directed graph (dictionary) came_from, it backtraces and returns a list
        containing the nodes that constructs the path.

        The current_node was initially just a list, containing the y and x coordinates 
        for the node. But I tried to solve task3, and realized I needed something more
        to store state, since it could happen when the goal moves, the correct path takes
        a u-turn, and it would then overwrite the path. Therefore, I changed it to be a list
        of list, where the first element specifies the node position, and the second specifies
        the goal position. This however, did not work:P
        """

        path = [current_node[0]]
        current = current_node
        # while we have not reached the end (start) of the path.
        while str(current) in came_from.keys():
            current = came_from[str(current)]
            path.insert(0, current[0])
        return path

    def manhattan_func(self, node: list[int, int]) -> int:
        """Heuristic function based on manhatten function."""

        return abs(node[0] - self.goal[0]) + abs(node[1] - self.goal[1])

    def euclid_func(self, node: list[int, int]) -> float:
        """Heuristic function based on euclidian distance."""

        return math.sqrt((node[0] - self.goal[0]) ** 2 + (node[1] - self.goal[1]) ** 2)

    def get_neighbors(self, node: list[int, int]) -> list[list]:
        """
        Function that returns a list of lists, where the list elements are the neighbors
        of the node argument.

        It assumes that a diagonally placed node is not a neighbor,
        meaning a neighbor can be at x+-1 or y+-1. It also checks whether a neighbor
        would be "out of bounce", i.e outside the grid.
        """
        neighbors = []
        min_x = node[1] - 1
        max_x = node[1] + 1
        min_y = node[0] - 1
        max_y = node[0] + 1
        # the first element in a node is the y-coordinate
        if min_x >= 0:
            neighbors.append([node[0], min_x])
        if max_x < len(self.map.int_map[0]):
            neighbors.append([node[0], max_x])
        if min_y >= 0:
            neighbors.append([min_y, node[1]])
        if max_y < len(self.map.int_map):
            neighbors.append([max_y, node[1]])

        return neighbors

    def show_solution(self, h):
        """
        Shows the solution as an image.

        It does this by solving the search in the call to astar (which returns a path),
        and sets position string value on the map to a special color. It then shows the map.

        A map drawn with no path means there was not found a solution.
        """

        path = self.astar(h)
        for node in path:
            self.map.set_cell_value(node, ' P ')
        self.map.show_map()

    def astar(self, h) -> dict:
        """
        A* algorithm based on the pseudo-code in the wikipedia article linked to in the class docs.

        It takes in a heuristic function, h, that is uses to calculate the predicted
        distance to the goal. The open set includes unexpanded nodes, and is filled
        during runtime.

        The parent-graph works as the closed set, but with some tweeks. It shows
        from which node a child came from, i.e who is the parent of the current node. It is implemented
        as a dictionary, where the key is the current node and the value is the node it came from.
        This is used in the backtrace_path function.

        gScore and fScore are the normal functions used in A*, and implemented as a dictionary.

        dScore is the "distance" between two neighbor nodes. I have assumed that the distance
        is the same as the weight (or int value on the int_map) of the node. This is needed
        when areas have different traffic, i.e for task 2.

        The general idea of the algorithm is as follows (more specific details as comments
        in the implementation):
        It first initiates all variables it needs. Then it starts the main loop.
        The main loop first increments the time, defined in Map.py. It then
        pops the first node in the open set, note that the open set is sorted in
        ascending fScore. The node popped is our "current_node". It then checks
        if this node is the goal. If it is, returns a list of nodes, that is the path found.
        If it isn't the goal, it expands the node, by iterating over every neighbor node.
        For each node it checks first if its unwalkable, if it is it continues at the
        next iteration. For a walkable node, it calculates a tentative gScore. This is
        the gScore of the current_node plus the distance to the neighbor. If this 
        tentative score is smaller than what is stored in the gScore dictionary, we
        update the neighbor. Updating means to set the parent in parent_graph for the
        neighbor node to the current_node, updating the gScore (which is just the tentative score)
        and updating the fScore, which is the gScore + the heuristic value.

        We then check if the neighbor is in the open set. If it isn't it is added
        in place and correctly sorted. If it is in the open set, sort the list.
        The re-expansion of nodes corresponds to the tentative gScore and checking of that. 
        """

        # initiate variables used
        open_set = [self.start]

        parent_graph = {}

        gScore = {}
        fScore = {}
        dScore = {}
        for x in range(len(self.map.int_map[0])):
            for y in range(len(self.map.int_map)):
                gScore[str([y, x])] = 10000
                fScore[str([y, x])] = 10000
                dScore[str([y, x])] = self.map.int_map[y, x]
        gScore[str(self.start)] = 0
        fScore[str(self.start)] = h(self.start)

        # function for appending a node to a set and sorting it.
        def append_node(node, set):
            f = fScore[str(node)]
            added = False
            for i in range(len(set)):
                if f < fScore[str(set[i])]:
                    set.insert(i, node)
                    added = True
                    break
            if not added:  # if it needs to be places last
                set.append(node)

        # main loop
        while len(open_set) != 0:
            # tick. On second thought, I believe this is not the correct use of this.
            self.map.tick()
            if (self.map.get_goal_pos() != self.goal):
                self.goal = self.map.get_goal_pos()
                print("Updated goal position!")

            # pop the most promising node
            current_node = open_set.pop(0)

            # check if its the goal and backtrace
            if current_node == self.goal:
                return self.backtrace_path(parent_graph, [current_node, self.goal])

            # for each neighbor
            for neighbor in self.get_neighbors(current_node):
                # check if the node is walkable
                if dScore[str(neighbor)] == -1:
                    continue

                # calculate tentaive gScore
                tent_gScore = gScore[str(current_node)] + dScore[str(neighbor)]
                # check if the tentative score is better than the one stored
                if tent_gScore < gScore[str(neighbor)]:
                    # set the neighbors parent to the current_node
                    parent_graph[str([neighbor, self.goal])] = [
                        current_node, self.goal]
                    # update gScore and fScore
                    gScore[str(neighbor)] = tent_gScore
                    fScore[str(neighbor)] = tent_gScore + h(neighbor)
                    # sort and append node to open_set/queued for reexpansion
                    if neighbor in open_set:
                        open_set.remove(neighbor)
                    append_node(neighbor, open_set)
        # if it has expanded each node, return the empty list, meaning no solution
        return []


def do_tasks():
    for i in range(1, 6):
        task = Astar(i)
        task.show_solution(task.euclid_func)
        time.sleep(10)


do_tasks()
