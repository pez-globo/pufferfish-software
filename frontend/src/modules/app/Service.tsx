import { BehaviorSubject, Observable } from 'rxjs';
import { distinctUntilChanged } from 'rxjs/operators';

const OnLogEvent = new BehaviorSubject<boolean>(false);

export function setActiveEventState(state: boolean): void {
  OnLogEvent.next(state);
}

export function getActiveEventState(): Observable<boolean> {
  return OnLogEvent.asObservable().pipe(distinctUntilChanged());
}
