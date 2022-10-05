# multiAgents.py
# --------------
# Licensing Information:  You are free to use or extend these projects for
# educational purposes provided that (1) you do not distribute or publish
# solutions, (2) you retain this notice, and (3) you provide clear
# attribution to UC Berkeley, including a link to http://ai.berkeley.edu.
#
# Attribution Information: The Pacman AI projects were developed at UC Berkeley.
# The core projects and autograders were primarily created by John DeNero
# (denero@cs.berkeley.edu) and Dan Klein (klein@cs.berkeley.edu).
# Student side autograding was added by Brad Miller, Nick Hay, and
# Pieter Abbeel (pabbeel@cs.berkeley.edu).


from audioop import minmax
from shutil import move
from util import manhattanDistance
from game import Directions
import random
import util

from game import Agent
from pacman import GameState


class ReflexAgent(Agent):
    """
    A reflex agent chooses an action at each choice point by examining
    its alternatives via a state evaluation function.

    The code below is provided as a guide.  You are welcome to change
    it in any way you see fit, so long as you don't touch our method
    headers.
    """

    def getAction(self, gameState: GameState):
        """
        You do not need to change this method, but you're welcome to.

        getAction chooses among the best options according to the evaluation function.

        Just like in the previous project, getAction takes a GameState and returns
        some Directions.X for some X in the set {NORTH, SOUTH, WEST, EAST, STOP}
        """
        # Collect legal moves and successor states
        legalMoves = gameState.getLegalActions()

        # Choose one of the best actions
        scores = [self.evaluationFunction(
            gameState, action) for action in legalMoves]
        bestScore = max(scores)
        bestIndices = [index for index in range(
            len(scores)) if scores[index] == bestScore]
        # Pick randomly among the best
        chosenIndex = random.choice(bestIndices)

        "Add more of your code here if you want to"

        return legalMoves[chosenIndex]

    def evaluationFunction(self, currentGameState, action):
        """
        Design a better evaluation function here.

        The evaluation function takes in the current and proposed successor
        GameStates (pacman.py) and returns a number, where higher numbers are better.

        The code below extracts some useful information from the state, like the
        remaining food (newFood) and Pacman position after moving (newPos).
        newScaredTimes holds the number of moves that each ghost will remain
        scared because of Pacman having eaten a power pellet.

        Print out these variables to see what you're getting, then combine them
        to create a masterful evaluation function.
        """
        # Useful information you can extract from a GameState (pacman.py)
        successorGameState = currentGameState.generatePacmanSuccessor(action)
        newPos = successorGameState.getPacmanPosition()
        newFood = successorGameState.getFood()
        newGhostStates = successorGameState.getGhostStates()
        newScaredTimes = [
            ghostState.scaredTimer for ghostState in newGhostStates]

        "*** YOUR CODE HERE ***"
        return successorGameState.getScore()


def scoreEvaluationFunction(currentGameState):
    """
    This default evaluation function just returns the score of the state.
    The score is the same one displayed in the Pacman GUI.

    This evaluation function is meant for use with adversarial search agents
    (not reflex agents).
    """
    return currentGameState.getScore()


class MultiAgentSearchAgent(Agent):
    """
    This class provides some common elements to all of your
    multi-agent searchers.  Any methods defined here will be available
    to the MinimaxPacmanAgent, AlphaBetaPacmanAgent & ExpectimaxPacmanAgent.

    You *do not* need to make any changes here, but you can if you want to
    add functionality to all your adversarial search agents.  Please do not
    remove anything, however.

    Note: this is an abstract class: one that should not be instantiated.  It's
    only partially specified, and designed to be extended.  Agent (game.py)
    is another abstract class.
    """

    def __init__(self, evalFn='scoreEvaluationFunction', depth='2'):
        self.index = 0  # Pacman is always agent index 0
        self.evaluationFunction = util.lookup(evalFn, globals())
        self.depth = int(depth)


class MinimaxAgent(MultiAgentSearchAgent):
    """
    Your minimax agent (question 2)
    """

    def getAction(self, gameState: GameState):
        """
        Returns the minimax action from the current gameState using self.depth
        and self.evaluationFunction.

        Here are some method calls that might be useful when implementing minimax.

        gameState.getLegalActions(agentIndex):
        Returns a list of legal actions for an agent
        agentIndex=0 means Pacman, ghosts are >= 1

        gameState.generateSuccessor(agentIndex, action):
        Returns the successor game state after an agent takes an action

        gameState.getNumAgents():
        Returns the total number of agents in the game

        gameState.isWin():
        Returns whether or not the game state is a winning state

        gameState.isLose():
        Returns whether or not the game state is a losing state
        """

        def getMaxMove(values):
            """Helper function to fetch the move that gives the highest score."""

            currentMax = -float("inf")
            currentMove = ""

            for score, move in values:
                # check if the score for the node is bigger than the current largest score
                if score > currentMax:
                    currentMax = score
                    currentMove = move

            return currentMax, currentMove

        def getMinMove(values):
            """Helper function to fetch the move that gives the lowest score."""

            currentMin = float("inf")
            currentMove = ""

            for score, move in values:
                # check if the score for the node is smaller than the current smallest score
                if score < currentMin:
                    currentMin = score
                    currentMove = move

            return currentMin, currentMove

        def isTerminal(gameState: GameState):
            """Helper function to check if game is won or lost."""

            return gameState.isWin() or gameState.isLose()

        def minMaxSearchHelper(gameState: GameState, currentDepth, agent):
            """
            Helper function for the min-max-search. Also the driver code for getAction.

            Takes in the gameState, currentDept and the agent to do a max/min search for.
            It first checks if we are done with a depth level, and resets the agent value
            and increases the depth.
            It then checks if we have finished searching the depth, or if the state is terminal.
            If this is false, it returns the appropiate max/min-search (max for the pacman agent
            and min 1 for the ghost agents).
            """

            if agent >= gameState.getNumAgents():
                agent = 0
                currentDepth += 1

            # does not really matter which direction we return
            if isTerminal(gameState) or currentDepth == self.depth:
                return self.evaluationFunction(gameState), Directions.STOP

            # pacman agent
            if agent == 0:
                return maxValue(gameState, currentDepth, agent)
            # ghost agent
            else:
                return minValue(gameState, currentDepth, agent)

        def maxValue(gameState: GameState, currentDepth, agent):
            """
            Does the max search. Returns a tuple with the best move and its score.

            It finds all the possible moves for the agent. For each move
            it recursiveliy does a depth-first search, and calls minMaxSearchHelper as
            the recursive call. It also appends the result of the recursion to the values
            list. When the for loop has terminated, values will contain the "worst" chocies
            for the pacman, assumed the ghost always minimizes.
            """

            values = []

            possibleActions = gameState.getLegalActions(agent)

            for action in possibleActions:
                # generate the state if pacman did this move
                currentState = gameState.generateSuccessor(agent, action)
                # recursive call, notice we call minMaxSeachHelper with agent+1
                values.append((minMaxSearchHelper(
                    currentState, currentDepth, agent+1)[0], action))

            return getMaxMove(values)

        def minValue(gameState: GameState, currentDepth, agent):
            """
            Does the min search. Returns a tuple with the move that minimizes the most.

            It works in the same way as maxValue. The only difference is that it uses
            getMinMove to get the move that minimizes.
            """
            values = []

            possibleActions = gameState.getLegalActions(agent)

            for action in possibleActions:
                currentState = gameState.generateSuccessor(agent, action)
                values.append(minMaxSearchHelper(
                    currentState, currentDepth, agent+1))

            return getMinMove(values)

        # do the min-max search
        result = minMaxSearchHelper(gameState, 0, 0)
        # and return the move
        return result[1]


class AlphaBetaAgent(MultiAgentSearchAgent):
    """
    Your minimax agent with alpha-beta pruning (question 3)
    """

    def getAction(self, gameState):
        """
        Returns the minimax action using self.depth and self.evaluationFunction

        Much of the same as MinmaxAgent's getAction. The only difference is that
        minMaxSearchHelper, minValue and maxValue now also takes in alpha and beta
        as parameters, for pruning purposes. The pruning implementation is also
        changed, just a few simple if-checks.
        """

        def getMaxMove(values):
            """Helper function to fetch the move that gives the highest score."""

            currentMax = -float("inf")
            currentMove = ""

            for score, move in values:
                # check if the score for the node is bigger than the current largest score
                if score > currentMax:
                    currentMax = score
                    currentMove = move

            return currentMax, currentMove

        def getMinMove(values):
            """Helper function to fetch the move that gives the lowest score."""

            currentMin = float("inf")
            currentMove = ""

            for score, move in values:
                # check if the score for the node is smaller than the current smallest score
                if score < currentMin:
                    currentMin = score
                    currentMove = move

            return currentMin, currentMove

        def isTerminal(gameState: GameState):
            """Helper function to check if game is won or lost."""

            return gameState.isWin() or gameState.isLose()

        def minMaxSearchHelper(gameState: GameState, currentDepth, agent, alpha, beta):
            """
            Helper function for the min-max-search. Also the driver code for getAction.

            Takes in the gameState, currentDept and the agent to do a max/min search for.
            It first checks if we are done with a depth level, and resets the agent value
            and increases the depth.
            It then checks if we have finished searching the depth, or if the state is terminal.
            If this is false, it returns the appropiate max/min-search (max for the pacman agent
            and min 1 for the ghost agents).
            """

            if agent >= gameState.getNumAgents():
                agent = 0
                currentDepth += 1

            # does not really matter which direction we return
            if isTerminal(gameState) or currentDepth == self.depth:
                return self.evaluationFunction(gameState), Directions.STOP

            # pacman agent
            if agent == 0:
                return maxValue(gameState, currentDepth, agent, alpha, beta)
            # ghost agent
            else:
                return minValue(gameState, currentDepth, agent, alpha, beta)

        def maxValue(gameState: GameState, currentDepth, agent, alpha, beta):
            """
            Does the max search. Returns a tuple with the best move and its score.

            It finds all the possible moves for the agent. For each move
            it recursiveliy does a depth-first search, and calls minMaxSearchHelper as
            the recursive call. It also appends the result of the recursion to the values
            list. When the for loop has terminated, values will contain the "worst" chocies
            for the pacman, assumed the ghost always minimizes.
            """

            values = []

            possibleActions = gameState.getLegalActions(agent)

            for action in possibleActions:
                # generate the state if pacman did this move
                currentState = gameState.generateSuccessor(agent, action)
                # recursive call, notice we call minMaxSeachHelper with agent+1
                score, move = minMaxSearchHelper(
                    currentState, currentDepth, agent+1, alpha, beta)
                values.append((score, action))
                # pruning
                if score > alpha:
                    alpha = score
                # the actual pruning
                if score > beta:
                    # dont return a value, just break and return in getMaxMove
                    break

            return getMaxMove(values)

        def minValue(gameState: GameState, currentDepth, agent, alpha, beta):
            """
            Does the min search. Returns a tuple with the move that minimizes the most.

            It works in the same way as maxValue. The only difference is that it uses
            getMinMove to get the move that minimizes.
            """
            values = []

            possibleActions = gameState.getLegalActions(agent)

            for action in possibleActions:
                currentState = gameState.generateSuccessor(agent, action)
                score, move = minMaxSearchHelper(
                    currentState, currentDepth, agent+1, alpha, beta)
                values.append((score, action))
                # pruning
                if score < beta:
                    beta = score
                # the actual pruning
                if score < alpha:
                    # dont return a value, just break and return in getMinMove
                    break

            return getMinMove(values)

        # do the min-max search
        result = minMaxSearchHelper(
            gameState, 0, 0, -float("inf"), float("inf"))
        # and return the move
        return result[1]


class ExpectimaxAgent(MultiAgentSearchAgent):
    """
      Your expectimax agent (question 4)
    """

    def getAction(self, gameState):
        """
        Returns the expectimax action using self.depth and self.evaluationFunction

        All ghosts should be modeled as choosing uniformly at random from their
        legal moves.
        """
        "*** YOUR CODE HERE ***"
        util.raiseNotDefined()


def betterEvaluationFunction(currentGameState):
    """
    Your extreme ghost-hunting, pellet-nabbing, food-gobbling, unstoppable
    evaluation function (question 5).

    DESCRIPTION: <write something here so we know what you did>
    """
    "*** YOUR CODE HERE ***"
    util.raiseNotDefined()


# Abbreviation
better = betterEvaluationFunction
