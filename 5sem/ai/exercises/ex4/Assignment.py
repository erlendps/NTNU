# CSP Assignment
# Original code by Håkon Måløy
# Updated by Xavier Sánchez Díaz

import copy
from itertools import product as prod
from operator import ne
from random import choice
from typing import List


class CSP:
    def __init__(self):
        # self.variables is a list of the variable names in the CSP
        self.variables = []

        # self.domains is a dictionary of domains (lists)
        self.domains = {}

        # self.constraints[i][j] is a list of legal value pairs for
        # the variable pair (i, j)
        self.constraints = {}

        # stats
        self.backtrack_count = 0
        self.backtrack_count_failure = 0

    def add_variable(self, name: str, domain: list):
        """Add a new variable to the CSP.

        Parameters
        ----------
        name : str
            The name of the variable to add
        domain : list
            A list of the legal values for the variable
        """
        self.variables.append(name)
        self.domains[name] = list(domain)
        self.constraints[name] = {}

    def get_all_possible_pairs(self, a: list, b: list) -> list[tuple]:
        """Get a list of all possible pairs (as tuples) of the values in
        lists 'a' and 'b', where the first component comes from list
        'a' and the second component comes from list 'b'.

        Parameters
        ----------
        a : list
            First list of values
        b : list
            Second list of values

        Returns
        -------
        list[tuple]
            List of tuples in the form (a, b)
        """
        return prod(a, b)

    def get_all_arcs(self) -> list[tuple]:
        """Get a list of all arcs/constraints that have been defined in
        the CSP.

        Returns
        -------
        list[tuple]
            A list of tuples in the form (i, j), which represent a
            constraint between variable `i` and `j`
        """
        return [(i, j) for i in self.constraints for j in self.constraints[i]]

    def get_all_neighboring_arcs(self, var: str) -> list[tuple]:
        """Get a list of all arcs/constraints going to/from variable 'var'.

        Parameters
        ----------
        var : str
            Name of the variable

        Returns
        -------
        list[tuple]
            A list of all arcs/constraints in which `var` is involved
        """
        return [(i, var) for i in self.constraints[var]]

    def add_constraint_one_way(self, i: str, j: str,
                               filter_function: callable):
        """Add a new constraint between variables 'i' and 'j'. Legal
        values are specified by supplying a function 'filter_function',
        that should return True for legal value pairs, and False for
        illegal value pairs.

        NB! This method only adds the constraint one way, from i -> j.
        You must ensure to call the function the other way around, in
        order to add the constraint the from j -> i, as all constraints
        are supposed to be two-way connections!

        Parameters
        ----------
        i : str
            Name of the first variable
        j : str
            Name of the second variable
        filter_function : callable
            A callable (function name) that needs to return a boolean.
            This will filter value pairs which pass the condition and
            keep away those that don't pass your filter.
        """
        if j not in self.constraints[i]:
            # First, get a list of all possible pairs of values
            # between variables i and j
            self.constraints[i][j] = self.get_all_possible_pairs(
                self.domains[i],
                self.domains[j])

        # Next, filter this list of value pairs through the function
        # 'filter_function', so that only the legal value pairs remain
        self.constraints[i][j] = list(filter(lambda
                                             value_pair:
                                             filter_function(*value_pair),
                                             self.constraints[i][j]))

    def add_all_different_constraint(self, var_list: list):
        """Add an Alldiff constraint between all of the variables in the
        list provided.

        Parameters
        ----------
        var_list : list
            A list of variable names
        """
        for (i, j) in self.get_all_possible_pairs(var_list, var_list):
            if i != j:
                self.add_constraint_one_way(i, j, lambda x, y: x != y)

    def backtracking_search(self):
        """This functions starts the CSP solver and returns the found
        solution.
        """
        # first reset the stats
        self.backtrack_count = 0
        self.backtrack_count_failure = 0

        # Make a so-called "deep copy" of the dictionary containing the
        # domains of the CSP variables. The deep copy is required to
        # ensure that any changes made to 'assignment' does not have any
        # side effects elsewhere.
        assignment = copy.deepcopy(self.domains)

        # Run AC-3 on all constraints in the CSP, to weed out all of the
        # values that are not arc-consistent to begin with
        self.inference(assignment, self.get_all_arcs())

        # Call backtrack with the partial assignment 'assignment'
        return self.backtrack(assignment)

    def backtrack(self, assignment: dict) -> dict:
        """The function 'Backtrack' from the pseudocode in the
        textbook.

        The function is called recursively, with a partial assignment of
        values 'assignment'. 'assignment' is a dictionary that contains
        a list of all legal values for the variables that have *not* yet
        been decided, and a list of only a single value for the
        variables that *have* been decided.

        When all of the variables in 'assignment' have lists of length
        one, i.e. when all variables have been assigned a value, the
        function should return 'assignment'. Otherwise, the search
        should continue. When the function 'inference' is called to run
        the AC-3 algorithm, the lists of legal values in 'assignment'
        should get reduced as AC-3 discovers illegal values.

        IMPORTANT: For every iteration of the for-loop in the
        pseudocode, you need to make a deep copy of 'assignment' into a
        new variable before changing it. Every iteration of the for-loop
        should have a clean slate and not see any traces of the old
        assignments and inferences that took place in previous
        iterations of the loop.
        """
        # increment stats
        self.backtrack_count += 1

        # check if the assignment is complete, i.e for every variable, the variable's domain
        # only has one element
        completed = True
        for values in assignment.values():
            if len(values) > 1:
                completed = False
                break
        if completed:
            return assignment

        # select the next variable
        var = self.select_unassigned_variable(assignment)
        # iterate over every value in its domain
        for value in self.order_domain_values(var, assignment):
            # check if the value is consistent with the assignment
            consistent = True
            # for each arc in the current variables constraints, check if
            # the current value is not included in any of the arcs the other way
            # if the above evaluates to true, this value is not consistent with the assignment
            for arc in self.constraints[var].keys():
                if not value in [constraint[1] for constraint in self.constraints[arc][var]]:
                    consistent = False
                    break

            if consistent:
                # value is consistent, make a deepcopy and set the variables domain to be only the value
                new_assignment = copy.deepcopy(assignment)
                new_assignment[var] = [value]
                # inference step (AC-3)
                if self.inference(new_assignment, self.get_all_neighboring_arcs(var)):
                    # recursive call to backtrack, check if this is indeed a valid placement
                    result = self.backtrack(new_assignment)
                    if result != {}:
                        # it it is not a failure, we return the result
                        return result
        # increment failure stat and return a failure
        self.backtrack_count_failure += 1
        return {}

    def select_unassigned_variable(self, assignment: dict) -> str:
        """The function 'Select-Unassigned-Variable' from the pseudocode
        in the textbook. Should return the name of one of the variables
        in 'assignment' that have not yet been decided, i.e. whose list
        of legal values has a length greater than one.

        Selects the variable with the fewest values by iterating over
        the assignment. Returns the variable with the fewest values.
        """
        # start values
        var_with_fewest_values = choice(
            [var for var in assignment.keys()])  # get a random var
        fewest_values = float("inf")  # set it to infinity
        for var, value in assignment.items():  # iterate over dict
            domain_size = len(value)
            if domain_size == 1:  # case if the var is assigned a value, i.e the size of the list is 1
                continue
            elif domain_size == 2:  # the smallest case, if we encounter it, just return this for saving processing
                return var
            else:
                # minimum-problem
                if domain_size < fewest_values:
                    var_with_fewest_values = var
                    fewest_values = domain_size
        return var_with_fewest_values

    def order_domain_values(self, var: str, assignment: dict):
        """Returns an ordering of domain values. In this case it just
        gives the ordering it already has, but this method can be extended
        to for example choose the value that 'closes' the fewest doors.
        """

        return assignment[var]

    def inference(self, assignment: dict, queue: List):
        """The function 'AC-3' from the pseudocode in the textbook.
        'assignment' is the current partial assignment, that contains
        the lists of legal values for each undecided variable. 'queue'
        is the initial queue of arcs that should be visited.
        """
        while len(queue) != 0:
            arc = queue.pop(0)
            # revise step
            if self.revise(assignment, arc[0], arc[1]):
                if len(assignment[arc[0]]) == 0:
                    # the revise deleted all values for the var, meaning an invalid value
                    return False
                for neighbor in self.get_all_neighboring_arcs(arc[0]):
                    if neighbor != (arc[1], arc[0]):  # stop infinite iteration
                        queue.append((neighbor[0], arc[0]))
        return True

    def revise(self, assignment, i, j):
        """The function 'Revise' from the pseudocode in the textbook.
        'assignment' is the current partial assignment, that contains
        the lists of legal values for each undecided variable. 'i' and
        'j' specifies the arc that should be visited. If a value is
        found in variable i's domain that doesn't satisfy the constraint
        between i and j, the value should be deleted from i's list of
        legal values in 'assignment'.
        """
        revised = False
        constraints: List = self.constraints[i][j]  # list of tuples
        # iterate over every value in i's domain
        for val in assignment[i]:
            possible_values = []
            for v in assignment[j]:
                # special case, equal values
                if v == val:
                    # evil hack
                    # I believe that for sudoko, if we do not also revise the constraints
                    # i.e removing constraints as the domain shrinks, the only way to
                    # revise if we we encounter a variable that has a single element in it's
                    # domain, therefore I added this hack, that checks just that.
                    if len(assignment[j]) == 1:
                        assignment[i].remove(val)
                        revised = True
                        break
                    else:
                        # else dont append the value, since it will then be e.g
                        # a constraint (2, 2), which will always remove the value
                        # from i's domain.
                        continue
                possible_values.append((val, v))
            # iterate over possible values on the form (x, y), where x is from i's domain
            # and y is from j's.
            # as I mention above, I dont believe this ever do anything, but I added it
            # for completeness
            for value in possible_values:
                if not value in constraints:
                    assignment[i].remove(val)
                    revised = True
                    break
        return revised


def create_map_coloring_csp():
    """Instantiate a CSP representing the map coloring problem from the
    textbook. This can be useful for testing your CSP solver as you
    develop your code.
    """
    csp = CSP()
    states = ['WA', 'NT', 'Q', 'NSW', 'V', 'SA', 'T']
    edges = {'SA': ['WA', 'NT', 'Q', 'NSW', 'V'],
             'NT': ['WA', 'Q'], 'NSW': ['Q', 'V']}
    colors = ['red', 'green', 'blue']
    for state in states:
        csp.add_variable(state, colors)
    for state, other_states in edges.items():
        for other_state in other_states:
            csp.add_constraint_one_way(state, other_state, lambda i, j: i != j)
            csp.add_constraint_one_way(other_state, state, lambda i, j: i != j)
    return csp


def create_sudoku_csp(filename: str) -> CSP:
    """Instantiate a CSP representing the Sudoku board found in the text
    file named 'filename' in the current directory.

    Parameters
    ----------
    filename : str
        Filename of the Sudoku board to solve

    Returns
    -------
    CSP
        A CSP instance
    """
    csp = CSP()
    board = list(map(lambda x: x.strip(), open(filename, 'r')))

    for row in range(9):
        for col in range(9):
            if board[row][col] == '0':
                csp.add_variable('%d-%d' % (row, col), list(map(str,
                                                                range(1, 10))))
            else:
                csp.add_variable('%d-%d' % (row, col), [board[row][col]])

    for row in range(9):
        csp.add_all_different_constraint(['%d-%d' % (row, col)
                                          for col in range(9)])
    for col in range(9):
        csp.add_all_different_constraint(['%d-%d' % (row, col)
                                         for row in range(9)])
    for box_row in range(3):
        for box_col in range(3):
            cells = []
            for row in range(box_row * 3, (box_row + 1) * 3):
                for col in range(box_col * 3, (box_col + 1) * 3):
                    cells.append('%d-%d' % (row, col))
            csp.add_all_different_constraint(cells)

    return csp


def print_sudoku_solution(solution):
    """Convert the representation of a Sudoku solution as returned from
    the method CSP.backtracking_search(), into a human readable
    representation.
    """
    for row in range(9):
        for col in range(9):
            print(solution['%d-%d' % (row, col)][0], end=" "),
            if col == 2 or col == 5:
                print('|', end=" "),
        print("")
        if row == 2 or row == 5:
            print('------+-------+------')


def print_csp_stats(csp: CSP):
    print("\nNumber of times backtrack was called:", csp.backtrack_count)
    print("Number of times backtrack returned a failure:",
          csp.backtrack_count_failure)


def solve_sudoku(filename="easy.txt"):
    csp = create_sudoku_csp(filename)
    sol = csp.backtracking_search()
    print_sudoku_solution(sol)
    print_csp_stats(csp)


sodukos = ["easy.txt", "medium.txt", "hard.txt", "veryhard.txt"]

for sudoku in sodukos:
    print("Current sudoku: " + sudoku + "\n\n")
    solve_sudoku(sudoku)
    print("\n\n\n" + "#"*21)
