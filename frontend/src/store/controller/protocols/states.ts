import { PBMessageType } from '../types';

// Send state update scheduling

export type Schedule = Array<PBMessageType>;

// Note: this function mutates the input array
export const advanceSchedule = (schedule: Schedule): PBMessageType => {
  const entry = schedule.shift();
  if (entry === undefined) {
    throw new Error('State Synchronization: invalid schedule');
  }
  schedule.push(entry);
  return entry;
};
