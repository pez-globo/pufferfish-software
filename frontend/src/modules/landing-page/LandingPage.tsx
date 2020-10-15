import React, { useCallback, useEffect } from 'react';
import { makeStyles, Theme } from '@material-ui/core/styles';
import {
  Button,
  Grid,
  Typography,
} from '@material-ui/core';
import { Link } from 'react-router-dom';
import TestTool from '../controllers/TestTool';
import ValueClicker from '../controllers/ValueController';
import ModeBanner from '../displays/ModeBanner';
import ToggleValue from '../displays/ToggleValue';
import { QUICKSTART_ROUTE } from '../navigation/constants';

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
      <Button
        style={
          {
            position: 'absolute',
            bottom: '30px',
            right: 0,
            marginRight: '20px'
        }
      }
              component={Link}
              to={QUICKSTART_ROUTE.path}
              variant="contained"
              color="primary"
      >
        Ventilation
      </Button>
    </div >
  );
};

export default LandingPage;
