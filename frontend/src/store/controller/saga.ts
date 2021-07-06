import { EventChannel } from 'redux-saga';
import { take, takeEvery, fork, delay, takeLatest, all } from 'redux-saga/effects';
import { INITIALIZED } from '../app/types';
import {
  receiveState,
  stateSender,
  GeneratorYieldType,
  SenderYield,
} from './protocols/backend/backend';
import { createReceiveChannel, receiveBuffer, sendBuffer, setupConnection } from './io/websocket';
import updateClock from './io/clock';

function* receiveAll(channel: EventChannel<Response>) {
  while (true) {
    const response = yield take(channel);
    const body = yield receiveBuffer(yield response);
    yield receiveState(body);
  }
}

function* sendAll(sock: WebSocket) {
  const sender = stateSender();
  let effectResult = null;
  while (sock.readyState === WebSocket.OPEN) {
    const yieldValue: SenderYield = sender.next(effectResult).value;
    switch (yieldValue.type) {
      case GeneratorYieldType.Effect: {
        effectResult = yield yieldValue.value;
        break;
      }
      case GeneratorYieldType.Result: {
        const body = yieldValue.value as Uint8Array | null;
        if (body !== null) {
          yield sendBuffer(sock, body);
        }
        effectResult = null;
        break;
      }
      default:
        break;
    }
  }
}

function* serviceConnection() {
  const { sock, connectionChannel } = yield setupConnection();
  const receiveChannel = createReceiveChannel(sock);
  yield fork(receiveAll, receiveChannel);
  yield fork(sendAll, sock);
  yield take(connectionChannel);
  receiveChannel.close();
}

const retryConnectInterval = 10; // ms

export function* serviceConnectionPersistently(): IterableIterator<unknown> {
  while (true) {
    yield serviceConnection();
    console.warn('Reestablishing WebSocket connection...');
    yield delay(retryConnectInterval);
  }
}

export function* controllerSaga(): IterableIterator<unknown> {
  yield all([
    yield takeEvery(INITIALIZED, serviceConnectionPersistently),
    yield takeLatest(INITIALIZED, updateClock),
  ]);
}

export default controllerSaga;
