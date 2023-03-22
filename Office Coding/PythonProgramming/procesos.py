import multiprocessing
import time

def worker(name, delay):
    print(f"Worker {name} starting")
    time.sleep(delay)
    print(f"Worker {name} finished")

if __name__ == '__main__':
    processes = []
    for i in range(3):
        p = multiprocessing.Process(target=worker, args=(i, i+1))
        processes.append(p)
        p.start()

    for process in processes:
        process.join()

    print("All processes finished")
    