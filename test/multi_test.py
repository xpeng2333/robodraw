import multiprocessing
import time


class ClockProcess(multiprocessing.Process):
    def __init__(self, interval):
        multiprocessing.Process.__init__(self)
        self.interval = interval

    def run(self):
        while True:
            print("The time is %s" % time.ctime())
            time.sleep(self.interval)


def main():
    p = ClockProcess(15)
    p.start()


if __name__ == '__main__':
    main()
