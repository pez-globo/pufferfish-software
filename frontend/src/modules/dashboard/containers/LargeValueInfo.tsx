/**
 * @summary A short one-line description for the file
 *
 * @file More detailed description for the file, if necessary;
 * perhaps spanning multiple lines.
 *
 */
import { Grid, makeStyles, Typography } from '@material-ui/core';
import React, { useEffect, useState } from 'react';
import { shallowEqual, useSelector } from 'react-redux';
import { getAlarmLimits } from '../../../store/controller/selectors';
import { setMultiPopupOpen } from '../../app/Service';
import { AlarmModal } from '../../controllers';
import { SelectorType, ValueSelectorDisplay } from '../../displays/ValueSelectorDisplay';
import { Range } from '../../../store/controller/proto/mcu_pb';

const useStyles = makeStyles(() => ({
  rootParent: {
    flexDirection: 'initial',
    height: '100%',
  },
  root: {
    width: '100%',
    // border: '1px solid red'
    backgroundColor: '#010010',
    borderRadius: '8px',
    padding: '2rem',
    height: '100%',
  },
  displayContainer: {
    padding: 0,
    // border: '1px solid orange',
    display: 'flex',
    alignItems: 'center',
    justifyContent: 'center',
    width: '100%',
  },
  liveContainer: {
    justifyContent: 'flex-end',
    alignItems: 'center',
    height: '100%',
    display: 'block',
    textAlign: 'right',
    // border: '1px solid blue'
  },
  valueContainer: {
    justifyContent: 'flex-start',
    alignItems: 'center',
    flexGrow: 1,
    // border: '1px solid yellow',
  },
  valueLabel: {
    fontSize: '5rem',
    lineHeight: '1',
    // border: '1px solid red',
  },
  unitsLabel: {
    fontSize: '1.5rem',
    opacity: 0.8,
    // border: '1px solid red'
  },
  valuesPanel: {
    padding: '15px',
    display: 'grid',
    gridTemplateColumns: '135px 1fr',
    gridGap: '10px',
    gridTemplateAreas: `'gridAreavalues1 gridAreavalues2'
    'gridAreavalues1 gridAreavalues3'`,
  },
  mainContainer: {
    gridTemplateAreas: `'gridAreavalues1 gridAreavalues1'
    'gridAreavalues1 gridAreavalues1'`,
  },
  gridAreavalues1: {
    gridArea: 'gridAreavalues1',
  },
  gridAreavalues2: {
    gridArea: 'gridAreavalues2',
  },
  gridAreavalues3: {
    gridArea: 'gridAreavalues3',
  },
  gridLiveContainer: {
    // display: 'flex',
    justifyContent: 'flex-end',
    height: '100%',
    // border: '1px solid blue'
    textAlign: 'right',
  },
  whiteFont: {
    color: '#fff',
  },
}));

/**
 * @typedef ValueInfoProps
 *
 * some description
 * 
 * @prop {Props} mainContainer desc for mainContainer
 * 
 */
export interface ValueInfoProps {
  mainContainer: Props;
}

/**
 * @typedef Props
 *
 * Props interface for the control value information.
 * 
 * @prop {SelectorType} selector desc for selector
 * @prop {string} label desc for label
 * @prop {string} stateKey desc for stateKey
 * @prop {string} units desc for units
 * @prop {boolean} isLive desc for isLive
 * @prop {boolean} showLimits desc for showLimits
 * @prop {number} decimal desc for decimal
 * 
 */
export interface Props {
  selector: SelectorType;
  label: string;
  stateKey: string;
  units?: string;
  isLive?: boolean;
  showLimits?: boolean;
  decimal?: number;
}

/**
 * ClickHandler
 *
 * @component Component for handling click event listener.
 *
 * @prop {function} singleClickAction handling single click
 * @prop {function} doubleClickAction handling double click
 * 
 * @returns {function}
 * 
 */
export const ClickHandler = (
  singleClickAction: () => void,
  doubleClickAction: () => void,
): (() => void) => {
  const [click, setClick] = useState(0);

  useEffect(() => {
    const timer = setTimeout(() => {
      if (click === 1) singleClickAction();
      setClick(0);
    }, 100);
    if (click === 2) doubleClickAction();

    return () => {
      clearTimeout(timer);
    };
  }, [click, singleClickAction, doubleClickAction]);

  return () => {
    setClick((prev) => prev + 1);
  };
};

/**
 * ControlValuesDisplay
 *
 * @component Component for handling value display.
 *
 * Uses the [[Props]] interface
 * 
 * @return {JSX.Element}
 */
const ControlValuesDisplay = ({
  selector,
  label,
  stateKey,
  units = '',
  showLimits = false,
  decimal,
}: Props): JSX.Element => {
  const classes = useStyles();
  const [open, setOpen] = useState(false);
  const alarmLimits = useSelector(getAlarmLimits, shallowEqual) as Record<string, Range>;

  /**
   * Function for handling click event listener
   */
  const onClick = () => {
    // setOpen(true);
    if (stateKey) {
      setMultiPopupOpen(true, stateKey);
    }
  };

  /**
   * some description
   */
  const handleClick = ClickHandler(onClick, () => {
    return false;
  });

  /**
   * Function for updating modal status.
   * 
   * @param {boolean} status desc for status
   * 
   */
  const updateModalStatus = (status: boolean) => {
    setOpen(status);
  };
  return (
    <div
      style={{ outline: 'none', height: '100%' }}
      role="button"
      onKeyDown={() => null}
      onClick={handleClick}
      tabIndex={0}
    >
      <Grid container direction="column" className={classes.rootParent}>
        <Grid item xs style={{ width: '100%', height: '100%' }}>
          <Grid
            container
            direction="column"
            className={classes.root}
            style={{ width: '80%', margin: '0 auto' }}
          >
            <Grid
              container
              item
              direction="row"
              justify="space-between"
              alignItems="center"
              wrap="nowrap"
            >
              <Grid item xs style={{ width: '100%' }}>
                <Typography className={classes.whiteFont} style={{ fontSize: '2rem' }}>
                  {label}
                </Typography>
              </Grid>
              {showLimits && stateKey && (
                <Grid container item xs={3} className={classes.liveContainer}>
                  <Typography className={classes.whiteFont} style={{ fontSize: '1.25rem' }}>
                    {alarmLimits[stateKey].lower}
                  </Typography>
                  <Typography className={classes.whiteFont} style={{ fontSize: '1.25rem' }}>
                    {alarmLimits[stateKey].upper}
                  </Typography>
                </Grid>
              )}
            </Grid>
            <Grid item xs alignItems="center" className={classes.displayContainer}>
              <Grid>
                <Typography
                  align="center"
                  variant="h2"
                  className={`${classes.valueLabel} ${classes.whiteFont}`}
                >
                  <ValueSelectorDisplay decimal={decimal} selector={selector} />
                </Typography>
                {units !== '' && (
                  <Typography
                    align="center"
                    variant="body1"
                    className={`${classes.unitsLabel} ${classes.whiteFont}`}
                  >
                    {units}
                  </Typography>
                )}
              </Grid>
            </Grid>
          </Grid>
        </Grid>
        {stateKey && (
          <AlarmModal
            updateModalStatus={updateModalStatus}
            openModal={open}
            disableAlarmButton={true}
            label={label}
            units={units}
            stateKey={stateKey}
            requestCommitRange={() => null}
          />
        )}
      </Grid>
    </div>
  );
};

/**
 * ValueInfo
 *
 * @component Component for showing information.
 *
 * Uses the [[Props]] interface
 * 
 * @returns {JSX.Element}
 * 
 */
const ValueInfo = ({
  selector,
  label,
  stateKey,
  units = '',
  showLimits = false,
  decimal,
}: Props): JSX.Element => {
  const classes = useStyles();
  return (
    <Grid item xs container className={`${classes.valuesPanel} ${classes.mainContainer}`}>
      <Grid item xs className={classes.gridAreavalues1}>
        <ControlValuesDisplay
          stateKey={stateKey}
          selector={selector}
          label={label}
          units={units}
          showLimits={showLimits}
          decimal={decimal || 0}
        />
      </Grid>
    </Grid>
  );
};

export default ValueInfo;
