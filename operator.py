import socket
import time
import random

class TowerDefenseAI:
    def __init__(self, host='localhost', port=8081):
        self.host = host
        self.port = port

    def send_command(self, command):
        try:
            with socket.socket(socket.AF_INET, socket.SOCK_STREAM) as s:
                s.connect((self.host, self.port))
                s.sendall(f"{command}\n".encode())
                response = s.recv(1024).decode()
                print(f"Port {self.port}: Command '{command}' sent. Received: {response}")
                return response
        except ConnectionRefusedError:
            print(f"Port {self.port}: Connection refused. Is the server running?")
        except Exception as e:
            print(f"Port {self.port}: An error occurred: {str(e)}")
        return ""

    def play(self):
        while True:
            self.start_game()
            self.allocate_turrets_randomly()
            while not self.check_game_over():
                self.random_game_actions()
                time.sleep(1)  # Adjust the sleep time based on game requirements
            self.reset_game()

    def start_game(self):
        self.send_command("startGame")

    def allocate_turrets_randomly(self):
        for _ in range(10):  # Example: place 10 turrets
            tower_type = random.randint(1, 5)  # Assuming tower types are 1-5
            grid_x = random.randint(0, 14)  # Assuming grid size 15x15
            grid_y = random.randint(0, 14)
            self.send_command(f"placeTower {tower_type} {grid_x} {grid_y}")

    def upgrade_turrets_randomly(self):
        for _ in range(5):  # Example: attempt to upgrade 5 turrets
            grid_x = random.randint(0, 14)  # Assuming grid size 15x15
            grid_y = random.randint(0, 14)
            self.send_command(f"upgradeTower {grid_x} {grid_y}")

    def sell_turrets_randomly(self):
        for _ in range(5):  # Example: attempt to sell 5 turrets
            grid_x = random.randint(0, 14)  # Assuming grid size 15x15
            grid_y = random.randint(0, 14)
            self.send_command(f"sellTower {grid_x} {grid_y}")

    def random_game_actions(self):
        action = random.choice(['place', 'upgrade', 'sell'])
        if action == 'place':
            tower_type = random.randint(1, 5)
            grid_x = random.randint(0, 14)
            grid_y = random.randint(0, 14)
            self.send_command(f"placeTower {tower_type} {grid_x} {grid_y}")
        elif action == 'upgrade':
            grid_x = random.randint(0, 14)
            grid_y = random.randint(0, 14)
            self.send_command(f"upgradeTower {grid_x} {grid_y}")
        elif action == 'sell':
            grid_x = random.randint(0, 14)
            grid_y = random.randint(0, 14)
            self.send_command(f"sellTower {grid_x} {grid_y}")

    def check_game_over(self):
        response = self.send_command("getGameOver")
        return "true" in response.lower()

    def reset_game(self):
        self.send_command("resetGame")

if __name__ == "__main__":
    ai = TowerDefenseAI()
    ai.play()
