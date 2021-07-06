import { SelectEffect, PutEffect, CallEffect, select, put, delay } from 'redux-saga/effects';
import { AppAction } from '../../../app/types';
import { receivedBackendHeartbeat } from '../../../app/actions';
import { updateState } from '../../actions';
import { PBMessage, StateUpdateAction } from '../../types';
import { serialize, deserialize } from './transport';
import { getSelector, backendStateSender, sendInterval } from './states';

export function* receiveState(
  body: Uint8Array,
): Generator<PutEffect<StateUpdateAction | AppAction>, void, void> {
  try {
    const results = deserialize(body);
    yield put(updateState(results.messageType, results.pbMessage));
    yield put(receivedBackendHeartbeat());
  } catch (err) {
    console.error(err);
  }
}

export enum GeneratorYieldType {
  Effect,
  Result,
}
type SenderYieldResult = Uint8Array | null;
type SenderYieldEffect = SelectEffect | CallEffect;
export interface SenderYield {
  type: GeneratorYieldType;
  value: SenderYieldResult | SenderYieldEffect;
}

const makeYieldEffect = (effect: SenderYieldEffect): SenderYield => ({
  type: GeneratorYieldType.Effect,
  value: effect,
});
const makeYieldResult = (result: SenderYieldResult): SenderYield => ({
  type: GeneratorYieldType.Result,
  value: result,
});

// This generator is tagged as returning SenderYield to make typescript eslinting
// behave nicely, but it actually never returns (it only yields).
// It yields either redux-saga effects (which require an input into the generator's
// subsequent next() method call) or optional byte buffers to send.
export function* stateSender(
  skipUnavailable = false,
): Generator<SenderYield, SenderYield, PBMessage> {
  const schedule = backendStateSender();
  while (true) {
    const pbMessageType = schedule.next().value;
    const selector = getSelector(pbMessageType);
    const pbMessage = yield makeYieldEffect(select(selector));
    const body = pbMessage === null ? null : serialize(pbMessageType, pbMessage as PBMessage);
    yield makeYieldResult(body);
    if (pbMessage !== null || !skipUnavailable) {
      yield makeYieldEffect(delay(sendInterval));
    }
  }
}
