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

const isMultiPopupOpen = new BehaviorSubject<boolean>(false);
const isModalPopupOpen = new BehaviorSubject<boolean>(false);
const currentStateKey = new Subject<string>();
const isScreenLockPopupOpen = new Subject<boolean>();
const activeRotaryReference = new Subject<string | null>();

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

export function setModalPopupOpen(state: boolean): void {
  isModalPopupOpen.next(state);
}

export function getModalPopupOpenState(): Observable<boolean> {
  return isModalPopupOpen.asObservable().pipe(distinctUntilChanged());
}

/**
 * Function to get current state key (Unique identifier of parameters (eg spo2, fio2...))
 *
 * @return {Observable<boolean>}
 */
export function getcurrentStateKey(): Observable<string> {
  return currentStateKey.asObservable();
}

/**
 * Function to set screen lock popup
 *
 * @param {boolean} state - desc for state
 *
 * @return {void}
 */
export function setScreenLockPopup(state: boolean): void {
  isScreenLockPopupOpen.next(state);
}

/**
 * Function to get screen lock popup
 *
 * @return {Observable<boolean>}
 *
 */
export function getScreenLockPopup(): Observable<boolean> {
  return isScreenLockPopupOpen.asObservable();
}

/**
 * Function to set the active rotary reference
 *
 * @param {string | null} refString - Wrapper HTML reference identifier
 *
 * @return {void}
 *
 */
export function setActiveRotaryReference(refString: string | null): void {
  activeRotaryReference.next(refString);
}

/**
 * Function to get the active rotary reference
 *
 * @return {Observable<string | null>}
 *
 */
export function getActiveRotaryReference(): Observable<string | null> {
  return activeRotaryReference.asObservable();
}
