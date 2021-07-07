import { SelectEffect, PutEffect, CallEffect, put, delay } from 'redux-saga/effects';
import { AppAction } from '../../../app/types';
import { receivedBackendHeartbeat } from '../../../app/actions';
import { updateState } from '../../actions';
import { StateUpdateAction } from '../../types';
import { GeneratorYieldType, GeneratorYield, makeYieldResult, makeYieldEffect } from '../sagas';
import {
  SenderYield as StateSenderYield,
  SenderYieldEffect as StateSenderYieldEffect,
} from '../application/states';
import { serialize, deserialize } from './transport';
import { TaggedPBMessage, backendStateSender, sendMinInterval } from './states';

export function* receive(
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

export type SenderYieldResult = Uint8Array;
export type SenderYieldEffect = SelectEffect | CallEffect;
export type SenderYield = GeneratorYield<SenderYieldResult, SenderYieldEffect>;
const makeSenderYieldResult = (result: SenderYieldResult) =>
  makeYieldResult<SenderYieldResult, SenderYieldEffect>(result);
const makeSenderYieldEffect = (effect: SenderYieldEffect) =>
  makeYieldEffect<SenderYieldResult, SenderYieldEffect>(effect);

// This generator is tagged as returning SenderYield to make typescript eslinting
// behave nicely, but it actually never returns (it only yields).
// It yields either redux-saga effects (which require an input into the generator's
// subsequent next() method call) or byte buffers to send.
export function* sender(): Generator<SenderYield, SenderYield, TaggedPBMessage> {
  const sender = backendStateSender();
  let nextInput = null;
  while (true) {
    const yieldValue: StateSenderYield<TaggedPBMessage> = sender.next(nextInput).value;
    switch (yieldValue.type) {
      case GeneratorYieldType.Result:
        nextInput = null;
        if (yieldValue.value !== null) {
          const { type: messageType, value: pbMessage } = yieldValue.value as TaggedPBMessage;
          yield makeSenderYieldResult(serialize(messageType, pbMessage));
        }
        yield makeSenderYieldEffect(delay(sendMinInterval));
        break;
      case GeneratorYieldType.Effect:
        nextInput = yield {
          type: yieldValue.type,
          value: yieldValue.value as StateSenderYieldEffect,
        };
        break;
    }
  }
}
