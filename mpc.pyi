from typing import List

class MPCPolicy:
    def __init__(self) -> None: 
        ...

    def reset() -> None:
        ...

    def step(next_waypoints: List[List[float]], state: List[float]) -> List[float]:
        ...

    def set_dt(dt: float) -> None:
        ...

    def get_dt() -> float:
        ...

    def get_lf() -> float:
        ...
