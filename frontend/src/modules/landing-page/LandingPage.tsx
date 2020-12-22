import { Grid } from '@material-ui/core';
import { makeStyles } from '@material-ui/core/styles';
import React from 'react';
import Carousel from '../displays/Carousel';

const useStyles = makeStyles(() => ({
  slideHead: {
    margin: '0px',
    textAlign: 'left',
    marginBottom: '10px',

    '& ul': {
      padding: '0px 20px',
      listStyleType: 'disc',

      '& li': {
        fontWeight: 'normal',
      },
    },
  },
  slideContent: {
    backgroundColor: '#042139',
    padding: '20px',
    borderRadius: '10px',
    minHeight: '235px',
  },
}));

export const LandingPage = (): JSX.Element => {
  const classes = useStyles();

  return (
    <div
      style={{
        width: '100%',
        position: 'relative',
      }}
    >
      <Carousel
        slides={[
          <div>
            <h1 className={classes.slideHead}>Invasive Modes</h1>
            <Grid item xs direction="row" container className={classes.slideContent}>
              <Grid item xs className={classes.slideHead}>
                <h4>Pressure Controlled</h4>
                <ul>
                  <li>PC-AC</li>
                  <li>PSV</li>
                  <li>PC-SIMV</li>
                  <li>Bi-level</li>
                </ul>
              </Grid>
              <Grid item xs className={classes.slideHead}>
                <h4>Volume Controlled</h4>
                <ul>
                  <li>VC-AC</li>
                  <li>VC-SIMV</li>
                </ul>
              </Grid>
              <Grid item xs className={classes.slideHead}>
                <h4>Miscellaneous</h4>
                <ul>
                  <li>PRVC</li>
                </ul>
              </Grid>
            </Grid>
          </div>,
          <div />,
          <div />,
        ]}
      />
    </div>
  );
};

export default LandingPage;
