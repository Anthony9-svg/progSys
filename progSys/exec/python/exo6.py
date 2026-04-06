import sys
import multiprocessing

def binomial(n, k, queue):
    if k == 0 or k == n:
        queue.put(1)
        return

    q1 = multiprocessing.Queue()
    q2 = multiprocessing.Queue()

    p1 = multiprocessing.Process(target=binomial, args=(n-1, k-1, q1))
    p2 = multiprocessing.Process(target=binomial, args=(n-1, k, q2))

    p1.start()
    p2.start()

    p1.join()
    p2.join()

    a = q1.get()
    b = q2.get()

    queue.put(a + b)

if __name__ == "__main__":
    if len(sys.argv) < 3:
        print("Usage: python script.py <n> <k>")
        sys.exit(1)

    n = int(sys.argv[1])
    k = int(sys.argv[2])

    q = multiprocessing.Queue()
    binomial(n, k, q)
    print(q.get())