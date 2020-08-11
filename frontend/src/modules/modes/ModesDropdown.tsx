import React from 'react'
import { Button, Menu, MenuItem, makeStyles, Theme } from '@material-ui/core'

const useStyles = makeStyles((theme: Theme) => ({
    button: {
        minWidth: 245
    },
}))

const modes = [
    'Pressure Control (AC)',
    'Pressure Control (SIMV)',
    'Volume Control (AC)',
    'Volume Control (SIMV)',
    'Non-Invasive (NIV)'
]

/**
 * ModesDropdown
 * 
 * TODO: This component should rely on a redux state that stores the current
 *       mode as it has application-wide consequences. Updating that redux 
 *       state should be triggered by a dispatcher in the `handleItemClick` 
 *       handler.
 */
export const ModesDropdown = () => {
    const classes = useStyles()
    const [anchorEl, setAnchorEl] = React.useState<null | HTMLElement>(null)
    const [selectedIndex, setSelectedIndex] = React.useState(0)

    const handleClick = (event: React.MouseEvent<HTMLButtonElement>) => {
        setAnchorEl(event.currentTarget)
    }

    const handleItemClick = (
        _event: React.MouseEvent<HTMLElement, MouseEvent>,
        index: number
    ) => {
        setSelectedIndex(index)
        handleClose()
    }

    const handleClose = () => {
        setAnchorEl(null)
    }

    return (
        <div>
            <Button onClick={handleClick} variant='contained' color='primary' className={classes.button}>
                {modes[selectedIndex]}
            </Button>
            <Menu
                id='simple-menu'
                anchorEl={anchorEl}
                keepMounted
                open={Boolean(anchorEl)}
                onClose={handleClose}
            >
                {modes.map((mode, index) => (
                    <MenuItem
                        key={mode}
                        selected={index === selectedIndex}
                        onClick={(event) => handleItemClick(event, index)}
                    >
                        {mode}
                    </MenuItem>
                ))}
            </Menu>
        </div>
    )
}

export default ModesDropdown