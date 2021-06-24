/**
 * @summary UI wrapper for Tabs
 * wrapper with minimal configuration, to be used instead of Material UI's TabPanel
 *
 */
import Grid from '@material-ui/core/Grid';
import { makeStyles } from '@material-ui/core/styles';
import React from 'react';

const useStyles = makeStyles(() => ({
  root: {
    // flexGrow: 1,
    width: '100%',
    height: '100%',
  },
}));

/**
 * @typedef Props
 *
 * Props interface for the tab panel component
 *
 * @prop {React.ReactNode} children Content loaded inside the tab
 * @prop {number} index Tab index
 * @prop {number} value Current tab index value
 */
interface Props {
  children?: React.ReactNode;
  index: number;
  value: number;
}

/**
 * Add Unique Id to DOM element of `Tab` component
 *
 * @param {number} index - Tab index
 *
 * @returns {Record<string, unknown>} Attributes object with id
 *
 */
export const a11yProps = (index: number): Record<string, unknown> => {
  return {
    id: `tab-${index}`,
    'aria-controls': `tabpanel-${index}`,
  };
};

/**
 * TabPanel
 *
 * @component Component wrapper for Tabs
 *
 * Uses the [[Props]] interface
 *
 * @returns JSX.Element
 */
export const TabPanel = (props: Props): JSX.Element => {
  const { children = null, value, index, ...other } = props;
  const classes = useStyles();

  return (
    <div
      role="tabpanel"
      hidden={value !== index}
      id={`tabpanel-${index}`}
      aria-labelledby={`tab-${index}`}
      {...other}
      className={classes.root}
    >
      {value === index && (
        <Grid
          container
          direction="row"
          alignItems="stretch"
          wrap="nowrap"
          style={{ height: '100%' }}
        >
          {children}
        </Grid>
      )}
    </div>
  );
};

export default TabPanel;
