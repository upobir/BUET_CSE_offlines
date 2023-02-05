class ProgressBar:
    def __init__(self, bar_count):
        self.bar_count = bar_count
        self.cur_progress = 0

    def update(self, progress):
        progress = int(progress * self.bar_count)
        while progress > self.cur_progress and self.cur_progress < self.bar_count:
            print('|', end='', flush=True)
            self.cur_progress += 1
        if progress == self.bar_count:
            print()