import { EventChannel } from 'redux-saga';
import {
  put,
  take,
  takeEvery,
  fork,
  delay,
  select,
  takeLatest,
  all,
  ChannelTakeEffect,
} from 'redux-saga/effects';
import { PBMessageType } from './types';
import { ParametersRequest } from './proto/mcu_pb';
import { INITIALIZED, CLOCK_UPDATED } from '../app/types';
import { updateState } from './actions';
import { deserializeMessage } from './protocols/messages';
import { getStateProcessor } from './protocols/backend';
import {
  createConnectionChannel,
  createReceiveChannel,
  sendBuffer,
  setupConnection,
} from './io/websocket';
import updateClock from './io/clock';

function* receive(message: ChannelTakeEffect<unknown>) {
  const response = new Response(yield message);
  const buffer = new Uint8Array(yield response.arrayBuffer());
  const results = deserializeMessage(buffer);
  if (results === undefined) {
    // console.warn('Unknown message type', messageType, messageBody);
    return;
  }
  yield put(updateState(results.messageType, results.pbMessage));
}

function* receiveAll(channel: EventChannel<unknown>) {
  while (true) {
    const message = yield take(channel);
    yield receive(message);
  }
}

function* sendState(sock: WebSocket, pbMessageType: PBMessageType) {
  const processor = getStateProcessor(pbMessageType);
  if (processor === undefined) {
    // TODO: handle this error
    return;
  }

  const pbMessage = yield select(processor.selector);
  const body = processor.serializer(pbMessage);
  yield sendBuffer(sock, body);
}

function* sendAll(sock: WebSocket) {
  let clock = 0;
  while (sock.readyState === WebSocket.OPEN) {
    // TODO: use the output schedule of state synchronization to set pbMessageType
    const pbMessageType = ParametersRequest;
    yield sendState(sock, pbMessageType);
    yield delay(90);
    clock += 1;
  }
  // console.log('Websocket is no longer open!');
}

function* serviceConnection() {
  const { sock, connectionChannel } = yield setupConnection();
  const receiveChannel = createReceiveChannel(sock);
  yield fork(receiveAll, receiveChannel);
  yield fork(sendAll, sock);
  const connection = yield take(connectionChannel);
  receiveChannel.close();
}

export function* serviceConnectionPersistently(): IterableIterator<unknown> {
  while (true) {
    yield serviceConnection();
    // console.log('Reestablishing WebSocket connection...');
  }
}

export function* controllerSaga(): IterableIterator<unknown> {
  yield all([
    yield takeEvery(INITIALIZED, serviceConnectionPersistently),
    yield takeLatest(INITIALIZED, updateClock),
  ]);
}

export default controllerSaga;
