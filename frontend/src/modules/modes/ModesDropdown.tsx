/**
 * @summary Component for display Ventilation mode dropdown
 *
 */
import { Button, makeStyles, Menu, MenuItem, MenuProps, Theme } from '@material-ui/core';
import ListItemIcon from '@material-ui/core/ListItemIcon';
import { withStyles } from '@material-ui/core/styles';
import ArrowDropDown from '@material-ui/icons/ArrowDropDown';
import FiberManualRecord from '@material-ui/icons/FiberManualRecord';
import React from 'react';
import { useDispatch, useSelector } from 'react-redux';
import { VentilationMode, ParametersRequest } from '../../store/controller/proto/mcu_pb';
import { MessageType } from '../../store/controller/types';
import { commitRequest } from '../../store/controller/actions';
import { getParametersRequestMode } from '../../store/controller/selectors';
import { getModeText } from '../displays/ModeBanner';

const useStyles = makeStyles(() => ({
  button: {
    minWidth: 245,
  },
  text: {
    marginRight: -25,
  },
  arrow: {
    marginLeft: 10,
    marginRight: -50,
    color: 'white',
    display: 'none',
  },
  alertColor: {
    backgroundColor: '#FF0000',
    color: '#fff',
    '&:hover': {
      backgroundColor: '#FF0000',
    },
  },
}));

const modes = [
  VentilationMode.hfnc,
  VentilationMode.pc_ac,
  VentilationMode.vc_ac,
  VentilationMode.niv_pc,
  VentilationMode.niv_ps,
  VentilationMode.psv,
];

const StyledMenu = withStyles((theme: Theme) => ({
  paper: {
    backgroundColor: theme.palette.primary.main,
    marginTop: '5px',
    minWidth: 245,
  },
}))((props: MenuProps) => (
  <Menu
    elevation={0}
    getContentAnchorEl={null}
    anchorOrigin={{
      vertical: 'bottom',
      horizontal: 'center',
    }}
    transformOrigin={{
      vertical: 'top',
      horizontal: 'center',
    }}
    {...props}
  />
));

const StyledMenuItem = withStyles((theme) => ({
  root: {
    // borderTop: '1px solid black',

    '& .MuiListItemIcon-root, & .MuiListItemText-primary': {
      color: theme.palette.primary.main,
    },
    color: 'white',
    '&:focus': {
      '& .MuiListItemIcon-root, & .MuiListItemText-primary': {
        color: theme.palette.secondary.main,
      },
    },
  },
}))(MenuItem);

/**
 * ModesDropdown
 *
 * @component some description
 *
 * TODO: This component should rely on a redux state that stores the current
 *       mode as it has application-wide consequences. Updating that redux
 *       state should be triggered by a dispatcher in the `handleItemClick`
 *       handler.
 *
 * @returns {JSX.Element}
 *
 */
export const ModesDropdown = (): JSX.Element => {
  const classes = useStyles();
   /** 
   * State to manage HTML element of dropdown anchor
   */
  const [anchorEl, setAnchorEl] = React.useState<null | HTMLElement>(null);
  const dispatch = useDispatch();
  const ventilationMode = useSelector(getParametersRequestMode);

  /**
   * Update Ventilation mode in Redux store
   *
   * @param {VentilationMode} mode Ventilation mode
   *
   */
  const updateMode = (mode: VentilationMode) =>
    dispatch(
      commitRequest<ParametersRequest>(MessageType.ParametersRequest, { mode }),
    );

  /**
   * Function to open dropdown selection
   *
   * @param {React.MouseEvent<HTMLElement} event Mouse Event
   *
   */
  const handleClick = (event: React.MouseEvent<HTMLButtonElement>) => {
    setAnchorEl(event.currentTarget);
  };

  /**
   * Function for handling dropdown item event.
   *
   * @param {React.MouseEvent<HTMLElement} event Mouse Event
   * @param {number} index index of dropdown
   *
   */
  const handleItemClick = (
    _event: React.MouseEvent<HTMLElement, MouseEvent>,
    value: VentilationMode,
  ) => {
    updateMode(value);
    handleClose();
  };

  /**
   * Function for handling dropdown close
   */
  const handleClose = () => {
    setAnchorEl(null);
  };

  /**
   * Function for adding border using css property
   *
   * @param {number} view Index of Dropdown value
   *
   * @returns {object}
   *
   */
  function addBorder(mode: number) {
    if (mode > 0) return { borderTop: '1px solid black' };
    return {};
  }

  return (
    <div style={{ pointerEvents: 'none' }}>
      <Button onClick={handleClick} variant="contained" color="primary" className={classes.button}>
        {getModeText(ventilationMode)}
        <ListItemIcon className={classes.arrow}>
          <ArrowDropDown fontSize="default" />
        </ListItemIcon>
      </Button>
      <StyledMenu
        id="simple-menu"
        anchorEl={anchorEl}
        keepMounted
        open={Boolean(anchorEl)}
        onClose={handleClose}
      >
        {modes.map((mode) => (
          <StyledMenuItem
            key={mode}
            selected={mode === ventilationMode}
            onClick={(event) => handleItemClick(event, mode)}
            style={addBorder(mode)}
          >
            <ListItemIcon className={classes.text}>
              <FiberManualRecord fontSize="inherit" />
            </ListItemIcon>
            {getModeText(mode)}
          </StyledMenuItem>
        ))}
      </StyledMenu>
    </div>
  );
};

export default ModesDropdown;
