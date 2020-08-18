import React from 'react'
import { Button, Menu, MenuItem} from '@material-ui/core'

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
 *       state should be triggered by the `handleItemClick` handler.
 */
export const ModesDropdown = () => {
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
            <Button
                color='primary'
                disableElevation
                style={{ color:'#FFF', width: 256 }}
                aria-haspopup='true'
                onClick={handleClick}
            >
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