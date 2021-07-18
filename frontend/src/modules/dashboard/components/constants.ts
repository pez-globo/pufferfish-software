import { OutputSelector } from 'reselect';

// The Selector type is generic and its complicated to define its type.
// eslint-disable @typescript-eslint/no-explicit-any
// eslint-disable-next-line
export type SelectorType = OutputSelector<any, any, any>;
