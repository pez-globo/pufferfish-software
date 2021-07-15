import { loadFeature, defineFeature } from 'jest-cucumber';
import configureStore, { MockStore } from 'redux-mock-store';
import { commitRequest } from '../../src/store/controller/actions';
import { AlarmLimitsRequest } from '../../src/store/controller/proto/mcu_pb';
import { MessageType, CommitAction } from '../../src/store/controller/types';

const feature = loadFeature('tests/features/alarm-modal.feature');

defineFeature(feature, (test) => {
  const mockStore = configureStore();
  let store: MockStore;
  const clickInterval = 100;

  beforeEach(() => {
    store = mockStore({
      controller: { alarmLimits: { request: { rr: { lower: 0, upper: clickInterval } } } },
    });
  });

  test('To check if alarm values are stored in redux', ({ given, when, then }) => {
    let actionCommitted: CommitAction;

    given(/^I set alarm limit values$/, async () => {
      actionCommitted = commitRequest<AlarmLimitsRequest>(MessageType.AlarmLimitsRequest, {
        rr: { lower: 10, upper: 20 },
      });
    });

    when(/^I commit changes to redux action$/, async () => {
      store.dispatch(actionCommitted);
    });

    then(/^I should get action values verified with triggered one's$/, () => {
      const actions = store.getActions();
      const expectedPayload = {
        type: '@controller/REQUEST_COMMITTED',
        messageType: MessageType.AlarmLimitsRequest,
        update: { rr: { lower: 10, upper: 20 } },
      };
      expect(actions).toEqual([expectedPayload]);
    });
  });
});
