import { EventChannel } from 'redux-saga';
import {
  put,
  take,
  takeEvery,
  fork,
  delay,
  select,
  takeLatest,
  ChannelTakeEffect,
  all,
} from 'redux-saga/effects';
import { INITIALIZED, BACKEND_HEARTBEAT } from '../app/types';
import { PBMessageType } from './types';
import { updateState } from './actions';
import { deserialize } from './protocols/backend/transport';
import { sequentialStateSender, getStateProcessor, sendInterval } from './protocols/backend/states';
import { createReceiveChannel, receiveBuffer, sendBuffer, setupConnection } from './io/websocket';
import updateClock from './io/clock';

function* deserializeResponse(response: Response) {
  const buffer = yield receiveBuffer(response);
  return deserialize(buffer);
}

function* receive(response: ChannelTakeEffect<Response>) {
  try {
    const results = yield deserializeResponse(yield response);
    yield put(updateState(results.messageType, results.pbMessage));
    yield put({ type: BACKEND_HEARTBEAT });
  } catch (err) {
    console.error(err);
  }
}

function* receiveAll(channel: EventChannel<Response>) {
  while (true) {
    const response = yield take(channel);
    yield receive(response);
  }
}

function* sendState(sock: WebSocket, pbMessageType: PBMessageType) {
  const processor = getStateProcessor(pbMessageType);
  const pbMessage = yield select(processor.selector);
  if (pbMessage !== null) {
    const body = processor.serializer(pbMessage);
    yield sendBuffer(sock, body);
  }
}

function* sendAll(sock: WebSocket) {
  const sender = sequentialStateSender();
  while (sock.readyState === WebSocket.OPEN) {
    const pbMessageType = sender.next().value;
    yield sendState(sock, pbMessageType);
    yield delay(sendInterval);
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

export function* serviceConnectionPersistently(): IterableIterator<unknown> {
  while (true) {
    yield serviceConnection();
    console.warn('Reestablishing WebSocket connection...');
  }
}

export function* controllerSaga(): IterableIterator<unknown> {
  yield all([
    yield takeEvery(INITIALIZED, serviceConnectionPersistently),
    yield takeLatest(INITIALIZED, updateClock),
  ]);
}

export default controllerSaga;
