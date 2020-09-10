import React from 'react'
import { Grid, makeStyles, Theme } from '@material-ui/core'
import PressureControlMainView from './views/PressureControlMainView'

const useStyles = makeStyles((theme: Theme) => ({
  root: {
    flexGrow: 1,
    justifyContent: 'center',
    alignItems: 'stretch',
    marginBottom: theme.spacing(2)
  },
}))

/**
 * DashboardPage
 * 
 * TODO: Hook into the redux store to get the current dashboard `view` from the
 * `ViewDropdown` 
 */
export const DashboardPage = () => {
  const classes = useStyles()

  return (
    <Grid container className={classes.root}>
      {/* TODO: Switch views based on `view` state. */}
      <PressureControlMainView />
    </Grid>
  )
}

export default DashboardPage