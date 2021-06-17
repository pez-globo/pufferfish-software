/**
 * @summary Re-usable UI component for Toggle based Input Field
 * currently used in Settings > Display > Theme color & Unit field
 *
 */
import { Grid, makeStyles, Theme, withStyles } from '@material-ui/core';
import ToggleButton from '@material-ui/lab/ToggleButton';
import ToggleButtonGroup from '@material-ui/lab/ToggleButtonGroup';
import React from 'react';

const useStyles = makeStyles((theme: Theme) => ({
  toggleButton: {
    backgroundColor: theme.palette.primary.main,
    marginRight: theme.spacing(2),
    borderRadius: 5,
    height: '40px',
  },
  toggleButtonGroup: {
    width: '100%',
    height: '40px',
  },
  button: {
    margin: theme.spacing(0.6, 1),
    border: 'none',
    flex: 1,
    borderTopRightRadius: 5,
    borderBottomRightRadius: 0,
    borderRight: 5,
  },
}));

const StyledToggleButton = withStyles(({ palette }) => ({
  root: {
    padding: 0,
  },
  selected: {
    '&&': {
      backgroundColor: palette.background.paper,
      borderRadius: 5,
    },
    '&&:hover': {
      backgroundColor: palette.background.paper,
    },
    '& .MuiToggleButton-label': {
      color: palette.text.primary,
    },
  },
  label: {
    color: '#fff',
  },
}))(ToggleButton);

/**
 * @typedef Props
 *
 * Props Interface for the ToggleValue component
 *
 * @prop {string} toggleBetween.label Toggle Input label
 * @prop {number} toggleBetween.value Toggle Input value (positive & !null)
 * @prop {number} selected Selected toggle value
 * @prop {function} onToggle Callback on Toggling between values
 */
interface Props {
  toggleBetween: { label: string; value: number }[];
  selected: number;
  onToggle(value: number): void;
}

/**
 * ToggleValue
 *
 * @component A re-usable component for toggling between values.
 *
 * Uses the [[Props]] interface
 *
 * @returns {JSX.Element}
 */
export const ToggleValue = ({ toggleBetween, selected, onToggle }: Props): JSX.Element => {
  const classes = useStyles();
  const [toggleValue, setToggleValue] = React.useState(selected);

  /**
   * Function for handling the alignment
   *
   * @param {React.MouseEvent<HTMLElement>} event - DOM Mouse event
   * @param {React.SetStateAction<number>} newToggleValue - New toggle value to update
   */
  const handleAlignment = (
    event: React.MouseEvent<HTMLElement>,
    newToggleValue: React.SetStateAction<number>,
  ) => {
    if (newToggleValue !== null) {
      setToggleValue(newToggleValue);
      onToggle(newToggleValue as number);
    }
  };

  return (
    <Grid className={classes.toggleButton}>
      <ToggleButtonGroup
        className={classes.toggleButtonGroup}
        value={toggleValue}
        exclusive
        onChange={handleAlignment}
        aria-label="Toggle"
      >
        <StyledToggleButton
          value={toggleBetween[0].value}
          aria-label="aligned"
          className={classes.button}
        >
          {toggleBetween[0].label}
        </StyledToggleButton>
        <StyledToggleButton
          value={toggleBetween[1].value}
          aria-label="centered"
          className={classes.button}
        >
          {toggleBetween[1].label}
        </StyledToggleButton>
      </ToggleButtonGroup>
    </Grid>
  );
};

export default ToggleValue;
