/**
 * @summary Service files has custom `events` & getter/setter of events
 *
 * @file It is based out of package RxJs [https://www.npmjs.com/package/rxjs]
 * Its similar to Pub-Sub pattern & used here to communicate between different components
 * For example, Opening Multistep popup from triggering event from Dashboard component.
 * Multistep component Listens to this event and acts accordingly
 */
import { BehaviorSubject, Subject, Observable } from 'rxjs';
import { distinctUntilChanged } from 'rxjs/operators';

//TODO: USE REDUX INSTEAD OF RXJS FOR MULTISTEPWIZARD, as we intend to remove it

const isMultiPopupOpen = new BehaviorSubject<boolean>(false);
const currentStateKey = new Subject<string>();

/**
 * Function to set multistep popup open.
 *
 * @param {boolean} state - Its open/close for multistep popup
 * @param {string} stateKey - Unique identifier of parameters (eg spo2, fio2...)
 *
 * @return {void}
 *
 */
export function setMultiPopupOpen(state: boolean, stateKey?: string): void {
  isMultiPopupOpen.next(state);
  currentStateKey.next(stateKey);
}

/**
 * Function to get popup state
 *
 * @return {Observable<boolean>}
 *
 */
export function getMultiPopupOpenState(): Observable<boolean> {
  return isMultiPopupOpen.asObservable().pipe(distinctUntilChanged());
}

/**
 * Function to get current state key (Unique identifier of parameters (eg spo2, fio2...))
 *
 * @return {Observable<boolean>}
 */
export function getcurrentStateKey(): Observable<string> {
  return currentStateKey.asObservable();
}
