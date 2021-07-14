import { delay, put, select } from 'redux-saga/effects';
import { establishedBackendConnection, lostBackendConnection } from '../../connection/actions';
import { getBackendConnected } from '../../connection/selectors';

export function* watchBackendHeartbeat() {
  while (true) {
    yield delay(3000);
    const backendConnected = yield select(getBackendConnected);
    if (backendConnected) {
      yield put(lostBackendConnection());
    }
  }
}

export function* watchBackendConnection() {
  while (true) {
    const backendConnected = yield select(getBackendConnected);
    if (!backendConnected) {
      yield put(establishedBackendConnection());
    }
  }
}
