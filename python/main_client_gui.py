import sys

from PySide6.QtWidgets import QApplication, QWidget, QFormLayout, QComboBox, QLineEdit, QPushButton, QMessageBox
import zmq

class Client(object):

    def __init__(self, host="localhost", port = 8888):
        context = zmq.Context()
        print("Connecting to server...")
        self.socket = context.socket(zmq.REQ)
        self.socket.connect(f"tcp://{host}:{port}")

    def send_request(self, request):
        print("Sending request:", request)
        self.socket.send(request.encode())
        #  Get the reply.
        message = self.socket.recv().decode()
        reply = message.split()

        return reply[0], reply[1:]


def fill_qbox(qbox, items):
    qbox.clear()
    qbox.insertItems(0, items)

class BookingWidget(QWidget):
    def __init__(self, host="localhost", port = 8888):
        super().__init__(parent=None)
        self.setFixedWidth(600)
        self.client = Client(host, port)

        self.movies = QComboBox()
        self.theaters = QComboBox()
        self.available_seats = QLineEdit()
        self.refresh_btn = QPushButton("refresh available seats")
        self.request_seats = QLineEdit()
        self.book_seats_btn = QPushButton("book seats")

        layout = QFormLayout()
        layout.addRow("movies", self.movies)
        layout.addRow("theaters", self.theaters)
        layout.addRow("refresh available", self.refresh_btn)
        layout.addRow("available seats", self.available_seats)
        layout.addRow("book seats", self.request_seats)
        layout.addRow("request booking", self.book_seats_btn)

        self.setLayout(layout)

        self.movies.currentTextChanged.connect(self.movies_currentTextChanged)
        self.theaters.currentTextChanged.connect(self.theaters_currentTextChanged)
        self.refresh_btn.clicked.connect(self.refresh_btn_clicked)
        self.book_seats_btn.clicked.connect(self.book_seats_btn_clicked)

        status, movies = self.client.send_request("get_movies")
        if status == "OK":
            fill_qbox(self.movies, movies)

    def movies_currentTextChanged(self):
        movie = self.movies.currentText()
        status, theaters = self.client.send_request(f"get_theaters {movie}")
        if status == "OK":
            fill_qbox(self.theaters, theaters)

    def theaters_currentTextChanged(self):
        self.refresh_btn_clicked()

    def refresh_btn_clicked(self):
        movie = self.movies.currentText()
        theater = self.theaters.currentText()
        status, available_seats = self.client.send_request(f"get_available_seats {movie} {theater}")
        if status == "OK":
            self.available_seats.setText(" ".join(available_seats))

    def book_seats_btn_clicked(self):
        movie = self.movies.currentText()
        theater = self.theaters.currentText()
        req_seats =  self.request_seats.text()
        status, _ = self.client.send_request(f"book_seats {movie} {theater} {req_seats}")
        if status == "OK":
            QMessageBox.information(self, "booking done", f"the seats {req_seats} are booked")
        else:
            QMessageBox.critical(self, "error while booking", f"There were a issue. nothing booked:\n{status}")
        self.refresh_btn_clicked()


if __name__ == "__main__":
    app = QApplication([])
    window = BookingWidget()
    window.show()
    sys.exit(app.exec())
