/**
 * @summary Function to update Border Styles which is added to highlight the HTML element for Rotary encoder
 *
 * Function is called on initalization of component which uses Rotary encoder
 */
import { Theme } from '@material-ui/core';
import { RefObject } from 'react';
import { useSelector } from 'react-redux';
import { getRotaryReference } from '../../../store/app/selectors';

/**
 * function for handling rotary reference.
 * Highlights the current active ValueClicker in the component
 *
 * @param {Theme} theme Material's Theme Object
 *
 * @returns {function} Function to Run on Component Initalization
 *
 */
export function useRotaryReference(
  theme: Theme,
): { initRefListener(elRefs: Record<string, RefObject<HTMLDivElement>>): void } {
  const refSubscription = useSelector(getRotaryReference);
  const initRefListener = (elRefs: Record<string, RefObject<HTMLDivElement>>) => {
    if (refSubscription) {
      if (elRefs[refSubscription] && elRefs[refSubscription].current) {
        const ref = elRefs[refSubscription].current;
        if (ref) ref.style.border = `1px solid ${theme.palette.text.primary}`;
      }
    }
  };
  return { initRefListener };
}
