#!/usr/bin/python3
# coding=utf-8

def find_path(weights):
    least_weight = find_least_weight(weights)
    if (type(least_weight) == tuple):
        return [least_weight]
    shortest_paths = {}
    height = len(weights)
    width = len(weights[0])
    for w in range(width):
        path = []
        path.append((w, 0))
        weight = weights[0][w]
        value = least_weight[(w, 0)]
        for i in range(height-1):
            path.append(value)
            weight += weights[value[1]][value[0]]
            if (i < height-2):
                value = least_weight[value]
        
        #weight += weights[value[1]][value[0]]
        shortest_paths[weight] = path
    lowest_weight = min(shortest_paths.keys())
    return shortest_paths[lowest_weight]

def find_least_weight(weights):
    least_weight = {}
    height = len(weights)
    if (height == 1):
        return (weights[0].index(min(weights[0])), 0)

    width = len(weights[0])
    if (width == 0):
        return []
    for h in range(height - 1):
        for w in range(width):
            minimum = 100000000000
            for j in range(3):
                if (w+j-1 < 0 or w+j-1 >= width):
                    continue
                weight = weights[h+1][w+j-1]
                if (weight < minimum):
                    least_weight[(w, h)] = (w+j-1, h + 1)
                    minimum = weight
    return least_weight
# Tester på formatet (vekter, minste mulige vekt på sti).
tests = [
    ([[1]], 1),
    ([[1, 1]], 1),
    ([[1], [1]], 2),
    ([[2, 1], [2, 1]], 2),
    ([[1, 1], [1, 1]], 2),
    ([[2, 1], [1, 2]], 2),
    ([[3, 2, 1], [1, 3, 2], [2, 1, 3]], 4),
    ([[1, 10, 3, 3], [1, 10, 3, 3], [10, 10, 3, 3]], 9),
    ([[1, 2, 7, 4], [9, 3, 2, 5], [5, 7, 8, 3], [1, 3, 4, 6]], 10),
]


# Verifiserer at en løsning er riktig gitt vektene, stien og den minst
# mulige vekten man kan ha på en sti.
def verify(weights, path, optimal):
    if len(path) != len(weights):
        return False, "Stien er enten for lang eller for kort."

    last = -1
    for index, element in enumerate(path):
        if type(element) != tuple:
            return False, "Stien består ikke av tupler."
        if len(element) != 2:
            return False, "Stien består ikke av tupler på formatet (x,y)."
        if index != element[1]:
            return False, "Stien er ikke vertikal."
        if element[0] < 0 or element[0] >= len(weights[0]):
            return False, "Stien går utenfor bildet."
        if last != -1 and not last - 1 <= element[0] <= last + 1:
            return False, "Stien hopper mer enn en piksel per rad."
        last = element[0]

    weight = sum(weights[y][x] for x, y in path)
    if weight != optimal:
        return (
            False,
            "Stien er ikke optimal. En optimal sti ville hatt"
            + "vekten {:}, mens din hadde vekten {:}".format(optimal, weight),
        )

    return True, ""


failed = False

for test, optimal_weight in tests:
    answer = find_path([row[:] for row in test])
    correct, error_message = verify(test, answer, optimal_weight)
    if not correct:
        failed = True
        print(
            'Feilet med feilmeldingen "{:}" for testen '.format(error_message)
            + "{:}. Ditt svar var {:}.".format(test, answer)
        )

if not failed:
    print("Koden din fungerte for alle eksempeltestene.")