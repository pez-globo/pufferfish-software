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

function createEventChannel() {
  console.log('Connecting to WebSocket...')
  const sock = new WebSocket('ws://localhost:8000/')
  return [
    eventChannel((emit) => {
      sock.onmessage = (message) => emit(message.data)
      return () => {
        sock.close()
      }
    }),
    eventChannel((emit) => {
      sock.onopen = (event) => emit(sock)
      return () => {
        sock.close()
      }
    })
  ]
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
  const [receiveChannel, sendChannel] = yield call(createEventChannel)
  console.log('Connected to WebSocket!')
  yield fork(receiveAll, receiveChannel)
  const sock = yield take(sendChannel)
  yield fork(sendAll, sock)
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
