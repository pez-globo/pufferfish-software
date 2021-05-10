/**
 * @summary A short one-line description for the file
 *
 * @file More detailed description for the file, if necessary;
 * perhaps spanning multiple lines.
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
 * @prop {React.ReactNode} children desc for children
 * @prop {number} index desc for index
 * @prop {number} value desc for value
 */
interface Props {
  children?: React.ReactNode;
  index: number;
  value: number;
}

/**
 * @param {number} index - desc for index
 *
 * @returns {Record<string, unknown>} description for return value
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
 * @component Some desc for tab panel component
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
