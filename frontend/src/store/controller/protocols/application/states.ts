// Send state update scheduling

// Note: this function mutates the input array
const advanceSchedule = <T>(schedule: Array<T>): T => {
  const entry = schedule.shift();
  if (entry === undefined) {
    throw new Error('State Synchronization: invalid schedule');
  }
  schedule.push(entry);
  return entry;
};

export default advanceSchedule;
