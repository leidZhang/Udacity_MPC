import sys
sys.path.append("build/")
from mpc import MPC 

if __name__ == "__main__":
    mpc = MPC()
    print(mpc.__doc__)
    for i in range(1):
        res = mpc.solve([i, i, i, i, i, i], [1])
        print("Res: ", res)