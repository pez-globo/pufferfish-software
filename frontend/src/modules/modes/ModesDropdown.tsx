import { Button, makeStyles, Menu, MenuItem, MenuProps, Theme } from '@material-ui/core';
import ListItemIcon from '@material-ui/core/ListItemIcon';
import { withStyles } from '@material-ui/core/styles';
import ArrowDropDown from '@material-ui/icons/ArrowDropDown';
import FiberManualRecord from '@material-ui/icons/FiberManualRecord';
import React from 'react';
import { useDispatch, useSelector } from 'react-redux';
import { updateCommittedParameter } from '../../store/controller/actions';
import { VentilationMode } from '../../store/controller/proto/mcu_pb';
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
  VentilationMode.pc_ac,
  VentilationMode.pc_simv,
  VentilationMode.vc_ac,
  VentilationMode.vc_simv,
  VentilationMode.niv,
  VentilationMode.hfnc,
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
 * TODO: This component should rely on a redux state that stores the current
 *       mode as it has application-wide consequences. Updating that redux
 *       state should be triggered by a dispatcher in the `handleItemClick`
 *       handler.
 */
export const ModesDropdown = (): JSX.Element => {
  const classes = useStyles();
  const [anchorEl, setAnchorEl] = React.useState<null | HTMLElement>(null);
  const dispatch = useDispatch();
  const ventilationMode = useSelector(getParametersRequestMode);
  const updateMode = (mode: VentilationMode) => dispatch(updateCommittedParameter({ mode }));

  const handleClick = (event: React.MouseEvent<HTMLButtonElement>) => {
    setAnchorEl(event.currentTarget);
  };

  const handleItemClick = (
    _event: React.MouseEvent<HTMLElement, MouseEvent>,
    value: VentilationMode,
  ) => {
    updateMode(value);
    handleClose();
  };

  const handleClose = () => {
    setAnchorEl(null);
  };

  function addBorder(mode: number) {
    if (mode > 0) return { borderTop: '1px solid black' };
    return {};
  }

  return (
    <div>
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
