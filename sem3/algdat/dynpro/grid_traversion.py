def f(x, y):
    m = max(x, y)
    grid = [[1 for i in range(m)] for u in range(m)]
    for j in range(1, m):
        for i in range(j, m):
            grid[j][i] = grid[j][i-1] + grid[j-1][i]
            grid[i][j] = grid[j][i]
    return grid[x-1][y-1]


tests = [
    (1, 1, 1),
    (1, 2, 1),
    (3, 3, 6),
    (3, 5, 15),
    (9, 13, 125970),
    (13, 7, 18564),
    (15, 15, 40116600),
]

for x, y, solution in tests:
    found_solution = f(x, y)
    if solution != found_solution:
        print(
            f"Feilet for testen x={x} y={y}, resulterte i svaret"
            f" {found_solution} i stedet for {solution}."
        )