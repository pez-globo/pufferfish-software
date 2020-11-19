import { Grid, makeStyles, Theme, Typography } from '@material-ui/core';
import React, { useState } from 'react';
import { useDispatch } from 'react-redux';
import { updateCommittedParameter, updateCommittedState } from '../../../store/controller/actions';
import { PARAMETER_STANDBY } from '../../../store/controller/types';
import { ValueModal } from '../../controllers';
import { ClickHandler } from './ValueInfo';

interface Props {
  value: number | undefined;
  committedSetting: number;
  label: string;
  stateKey: string;
  units?: string;
  min?: number;
  max?: number;
}

interface ValueProps {
  value: number | undefined;
  label: string;
  units: string;
}

const useStyles = makeStyles((theme: Theme) => ({
  rootParent: {
    flexDirection: 'initial',
    height: '100%',
  },
  root: {
    padding: '7px',
    paddingLeft: theme.spacing(2),
    paddingRight: theme.spacing(2),
    flexDirection: 'initial',
    height: '100%',
  },
  controlPanel: {
    justifyContent: 'space-between',
    alignItems: 'center',
  },
  marginRight: {
    marginRight: theme.spacing(1),
  },
  displayContainer: {
    padding: 0,
    justifyContent: 'center',
  },
  liveContainer: {
    justifyContent: 'center',
    alignItems: 'center',
    height: '100%',
    // border: '1px solid blue'
  },
  liveBox: {
    textAlign: 'center',
    width: '100%',
    fontSize: 14,
    borderRadius: 5,
    border: `2px solid ${theme.palette.primary.main}`,
  },
  valueContainer: {
    justifyContent: 'flex-start',
    alignItems: 'center',
    flexGrow: 1,
    // border: '1px solid yellow',
  },
  valueLabel: {
    fontSize: '3rem',
    lineHeight: '1',
    // border: '1px solid red',
  },
  unitsLabel: {
    opacity: 0.8,
    // border: '1px solid red'
  },
}));

export const ValueControl = ({ value, label, units }: ValueProps): JSX.Element => {
  const classes = useStyles();
  return (
    <Grid container direction="column" className={classes.rootParent}>
      <Grid item xs style={{ width: '100%' }}>
        <Grid container direction="column" className={classes.root}>
          <Grid
            container
            item
            direction="row"
            justify="space-between"
            alignItems="center"
            wrap="nowrap"
          >
            <Grid item xs style={{ width: '100%', textAlign: 'center' }}>
              <Typography variant="h6" style={{ fontSize: '1rem', fontWeight: 500 }}>
                {label}
              </Typography>
            </Grid>
          </Grid>
          <Grid
            item
            xs
            justify="flex-start"
            alignItems="center"
            className={classes.displayContainer}
            wrap="nowrap"
          >
            <Typography align="center" variant="h2" className={classes.valueLabel}>
              {value !== undefined && !Number.isNaN(value)
                ? value.toFixed(0).replace(/^-0$/, '0')
                : '--'}
            </Typography>
            {value !== undefined && units !== '' && (
              <Typography align="center" variant="body1" className={classes.unitsLabel}>
                {units}
              </Typography>
            )}
          </Grid>
        </Grid>
      </Grid>
    </Grid>
  );
};

/**
 * Control Info
 *
 * Component for showing information.
 *
 */
const ControlInfo = (props: Props): JSX.Element => {
  const { value, label, units, stateKey, committedSetting, min, max } = props;
  const [open, setOpen] = useState(false);
  const dispatch = useDispatch();
  const doSetValue = (setting: number) => {
    dispatch(updateCommittedParameter({ [stateKey]: setting }));
    dispatch(updateCommittedState(PARAMETER_STANDBY, { [stateKey]: setting }));
  };
  const onClick = () => {
    setOpen(true);
  };
  const handleClick = ClickHandler(onClick, () => {
    return false;
  });
  const updateModalStatus = (status: boolean) => {
    setOpen(status);
  };

  return (
    <div
      style={{ outline: 'none' }}
      role="button"
      onKeyDown={() => null}
      onClick={handleClick}
      tabIndex={0}
    >
      <ValueControl value={value} label={label} units={units || ''} />
      <ValueModal
        updateModalStatus={updateModalStatus}
        openModal={open}
        disableSetNewButton={true}
        committedSetting={committedSetting}
        label={label}
        units={units}
        requestCommitSetting={doSetValue}
        {...(min && { min })}
        {...(max && { max })}
      />
    </div>
  );
};

export default ControlInfo;
