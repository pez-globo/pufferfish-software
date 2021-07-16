/**
 * @deprecated
 * @summary Re-usable UI wrapper for displaying Value Info
 *
 * @file ValueInfo is a configurable component on Layout level
 * Each Value Info must have 1 main Container & optional 2 subContainer
 * This matches with Dashboard  Display Value Layout design
 *
 */
import { Grid, makeStyles, Theme, Typography } from '@material-ui/core';
import React, { useEffect, useState } from 'react';
import { shallowEqual, useSelector } from 'react-redux';
import { getAlarmLimitsRequest } from '../../../store/controller/selectors';
import { setMultiPopupOpen } from '../../app/Service';
import { AlarmModal } from '../../controllers';
import { SelectorType, ValueSelectorDisplay } from '../../displays/ValueSelectorDisplay';
import { Range } from '../../../store/controller/proto/mcu_pb';

const useStyles = makeStyles((theme: Theme) => ({
  rootParent: {
    flexDirection: 'initial',
    height: '100%',
  },
  root: {
    width: '100%',
    // border: '1px solid red'
    backgroundColor: '#010010',
    borderRadius: '8px',
    padding: '10px',
    height: '100%',
  },
  rootLarge: {
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
    lineHeight: '1',
    // border: '1px solid red',
  },
  unitsLabel: {
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
  mainWithSubcontainer: {
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

  gridRoot: {
    width: '100%',
    // border: '1px solid red'
    backgroundColor: '#010010',
    borderRadius: '8px',
    padding: '5px 10px',
    height: '100%',
  },
  gridLiveContainer: {
    // display: 'flex',
    justifyContent: 'flex-end',
    height: '100%',
    // border: '1px solid blue'
    textAlign: 'right',
  },

  gridValueLabel: {
    fontSize: '2rem',
    lineHeight: '1',
    // border: '1px solid red',
  },
  gridUnitsLabel: {
    // paddingLeft: theme.spacing(1),
    paddingTop: theme.spacing(3),
    opacity: 0.8,
    fontSize: '12px',
    // border: '1px solid red'
  },
  whiteFont: {
    color: '#fff',
  },
}));

/**
 * @deprecated
 * @typedef ValueInfoProps
 *
 * Props interface for the showing value information.
 *
 * @prop {Props} mainContainer Main Container placed on Left side in layout with more space
 * @prop {Props} subContainer1 Sub Container placed on Right side, with half the Main container height
 * @prop {Props} subContainer2 Sub Container placed on Right side, with other half
 *
 */
export interface ValueInfoProps {
  mainContainer: Props;
  subContainer1?: Props;
  subContainer2?: Props;
}

/**
 * @typedef Props
 *
 * Props interface for the showing information.
 *
 * @prop {SelectorType: number} selector Redux Selector
 * @prop {string} label Value label
 * @prop {string} stateKey Unique identifier
 * @prop {string} units Unit measurement to display
 * @prop {boolean} isMain Config to know if its main or sub container (if true occupies the entire space)
 * @prop {boolean} showLimits Config to show Alarm limts in container (on top left - small size)
 * @prop {number} decimal Number of Decimals on the value
 *
 */
export interface Props {
  selector: SelectorType;
  label: string;
  stateKey: string;
  alarmLimits?: Range | null;
  units?: string;
  isLarge?: boolean;
  isMain?: boolean;
  showLimits?: boolean;
  decimal?: number;
}

/**
 * ClickHandler
 *
 * @component Component for manually dispatching Click & Double Click event based on timeout
 *
 * @prop {function} singleClickAction Callback on Single click
 * @prop {function} doubleClickAction Callback on Double click
 *
 * @returns {function}
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
 * @returns {JSX.Element}
 *
 */
const ControlValuesDisplay = ({
  selector,
  alarmLimits,
  label,
  stateKey,
  units = '',
  isMain = false,
  isLarge = false,
  showLimits = false,
  decimal,
}: Props): JSX.Element => {
  const classes = useStyles();
  /**
   * State to toggle opening Alarm popup
   */
  const [open, setOpen] = useState(false);
  const alarmLimitsRequest = useSelector(getAlarmLimitsRequest, shallowEqual);
  const range =
    alarmLimitsRequest === null
      ? undefined
      : ((alarmLimitsRequest as unknown) as Record<string, Range>)[stateKey];
  const rangeValues = range === undefined ? { lower: '--', upper: '--' } : range;
  const { lower, upper } =
    alarmLimits && alarmLimits?.upper === 0
      ? rangeValues
      : { lower: alarmLimits?.lower, upper: alarmLimits?.upper };

  /**
   * Opens Multistep Popup on Clicking over component
   */
  const onClick = () => {
    // setOpen(true);
    if (stateKey) {
      setMultiPopupOpen(true, stateKey);
    }
  };

  /**
   * Disable double click events over component & dispatch onClick manually
   */
  const handleClick = ClickHandler(onClick, () => {
    return false;
  });

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
            className={isLarge ? classes.rootLarge : classes.root}
            style={isMain ? { width: '80%', margin: '0 auto' } : {}}
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
                <Typography
                  className={classes.whiteFont}
                  style={isLarge ? { fontSize: '2rem' } : { fontSize: '16px' }}
                >
                  {label}
                </Typography>
              </Grid>
              {showLimits && stateKey && (
                <Grid container item xs={3} className={classes.liveContainer}>
                  <Typography
                    className={classes.whiteFont}
                    style={isLarge ? { fontSize: '1.25rem' } : {}}
                  >
                    {lower}
                  </Typography>
                  <Typography
                    className={classes.whiteFont}
                    style={isLarge ? { fontSize: '1.25rem' } : {}}
                  >
                    {upper}
                  </Typography>
                </Grid>
              )}
            </Grid>
            <Grid item xs className={classes.displayContainer}>
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
                    style={isLarge ? { fontSize: '1.5rem' } : {}}
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
            updateModalStatus={(status: boolean) => setOpen(status)}
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
 * @component Component for showing information based on layout configured.
 *
 * Uses the [[ValueInfoProps]] interface
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
  isLarge = false,
  alarmLimits,
}: Props): JSX.Element => {
  const classes = useStyles();
  return (
    <Grid item xs container className={`${classes.valuesPanel} ${classes.mainContainer}`}>
      <Grid item xs className={classes.gridAreavalues1}>
        <ControlValuesDisplay
          isMain={true}
          stateKey={stateKey}
          selector={selector}
          label={label}
          units={units}
          alarmLimits={alarmLimits}
          showLimits={showLimits}
          decimal={decimal || 0}
          isLarge={isLarge}
        />
      </Grid>
    </Grid>
  );
};

export default ValueInfo;
