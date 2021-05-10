/**
 * @summary A short one-line description for the file
 *
 * @file More detailed description for the file, if necessary;
 * perhaps spanning multiple lines.
 *
 */
import { Grid } from '@material-ui/core';
import { makeStyles } from '@material-ui/core/styles';
import React from 'react';
import Carousel from '../displays/Carousel';

const useStyles = makeStyles(() => ({
  slideHead: {
    margin: '0px',
    textAlign: 'left',
    marginBottom: '2.5rem',

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
    minHeight: '275px',
  },
  slideText: {
    textAlign: 'left',
    fontWeight: 'normal',
    fontSize: '1.5rem',
  },
}));

/**
 * LandingPage
 *
 * @component Landing page component with pufferfish ventilator information data.
 *
 * @returns {JSX.Element}
 * 
 */
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
          /* <div>
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
        </div>, */
          <div>
            <h1 className={classes.slideHead}>Emergency Use Notice</h1>
            <Grid item xs direction="row" container className={classes.slideContent}>
              <Grid item xs className={classes.slideText}>
                <p>
                  This Pufferfish ventilator is running a test version of software with known
                  problems, limitations, and missing safety measures. For this reason, any person
                  connected to this ventilator should always be supervised by someone trained in
                  what to do if a problem occurs.
                </p>
                <p>
                  This ventilator provides oxygen therapy and may be considered for use if no safer
                  alternatives are available. It should not be used outside of extreme crisis
                  situations.
                </p>
              </Grid>
            </Grid>
          </div>,
          <div>
            <h1 className={classes.slideHead}>Capabilities</h1>
            <Grid item xs direction="row" container className={classes.slideContent}>
              <Grid item xs className={classes.slideText}>
                <p>
                  This version of software provides a visual alarm when the oxygen supply runs out,
                  and it allows adjusting FiO2 and flow rate. Because alarms do not generate sounds,
                  the ventilator must always be watched in case of alarms.
                </p>
                <p>
                  This ventilator has a small internal battery in case of power outage. Because the
                  current version of software does not have any way to read the charge level of the
                  battery, it will shut off without warning if it runs out of power. Thus, the
                  ventilator should never be disconnected from its power source.
                </p>
              </Grid>
            </Grid>
          </div>,
        ]}
      />
    </div>
  );
};

export default LandingPage;
