from mpc import MPCPolicy

if __name__ == "__main__":
    policy: MPCPolicy = MPCPolicy()
    policy.set_dt(0.2)
    print(policy.get_dt())