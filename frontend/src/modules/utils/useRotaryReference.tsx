import { Theme } from '@material-ui/core';
import { RefObject } from 'react';
import { Subscription } from 'rxjs';
import { getActiveRotaryReference } from '../app/Service';

export const useRotaryReference = (
  theme: Theme,
): { initRefListener(elRefs: Record<string, RefObject<HTMLDivElement>>): void } => {
  const initRefListener = (elRefs: Record<string, RefObject<HTMLDivElement>>) => {
    const refSubscription: Subscription = getActiveRotaryReference().subscribe(
      (refString: string | null) => {
        Object.values(elRefs).forEach((elRef: RefObject<HTMLDivElement>) => {
          const ref = elRef.current;
          if (ref) ref.style.border = 'none';
        });
        if (refString) {
          if (elRefs[refString] && elRefs[refString].current) {
            const ref = elRefs[refString].current;
            if (ref) ref.style.border = `1px solid ${theme.palette.text.primary}`;
          }
        }
      },
    );
    return () => {
      if (refSubscription) {
        refSubscription.unsubscribe();
      }
    };
  };
  return { initRefListener };
};

export default useRotaryReference;
