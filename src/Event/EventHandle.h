#ifndef CMF_EVENTHANDLE_H
#define CMF_EVENTHANDLE_H

// TODO: make sure objects have a way to retrieve max event delay from them, then when scanning for events in a thread
// TODO: event must be waited for at most the amount specified by its owning object. If done in a entity, it cannot wait for events so long as to over-delay next tick
// TODO: wait for each event handle at rate of smallest delay / (num of handles scanned - 1), this ensures that each event will be at most delayed for the minimum delay between them all
class EventHandle {

};

#endif //CMF_EVENTHANDLE_H