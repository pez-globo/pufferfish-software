import React from 'react'
import { Grid, IconButton, ButtonGroup } from '@material-ui/core'
import AddIcon from '@material-ui/icons/Add'
import RemoveIcon from '@material-ui/icons/Remove'

interface Props {
    onAction: (value: any) => void
}

export const ValueController = ({ onAction }: Props) => {

    const handleAction = (step: number) => () => {
        onAction(step)
    }

    return (
        <Grid container item>
            <ButtonGroup orientation='horizontal'>
                <IconButton onClick={() => onAction(10)}>
                    <RemoveIcon /><RemoveIcon />
                </IconButton>
                <IconButton onClick={handleAction(1)}>
                    <RemoveIcon />
                </IconButton>
                <IconButton onClick={handleAction(1)}>
                    <AddIcon />
                </IconButton>
                <IconButton onClick={handleAction(10)}>
                    <AddIcon /><AddIcon />
                </IconButton>
            </ButtonGroup>
        </Grid>
    )
}

export default ValueController