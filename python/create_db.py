import json

letters = 'abcdefghijklmnopqrstuvwxyz'


def create_seats(Nrow, Ncol):
    seats = {}
    for r in range(Nrow):
        for c in range(Ncol):
            sname = letters[r] + str(c+1)
            seats[sname] = 0  # 0 means free
    return seats


def create_sessions(tdata):
    sessions = {}
    for name, Nrow, Ncol in tdata:
        sessions[name] = create_seats(Nrow, Ncol)

    return sessions

def main():
    movies = ["Movie1", "Movie2", "Movie3"]
    theaters = [("Theater1", 2, 3), ("Theater2", 2, 3), ("Theater3", 2, 3)]

    booking = {}
    for idx, m in enumerate(movies):
        idx0, idx1 = idx % len(theaters), (idx+1) % len(theaters),
        booking[m] = create_sessions([theaters[idx0], theaters[idx1]])

    with open(r"../test/test_movie_sessions.json", "w") as f:
        json.dump(booking, f, indent=2, sort_keys=True)


if __name__ == "__main__":
    main()