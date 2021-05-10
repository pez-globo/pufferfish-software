/**
 * @summary A short one-line description for the file
 *
 * @file More detailed description for the file, if necessary;
 * perhaps spanning multiple lines.
 *
 */
import { BehaviorSubject, Subject, Observable } from 'rxjs';
import { distinctUntilChanged } from 'rxjs/operators';

const isMultiPopupOpen = new BehaviorSubject<boolean>(false);
const currentStateKey = new Subject<string>();
const isScreenLockPopupOpen = new Subject<boolean>();
const activeRotaryReference = new Subject<string | null>();

/**
 * Function to set multiple popup open.
 *
 * @param {boolean} state - desc for state
 * @param {string} stateKey - desc for stateKey
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
 * Function to get current state key
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
 * @param {string | null} refString - desc for refString  
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
