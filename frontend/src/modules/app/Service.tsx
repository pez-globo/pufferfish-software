import { BehaviorSubject, Subject, Observable } from 'rxjs';
import { distinctUntilChanged } from 'rxjs/operators';

const isMultiPopupOpen = new BehaviorSubject<boolean>(false);
const currentStateKey = new Subject<string>();

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
