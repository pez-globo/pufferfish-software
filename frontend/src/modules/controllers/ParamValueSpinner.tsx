import { useTheme } from '@material-ui/core';
import React, { RefObject, useRef, useEffect } from 'react';
import { useDispatch, useSelector } from 'react-redux';
import { commitDraftRequest } from '../../store/controller/actions';
import { ParametersRequest } from '../../store/proto/mcu_pb';
import { MessageType } from '../../store/proto/types';
import ValueSpinner from './ValueSpinner';
import { SelectorType } from '../controllers/ValueSelectorDisplay';
import {
  PEEP_REFERENCE_KEY,
  RR_REFERENCE_KEY,
  FIO2_REFERENCE_KEY,
  TV_REFERENCE_KEY,
  FLOW_REFERENCE_KEY,
} from '../settings/tabs/constants';
import useRotaryReference from '../shared/useRotaryReference';

interface ValueProps {
  selector: SelectorType;
  reference: string;
  label: string;
  stateKey: string;
  units: string;
}

const ParamValueSpinner = ({
  label,
  reference,
  selector,
  stateKey,
  units,
}: ValueProps): JSX.Element => {
  const dispatch = useDispatch();
  const theme = useTheme();
  const value = useSelector(selector);
  const { initRefListener } = useRotaryReference(theme);

  /**
   * State to manage Wrapper HTML reference of parameter `ValueSpinner`
   * This wrapper's HTML border is added or removed based on user's interaction with Controls
   * It is used for UI identification of which control value is changing via rotary encoder
   */
  const [elRefs] = React.useState<Record<string, RefObject<HTMLDivElement>>>({
    [PEEP_REFERENCE_KEY]: useRef(null),
    [RR_REFERENCE_KEY]: useRef(null),
    [FIO2_REFERENCE_KEY]: useRef(null),
    [TV_REFERENCE_KEY]: useRef(null),
    [FLOW_REFERENCE_KEY]: useRef(null),
  });

  /**
   * Calls on initalization of the component
   * This is an event listener which listens to user input on `ValueSpinner` buttons click
   * Based on this event Border around Alarm's HTML wrapper is added/removed
   */
  useEffect(() => {
    initRefListener(elRefs);
  }, [initRefListener, elRefs]);

  const updateParam = (key: string, value: number) => {
    const update = { [key]: value } as Partial<ParametersRequest>;
    dispatch(commitDraftRequest<ParametersRequest>(MessageType.ParametersRequest, update));
  };

  return (
    <ValueSpinner
      reference={elRefs[reference]}
      referenceKey={reference}
      label={label}
      units={units}
      value={value}
      onClick={(value: number) => updateParam(stateKey, value)}
    />
  );
};

export default ParamValueSpinner;
