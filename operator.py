import socket
import time

class TowerDefenseAI:
    def __init__(self, host='localhost', port=12345):
        self.host = host
        self.port = port

    def send_command(self, command):
        try:
            with socket.socket(socket.AF_INET, socket.SOCK_STREAM) as s:
                s.connect((self.host, self.port))
                s.sendall(f"{command}\n".encode())
                response = s.recv(1024)
                print(f"Port {self.port}: Command '{command}' sent. Received: {response.decode()}")
        except ConnectionRefusedError:
            print(f"Port {self.port}: Connection refused. Is the server running?")
        except Exception as e:
            print(f"Port {self.port}: An error occurred: {str(e)}")

    def play(self):
        self.start_game()
        self.place_initial_towers()
        while True:
            self.analyze_and_act()
            time.sleep(1)

    def start_game(self):
        self.send_command("startGame")

    def place_initial_towers(self):
        self.send_command("placeTower 1 5 5")
        self.send_command("placeTower 2 10 10")

    def analyze_and_act(self):
        for x in range(5, 15, 5):
            for y in range(5, 15, 5):
                self.send_command(f"upgradeTower {x} {y}")

if __name__ == "__main__":
    ai = TowerDefenseAI()
    ai.play()
