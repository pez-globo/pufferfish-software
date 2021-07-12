import { loadFeature, defineFeature } from 'jest-cucumber';
import { takeEvery } from 'redux-saga/effects';
import controllerSaga, { serviceConnectionPersistently } from '../../src/store/controller/saga';
import { INITIALIZED } from '../../src/store/app/types';

const feature = loadFeature('tests/features/saga.feature');

defineFeature(feature, (test) => {
  test('To check if redux saga workers is running as expected', ({ given, when, then }) => {
    let genObject: IterableIterator<unknown>;

    given(/^Saga worker is initialized$/, async () => {
      genObject = controllerSaga();
    });

    when(/^Saga workers are defined$/, async () => {
      expect(controllerSaga).toBeDefined();
      expect(serviceConnectionPersistently).toBeDefined();
    });

    then(/^Saga should run workers for socket connection & clock$/, () => {
      expect(genObject.next().value).toEqual(takeEvery(INITIALIZED, serviceConnectionPersistently));
    });
  });
});
