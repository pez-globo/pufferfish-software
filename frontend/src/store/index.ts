import { combineReducers, createStore, applyMiddleware } from 'redux';
import createSagaMiddleware from 'redux-saga';
import { composeWithDevTools } from 'redux-devtools-extension';

import { appReducer } from './app/reducers';
import { connectionSaga } from './connection/saga';
import { connectionReducer } from './connection/reducers';
import { controllerReducer } from './controller/reducers';
import { plotsReducer } from './plots/reducers';

const reducers = combineReducers({
  app: appReducer,
  controller: controllerReducer,
  connection: connectionReducer,
  plots: plotsReducer,
});

const sagaMiddleware = createSagaMiddleware();
const store = createStore(reducers, composeWithDevTools(applyMiddleware(sagaMiddleware)));

sagaMiddleware.run(connectionSaga);

export default store;
