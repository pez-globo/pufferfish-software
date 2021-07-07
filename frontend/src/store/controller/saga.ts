import { EventChannel } from 'redux-saga';
import { take, takeEvery, fork, delay, takeLatest, all } from 'redux-saga/effects';
import { INITIALIZED } from '../app/types';
import { GeneratorYieldType } from './protocols/sagas';
import {
  receive as backendReceive,
  sender as backendSender,
  SenderYieldResult,
  SenderYield,
} from './protocols/backend/backend';
import { createReceiveChannel, receiveBuffer, sendBuffer, setupConnection } from './io/websocket';
import updateClock from './io/clock';

function* receiveAll(channel: EventChannel<Response>) {
  while (true) {
    const response = yield take(channel);
    const body = yield receiveBuffer(yield response);
    yield backendReceive(body);
  }
}

function* sendAll(sock: WebSocket) {
  const sender = backendSender();
  let nextInput = null;
  while (sock.readyState === WebSocket.OPEN) {
    // Service results and effects yielded by the backend sender
    const yieldValue: SenderYield = sender.next(nextInput).value;
    switch (yieldValue.type) {
      case GeneratorYieldType.Result: {
        nextInput = null;
        const body = yieldValue.value as SenderYieldResult;
        yield sendBuffer(sock, body);
        break;
      }
      case GeneratorYieldType.Effect: {
        nextInput = yield yieldValue.value;
        break;
      }
      default:
        throw new Error('Unhandled generator yield type!');
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
