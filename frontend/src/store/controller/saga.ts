import { BufferReader } from 'protobufjs/minimal';
import { eventChannel, EventChannel } from 'redux-saga';
import {
  all,
  apply,
  call,
  ChannelTakeEffect,
  delay,
  fork,
  put,
  select,
  take,
  takeEvery,
  takeLatest,
} from 'redux-saga/effects';
import { CLOCK_UPDATED, INITIALIZED } from '../app/types';
import { updateState } from './actions';
import { ParametersRequest } from './proto/mcu_pb';
import { getParametersRequest } from './selectors';
import { MessageClass, MessageTypes } from './types';

function createConnectionChannel() {
  return eventChannel((emit) => {
    const sock = new WebSocket('ws://localhost:8000/');
    sock.onerror = (err) => emit({ err, sock: null });
    sock.onopen = () => emit({ err: null, sock });
    sock.onclose = () => emit({ err: 'Closing!', sock });
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
  const messageType = buffer[0];
  const messageBody = buffer.slice(1);
  const messageBodyReader = new BufferReader(messageBody);
  const messageClass = MessageClass.get(messageType);
  if (messageClass !== undefined) {
    const pbMessage = messageClass.decode(messageBodyReader);
    yield put(updateState(messageType, pbMessage));
  } else {
    // console.warn('Unknown message type', messageType, messageBody);
  }
}

function* receiveAll(channel: EventChannel<unknown>) {
  while (true) {
    const message = yield take(channel);
    yield receive(message);
  }
}

function* sendParametersRequest(sock: WebSocket, parametersRequest: ParametersRequest) {
  const messageType = MessageTypes.get(ParametersRequest);
  if (messageType === undefined) {
    // console.warn('Error: unknown message type for', ParametersRequest);
    return;
  }
  const messageBody = ParametersRequest.encode(parametersRequest).finish();
  const buffer = new Uint8Array(1 + messageBody.length);
  buffer.set([messageType], 0);
  buffer.set(messageBody, 1);
  yield apply(sock, sock.send, [buffer]);
  yield delay(90);
}

function* sendAll(sock: WebSocket) {
  let clock = 0;
  while (sock.readyState === WebSocket.OPEN) {
    const parametersRequest = yield select(getParametersRequest);
    yield sendParametersRequest(sock, { ...parametersRequest, time: clock });
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
