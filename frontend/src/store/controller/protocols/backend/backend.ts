import { SelectEffect, PutEffect, select, put } from 'redux-saga/effects';
import { AppAction } from '../../../app/types';
import { receivedBackendHeartbeat } from '../../../app/actions';
import { updateState } from '../../actions';
import { PBMessageType, PBMessage, StateUpdateAction } from '../../types';
import { serialize, deserialize } from './transport';
import { getSelector } from './states';

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

export function* sendState(
  pbMessageType: PBMessageType,
): Generator<SelectEffect, Uint8Array | null, PBMessage> {
  const pbMessage = yield select(getSelector(pbMessageType));
  if (pbMessage === null) {
    return null;
  }

  return serialize(pbMessageType, pbMessage as PBMessage);
}
