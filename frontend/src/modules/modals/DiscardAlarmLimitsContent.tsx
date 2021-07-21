/**
 * @summary Modal Popup Content
 *
 * @file Reusable component to populate modal popup content
 */
import { Grid, makeStyles, Theme, Typography } from '@material-ui/core';
import React from 'react';
import { useSelector } from 'react-redux';
import { alarmConfiguration, AlarmConfiguration } from '../alarms/AlarmsPage';
import { VentilationMode, Range } from '../../store/proto/mcu_pb';
import {
  getAlarmLimitsRequestDraft,
  getAlarmLimitsRequest,
  getAlarmLimitsUnsavedKeys,
} from '../../store/controller/selectors';

const useStyles = makeStyles((theme: Theme) => ({
  marginContent: {
    textAlign: 'center',
    marginTop: theme.spacing(2),
    marginBottom: theme.spacing(3),
  },
  marginHeader: {
    textAlign: 'center',
    marginTop: theme.spacing(3),
    marginBottom: theme.spacing(1),
  },
}));

export const DiscardAlarmLimitsContent = (): JSX.Element => {
  const classes = useStyles();
  const alarmLimitsRequestDraftSelect = useSelector(getAlarmLimitsRequestDraft);
  const alarmLimitsRequestSelect = useSelector(getAlarmLimitsRequest);
  const alarmLimitsUnsavedKeys = useSelector(getAlarmLimitsUnsavedKeys);
  const alarmLimitsRequest = (alarmLimitsRequestSelect as unknown) as Record<string, Range>;
  const alarmLimitsRequestDraft = (alarmLimitsRequestDraftSelect as unknown) as Record<
    string,
    Range
  >;
  const alarmConfig = alarmConfiguration(VentilationMode.hfnc);

  return (
    <Grid container alignItems="center">
      <Grid container alignItems="center" justify="center">
        <Grid container alignItems="center" className={classes.marginHeader}>
          <Grid item xs>
            <Typography variant="h4">Keep Previous Values?</Typography>
          </Grid>
        </Grid>
        <Grid item className={classes.marginContent}>
          {alarmConfig.map((param: AlarmConfiguration) => {
            if (alarmLimitsRequest !== null && alarmLimitsRequestDraft !== null) {
              if (alarmLimitsUnsavedKeys.includes(param.stateKey)) {
                return (
                  <Typography variant="subtitle1">{`Keep ${param.label} alarm limits at [${
                    alarmLimitsRequest[param.stateKey].lower
                  } ${param.units} - ${alarmLimitsRequest[param.stateKey].upper} ${
                    param.units
                  }] instead of changing to [${alarmLimitsRequestDraft[param.stateKey].lower} ${
                    param.units
                  } -
                         ${alarmLimitsRequestDraft[param.stateKey].upper} ${
                    param.units
                  }]?`}</Typography>
                );
              }
            }
            return <React.Fragment />;
          })}
        </Grid>
      </Grid>
    </Grid>
  );
};

export default DiscardAlarmLimitsContent;
