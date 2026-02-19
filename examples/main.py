import sys
import time
import pickle
import logging
from queue import Queue
from socket import socket, AF_INET, SOCK_STREAM
from typing import Callable, Tuple, Any, List, Dict
from threading import Thread, Event

sys.path.append('./build')
from mpc.mpc import MPCPolicy


class TCPClient:
    def __init__(self, ip: str, port: int = 8080) -> None:
        self.client_socket: socket = socket(AF_INET, SOCK_STREAM)
        self.address: Tuple[str, int] = (ip, port)

    def connect(self) -> None:
        self.client_socket.connect(self.address)

    def _send_request(self, data: Any) -> Any:
        serialized_data: bytes = pickle.dumps(data)
        self.client_socket.sendall(serialized_data)
        response_data: bytes = self.client_socket.recv(102400)
        return pickle.loads(response_data)

    def step(self, data_queue: Queue) -> Any:
        response_data: Any = None

        try:
            data = data_queue.get()
            response_data = self._send_request(data)
        except Exception as e:
            logging.warning(str(e))
        
        return response_data
    

class TCPServer:
    def __init__(self, port: int = 8080) -> None:
        self.server_socket: socket = socket(AF_INET, SOCK_STREAM)
        self.address: Tuple[str, int] = ('0.0.0.0', port)
        self.server_socket.bind(self.address)

    def reset(self) -> None:
        self.server_socket.listen(1)

    def step(self, data_queue: Queue, response: Callable = lambda *args: "received") -> None:
        # wait for client to connect to the server
        print("The server module is ready to accept connection...")
        self.client_socket, self.client_address = self.server_socket.accept()
        print(f"Connected to {self.client_address}")

        # transmit image to the client
        client_is_alive: bool = True
        while client_is_alive:
            try:
                # receive the data from the client
                serialized_data: bytes = self.client_socket.recv(102400)
                data: Any = pickle.loads(serialized_data)
                data_queue.put(data) 
                print(f"Received data: {data}")

                # send the response back to the client
                response_data: Any = response(data)
                serialized_data: bytes = pickle.dumps(response_data)
                self.client_socket.sendall(serialized_data)
            except Exception as e:
                logging.warning(str(e))
                client_is_alive = False

        # close the connection if there's any issues
        self.client_address = ''
        self.client_socket.close()
        print("Disconnected to the client")


def run_tcp_server(obs_queue: Queue, act_queue: Queue, event: Event) -> None:
    server: TCPServer = TCPServer(8080)
    while True:
        server.reset()
        event.set()
        server.step(obs_queue, lambda obs: act_queue.get())
        event.clear()


def run_mpc_policy(obs_queue: Queue, act_queue: Queue, event: Event) -> None:
    policy: MPCPolicy = MPCPolicy()
    while True:
        event.wait()
        policy.reset()
        policy.set_dt(0.2)
        policy.set_lf(0.19)
        while event.is_set():
            start: float = time.perf_counter()
            obs: Dict[str, Any] = obs_queue.get()
            waypoints: List[float] = obs["waypoints"]
            state: List[float] = obs["state_info"]

            action: List[float] = policy.step(waypoints, state)
            act_queue.put(action)
            elapsed: float = time.perf_counter() - start
            time.sleep(max(0.0, policy.get_dt() - elapsed))


if __name__ == "__main__":
    obs_queue: Queue = Queue(5)
    act_queue: Queue = Queue(5)
    event: Event = Event()

    server_thread: Thread = Thread(
        target=run_tcp_server, args=(obs_queue, act_queue, event)
    )
    mpc_thread: Thread = Thread(
        target=run_mpc_policy, args=(obs_queue, act_queue, event)
    )
    server_thread.start()
    mpc_thread.start()
