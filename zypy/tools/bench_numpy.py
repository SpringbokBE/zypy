import numpy as np

def heat2d_numpy(h, w, epsilon=0.005):
    
    grid = np.zeros((h + 2, w + 2), dtype=np.float32)
    grid[:, 0] = -273.15
    grid[:, -1] = -273.15
    grid[-1, :] = -273.15
    grid[0, :] = 40.0
    
    center = grid[1:-1, 1:-1]
    north = grid[:-2, 1:-1]
    south = grid[2:, 1:-1]
    east = grid[1:-1, :-2]
    west = grid[1:-1, 2:]
    delta = epsilon + 1
    
    i = 0
    while delta > epsilon:
        if not (i % (100_000 / h)): print(delta)
        tmp = 0.2 * (center + north + south + east + west)
        delta = np.sum(np.absolute(tmp - center))
        center[:] = tmp
        i += 1

    return center
