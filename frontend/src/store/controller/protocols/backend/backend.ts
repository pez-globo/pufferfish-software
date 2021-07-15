import { SelectEffect, PutEffect, CallEffect, put, delay, select } from 'redux-saga/effects';
import { AppAction } from '../../../app/types';
import { updateState } from '../../actions';
import { StateUpdateAction } from '../../types';
import { GeneratorYieldType, GeneratorYield, makeYieldResult, makeYieldEffect } from '../sagas';
import { serialize, deserialize } from './transport';
import {
  TaggedPBMessage,
  SenderYield as StateSenderYield,
  SenderYieldEffect as StateSenderYieldEffect,
  SenderInputs as StateSenderInputs,
  backendStateSender,
  sendMinInterval,
} from './states';
import { ConnectionAction } from '../../../connection/types';
import { establishedBackendConnection, lostBackendConnection } from '../../../connection/actions';
import { getBackendConnected } from '../../../connection/selectors';

// Buffer Receiving

const connectionTimeout = 2000; // ms

export function* handleConnectionTimeout(): Generator<
  PutEffect<ConnectionAction> | SelectEffect | CallEffect,
  void,
  void | boolean
> {
  yield delay(connectionTimeout);
  const backendConnected = yield select(getBackendConnected);
  if (backendConnected) {
    console.warn('Connection timed out!');
    yield put(lostBackendConnection());
  }
}

export function* receive(
  body: Uint8Array,
): Generator<
  PutEffect<StateUpdateAction | AppAction | ConnectionAction> | SelectEffect,
  void,
  void | boolean
> {
  try {
    const results = deserialize(body);
    const backendConnected = yield select(getBackendConnected);
    if (!backendConnected) {
      yield put(establishedBackendConnection());
    }
    yield put(updateState(results.messageType, results.pbMessage));
  } catch (err) {
    console.error(err);
  }
}

// Buffer Sending

export type SenderYieldResult = Uint8Array;
export type SenderYieldEffect = SelectEffect | CallEffect;
export type SenderYield = GeneratorYield<SenderYieldResult, SenderYieldEffect>;
export type SenderInputs = StateSenderInputs;
// SagaSender is tagged as returning SenderYield to make typescript eslinting
// behave nicely, but it actually never returns (it only yields).
type SagaSender = Generator<SenderYield, SenderYield, SenderInputs>;

// This generator yields byte buffers to send, as well as redux-saga effects (which
// require an input into the generator's subsequent next() method call).
export function* sender(): SagaSender {
  const sender = backendStateSender();
  let nextInput = null;
  while (true) {
    const yieldValue: StateSenderYield = sender.next(nextInput).value;
    switch (yieldValue.type) {
      case GeneratorYieldType.Result:
        nextInput = null;
        if (yieldValue.value !== null) {
          const { type: messageType, value: pbMessage } = yieldValue.value as TaggedPBMessage;
          yield makeYieldResult(serialize(messageType, pbMessage));
        }
        yield makeYieldEffect(delay(sendMinInterval));
        break;
      case GeneratorYieldType.Effect:
        nextInput = yield {
          type: yieldValue.type,
          value: yieldValue.value as StateSenderYieldEffect,
        };
        break;
      default:
        throw new Error('Unhandled generator yield type!');
    }
  }
}
