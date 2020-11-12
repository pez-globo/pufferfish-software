import { PBMessageType } from '../types';
import { MessageSelectors } from '../selectors';
import { MessageSerializer, MessageSerializers } from './messages';

export interface StateProcessor {
  // This "any" is needed because MessageSelectors has a value type of "any".
  // Refer to the "Dynamic Dispatch" section of ./selectors to justify the use
  // of any in MessageSelectors.
  // eslint-disable @typescript-eslint/no-explicit-any
  selector: any;
  serializer: MessageSerializer;
}

export const getStateProcessor = (pbMessageType: PBMessageType): StateProcessor | undefined => {
  const selector = MessageSelectors.get(pbMessageType);
  if (selector === undefined) {
    // TODO: raise an exception instead?
    return undefined;
  }

  const serializer = MessageSerializers.get(pbMessageType);
  if (serializer === undefined) {
    // TODO: raise an exception instead?
    return undefined;
  }

  return { selector, serializer };
};
