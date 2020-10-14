import React, { useCallback, useEffect } from 'react';
import { makeStyles, Theme } from '@material-ui/core/styles';
import {
  Grid,
  Typography,
} from '@material-ui/core';
import TestTool from '../controllers/TestTool';
import ValueClicker from '../controllers/ValueController';
import ModeBanner from '../displays/ModeBanner';
import ToggleValue from '../displays/ToggleValue';

export const LandingPage = (): JSX.Element => {

  return (
    <div style={
      {
        width: '100%',
        position: 'relative'
      }
    }>
      <img style={{
        width: '100%',
        height: '100%',
        position: 'absolute',
        top: 0,
        bottom: 0,
        left: 0
      }} src={process.env.PUBLIC_URL + '/home.png'} />
    </div>
  );
};

export default LandingPage;
