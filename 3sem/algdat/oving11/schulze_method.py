def schulze_method(A):
    P = A
    n = len(A)
    for i in range(n):
        for j in range(n):
            if (i != j):
                if A[i][j] > A[j][i]:
                    P[i][j] = A[i][j]
                else:
                    P[i][j] = 0

    for i in range(n):
        for j in range(n):
            if i != j:
                for k in range(n):
                    if i != k and j != k:
                        P[j][k] = max(P[i][k], min(P[j][i], P[i][k]))
    winners = []
    C = [i for i in range(n)]
    for d in range(n):
        for i in C:
            greater = True
                    # equal_greater = True
            for j in range(n):
                if i != j:
                    if P[i][j] < P[j][i]:
                        greater = False
                        break
                    elif P[i][j] == P[j][i]:
                                num_i = 0
                                num_j = 0
                                for k in range(n):
                                    if P[i][k] > P[k][i]:
                                        num_i += 1
                                    if P[j][k] > P[k][j]:
                                        num_j += 1
                                if num_j > num_i:
                                    # equal_greater = False
                                    greater = False
            if greater:
                winners.append(i)
                C.remove(i)
            print(C)
            if (len(C) == 1):
                winners.append(C[-1])
    return winners
                
                    

from copy import deepcopy

tests = [
    ([[0]], [0]),
    ([[0, 1], [3, 0]], [1, 0]),
    ([[0, 2], [2, 0]], [0, 1]),
    ([[0, 4, 3], [2, 0, 2], [3, 4, 0]], [0, 2, 1]),
    ([[0, 2, 1], [4, 0, 4], [5, 2, 0]], [1, 2, 0]),
    (
        [
            [0, 1, 3, 3, 3],
            [9, 0, 5, 5, 7],
            [7, 5, 0, 5, 4],
            [7, 5, 5, 0, 6],
            [7, 3, 6, 4, 0],
        ],
        [1, 3, 4, 2, 0],
    ),
    (
        [
            [0, 6, 7, 8, 7, 8],
            [6, 0, 6, 8, 7, 8],
            [5, 6, 0, 6, 5, 7],
            [4, 4, 6, 0, 5, 6],
            [5, 5, 7, 7, 0, 6],
            [4, 4, 5, 6, 6, 0],
        ],
        [0, 1, 4, 2, 3, 5],
    ),
]


def generate_feedback(test, expected, student):
    feedback = ""
    feedback += "Koden din feilet for input\n"
    feedback += str(test) + "\n"
    feedback += "Ditt svar er\n"
    feedback += str(student) + ",\n"
    feedback += "men riktig svar er\n"
    feedback += str(expected) + "."
    return feedback


for test, expected in tests:
    unchanged_input = deepcopy(test)
    student = schulze_method(test)
    n = len(unchanged_input)
    assert (
        len(student) == n
    ), "Listen din inneholder ikke riktig antall kandidater"
    for i in range(n):
        assert student[i] == expected[i], generate_feedback(
            unchanged_input, expected, student
        )

print("Koden din passerte alle testene.")
