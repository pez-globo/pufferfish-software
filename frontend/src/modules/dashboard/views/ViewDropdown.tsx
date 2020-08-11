import React from 'react'
import { Button, Menu, MenuItem } from '@material-ui/core'

const views = ['Main View', 'Data View', 'Waveform View']

/**
 * ViewDropdown 
 * 
 * TODO: This component may rely on a redux state that stores the current
 *       view.
 */
export const ViewDropdown = () => {
    const [anchorEl, setAnchorEl] = React.useState<null | HTMLElement>(null)
    const [selectedIndex, setSelectedIndex] = React.useState(0)

    const handleClick = (event: React.MouseEvent<HTMLButtonElement>) => {
        setAnchorEl(event.currentTarget)
    }

    const handleItemClick = (
        event: React.MouseEvent<HTMLElement, MouseEvent>,
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
                variant='contained'
                style={{ minWidth: 160 }}
                onClick={handleClick}
            >
                {views[selectedIndex]}
            </Button>
            <Menu
                id='simple-menu'
                anchorEl={anchorEl}
                keepMounted
                open={Boolean(anchorEl)}
                onClose={handleClose}
            >
                {views.map((view, index) => (
                    <MenuItem
                        key={view}
                        selected={index === selectedIndex}
                        onClick={(event) => handleItemClick(event, index)}
                    >
                        {view}
                    </MenuItem>
                ))}
            </Menu>
        </div>
    )
}

export default ViewDropdown