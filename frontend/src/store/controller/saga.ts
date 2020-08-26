import {eventChannel} from 'redux-saga'
import {
  put, call, take, takeEvery, fork, delay, select, apply, takeLatest, all
} from 'redux-saga/effects'
import {BufferReader} from 'protobufjs/minimal'
import {
  MessageClass,
  MessageTypes,
} from './types'
import {ParametersRequest} from './proto/mcu_pb'
import {INITIALIZED, CLOCK_UPDATED} from '../app/types'
import {updateState} from './actions'
import {getParametersRequest} from './selectors'

function createConnectionChannel() {
  return eventChannel((emit) => {
    console.log('Connecting to WebSocket...')
    const sock = new WebSocket('ws://localhost:8000/')
    sock.onerror = (err) => emit({'err': err, 'sock': null})
    sock.onopen = (event) => emit({'err': null, 'sock': sock})
    return () => {
      console.log('Closing WebSocket...')
      sock.close()
    }
  })
}

function createReceiveChannel(sock: WebSocket) {
  return eventChannel((emit) => {
    sock.onmessage = (message) => emit(message.data)
    return () => {
      sock.close()
    }
  })
}

function *receive(message: any) {
  const response = new Response(yield message)
  const buffer = new Uint8Array(yield response.arrayBuffer())
  const messageType = buffer[0]
  const messageBody = buffer.slice(1)
  const messageBodyReader = new BufferReader(messageBody)
  const messageClass = MessageClass.get(messageType)
  if (messageClass !== undefined)  {
    const pbMessage = messageClass.decode(messageBodyReader)
    yield put(updateState(messageType, pbMessage))
  } else {
    console.warn('Unknown message type', messageType, messageBody)
  }
}

function *receiveAll(channel: any) {
  while (true) {
    const message = yield take(channel)
    yield receive(message)
  }
}

function *sendParametersRequest(
  sock: WebSocket, parametersRequest: ParametersRequest
) {
  const messageType = MessageTypes.get(ParametersRequest)
  if (messageType === undefined) {
    console.warn('Error: unknown message type for', ParametersRequest)
    return
  }
  const messageBody = ParametersRequest.encode(parametersRequest).finish()
  const buffer = new Uint8Array(1 + messageBody.length)
  buffer.set([messageType], 0)
  buffer.set(messageBody, 1)
  yield apply(sock, sock.send, [buffer])
  yield delay(90)
}

function *sendAll(sock: WebSocket) {
  var clock = 0
  while (sock.readyState === WebSocket.OPEN) {
    const parametersRequest = yield select(getParametersRequest)
    yield sendParametersRequest(
      sock, Object.assign({}, parametersRequest, {time: clock})
    )
    clock += 1
  }
}

function* initConnection() {
  var connection
  while (true) {
    const connectionChannel = yield call(createConnectionChannel)
    connection = yield take(connectionChannel)
    if (connection.err == null) {
      break
    }
    console.warn('WebSocket connection error', connection.err)
    yield delay(1)
  }
  console.log('Connected to WebSocket!')
  const receiveChannel = createReceiveChannel(connection.sock)
  yield all([
    yield fork(receiveAll, receiveChannel),
    yield fork(sendAll, connection.sock)
  ])
}

function* updateClock() {
  while(true) {
    yield delay(1000)
    yield put({type: CLOCK_UPDATED})
  }
}

export function* controllerSaga() {
  yield all([
    yield takeEvery(INITIALIZED, initConnection),
    yield takeLatest(INITIALIZED, updateClock)
  ])
}
