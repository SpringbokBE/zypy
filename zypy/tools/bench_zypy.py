import zypy as np

def heat2d_zypy(h, w, epsilon=0.005):
    
    np.hint("filter_avg")
    
    grids = [
        np.allocate((h + 2, w + 2), dtype=np.float32),
        np.allocate((h + 2, w + 2), dtype=np.float32)
    ]
    
    grids[0][:] = 0
    grids[0][:, 0] = -273.15
    grids[0][:, -1] = -273.15
    grids[0][-1, :] = -273.15
    grids[0][0, :] = 40.0
    
    delta = epsilon + 1
    
    i = 0
    switch = False
    while delta > epsilon:
#         if not (i % (100_000 / h)): print(delta)
        np.filter_avg(grids[switch], grids[not switch])
        delta = np.sad(grids[0], grids[1])
        switch = not switch
        i += 1
    print(f"i = {i}")
    return grids[0]
