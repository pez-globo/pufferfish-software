import React from 'react';
import { Grid, Typography, makeStyles, Theme } from '@material-ui/core';
import { useSelector } from 'react-redux';
import { getParametersRequestMode } from '../../store/controller/selectors';
import { VentilationMode } from '../../store/controller/proto/mcu_pb';

const useStyles = makeStyles((theme: Theme) => ({
  normal: {
    color: theme.palette.background.paper,
  },
  screenSaver: {
    color: theme.palette.primary.contrastText,
  },
}));

export const getModeText = (mode: VentilationMode): string => {
  switch (mode) {
    case VentilationMode.hfnc:
      return 'High Flow Nasal Cannula (HFNC)';
    case VentilationMode.pc_ac:
      return 'Pressure Control (A/C)';
    case VentilationMode.vc_ac:
      return 'Volume Control (A/C)';
    case VentilationMode.niv_pc:
      return 'Non-Invasive (PC)';
    case VentilationMode.niv_ps:
      return 'Non-Invasive (PS)';
    case VentilationMode.psv:
      return 'Pressure Support Ventilation';
    default:
      return 'No Mode Selected';
  }
};

export interface Props {
  bannerType: string;
}

/**
 * ModeBanner
 *
 * A contianer for displaying the current mode in a large font banner.
 *
 * TODO: Hook up the `mode` prop to the redux store so we can display
 *       the current mode below.
 */
export const ModeBanner = ({ bannerType }: Props): JSX.Element => {
  const classes = useStyles();
  const mode = useSelector(getParametersRequestMode);
  function color(bannerType: string) {
    if (bannerType === 'screenSaver') return classes.screenSaver;
    return classes.normal;
  }
  return (
    <Grid container justify="center" alignItems="center">
      <Grid item>
        <Typography align="center" variant="h3" className={color(bannerType)}>
          {getModeText(mode)}
        </Typography>
      </Grid>
    </Grid>
  );
};

export default ModeBanner;
