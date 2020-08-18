import React from 'react'
import Grid from '@material-ui/core/Grid'
import Typography from '@material-ui/core/Typography'

export interface Props {
  value: number,
}

export const SimpleValueDisplay = ({ value }: Props) => (
  <Grid item container direction='column' alignItems='center' justify='center'>
    <Typography variant='h2'>{value !== undefined ? value.toFixed(2) : '--'}</Typography>
  </Grid>
)

export default SimpleValueDisplay