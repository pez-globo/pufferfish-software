// Plots

import { createSelector, OutputSelector } from 'reselect';
import { getController } from '../controller/selectors';
import {
  ControllerStates,
  Plots,
  WaveformHistories,
  WaveformHistory,
  WaveformPoint,
  PVHistory,
  PVPoint,
} from '../controller/types';
import { StoreState } from '../types';

export const getPlots = createSelector(
  getController,
  (states: ControllerStates): Plots => states.plots,
);
// Generic waveforms
export const getWaveforms = createSelector(
  getPlots,
  (plots: Plots): WaveformHistories => plots.waveforms,
);
type WaveformSelector = OutputSelector<
  StoreState,
  WaveformHistory,
  (res: WaveformHistories) => WaveformHistory
>;
const waveformOldSelector = (waveformSelector: WaveformSelector) =>
  createSelector(
    waveformSelector,
    (waveformHistory: WaveformHistory): WaveformPoint[] => waveformHistory.waveformOld.full,
  );
const waveformNewSegmentsSelector = (waveformSelector: WaveformSelector) =>
  createSelector(
    waveformSelector,
    (waveformHistory: WaveformHistory): WaveformPoint[][] => waveformHistory.waveformNew.segmented,
  );
const waveformNewSegmentSelector = (
  waveformNewSegmentsSelector: OutputSelector<
    StoreState,
    WaveformPoint[][],
    (res: WaveformHistory) => WaveformPoint[][]
  >,
) => (segmentIndex: number) =>
  createSelector(
    waveformNewSegmentsSelector,
    (waveformSegments: WaveformPoint[][]): WaveformPoint[] => waveformSegments[segmentIndex],
  );
// Paw Waveform
export const getWaveformPaw = createSelector(
  getWaveforms,
  (histories: WaveformHistories): WaveformHistory => histories.paw,
);
export const getWaveformPawOld = waveformOldSelector(getWaveformPaw);
export const getWaveformPawNewSegments = waveformNewSegmentsSelector(getWaveformPaw);
export const getWaveformPawNewSegment = waveformNewSegmentSelector(getWaveformPawNewSegments);
// Flow Waveform
export const getWaveformFlow = createSelector(
  getWaveforms,
  (histories: WaveformHistories): WaveformHistory => histories.paw,
);
export const getWaveformFlowOld = waveformOldSelector(getWaveformFlow);
export const getWaveformFlowNewSegments = waveformNewSegmentsSelector(getWaveformFlow);
export const getWaveformFlowNewSegment = waveformNewSegmentSelector(getWaveformFlowNewSegments);
// Volume Waveform
export const getWaveformVolume = createSelector(
  getWaveforms,
  (histories: WaveformHistories): WaveformHistory => histories.paw,
);
export const getWaveformVolumeOld = waveformOldSelector(getWaveformVolume);
export const getWaveformVolumeNewSegments = waveformNewSegmentsSelector(getWaveformVolume);
export const getWaveformVolumeNewSegment = waveformNewSegmentSelector(getWaveformVolumeNewSegments);
// P-V Loop
export const getPVHistory = createSelector(getPlots, (plots: Plots): PVHistory => plots.pvLoop);
export const getPVLoop = createSelector(
  getPVHistory,
  (pvHistory: PVHistory): PVPoint[] => pvHistory.loop,
);
