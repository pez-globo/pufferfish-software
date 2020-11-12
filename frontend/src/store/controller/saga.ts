import { eventChannel, Subscribe, END, EventChannel } from 'redux-saga';
import {
  put,
  call,
  take,
  takeEvery,
  fork,
  delay,
  select,
  apply,
  takeLatest,
  all,
  ChannelTakeEffect,
} from 'redux-saga/effects';
import { BufferReader } from 'protobufjs/minimal';
import { Socket } from 'dgram';
import { MessageClass, MessageTypes, PBMessage, PBMessageType } from './types';
import { ParametersRequest } from './proto/mcu_pb';
import { INITIALIZED, CLOCK_UPDATED } from '../app/types';
import { updateState } from './actions';
import { deserializeMessage, MessageSerializer, MessageSerializers } from './protocols/messages';
import { getStateProcessor } from './protocols/backend';

function createConnectionChannel() {
  return eventChannel((emit) => {
    const sock = new WebSocket('ws://localhost:8000/');
    sock.onerror = (err) => emit({ err, sock: null });
    sock.onopen = (event) => emit({ err: null, sock });
    sock.onclose = (event) => emit({ err: 'Closing!', sock });
    return () => {
      // console.log('Closing WebSocket...');
      sock.close();
    };
  });
}

function createReceiveChannel(sock: WebSocket) {
  const sockCopy = sock;
  return eventChannel((emit) => {
    sockCopy.onmessage = (message) => emit(message.data);
    return () => {
      sockCopy.close();
    };
  });
}

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

function* sendBuffer(sock: WebSocket, buffer: Uint8Array | undefined) {
  if (buffer === undefined) {
    return;
  }

  yield apply(sock, sock.send, [buffer]);
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

function* initConnection() {
  let connectionChannel;
  let connection;
  while (true) {
    connectionChannel = yield call(createConnectionChannel);
    connection = yield take(connectionChannel);
    if (connection.err == null) {
      break;
    }
    // console.warn('WebSocket connection error', connection.err)
    yield delay(1);
  }
  // console.log('Connected to WebSocket!');
  const receiveChannel = createReceiveChannel(connection.sock);
  yield fork(receiveAll, receiveChannel);
  yield fork(sendAll, connection.sock);
  connection = yield take(connectionChannel);
  receiveChannel.close();
}

export function* initConnectionPersistently(): IterableIterator<unknown> {
  while (true) {
    yield initConnection();
    // console.log('Reestablishing WebSocket connection...');
  }
}

export function* updateClock(): IterableIterator<unknown> {
  while (true) {
    yield delay(1000);
    yield put({ type: CLOCK_UPDATED });
  }
}

export function* controllerSaga(): IterableIterator<unknown> {
  yield all([
    yield takeEvery(INITIALIZED, initConnectionPersistently),
    yield takeLatest(INITIALIZED, updateClock),
  ]);
}

export default controllerSaga;
