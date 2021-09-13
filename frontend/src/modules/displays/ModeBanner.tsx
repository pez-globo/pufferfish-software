/**
 * @summary Component to display Mode text in banner
 *
 */
import React from 'react';
import { Grid, Typography, makeStyles, Theme } from '@material-ui/core';
import { useSelector } from 'react-redux';
import { getParametersRequestMode } from '../../store/controller/selectors/measurements';
import { VentilationMode } from '../../store/proto/mcu_pb';

const useStyles = makeStyles((theme: Theme) => ({
  normal: {
    color: theme.palette.background.paper,
  },
  screenSaver: {
    color: theme.palette.primary.contrastText,
  },
}));

/**
 * Get display-text for the selected ventilation mode
 *
 * @param {VentilationMode | null} mode - description for mode
 *
 * @returns {string} - corresponding text for the selected mode
 */
export const getModeText = (mode: VentilationMode | null): string => {
  switch (mode) {
    case VentilationMode.hfnc:
      return 'High Flow Nasal Cannula';
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

export enum BannerType {
  NORMAL,
  SCREENSAVER,
}

/**
 * @typedef Props
 *
 * Props Interface for the Mode Banner component
 *
 * @prop {string} bannerType banner type for deciding font color
 */
export interface Props {
  bannerType: BannerType;
}

/**
 * ModeBanner
 *
 * @component A container for displaying the current mode in a large font banner.
 *
 * Uses the [[Props]] interface
 *
 * TODO: Hook up the `mode` prop to the redux store so we can display
 *       the current mode below.
 *
 * @returns {JSX.Element}
 *
 */
export const ModeBanner = ({ bannerType }: Props): JSX.Element => {
  const classes = useStyles();
  const mode = useSelector(getParametersRequestMode);

  /**
   * Function to configure color of the banner text
   *
   * @param {string} bannerType banner type for deciding font color
   *
   * @returns CSS class
   */
  function color(bannerType: BannerType) {
    if (bannerType === BannerType.SCREENSAVER) return classes.screenSaver;
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
