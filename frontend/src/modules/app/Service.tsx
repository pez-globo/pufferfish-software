import { BehaviorSubject, Subject, Observable } from 'rxjs';
import { distinctUntilChanged } from 'rxjs/operators';

const isMultiPopupOpen = new BehaviorSubject<boolean>(false);
const currentStateKey = new Subject<string>();
const isScreenLockPopupOpen = new Subject<boolean>();
const activeRotaryReference = new Subject<string | null>();

export function setMultiPopupOpen(state: boolean, stateKey?: string): void {
  isMultiPopupOpen.next(state);
  currentStateKey.next(stateKey);
}

export function getMultiPopupOpenState(): Observable<boolean> {
  return isMultiPopupOpen.asObservable().pipe(distinctUntilChanged());
}

export function getcurrentStateKey(): Observable<string> {
  return currentStateKey.asObservable();
}

export function setScreenLockPopup(state: boolean): void {
  isScreenLockPopupOpen.next(state);
}

export function getScreenLockPopup(): Observable<boolean> {
  return isScreenLockPopupOpen.asObservable();
}

export function setActiveRotaryReference(refString: string | null): void {
  activeRotaryReference.next(refString);
}

export function getActiveRotaryReference(): Observable<string | null> {
  return activeRotaryReference.asObservable();
}
