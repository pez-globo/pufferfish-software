import { CallEffect, delay, put, PutEffect, select, SelectEffect } from 'redux-saga/effects';
import { establishedBackendConnection, lostBackendConnection } from '../../connection/actions';
import { getBackendConnected } from '../../connection/selectors';
import { ConnectionAction } from '../../connection/types';

export function* watchBackendHeartbeat(): Generator<
  SelectEffect | CallEffect<true> | PutEffect<ConnectionAction>,
  void,
  unknown
> {
  while (true) {
    yield delay(3000);
    const backendConnected = yield select(getBackendConnected);
    if (backendConnected) {
      yield put(lostBackendConnection());
    }
  }
}

export function* watchBackendConnection(): Generator<
  SelectEffect | PutEffect<ConnectionAction>,
  void,
  unknown
> {
  while (true) {
    const backendConnected = yield select(getBackendConnected);
    if (!backendConnected) {
      yield put(establishedBackendConnection());
    }
  }
}
