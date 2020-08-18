"""Trio I/O with sans-I/O protocol, running application."""

import trio

import yappi  # type: ignore

from ventserver.application import main


if __name__ == '__main__':
    yappi.set_clock_type('CPU')
    with yappi.run():
        try:
            trio.run(main)
        except KeyboardInterrupt:
            print('Quitting!')
    stats = yappi.get_func_stats()
    stats.sort('tavg').print_all()
    stats.save('callgrind.out', 'CALLGRIND')
