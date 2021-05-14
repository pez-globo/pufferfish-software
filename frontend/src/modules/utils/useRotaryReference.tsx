/**
 * @summary Function to update Border Styles which is added to highlight the HTML element for Rotary encoder
 *
 * Function is called on initalization of component which uses Rotary encoder
 */
import { Theme } from '@material-ui/core';
import { RefObject } from 'react';
import { Subscription } from 'rxjs';
import { getActiveRotaryReference } from '../app/Service';

/**
 * function for handling rotary reference.
 *
 * @param {Theme} theme Material's Theme Object
 *
 * @returns {function} Function to Run on Component Initalization
 *
 */
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
